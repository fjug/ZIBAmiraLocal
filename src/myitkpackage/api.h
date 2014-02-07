// AUTOMATICALLY GENERATED FILE.  DO NOT MODIFY.
#ifndef MYITKPACKAGE_API_EXPORT_MACRO_H
#define MYITKPACKAGE_API_EXPORT_MACRO_H

#ifdef MYITKPACKAGE_STATIC
#   define MYITKPACKAGE_API
#else
#   ifdef _WIN32
#       ifdef MYITKPACKAGE_EXPORTS
#           define MYITKPACKAGE_API __declspec(dllexport)
#       else
#           define MYITKPACKAGE_API __declspec(dllimport)
#       endif
#   else
#       define MYITKPACKAGE_API __attribute__ ((visibility("default")))
#   endif
#endif

#endif
