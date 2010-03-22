#ifndef HMATHBS_GLOBAL_H
#define HMATHBS_GLOBAL_H

#include <qglobal.h>

#ifndef FML_STATIC_LINK
#ifdef HMATHBS_LIB
# define MATHBASE_API Q_DECL_EXPORT
#else
# define MATHBASE_API Q_DECL_IMPORT
#endif
#else
# define MATHBASE_API
#endif

#endif // HMATHBS_GLOBAL_H
