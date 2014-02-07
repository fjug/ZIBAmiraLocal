///////////////////////////////////////////////////////////////////////
//
// Example of a compute module (version 2)
//
///////////////////////////////////////////////////////////////////////

#include <hxcore/HxMessage.h>                // for output in Amira console
#include <hxcore/HxWorkArea.h>               // busy-cursor and progress bar
#include <hxfield/HxUniformScalarField3.h>  // class representing 3D images
#include <mypackage/MyComputeThreshold2.h>  // declaration of this class

HX_INIT_CLASS(MyComputeThreshold2,HxCompModule) // required macro

MyComputeThreshold2::MyComputeThreshold2() :
    HxCompModule(HxUniformScalarField3::getClassTypeId()),
    portRange(this,"range",2) // we want to have two float fields
{
    // Set default value for the range port:
    portRange.setValue(0,30);  // min value is 30
    portRange.setValue(1,200); // max value is 200 
}

void MyComputeThreshold2::compute()
{
    // Access the input data object. The member portData (which is of
    // type HxConnection) is inherited from the base class HxModule.
    HxUniformScalarField3* field = (HxUniformScalarField3*) portData.source();

    // Check whether the input port is connected
    if (!field) return;

    // Get the input parameters from the user interface:
    float minValue = portRange.getValue(0);
    float maxValue = portRange.getValue(1);
    
    // Turn Amira into busy state, don't activate the Stop button.
    theWorkArea->startWorkingNoStop("Computing threshold");

    // Access size of data volume:
    const int* dims = field->lattice.dims();

    // Create output field with same primitive data type as input:
    HxUniformScalarField3* output =
	new HxUniformScalarField3(dims, field->primType());
    
    // Output shall have same bounding box as input:
    output->coords()->setBoundingBox(field->bbox());

    // Now loop through the whole field and count the pixels.
    int belowCnt=0, aboveCnt=0;
    for (int k=0; k<dims[2]; k++) {
	
	// Set progress bar, the argument ranges between 0 and 1.
	theWorkArea->setProgressValue((float)(k+1)/dims[2]);
	
	for (int j=0; j<dims[1]; j++) {
	    for (int i=0; i<dims[0]; i++) {
		// This function returns the value at this specfic grid node.
		// It implicitely casts the result to float if necessary.
		float value = field->evalReg(i,j,k);
		float newValue = 0; 

		if (value<minValue)
		    belowCnt++;
		else if (value>maxValue)
		    aboveCnt++;
		else newValue = value;

                output->set(i,j,k,newValue);
	    }
	}
    }

    theWorkArea->stopWorking(); // stop progress bar
    
    setResult(output); // register result

    // Finally print the result.
    theMsg->printf("%d voxels < %g, %d voxels > %g\n",
        belowCnt, minValue, aboveCnt, maxValue);
}

