// AUTOMATICALLY GENERATED FILE.  DO NOT MODIFY.  Place custom code in custominit.h.
void mcExitClass_MyVRDemo();
void mcInitClass_MyVRDemo();
void mcExitClass_MyPlotAreaPerSlice();
void mcInitClass_MyPlotAreaPerSlice();
void mcExitClass_MyDynamicColormap();
void mcInitClass_MyDynamicColormap();
void mcExitClass_MyDisplayVertices3();
void mcInitClass_MyDisplayVertices3();
void mcExitClass_MyDisplayVertices2();
void mcInitClass_MyDisplayVertices2();
void mcExitClass_MyDisplayVertices1();
void mcInitClass_MyDisplayVertices1();
void mcExitClass_MyComputeThreshold3();
void mcInitClass_MyComputeThreshold3();
void mcExitClass_MyComputeThreshold2();
void mcInitClass_MyComputeThreshold2();
void mcExitClass_MyComputeThreshold1();
void mcInitClass_MyComputeThreshold1();
void mcExitClass_MyAnimateColormap();
void mcInitClass_MyAnimateColormap();


extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
void amirapackage_mypackage_init()
{
    static bool isInitialized = false;
    if (isInitialized)
      return;

    isInitialized = true;

    mcInitClass_MyVRDemo();
    mcInitClass_MyPlotAreaPerSlice();
    mcInitClass_MyDynamicColormap();
    mcInitClass_MyDisplayVertices3();
    mcInitClass_MyDisplayVertices2();
    mcInitClass_MyDisplayVertices1();
    mcInitClass_MyComputeThreshold3();
    mcInitClass_MyComputeThreshold2();
    mcInitClass_MyComputeThreshold1();
    mcInitClass_MyAnimateColormap();

}


extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
void amirapackage_mypackage_finish()
{
    static bool isFinished = false;
    if (isFinished)
      return;

    isFinished = true;


    mcExitClass_MyAnimateColormap();
    mcExitClass_MyComputeThreshold1();
    mcExitClass_MyComputeThreshold2();
    mcExitClass_MyComputeThreshold3();
    mcExitClass_MyDisplayVertices1();
    mcExitClass_MyDisplayVertices2();
    mcExitClass_MyDisplayVertices3();
    mcExitClass_MyDynamicColormap();
    mcExitClass_MyPlotAreaPerSlice();
    mcExitClass_MyVRDemo();
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
        amirapackage_mypackage_init();
        break;
    case DLL_PROCESS_DETACH:
        amirapackage_mypackage_finish();
        break;
    default:
        ;
    }
    return true;
}
#endif

#if defined(__GNUC__)
void __attribute__((constructor)) soconstructor_mypackage() {
    amirapackage_mypackage_init();
}

void __attribute__((destructor)) sodestructor_mypackage() {
    amirapackage_mypackage_finish();
}
#endif
