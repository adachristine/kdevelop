/***************************************************************************
                    kdevelop.cpp - the main class in CKDevelop
                             -------------------                                         

    version              :                                   
    begin                : 20 Jul 1998                                        
    copyright            : (C) 1998 by Sandy Meier                         
    email                : smeier@rz.uni-potsdam.de                                     
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/


#include "ckdevelop.h"
#include <kmsgbox.h>
#include <qfile.h>
#include <qtstream.h>
#include <iostream.h>
#include <kfiledialog.h>
#include <qfont.h>
#include <qfileinf.h>
#include <ktabctl.h>
#include <qregexp.h>
#include "ckdevsetupdlg.h"
#include "ckappwizard.h"
#include "cupdatekdedocdlg.h"
#include <kdebug.h>
#include "./kwrite/kwdoc.h"
#include "ccreatedocdatabasedlg.h"



void CKDevelop::slotFileNewAppl(){
  slotStatusMsg(i18n("Creating a new frame application..."));
  CKAppWizard* kappw  = new CKAppWizard (this,"zutuz");
  kappw->exec();
  QString file = kappw->getProjectFile();

  if(kappw->generatedProject()){
    readProjectFile(file);
  }
  
  //cerr << kappw->getProjectFile();
  slotStatusMsg(IDS_DEFAULT); 
}
void CKDevelop::slotFileNewFile(){
  slotStatusMsg(i18n("Creating new file..."));
  
  newFile(false);
  slotStatusMsg(IDS_DEFAULT);
}


void CKDevelop::slotFileOpenFile(){
  slotStatusMsg(i18n("Opening file..."));

  QString str;
  str = KFileDialog::getOpenFileName(prj.getProjectDir(),"*",this);
  if (str.isEmpty()) return; //cancel
  switchToFile(str);
  
  slotStatusMsg(IDS_DEFAULT); 
  
}
void CKDevelop::slotFileOpenPrj(){
  slotStatusMsg(i18n("Opening project..."));

  QString str;
  str = KFileDialog::getOpenFileName(prj.getProjectDir(),"*.kdevprj",this);
  slotStatusMsg(IDS_DEFAULT); 
  if (str.isEmpty()) return; //cancel
  QFileInfo info(str);
  
  if (info.isFile()){
    if(!(readProjectFile(str))){
      KMsgBox::message(0,str,"This is a Project-File from KDevelop 0.1\nSorry,but it's incompatible with KDevelop >= 0.2.\nPlease use only new generated projects!");
    }
  }

}

void CKDevelop::slotFileSave(){

  slotStatusMsg(i18n("Saving file..."));
  if((edit_widget->getName() == "Untitled.cpp") || (edit_widget->getName() == "Untitled.h")){
    slotFileSaveAs();
  }
  else{
    edit_widget->doSave();
  }
  slotOptionsRefresh();
  slotStatusMsg(IDS_DEFAULT); 
}
void CKDevelop::slotFileSaveAll(){
  // ok,its a dirty implementation  :-)
  slotStatusMsg(i18n("Saving all changed files..."));

  TEditInfo* actual_info;
  bool mod = false;
	
  // first the 2 current edits
  if(header_widget->isModified()){
    if(header_widget->getName() == "Untitled.h"){
      switchToFile("Untitled.h");
      slotFileSaveAs();
    }
    else{
    header_widget->doSave();
    }
    mod=true;
  }
  if(cpp_widget->isModified()){
    if(cpp_widget->getName() == "Untitled.cpp"){
      switchToFile("Untitled.cpp");
      slotFileSaveAs();
    }
    else{
      cpp_widget->doSave();
    }
    mod=true;
  }
  
  for(actual_info=edit_infos.first();actual_info != 0;actual_info=edit_infos.next()){
    cerr << "check:" << actual_info->filename << endl;
    if(actual_info->modified){
      if((actual_info->filename == "Untitled.cpp") || (actual_info->filename == "Untitled.h")){
	switchToFile(actual_info->filename);
	slotFileSaveAs();
	cerr << actual_info->filename << "UNTITLED";
	mod = true;
      }
      else{
	switchToFile(actual_info->filename);
	edit_widget->doSave();
	cerr << actual_info->filename;
	mod = true;
      }
    }
  }
  if(mod){
    slotOptionsRefresh();
  }
  slotStatusMsg(IDS_DEFAULT); 
}
void CKDevelop::slotFileSaveAs(){
  slotStatusMsg(i18n("Save file as..."));
  QString name;
  TEditInfo* actual_info;
  name = KFileDialog::getSaveFileName(prj.getProjectDir(),0,this,edit_widget->getName());
  if (name.isNull()){
     cerr << "CANCEL\n";
     return;
   }
   else {
     edit_widget->toggleModified(true);
     edit_widget->doSave(name); // try the save
     //     if (err == KEdit::KEDIT_OS_ERROR){
     //  cerr << "error";
     //  return; // no action
     // }
  // and now the modifications for the new file
  //search the actual edit_info
  for(actual_info=edit_infos.first();actual_info != 0;actual_info=edit_infos.next()){
    if (actual_info->filename == edit_widget->getName()){ // found
	
 	//update the info-struct in the list
      actual_info->filename = name;
      actual_info->modified = false;
      //update the widget
      edit_widget->setName(name);
      edit_widget->toggleModified(false);
      //update the menu
      QFileInfo fileinfo(name);
      menu_buffers->changeItem(fileinfo.fileName(),actual_info->id);
      //update kdevelop
      setCaption(name);
    }
  } // end_for
} // end_else
  slotStatusMsg(IDS_DEFAULT); 
}

void CKDevelop::slotFileClose(){

  slotStatusMsg(i18n("Closing file..."));
  TEditInfo* actual_info;
  int message_result;
  
  if(edit_widget->isModified()){
    message_result = KMsgBox::yesNoCancel(this,i18n("Save?"),
					  i18n("The document was modified,save?"),KMsgBox::QUESTION);
    if (message_result == 1){ // yes
      edit_widget->doSave();
    }
    if (message_result == 3){ // cancel
      slotStatusMsg(IDS_DEFAULT); 
      return;
    }
  }
  
  //search the actual edit_info and remove it
  for(actual_info=edit_infos.first();actual_info != 0;actual_info=edit_infos.next()){
    if (actual_info->filename == edit_widget->getName()){ // found
             cerr << "remove edit_info begin\n";
             menu_buffers->removeItem(actual_info->id);
	     if(edit_infos.removeRef(actual_info)){
	cerr << "remove edit_info end\n";
       }
    }
  }
  // add the next edit to the location
   for(actual_info=edit_infos.first();actual_info != 0;actual_info=edit_infos.next()){
     if ( getTabLocation(actual_info->filename) == getTabLocation(edit_widget->getName())){ // found
       edit_widget->setText(actual_info->text);
       edit_widget->toggleModified(actual_info->modified);
       edit_widget->setName(actual_info->filename);
       setCaption(actual_info->filename);
       cerr << "FOUND A NEXT" << actual_info->filename;
       slotStatusMsg(IDS_DEFAULT); 
       return;
     }
   }
   // if not found a successor create an new file
   
   actual_info = new TEditInfo;
   actual_info->modified=false;
   if (getTabLocation(edit_widget->getName()) == 0) {// header
     actual_info->id = menu_buffers->insertItem("Untitled.h",-2,0);
     actual_info->filename = "Untitled.h";
  }
   else{
     actual_info->id = menu_buffers->insertItem("Untitled.cpp",-2,0);
     actual_info->filename = "Untitled.cpp";
   }
   edit_infos.append(actual_info);
   edit_widget->setName(actual_info->filename);
   edit_widget->clear();
   setCaption(actual_info->filename);

   slotStatusMsg(IDS_DEFAULT); 
}
void CKDevelop::slotFileCloseAll(){
  slotStatusMsg(i18n("Closing all files..."));
  slotStatusMsg(IDS_DEFAULT); 
}
void CKDevelop::slotFilePrint(){
}
void CKDevelop::closeEvent(QCloseEvent* e){
  e->accept();
  cerr << "QUIT5";
  //swallow_widget->sWClose(false);

  config->setGroup("General Options");
  config->writeEntry("width",width());
  config->writeEntry("height",height());
  config->writeEntry("view_panner_pos",view->absSeparatorPos());
  config->writeEntry("top_panner_pos",top_panner->absSeparatorPos());
  config->setGroup("Files");
  if(project){
    config->writeEntry("project_file",prj.getProjectFile());
  }
  config->writeEntry("cpp_file",cpp_widget->getName());
  config->writeEntry("header_file",header_widget->getName());
  config->writeEntry("browser_file",history_list.current());
  cerr << "QUIT4";
  config->setGroup("View Configuration");
  // config->writeEntry("show_tree_view",options_menu->isItemChecked(ID_OPTIONS_TREEVIEW));
  // config->writeEntry("show_output_view",options_menu->isItemChecked(ID_OPTIONS_OUTPUTVIEW));
  config->writeEntry("show_std_toolbar",options_menu->isItemChecked(ID_OPTIONS_STD_TOOLBAR));
  config->writeEntry("show_browser_toolbar",options_menu->isItemChecked(ID_OPTIONS_BROWSER_TOOLBAR));
  config->writeEntry("show_statusbar",options_menu->isItemChecked(ID_OPTIONS_STATUSBAR));
  cerr << "QUIT3";
  //config->writeEntry("x",x());
  //config->writeEntry("y",y());
  config->sync();
  cerr << "QUIT2";
  if(project){
    prj.writeProject();
  }
  cerr << "QUIT1";
  KTMainWindow::closeEvent(e);
}

void CKDevelop::slotFileQuit(){
  slotStatusMsg(i18n("Exiting..."));
  close(); 
}
void CKDevelop::slotEditUndo(){
  edit_widget->undo();
}
void CKDevelop::slotEditRedo(){
  edit_widget->redo();
}
void CKDevelop::slotEditCut(){
  slotStatusMsg(i18n("Cutting..."));
  edit_widget->cut();
  slotStatusMsg(IDS_DEFAULT); 
}
void CKDevelop::slotEditCopy(){
  slotStatusMsg(i18n("Copying..."));
  edit_widget->copyText();
  slotStatusMsg(IDS_DEFAULT); 
}
void CKDevelop::slotEditPaste(){
  slotStatusMsg(i18n("Pasting selection..."));
  edit_widget->paste();
  slotStatusMsg(IDS_DEFAULT); 
}
void CKDevelop::slotEditSelectAll(){
  slotStatusMsg(i18n("Selecting all..."));
  edit_widget->selectAll();
  slotStatusMsg(IDS_DEFAULT); 
}
void CKDevelop::slotEditInvertSelection(){
  edit_widget->invertSelection();
}
void CKDevelop::slotEditDeselectAll(){
  edit_widget->deselectAll();
}
void CKDevelop::slotEditInsertFile(){
  slotStatusMsg(i18n("Inserting file contents..."));
  edit_widget->insertFile();
  slotStatusMsg(IDS_DEFAULT); 
}
void CKDevelop::slotEditSearch(){
  slotStatusMsg(i18n("Searching..."));
  edit_widget->search();
  slotStatusMsg(IDS_DEFAULT); 
}
void CKDevelop::slotEditRepeatSearch(){
  slotStatusMsg(i18n("Repeating last search..."));
  edit_widget->searchAgain();
  slotStatusMsg(IDS_DEFAULT); 
}
void CKDevelop::slotEditReplace(){
  slotStatusMsg(i18n("Replacing..."));
  edit_widget->replace();
  slotStatusMsg(IDS_DEFAULT); 
}
void CKDevelop::slotEditGotoLine(){
  slotStatusMsg(i18n("Switching to selected line..."));
  edit_widget->gotoLine();
  slotStatusMsg(IDS_DEFAULT); 
}
void CKDevelop::slotOptionsTStdToolbar(){
 if(options_menu->isItemChecked(ID_OPTIONS_STD_TOOLBAR)){
   options_menu->setItemChecked(ID_OPTIONS_STD_TOOLBAR,false);
    enableToolBar(KToolBar::Hide);
  }
  else{
    options_menu->setItemChecked(ID_OPTIONS_STD_TOOLBAR,true);
    enableToolBar(KToolBar::Show);
  }

}
void CKDevelop::slotOptionsTBrowserToolbar(){
  if(options_menu->isItemChecked(ID_OPTIONS_BROWSER_TOOLBAR)){
    options_menu->setItemChecked(ID_OPTIONS_BROWSER_TOOLBAR,false);
    enableToolBar(KToolBar::Hide,ID_BROWSER_TOOLBAR);
  }
  else{
    options_menu->setItemChecked(ID_OPTIONS_BROWSER_TOOLBAR,true);
    enableToolBar(KToolBar::Show,ID_BROWSER_TOOLBAR);
  }
}

void CKDevelop::slotOptionsTStatusbar(){
  
  bViewStatusbar=!bViewStatusbar;
    options_menu->setItemChecked(ID_OPTIONS_STATUSBAR,bViewStatusbar);
    enableStatusBar();
  
}
void CKDevelop::slotOptionsTTreeView(){
  /*
  if(options_menu->isItemChecked(ID_OPTIONS_TREEVIEW)){
    options_menu->setItemChecked(ID_OPTIONS_TREEVIEW,false);
    top_panner->deactivate();

    view->recreate(this,0,QPoint(0,0),true);
    
    s_tab_view->recreate(view,0,QPoint(0,0),true);
    top_panner->removeChild(s_tab_view);
    top_panner->removeChild(t_tab_view);
    view->removeChild(top_panner);

    header_widget->recreate(s_tab_view,0,QPoint(0,0),true);
    cpp_widget->recreate(s_tab_view,0,QPoint(0,0),true);
    browser_widget->recreate(s_tab_view,0,QPoint(0,0),true);
    swallow_widget->recreate(s_tab_view,0,QPoint(0,0),true);

    //output_widget->recreate(view,0,QPoint(0,0),true);
    view->deactivate();
    view->activate(s_tab_view,output_widget);
    //view->setSeperatorPos(100);
    cerr << "IF";
    show();
     view->show();
    // s_tab_view->show();
    // output_widget->show();
  }
  else{
    options_menu->setItemChecked(ID_OPTIONS_TREEVIEW,true);
    view->deactivate();
    s_tab_view->recreate(top_panner,0,QPoint(0,0),true);
    header_widget->recreate(s_tab_view,0,QPoint(0,0),true);
    cpp_widget->recreate(s_tab_view,0,QPoint(0,0),true);
    browser_widget->recreate(s_tab_view,0,QPoint(0,0),true);
    swallow_widget->recreate(s_tab_view,0,QPoint(0,0),true);
    top_panner->activate(t_tab_view,s_tab_view);// activate the top_panner
    view->activate(top_panner,output_widget); 
    cerr << "ELSE";
  }
  */
}
void CKDevelop::slotOptionsTOutputView(){
}

