// AUTOMATICALLY GENERATED FILE.  DO NOT MODIFY.  Place custom code in custominit.h.
void mcExitClass_Paramaxflow();
void mcInitClass_Paramaxflow();
void mcExitClass_ParamaxflowWithMap();
void mcInitClass_ParamaxflowWithMap();


extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
void amirapackage_paramaxflow_init()
{
    static bool isInitialized = false;
    if (isInitialized)
      return;

    isInitialized = true;

    mcInitClass_Paramaxflow();
    mcInitClass_ParamaxflowWithMap();

}


extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
void amirapackage_paramaxflow_finish()
{
    static bool isFinished = false;
    if (isFinished)
      return;

    isFinished = true;


    mcExitClass_ParamaxflowWithMap();
    mcExitClass_Paramaxflow();
}

#if defined(_WIN32)
#  include <windows.h>


BOOL WINAPI DllMain(
    __in  HINSTANCE hinstDLL,
    __in  DWORD fdwReason,
    __in  LPVOID lpvReserved
    )
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        amirapackage_paramaxflow_init();
        break;
    case DLL_PROCESS_DETACH:
        amirapackage_paramaxflow_finish();
        break;
    default:
        ;
    }
    return true;
}
#endif

#if defined(__GNUC__)
void __attribute__((constructor)) soconstructor_paramaxflow() {
    amirapackage_paramaxflow_init();
}

void __attribute__((destructor)) sodestructor_paramaxflow() {
    amirapackage_paramaxflow_finish();
}
#endif
