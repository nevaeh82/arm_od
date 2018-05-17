#ifndef PWTACTICMAP_GLOBAL_H
#define PWTACTICMAP_GLOBAL_H
#include <qglobal.h>

#ifdef TACTICMAP_BUILD
 #if !defined(TACTICMAP_EXPORT) && (defined(QT_DLL) || defined(QT_SHARED)) && !defined(QT_PLUGIN)
   // lib being compiled shared
  #define TACTICMAP_EXPORT Q_DECL_EXPORT
 #else
  // lib being embedded
  #define TACTICMAP_EXPORT
 #endif
#else
 // lib being linked against (must be shared on Window$!)
 #define TACTICMAP_EXPORT Q_DECL_IMPORT
#endif


#endif // PWGISCLIENT_H