void CKDevelop::slotOptionsRefresh(){
  refreshTrees();
}
void CKDevelop::slotOptionsEditor(){
  cpp_widget->optDlg();
  header_widget->copySettings(cpp_widget);
  config->setGroup("KWrite Options");
  edit_widget->writeConfig(config);
  edit_widget->doc()->writeConfig(config);
}
void CKDevelop::slotOptionsEditorColors(){
  cpp_widget->colDlg();
  header_widget->copySettings(cpp_widget);
  config->setGroup("KWrite Options");
  edit_widget->writeConfig(config);
  edit_widget->doc()->writeConfig(config);
}
void CKDevelop::slotOptionsSyntaxHighlighting(){
  cpp_widget->hlDlg();
  header_widget->copySettings(cpp_widget);
  config->setGroup("KWrite Options");
  edit_widget->writeConfig(config);
  edit_widget->doc()->writeConfig(config);
}

void CKDevelop::slotOptionsKDevelop(){
  slotStatusMsg(i18n("Setting up KDevelop..."));
  CKDevSetupDlg setup;
  setup.show();
  slotStatusMsg(IDS_DEFAULT); 
}

void CKDevelop::slotDocBack(){
  slotStatusMsg(i18n("Switching to last page..."));
  QString str = history_list.prev();
  if (str != 0){
    s_tab_view->setCurrentTab(BROWSER);
    browser_widget->showURL(str);
    enableCommand(ID_DOC_FORWARD);
  }
  if (history_list.prev() == 0){ // no more backs
    disableCommand(ID_DOC_BACK);
    history_list.first(); // set it at first
  }
  else{
    history_list.next();
  }
  cerr << endl << "COUNT HISTORYLIST:" << history_list.count();;
  slotStatusMsg(IDS_DEFAULT); 
}
void CKDevelop::slotDocForward(){
  slotStatusMsg(i18n("Switching to next page..."));
  QString str = history_list.next();
  if (str != 0){
    s_tab_view->setCurrentTab(BROWSER);
    browser_widget->showURL(str);
    enableCommand(ID_DOC_BACK);
  }
  if (history_list.next() == 0){ // no more forwards
   disableCommand(ID_DOC_FORWARD);
    history_list.last(); // set it at last
  }
  else{
    history_list.prev();
  }
  slotStatusMsg(IDS_DEFAULT); 
}
void CKDevelop::slotSearchReceivedStdout(KProcess* proc,char* buffer,int buflen){
  QString str(buffer,buflen+1);
  search_output = search_output + str;
}
void CKDevelop::slotSearchProcessExited(KProcess*){
  //  cerr << search_output;
  int pos=0;
  int nextpos=0;
  QStrList list;
  QStrList sort_list;
  QString str;
  QString found_str;
  int i=0;
  int max=0;

  while((nextpos = search_output.find('\n',pos)) != -1){
    str = search_output.mid(pos,nextpos-pos);
    list.append(str);
    pos = nextpos+1;
  }
  
  // //lets sort it a little bit
   for(;i<30;i++){
    max =0;
    found_str = "";
    for(str = list.first();str != 0;str = list.next()){
      if (searchToolGetNumber(str) >= max){
	found_str = str.copy();
	max = searchToolGetNumber(str);
      }
    }
    if(found_str != ""){
      sort_list.append(found_str);
      list.remove(found_str);
    }
    
  }

   QString filename = QDir::homeDirPath() + "/.kdevelop/search_result.html";
   QFile file(filename);
   QTextStream stream(&file);
   file.open(IO_WriteOnly);
   
   stream << "<HTML><HEAD></HEAD><BODY BGCOLOR=\"#ffffff\"><BR> <TABLE><TR><TH>Title<TH>Hits\n";
   QString numstr;
   for(str = sort_list.first();str != 0;str = sort_list.next() ){
     stream << "<TR><TD><A HREF=\""+searchToolGetURL(str)+"\">"+
			   searchToolGetTitle(str)+"</A><TD>"+
			   numstr.setNum(searchToolGetNumber(str)) + "\n";
   }
   
   stream << "\n</TABLE></BODY></HTML>";
   
   file.close();
   slotURLSelected(browser_widget,"file:" + filename,1,"test");

}
QString CKDevelop::searchToolGetTitle(QString str){
  int pos = str.find(' ');
  pos = str.find(' ',pos);
  int end_pos = str.find(':',pos);
  return str.mid(pos,end_pos-pos);
}
QString CKDevelop::searchToolGetURL(QString str){
  int pos = str.find(' ');
  return str.left(pos);
}
int CKDevelop::searchToolGetNumber(QString str){
  int pos =str.findRev(':');
  QString sub = str.right((str.length()-pos-2));
  cerr << sub << endl;
  return sub.toInt();
}
void CKDevelop::slotCreateSearchDatabase(){

  CCreateDocDatabaseDlg dlg(this,"DLG",&shell_process,config);
  if(dlg.exec()){
    slotStatusMsg(i18n("Creating Search Database..."));
  }

  return;
  
}
void CKDevelop::slotDocSText(){
  slotStatusMsg(i18n("Searching selected text in documentation..."));
  QString text = edit_widget->markedText();
  if(text == ""){
    text = edit_widget->currentWord();
  }
  
  search_output = ""; // delete all from the last search
  search_process.clearArguments();
  search_process << "glimpse  -H "+ QDir::homeDirPath() + "/.kdevelop -U -c -y '"+ text +"'";
  search_process.start(KShellProcess::NotifyOnExit,KShellProcess::AllOutput); 

 //  config->setGroup("Doc_Location");
//   QString filename =  config->readEntry("doc_qt") + classname.lower() + ".html";
//   if (QFile::exists(filename)){
//     slotURLSelected(browser_widget,filename,1,"test");
//   }
//   filename = config->readEntry("doc_kde_core") + classname + ".html";
//   if (QFile::exists(filename)){
//     slotURLSelected(browser_widget,filename,1,"test");
//   }
//   filename =  config->readEntry("doc_kde_gui") + classname + ".html";
//   if (QFile::exists(filename)){
//     slotURLSelected(browser_widget,filename,1,"test");
//   }
//   filename =  config->readEntry("doc_kde_kfile") + classname + ".html";
//   if (QFile::exists(filename)){
//     slotURLSelected(browser_widget,filename,1,"test");
//   }
//   filename =  config->readEntry("doc_kde_html") + classname + ".html";
//   if (QFile::exists(filename)){
//     slotURLSelected(browser_widget,filename,1,"test");
//   }
  slotStatusMsg(IDS_DEFAULT); 
}
void CKDevelop::slotDocQtLib(){
  config->setGroup("Doc_Location");
  slotURLSelected(browser_widget,"file:" + config->readEntry("doc_qt") + "index.html",1,"test");
}
void CKDevelop::slotDocKDECoreLib(){
  config->setGroup("Doc_Location");
  slotURLSelected(browser_widget,"file:" + config->readEntry("doc_kde") + "kdecore/index.html",1,"test");
}
void CKDevelop::slotDocKDEGUILib(){
  config->setGroup("Doc_Location");
  slotURLSelected(browser_widget,"file:" +  config->readEntry("doc_kde") + "kdeui/index.html",1,"test");
}
void CKDevelop::slotDocKDEKFileLib(){
  config->setGroup("Doc_Location");
  slotURLSelected(browser_widget,"file:" +  config->readEntry("doc_kde") + "kfile/index.html",1,"test");
}
void CKDevelop::slotDocKDEHTMLLib(){
  config->setGroup("Doc_Location");
  slotURLSelected(browser_widget,"file:" +  config->readEntry("doc_kde") + "khtmlw/index.html",1,"test");
}


