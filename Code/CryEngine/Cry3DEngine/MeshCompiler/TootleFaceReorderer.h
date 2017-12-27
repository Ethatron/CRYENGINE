// Copyright 2001-2017 Crytek GmbH / Crytek Group. All rights reserved. 

#pragma once
#ifndef __TootleFaceReorderer_h__
#define __TootleFaceReorderer_h__
#include <tootlelib.h>

class TootleFaceReorderer
{
public:
	enum TootleAlgorithm
	{
		NA_TOOTLE_ALGORITHM,                   // Default invalid choice.
		TOOTLE_VCACHE_ONLY,                    // Only perform vertex cache optimization.
		TOOTLE_VCACHE_OVERDRAW,                // Don't use clustering
		TOOTLE_CLUSTER_VCACHE_OVERDRAW,        // Call the clustering, optimize vertex cache and overdraw individually.
		TOOTLE_APPROX_CLUSTER_VCACHE_OVERDRAW, // Call the functions to optimize vertex cache and overdraw individually.  This is using
		                                       // the algorithm from SIGGRAPH 2007.
	};

	TootleFaceReorderer(const unsigned int indexCountMax, const unsigned int vpC = 0, const float* vpP = nullptr);
	~TootleFaceReorderer();

	bool reorderFaces(
		const size_t indexCount,
		const unsigned int* const inVertexIndices,
		const size_t positionsCount, // equates max(index)
		const void* const inVertexPositions,
		unsigned int* const outVertexIndices,

		const unsigned int clusterLimit = 0,
		unsigned int* const outFaceClusters = nullptr,
		const unsigned int algorithmComplexity = 1,
		const TootleAlgorithm algorithmChoice = TOOTLE_CLUSTER_VCACHE_OVERDRAW,
		const TootleFaceWinding windingOrder = TOOTLE_CCW,
		const unsigned int cacheSize = TOOTLE_DEFAULT_VCACHE_SIZE);

	bool reorderVertices(
		const size_t indexCount,
		const unsigned int* const inVertexIndices,
		const size_t positionsCount, // equates max(index)
		const void* const inVertexPositions,
		unsigned int* const outVertexIndices,
		void* const outVertexPositions,

		unsigned int* const outVertexMap = nullptr,
		const TootleFaceWinding windingOrder = TOOTLE_CCW,
		const unsigned int cacheSize = TOOTLE_DEFAULT_VCACHE_SIZE);

private:
	std::vector<uint32> indicesT;
	uint32* tmpVertexIndices;

	const float* viewpointPositions;
	unsigned int viewpointCount;

public:
	float        fVCacheIn;
	float        fVCacheOut;
	float        fOverdrawIn;
	float        fOverdrawOut;
	float        fMaxOverdrawIn;
	float        fMaxOverdrawOut;
};

#endif
