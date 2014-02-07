///////////////////////////////////////////////////////////////////////
//
// Example of a compute module (version 1)
//
///////////////////////////////////////////////////////////////////////

#include <hxcore/HxMessage.h>      // for output in Amira console
#include <hxsurface/HxSurface.h>  // class representing a surface
#include <mypackage/MyDisplayVertices3.h> // header of this class

#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoMaterial.h>

HX_INIT_CLASS(MyDisplayVertices3,HxModule)

MyDisplayVertices3::MyDisplayVertices3() :
    HxModule(HxSurface::getClassTypeId()),
    portColormap(this,"colormap"),
    portMode(this,"mode",2),
    portNumTriangles(this,"numTriangles")
{
    portColormap.enableAlpha(0);
    portMode.setLabel(0,"selected vertices");
    portMode.setLabel(1,"all vertices");
    portNumTriangles.setMinMax(1,12);
    portNumTriangles.setValue(6);
    scene = new SoSeparator;
    scale = 0.01;
}

MyDisplayVertices3::~MyDisplayVertices3()
{
    hideGeom(scene);
}

void MyDisplayVertices3::update()
{
    if (portMode.getValue()==0)
        portNumTriangles.show();
    else portNumTriangles.hide();
}

void MyDisplayVertices3::compute()
{
    int i;

    // Access input object (portData is inherited from HxModule):
    HxSurface* surface = (HxSurface*) portData.source();

    if (!surface) { // Check if input object is available
        hideGeom(scene);
	return;
    }

    // Get value from input port, query size of surface:
    int numTriPerVertex = portNumTriangles.getValue();
    int nVertices = surface->points.size();
    int nTriangles = surface->triangles.size();

    // We need a triangle counter for every vertex: 
    McDArray<unsigned short> triCount(nVertices);
    triCount.fill(0);
    
    // Loop through all triangles and increase counter of the vertices:
    for (i=0; i<nTriangles; i++)
        for (int j=0; j<3; j++)
            triCount[surface->triangles[i].points[j]]++;

    // Now create the scene graph. First remove all previous childs:
    scene->removeAllChildren();
    
    // Cube size should be proportional to diagonal of the bounding box.
    float size = surface->getBoundingBoxSize().length() * scale;

    // Pointer to surface coordinates casted from McVec3f to SbVec3f.
    SbVec3f* p = (SbVec3f*) surface->points.dataPtr();

    SbVec3f q(0,0,0); // position of last point
    int count = 0; // vertex counter
    int lastNumTriPerVertex = -1;
    int allVertices = portMode.getValue();

    for (i=0; i<nVertices; i++) {
        if (allVertices || triCount[i]==numTriPerVertex) {

            if (triCount[i]!=lastNumTriPerVertex) {
                SoMaterial* material = new SoMaterial;
                material->diffuseColor = portColormap.getColor(triCount[i]);
                scene->addChild(material);
                lastNumTriPerVertex = triCount[i];
            }

            SoTranslation* trans = new SoTranslation;
            trans->translation.setValue(p[i]-q);

            SoCube* cube = new SoCube;
            cube->width = cube->height = cube->depth = size;

            scene->addChild(trans);
            scene->addChild(cube);

            count++;
            q=p[i];
        }
    }

    if (!allVertices)
        theMsg->printf("Found %d vertices belonging to %d triangles",
            count, numTriPerVertex);

    showGeom(scene); // finally show scene in viewer
}

int MyDisplayVertices3::parse(Tcl_Interp* t, int argc, char **argv)
{
    if (argc < 2) return TCL_OK;
    char *cmd = argv[1];

    if (CMD("setScale")) {
        ASSERTARG(3);
        scale = atof(argv[2]);
        fire();
    }
    else if (CMD("getScale")) {
        Tcl_VaSetResult(t, "%g", scale);
    }
    else return HxModule::parse(t,argc,argv);

    return TCL_OK;
}

