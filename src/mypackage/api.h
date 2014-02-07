// AUTOMATICALLY GENERATED FILE.  DO NOT MODIFY.
#ifndef MYPACKAGE_API_EXPORT_MACRO_H
#define MYPACKAGE_API_EXPORT_MACRO_H

#ifdef MYPACKAGE_STATIC
#   define MYPACKAGE_API
#else
#   ifdef _WIN32
#       ifdef MYPACKAGE_EXPORTS
#           define MYPACKAGE_API __declspec(dllexport)
#       else
#           define MYPACKAGE_API __declspec(dllimport)
#       endif
#   else
#       define MYPACKAGE_API __attribute__ ((visibility("default")))
#   endif
#endif

#endif
