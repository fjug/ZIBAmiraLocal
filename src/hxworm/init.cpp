// AUTOMATICALLY GENERATED FILE.  DO NOT MODIFY.  Place custom code in custominit.h.
void mcExitClass_HxWormAtlasReg();
void mcInitClass_HxWormAtlasReg();
void mcExitClass_HxStandardizeWorm();
void mcInitClass_HxStandardizeWorm();
void mcExitClass_HxSelectEllipses();
void mcInitClass_HxSelectEllipses();
void mcExitClass_HxSegmentNuclei();
void mcInitClass_HxSegmentNuclei();
void mcExitClass_HxNucleiHypotheses();
void mcInitClass_HxNucleiHypotheses();
void mcExitClass_HxEditNuclei();
void mcInitClass_HxEditNuclei();
void mcExitClass_HxAlignWorms();
void mcInitClass_HxAlignWorms();


extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
void amirapackage_hxworm_init()
{
    static bool isInitialized = false;
    if (isInitialized)
      return;

    isInitialized = true;

    mcInitClass_HxWormAtlasReg();
    mcInitClass_HxStandardizeWorm();
    mcInitClass_HxSelectEllipses();
    mcInitClass_HxSegmentNuclei();
    mcInitClass_HxNucleiHypotheses();
    mcInitClass_HxEditNuclei();
    mcInitClass_HxAlignWorms();

}


extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
void amirapackage_hxworm_finish()
{
    static bool isFinished = false;
    if (isFinished)
      return;

    isFinished = true;


    mcExitClass_HxAlignWorms();
    mcExitClass_HxEditNuclei();
    mcExitClass_HxNucleiHypotheses();
    mcExitClass_HxSegmentNuclei();
    mcExitClass_HxSelectEllipses();
    mcExitClass_HxStandardizeWorm();
    mcExitClass_HxWormAtlasReg();
}

#if defined(_WIN32)
#  include <windows.h>
#ifdef _OPENMP
    #include <omp.h>
#endif

BOOL WINAPI DllMain(
    __in  HINSTANCE hinstDLL,
    __in  DWORD fdwReason,
    __in  LPVOID lpvReserved
    )
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        amirapackage_hxworm_init();
        break;
    case DLL_PROCESS_DETACH:
        amirapackage_hxworm_finish();
        break;
    default:
        ;
    }
    return true;
}
#endif

#if defined(__GNUC__)
void __attribute__((constructor)) soconstructor_hxworm() {
    amirapackage_hxworm_init();
}

void __attribute__((destructor)) sodestructor_hxworm() {
    amirapackage_hxworm_finish();
}
#endif
