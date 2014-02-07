///////////////////////////////////////////////////////////////////////
//
// Read routine for the PPM3D file format
//
///////////////////////////////////////////////////////////////////////

#include <hxcore/HxMessage.h>               // for output in Amira console
#include <hxfield/HxUniformScalarField3.h> // class representing 3D images
#include <mypackage/api.h>        // storage-class specification

MYPACKAGE_API int readppm3d(const char* filename)
{
    FILE* f = fopen(filename, "r"); // open the file
    
    if (!f) {
	theMsg->ioError(filename);
	return 0; // indicate error
    }
    
    // Skip header (first line). We could do some checking here:
    char buf[80];
    fgets(buf, 80, f);
    
    // Read size of volume:
    int dims[3];
    dims[0] = dims[1] = dims[2] = 0;
    fscanf(f, "%d %d %d", &dims[0], &dims[1], &dims[2]);

    // Do some consistency checking.
    if (dims[0]*dims[1]*dims[2] <= 0) {
	theMsg->printf("Error in file %s.", filename);
	fclose(f);
	return 0;
    }

    // Now create 3D image data object. The constructor takes the
    // dimensions and the primary data type. In this case we create 
    // a field containing unsigned bytes (8 bit). 
    HxUniformScalarField3* field = 
	new HxUniformScalarField3(dims, McPrimType::mc_uint8);

    // The HxUniformScalarField3 stores its data in a member variable
    // called lattice. We know, that the data is unsigned 8 bit,
    // because we specified this in the constructor. 
    unsigned char* data = (unsigned char*) field->lattice.dataPtr();
    
    // Now we have to read dims[0]*dims[1]*dims[2] data values
    for (int i=0; i<dims[0]*dims[1]*dims[2]; i++) {
	int val=0;
	fscanf(f,"%d",&val);
	data[i] = (unsigned char) val;
    }
        
    // We are done with reading, close the file.
    fclose(f);
    
    // Register the data object to make it visible in the object pool. The
    // name for the new object is automatically generated from the filename. 
    HxData::registerData(field, filename);

    return 1; // indicate success
}

