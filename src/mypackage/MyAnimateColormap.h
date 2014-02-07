///////////////////////////////////////////////////////////////////////
//
// Illustrates the use of time-out methods
//
///////////////////////////////////////////////////////////////////////
#ifndef MY_ANIMATE_COLORMAP_H
#define MY_ANIMATE_COLORMAP_H

#include <hxcore/HxCompModule.h>
#include <hxcore/HxPortIntSlider.h>
#include <hxcore/HxPortToggleList.h>
#include <mypackage/api.h>

class MYPACKAGE_API MyAnimateColormap : public HxCompModule
{
    HX_HEADER(MyAnimateColormap);

  public:
    // Constructor.
    MyAnimateColormap();

    // This will be called when an input port changes.
    virtual void compute();

    // The animation interval
    HxPortIntSlider portInterval;

    // Enables or disables animation
    HxPortToggleList portAnimate;

  protected:
    ~MyAnimateColormap();

    int isAnimating;
    void startAnimating();
    void stopAnimating();
};

#endif
