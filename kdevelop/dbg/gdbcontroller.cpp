// *************************************************************************
//                          gdbcontroller.cpp  -  description
//                             -------------------
//    begin                : Sun Aug 8 1999
//    copyright            : (C) 1999 by John Birch
//    email                : jb.nz@writeme.com
// **************************************************************************
//
// **************************************************************************
// *                                                                        *
// *   This program is free software; you can redistribute it and/or modify *
// *   it under the terms of the GNU General Public License as published by *
// *   the Free Software Foundation; either version 2 of the License, or    *
// *   (at your option) any later version.                                  *
// *                                                                        *
// **************************************************************************

#include "gdbcontroller.h"

#include "breakpoint.h"
#include "framestack.h"
#include "gdbcommand.h"
#include "stty.h"
#include "vartree.h"

#include <kapp.h>
#include <kprocess.h>

#include <qstring.h>

#include <iostream>
#include <ctype.h>
#include <stdlib.h>

#if defined(DBG_MONITOR)
  #define GDB_MONITOR
  #define DBG_DISPLAY(X)          {emit rawData(X);}
#else
  #define DBG_DISPLAY(X)          {;}
#endif

#if defined(GDB_MONITOR)
  #define GDB_DISPLAY(X)          {emit rawData(X);}
#else
  #define GDB_DISPLAY(X)          {;}
#endif

// **************************************************************************
//
// Does all the communication between gdb and the kdevelop's debugger code.
// Significatant classes being used here are
//
// GDBParser  - parses the "variable" data using the vartree and varitems
// VarTree    - where the variable data will end up
// FrameStack - tracks the program frames and allows the user to switch between
//              and therefore view the calling funtions and their data
// Breakpoint - Where and what to do with breakpoints.
// STTY       - the tty that the _application_ will run on.
//
// Significant variables
// state_     - be very careful setting this. The controller is totally
//              dependent on this refelecting the correct state. For instance,
//              if he app is busy but we don't think so then bad things will
//              happen. The only way to get out of these situations is to delete
//              the controller.
// currentFrame_
//            - Holds the frame number where and locals/variable information will
//              go to
//
// Certain commands need to be "wrapped", so that the output gdb produces is
// of the form "\032data_id gdb output \032data_id"
// Then a very simple parse can extract this gdb output and hand it off
// to its' respective parser.
// To do this we set the prompt to be \032data_id before the command and then
// reset to \032i to indicate the "idle".
//
// Note that the following does not work because in certain situations
// gdb can get an error in performing the command and therefore will not
// output the final echo. Hence the data will be thrown away.
// (certain "info locals" will generate this error.
//
//  queueCmd(new GDBCommand(QString().sprintf("define printlocal\n"
//                                              "echo \32%c\ninfo locals\necho \32%c\n"
//                                              "end",
//                                              LOCALS, LOCALS)));
// although replacing echo with prompt appropriately will probably work Hmmmm.
//
// **************************************************************************

GDBController::GDBController(VarTree* varTree, FrameStack* frameStack) :
  DbgController(),
  frameStack_(frameStack),
  varTree_(varTree),
  currentFrame_(0),
  state_(s_dbgNotStarted|s_appNotStarted),
  gdbSizeofBuf_(2048),
  gdbOutputLen_(0),
  gdbOutput_(new char[2048]),
  currentCmd_(0),
  tty_(0),
  programHasExited_(false),
  config_breakOnLoadingLibrary_(true),
  config_forceBPSet_(true),
  config_displayStaticMembers_(false),
  config_asmDemangle_(true)
{
  KConfig* config = kapp->getConfig();
  config->setGroup("Debug");
  ASSERT(!config->readBoolEntry("Use external debugger", false));

  config_displayStaticMembers_  = config->readBoolEntry("Display static members", false);
  config_asmDemangle_           = !config->readBoolEntry("Display mangled names", true);
  config_breakOnLoadingLibrary_ = config->readBoolEntry("Break on loading libs", true);
  config_forceBPSet_            = config->readBoolEntry("Allow forced BP set", true);

#if defined (GDB_MONITOR)
    connect(  this,   SIGNAL(dbgStatus(const QString&, int)),
                      SLOT(slotDbgStatus(const QString&, int)));
#endif
#if defined (DBG_MONITOR)
    connect(  this,   SIGNAL(showStepInSource(const QString&, int)),
                      SLOT(slotStepInSource(const QString&,int)));
#endif
}

// **************************************************************************

GDBController::~GDBController()
{
  destroyCmds();

  if (dbgProcess_)
  {
    setStateOn(s_waitForWrite);
    emit dbgStatus ("", state_);

    pauseApp();
    dbgProcess_->writeStdin("quit", 4);

    int waitFor(20);
    while (waitFor--)
    {
      if (!stateIsOn(s_waitForWrite))
        break;
      kapp->processEvents(250);
   }
    dbgProcess_->kill(SIGKILL);
  }

  delete tty_;
  emit dbgStatus ("Debugger stopped", (s_dbgNotStarted|s_appNotStarted));
}

// **************************************************************************

