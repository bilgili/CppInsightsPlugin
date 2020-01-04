// Copyright (c) 2019 Ahmet Bilgili
// Licensed under the MIT licence

#pragma once

#include <utils/mimetypes/mimetype.h>

#include <QList>

namespace CppInsightsPlugin
{
namespace Internal
{
class Settings
{
public:
  explicit Settings();
  void read();
  void save();

  QString cppInsightsTool() const;
  void setCppInsightsTool(const QString &cppInsightsTool);

  QList<Utils::MimeType> cppInsightsMimes() const;
  QString cppInsightsMimesAsString() const;
  void setCppInsightsMimes(const QList<Utils::MimeType> &cppInsightsMime);
  void setCppInsightsMimes(const QString &mimeTypes);

private:
  QString m_cppInsightsTool;
  QList<Utils::MimeType> m_mimeTypes;
};
} // namespace Internal
} // namespace Beautifier