void CKDevelop::slotDocAPI(){
  slotStatusMsg(i18n("Switching to project API Documentation..."));
  slotURLSelected(browser_widget,prj.getProjectDir() + prj.getSubDir() +  "api/index.html",1,"test");
  slotStatusMsg(IDS_DEFAULT);
}
void CKDevelop::slotDocManual(){
  slotStatusMsg(i18n("Switching to project Manual..."));
  prj.createMakefilesAm();
  unsigned int index = prj.getSGMLFile().length()-4;
  QString name = prj.getSGMLFile().copy();
  name.remove(index,4);
  slotURLSelected(browser_widget,prj.getProjectDir() + prj.getSubDir() + "docs/en/" + name + "html",1,"test");
  slotStatusMsg(IDS_DEFAULT);
}
void CKDevelop::slotDocUpdateKDEDocumentation(){
  slotStatusMsg(i18n("Updating KDE-Libs documentation..."));
  config->setGroup("Doc_Location");
  CUpdateKDEDocDlg dlg(this,"test",&shell_process, config);
  if(dlg.exec()){
    slotStatusMsg(i18n("Generating Documentation..."));
    setToolMenuProcess(false);
  }
}

void CKDevelop::slotBuildRun(){
  slotBuildMake();
  next_job = "run";
}
void CKDevelop::slotBuildMake(){
  setToolMenuProcess(false);
  slotFileSaveAll();
  slotStatusMsg(i18n("Running make..."));
  output_widget->clear();
  QDir::setCurrent(prj.getProjectDir() + prj.getSubDir()); 
  process.clearArguments();
  process << "make";
  
  process.start(KProcess::NotifyOnExit,KProcess::AllOutput);
}

