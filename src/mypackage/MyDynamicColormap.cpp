///////////////////////////////////////////////////////////////////////
//
// Illustrates the use of the time slider
//
///////////////////////////////////////////////////////////////////////
#include <math.h>
#include <mclib/McColor.h>
#include <mypackage/MyDynamicColormap.h>

HX_INIT_CLASS(MyDynamicColormap,HxColormap)

MyDynamicColormap::MyDynamicColormap() :
    portTime(this,"time")
{
    portTime.setMinMax(0,1);
    portTime.setIncrement(0.1);
    portTime.setDiscrete(0);

    // After 1000 msecs the value should be incremented by 0.5
    portTime.setRealTimeFactor(0.5*0.001);
}

MyDynamicColormap::~MyDynamicColormap()
{
    portTime.stop();
}

void MyDynamicColormap::compute()
{
    if (portTime.isNew())
        touch();
}

void MyDynamicColormap::getRGBA1(float u, float rgba[4]) const
{
    McColor color;
    color.setHSVValue(fmod(portTime.getValue()+u,1),1,1);

    rgba[0] = color[0];
    rgba[1] = color[1];
    rgba[2] = color[2];
    rgba[3] = 1;
}

