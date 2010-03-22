#ifndef HMATHAST_GLOBAL_H
#define HMATHAST_GLOBAL_H

#include <qglobal.h>

#ifndef FML_STATIC_LINK
#ifdef HMATHAST_LIB
# define __HMATHLNGDLL__ Q_DECL_EXPORT
#else
# define __HMATHLNGDLL__ Q_DECL_IMPORT
#endif
#else
# define __HMATHLNGDLL__
#endif

#endif // HMATHAST_GLOBAL_H