void CKDevelop::slotBuildRebuildAll(){
  setToolMenuProcess(false);
  slotFileSaveAll();
  slotStatusMsg(i18n("Running make clean-command "));
  output_widget->clear();
  QDir::setCurrent(prj.getProjectDir() + prj.getSubDir()); 
  process.clearArguments();
  process << "make";
  process << "clean";
  next_job = "make"; // checked in slotProcessExited()
  
  process.start(KProcess::NotifyOnExit,KProcess::AllOutput);
}
void CKDevelop::slotBuildCleanRebuildAll(){
  setToolMenuProcess(false);
  slotFileSaveAll();
  output_widget->clear();
  slotStatusMsg(i18n("Running make clean and rebuilding all..."));
  QDir::setCurrent(prj.getProjectDir()); 
  process.clearArguments();
  QString path = kapp->kde_datadir()+"/kdevelop/tools/";
  process << "sh" << path + "cleanrebuildall";
  
  process.start(KProcess::NotifyOnExit,KProcess::AllOutput);
}
void CKDevelop::slotBuildAutoconf(){
  setToolMenuProcess(false);
  slotFileSave();
  output_widget->clear();
  QDir::setCurrent(prj.getProjectDir()); 
  process.clearArguments();
  process << "autoconf";
  process.start(KProcess::NotifyOnExit,KProcess::AllOutput);
}
void CKDevelop::slotBuildStop(){
  slotStatusMsg(i18n("Killing current process..."));
  setToolMenuProcess(true);
  process.kill();
  shell_process.kill();
  slotStatusMsg(IDS_DEFAULT);
}
void CKDevelop::slotBuildAPI(){
  setToolMenuProcess(false);
  slotFileSaveAll();
  slotStatusMsg(i18n("Creating project API-Documentation..."));
  output_widget->clear();
  QDir::setCurrent(prj.getProjectDir() + prj.getSubDir()); 
  shell_process.clearArguments();
  shell_process << "kdoc";
  shell_process << "-p -d" + prj.getProjectDir() + prj.getSubDir() +  "api";
  shell_process << prj.getProjectName();
  shell_process << "*.h";
  
  shell_process.start(KShellProcess::NotifyOnExit,KShellProcess::AllOutput);

}

