///////////////////////////////////////////////////////////////////////
//
// Example of a display module (version 2)
//
///////////////////////////////////////////////////////////////////////
#ifndef MY_DISPLAY_VERTICES2_H
#define MY_DISPLAY_VERTICES2_H

#include <mclib/McHandle.h>                // smart pointer template class
#include <hxcore/HxModule.h>          // include declaration of base class
#include <hxcore/HxPortIntSlider.h>   // provides integer slider
#include <hxcolor/HxPortColormap.h>  // connection to a colormap
#include <mypackage/api.h>  // storage-class specification

#include <Inventor/nodes/SoSeparator.h>

class MYPACKAGE_API MyDisplayVertices2 : public HxModule
{
    HX_HEADER(MyDisplayVertices2);

  public:
    // Constructor.
    MyDisplayVertices2();

    // Destructor.
    ~MyDisplayVertices2();

    // Connection to a colormap.
    HxPortColormap portColormap;

    // Input parameter. 
    HxPortIntSlider portNumTriangles;

    // This is called when an input port changes.
    virtual void compute();

    // Tcl command interface.
    virtual int parse(Tcl_Interp* t, int argc, char **argv);

 protected:
    float scale;
    McHandle<SoSeparator> scene;
};

#endif
