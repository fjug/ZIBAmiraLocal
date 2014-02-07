///////////////////////////////////////////////////////////////////////
//
// Write routine for the trimesh file format
//
///////////////////////////////////////////////////////////////////////

#include <hxcore/HxMessage.h>
#include <hxsurface/HxSurface.h>
#include <hxsurface/HxSurfaceField.h>
#include <mypackage/api.h>

static
int writetrimesh(HxSurface* surface,
    McDArray<HxSurfaceField*> fields, const char* filename)
{
    FILE *f = fopen(filename, "w");

    if (!f) {
        theMsg->ioError(filename);
        return 0;
    }

    int i,j,k;
    McDArray<McVec3f>& points = surface->points;
    McDArray<Surface::Triangle>& triangles = surface->triangles;

    // Write number of points and number of triangles
    fprintf(f, "%d %d\n", points.size(), triangles.size());
        
    // Write point coordinates
    for (i=0; i<points.size(); i++) {
        McVec3f& v = points[i];
        fprintf(f, "%g %g %g\n", v[0], v[1], v[2]);
    }

    // Write point indices of all triangles
    for (i=0; i<triangles.size(); i++) {
        int* idx = triangles[i].points;
        fprintf(f, "%d %d %d\n", idx[0]+1, idx[1]+1, idx[2]+1);
    }

    // If there are data fields write them out, too.
    if (fields.size()) {
        for (j=0; j<fields.size(); j++)
            fprintf(f, "%d ", fields[j]->nDataVar());
        fprintf(f, "\n");

        for (i=0; i<points.size(); i++) {
            for (j=0; j<fields.size(); j++) {
                int n = fields[j]->nDataVar();
                float* v = &fields[j]->dataPtr()[i*n];
                for (k=0; k<n; k++)
                    fprintf(f, "%g ", v[k]);
            }
            fprintf(f, "\n");
        }
    }

    fclose(f); // done
    return 1;
}

MYPACKAGE_API
int writetrimesh(HxSurface* surface, const char* filename)
{
    // Temporary array of surface data fields
    McDArray<HxSurfaceField*> fields;

    // Check if there are data fields attached to surface
    for (int i=0; i<surface->downStreamConnections.size(); i++) {
        HxSurfaceField* field =
            (HxSurfaceField*) surface->downStreamConnections[i];
        if (field->isOfType(HxSurfaceField::getClassTypeId()) &&
            field->getEncoding() == HxSurfaceField::OnNodes)
            fields.append(field);
    }

    // Write surface and all attached data fields
    return writetrimesh(surface, fields, filename);
}

MYPACKAGE_API
int writetrimesh(HxSurfaceField* field, const char* filename)
{
    // Check if data is defined on nodes
    if (field->getEncoding() != HxSurfaceField::OnNodes) {
        theMsg->printf("Data must be defined on nodes.");
        return 0;
    }

    // Store pointer to field in dynamic array
    McDArray<HxSurfaceField*> fields;
    fields.append(field);

    // Write surface and this data field
    return writetrimesh(field->surface(), fields, filename);
}