void CKDevelop::slotBuildManual(){

  setToolMenuProcess(false);
  //  slotFileSaveAll();
  slotStatusMsg(i18n("Creating project Manual..."));
  output_widget->clear();
  QDir::setCurrent(prj.getProjectDir() + prj.getSubDir() + "/docs/en/");
  process.clearArguments();
  process << "sgml2html";
  process << prj.getSGMLFile();
  process.start(KProcess::NotifyOnExit,KProcess::AllOutput); 

}


void CKDevelop::slotBookmarksAdd(){
}
void CKDevelop::slotBookmarksEdit(){
}

void CKDevelop::slotURLSelected(KHTMLView* ,const char* url,int,const char*){
  s_tab_view->setCurrentTab(BROWSER);
  browser_widget->showURL(url);

  if (!history_list.isEmpty()){
    enableCommand(ID_DOC_BACK);
  }
  // insert into the history-list
  history_list.append(url);
}

void CKDevelop::slotReceivedStdout(KProcess*,char* buffer,int buflen){ 
  int x,y;
  output_widget->cursorPosition(&x,&y);
  QString str(buffer,buflen+1);
  output_widget->insertAt(str,x,y);
}
void CKDevelop::slotReceivedStderr(KProcess*,char* buffer,int buflen){ 
 
  int x,y;
  output_widget->cursorPosition(&x,&y);
  QString str(buffer,buflen+1);
  output_widget->insertAt(str,x,y);
}

