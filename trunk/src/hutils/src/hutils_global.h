#ifndef HUTILS_GLOBAL_H
#define HUTILS_GLOBAL_H

#include <qglobal.h>

#ifndef FML_STATIC_LINK
#ifdef HUTILS_LIB
# define HUTILS_EXPORT Q_DECL_EXPORT
#else
# define HUTILS_EXPORT Q_DECL_IMPORT
#endif
#else
# define HUTILS_EXPORT
#endif

#endif // HUTILS_GLOBAL_H