void GDBController::reConfig()
{
  KConfig* config = kapp->getConfig();
  config->setGroup("Debug");
  ASSERT(!config->readBoolEntry("Use external debugger", false));

  bool old_displayStatic = config_displayStaticMembers_;
  config_displayStaticMembers_ = config->readBoolEntry("Display static members", false);

  bool old_asmDemangle = config_asmDemangle_;
  config_asmDemangle_ = !config->readBoolEntry("Display mangled names", true);

  bool old_breakOnLoadingLibrary_ = config_breakOnLoadingLibrary_;
  config_breakOnLoadingLibrary_ = config->readBoolEntry("Break on loading libs", true);

  if (( old_displayStatic           != config_displayStaticMembers_   ||
        old_asmDemangle             != config_asmDemangle_            ||
        old_breakOnLoadingLibrary_  != config_breakOnLoadingLibrary_ )&&
        dbgProcess_)
  {
    if (stateIsOn(s_appBusy))
    {
      setStateOn(s_silentBreakInto);
      pauseApp();
    }

    if (old_displayStatic != config_displayStaticMembers_)
    {
      if (config_displayStaticMembers_)
        queueCmd(new GDBCommand("set print static-members on", NOTRUNCMD, NOTINFOCMD));
      else
        queueCmd(new GDBCommand("set print static-members off", NOTRUNCMD, NOTINFOCMD));
    }
    if (old_asmDemangle != config_asmDemangle_)
    {
      if (config_asmDemangle_)
        queueCmd(new GDBCommand("set print asm-demangle on", NOTRUNCMD, NOTINFOCMD));
      else
        queueCmd(new GDBCommand("set print asm-demangle off", NOTRUNCMD, NOTINFOCMD));
      // Testing -TODO REMOVE
      queueCmd(new GDBCommand("continue", RUNCMD, NOTINFOCMD));
      queueCmd(new GDBCommand("continue", RUNCMD, NOTINFOCMD));
    }

    if (old_breakOnLoadingLibrary_ != config_breakOnLoadingLibrary_)
    {
      if (config_breakOnLoadingLibrary_)
        queueCmd(new GDBCommand("set stop-on 1", NOTRUNCMD, NOTINFOCMD));
      else
        queueCmd(new GDBCommand("set stop-on 0", NOTRUNCMD, NOTINFOCMD));
    }

    if (stateIsOn(s_silentBreakInto))
      queueCmd(new GDBCommand("continue", RUNCMD, NOTINFOCMD));
  }
}

// **************************************************************************

void GDBController::slotStart(const QString& application, const QString& args)
{
  ASSERT (!dbgProcess_ && !tty_);
  tty_ = new STTY();
  connect( tty_, SIGNAL(OutOutput( const char* )), SIGNAL(ttyStdout( const char* )) );
  connect( tty_, SIGNAL(ErrOutput( const char* )), SIGNAL(ttyStderr( const char* )) );

  DBG_DISPLAY("Starting GDB");
  dbgProcess_ = new KProcess;

  connect(  dbgProcess_,  SIGNAL(receivedStdout(KProcess *, char *, int)),
            this,         SLOT(slotDbgStdout(KProcess *, char *, int)));

  connect(  dbgProcess_,  SIGNAL(receivedStderr(KProcess *, char *, int)),
            this,         SLOT(slotDbgStdout(KProcess *, char *, int)));

  connect(  dbgProcess_,  SIGNAL(wroteStdin(KProcess *)),
            this,         SLOT(slotDbgWroteStdin(KProcess *)));

  connect(  dbgProcess_,  SIGNAL(processExited(KProcess*)),
            this,         SLOT(slotDbgProcessExited(KProcess*)));

  // fires up gdb under the process
  *dbgProcess_<<"gdb"<<"-fullname"<<"-nx"<<"-quiet";

  dbgProcess_->start( KProcess::NotifyOnExit,
                      KProcess::Communication(KProcess::All));

  setStateOff(s_dbgNotStarted);
  emit dbgStatus ("", state_);

  // Initialise gdb. At this stage gdb is sitting wondering what to do,
  // and to whom. Organise a few things, then set up the tty for the application,
  // and the application itself
  queueCmd(new GDBCommand(QString().sprintf("set prompt \32%c", IDLE), NOTRUNCMD, NOTINFOCMD));
  queueCmd(new GDBCommand("set confirm off", NOTRUNCMD, NOTINFOCMD));

  if (config_displayStaticMembers_)
    queueCmd(new GDBCommand("set print static-members on", NOTRUNCMD, NOTINFOCMD));
  else
    queueCmd(new GDBCommand("set print static-members off", NOTRUNCMD, NOTINFOCMD));

  queueCmd(new GDBCommand(QString().sprintf("tty %s", (tty_->getMainTTY()).data()), NOTRUNCMD, NOTINFOCMD));

  if (!args.isEmpty())
    queueCmd(new GDBCommand(QString().sprintf("set args %s", args.data()), NOTRUNCMD, NOTINFOCMD));

  queueCmd(new GDBCommand(QString().sprintf("file %s", application.data()), NOTRUNCMD, NOTINFOCMD));

  // This makes gdb pump a variable out on one line.
  queueCmd(new GDBCommand("set width 0", NOTRUNCMD, NOTINFOCMD));
  queueCmd(new GDBCommand("set height 0", NOTRUNCMD, NOTINFOCMD));

  // Get gdb to notify us of shared library events. This allows us to
  // set breakpoints in shared libraries, that the user has set previously.
  // The 1 doesn't mean anything specific, just any non-zero value to
  // satisfy gdb!
  // An alternative to this would be catch load, catch unload, but they don't work!
  if (config_breakOnLoadingLibrary_)
    queueCmd(new GDBCommand("set stop-on 1", NOTRUNCMD, NOTINFOCMD));

  // Print some nicer names in disassembly output. Although for an assembler
  // person this may actually be wrong and the mangled name could be better.
  if (config_asmDemangle_)
    queueCmd(new GDBCommand("set print asm-demangle on", NOTRUNCMD, NOTINFOCMD));
  else
    queueCmd(new GDBCommand("set print asm-demangle off", NOTRUNCMD, NOTINFOCMD));


  // Organise any breakpoints.
  emit acceptPendingBPs();

  // Now gdb has been started and the application has been loaded,
  // BUT the app hasn't been started yet! A run command is about to be issued
  // by whoever is controlling us.
}

// **************************************************************************

