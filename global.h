#pragma once

#include <QtGlobal>

#if defined(MYPLUGIN_LIBRARY)
#  define CPPINSIGHTSPLUGIN_EXPORT Q_DECL_EXPORT
#else
#  define CPPINSIGHTSPLUGIN_EXPORT Q_DECL_IMPORT
#endif

