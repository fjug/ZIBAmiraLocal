/*
 *  Template of a compute module
 */

#include <hxcore/HxMessage.h>                // for output in Amira console
#include <hxcore/HxWorkArea.h>               // busy-cursor and progress bar
#include <hxfield/HxUniformScalarField3.h>   // class representing 3D images#include <hxfield/HxUniformScalarField3.h>

#include "ParamaxflowWithMap.h"
#include "api.h"
#include "parametric.h"

HX_INIT_CLASS(ParamaxflowWithMap,HxCompModule)

ParamaxflowWithMap::ParamaxflowWithMap() :
    HxCompModule(HxUniformScalarField3::getClassTypeId()),
	portProbMap(this,"probMap",HxUniformScalarField3::getClassTypeId()), // the second incoming connection
	portIs3d(this,"2D stack / 3D volume?",2),
    portLambdaRange(this,"Lambda-range",2), // we want to have two float fields
	portUnary(this,"U(k1,mean,sd)",3),
    portIsing(this,"Ising(k2)",1),
    portEdgeX(this,"Edge_x(k3x,mean,sd)",3),
    portEdgeY(this,"Edge_y(k3y,mean,sd)",3),
    portEdgeZ(this,"Edge_z(k3z,mean,sd)",3),
    portAction(this,"action") {

	// some initializations
	p = NULL;
	fieldInP = NULL;

    // Set default value for the range port:
    portLambdaRange.setValue(0,-1000);
    portLambdaRange.setValue(1,1000); 

	// Set default values for the unary potential
	portUnary.setValue(0, -1.0);
	portUnary.setValue(1, 0.45);
	portUnary.setValue(2, 0.15);

	// Set default values for the binary potentials
	portIsing.setValue(0, 1.00); // factor for Ising Prior

	portEdgeX.setValue(0, 1.00); // factor for Edge Prior
	portEdgeX.setValue(1, 1.00); // mean
	portEdgeX.setValue(2, 0.10); // sd

	portEdgeY.setValue(0, -.50); // factor for Edge Prior
	portEdgeY.setValue(1, 0.50); // mean
	portEdgeY.setValue(2, 10.0); // sd

	portEdgeZ.setValue(0, 0.00); // factor for Edge Prior
	portEdgeZ.setValue(1, 0.50); // mean
	portEdgeZ.setValue(2, 10.0); // sd

    // Set text of doIt button
    portAction.setLabel(0,"DoIt");
}

ParamaxflowWithMap::~ParamaxflowWithMap() {
	delete p;
}

Parametric* ParamaxflowWithMap::buildGraph(HxUniformScalarField3* field, HxUniformScalarField3* map, 
										   bool is3d,
										   double k1, double k1_mean, double k1_sd, 
										   double k2, 
										   double k3x, double k3x_mean, double k3x_sd, 
										   double k3y, double k3y_mean, double k3y_sd, 
										   double k3z, double k3z_mean, double k3z_sd)
{
	long k, N, nodeNum = 3;
	Parametric::RegionId r;

	if (p) delete p;
	p = new Parametric(/*maximum # of nodes*/ field->lattice.nNodes(), /*maximum # of edges*/ 2*field->lattice.nNodes()); 
	p -> AddNode(field->lattice.nNodes()); // add as many nodes as the input image has pixels

	const int* dims = field->lattice.dims();
	long pixelId = 0;	
	float eps = 0.0000001;

	// for each pixel in input image --> create unary term
	for (long z=0; z<dims[2]; z++) {
		for (long y=0; y<dims[1]; y++) {
			for (long x=0; x<dims[0]; x++) {
				float value = field->evalReg(x,y,z);
				double likelihood = k1 * gaussian(k1_mean, k1_sd, value);
				likelihood *= (1.0 - map->evalReg(x,y,z));
				
				pixelId = z*dims[0]*dims[1] + y*dims[0] + x;
				p->AddUnaryTerm( pixelId, 1.0, likelihood );

				// Set progress bar, the argument ranges between 0 and 1.
				theWorkArea->setProgressValue(.5*pixelId/field->lattice.nNodes());
			}
		}
    }

	// for each pixel in input image --> create pairwise terms towards right (x), down (y) and back (z)
	for (long z=0; z<dims[2]; z++) {
		for (long y=0; y<dims[1]; y++) {
			for (long x=0; x<dims[0]; x++) {
				float value = field->evalReg(x,y,z);

				pixelId = z*dims[0]*dims[1] + y*dims[0] + x;
				
				if ( x+1 < dims[0] ) {
					float diff = std::abs( field->evalReg(x,y,z) - field->evalReg(x+1,y,z) ); //diff *= diff;
					if ( diff < eps ) diff = eps;
					float cost = k2 + k3x * gaussian(k3x_mean, k3x_sd, diff); // k2*Ising + k3*Edge
					cost *= (1.0 - map->evalReg(x,y,z));

					long xNeighborId = pixelId + 1;
					p->AddPairwiseTerm( pixelId, xNeighborId, 0.0, cost, cost, 0.0); // add term with costs E00, E01, E10, and E11, 
				}
				if ( y+1 < dims[1] ) {
					float diff = std::abs( field->evalReg(x,y,z) - field->evalReg(x,y+1,z) ); //diff *= diff;
					if ( diff < eps ) diff = eps;
					float cost = k2 + k3y * gaussian(k3y_mean, k3y_sd, diff);
					cost *= (1.0 - map->evalReg(x,y,z));

					long yNeighborId = pixelId + dims[0];
					p->AddPairwiseTerm( pixelId, yNeighborId, 0.0, cost, cost, 0.0); // add term with costs E00, E01, E10, and E11, 
				}
				// connect in z-direction ONLY if is3d==TRUE!!!
				if ( is3d && z+1 < dims[2] ) {
					float diff = std::abs( field->evalReg(x,y,z) - field->evalReg(x,y,z+1) ); //diff *= diff;
					if ( diff < eps ) diff = eps;
					float cost = k2 + k3z * gaussian(k3z_mean, k3z_sd, diff);
					cost *= (1.0 - map->evalReg(x,y,z));

					long zNeighborId = pixelId + dims[0] * dims[1];
					p->AddPairwiseTerm( pixelId, zNeighborId, 0.0, cost, cost, 0.0); // add term with costs E00, E01, E10, and E11, 
				}

				// Set progress bar, the argument ranges between 0 and 1.
				theWorkArea->setProgressValue(.5 + .5*pixelId/field->lattice.nNodes());
			}
		}
    }
	return p;
}

