#pragma once

#include <coreplugin/dialogs/ioptionspage.h>

#include <QPointer>
#include <QSharedPointer>
#include <QWidget>

namespace CppInsightsPlugin {
namespace Internal {

class Settings;

namespace Ui {
class OptionsPage;
}

class SettingsPageWidget : public QWidget
{
  Q_OBJECT

public:
  explicit SettingsPageWidget(const QSharedPointer<Settings> &settings, QWidget *parent = nullptr);
  ~SettingsPageWidget();
  void restore();
  void apply();

private:
  Ui::OptionsPage *ui;
  QSharedPointer<Settings> m_settings;
};

class SettingsPage : public Core::IOptionsPage
{
  Q_OBJECT

public:
  explicit SettingsPage(const QSharedPointer<Settings> &settings, QObject *parent = nullptr);
  QWidget *widget() override;
  void apply() override;
  void finish() override;

private:
  QPointer<SettingsPageWidget> m_widget;
  QSharedPointer<Settings> m_settings;
  QStringList m_toolIds;
};
} // namespace Internal
} // namespace CppInsightsPlugin
