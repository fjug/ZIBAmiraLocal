///////////////////////////////////////////////////////////////////////
//
// Sample write routine for the PPM3D file format
//
///////////////////////////////////////////////////////////////////////

#include <hxcore/HxMessage.h>               // for output in Amira console
#include <hxfield/HxUniformScalarField3.h> // class representing 3D images
#include <mypackage/api.h>        // storage-class specification

MYPACKAGE_API
int writeppm3d(HxUniformScalarField3* field, const char* filename)
{
    // For the moment we only want to support byte data
    if (field->primType() != McPrimType::mc_uint8) {
	theMsg->printf("This format only supports byte data.");
	return 0; // indicate error
    }

    FILE* f = fopen(filename, "w"); // open the file
    
    if (!f) {
	theMsg->ioError(filename);
	return 0; // indicate error
    }
    
    // Write header:
    fprintf(f, "# PPM3D\n");

    // Write fields dimensions:
    const int* dims = field->lattice.dims();
    fprintf(f, "%d %d %d\n", dims[0], dims[1], dims[2]);

    // Write dims[0]*dims[1]*dims[2] data values:
    unsigned char* data = (unsigned char*) field->lattice.dataPtr();
    for (int i=0; i<dims[0]*dims[1]*dims[2]; i++) {
	fprintf(f,"%d ",data[i]);
	if (i%20 == 19) // Do some formatting:
            fprintf(f,"\n");
    }
        
    // Close the file.
    fclose(f);
    
    return 1; // indicate success
}