void ParamaxflowWithMap::compute()
{
    // Access the input data object. The member portData (which is of
    // type HxConnection) is inherited from the base class HxModule.
    HxUniformScalarField3* field = dynamic_cast<HxUniformScalarField3 *>(portData.source());
	HxUniformScalarField3* map = dynamic_cast<HxUniformScalarField3 *>(portProbMap.source());

    // Check whether doIt button was hit
    if (!portAction.wasHit()) return;
    
    // Check whether the input port is connected
    if (!field) return;
    if (!map) return;

	// Check if probMap was given... create one containing all 0's if not
	//if (!map) {
	//	map = new HxUniformScalarField3(dims, field->primType());
	//	map->composeLabel(field->getName(),"probMap");
	//	map->coords()->setBoundingBox(field->bbox());
	//}


    // Turn Amira longo busy state, don't activate the Stop button.
    theWorkArea->startWorking("Computing Parametrix MaxFlow...");

	bool is3d = (portIs3d.getValue()==0)?false:true;
	//theMsg->printf(">>>>> is3d = %d\n", is3d);

	double l_min = portLambdaRange.getValue(0);
	double l_max = portLambdaRange.getValue(1);
	
	double k1 = portUnary.getValue(0);
	double k1_mean = portUnary.getValue(1);
	double k1_sd = portUnary.getValue(2);
	
	double k2 = portIsing.getValue(0);

	double k3x = portEdgeX.getValue(0);
	double k3x_mean = portEdgeX.getValue(1);
	double k3x_sd = portEdgeX.getValue(2);

	double k3y = portEdgeY.getValue(0);
	double k3y_mean = portEdgeY.getValue(1);
	double k3y_sd = portEdgeY.getValue(2);

	double k3z = portEdgeZ.getValue(0);
	double k3z_mean = portEdgeZ.getValue(1);
	double k3z_sd = portEdgeZ.getValue(2);

	Parametric* p = buildGraph(field,map,is3d,k1,k1_mean,k1_sd,k2,k3x,k3x_mean,k3x_sd,k3y,k3y_mean,k3y_sd,k3z,k3z_mean,k3z_sd);
	long N = p -> Solve(l_min,l_max);
	fieldInP = field;
	
    // Access size of data volume:
    const int* dims = field->lattice.dims();

    // Check if there is a result which we can reuse.
    HxUniformScalarField3* output = (HxUniformScalarField3*) getResult();

    // Check for proper type.
    if (output && !output->isOfType(HxUniformScalarField3::getClassTypeId()))
        output = NULL;

    // Check if size and primType still match the current input:
    if (output) {
		const int* outdims = output->lattice.dims();
		if (dims[0]!=outdims[0] || dims[1]!=outdims[1] || dims[2]!=outdims[2] || 
			field->primType() != output->primType())
			output=NULL;
    }

	// Check if dims of map fit to the ones of field
	const int* mapdims = map->lattice.dims();
	if ( mapdims[0]!=dims[0] || mapdims[1]!=dims[1] || mapdims[2]!=dims[2] ) {
		return;
	}


    // If necessary, create a new result data set.
    if (!output) {
        output = new HxUniformScalarField3(dims, field->primType());
        output->composeLabel(field->getName(),"numRegions");
    }
    
    // Output shall have same bounding box as input:
    output->coords()->setBoundingBox(field->bbox());


	// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	for (long graphNodeId=0; graphNodeId < field->lattice.nNodes(); graphNodeId++) {
		long numRegions = p->GetRegionCount(p->GetRegion(graphNodeId));

		long z = graphNodeId / (dims[0]*dims[1]);
		long remainder = graphNodeId - z*(dims[0]*dims[1]);
		long y = remainder / dims[0];
		long x = remainder - y*dims[0];
		
		output->set(x,y,z,numRegions);
	}

	// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    theWorkArea->stopWorking(); // stop progress bar
    
    setResult(output); // register result

    // Finally prlong the result.
	theMsg->printf("Done! --> N = %d.\n", N);
}