// Fairly obvious that we'll add whatever command you give me to a queue
// If you tell me to, I'll put it at the head of the queue so it'll run ASAP
// Not quite so obvious though is that if we are going to run again. then any
// information requests become redundent and must be rwmoved.
// We also try and run whatever command happens to be at the head of
// the queue.
void GDBController::queueCmd(DbgCommand* cmd, bool executeNext)
{
  // We remove any info command or _run_ command if we are about to
  // add a run command.
  if (cmd->isARunCmd())
    removeInfoRequests();

  if (executeNext)
    cmdList_.insert(0, cmd);
  else
    cmdList_.append (cmd);

  executeCmd();
}

// **************************************************************************

// If the appliction can accept a command and we've got one waiting
// then send it.
// Commands can be just request for data (or change gdbs state in someway)
// or they can be "run" commands. If a command is sent to gdb our internal
// state will get updated.
void GDBController::executeCmd()
{
  if (stateIsOn(s_dbgNotStarted|s_waitForWrite|s_appBusy))
    return;

  if (!currentCmd_)
  {
    if (cmdList_.isEmpty())
      return;

    currentCmd_ = cmdList_.take(0);
  }

  if (!currentCmd_->moreToSend())
  {
    if (currentCmd_->expectReply())
      return;

    delete currentCmd_;
    if (cmdList_.isEmpty())
    {
      currentCmd_ = 0;
      return;
    }

    currentCmd_ = cmdList_.take(0);
  }

  ASSERT(currentCmd_ && currentCmd_->moreToSend());

  QString cmdStr = currentCmd_->cmdToSend();
  dbgProcess_->writeStdin(cmdStr.data(), cmdStr.length());
  setStateOn(s_waitForWrite);

  if (currentCmd_->isARunCmd())
  {
    setStateOn(s_appBusy);
    setStateOff(s_appNotStarted|s_programExited|s_silentBreakInto);
  }

  GDB_DISPLAY("[gdb]<< "+cmdStr);
  emit dbgStatus ("", state_);
}

// **************************************************************************

void GDBController::destroyCmds()
{
  if (currentCmd_)
	{
	  delete currentCmd_;
		currentCmd_ = 0;
	}
	
	while (!cmdList_.isEmpty())
	  delete cmdList_.take(0);
}

// **********************************************************************

void GDBController::removeInfoRequests()
{
  int i = cmdList_.count();
	while (i)
	{
  	i--;
    DbgCommand* cmd = cmdList_.at(i);
	  if (cmd->isAnInfoCmd() || cmd->isARunCmd())
  	  delete cmdList_.take(i);
  }
}

// **********************************************************************

// Pausing an app removes any pending run commands so that the app doesn't
// start again. If we want to be silent then we remove any pending info
// commands as well.
void GDBController::pauseApp()
{
  int i = cmdList_.count();
	while (i)
	{
  	i--;
    DbgCommand* cmd = cmdList_.at(i);
    if ((stateIsOn(s_silentBreakInto) && cmd->isAnInfoCmd()) || cmd->isARunCmd())
  	  delete cmdList_.take(i);
  }

  if (dbgProcess_ && stateIsOn(s_appBusy))
    dbgProcess_->kill(SIGINT);
}

// **********************************************************************

// Whenever the program pauses we need to refresh the data visible to
// the user. The reason we've stooped may be passed in  to be emitted.
void GDBController::actOnProgramPause(const QString& msg)
{
  // We're only stopping if we were running, of course.
  if (stateIsOn(s_appBusy))
  {
    DBG_DISPLAY("Acting on program paused");
    setStateOff(s_appBusy);
    emit dbgStatus (msg, state_);

    // We're always at frame zero when the program stops
    // and we must reset the active flag
    currentFrame_ = 0;
    varTree_->setActiveFlag();

    // These two need to be actioned immediately. The order _is_ important
    queueCmd(new GDBCommand("backtrace", NOTRUNCMD, false, BACKTRACE), INFOCMD);
    if (stateIsOn(s_viewLocals))
      queueCmd(new GDBCommand("info local", NOTRUNCMD, INFOCMD, LOCALS));

    varTree_->findWatch()->requestWatchVars();
    varTree_->findWatch()->setActive();
    emit acceptPendingBPs();
  }
}

// **************************************************************************

enum lineStarts
{
  START_Brea  = 1634038338,
  START_Prog  = 1735357008,
  START_warn  = 1852989815,
  START_Cann  = 1852727619,
  START_Stop  = 1886352467,
  START__no_  = 544173608,
  START_curr  = 1920103779,
  START_Watc  = 1668571479,
};

