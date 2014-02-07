/////////////////////////////////////////////////////////////////
//
// Do not use CVS keywords because they cause merge conflicts and make branching unnecessarily hard.
//
// 
/////////////////////////////////////////////////////////////////

#ifndef _HX_STANDARDIZE_WORM_h
#define _HX_STANDARDIZE_WORM_h


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
#include <hxcore/HxPortToggleList.h>

#include <mclib/McMat3f.h>

class HXWORM_API HxStandardizeWorm : public HxCompModule
{
      HX_HEADER(HxStandardizeWorm);

  public:

	/// Default constructor.
    HxStandardizeWorm (void);

    /// Destructor.
    ~HxStandardizeWorm (void);

    HxConnection portRefSurface;

    HxPortDoIt portDoIt;

    /// Update method
    virtual void  update (void);

    /// Compute method
    virtual void  compute (void);


  protected:

  private:

    
};

#endif
