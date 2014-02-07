///////////////////////////////////////////////////////////////////////
//
// Example of a compute module (version 2)
//
///////////////////////////////////////////////////////////////////////

#include <hxcore/HxMessage.h>                // for output in Amira console
#include <hxcore/HxWorkArea.h>               // busy-cursor and progress bar
#include <hxfield/HxUniformScalarField3.h>  // class representing 3D images
#include <mypackage/MyComputeThreshold3.h>  // declaration of this class

HX_INIT_CLASS(MyComputeThreshold3,HxCompModule)

MyComputeThreshold3::MyComputeThreshold3() :
    HxCompModule(HxUniformScalarField3::getClassTypeId()),
    portRange(this,"range",2), // we want to have two float fields
    portDoIt(this,"action")
{
    // Set default value for the range port:
    portRange.setValue(0,30);  // min value is 30
    portRange.setValue(1,200); // max value is 200

    // Set text of doIt button
    portDoIt.setLabel(0,"DoIt");
}

void MyComputeThreshold3::compute()
{
    // Access the input data object. The member portData (which is of
    // type HxConnection) is inherited from the base class HxModule.
    HxUniformScalarField3* field = (HxUniformScalarField3*) portData.source();

    // Check whether the input port is connected
    if (!field) return;

    // Check whether doIt button was hit
    if (!portDoIt.wasHit()) return;

    // Get the input parameters from the user interface:
    float minValue = portRange.getValue(0);
    float maxValue = portRange.getValue(1);
    
    // Turn Amira into busy state, don't activate the Stop button.
    theWorkArea->startWorkingNoStop("Computing threshold");

    // Access size of data volume:
    const int* dims = field->lattice.dims();

    // Check if there is a result which we can reuse.
    HxUniformScalarField3* output = (HxUniformScalarField3*) getResult();

    // Check for proper type.
    if (output && !output->isOfType(HxUniformScalarField3::getClassTypeId()))
        output = 0;

    // Check if size and primType still match the current input:
    if (output) {
	const int* outdims = output->lattice.dims();
	if (dims[0]!=outdims[0] || dims[1]!=outdims[1] || dims[2]!=outdims[2] || 
	    field->primType() != output->primType())
	    output=0;
    }

    // If necessary, create a new result data set.
    if (!output) {
        output = new HxUniformScalarField3(dims, field->primType());
        output->composeLabel(field->getName(),"masked");
    }
    
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

