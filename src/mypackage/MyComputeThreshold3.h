///////////////////////////////////////////////////////////////////////
//
// Example of a compute module (version 3)
//
///////////////////////////////////////////////////////////////////////
#ifndef MY_COMPUTE_THRESHOLD3_H
#define MY_COMPUTE_THRESHOLD3_H

#include <hxcore/HxCompModule.h>      // include declaration of base class
#include <hxcore/HxPortDoIt.h>        // provides do it button
#include <hxcore/HxPortFloatTextN.h>  // provides float text input
#include <mypackage/api.h>  // storage-class specification

class MYPACKAGE_API MyComputeThreshold3 : public HxCompModule
{
    HX_HEADER(MyComputeThreshold3);

  public:
    // Every module must have a default constructor.
    MyComputeThreshold3();

    // This will be called when an input port changes.
    virtual void compute();

    // A port providing float text input fields.
    HxPortFloatTextN portRange;

    // Start computation when this button is clicked.
    HxPortDoIt portDoIt;
};

#endif
