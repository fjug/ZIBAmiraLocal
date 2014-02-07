/////////////////////////////////////////////////////////////////
//
// Do not use CVS keywords because they cause merge conflicts and make branching unnecessarily hard.
//
// 
/////////////////////////////////////////////////////////////////

#ifndef _HX_WORM_ATLAS_REG_h
#define _HX_WORM_ATLAS_REG_h


#include <hxworm/api.h>

#include <hxsurface/HxSurface.h>
#include <hxsurface/HxSurfaceField.h>
#include <hxsurface/HxSurfaceVectorField.h>
#include <hxsurface/HxSurfaceScalarField.h>

#include <hxactiveshape/HxActiveShape.h>

#include <hxcore/HxCompModule.h>
#include <hxcore/HxConnection.h>
#include <hxcore/HxPortDoIt.h>
#include <hxcore/HxPortFloatTextN.h>
#include <hxcore/HxPortRadioBox.h>
#include <hxcore/HxPortToggleList.h>
#include <hxcore/HxPortToggleList.h>

#include <mclib/McMat3f.h>

#include <boost/config.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
using namespace boost;

//#define GraphType adjacency_list < listS, vecS, directedS, no_property, property < edge_weight_t, int > >
//#define TrafoType SbMatrix

/// creates a surface vector field with normal vectors defined on all nodes
class HXWORM_API HxWormAtlasReg : public HxCompModule
{
      HX_HEADER(HxWormAtlasReg);

  public:

	/// Default constructor.
    HxWormAtlasReg (void);

    /// Destructor.
    ~HxWormAtlasReg (void);

    HxConnection portTargetSurface;

	HxPortFloatTextN portInit;
	HxPortFloatTextN portGroupingParams;
	HxPortFloatTextN portBestGroupTrafoParams;
	HxPortFloatTextN portGraphMatching;
	HxPortFloatTextN portSpatialDistParams;
	HxPortFloatTextN portColorParams;
	HxPortFloatTextN portTargetCostParams;
	HxPortFloatTextN portIncentiveToMatch;
	HxPortFloatTextN portShapeParams;
	HxPortFloatTextN portBinaryParams;
	HxPortToggleList portGraphMatchingSubproblems;
	
	HxPortFloatTextN portActiveMatchingIterations;
	HxPortRadioBox portActiveMatchingTrafo;
	HxPortFloatTextN portActiveMatchingModes;

	HxPortFloatTextN portRotInvariance;

	HxPortRadioBox	portNorm;

    HxPortDoIt portDoIt;

	enum pgm {MAXITER, MAXGAP, LOCALNEIGHS, MAXNUMNEIGHS};
	enum psd {SPATIALDISTWEIGHT, MAXDISTMATCHED};
	enum pcol {COLORDISTWEIGHT, MAXCOLDISTMATCHED, MINCOL};
	enum ptc {TARGETCOSTWEIGHT, MAXTARGETCOSTCONSIDERED};
	enum pitm {INCENTIVETOMATCH};
	enum pshd {SHAPEDISTWEIGHT, MAXSHAPEDISTMATCHED};
	enum pbp {DIFFSPATIALDISTWEIGHT,ANGLEDIFFERENCEWEIGHT};
	enum norm {L1,L2};

    /// Update method
    virtual void  update (void);

    /// Compute method
    virtual void  compute (void);

    /// Parse method.
    virtual int parse(Tcl_Interp* t, int argc, char **argv);


  protected:

    typedef SbMatrix TrafoType;
    typedef adjacency_list < listS, vecS, directedS,
    no_property, property < edge_weight_t, int > > GraphType;

	McDArray<McDArray<float>> computeDistanceMatrix(McDArray<McVec3f> &centerPoints);
	McDArray<McDArray<float>> computeAtlasProximityMatrix();

	struct Assignment {
		int i0;
		int i1;
		float cost;
		float angle;
	};

	struct Edge {
		int a;
		int b;
		float cost;
	};

	McDArray<McDArray<float>> computeCorrespondingNuclei(HxSurface * atlasSurf, HxSurface * targetSurf);

	GraphMatching* setupGraphMatching();
	McVec2f getSpatialDistance(int atlasIdx,int targetIdx);
	float getColorDistance(int atlasIdx,int targetIdx);
	float getShapeDistance(int atlasIdx,int targetIdx);
	float getTargetCost(int targetIdx);
	float getIncentiveToMatch(int atlasIdx);
	bool areNeighborsInAtlas(int atlasIdx0,int atlasIdx1);
	float getDifferenceOfSpatialDistances(Assignment a, Assignment b);

	McDArray<McVec3f> mAtlasCenterPoints;
	McDArray<McVec3f>  mTargetCenterPoints;

	McVec3f mAtlasCenter;
	McVec3f mAtlasBodyAxis;
	McVec3f mTargetCenter;

