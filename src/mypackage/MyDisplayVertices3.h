///////////////////////////////////////////////////////////////////////
//
// Example of a display module (version 3)
//
///////////////////////////////////////////////////////////////////////
#ifndef MY_DISPLAY_VERTICES3_H
#define MY_DISPLAY_VERTICES3_H

#include <mclib/McHandle.h>                // smart pointer template class
#include <hxcore/HxModule.h>          // include declaration of base class
#include <hxcore/HxPortRadioBox.h>    // radio box style selection
#include <hxcore/HxPortIntSlider.h>   // provides integer slider
#include <hxcolor/HxPortColormap.h>  // connection to a colormap
#include <mypackage/api.h>  // storage-class specification

#include <Inventor/nodes/SoSeparator.h>

class MYPACKAGE_API MyDisplayVertices3 : public HxModule
{
    HX_HEADER(MyDisplayVertices3);

  public:
    // Constructor.
    MyDisplayVertices3();

    // Destructor.
    ~MyDisplayVertices3();

    // Connection to a colormap.
    HxPortColormap portColormap;

    // Mode: 0=selected vertices, 1=all vertices
    HxPortRadioBox portMode;

    // Input parameter. 
    HxPortIntSlider portNumTriangles;

    // Shows or hides required ports.
    virtual void update();

    // This is called when an input port changes.
    virtual void compute();

    // Tcl command interface.
    virtual int parse(Tcl_Interp* t, int argc, char **argv);

 protected:
    float scale;
    McHandle<SoSeparator> scene;
};

#endif
