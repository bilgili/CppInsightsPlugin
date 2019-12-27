#pragma once

#include "global.h"

#include <extensionsystem/iplugin.h>

#include <QSharedPointer>

namespace CppInsightsPlugin {
namespace Internal {

class Settings;

class Plugin : public ExtensionSystem::IPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "CppInsightsPlugin.json")

public:
  Plugin() = default;

  bool initialize(const QStringList &arguments, QString *errorString) override;
  void extensionsInitialized() override{};
  ShutdownFlag aboutToShutdown() override;

private:
  void triggerAction();
  QSharedPointer<Settings> m_settings;
};

} // namespace Internal
} // namespace CppInsightsPlugin
