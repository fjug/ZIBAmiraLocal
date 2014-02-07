///////////////////////////////////////////////////////////////////////
//
// Example of a compute module (version 1)
//
///////////////////////////////////////////////////////////////////////
#ifndef MY_COMPUTE_THRESHOLD1_H
#define MY_COMPUTE_THRESHOLD1_H

#include <hxcore/HxCompModule.h>      // include declaration of base class
#include <hxcore/HxPortFloatTextN.h>  // provides float text input
#include <mypackage/api.h>  // storage-class specification

class MYPACKAGE_API MyComputeThreshold1 : public HxCompModule
{
    HX_HEADER(MyComputeThreshold1);  // required for all base classes

  public:
    // Every module must have a default constructor.
    MyComputeThreshold1();

    // This virtual method will be called when the port changes.
    virtual void compute();

    // A port providing float text input fields.
    HxPortFloatTextN portRange;
};

#endif