// Any data that isn't "wrapped", arrives here. Rather than do multiple
// string searches until we find (or don't find!) the data,
// we break the data up, depending on the first 4 four bytes, treated as an
// int. Hence those big numbers you see above.
void GDBController::parseLine(char* buf)
{
  //int t=*(int*)(char*)"Watc";
  ASSERT(*buf != BLOCK_START);

  switch (*(int*)buf)
  {
    case START_Prog:
    {
      if ((strncmp(buf, "Program exited", 14) == 0) ||
          (strncmp(buf, "Program terminated", 18) == 0))
      {
        // The app has finished - but gdb is still running.
        DBG_DISPLAY("Parsed (exit) <" + QString(buf) + ">");
        state_ = (s_appNotStarted|s_programExited|(state_&s_viewLocals));
        destroyCmds();
	      emit dbgStatus (QString(buf), state_);
	
	      // TODO - a nasty switch - it'll not last long!!
		    programHasExited_ = true;
	      break;
      }

      if (strncmp(buf, "Program received signal", 23) == 0)
      {
        // SIGINT is a "break into running program".
        // We do this when the user set/mod/clears a breakpoint but the
        // application is running.
        // And the user does this to stop the program for their own
        // nefarious purposes.
        if (strstr(buf+23, "SIGINT") && stateIsOn(s_silentBreakInto))
          break;

        if (strstr(buf+23, "SIGSEGV"))
        {
          // Oh, shame, shame. The app has died a horrible death
          // Lets remove the pending commands and get the current
          // state organised for the user to figure out what went
          // wrong.
          // Note we're not quite dead yet...
          DBG_DISPLAY("Parsed (SIGSEGV) <" + QString(buf) + ">");
		      destroyCmds();
          actOnProgramPause(QString(buf));
          break;
		    }
      }

      // All "Program" strings cause a refresh of the program state
      actOnProgramPause(QString(buf));
      DBG_DISPLAY("Unparsed (START_Prog)<" + QString(buf) + ">");
      break;
    }

    case START_Cann:
    {
      // If you end the app and then restart when you have breakpoints set
      // in a dynamically loaded library, gdb will halt because the set
      // breakpoint is trying to access memory no longer used. The breakpoint
      // must first be deleted, however, we want to retain the breakpoint for
      // when the library gets loaded again.
      // TODO  programHasExited_ isn't always set correctly,
      // but it's (almost) doesn't matter.
      char* found=0;
      if (programHasExited_ && (found = strstr(buf, "Cannot insert breakpoint")))
      {
        setStateOff(s_appBusy);
        int BPNo = atoi(found+25);
        if (BPNo)
        {
          emit unableToSetBPNow(BPNo);
          queueCmd(new GDBCommand(QString().sprintf("delete %d", BPNo), NOTRUNCMD, NOTINFOCMD));
          queueCmd(new GDBCommand("info breakpoints", NOTRUNCMD, NOTINFOCMD, BPLIST));
          queueCmd(new GDBCommand("continue", RUNCMD, NOTINFOCMD));
        }
        break;
      }
    }

    case START_warn:
    {
      //warning: Hardware watchpoint 2: Could not insert watchpoint
// It seems redundent to single this out, as the default case
// is to show a message anyway
//      if (strncmp(buf, "warning: Hardware watchpoint", 28) == 0)
//      {
//	      emit dbgStatus (QString(buf), state_);
//	      break;
//      }

      actOnProgramPause(QString(buf));
//      setStateOff(s_appBusy);
//      emit dbgStatus (QString(buf), state_);
      DBG_DISPLAY("Unparsed (START_warn)<" + QString(buf) + ">");
      break;
    }

    // When the watchpoint variable goes out of scope the program stops
    // and tells you. (sometimes)
    case START_Watc:
    {
      if ((strncmp(buf, "Watchpoint", 10)==0) &&
          (strstr(buf, "deleted because the program has left the block")))
      {
        int BPNo = atoi(buf+11);
        if (BPNo)
        {
          queueCmd(new GDBCommand(QString().sprintf("delete %d", BPNo), NOTRUNCMD, NOTINFOCMD));
          queueCmd(new GDBCommand("info breakpoints", NOTRUNCMD, NOTINFOCMD, BPLIST));
        }
        else
          ASSERT(false);

        actOnProgramPause(QString(buf));
        break;
      }
      actOnProgramPause(QString(buf));
      DBG_DISPLAY("Unparsed (START_Watc)<" + QString(buf) + ">");
      break;
    }

    case START_Stop:
    {
      if (strncmp(buf, "Stopped due to shared library event", 35) == 0)
      {
        // When it's a library event, we try and set any pending
        // breakpoints, and that done, just continue onwards.
        setStateOff(s_appBusy);
        DBG_DISPLAY("Parsed (sh.lib) <" + QString(buf) + ">");
        emit acceptPendingBPs();
        queueCmd(new GDBCommand("continue", RUNCMD, NOTINFOCMD));
        break;
      }

      // A stop line means we've stopped. We're not really expecting one
      // of these unless it's a library event so just call actOnPause
      actOnProgramPause(QString(buf));
      DBG_DISPLAY("Unparsed (START_Stop)<" + QString(buf) + ">");
      break;
    }

    case START_curr:
    {
      // A watch point problem - show the line on status.
      // The default does the same anyway so no need to distinguish
      // between them.
//      if (strncmp(buf, "current stack frame not in method", 33) == 0)
//      {
//        actOnProgramPause(QString(buf));
//        break;
//      }

      DBG_DISPLAY("Unparsed (START_curr)<" + QString(buf) + ">");
      actOnProgramPause(QString(buf));
      break;
    }

    case START_Brea:
    {
      // Starts with "Brea" so assume "Breakpoint" and just get a full
      // breakpoint list. Note that the state is unchanged. This probably
      // isn't triggered?
      queueCmd(new GDBCommand("info breakpoints", NOTRUNCMD, NOTINFOCMD, BPLIST));

      DBG_DISPLAY("Parsed (BP) <" + QString(buf) + ">");
      break;
    }

    default:
    {
      if (strstr(buf, "No symbol table is loaded"))
      {
        actOnProgramPause(buf);
        break;
      }

      // The first "step into" into a source file that is missing
      // prints on stderr with a message that there's no source. Subsequent
      // "step into"s just print line number at filename. Both start with a
      // numeric char.
      if (isdigit(*buf))
      {
        DBG_DISPLAY("Parsed (digit)<" + QString(buf) + ">");
        parseProgramLocation(buf);
        break;
      }

      if (strstr(buf, "not meaningful in the outermost frame."))
      {
        DBG_DISPLAY("Parsed <" + QString(buf) + ">");
        actOnProgramPause(buf);
        break;
      }

      DBG_DISPLAY("Unparsed (default)<" + QString(buf) + ">");
      break;
    }
  }
}

// **************************************************************************