	McDArray<McDArray<float>> mAtlasProximityMatrix;
	McDArray<McDArray<WormHelpers::DistToIdx>> mAtlasProximitiesSorted;
	McDArray<McDArray<int>> mAtlasNeighs;

	//obsolete:
//	McDArray<McDArray<float>> mAtlasDistMatrix;
//	McDArray<McDArray<float>> mTargetDistMatrix;

	McDArray<McMat3f> mInvCovMatrices;
	McDArray< McDArray<McMat3f> > mInvCovDiffMatrices;

	McDArray<McMat3f> mInvCovShapeMatrices;
	McDArray<McVec3f> mMeanShapeVecs;
	McDArray<McVec3f> mTargetRadii;


	GraphType* buildWormGraph(McDArray<McDArray<float>>& distMatrix, float maxNeighDist);
	bool isConnected(GraphType* graph);
	float computeWormLength(std::vector<float> &d_vec); 

	McDArray<McDArray<int>> groupNuclei(std::vector<float> &d_vec, float distThresh);
	McDArray<McVec3f> extractGroupPoints(McDArray<int> group, McDArray<McVec3f> & centerPoints);

	McVec3f computeGroupAxis(McDArray<int> group, McDArray<McVec3f>& centerPoints, std::vector<float> &d_vec);
	TrafoType computeStartTrafo(McDArray<McVec3f> &atlasGroupPoints, McVec3f atlasGroupAxis, float atlasWormLength, McDArray<McVec3f> &targetGroupPoints, McVec3f targetGroupAxis, float targetWormLength); 
	void computeBestTrafos(McDArray<McVec3f> &atlasGroupPoints, McVec3f atlasGroupAxis, float atlasWormLength, McDArray<McVec3f> &targetGroupPoints, McVec3f targetGroupAxis, float targetWormLength, 
		int numTrafos, int numAngles, float* offsetRange, int* offsetSampling, McDArray<TrafoType> &resultTrafos, McDArray<float>& resultTrafoResiduals);

	GraphType* buildTrafoGraph(McDArray<McDArray<TrafoType>> bestGroupTrafos);
	void computeBestTrafoSequence(GraphType* trafoGraph){};

	std::vector<float>* computeWormDistanceMap(int startIdx, GraphType* wormGraph);

	McDArray<SbColor> mAtlasColors;
	McDArray<SbColor> mTargetColors;
	McDArray<SbColor> extractPatchColors(HxSurface * surf);
	McDArray<McVec3f> extractPatchRadii(HxSurface * surf);


	bool considerAtlasIdx(int idx)
	{
		float minCol = portColorParams.getValue(MINCOL);
		return (mAtlasColors[idx].length()>minCol);
	}
	bool considerTargetIdx(int idx)
	{
		float minCol = portColorParams.getValue(MINCOL);
		float col = mTargetColors[idx].length();
		float maxCost = portTargetCostParams.getValue(MAXTARGETCOSTCONSIDERED);
		float cost = getTargetCost(idx); 
		return (col>minCol && cost<maxCost);
	}


	bool considerAssignment(int atlasIdx, int targetIdx)
	{
		// ToDo: learn from atlas; per nucleus!
		float maxDistanceOfMatchedNuclei = portSpatialDistParams.getValue(MAXDISTMATCHED);
		float maxColorDistanceOfMatchedNuclei = portColorParams.getValue(MAXCOLDISTMATCHED);
		float maxShapeDistanceOfMatchedNuclei = portShapeParams.getValue(MAXSHAPEDISTMATCHED);

		McVec2f spatialDistance = getSpatialDistance(atlasIdx,targetIdx);
		if (spatialDistance[0]>maxDistanceOfMatchedNuclei) {
			return false;
		}
		float colorDistance = getColorDistance(atlasIdx,targetIdx);
		if (colorDistance>maxColorDistanceOfMatchedNuclei) {
			return false;
		}
		float shapeDistance = getShapeDistance(atlasIdx,targetIdx);
		if (shapeDistance>maxShapeDistanceOfMatchedNuclei) {
			return false;
		}
		return true;
	}

	float getLengthA(McVec3f offset, int atlasIdx)
	{
		McMat3f invCov = mInvCovMatrices[atlasIdx];
		McVec3f dummy;
		invCov.multMatrixVec(offset,dummy);
		float ret = offset.dot(dummy);
		int norm = portNorm.getValue();
		if (norm==L1) {
			return std::sqrt(ret);
		} else if (norm==L2) { 
			return ret;
		}
	}

	float getLengthDiffAB(McVec3f offset, int atlasIdx0, int atlasIdx1)
	{
		McMat3f invCovDiff = mInvCovDiffMatrices[atlasIdx0][atlasIdx1];
		McVec3f dummy;
		invCovDiff.multMatrixVec(offset,dummy);
		float ret = offset.dot(dummy);
		int norm = portNorm.getValue();
		if (norm==L1) {
			return std::sqrt(ret);
		} else if (norm==L2) { 
			return ret;
		}
	}

  private:

    
};

#endif
