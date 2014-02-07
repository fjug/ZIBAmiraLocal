// AUTOMATICALLY GENERATED FILE.  DO NOT MODIFY.
#ifndef HXWORM_API_EXPORT_MACRO_H
#define HXWORM_API_EXPORT_MACRO_H

#ifdef HXWORM_STATIC
#   define HXWORM_API
#else
#   ifdef _WIN32
#       ifdef HXWORM_EXPORTS
#           define HXWORM_API __declspec(dllexport)
#       else
#           define HXWORM_API __declspec(dllimport)
#       endif
#   else
#       define HXWORM_API __attribute__ ((visibility("default")))
#   endif
#endif

#endif