// The program location falls out of gdb, preceeded by \032\032. We treat
// it as a wrapped command (even though it doesn't have a trailing \032\032.
// The data gets parsed here and emitted in its component parts.
void GDBController::parseProgramLocation(char* buf)
{
  if (stateIsOn(s_silentBreakInto))
  {
    // It's a forced breakpoint stoppage. This means that the queue
    // will have a "continue" in it somewhere. The only action needed
    // is to reset the state so that queue'd items can be sent to gdb
    DBG_DISPLAY("Program location (but silentBreakInto) <" + QString(buf) + ">");
    setStateOff(s_appBusy);
    return;
  }

  char* bp_colon = strchr(buf, ':');
  if (bp_colon && strchr(bp_colon+2, ':'))    // make sure we avoid "::" problems
  {
    QString filename(buf, (bp_colon-buf)+1);
    int lineno = atoi(bp_colon+1);
    if (lineno)
    {
      actOnProgramPause(QString());
      emit showStepInSource(filename, lineno);
      return;
    }
  }

  actOnProgramPause("No source: "+QString(buf));
  emit showStepInSource("", -1);
}

// **************************************************************************

void GDBController::parseBacktraceList(char* buf)
{
  frameStack_->parseGDBBacktraceList(buf);

  varTree_->viewport()->setUpdatesEnabled(false);

  FrameRoot* frame;
  // The locals are always attached to the currentFrame
  // so make sure we have one of those.
  if (!(frame = varTree_->findFrame(currentFrame_)))
    frame = new FrameRoot(varTree_, currentFrame_);

  ASSERT(frame);

  frame->setFrameName(frameStack_->getFrameName(currentFrame_));

  // Add the frame params to the variable list
  frame->setParams(frameStack_->getFrameParams(currentFrame_));

  if (currentFrame_ == 0)
    varTree_->trimExcessFrames();

  varTree_->viewport()->setUpdatesEnabled(true);
  varTree_->repaint();
}

// **************************************************************************

// When a breakpoint has been set, gdb responds with some data about the
// new breakpoint. We just inform the breakpoint system about this.
void GDBController::parseBreakpointSet(char* buf)
{
  if (GDBSetBreakpointCommand* BPCmd = dynamic_cast<GDBSetBreakpointCommand*>(currentCmd_))
  {
    // ... except in this case :-) A -1 key tells us that this is
    // a special internal breakpoint, and we shouldn't do anything with it.
    // Currently there are _no_ internal breakpoints.
    if (BPCmd->getKey() != -1)
      emit rawGDBBreakpointSet(buf, BPCmd->getKey());
  }
}

// **************************************************************************

// Extra data needed by an item was requested. Here's the result.
void GDBController::parseRequestedData(char* buf)
{
  if (GDBItemCommand* gdbItemCommand = dynamic_cast<GDBItemCommand*> (currentCmd_))
  {
    // Fish out the item from the command and let it deal with the data
    VarItem* item = gdbItemCommand->getItem();
    item->updateValue(buf);
    item->trim();
  }
}

// **************************************************************************

// The user selects a different frame to view. We need to get and display
// where we are in the program source.
void GDBController::parseFrameSelected(char* buf)
{
  char lookup[3] = {BLOCK_START, SRC_POSITION, 0};
  if (char* start = strstr(buf, lookup))
  {
    if (char* end = strchr(start, '\n'))
    {
      *end = 0;      // clobber the new line
      parseProgramLocation(start+2);
      return;
    }
  }

  if (char* end = strchr(buf, '\n'))
    *end = 0;      // clobber the new line
  emit showStepInSource("", -1);
  emit dbgStatus ("No source: "+QString(buf), state_);
}

// **************************************************************************

// This is called when a completely new set of local data arrives. This data
// is always attached to (and completely updates) the current frame
// _All_ inactive items in the tree are trimmed here.
void GDBController::parseLocals(char* buf)
{
  varTree_->viewport()->setUpdatesEnabled(false);

  FrameRoot* frame;
  // The locals are always attached to the currentFrame
  // so make sure we have one of those.
  if (!(frame = varTree_->findFrame(currentFrame_)))
    frame = new FrameRoot(varTree_, currentFrame_);

  ASSERT(frame);

  frame->setFrameName(frameStack_->getFrameName(currentFrame_));

  // Frame data consists of the parameters of the calling function
  // and the local data.
  frame->setLocals(buf);

  // This is tricky - trim the whole tree when we're on the top most
  // frame so that they always see onlu "frame 0" on a program stop.
  // User selects frame 1, will show both frame 0 and frame 1.
  // Reselecting a frame 0 regenerates the data and therefore trims
  // the whole tree _but_ all the items in every frame will be active
  // so nothing will be deleted.
  if (currentFrame_ == 0)
    varTree_->trim();
  else
    frame->trim();

  varTree_->viewport()->setUpdatesEnabled(true);
  varTree_->repaint();
}

// **************************************************************************

// We are given a block of data that starts with \032. We now try to find a
// matching end block and if we can we shoot the data of to the appropriate
// parser for that type of data.
char* GDBController::parseCmdBlock(char* buf)
{
  ASSERT(*buf == BLOCK_START);

  char* end = 0;
  switch(*(buf+1))
  {
    case IDLE:
      // remove the idle tag because they often don't come in pairs
      return buf+1;

    case SRC_POSITION:
      // file and line number info that gdb just drops out starts with a
      // \32 but ends with a \n. Could treat this as a line rather than
      // a block. Ah well!
      if((end = strchr(buf, '\n')))
        *end = 0;      // Make a null terminated c-string
      break;

    default:
    {
      // match the start block with the end block if we can.
      char lookup[3] = {BLOCK_START, *(buf+1), 0};
      if ((end = strstr(buf+2, lookup)))
      {
        *end = 0;      // Make a null terminated c-string
        end++;         // The real end!
      }
      break;
    }
  }

  if (end)
  {
    char cmdType = *(buf+1);
    buf +=2;
    switch (cmdType)
    {
      case FRAME:           parseFrameSelected        (buf);      break;
      case SET_BREAKPT:     parseBreakpointSet        (buf);      break;
      case SRC_POSITION:    parseProgramLocation      (buf);      break;
      case LOCALS:          parseLocals               (buf);      break;
      case DATAREQUEST:     parseRequestedData        (buf);      break;
      case BPLIST:          emit rawGDBBreakpointList (buf);      break;
      case BACKTRACE:       parseBacktraceList        (buf);      break;
      case DISASSEMBLE:     emit rawGDBDisassemble    (buf);      break;
      case MEMDUMP:         emit rawGDBMemoryDump     (buf);      break;
      case REGISTERS:       emit rawGDBRegisters      (buf);      break;
      case LIBRARIES:       emit rawGDBLibraries      (buf);      break;
      default:                                                    break;
    }

    // Once we've dealt with the data, we can remove the current command if
    // it is a match for this data.
    if (currentCmd_ && currentCmd_->typeMatch(cmdType))
    {
      delete currentCmd_;
      currentCmd_ = 0;
    }
  }

  return end;
}

