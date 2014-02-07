// AUTOMATICALLY GENERATED FILE.  DO NOT MODIFY.
#ifndef GRAPHMATCHING_API_EXPORT_MACRO_H
#define GRAPHMATCHING_API_EXPORT_MACRO_H

#ifdef GRAPHMATCHING_STATIC
#   define GRAPHMATCHING_API
#else
#   ifdef _WIN32
#       ifdef GRAPHMATCHING_EXPORTS
#           define GRAPHMATCHING_API __declspec(dllexport)
#       else
#           define GRAPHMATCHING_API __declspec(dllimport)
#       endif
#   else
#       define GRAPHMATCHING_API __attribute__ ((visibility("default")))
#   endif
#endif

#endif
