/////////////////////////////////////////////////////////////////
//
// Do not use CVS keywords because they cause merge conflicts and make branching unnecessarily hard.
//
// 
/////////////////////////////////////////////////////////////////

#ifndef _HX_ALIGN_WORMS_h
#define _HX_ALIGN_WORMS_h


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

class HXWORM_API HxAlignWorms : public HxCompModule
{
      HX_HEADER(HxAlignWorms);

  public:

	/// Default constructor.
    HxAlignWorms (void);

    /// Destructor.
    ~HxAlignWorms (void);

    HxConnection portTargetSurface;

	HxPortRadioBox portTrafo;
    HxPortDoIt portDoIt;

    /// Update method
    virtual void  update (void);

    /// Compute method
    virtual void  compute (void);


  protected:


  private:

    
};

#endif
