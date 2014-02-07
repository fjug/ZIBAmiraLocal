/////////////////////////////////////////////////////////////////
//
// Do not use CVS keywords because they cause merge conflicts and make branching unnecessarily hard.
//
// 
/////////////////////////////////////////////////////////////////

#ifndef _WORM_HELPERS_h
#define _WORM_HELPERS_h


#include <hxworm/api.h>

#include <hxsurface/HxSurface.h>
#include <hxsurface/HxSurfaceField.h>
#include <hxsurface/HxSurfaceVectorField.h>
#include <hxsurface/HxSurfaceScalarField.h>

#include <hxtransforms/HxTransforms.h>
#include <hxfield/HxUniformScalarField3.h>
#include <hxfield/HxUniformLabelField3.h>

class HXWORM_API WormHelpers 
{
  public:

	/// Default constructor.
	  WormHelpers (void){};

    /// Destructor.
	  ~WormHelpers (void){};

	static struct trafoIdxAndFit {
		int trafoIdx;
		float fit;
		McDArray<float> radii;
	};

	static int
	compareBiggerSmallestFit(McDArray<WormHelpers::trafoIdxAndFit> const & t1, McDArray<WormHelpers::trafoIdxAndFit> const & t2)
	{
		int fit1 = t1.last().fit;
		int fit2 = t2.last().fit;
		if (fit1<fit2) return -1;
		if (fit2<fit1) return 1;
		return 0;
	}

	static struct DistToIdx {
		float dist;
		int idx;
	};

	static int
	compareDistToIdx(DistToIdx const & t1, DistToIdx const & t2)
	{
		if (t1.dist<t2.dist) return -1;
		if (t2.dist<t1.dist) return 1;
		return 0;
	}

	//static HxSurface* selectEllipses();
	static int getVoxelFromGridPos(const int* dims, const int* gridPos);
	static bool getGridPosFromPosition(const float* bbox, const int* dims, McVec3f voxelSize, McVec3f translation, int* gridPos);
	
	static int getVoxelFromPosition(const float* bbox, const int* dims, McVec3f voxelSize, McVec3f translation)
	{
		int gridPos[3];
		bool isInGrid = getGridPosFromPosition(bbox, dims, voxelSize, translation,gridPos);
		if (isInGrid) {
			return getVoxelFromGridPos(dims, gridPos);
		} else {
			return -1;
		}

	}

	static HxSurface* adaptFromOutside(HxSurface* surf, HxUniformScalarField3* image, HxUniformLabelField3* mask, float profLength, int numSamples, float thresh, float tol, float gradThresh, bool doLinearTrafo, float & fit);

	static McDArray<McVec3f> extractPatchCenterPoints(HxSurface* surf);
	static HxSurface* extractPatchCenterVertexSurf(HxSurface* surf);

	static HxSurface* extractPatches(int from, int to, HxSurface* surf){

		HxSurface* extractedSurf = dynamic_cast<HxSurface*>(surf->duplicate());
		int numPatches = surf->patches.size();
		if (from<0 || to<0 || from>=numPatches || to>=numPatches) {
			return NULL;
		}

		// remove tris with patchIdx <from and >to:
		for (int p=0; p<from; p++) {
			McDArray<int> tris = extractedSurf->patches[p]->triangles;
			for (int t=0; t<tris.size(); t++) {
				extractedSurf->triangles[tris[t]].patch=-1;
			}
		}
		for (int p=to+1; p<numPatches; p++) {
			McDArray<int> tris = extractedSurf->patches[p]->triangles;
			for (int t=0; t<tris.size(); t++) {
				extractedSurf->triangles[tris[t]].patch=-1;
			}
		}
		// assign correct patch num to remaining tris:
		for (int p=from; p<=to; p++) {
			//give correct material index
			extractedSurf->patches[p]->innerRegion=p+1;
		}

		extractedSurf->removeObsoleteTriangles();
		extractedSurf->removeEmptyPatches();
		extractedSurf->removeUnusedPoints();

		extractedSurf->fire();

		theMsg->printf("extracted surf num points: %d", extractedSurf->getNumPoints());

		return extractedSurf;
	}