void CKDevelop::slotClickedOnOutputWidget(){
  int x,y;
  int error_line;
  QString text;
  QString error_line_str;
  QString error_filename;
  int pos1,pos2; // positions in the string
  QRegExp reg(":[0-9]*:"); // is it a error line?, I hope it works

  output_widget->cursorPosition(&x,&y);
  text = output_widget->textLine(x);
  if((pos1=reg.match(text)) == -1) return; // not a error line

  // extract the error-line
  pos2 = text.find(':',pos1+1);
  error_line_str = text.mid(pos1+1,pos2-pos1-1);
  error_line = error_line_str.toInt();

  // extract the filename
  pos2 = text.findRev(' ',pos1);
  if (pos2 == -1) {
    pos2 = 0; // the filename is at the begining of the string
  }
  else { pos2++; }

  error_filename = text.mid(pos2,pos1-pos2);

  // switch to the file
  if (error_filename.find('/') == -1){ // it is a file outer the projectdir ?
    error_filename = prj.getProjectDir() + prj.getSubDir() + error_filename;
  }
  if (QFile::exists(error_filename)){
    switchToFile(error_filename);
    edit_widget->setCursorPosition(error_line-1,0);
    edit_widget->setFocus();
  }

}
void CKDevelop::slotProcessExited(KProcess*){
  setToolMenuProcess(true);
  slotStatusMsg(IDS_DEFAULT);
  if (process.normalExit()) {
    if (next_job == "make"){ // rest from the rebuild all
      QDir::setCurrent(prj.getProjectDir() + prj.getSubDir()); 
      process.clearArguments();
      process << "make";
      setToolMenuProcess(false);
      process.start(KProcess::NotifyOnExit,KProcess::AllOutput);
      next_job = "";
    }
    if (next_job == "run" && process.exitStatus() == 0){ // rest from the buildRun
      QDir::setCurrent(prj.getProjectDir() + prj.getSubDir()); 
      process.clearArguments();
      // Warning: not every user has the current directory in his path !
      process << "./" + prj.getBinPROGRAM().lower();
      setToolMenuProcess(false);
      process.start(KProcess::NotifyOnExit,KProcess::AllOutput);
      next_job = "";
    }
    if (next_job == "refresh"){ // rest from the add projectfile
      refreshTrees();
    }
  } 
  else {
    cerr << "Make exited with error(s)..." << endl;
  }
}

void CKDevelop::slotSTabSelected(int item){
  if (item == HEADER){
    edit_widget = header_widget;
    edit_widget->setFocus();
    slotNewUndo();
    slotNewStatus();
    slotNewLineColumn();
    setCaption("KDevelop V" + version + ": " + edit_widget->getName());
  }
  if (item == CPP){
    edit_widget = cpp_widget;
    edit_widget->setFocus();
    slotNewUndo();
    slotNewStatus();
    slotNewLineColumn();
    setCaption("KDevelop V" + version + ": " + edit_widget->getName());
  }
  if(item == BROWSER){
    browser_widget->setFocus();
  }
 
  //  s_tab_current = item;
  
}

void CKDevelop::slotMenuBuffersSelected(int id){
  TEditInfo* info;
  
  for(info=edit_infos.first();info != 0;info=edit_infos.next()){
    if (info->id == id){
      switchToFile(info->filename);
      return; // if found than return
    }
  }  
}


void CKDevelop::slotLogFileTreeSelected(int index){
  cerr << "SELECTED\n";
  KPath* path;
  QString* str;
  if (log_file_tree->itemAt(index)->hasChild() == true) return; // no action
  if(!(log_file_tree->isFile(index))) return; // it is not file
  
  if(!log_file_tree->leftButton()) return; // not pressed the left button
  path = log_file_tree->itemPath(index);
  str = path->pop();
  switchToFile(prj.getProjectDir() + *str);
  cerr << "SELECTED2\n";
}

void CKDevelop::slotRealFileTreeSelected(int index){
  if (real_file_tree->itemAt(index)->hasChild() == true) return; // no action
  KPath* path = real_file_tree->itemPath(index);
  QString str_path;
  QString* str;
  //cerr << index;
  str = path->pop();
  str_path =  *str + str_path;
  while ((str = path->pop()) != 0){
    if (str->right(1) == "/"){
      str_path =  *str + str_path;
    }
    else{
      str_path =  *str + "/"+ str_path;
    }
  }
  //  cerr << str_path;
  switchToFile(str_path);
  
}

void CKDevelop::slotDocTreeSelected(int index){
  QString string = doc_tree->itemAt(index)->getText() ;
  if( string == "Documentation"){
    slotHelpContent();
    return;
  }
  if (doc_tree->itemAt(index)->hasChild() == true) return; // no action
  KPath* path;
  QString* str;
  path = doc_tree->itemPath(index);
  str = path->pop();
  config->setGroup("Doc_Location");
  if(*str == "Tutorial"){
    slotURLSelected(browser_widget,"file:" + KApplication::kde_htmldir() + 
		    "/en/kdevelop/tutorial.html",1,"test");
  }
  if(*str == "Manual"){
    slotURLSelected(browser_widget,"file:" + KApplication::kde_htmldir() + 
		    "/en/kdevelop/index.html",1,"test");
  }
  if(*str == "Qt-Library"){
    slotURLSelected(browser_widget,"file:" + config->readEntry("doc_qt") + "index.html",1,"test");
  }
  if(*str == "KDE-Core-Library"){
    slotURLSelected(browser_widget,"file:" + config->readEntry("doc_kde") + "kdecore/index.html",1,"test");
  }
  if(*str == "KDE-UI-Library"){
    slotURLSelected(browser_widget,"file:" + config->readEntry("doc_kde") + "kdeui/index.html",1,"test");
  }
  if(*str == "KDE-KFile-Library"){
    slotURLSelected(browser_widget,"file:" + config->readEntry("doc_kde") + "kfile/index.html",1,"test");
  }
  if(*str == "KDE-HTMLW-Library"){
    slotURLSelected(browser_widget,"file:" + config->readEntry("doc_kde") + "khtmlw/index.html",1,"test");
  }
  if(*str == "KDE-KFM-Library"){
    slotURLSelected(browser_widget,"file:" + config->readEntry("doc_kde") + "kfmlib/index.html",1,"test");
  }
  if(*str == "KDE-KAB-Library"){
    slotURLSelected(browser_widget,"file:" + config->readEntry("doc_kde") + "kab/index.html",1,"test");
  }
  if(*str == "KDE-KSpell-Library"){
    slotURLSelected(browser_widget,"file:" + config->readEntry("doc_kde") + "kspell/index.html",1,"test");
  }
  if(*str == "User-Manual"){
    slotDocManual();
  }
  if(*str == "API-Documentation"){
    slotDocAPI();
    
  }
}
void CKDevelop::slotToolsKIconEdit(){
  s_tab_view->setCurrentTab(TOOLS);
  swallow_widget->sWClose(false);
  swallow_widget->setExeString("kiconedit");
  swallow_widget->sWExecute();
  swallow_widget->init();
}

