///////////////////////////////////////////////////////////////////////
//
// Illustrates the use of the time slider
//
///////////////////////////////////////////////////////////////////////
#ifndef MY_DYNAMIC_COLORMAP_H
#define MY_DYNAMIC_COLORMAP_H

#include <hxcolor/HxColormap.h>
#include <hxtime/HxPortTime.h>
#include <mypackage/api.h>

class MYPACKAGE_API MyDynamicColormap : public HxColormap
{
    HX_HEADER(MyDynamicColormap);

  public:
    // Constructor.
    MyDynamicColormap();

    // Constructor.
    ~MyDynamicColormap();

    // This will be called when an input port changes.
    virtual void compute();

    // The time slider
    HxPortTime portTime;

    // Implements the colormap
    virtual void getRGBA1(float u, float rgba[4]) const;
};

#endif
