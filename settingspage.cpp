#include "settingspage.h"
#include "constants.h"
#include "settings.h"

#include "ui_settingspage.h"

#include <coreplugin/icore.h>
#include <cpptools/cpptoolsconstants.h>
#include <utils/pathchooser.h>

#include <QTextStream>

namespace CppInsightsPlugin
{
namespace Internal
{
SettingsPageWidget::SettingsPageWidget(
    const QSharedPointer<Settings> &settings, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::OptionsPage)
    , m_settings(settings) {
  ui->setupUi(this);
  ui->command->setExpectedKind(Utils::PathChooser::ExistingCommand);
  ui->command->setCommandVersionArguments({"--version"});
  ui->command->setPromptDialogTitle(tr("CppInsights"));
}

SettingsPageWidget::~SettingsPageWidget() {
  delete ui;
}

void SettingsPageWidget::restore()
{
  ui->command->setPath(m_settings->cppInsightsTool());
  ui->mime->setText(m_settings->cppInsightsMimesAsString());
}

void SettingsPageWidget::apply()
{
  m_settings->setCppInsightsTool(ui->command->path());
  m_settings->setCppInsightsMimes(ui->mime->text());
  m_settings->save();
}

SettingsPage::SettingsPage(const QSharedPointer<Settings> &settings, QObject *parent)
    : IOptionsPage(parent)
    , m_settings(settings)
{

  setId(Constants::SETTINGS_GROUP);
  setDisplayName("CppInsights");
  setCategory(CppTools::Constants::CPP_SETTINGS_CATEGORY);
  setDisplayCategory(QCoreApplication::translate("CppTools", CppTools::Constants::CPP_SETTINGS_NAME));
}

QWidget *SettingsPage::widget()
{
  m_settings->read();

  if (!m_widget)
    m_widget = new SettingsPageWidget(m_settings);
  m_widget->restore();

  return m_widget;
}

void SettingsPage::apply()
{
  if (m_widget)
    m_widget->apply();
}

void SettingsPage::finish()
{
}
} // namespace Internal
} // namespace CppInsightsPlugin
