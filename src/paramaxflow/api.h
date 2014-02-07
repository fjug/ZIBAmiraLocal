// AUTOMATICALLY GENERATED FILE.  DO NOT MODIFY.
#ifndef PARAMAXFLOW_API_EXPORT_MACRO_H
#define PARAMAXFLOW_API_EXPORT_MACRO_H

#ifdef PARAMAXFLOW_STATIC
#   define PARAMAXFLOW_API
#else
#   ifdef _WIN32
#       ifdef PARAMAXFLOW_EXPORTS
#           define PARAMAXFLOW_API __declspec(dllexport)
#       else
#           define PARAMAXFLOW_API __declspec(dllimport)
#       endif
#   else
#       define PARAMAXFLOW_API __attribute__ ((visibility("default")))
#   endif
#endif

#endif
