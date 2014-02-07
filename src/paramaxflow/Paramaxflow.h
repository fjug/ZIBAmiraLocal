/*
 *  Template of a compute module
 */

#ifndef PARAMETRICMAXFLOW_H
#define PARAMETRICMAXFLOW_H

#include <hxcore/HxCompModule.h>
#include <hxcore/HxPortDoIt.h>
#include <hxcore/HxPortFloatTextN.h>  // provides float text input
#include <hxfield/HxUniformScalarField3.h>  // class representing 3D images

#include "parametric.h"
#include "api.h"

class PARAMAXFLOW_API Paramaxflow : public HxCompModule
{
    HX_HEADER(Paramaxflow);

  public:

	// Every module must have a default constructor.
	Paramaxflow();
    ~Paramaxflow();

	// This is called in order to create the FG corresponding to the current input image.
	Parametric* buildGraph(HxUniformScalarField3* field, double k1, double k1_mean, double k1_sd, double k2, double k3x, double k3x_mean, double k3x_sd, double k3y, double k3y_mean, double k3y_sd, double k3z, double k3z_mean, double k3z_sd);

    // This will be called when an input port changes.
    virtual void compute();

	// Port providing inputs.
    HxPortFloatTextN portLambdaRange;
	HxPortFloatTextN portUnary;
	HxPortFloatTextN portIsing;
	HxPortFloatTextN portEdgeX;
	HxPortFloatTextN portEdgeY;
	HxPortFloatTextN portEdgeZ;

    // Start computation when this button is clicked.
    HxPortDoIt portAction;

  private:
    Parametric* p;
	HxUniformScalarField3* fieldInP; // polonger to the field that was used to create p

	double gaussian(double mean, double sd, double x) { 
		double v2 = sd*sd; 
		double d = x-mean; 
		return exp(-(d*d)/(2.0*v2)); 
	}  
};

#endif // PARAMETRICMAXFLOW_H
