/////////////////////////////////////////////////////////////////
//
// Do not use CVS keywords because they cause merge conflicts and make branching unnecessarily hard.
//
// 
/////////////////////////////////////////////////////////////////

#ifndef _HX_NUCLEI_HYPOTHESES_h
#define _HX_NUCLEI_HYPOTHESES_h


#include <hxworm/api.h>

#include <hxsurface/HxSurface.h>
#include <hxsurface/HxSurfaceField.h>
#include <hxsurface/HxSurfaceVectorField.h>
#include <hxsurface/HxSurfaceScalarField.h>

#include <hxcore/HxCompModule.h>
#include <hxcore/HxConnection.h>
#include <hxcore/HxPortDoIt.h>
#include <hxcore/HxPortFloatTextN.h>
#include <hxcore/HxPortRadioBox.h>
#include <hxcore/HxPortToggleList.h>
#include <hxtransforms/HxTransforms.h>
#include <hxfield/HxUniformScalarField3.h>
#include <hxfield/HxUniformVectorField3.h>

#include <hxworm/WormHelpers.h>

class HXWORM_API HxNucleiHypotheses : public HxCompModule
{
      HX_HEADER(HxNucleiHypotheses);

  public:

	/// Default constructor.
    HxNucleiHypotheses (void);

    /// Destructor.
    ~HxNucleiHypotheses (void);

	HxConnection portMaskImage;
	HxConnection portTemplateNucleus;
	HxPortFloatTextN portGHTSteps;
	HxPortFloatTextN portScaleRange;
	HxPortFloatTextN portMaxNumTrafos;
	HxPortFloatTextN portNumCores;
	HxPortFloatTextN portSelectParams;
	HxPortDoIt portDoIt;

    /// Update method
    virtual void  update (void);

    /// Compute method
    virtual void  compute (void);

  protected:

  private:

};

#endif
