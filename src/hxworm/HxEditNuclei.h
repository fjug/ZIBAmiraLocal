/////////////////////////////////////////////////////////////////
//
// Do not use CVS keywords because they cause merge conflicts and make branching unnecessarily hard.
//
// 
/////////////////////////////////////////////////////////////////

#ifndef _HX_EDIT_NUCLEI_h
#define _HX_EDIT_NUCLEI_h


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

#include <hxlattice/HxObliqueSlice.h>
#include <hxsurftools/HxDisplaySurface.h>
#include <hxlines/HxLineSet.h>
#include <hxlines/HxDisplayLineSet.h>

#include <hxworm/WormHelpers.h>

class HXWORM_API HxEditNuclei : public HxCompModule
{
      HX_HEADER(HxEditNuclei);

  public:

	/// Default constructor.
    HxEditNuclei (void);

    /// Destructor.
	~HxEditNuclei (void);

	HxConnection portImage;
	
	HxConnection portTemplateSurf;

	HxPortToggleList portOptions;

	HxPortFloatTextN portAdaptParams;
    
	HxPortIntSlider portShowEllipse; 

	HxPortButtonList portActions;
	HxPortButtonList portUndo;



    /// Update method
    virtual void  update (void);

    /// Compute method
    virtual void  compute (void);

  protected:

	enum adaptParams { PROFLENGTH, NUMSMP, THRESH, TOL, GRADTHRESH, OUTSIDE };
	enum actions { FOCUS, REMOVE, SPLIT, MERGE, GROW, SHRINK};
	enum undoActions { UNDO, REDO };

	McDArray<McString> mSliceLabel;

	McVec4i viewerMasks;

	void showEllipse(int id, bool updateView = 1);

	void splitEllipse(int id);

	void mergeEllipse(int id);

	void growEllipse(int id);

	void shrinkEllipse(int id);

	void rescaleEllipse(int id, float scaleFactor);

	void addNucleus(McVec3f center, int atId=-1, SbMatrix trafo=SbMatrix::identity());

	struct UndoData {
		McHandle<HxSurface> surface;
		McDArray< SbRotation > camOr;
		McDArray< SbVec3f > camPos;
		McDArray< SoSFFloat > camHeight;
		McDArray<float> translates;
		int slider;
	};

	McDArray<UndoData> undoBuffer;
	McDArray<UndoData> redoBuffer;

	// xy: viewer 1; yz: viewer2, xz: viewer3
	McSmallArray< McHandle<HxObliqueSlice>,3 > mSlices;
	McSmallArray< McHandle<HxOverlayGrid>,3 > mIntersects;
	McSmallArray< McHandle<HxOverlayGrid>,3 > mIntersects2;

	McHandle<HxDisplaySurface> mSurfView;
	McHandle<HxLineSet> mFocus;
	McHandle<HxDisplayLineSet> mFocusView;

	/// Callback method.
    static void pickCB(void *that, SoEventCallback* node)
	{
		HxEditNuclei* editor = (HxEditNuclei*) that;
		editor->pickCallback(node);
	}
    static int keyCB(const SoEvent *event, HxViewer *viewer, void *that)
	{
		HxEditNuclei* editor = (HxEditNuclei*) that;
		if ( event->isOfType(SoKeyboardEvent::getClassTypeId()) )
			return (editor->keyCallback(event));
		return 0;
	}

    /// Method called after picking some geometry.
    void pickCallback(SoEventCallback* node);

	int keyCallback(const SoEvent *event);

    /// Save the last pressed key.
    SoKeyboardEvent::Key m_keyPressed;

	/// Event callback.
    McHandle<SoEventCallback> mEventCallback;

	void saveUndoData (McDArray<UndoData> &buffer );
	void applyUndoData (UndoData &ud );

	void surfaceChanged (int newHighlight);
	void doFocus(int id);

	// find nucleus closest to position: 
	int findClosestNucleus(McVec3f point) {
		HxSurface *wormSurf = dynamic_cast<HxSurface *>(portData.source());
		float closest = std::numeric_limits<float>::max();
		int closestIdx=-1;
		McVec3f closestCenter(0);
		for (int i=0; i<wormSurf->patches.size(); i++) {
			McVec3f center = WormHelpers::getPatchCenter(wormSurf, i);
			float dist2 = (point-center).length2();
			if (dist2<closest) {
				closest=dist2;
				closestIdx=i;
				closestCenter=center;
			}
		}
		return closestIdx;
	}


  private:

};

#endif