// **************************************************************************

// Deals with data that just falls out of gdb. Basically waits for a line
// terminator to arrive and then gives it to the line parser.
char* GDBController::parseOther(char* buf)
{
  // Could be the start of a block that isn't terminated yet
  ASSERT (*buf != BLOCK_START);

  char* end = buf;
  while (*end)
  {
    if (*end=='(')    // quick test before a big test
    {
      // This falls out of gdb without a \n terminator. Sometimes
      // a "Stopped due" message will fall out imediately behind this
      // creating a "line". Soemtimes it doesn'y. So we need to check
      // for and remove them first then continue as if it wasn't there.
      // And there can be more that one in a row!!!!!
      // Isn't this bloody awful...
      if (strncmp(end, "(no debugging symbols found)...", 31) == 0)
      {
        emit dbgStatus (QString(end, 32), state_);
        return end+30;    // The last char parsed
      }
    }

    if (*end=='\n')
    {
      *end = 0;        // make a null terminated c-string
      parseLine(buf);
      return end;
    }

    // Remove stuff like "junk\32i".
    // This only removes "junk" and leaves "\32i"
    if (*end == BLOCK_START)
      return end-1;

    end++;
  }

  return 0;
}

// **************************************************************************

char* GDBController::parse(char* buf)
{
  char* unparsed = buf;
  while (*unparsed)
  {
    char* parsed;
    if (*unparsed == BLOCK_START)
      parsed = parseCmdBlock(unparsed);
    else
      parsed = parseOther(unparsed);

    if (!parsed)
      break;

    // Move one beyond the end of the parsed data
    unparsed = parsed+1;
  }

  return (unparsed==buf) ? 0 : unparsed;
}

// **************************************************************************

void GDBController::setBreakpoint(const QString& BPSetCmd, int key)
{
  queueCmd(new GDBSetBreakpointCommand(BPSetCmd, key));
}

// **************************************************************************

void GDBController::clearBreakpoint(const QString& BPClearCmd)
{
  queueCmd(new GDBCommand(BPClearCmd, NOTRUNCMD, NOTINFOCMD));
  // Note: this is NOT an info command, because gdb doesn't explictly tell
  // us that the breakpoint has been deleted, so if we don't have it the
  // BP list doesn't get updated.
  queueCmd(new GDBCommand("info breakpoints", NOTRUNCMD, NOTINFOCMD, BPLIST));
}

// **************************************************************************

void GDBController::modifyBreakpoint(Breakpoint* BP)
{
  ASSERT(BP->isActionModify());
  if (BP->dbgId())
  {
    if (BP->changedCondition())
      queueCmd(new GDBCommand(QString().sprintf(
                  "condition %d %s",  BP->dbgId(), BP->conditional().data()), NOTRUNCMD, NOTINFOCMD));

    if (BP->changedIgnoreCount())
      queueCmd(new GDBCommand(QString().sprintf(
                  "ignore %d %d", BP->dbgId(), BP->ignoreCount()), NOTRUNCMD, NOTINFOCMD));

    if (BP->changedEnable())
      queueCmd(new GDBCommand(QString().sprintf("%s %d",
                  BP->isEnabled() ? "enable" : "disable", BP->dbgId()), NOTRUNCMD, NOTINFOCMD));

    BP->setDbgProcessing(true);
    // Note: this is NOT an info command, because gdb doesn't explictly tell
    // us that the breakpoint has been deleted, so if we don't have it the
    // BP list doesn't get updated.
    queueCmd(new GDBCommand("info breakpoints", NOTRUNCMD, NOTINFOCMD, BPLIST));
  }
}

// **************************************************************************
//	SLOTS
// **************************************************************************

// For most of these slots data can only be sent to gdb when it
// isn't busy but it is running.

void GDBController::slotRun()
{
  if (stateIsOn(s_appBusy|s_dbgNotStarted))
    return;

  queueCmd(new GDBCommand(stateIsOn(s_appNotStarted) ? "run" : "continue", RUNCMD, NOTINFOCMD));
}

// **************************************************************************

void GDBController::slotRunUntil(const QString& filename, int lineNo)
{
  if (stateIsOn(s_appBusy|s_dbgNotStarted))
    return;

  if (filename == "")
    queueCmd(new GDBCommand(QString().sprintf("until %d",lineNo), RUNCMD, NOTINFOCMD));
  else
    queueCmd(new GDBCommand(QString().sprintf("until %s:%d",
                          filename.data(), lineNo), RUNCMD, NOTINFOCMD));
}

// **************************************************************************

void GDBController::slotStepInto()
{
  if (stateIsOn(s_appBusy|s_appNotStarted))
    return;

  queueCmd(new GDBCommand("step", RUNCMD, NOTINFOCMD));
}

// **************************************************************************

