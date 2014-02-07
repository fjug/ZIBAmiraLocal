///////////////////////////////////////////////////////////////////////
//
// Example of a compute module (version 1)
//
///////////////////////////////////////////////////////////////////////

#include <hxcore/HxMessage.h>                // for output in Amira console
#include <hxfield/HxUniformScalarField3.h>  // class representing 3D images
#include <mypackage/MyComputeThreshold1.h>  // declaration of this class

HX_INIT_CLASS(MyComputeThreshold1,HxCompModule) // required macro

MyComputeThreshold1::MyComputeThreshold1() :
    HxCompModule(HxUniformScalarField3::getClassTypeId()),
    portRange(this,"range",2) // we want to have two float fields
{
}

void MyComputeThreshold1::compute()
{
    // Access the input data object. The member portData (which is of
    // type HxConnection) is inherited from the base class HxModule.
    HxUniformScalarField3* field = (HxUniformScalarField3*) portData.source();

    // Check whether the input port is connected
    if (!field) return;

    // Get the input parameters from the user interface:
    float minValue = portRange.getValue(0);
    float maxValue = portRange.getValue(1);
    
    // Access size of data volume:
    const int* dims = field->lattice.dims();

    // Now loop through the whole field and count the pixels.
    int belowCnt=0, aboveCnt=0;
    for (int k=0; k<dims[2]; k++) {
	for (int j=0; j<dims[1]; j++) {
	    for (int i=0; i<dims[0]; i++) {
		// This function returns the value at this specfic grid node.
		// It implicitely casts the result to float if necessary.
		float value = field->evalReg(i,j,k);
		if (value<minValue)
		    belowCnt++;
		else if (value>maxValue)
		    aboveCnt++;
	    }
	}
    }

    // Finally print the result.
    theMsg->printf("%d voxels < %g, %d voxels > %g\n",
        belowCnt, minValue, aboveCnt, maxValue);
}

