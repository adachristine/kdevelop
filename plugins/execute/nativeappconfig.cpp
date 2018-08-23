/*  This file is part of KDevelop
    Copyright 2009 Andreas Pakulat <apaku@gmx.de>
    Copyright 2010 Aleix Pol Gonzalez <aleixpol@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/
#include "nativeappconfig.h"

#include <interfaces/icore.h>
#include <interfaces/iprojectcontroller.h>
#include <interfaces/iruncontroller.h>
#include <interfaces/ilaunchconfiguration.h>

#include <project/projectmodel.h>

#include "nativeappjob.h"
#include <interfaces/iproject.h>
#include <project/interfaces/iprojectfilemanager.h>
#include <util/executecompositejob.h>

#include <interfaces/iplugincontroller.h>

#include "executeplugin.h"
#include "debug.h"
#include <util/kdevstringhandler.h>
#include "projecttargetscombobox.h"

#include <QIcon>
#include <QMenu>

#include <KConfigGroup>
#include <KLineEdit>
#include <KLocalizedString>
#include <KShell>


using namespace KDevelop;

QIcon NativeAppConfigPage::icon() const
{
    return QIcon::fromTheme(QStringLiteral("system-run"));
}

static KDevelop::ProjectBaseItem* itemForPath(const QStringList& path, KDevelop::ProjectModel* model)
{
    return model->itemFromIndex(model->pathToIndex(path));
}

//TODO: Make sure to auto-add the executable target to the dependencies when its used.

void NativeAppConfigPage::loadFromConfiguration(const KConfigGroup& cfg, KDevelop::IProject* project )
{
    QSignalBlocker blocker(this);
    projectTarget->setBaseItem( project ? project->projectItem() : nullptr, true);
    projectTarget->setCurrentItemPath( cfg.readEntry( ExecutePlugin::projectTargetEntry, QStringList() ) );

    QUrl exe = cfg.readEntry( ExecutePlugin::executableEntry, QUrl());
    if( !exe.isEmpty() || project ){
        executablePath->setUrl( !exe.isEmpty() ? exe : project->path().toUrl() );
    }else{
        KDevelop::IProjectController* pc = KDevelop::ICore::self()->projectController();
        if( pc ){
            executablePath->setUrl( pc->projects().isEmpty() ? QUrl() : pc->projects().at(0)->path().toUrl() );
        }
    }
    dependencies->setSuggestion(project);

    //executablePath->setFilter("application/x-executable");

    executableRadio->setChecked( true );
    if ( !cfg.readEntry( ExecutePlugin::isExecutableEntry, false ) && projectTarget->count() ){
        projectTargetRadio->setChecked( true );
    }

    arguments->setClearButtonEnabled( true );
    arguments->setText( cfg.readEntry( ExecutePlugin::argumentsEntry, "" ) );
    workingDirectory->setUrl( cfg.readEntry( ExecutePlugin::workingDirEntry, QUrl() ) );
    environment->setCurrentProfile(cfg.readEntry(ExecutePlugin::environmentProfileEntry, QString()));
    runInTerminal->setChecked( cfg.readEntry( ExecutePlugin::useTerminalEntry, false ) );
    terminal->setEditText( cfg.readEntry( ExecutePlugin::terminalEntry, terminal->itemText(0) ) );
    dependencies->setDependencies(KDevelop::stringToQVariant( cfg.readEntry( ExecutePlugin::dependencyEntry, QString() ) ).toList());

    dependencyAction->setCurrentIndex( dependencyAction->findData( cfg.readEntry( ExecutePlugin::dependencyActionEntry, "Nothing" ) ) );
}

NativeAppConfigPage::NativeAppConfigPage( QWidget* parent )
    : LaunchConfigurationPage( parent )
{
    setupUi(this);
    //Setup data info for combobox
    dependencyAction->setItemData(0, QStringLiteral("Nothing"));
    dependencyAction->setItemData(1, QStringLiteral("Build"));
    dependencyAction->setItemData(2, QStringLiteral("Install"));
    dependencyAction->setItemData(3, QStringLiteral("SudoInstall"));

    //Set workingdirectory widget to ask for directories rather than files
    workingDirectory->setMode(KFile::Directory | KFile::ExistingOnly | KFile::LocalOnly);

    configureEnvironment->setSelectionWidget(environment);

    //connect signals to changed signal
    connect( projectTarget, static_cast<void(ProjectTargetsComboBox::*)(const QString&)>(&ProjectTargetsComboBox::currentIndexChanged), this, &NativeAppConfigPage::changed );
    connect( projectTargetRadio, &QRadioButton::toggled, this, &NativeAppConfigPage::changed );
    connect( executableRadio, &QRadioButton::toggled, this, &NativeAppConfigPage::changed );
    connect( executablePath->lineEdit(), &KLineEdit::textEdited, this, &NativeAppConfigPage::changed );
    connect( executablePath, &KUrlRequester::urlSelected, this, &NativeAppConfigPage::changed );
    connect( arguments, &QLineEdit::textEdited, this, &NativeAppConfigPage::changed );
    connect( workingDirectory, &KUrlRequester::urlSelected, this, &NativeAppConfigPage::changed );
    connect( workingDirectory->lineEdit(), &KLineEdit::textEdited, this, &NativeAppConfigPage::changed );
    connect( environment, &EnvironmentSelectionWidget::currentProfileChanged, this, &NativeAppConfigPage::changed );
    connect( dependencyAction, static_cast<void(KComboBox::*)(int)>(&KComboBox::currentIndexChanged), this, &NativeAppConfigPage::changed );
    connect( runInTerminal, &QCheckBox::toggled, this, &NativeAppConfigPage::changed );
    connect( terminal, &KComboBox::editTextChanged, this, &NativeAppConfigPage::changed );
    connect( terminal, static_cast<void(KComboBox::*)(int)>(&KComboBox::currentIndexChanged), this, &NativeAppConfigPage::changed );
    connect( dependencyAction, static_cast<void(KComboBox::*)(int)>(&KComboBox::currentIndexChanged), this, &NativeAppConfigPage::activateDeps );
    connect( dependencies, &DependenciesWidget::changed, this, &NativeAppConfigPage::changed );
}

void NativeAppConfigPage::activateDeps( int idx )
{
    dependencies->setEnabled( dependencyAction->itemData( idx ).toString() != QLatin1String("Nothing") );
}

void NativeAppConfigPage::saveToConfiguration( KConfigGroup cfg, KDevelop::IProject* project ) const
{
    Q_UNUSED( project );
    cfg.writeEntry( ExecutePlugin::isExecutableEntry, executableRadio->isChecked() );
    cfg.writeEntry( ExecutePlugin::executableEntry, executablePath->url() );
    cfg.writeEntry( ExecutePlugin::projectTargetEntry, projectTarget->currentItemPath() );
    cfg.writeEntry( ExecutePlugin::argumentsEntry, arguments->text() );
    cfg.writeEntry( ExecutePlugin::workingDirEntry, workingDirectory->url() );
    cfg.writeEntry( ExecutePlugin::environmentProfileEntry, environment->currentProfile() );
    cfg.writeEntry( ExecutePlugin::useTerminalEntry, runInTerminal->isChecked() );
    cfg.writeEntry( ExecutePlugin::terminalEntry, terminal->currentText() );
    cfg.writeEntry( ExecutePlugin::dependencyActionEntry, dependencyAction->itemData( dependencyAction->currentIndex() ).toString() );
    QVariantList deps = dependencies->dependencies();
    cfg.writeEntry( ExecutePlugin::dependencyEntry, KDevelop::qvariantToString( QVariant( deps ) ) );
}

QString NativeAppConfigPage::title() const
{
    return i18n("Configure Native Application");
}

QList< KDevelop::LaunchConfigurationPageFactory* > NativeAppLauncher::configPages() const
{
    return QList<KDevelop::LaunchConfigurationPageFactory*>();
}

QString NativeAppLauncher::description() const
{
    return i18n("Executes Native Applications");
}

QString NativeAppLauncher::id()
{
    return QStringLiteral("nativeAppLauncher");
}

QString NativeAppLauncher::name() const
{
    return i18n("Native Application");
}

NativeAppLauncher::NativeAppLauncher()
{
}

KJob* NativeAppLauncher::start(const QString& launchMode, KDevelop::ILaunchConfiguration* cfg)
{
    Q_ASSERT(cfg);
    if( !cfg )
    {
        return nullptr;
    }
    if( launchMode == QLatin1String("execute") )
    {
        IExecutePlugin* iface = KDevelop::ICore::self()->pluginController()->pluginForExtension(QStringLiteral("org.kdevelop.IExecutePlugin"), QStringLiteral("kdevexecute"))->extension<IExecutePlugin>();
        Q_ASSERT(iface);
        KJob* depjob = iface->dependencyJob( cfg );
        QList<KJob*> l;
        if( depjob )
        {
            l << depjob;
        }
        l << new NativeAppJob( KDevelop::ICore::self()->runController(), cfg );
        return new KDevelop::ExecuteCompositeJob( KDevelop::ICore::self()->runController(), l );

    }
    qCWarning(PLUGIN_EXECUTE) << "Unknown launch mode " << launchMode << "for config:" << cfg->name();
    return nullptr;
}

QStringList NativeAppLauncher::supportedModes() const
{
    return QStringList() << QStringLiteral("execute");
}

KDevelop::LaunchConfigurationPage* NativeAppPageFactory::createWidget(QWidget* parent)
{
    return new NativeAppConfigPage( parent );
}

NativeAppPageFactory::NativeAppPageFactory()
{
}

NativeAppConfigType::NativeAppConfigType()
{
    factoryList.append( new NativeAppPageFactory() );
}

NativeAppConfigType::~NativeAppConfigType()
{
    qDeleteAll(factoryList);
    factoryList.clear();
}

QString NativeAppConfigType::name() const
{
    return i18n("Compiled Binary");
}


QList<KDevelop::LaunchConfigurationPageFactory*> NativeAppConfigType::configPages() const
{
    return factoryList;
}

QString NativeAppConfigType::id() const
{
    return ExecutePlugin::_nativeAppConfigTypeId;
}

QIcon NativeAppConfigType::icon() const
{
    return QIcon::fromTheme(QStringLiteral("application-x-executable"));
}

bool NativeAppConfigType::canLaunch ( KDevelop::ProjectBaseItem* item ) const
{
    if( item->target() && item->target()->executable() ) {
        return canLaunch( item->target()->executable()->builtUrl() );
    }
    return false;
}

bool NativeAppConfigType::canLaunch ( const QUrl& file ) const
{
    return ( file.isLocalFile() && QFileInfo( file.toLocalFile() ).isExecutable() );
}

void NativeAppConfigType::configureLaunchFromItem ( KConfigGroup cfg, KDevelop::ProjectBaseItem* item ) const
{
    cfg.writeEntry( ExecutePlugin::isExecutableEntry, false );
    KDevelop::ProjectModel* model = KDevelop::ICore::self()->projectController()->projectModel();
    cfg.writeEntry( ExecutePlugin::projectTargetEntry, model->pathFromIndex( model->indexFromItem( item ) ) );
    cfg.writeEntry( ExecutePlugin::workingDirEntry, item->executable()->builtUrl().adjusted(QUrl::RemoveFilename) );
    cfg.sync();
}

void NativeAppConfigType::configureLaunchFromCmdLineArguments ( KConfigGroup cfg, const QStringList& args ) const
{
    cfg.writeEntry( ExecutePlugin::isExecutableEntry, true );
    Q_ASSERT(QFile::exists(args.first()));
//  TODO: we probably want to flexibilize, but at least we won't be accepting wrong values anymore
    cfg.writeEntry( ExecutePlugin::executableEntry, QUrl::fromLocalFile(args.first()) );
    QStringList a(args);
    a.removeFirst();
    cfg.writeEntry( ExecutePlugin::argumentsEntry, KShell::joinArgs(a) );
    cfg.sync();
}

QList<KDevelop::ProjectTargetItem*> targetsInFolder(KDevelop::ProjectFolderItem* folder)
{
    QList<KDevelop::ProjectTargetItem*> ret;
    foreach(KDevelop::ProjectFolderItem* f, folder->folderList())
        ret += targetsInFolder(f);

    ret += folder->targetList();
    return ret;
}

bool actionLess(QAction* a, QAction* b)
{
    return a->text() < b->text();
}

bool menuLess(QMenu* a, QMenu* b)
{
    return a->title() < b->title();
}

QMenu* NativeAppConfigType::launcherSuggestions()
{
    QMenu* ret = new QMenu(i18n("Project Executables"));

    KDevelop::ProjectModel* model = KDevelop::ICore::self()->projectController()->projectModel();
    const QList<KDevelop::IProject*> projects = KDevelop::ICore::self()->projectController()->projects();

    for (KDevelop::IProject* project : projects) {
        if(project->projectFileManager()->features() & KDevelop::IProjectFileManager::Targets) {
            const QList<KDevelop::ProjectTargetItem*> targets = targetsInFolder(project->projectItem());
            QHash<KDevelop::ProjectBaseItem*, QList<QAction*> > targetsContainer;
            QMenu* projectMenu = ret->addMenu(QIcon::fromTheme(QStringLiteral("project-development")), project->name());
            for (KDevelop::ProjectTargetItem* target : targets) {
                if(target->executable()) {
                    QStringList path = model->pathFromIndex(target->index());
                    if(!path.isEmpty()){
                        QAction* act = new QAction(projectMenu);
                        act->setData(KDevelop::joinWithEscaping(path, QLatin1Char('/'), QLatin1Char('\\')));
                        act->setProperty("name", target->text());
                        path.removeFirst();
                        act->setText(path.join(QLatin1Char('/')));
                        act->setIcon(QIcon::fromTheme(QStringLiteral("system-run")));
                        connect(act, &QAction::triggered, this, &NativeAppConfigType::suggestionTriggered);
                        targetsContainer[target->parent()].append(act);
                    }
                }
            }

            QList<QAction*> separateActions;
            QList<QMenu*> submenus;
            foreach(KDevelop::ProjectBaseItem* folder, targetsContainer.keys()) {
                QList<QAction*> actions = targetsContainer.value(folder);
                if(actions.size()==1 || !folder->parent()) {
                    separateActions += actions.first();
                } else {
                    foreach(QAction* a, actions) {
                        a->setText(a->property("name").toString());
                    }
                    QStringList path = model->pathFromIndex(folder->index());
                    path.removeFirst();
                    QMenu* submenu = new QMenu(path.join(QLatin1Char('/')), projectMenu);
                    std::sort(actions.begin(), actions.end(), actionLess);
                    submenu->addActions(actions);
                    submenus += submenu;
                }
            }
            std::sort(separateActions.begin(), separateActions.end(), actionLess);
            std::sort(submenus.begin(), submenus.end(), menuLess);
            foreach(QMenu* m, submenus)
                projectMenu->addMenu(m);
            projectMenu->addActions(separateActions);

            projectMenu->setEnabled(!projectMenu->isEmpty());
        }
    }

    return ret;
}

void NativeAppConfigType::suggestionTriggered()
{
    QAction* action = qobject_cast<QAction*>(sender());
    KDevelop::ProjectModel* model = KDevelop::ICore::self()->projectController()->projectModel();
    KDevelop::ProjectTargetItem* pitem = dynamic_cast<KDevelop::ProjectTargetItem*>(itemForPath(KDevelop::splitWithEscaping(action->data().toString(), QLatin1Char('/'), QLatin1Char('\\')), model));
    if(pitem) {
        QPair<QString,QString> launcher = qMakePair( launchers().at( 0 )->supportedModes().at(0), launchers().at( 0 )->id() );
        KDevelop::IProject* p = pitem->project();

        KDevelop::ILaunchConfiguration* config = KDevelop::ICore::self()->runController()->createLaunchConfiguration(this, launcher, p, pitem->text());
        KConfigGroup cfg = config->config();

        QStringList splitPath = model->pathFromIndex(pitem->index());
//         QString path = KDevelop::joinWithEscaping(splitPath,'/','\\');
        cfg.writeEntry( ExecutePlugin::projectTargetEntry, splitPath );
        cfg.writeEntry( ExecutePlugin::dependencyEntry, KDevelop::qvariantToString( QVariantList() << splitPath ) );
        cfg.writeEntry( ExecutePlugin::dependencyActionEntry, "Build" );
        cfg.sync();

        emit signalAddLaunchConfiguration(config);
    }
}

