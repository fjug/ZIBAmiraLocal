///////////////////////////////////////////////////////////////////////
//
// Example of a plot module (header file)
//
///////////////////////////////////////////////////////////////////////
#ifndef MY_PLOT_AREA_PER_SLICE_H
#define MY_PLOT_AREA_PER_SLICE_H

#include <hxcore/HxModule.h>          // include declaration of base class
#include <hxcore/HxPortButtonList.h>  // provides a push button
#include <hxplot/PzEasyPlot.h>       // simple plot window
#include <mypackage/api.h>  // storage-class specification

class MYPACKAGE_API MyPlotAreaPerSlice : public HxModule
{
    HX_HEADER(MyPlotAreaPerSlice);

  public:
    // Constructor.
    MyPlotAreaPerSlice();

    // Shows the plot window.
    HxPortButtonList portAction;

    // Performs the actual computation.
    virtual void compute();

 protected:
    McHandle<PzEasyPlot> plot;
};

#endif
