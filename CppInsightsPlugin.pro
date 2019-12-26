DEFINES += CPPINSIGHTS_PLUGIN_LIBRARY

SOURCES +=         \
  plugin.cpp \
  settings.cpp \
  settingspage.cpp

HEADERS +=                         \
  constants.h \
  global.h \
  plugin.h \
  settings.h \
  settingspage.h

# Qt Creator linking

## set the QTC_SOURCE environment variable to override the setting here
QTCREATOR_SOURCES = $$QTC_SOURCE

## set the QTC_BUILD environment variable to override the setting here
IDE_BUILD_TREE = $$QTC_BUILD

## set the QTC_LIB_BASENAME environment variable to override the setting here
## this variable points to the library installation path, relative to IDE_BUILD_TREE,
## so that $$IDE_BUILD_TREE/$$IDE_LIBRARY_BASENAME/qtcreator will be used by
## qtcreatorplugin.pri automatically as the qtcreator library path
IDE_LIBRARY_BASENAME = $$(QTC_LIB_BASENAME)

## uncomment to build plugin into user config directory
## <localappdata>/plugins/<ideversion>
##    where <localappdata> is e.g.
##    "%LOCALAPPDATA%QtProjectqtcreator" on Windows Vista and later
##    "$XDG_DATA_HOME/data/QtProject/qtcreator" or "~/.local/share/data/QtProject/qtcreator" on Linux
##    "~/Library/Application Support/QtProject/Qt Creator" on OS X
# USE_USER_DESTDIR = yes

###### If the plugin can be depended upon by other plugins, this code needs to be outsourced to
###### <dirname>_dependencies.pri, where <dirname> is the name of the directory containing the
###### plugin's sources.


QTC_PLUGIN_NAME = CppInsightsPlugin
QTC_PLUGIN_DEPENDS += coreplugin projectexplorer

include($$QTCREATOR_SOURCES/src/qtcreatorplugin.pri)

FORMS += settingspage.ui

DISTFILES += CppInsightsPlugin.json.in
