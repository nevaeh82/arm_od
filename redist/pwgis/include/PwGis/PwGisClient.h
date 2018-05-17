#ifndef PWGISCLIENT_H
#define PWGISCLIENT_H
#include <qglobal.h>

#ifdef GISCLIENT_BUILD
 #if !defined(GISCLIENT_EXPORT) && (defined(QT_DLL) || defined(QT_SHARED)) && !defined(QT_PLUGIN)
   // lib being compiled shared
  #define GISCLIENT_EXPORT Q_DECL_EXPORT
 #else
  // lib being embedded
  #define GISCLIENT_EXPORT
 #endif
#else
 // lib being linked against (must be shared on Window$!)
 #define GISCLIENT_EXPORT Q_DECL_IMPORT
#endif


#endif // PWGISCLIENT_H
