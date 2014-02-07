////////////////////////////////////////////////////////////////
//
// Do not use CVS keywords because they cause merge conflicts and make branching unnecessarily hard.
//
// // Trims vectors in a SurfaceVectorField so that all surfaces generated by Displaces along the vectors are intersection-free
//
//
/////////////////////////////////////////////////////////////////
#define _USE_MATH_DEFINES
#include <math.h>

#include <hxcore/HxData.h>
#include <hxcore/HxWorkArea.h>
#include <hxcore/HxMessage.h>
#include <hxcore/HxObjectPool.h>
#include <hxcore/HxController.h>
#include <mclib/McDMatrix.h>
#include <mclib/McMat2d.h>
#include <hxworm/WormHelpers.h>
#include <hxlines/HxLineSet.h>


#ifdef _OPENMP
#include <omp.h>
#endif

#include <hxworm/HxStandardizeWorm.h>


HX_INIT_CLASS(HxStandardizeWorm, HxCompModule);


/// constructor of surface displacement calculation
HxStandardizeWorm::HxStandardizeWorm (void) :
    HxCompModule(HxSurface::getClassTypeId()),
    portRefSurface(this,"referenceWorm",HxSurface::getClassTypeId()),
    portDoIt(this, "doIt")
{
}


/// destructor of surface displacement calculation
HxStandardizeWorm::~HxStandardizeWorm (void)
{
}


/// update: handles any changes of the user interface
void HxStandardizeWorm::update (void)
{
}


/// the computational part of this module
void HxStandardizeWorm::compute (void)
{
	if (!portDoIt.wasHit()) return;

	HxSurface *surfToStandardize = dynamic_cast<HxSurface *>(portData.source());
	HxSurface *surfReference = dynamic_cast<HxSurface *>(portRefSurface.source());

	if (!surfToStandardize || !surfReference ) return;

	// get material list from reference
	HxParamBundle* refMaterials = surfReference->parameters.materials();
	McDArray<std::string> refMatList(0);
	int numBundles = refMaterials->nBundles();
	// remove Bundle*, Internal, External
	for (int i=0; i<numBundles; i++) {
		std::string matName = refMaterials->bundle(i)->name();
		if ( matName.compare("Exterior")==0 ) continue;
		if ( matName.compare("Interior")==0 ) continue;
		if ( (matName.substr(0,6)).compare("Bundle")==0 ) continue;
		refMatList.append(matName);
	}
	// sort refMatList
	refMatList.sort(mcStandardCompare);

	// init list of patches (NULL)
	McDArray< McHandle<HxSurface> > targetPatches(refMatList.size());
	targetPatches.fill(NULL);
	// init vertex surf: #points: number of reference materials, all set to 0 0 0
	McHandle<HxSurface> vertexSurf = new HxSurface(refMatList.size(),0,0,1);
	vertexSurf->points.fill(McVec3f(0,0,0));
	double dummy0[3] = {0,0,0};
	for (int m=0; m< refMatList.size(); m++) {
		// init materials
		HxParamBundle* targetMaterials = vertexSurf->parameters.materials();
		HxParamBundle* myMat = new HxParamBundle(refMatList[m].data());
		targetMaterials->insert(myMat);
	}
	// init vertex surf weights: all set to 0
	McHandle<HxSurfaceScalarField> vertexSurfWeights = dynamic_cast<HxSurfaceScalarField*>(HxSurfaceField::create(vertexSurf, HxSurfaceField::OnNodes, 1));
	memset(vertexSurfWeights->dataPtr(), 0, vertexSurfWeights->nDataElements()*sizeof(float));

	// for all patches in surfToStandardize
	for (int p=0; p<surfToStandardize->patches.size(); p++) {
		int matNumber = p+1;
		if (matNumber >= surfToStandardize->parameters.materials()->nBundles()) { 
			McString msg;
			theMsg->warning(msg.printf("%s does not have material %d",surfToStandardize->getLabel().dataPtr(), matNumber));
			break;
		}
		// fit ellipse
		McDArray<McVec3f> eVecs;
		McVec3d eVals, radiiMin, radii;
		McHandle<HxSurface> nucleus = WormHelpers::extractPatches(p, p, surfToStandardize);
		WormHelpers::getPrincipalAxes(nucleus, eVecs, eVals, radiiMin, radii);

		// insert radii of patch as params into surfToStandardize (and nucleus)
		double radiiDummy[3] = {radii[0],radii[1],radii[2]};
		surfToStandardize->parameters.materials()->bundle(matNumber)->set("Radii",3,radiiDummy);
		
		// getMaterialName of patch
		std::string matName = surfToStandardize->parameters.materials()->bundle(matNumber)->name();
		
		// if material in refMatList at index rmlIdx
		int refMatListIdx = refMatList.findSorted(matName, mcStandardCompare);
		if ( refMatListIdx >-1) {
			// move position of vertex no. rmlIdx in vertex surf to center point of patch
			McVec3f patchCenter = WormHelpers::getPatchCenter(surfToStandardize,p);
			vertexSurf->points[refMatListIdx] = patchCenter;

			// set weight rmlIdx to 1
			vertexSurfWeights->dataPtr()[refMatListIdx]=1;
			// insert eigenvalues as params of material[rmlIdx] into vertexSurf
			vertexSurf->parameters.materials()->bundle(refMatList[refMatListIdx].data())->set("Radii",3,radiiDummy);
			// list of patches [rmlIdx] = patch
			nucleus->parameters.materials()->removeAll();
			nucleus->parameters.materials()->insert(surfToStandardize->parameters.materials()->bundle("Exterior")->duplicate());
			nucleus->parameters.materials()->insert(vertexSurf->parameters.materials()->bundle(refMatList[refMatListIdx].data())->duplicate());
			nucleus->patches[0]->innerRegion = 1;
			nucleus->patches[0]->outerRegion = 0;
			targetPatches[refMatListIdx]=nucleus;
		}
	}
	// assemble patch surf
	HxSurface* patchSurf = new HxSurface(0,0,0,1);
	for (int m=0; m< refMatList.size(); m++) {
		if (targetPatches[m]) {
			patchSurf->merge(*targetPatches[m]);
		} else {
			//patchSurf->merge(*dummyPatch);
		}
	}

	// set weights (lookup in vertex weights)
	// set material names and eigenvalues as params ( = copy materials from vertex surf)

	setResult(0,vertexSurf);
	setResult(1,vertexSurfWeights);
	setResult(2,patchSurf);

}