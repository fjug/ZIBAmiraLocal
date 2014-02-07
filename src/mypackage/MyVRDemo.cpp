///////////////////////////////////////////////////////////////////////
//
// Illustrates 3D interaction in a VR environment
//
///////////////////////////////////////////////////////////////////////
#include <stdlib.h>

#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/nodes/SoMatrixTransform.h>
#include <Inventor/events/SoTrackerEvent.h>
#include <Inventor/events/SoControllerButtonEvent.h>

#include <hxcore/Hx3DWandBase.h>
#include <mypackage/MyVRDemo.h>

HX_INIT_CLASS(MyVRDemo,HxModule)

MyVRDemo::MyVRDemo() :
    HxModule(HxObject::getClassTypeId()),
    portAction(this,"action",1)
{
    isMoving = 0;
    portAction.setLabel(0,"Reset");
    scene = new SoSeparator;
    createScene(scene);
    showGeom(scene);
}

MyVRDemo::~MyVRDemo()
{
    hideGeom(scene);
}

void MyVRDemo::compute()
{
    if (portAction.isNew() && portAction.getIndex()==0)
        createScene(scene);
}

void MyVRDemo::createScene(SoSeparator* scene)
{
    scene->removeAllChildren();

    SoEventCallback* cb = new SoEventCallback;
    cb->addEventCallback(SoTrackerEvent::getClassTypeId(),
        trackerEventCB, this);
    cb->addEventCallback(SoControllerButtonEvent::getClassTypeId(),
        controllerEventCB, this);
    scene->addChild(cb);

    for (int j=0; j<4; j++) {
        for (int i=0; i<4; i++) {
            SoSeparator* child = new SoSeparator;

            SoMatrixTransform* xform = new SoMatrixTransform;
            SbMatrix M;
            M.setTranslate(SbVec3f(i*3.f,j*3.f,0));
            xform->matrix.setValue(M);

            SoMaterial* mat = new SoMaterial;
            float hue = (rand()%1000)/1000.;
            mat->diffuseColor.setHSVValue(hue,1.f,.8f);

            SoCube* cube = new SoCube;

            child->addChild(xform);
            child->addChild(mat);
            child->addChild(cube);

            scene->addChild(child);
        }
    }
}

SoSeparator* MyVRDemo::checkCube(const SbVec3f& p)
{
    for (int iChild=1; iChild<scene->getNumChildren(); iChild++) {
        SoSeparator* child = (SoSeparator*) scene->getChild(iChild);
        SoMatrixTransform* xform = (SoMatrixTransform*) child->getChild(0);
        const SbMatrix& M = xform->matrix.getValue();

        SbVec3f q;
        M.inverse().multVecMatrix(p,q);
        if (fabs(q[0])<1 && fabs(q[1])<1 && fabs(q[2])<1) {
            if (activeCube.ptr()!=child) {
                if (activeCube) {
                    SoMaterial* mat = (SoMaterial*) activeCube->getChild(1);
                    mat->emissiveColor = SbColor(0.f,0.f,0.f);
                }
                SoMaterial* mat = (SoMaterial*) child->getChild(1);
                mat->emissiveColor = mat->diffuseColor[0];
                activeCube = child;
            }
            return activeCube;
        }
    }

    if (activeCube) {
        SoMaterial* mat = (SoMaterial*) activeCube->getChild(1);
        mat->emissiveColor = SbColor(0.f,0.f,0.f);
        activeCube = 0;
    }

    return 0;
}

void MyVRDemo::trackerEventCB(void* userData, SoEventCallback* node)
{
    MyVRDemo* me = (MyVRDemo*) userData;
    me->trackerEvent(node);
}

void MyVRDemo::trackerEvent(SoEventCallback* node)
{
    SoTrackerEvent* e = (SoTrackerEvent*) node->getEvent();
    Hx3DWandBase* wand = GET_WAND(e);

    if (activeCube && isMoving) {
        if (!wand->getButton(0)) {
            node->setHandled();
            isMoving = 0;
            return;
        }

        SbMatrix T1; T1.setTranslate(-refPos);
        SbMatrix R; R.setRotate(refRotInverse*wand->orientation());
        SbMatrix T2; T2.setTranslate(wand->origin());
        
        SoMatrixTransform* xform = (SoMatrixTransform*) activeCube->getChild(0);
        xform->matrix = SbMatrix(refMatrix*T1*R*T2);

        wand->setHighlight(true);
        node->setHandled();
        return;
    }

    if (checkCube(wand->top()))
        node->setHandled();
}

void MyVRDemo::controllerEventCB(void* userData, SoEventCallback* node)
{
    MyVRDemo* me = (MyVRDemo*) userData;
    me->controllerEvent(node);
}

void MyVRDemo::controllerEvent(SoEventCallback* node)
{
    if (activeCube) {
        SoTrackerEvent* e = (SoTrackerEvent*) node->getEvent();
        Hx3DWandBase* wand = GET_WAND(e);
    
        if (wand->wasButtonPressed(0)) {
            SoMatrixTransform* xform = (SoMatrixTransform*) activeCube->getChild(0);
            refRotInverse = wand->orientation().inverse();
            refPos = wand->origin();
            refMatrix = xform->matrix.getValue();

            wand->setHighlight(true);
            isMoving = 1;
        }
        
        if (wand->wasButtonReleased(0))
            isMoving = 0;

        node->setHandled();
    }
}