	static McHandle<HxUniformLabelField3> scanConvertSmall(HxSurface* surf, HxUniformLabelField3* field, int &volumeOverlap);

	static void doGht(HxSurface* templateSurf, HxUniformScalarField3* image, HxUniformLabelField3* maskImage, const float* scaleRange, int* nGhtSteps, int maxNumTrafos, int numThreads, const float* wsParams, bool smoothAndResampleForWs, HxUniformScalarField3* &accuImage, HxTransforms* &trafos, HxUniformScalarField3* &wsImage );

	static void removePatch(HxSurface* surf, int patchIdx){
		int numPatchesOld = surf->patches.size();

		McDArray<int> tris = surf->patches[patchIdx]->triangles;
		// remove tris from patchIdx:
		for (int t=0; t<tris.size(); t++) {
			surf->triangles[tris[t]].patch=-1;
		}

		// assign correct patch num to tris above patchIdx:
		for (int p=patchIdx+1; p<numPatchesOld; p++) {
			//give correct material index
			surf->patches[p]->innerRegion--;
		}

		surf->removeObsoleteTriangles();
		surf->removeEmptyPatches();
		surf->removeUnusedPoints();

		surf->fire();
	}

	static void insertOnePatch(HxSurface* surf, int patchIdx, HxSurface* patchSurf)
	{
		int numPatchesOld = surf->patches.size();

		// create new material:
		McString materialName;
		materialName.printf("Material%d",numPatchesOld);
		patchSurf->parameters.materials()->bundle(1)->setName(materialName);

		surf->merge(*patchSurf);
		HxSurface::Patch* newPatch; 
		surf->patches.pop(newPatch);

		for (int p=patchIdx; p<numPatchesOld; p++) {
			HxSurface::Patch* patch = surf->patches[p];
			for (int i=patch->triangles.size()-1; i>=0; i--) {
				surf->triangles[patch->triangles[i]].patch++;
			}
			//give correct material index
			patch->innerRegion++;
		}

		for (int i=newPatch->triangles.size()-1; i>=0; i--) {
			surf->triangles[newPatch->triangles[i]].patch = patchIdx;
		}
		newPatch->innerRegion=patchIdx+1;

		surf->patches.insert(patchIdx,newPatch);
	}

	static void swapPatches(HxSurface* surf, int patchIdx1, int patchIdx2)
	{
		if (patchIdx1>=surf->patches.size() || patchIdx2>=surf->patches.size() || patchIdx1<0 || patchIdx2<0 || patchIdx1==patchIdx2) {
			return;
		}

		int i;
		HxSurface::Patch* patch;

		patch = surf->patches[patchIdx1];
		for (i=patch->triangles.size()-1; i>=0; i--)
			surf->triangles[patch->triangles[i]].patch = patchIdx2;

		patch = surf->patches[patchIdx2];
		for (i=patch->triangles.size()-1; i>=0; i--)
			surf->triangles[patch->triangles[i]].patch = patchIdx1;

		surf->patches[patchIdx2] = surf->patches[patchIdx1];
		surf->patches[patchIdx1] = patch;
	}


	static McVec3f getPatchCenter(HxSurface* surf, int patchIdx)
	{
		HxSurface::Patch* patch = surf->patches[patchIdx];
		McVec3f center(0);
		int numPoints=0;
		for (int triIdx=0; triIdx<patch->triangles.size(); triIdx++) {
			for (int pointIdx=0; pointIdx<3; pointIdx++) {
				int p=surf->triangles[patch->triangles[triIdx]].points[pointIdx];
				center += surf->points[p];
				numPoints++;
			}
		}
		center/=numPoints;
		return center;
	}

	static void getPrincipalAxes(HxSurface* surf, McDArray<McVec3f>& eVecs, McVec3d& eVals, McVec3d& radiiMin, McVec3d& radiiMax);

	static void getWormCoordSystem(HxSurface* surf, McVec3f& center, McVec3f& apAxis, McVec3f& dvAxis, McVec3f& lrAxis, McVec3d& extMin, McVec3d& extMax);
	static  HxSurface* cropToBWM(HxSurface* surf, McVec3f apAxis); 


};

#endif
