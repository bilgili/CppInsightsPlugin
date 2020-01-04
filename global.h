// Copyright (c) 2019 Ahmet Bilgili
// Licensed under the MIT licence

#pragma once

#include <QtGlobal>

#if defined(MYPLUGIN_LIBRARY)
#  define CPPINSIGHTSPLUGIN_EXPORT Q_DECL_EXPORT
#else
#  define CPPINSIGHTSPLUGIN_EXPORT Q_DECL_IMPORT
#endif

