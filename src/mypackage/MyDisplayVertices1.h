///////////////////////////////////////////////////////////////////////
//
// Example of a display module
//
///////////////////////////////////////////////////////////////////////
#ifndef MY_DISPLAY_VERTICES1_H
#define MY_DISPLAY_VERTICES1_H

#include <mclib/McHandle.h>                // smart pointer template class
#include <hxcore/HxModule.h>          // include declaration of base class
#include <hxcore/HxPortIntSlider.h>   // provides integer slider
#include <mypackage/api.h>  // storage-class specification

#include <Inventor/nodes/SoSeparator.h>

class MYPACKAGE_API MyDisplayVertices1 : public HxModule
{
    HX_HEADER(MyDisplayVertices1);

  public:
    // Constructor.
    MyDisplayVertices1();

    // Destructor.
    ~MyDisplayVertices1();

    // Input parameter. 
    HxPortIntSlider portNumTriangles;

    // This is called when an input port changes.
    virtual void compute();

 protected:
    McHandle<SoSeparator> scene;
};

#endif
