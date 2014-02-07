///////////////////////////////////////////////////////////////////////
//
// Example of a compute module (version 2)
//
///////////////////////////////////////////////////////////////////////
#ifndef MY_COMPUTE_THRESHOLD2_H
#define MY_COMPUTE_THRESHOLD2_H

#include <hxcore/HxCompModule.h>      // include declaration of base class
#include <hxcore/HxPortFloatTextN.h>  // provides float text input
#include <mypackage/api.h>  // storage-class specification

class MYPACKAGE_API MyComputeThreshold2 : public HxCompModule
{
    HX_HEADER(MyComputeThreshold2);

  public:
    // Every module must have a default constructor.
    MyComputeThreshold2();

    // This virtual method will be called when the port changes.
    virtual void compute();

    // A port providing float text input fields.
    HxPortFloatTextN portRange;
};

#endif
