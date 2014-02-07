///////////////////////////////////////////////////////////////////////
//
// Read routine for the trimesh file format
//
///////////////////////////////////////////////////////////////////////

#include <mclib/McStringTokenizer.h>
#include <hxcore/HxMessage.h>
#include <hxsurface/HxSurface.h>
#include <hxsurface/HxSurfaceField.h>
#include <mypackage/api.h>

MYPACKAGE_API int readtrimesh(const char* filename)
{
    FILE* fp = fopen(filename, "r");

    if (!fp) {
        theMsg->ioError(filename);
        return 0;
    }

    // 1. Read the surface itself

    char buffer[256];
    fgets(buffer,256,fp); // read first line

    int i,j,k,nPoints=0,nTriangles=0;
    sscanf(buffer, "%d %d", &nPoints, &nTriangles); // get numbers

    if (nPoints<0 || nTriangles<0) {
        theMsg->printf("Illegal number of points or triangles.");
        fclose(fp);
        return 0;
    }

    HxSurface* surface = new HxSurface; // create new surface
    surface->addMaterial("Inside",0); // add some materials
    surface->addMaterial("Outside",1);

    HxSurface::Patch* patch = new HxSurface::Patch; // create surface patch
    surface->patches.append(patch); // add patch to surface
    patch->innerRegion = 0;
    patch->outerRegion = 1;

    surface->points.resize(nPoints);
    surface->triangles.resize(nTriangles);

    for (i=0; i<nPoints; i++) { // read point coordinates
        McVec3f& p = surface->points[i];
        fgets(buffer,256,fp);
        sscanf(buffer, "%g %g %g", &p[0], &p[1], &p[2]);
    }

    for (i=0; i<nTriangles; i++) { // read triangles
        int idx[3];
        fgets(buffer,256,fp);
        sscanf(buffer, "%d %d %d", &idx[0], &idx[1], &idx[2]);

        Surface::Triangle& tri = surface->triangles[i];
        tri.points[0] = idx[0]-1; // indices should start at zero
        tri.points[1] = idx[1]-1;
        tri.points[2] = idx[2]-1;
        tri.patch = 0;
    }

    patch->triangles.resize(nTriangles);
    for (i=0; i<nTriangles; i++)
        patch->triangles[i] = i; // add all triangles to one patch

    HxData::registerData(surface,filename); // add surface to object pool

    // 2. Check if file also contains data fields

    fgets(buffer,256,fp);
    McStringTokenizer tk(buffer);
    McDArray<HxSurfaceField*> fields;

    while (tk.hasMoreTokens()) { // are there any numbers here ?
        int n = atoi(tk.nextToken());
        // Create appropriate field, e.g. HxSurfaceScalarField if n==1
        HxSurfaceField* field = HxSurfaceField::create(surface,
            HxSurfaceField::OnNodes, n);
        fields.append( field );
    }

    if (fields.size()) {
        for (i=0; i<nPoints; i++) { // read data values of all fields
            fgets(buffer,256,fp);
            tk = buffer;
            for (j=0; j<fields.size(); j++) {
                int n = fields[j]->nDataVar();
                float* v = &fields[j]->dataPtr()[i*n];
                for (k=0; k<n; k++)
                    v[k] = atof(tk.nextToken());
            }
        }

        for (i=0; i<fields.size(); i++) { // add fields to object pool
            HxData::registerData(fields[i], NULL);
            fields[i]->composeLabel(surface->getName(),"data");
        }
    }

    fclose(fp); // close file and return ok

    // Fix the load command of all created objects
    McString loadCmd;
    loadCmd.printf("set TMPIO [load -trimesh %s]\n"
                   "lindex $TMPIO 0", filename);
    surface->setLoadCmd(loadCmd,1);

    for (i=0; i<fields.size(); i++) {
        loadCmd.printf("lindex $TMPIO %d", i+1);
        fields[i]->setLoadCmd(loadCmd,1);
    }

    return 1;
}