void GDBController::slotStepOver()
{
  if (stateIsOn(s_appBusy|s_appNotStarted))
    return;

  queueCmd(new GDBCommand("next", RUNCMD, NOTINFOCMD));
}

// **************************************************************************

void GDBController::slotStepOutOff()
{
  if (stateIsOn(s_appBusy|s_appNotStarted))
    return;

  queueCmd(new GDBCommand("finish", RUNCMD, NOTINFOCMD));
}

// **************************************************************************

// Only interrupt a running program.
void GDBController::slotBreakInto()
{
  pauseApp();
}

// **************************************************************************

// See what, if anything needs doing to this breakpoint.
void GDBController::slotBPState(Breakpoint* BP)
{
  // Are we in a position to do anything to this breakpoint?
  if (stateIsOn(s_dbgNotStarted) || !BP->isPending() || BP->isActionDie())
    return;

  if (stateIsOn(s_appBusy))
  {
    if (!config_forceBPSet_)
      return;

    // When forcing breakpoints to be set/unset, interrupt a running app
    // and change the state.
  	setStateOn(s_silentBreakInto);
    pauseApp();
  }

  if (BP->isActionAdd())
  {
    setBreakpoint(BP->dbgSetCommand(), BP->key());
    BP->setDbgProcessing(true);
  }
  else
  {
    if (BP->isActionClear())
    {
      clearBreakpoint(BP->dbgRemoveCommand());
      BP->setDbgProcessing(true);
    }
    else
    {
      if (BP->isActionModify())
        modifyBreakpoint(BP); // Note: DbgProcessing gets set in modify fn
    }
  }

  if (stateIsOn(s_silentBreakInto))
    queueCmd(new GDBCommand("continue", RUNCMD, NOTINFOCMD));
}

// **************************************************************************

void GDBController::slotClearAllBreakpoints()
{
  // Are we in a position to do anything to this breakpoint?
  if (stateIsOn(s_dbgNotStarted))
    return;

  if (stateIsOn(s_appBusy))
  {
    if (!config_forceBPSet_)
      return;

    // When forcing breakpoints to be set/unset, interrupt a running app
    // and change the state.
  	setStateOn(s_silentBreakInto);
    pauseApp();
  }

  queueCmd(new GDBCommand("delete", NOTRUNCMD, NOTINFOCMD));
  // Note: this is NOT an info command, because gdb doesn't explictly tell
  // us that the breakpoint has been deleted, so if we don't have it the
  // BP list doesn't get updated.
  queueCmd(new GDBCommand("info breakpoints", NOTRUNCMD, NOTINFOCMD, BPLIST));

  if (stateIsOn(s_silentBreakInto))
    queueCmd(new GDBCommand("continue", RUNCMD, NOTINFOCMD));
}

// **************************************************************************

void GDBController::slotDisassemble(const QString& start, const QString& end)
{
  if (stateIsOn(s_appBusy|s_dbgNotStarted))
    return;

  QString cmd(QString().sprintf("disassemble %s %s", start.data(), end.data()));
  queueCmd(new GDBCommand(cmd, NOTRUNCMD, INFOCMD, DISASSEMBLE));
}

// **************************************************************************

void GDBController::slotMemoryDump(const QString& address, const QString& amount)
{
  if (stateIsOn(s_appBusy|s_dbgNotStarted))
    return;

  QString cmd(QString().sprintf("x/%sb %s", amount.data(), address.data()));
  queueCmd(new GDBCommand(cmd, NOTRUNCMD, INFOCMD, MEMDUMP));
}

// **************************************************************************

void GDBController::slotRegisters()
{
  if (stateIsOn(s_appBusy|s_dbgNotStarted))
    return;

  queueCmd(new GDBCommand("info all-registers", NOTRUNCMD, INFOCMD, REGISTERS));
}

// **************************************************************************

void GDBController::slotLibraries()
{
  if (stateIsOn(s_appBusy|s_dbgNotStarted))
    return;

  queueCmd(new GDBCommand("info sharedlibrary", NOTRUNCMD, INFOCMD, LIBRARIES));
}

// **************************************************************************

void GDBController::slotSelectFrame(int frameNo)
{
  if (stateIsOn(s_appBusy|s_dbgNotStarted))
    return;

  // Get gdb to switch the frame stack on a frame change.
  // This is an info command because _any_ run command will set the system back
  // to frame 0 regardless, so being removed with a run command is the best
  // thing that could happen here.
  if (frameNo != currentFrame_)
    queueCmd(new GDBCommand(QString().sprintf("frame %d", frameNo), NOTRUNCMD, NOTINFOCMD, FRAME));

  // Hold on to  this frame number so that we know where to put the
  // local variables if any are generated.
  currentFrame_ = frameNo;

  // Find or add the frame details. hold onto whether it existed because we're
  // about to create one if it didn't.
  FrameRoot* frame = varTree_->findFrame(frameNo);
  bool haveFrame = (bool)frame;
  if (!haveFrame)
    frame = new FrameRoot(varTree_, currentFrame_);

  ASSERT(frame);

  // Make vartree display a pretty frame description
  frame->setFrameName(frameStack_->getFrameName(currentFrame_));

  if (stateIsOn(s_viewLocals))
  {
    // Have we already got these details?
    if (frame->needLocals())
      queueCmd(new GDBCommand("info local", NOTRUNCMD, INFOCMD, LOCALS));
  }
}

// **************************************************************************

// This is called when the user desires to see the details of an item, by
// clicking open an varItem on the varTree.
void GDBController::slotExpandItem(VarItem* item)
{
  if (stateIsOn(s_appBusy|s_dbgNotStarted))
    return;

  switch (item->getDataType())
  {
    case typePointer:
      queueCmd(new GDBPointerCommand(item));
      break;

    default:
      queueCmd(new GDBItemCommand(item, "print "+ item->fullName()));
      break;
  }
}