void CKDevelop::slotToolsKDbg(){
  s_tab_view->setCurrentTab(TOOLS);
  swallow_widget->sWClose(false);
  swallow_widget->setExeString("kdbg");
  swallow_widget->sWExecute();
  swallow_widget->init();
}
void CKDevelop::slotHelpContent(){
  slotURLSelected(browser_widget,"file:" + KApplication::kde_htmldir() + 
		  "/en/kdevelop/index.html",1,"test");
}
void CKDevelop::slotHelpHomepage(){
  //  slotURLSelected(browser_widget,"http://anakonda.alpha.org/~smeier/kdevelop/index.html",1,"test");
}
void CKDevelop::slotHelpAbout(){
  KMsgBox::message(this,i18n("About..."),i18n("\tKDevelop Version "+version+" (Alpha) \n\n\t(c) 1998 KDevelop Team \n
Sandy Meier <smeier@rz.uni-potsdam.de>
Stefan Heidrich <sheidric@rz.uni-potsdam.de>
Stefan Bartel <bartel@rz.uni-potsdam.de>
Ralf Nolden <Ralf.Nolden@post.rwth-aachen.de>
                             
KDevelop contains sourcecode from KWrite 0.97 
(c) by Jochen Wilhelmy <digisnap@cs.tu-berlin.de>
"));

}

void CKDevelop::slotStatusMsg(const char *text)
{
  ///////////////////////////////////////////////////////////////////
  // change status message permanently
  statusBar()->clear();
  statusBar()->changeItem(text, ID_STATUS_MSG );
}


void CKDevelop::slotStatusHelpMsg(const char *text)
{
  ///////////////////////////////////////////////////////////////////
  // change status message of whole statusbar temporary (text, msec)
  statusBar()->message(text, 2000);
}






void CKDevelop::enableCommand(int id_)
{
  menuBar()->setItemEnabled(id_,true);
  toolBar()->setItemEnabled(id_,true);
  toolBar(ID_BROWSER_TOOLBAR)->setItemEnabled(id_,true);
}

void CKDevelop::disableCommand(int id_)
{
  menuBar()->setItemEnabled(id_,false);
  toolBar()->setItemEnabled(id_,false);
  toolBar(ID_BROWSER_TOOLBAR)->setItemEnabled(id_,false);
}

void CKDevelop::slotNewStatus()
{
  int config;
  config = edit_widget->config();
  statusBar()->changeItem(config & cfOvr ? "OVR" : "INS",ID_STATUS_INS_OVR);
}

void CKDevelop::slotNewLineColumn()
{
  QString linenumber;
  linenumber.sprintf(i18n("Line: %d Col: %d"), 
     			edit_widget->currentLine() +1,
			edit_widget->currentColumn() +1);
  statusBar()->changeItem(linenumber.data(), ID_STATUS_LN_CLM);
} 
void CKDevelop::slotNewUndo(){
  int state;

  state = edit_widget->undoState();
  //undo
  if(state & 1){
    enableCommand(ID_EDIT_UNDO);
  }
  else{
    disableCommand(ID_EDIT_UNDO);
  }
  //redo
  if(state & 2){
    enableCommand(ID_EDIT_REDO);
  }
  else{
    disableCommand(ID_EDIT_REDO);
  }
  
}

void CKDevelop::slotToolbarClicked(int item){
  switch (item) {
  case ID_FILE_NEW_FILE:
    slotFileNewFile();
    break;
  case ID_FILE_OPEN_FILE:
    slotFileOpenFile();
    break;
  case ID_FILE_SAVE:
    slotFileSave();
    break;
  case ID_FILE_SAVE_ALL:
    slotFileSaveAll();
    break;
    
  case ID_EDIT_COPY:
    slotEditCopy();
    break;
  case ID_EDIT_PASTE:
    slotEditPaste();
    break;
  case ID_EDIT_CUT:
    slotEditCut();
    break;
  case ID_OPTIONS_REFRESH:
    slotOptionsRefresh();
    break;
  case ID_BUILD_MAKE:
    slotBuildMake();
    break;
  case ID_BUILD_RUN:
    slotBuildRun();
    break;
  case ID_BUILD_STOP:
    slotBuildStop();
    break;
  case ID_DOC_SEARCH_TEXT:
    slotDocSText();
    break;
  case ID_DOC_BACK:
    slotDocBack();
    break;
  case ID_DOC_FORWARD:
    slotDocForward();
    break;
  }
}


BEGIN_STATUS_MSG(CKDevelop)

  ON_STATUS_MSG(ID_FILE_NEW_FILE,    i18n("Creates a new file"))
  ON_STATUS_MSG(ID_FILE_NEW_PROJECT, i18n("Generate a new project"))
  ON_STATUS_MSG(ID_FILE_OPEN_FILE,   i18n("Opens an existing file"))  
  ON_STATUS_MSG(ID_FILE_OPEN_PROJECT,i18n("Opens an existing project"))
  
  ON_STATUS_MSG(ID_FILE_SAVE,        i18n("Save the actual document"))
  ON_STATUS_MSG(ID_FILE_SAVE_AS,     i18n("Save the document as..."))
  ON_STATUS_MSG(ID_FILE_SAVE_ALL,    i18n("Save all changed files"))
  ON_STATUS_MSG(ID_FILE_CLOSE,       i18n("Closes the actual file"))
  ON_STATUS_MSG(ID_FILE_CLOSE_ALL,   i18n("Closes all open files"))

//  ON_STATUS_MSG(ID_FILE_PRINT,       i18n("Prints the current document"))

//  ON_STATUS_MSG(ID_FILE_CLOSE_WINDOW,i18n("Closes the current window"))

  ON_STATUS_MSG(ID_FILE_QUIT,        i18n("Exits the program"))  


  ON_STATUS_MSG(ID_EDIT_CUT,                     i18n("Cuts the selected section and puts it to the clipboard"))
  ON_STATUS_MSG(ID_EDIT_COPY,                    i18n("Copys the selected section to the clipboard"))
  ON_STATUS_MSG(ID_EDIT_PASTE,                   i18n("Pastes the clipboard contents to actual position"))
  ON_STATUS_MSG(ID_EDIT_SELECT_ALL,              i18n("Selects the whole document contents"))
  ON_STATUS_MSG(ID_EDIT_INSERT_FILE,             i18n("Inserts a file at the current position"))
  ON_STATUS_MSG(ID_EDIT_SEARCH,                  i18n("Search the file for an expression"))
  ON_STATUS_MSG(ID_EDIT_REPEAT_SEARCH,           i18n("Repeat the last search"))
  ON_STATUS_MSG(ID_EDIT_REPLACE,                 i18n("Search and replace expression"))
  ON_STATUS_MSG(ID_EDIT_GOTO_LINE,               i18n("Go to Line Number..."))


  ON_STATUS_MSG(ID_DOC_BACK,                      i18n("Switch to last browser page"))
  ON_STATUS_MSG(ID_DOC_FORWARD,                   i18n("Switch to next browser page"))
  ON_STATUS_MSG(ID_DOC_SEARCH_TEXT,              i18n("Search the selected text in the documentation"))
  ON_STATUS_MSG(ID_DOC_QT_LIBRARY,                i18n("Switch to the QT-Documentation"))
  ON_STATUS_MSG(ID_DOC_KDE_CORE_LIBRARY,          i18n("Switch to the KDE-Core-Documentation"))
  ON_STATUS_MSG(ID_DOC_KDE_GUI_LIBRARY,           i18n("Switch to the KDE-GUI-Documentation"))
  ON_STATUS_MSG(ID_DOC_KDE_KFILE_LIBRARY,          i18n("Switch to the KDE-File-Documentation"))
  ON_STATUS_MSG(ID_DOC_KDE_HTML_LIBRARY,          i18n("Switch to the KDE-Html-Documentation"))
  ON_STATUS_MSG(ID_DOC_UPDATE_KDE_DOCUMENTATION,  i18n("Update your KDE-Libs Documentation"))
  ON_STATUS_MSG(ID_DOC_CREATE_SEARCHDATABASE,    i18n("Create a search database of the current Documentation"))
  ON_STATUS_MSG(ID_DOC_PROJECT_API_DOC,           i18n("Switch to the project's API-Documentation"))
  ON_STATUS_MSG(ID_DOC_USER_MANUAL,               i18n("Switch to the project's User-Manual"))



  ON_STATUS_MSG(ID_BUILD_RUN,                     i18n("Invoke make-command and run the program"))
  ON_STATUS_MSG(ID_BUILD_MAKE,                    i18n("Invoke make-command"))
  ON_STATUS_MSG(ID_BUILD_REBUILD_ALL,             i18n("Rebuild the program"))
  ON_STATUS_MSG(ID_BUILD_CLEAN_REBUILD_ALL,       i18n("Invoke make clean and rebuild all"))
  ON_STATUS_MSG(ID_BUILD_STOP,                    i18n("Stop make immediately"))
  ON_STATUS_MSG(ID_BUILD_MAKE_PROJECT_API,        i18n("Create the Project's API with KDoc"))
  ON_STATUS_MSG(ID_BUILD_MAKE_USER_MANUAL,        i18n("Create the Project's User Manual with the sgml-file"))


  ON_STATUS_MSG(ID_PROJECT_ADD_FILE,              i18n("Add a file to the current project"))
  ON_STATUS_MSG(ID_PROJECT_ADD_FILE_NEW,          i18n("Add a new file to the project"))
  ON_STATUS_MSG(ID_PROJECT_ADD_FILE_EXIST,        i18n("Add an existing file to the project"))
  ON_STATUS_MSG(ID_PROJECT_REMOVE_FILE,           i18n("Remove file from the project"))

  ON_STATUS_MSG(ID_PROJECT_NEW_CLASS,             i18n("Create a new Class frame structure and files"))

  ON_STATUS_MSG(ID_PROJECT_FILE_PROPERTIES,       i18n("Show the current file properties"))
  ON_STATUS_MSG(ID_PROJECT_OPTIONS,               i18n("Set project and compiler options"))


  ON_STATUS_MSG(ID_OPTIONS_REFRESH,               i18n("Refresh current view"))

  ON_STATUS_MSG(ID_OPTIONS_STD_TOOLBAR,           i18n("Enables / disables the standard toolbar"))
  ON_STATUS_MSG(ID_OPTIONS_BROWSER_TOOLBAR,       i18n("Enables / disables the browser toolbar"))
  ON_STATUS_MSG(ID_OPTIONS_STATUSBAR,             i18n("Enables / disables the statusbar"))

  ON_STATUS_MSG(ID_OPTIONS_TREEVIEW,              i18n("Enables / disables the treeview"))
  ON_STATUS_MSG(ID_OPTIONS_OUTPUTVIEW,            i18n("Enables / disables the outputview"))

  ON_STATUS_MSG(ID_OPTIONS_KDEVELOP,       i18n("Set up the KDevelop environment"))



  ON_STATUS_MSG(ID_HELP_CONTENT,                  i18n("Switch to KDevelop's User Manual"))
  ON_STATUS_MSG(ID_HELP_HOMEPAGE,                 i18n("Enter the KDevelop Homepage"))
  ON_STATUS_MSG(ID_HELP_ABOUT,                    i18n("Programmer's Hall of Fame..."))

END_STATUS_MSG()


