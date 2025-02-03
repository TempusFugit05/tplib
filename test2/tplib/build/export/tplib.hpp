
#ifndef TPLIB_EXPORT_H
#define TPLIB_EXPORT_H

#ifdef TPLIB_STATIC_DEFINE
#  define TPLIB_EXPORT
#  define TPLIB_NO_EXPORT
#else
#  ifndef TPLIB_EXPORT
#    ifdef tplib_EXPORTS
        /* We are building this library */
#      define TPLIB_EXPORT 
#    else
        /* We are using this library */
#      define TPLIB_EXPORT 
#    endif
#  endif

#  ifndef TPLIB_NO_EXPORT
#    define TPLIB_NO_EXPORT 
#  endif
#endif

#ifndef TPLIB_DEPRECATED
#  define TPLIB_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef TPLIB_DEPRECATED_EXPORT
#  define TPLIB_DEPRECATED_EXPORT TPLIB_EXPORT TPLIB_DEPRECATED
#endif

#ifndef TPLIB_DEPRECATED_NO_EXPORT
#  define TPLIB_DEPRECATED_NO_EXPORT TPLIB_NO_EXPORT TPLIB_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef TPLIB_NO_DEPRECATED
#    define TPLIB_NO_DEPRECATED
#  endif
#endif

#endif /* TPLIB_EXPORT_H */
