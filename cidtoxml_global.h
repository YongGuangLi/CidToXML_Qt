#ifndef CIDTOXML_GLOBAL_H
#define CIDTOXML_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CIDTOXML_LIBRARY)
#  define CIDTOXMLSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CIDTOXMLSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // CIDTOXML_GLOBAL_H
