///////////////////////////////////////////////////////////////////////
//
// Illustrates 3D interaction in a VR environment
//
///////////////////////////////////////////////////////////////////////
#ifndef MY_VR_DEMO_H
#define MY_VR_DEMO_H

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/SbVec.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/SbRotation.h>

#include <mclib/McHandle.h>
#include <hxcore/HxModule.h>
#include <hxcore/HxPortButtonList.h>
#include <mypackage/api.h>

class MYPACKAGE_API MyVRDemo : public HxModule
{
    HX_HEADER(MyVRDemo);

  public:
    MyVRDemo();
    virtual void compute();
    HxPortButtonList portAction;

  protected:
    ~MyVRDemo();

    McHandle<SoSeparator> scene;
    McHandle<SoSeparator> activeCube;

    bool isMoving;
    SbVec3f refPos;
    SbMatrix refMatrix;
    SbRotation refRotInverse;

    void createScene(SoSeparator* scene);
    SoSeparator* checkCube(const SbVec3f& pos);
    void trackerEvent(SoEventCallback* node);
    void controllerEvent(SoEventCallback* node);

    static void trackerEventCB(void* userData, SoEventCallback* node);
    static void controllerEventCB(void* userData, SoEventCallback* node);
};

#endif