// **************************************************************************

// This is called when an item needs special processing to show a value.
// Example = QStrings. We want to display the QString string against the var name
// so the user doesn't have to open the qstring to find it. Here's where that happens
void GDBController::slotExpandUserItem(VarItem* item, const QString& userRequest)
{
  if (stateIsOn(s_appBusy|s_dbgNotStarted))
    return;

  ASSERT(item);

  // Bad user data!!
  if (userRequest.isEmpty())
    return;

  queueCmd(new GDBItemCommand(item, "print "+userRequest, false, DATAREQUEST));
}

// **************************************************************************

// TODO comment needed To speed up
void GDBController::slotSetLocalViewState(bool onOff, int frameNo)
{
  if (onOff)
    setStateOn(s_viewLocals);
  else
    setStateOff(s_viewLocals);

  emit dbgStatus (onOff ? "<Locals ON>": "<Locals OFF>", state_);

  slotSelectFrame(frameNo);
}

// **************************************************************************

// Data from gdb gets processed here.
void GDBController::slotDbgStdout(KProcess *proc, char *buf, int buflen)
{
  GDB_DISPLAY("[gdb]>> "+QString(buf, buflen+1));

  // Allocate some buffer space, if adding to this buffer will exceed it
  if (gdbOutputLen_+buflen+1 > gdbSizeofBuf_)
  {
    gdbSizeofBuf_ = gdbOutputLen_+buflen+1;
    char* newBuf = new char[gdbSizeofBuf_];     // ??? shoudn't this be malloc ???
    if (gdbOutputLen_)
      memcpy(newBuf, gdbOutput_, gdbOutputLen_+1);
    delete[] gdbOutput_;                        // ??? and free ???
    gdbOutput_ = newBuf;
  }

  // Copy the data out of the KProcess buffer before it gets overwritten
  // and fake a string so we can use the string fns on this buffer
  memcpy(gdbOutput_+gdbOutputLen_, buf, buflen);
  gdbOutputLen_ += buflen;
  *(gdbOutput_+gdbOutputLen_) = 0;

  if (char* nowAt = parse(gdbOutput_))
  {
    ASSERT(nowAt <= gdbOutput_+gdbOutputLen_+1);
    gdbOutputLen_ = strlen(nowAt);
    // Some bytes that wern't parsed need to be move to the head of the buffer
    if (gdbOutputLen_)
      memmove(gdbOutput_, nowAt, gdbOutputLen_);     // Overlapping data
  }

  // check the queue for any commands to send
  executeCmd();
}

// **************************************************************************

/*void GDBController::slotDbgStderr(KProcess *proc, char *buf, int buflen)
{
  QString bufData(buf, buflen+1);
  GDB_DISPLAY(QString("[gdb] STDERR: ")+bufData);
  char* found;
//  if ((found = strstr(buf, "No symbol table is loaded")))
//    emit dbgStatus (QString("No symbol table is loaded"), state_);

  // If you end the app and then restart when you have breakpoints set
  // in a dynamically loaded library, gdb will halt because the set
  // breakpoint is trying to access memory no longer used. The breakpoint
  // must first be deleted, however, we want to retain the breakpoint for
  // when the library gets loaded again.
  // TODO  programHasExited_ isn't always set correctly,
  // but it's (almost) doesn't matter.
  if (programHasExited_ && (found = strstr(bufData.data(), "Cannot insert breakpoint")))
  {
    setStateOff(s_appBusy);
    int BPNo = atoi(found+25);
    if (BPNo)
    {
      queueCmd(new GDBCommand(QString().sprintf("delete %d", BPNo), NOTRUNCMD, NOTINFOCMD));
      queueCmd(new GDBCommand("info breakpoints", NOTRUNCMD, NOTINFOCMD, BPLIST));
      queueCmd(new GDBCommand("continue", RUNCMD, NOTINFOCMD));
      emit unableToSetBPNow(BPNo);
    }
    return;
  }

  parse(bufData.data());
}
*/
// **************************************************************************

void GDBController::slotDbgWroteStdin(KProcess *proc)
{
  setStateOff(s_waitForWrite);
  emit dbgStatus ("", state_);
  executeCmd();
}

// **************************************************************************

void GDBController::slotDbgProcessExited(KProcess* proc)
{
  destroyCmds();
  state_ = (s_appNotStarted|s_programExited|(state_&s_viewLocals));
//  state_  = s_dbgNotStarted|s_appNotStarted;
  emit dbgStatus (QString("Process exited"), state_);

  GDB_DISPLAY(QString("[gdb] Process exited"));
}

// **************************************************************************

// These are here for debug display. I wanted them to be removed if DBG_MONITOR
// wasn't defined but qt's moc has problems with that.
void GDBController::slotStepInSource(const QString& filename, int lineNo)
{
  DBG_DISPLAY((QString("(Show step in source) ")+filename+QString().setNum(lineNo)).data());
}

// **************************************************************************

void GDBController::slotDbgStatus(const QString& status, int state)
{
#if defined(DBG_MONITOR)
  QString s("(status) ");
  if (!state)
    s += QString("<program paused>");
  if (state & s_dbgNotStarted)
    s += QString("<dbg not started>");
  if (state & s_appNotStarted)
    s += QString("<app not started>");
  if (state & s_appBusy)
    s += QString("<app busy>");
  if (state & s_waitForWrite)
    s += QString("<wait for write>");
  if (state & s_programExited)
    s += QString("<program exited>");
  if (state & s_silentBreakInto)
    s += QString("<silent break into>");
  if (state & s_viewLocals)
    s += QString("<viewing locals>");

  DBG_DISPLAY((s+status).data());
#endif
}

// **************************************************************************
// **************************************************************************
// **************************************************************************
