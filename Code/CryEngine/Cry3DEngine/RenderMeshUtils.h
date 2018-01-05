// Copyright 2001-2017 Crytek GmbH / Crytek Group. All rights reserved. 

// -------------------------------------------------------------------------
//  File name:   RenderMeshUtils.h
//  Created:     14/11/2006 by Timur.
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __RenderMeshUtils_h__
#define __RenderMeshUtils_h__
#pragma once

struct SIntersectionData;

//////////////////////////////////////////////////////////////////////////
// RenderMesh utilities.
//////////////////////////////////////////////////////////////////////////
class CRenderMeshUtils : public Cry3DEngineBase
{
public:
	// Do a render-mesh vs Ray intersection, return true for intersection.
	static bool RayIntersection(IRenderMesh* pRenderMesh, SRayHitInfo& hitInfo, IMaterial* pMtl = 0);
	static bool RayIntersectionFast(IRenderMesh* pRenderMesh, SRayHitInfo& hitInfo, IMaterial* pCustomMtl = 0);

	// async versions, aren't using the cache, and are used by the deferredrayintersection class
	static void RayIntersectionAsync(SIntersectionData* pIntersectionRMData);

	static void ClearHitCache();

	//////////////////////////////////////////////////////////////////////////
	//void FindCollisionWithRenderMesh( IRenderMesh *pRenderMesh, SRayHitInfo &hitInfo );
	//	void FindCollisionWithRenderMesh( IPhysiIRenderMesh2 *pRenderMesh, SRayHitInfo &hitInfo );
private:
	// functions implementing the logic for RayIntersection
	static bool RayIntersectionImpl(SIntersectionData* pIntersectionRMData, SRayHitInfo* phitInfo, IMaterial* pCustomMtl, bool bAsync);
	static bool RayIntersectionFastImpl(SIntersectionData& rIntersectionRMData, SRayHitInfo& hitInfo, IMaterial* pCustomMtl, bool bAsync);
	static bool ProcessBoxIntersection(Ray& inRay, SRayHitInfo& hitInfo, SIntersectionData& rIntersectionRMData, IMaterial* pMtl,
		vtx_idx* pInds, int nVerts, strided_pointer<Vec3> pPos32, strided_pointer<Vec2> pUV32, strided_pointer<Vec3f16> pPos16, strided_pointer<Vec2f16> pUV16, strided_pointer<UCol> pCol,
		int nInds, bool& bAnyHit, float& fBestDist, Vec3& vHitPos, Vec3* tri);
};

// struct to collect parameters for the wrapped RayInterseciton functions
struct SIntersectionData
{
	SIntersectionData() :
		pRenderMesh(NULL), nVertices(0), nIndices(0),
		pPositionsF32(NULL), pPositionsF16(NULL), pIndices(NULL),
		pTexCoordsF32(NULL), pTexCoordsF16(NULL),
		pColours(NULL),
		pTangents(NULL),
		bResult(false), bNeedFallback(false),
		fDecalPlacementTestMaxSize(1000.f), bDecalPlacementTestRequested(false),
		pHitInfo(0), pMtl(0)
	{
	}

	bool Init(IRenderMesh* pRenderMesh, SRayHitInfo* _pHitInfo, IMaterial* _pMtl, bool _bRequestDecalPlacementTest = false);

	IRenderMesh* pRenderMesh;
	SRayHitInfo* pHitInfo;
	IMaterial*   pMtl;
	bool         bDecalPlacementTestRequested;

	int          nVertices;
	int          nIndices;

	vtx_idx*     pIndices;
	strided_pointer<Vec3> pPositionsF32;
	strided_pointer<UCol> pColours;
	strided_pointer<Vec2> pTexCoordsF32;
	strided_pointer<Vec3f16> pPositionsF16;
	strided_pointer<Vec2f16> pTexCoordsF16;
	strided_pointer<SPipTangents> pTangents;

	bool         bResult;
	float        fDecalPlacementTestMaxSize; // decal will look acceptable in this place
	bool         bNeedFallback;
};

#endif // __RenderMeshUtils_h__
