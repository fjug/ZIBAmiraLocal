/////////////////////////////////////////////////////////////////
//
// Do not use CVS keywords because they cause merge conflicts and make branching unnecessarily hard.
//
// 
/////////////////////////////////////////////////////////////////

#ifndef _HX_SELECT_ELLIPSES_h
#define _HX_SELECT_ELLIPSES_h


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

#include <hxworm/WormHelpers.h>

class HXWORM_API HxSelectEllipses : public HxCompModule
{
      HX_HEADER(HxSelectEllipses);

  public:

	/// Default constructor.
    HxSelectEllipses (void);

    /// Destructor.
    ~HxSelectEllipses (void);

    HxConnection portSortedTrafos;
	
	HxConnection portImage;
	
	HxConnection portAccu;

    HxConnection portWatershedImg;

	HxPortToggleList portOptions;

	HxPortFloatTextN portSelectParams;

	HxPortFloatTextN portAdaptParams;
    
	HxPortDoIt portDoIt;

    /// Update method
    virtual void  update (void);

    /// Compute method
    virtual void  compute (void);

  protected:

	enum selectParams { MAXNO, FIRSTIDX, STARTTRAFO, ERODEBY };
	enum adaptParams { PROFLENGTH, NUMSMP, THRESH, TOL, GRADTHRESH, OUTSIDE };

	HxSurface* selectEllipses();
	
	McHandle<HxSurface> checkEllipse(int trafoID, float & fit);

	HxSurface* adapt(HxSurface* surf, HxUniformLabelField3* mask, float & fit);
	bool checkOverlap(HxSurface* surf, HxUniformLabelField3* labels, int maxErode, int &erode);
	void updateAuxiliaryFields(HxSurface* newEllipse, int erode, int wsCompartment);


  private:

	McDArray<int> mVoxelIdPerTrafo;
	
	McHandle<HxUniformLabelField3> mMergeSurfLabels;
	McHandle<HxUniformLabelField3> mMergeSurfLabelsEroded;

	McHandle<HxSurface> mTemplateSurfOnePatch;
};

#endif
