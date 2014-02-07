// AUTOMATICALLY GENERATED FILE.  DO NOT MODIFY.  Place custom code in custominit.h.
void mcExitClass_MyITKFilter();
void mcInitClass_MyITKFilter();


extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
void amirapackage_myitkpackage_init()
{
    static bool isInitialized = false;
    if (isInitialized)
      return;

    isInitialized = true;

    mcInitClass_MyITKFilter();

}


extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
void amirapackage_myitkpackage_finish()
{
    static bool isFinished = false;
    if (isFinished)
      return;

    isFinished = true;


    mcExitClass_MyITKFilter();
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
        amirapackage_myitkpackage_init();
        break;
    case DLL_PROCESS_DETACH:
        amirapackage_myitkpackage_finish();
        break;
    default:
        ;
    }
    return true;
}
#endif

#if defined(__GNUC__)
void __attribute__((constructor)) soconstructor_myitkpackage() {
    amirapackage_myitkpackage_init();
}

void __attribute__((destructor)) sodestructor_myitkpackage() {
    amirapackage_myitkpackage_finish();
}
#endif
