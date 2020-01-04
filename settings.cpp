// Copyright (c) 2019 Ahmet Bilgili
// Licensed under the MIT licence

#include "settings.h"

#include "constants.h"

#include <coreplugin/icore.h>
#include <utils/algorithm.h>
#include <utils/genericconstants.h>
#include <utils/mimetypes/mimedatabase.h>

namespace CppInsightsPlugin {
namespace Internal {

namespace {
const char CPPINSIGHTS_TOOL[] = "cppInsightsTool";
const char CPPINSIGHTS_MIME[] = "cppInsightsMime";
}

Settings::Settings()
{
  read();
}

void Settings::read()
{
  QSettings *s = Core::ICore::settings();
  s->beginGroup(Constants::SETTINGS_GROUP);
  s->beginGroup(Constants::OPTIONS_GROUP);
  m_cppInsightsTool = s->value(CPPINSIGHTS_TOOL, QString()).toString();
  setCppInsightsMimes(s->value(CPPINSIGHTS_MIME, "text/x-c++src").toString());
  s->endGroup();
  s->endGroup();
}

void Settings::save()
{
  QSettings *s = Core::ICore::settings();
  s->beginGroup(Constants::SETTINGS_GROUP);
  s->beginGroup(Constants::OPTIONS_GROUP);
  s->setValue(CPPINSIGHTS_TOOL, m_cppInsightsTool);
  s->setValue(CPPINSIGHTS_MIME, cppInsightsMimesAsString());
  s->endGroup();
  s->endGroup();
}

QString Settings::cppInsightsTool() const
{
  return m_cppInsightsTool;
}

void Settings::setCppInsightsTool(const QString &cppInsightsTool)
{
  m_cppInsightsTool = cppInsightsTool;
}

QList<Utils::MimeType> Settings::cppInsightsMimes() const
{
  return m_mimeTypes;
}

QString Settings::cppInsightsMimesAsString() const
{
  return Utils::transform(m_mimeTypes, &Utils::MimeType::name).join("; ");
}

void Settings::setCppInsightsMimes(const QList<Utils::MimeType> &cppInsightsMimes)
{
  m_mimeTypes = cppInsightsMimes;
}

void Settings::setCppInsightsMimes(const QString& mimeTypes)
{
  const QStringList stringTypes = mimeTypes.split(';');
  QList<Utils::MimeType> types;
  types.reserve(stringTypes.count());
  for (QString t : stringTypes) {
    t = t.trimmed();
    const Utils::MimeType mime = Utils::mimeTypeForName(t);
    if (mime.isValid())
      types << mime;
  }
  setCppInsightsMimes(types);
}
} // namespace Internal
} // namespace CppInsightsPlugin
