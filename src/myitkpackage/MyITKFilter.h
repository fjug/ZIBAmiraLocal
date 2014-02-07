/////////////////////////////////////////////////////////////////
//
// Example of an ITK compute module
//
/////////////////////////////////////////////////////////////////

#ifndef MYITKFILTER_H
#define MYITKFILTER_H

#include <hxcore/HxCompModule.h>
#include <hxcore/HxPortIntSlider.h>
#include <hxcore/HxPortDoIt.h>

#include "api.h"  // storage-class specification

class MYITKPACKAGE_API MyITKFilter : public HxCompModule
{
    HX_HEADER(MyITKFilter);

  public:

    MyITKFilter();

    HxPortIntSlider   portKernelSize;
    HxPortDoIt        portDoIt;

    virtual void compute();
};

#endif // MYITKFILTER_H
