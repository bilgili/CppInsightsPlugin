#include "plugin.h"
#include "constants.h"
#include "settingspage.h"
#include "settings.h"

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/messagemanager.h>

#include <projectexplorer/session.h>
#include <projectexplorer/project.h>
#include <projectexplorer/target.h>
#include <projectexplorer/buildinfo.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/buildmanager.h>
#include <projectexplorer/projecttree.h>

#include <resourceeditor/resourcenode.h>

#include <texteditor/texteditor.h>
#include <texteditor/textdocument.h>

#include <utils/mimetypes/mimedatabase.h>

#include <QAction>
#include <QMessageBox>
#include <QMainWindow>
#include <QMenu>
#include <QProcess>
#include <QStandardPaths>
#include <QDir>
#include <QTemporaryFile>

namespace
{
bool isCppSightsEnabled(QSharedPointer<CppInsightsPlugin::Internal::Settings> settings) {

  if(settings->cppInsightsTool().isEmpty()) return false;

  ProjectExplorer::Project *project = ProjectExplorer::SessionManager::startupProject();
  if (!project)
    return false;

  ProjectExplorer::Target *target = project->activeTarget();
  if (!target)
    return false;

  QDir buildBase;
  if (auto buildConfig = target->activeBuildConfiguration()) {
    buildBase = QDir(buildConfig->buildDirectory().toString());
  }

  if (buildBase.isEmpty())
    return false;

  const QFileInfo buildCommandsFile(buildBase.path() + "/compile_commands.json");
  if (!buildCommandsFile.exists()) {
    return false;
  }

  const auto *node = ProjectExplorer::ProjectTree::currentNode();
  const auto *fileNode = dynamic_cast<const ProjectExplorer::FileNode *>(node);
  if (!fileNode) {
    return false;
  }

  return true;
}
}

namespace CppInsightsPlugin {
namespace Internal {

bool Plugin::initialize(const QStringList &arguments, QString *errorString)
{
  Q_UNUSED(arguments)
  Q_UNUSED(errorString)

  auto action = new QAction(tr("CPP Insights compile"), this);
  Core::Command *cmd = Core::ActionManager::registerAction(action, Constants::ACTION_ID,
                                                           Core::Context(Core::Constants::C_GLOBAL));
  connect(action, &QAction::triggered, this, &Plugin::triggerAction);

  Core::ActionContainer *menu = Core::ActionManager::createMenu(Constants::MENU_ID);
  menu->menu()->setTitle(tr("CppInsights"));

  Core::Context projectTreeContext(ProjectExplorer::Constants::C_PROJECT_TREE);
  menu = Core::ActionManager::actionContainer(ProjectExplorer::Constants::M_FILECONTEXT);
  menu->addAction(cmd);

  m_settings.reset(new Settings);

  connect(ProjectExplorer::ProjectTree::instance(),
          &ProjectExplorer::ProjectTree::currentNodeChanged,
          [=]() {
            if (!isCppSightsEnabled(m_settings)) {
              return;
            }

            const auto *node = ProjectExplorer::ProjectTree::currentNode();
            const auto *fileNode = dynamic_cast<const ProjectExplorer::FileNode *>(node);

            const auto mimeType = Utils::mimeTypeForFile(fileNode->filePath().toString());
            const bool isMimeTypeSupported = m_settings->cppInsightsMimes().indexOf(mimeType) != -1;
            action->setVisible(isMimeTypeSupported);
            action->setEnabled(isMimeTypeSupported);
          });


  new SettingsPage(m_settings, this);
  return true;
}


ExtensionSystem::IPlugin::ShutdownFlag Plugin::aboutToShutdown() { return SynchronousShutdown; }

void Plugin::triggerAction() {
  const auto *project = ProjectExplorer::SessionManager::startupProject();
  const auto *target = project->activeTarget();
  const auto buildConfig = target->activeBuildConfiguration();
  const auto buildBase = QDir(buildConfig->buildDirectory().toString());

  const QString folder = Constants::SETTINGS_GROUP;
  const QDir cppInsightsFolder = QDir(buildBase.path() + "/" + folder);
  if (!cppInsightsFolder.exists()) {
    if (!buildBase.mkdir(folder)) {
      Core::MessageManager::write("Error creating the cppinsights folder in build dir");
    }
  }

  const auto *node = ProjectExplorer::ProjectTree::currentNode();
  const auto *fileNode = dynamic_cast<const ProjectExplorer::FileNode *>(node);
  if (!fileNode) {
    return;
  }

  const auto path = fileNode->filePath();
  const QString temporaryFile = cppInsightsFolder.path() + "/" + path.fileName();

  QProcess *process = new QProcess(this);
  connect(process,
          static_cast<void (QProcess::*)(QProcess::ProcessError)>(&QProcess::errorOccurred),
          [=](QProcess::ProcessError error) {
            if (error == QProcess::ProcessError::FailedToStart) {
              Core::MessageManager::write("Cannot start cpp insights");
            } else {
              Core::MessageManager::write("Unknown error executing cppinsights");
            }
          });

  connect(process,
          static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),

          [=](int, QProcess::ExitStatus) {
            if (!process->readAllStandardError().isEmpty()) {
              const QString error = QString::fromStdString(
                  process->readAllStandardError().toStdString());
              Core::MessageManager::write(error);
            }

            if (QFileInfo(temporaryFile).size() == 0) {
              Core::MessageManager::write("No output from CPP insights");
              return;
            };


          Core::MessageManager::write("Insights generated");
          Core::EditorManager::openEditor(temporaryFile);
        });


  QStringList arguments;
  arguments << node->filePath().toString() << "-p" << buildBase.path();
  process->setStandardOutputFile(temporaryFile);
  process->start(m_settings->cppInsightsTool(), arguments);
  Core::MessageManager::write("Starting cpp insights : "
                              + m_settings->cppInsightsTool()
                              + " " + arguments.join(" "));

}
} // namespace Internal
} // namespace CppInsightsPlugin
