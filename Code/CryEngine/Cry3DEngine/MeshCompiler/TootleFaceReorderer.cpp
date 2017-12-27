// Copyright 2001-2017 Crytek GmbH / Crytek Group. All rights reserved. 

#include "StdAfx.h"

#if CRY_PLATFORM_WINDOWS
#if USE_TOOTLE

#include "TootleFaceReorderer.h"

TootleFaceReorderer::TootleFaceReorderer(
	const unsigned int indexCountMax,
	const unsigned int vpC,
	const float* vpP)
{
	TootleResult bOK = TootleInit();

	indicesT.resize(indexCountMax);
	tmpVertexIndices = &indicesT[0];

	viewpointCount     = vpC;
	viewpointPositions = vpP;
}

TootleFaceReorderer::~TootleFaceReorderer()
{
	TootleCleanup();
}

//=================================================================================================================================
/// Enumeration for the choice of test cases for tootle.
//=================================================================================================================================

bool TootleFaceReorderer::reorderFaces(
	const size_t indexCount,
	const unsigned int* const inVertexIndices,
	const size_t positionsCount, // equates max(index)
	const void* const inVertexPositions,
	unsigned int* const outVertexIndices,
	const unsigned int clusterLimit,
	unsigned int* const outFaceClusters,
	const unsigned int algorithmComplexity,
	const TootleAlgorithm algorithmChoice,
	const TootleFaceWinding windingOrder,
	const unsigned int cacheSize)
{
	const unsigned int faceCount = indexCount / 3;
	const unsigned int positionStride = 3 * sizeof(float);

	const TootleOverdrawOptimizer optimizerDraw  = algorithmComplexity == 0 ? TOOTLE_OVERDRAW_FAST : TOOTLE_OVERDRAW_AUTO;
	const TootleVCacheOptimizer   optimizerCache = algorithmComplexity == 0 ? TOOTLE_VCACHE_TIPSY  : TOOTLE_VCACHE_AUTO;

	if (true)
	{
		// measure input VCache efficiency
		if (TootleMeasureCacheEfficiency(
			inVertexIndices,
			faceCount,
			cacheSize,
			&fVCacheIn) != TOOTLE_OK)
		{
			return false;
		}
	}

	if (true)
	{
		// measure input overdraw.  Note that we assume counter-clockwise vertex winding.
		if (TootleMeasureOverdraw(
			inVertexPositions,
			inVertexIndices,
			positionsCount,
			faceCount,
			positionStride,
			viewpointPositions,
			viewpointCount,
			windingOrder,
			&fOverdrawIn,
			&fMaxOverdrawIn) != TOOTLE_OK)
		{
			return false;
		}
	}

	// **********************************************************************************************************************
	//   Optimize the mesh:
	//
	// The following cases show five examples for developers on how to use the library functions in Tootle.
	// 1. If you are interested in optimizing vertex cache only, see the TOOTLE_VCACHE_ONLY case.
	// 2. If you are interested to optimize vertex cache and overdraw, see either TOOTLE_CLUSTER_VCACHE_OVERDRAW
	//     or TOOTLE_OPTIMIZE cases.  The former uses three separate function calls while the latter uses a single
	//     utility function.
	// 3. To use the algorithm from SIGGRAPH 2007 (v2.0), see TOOTLE_FAST_VCACHECLUSTER_OVERDRAW or TOOTLE_FAST_OPTIMIZE
	//     cases.  The former uses two separate function calls while the latter uses a single utility function.
	//
	// Note the algorithm from SIGGRAPH 2007 (v2.0) is very fast but produces less quality results especially for the
	//  overdraw optimization.  During our experiments with some medium size models, we saw an improvement of 1000x in
	//  running time (from 20+ minutes to less than 1 second) for using v2.0 calls vs v1.2 calls.  The resulting vertex
	//  cache optimization is very similar while the overdraw optimization drops from 3.8x better to 2.5x improvement over
	//  the input mesh.
	//  Developers should always run the overdraw optimization using the fast algorithm from SIGGRAPH initially.
	//  If they require a better result, then re-run the overdraw optimization using the old v1.2 path (TOOTLE_OVERDRAW_AUTO).
	//  Passing TOOTLE_OVERDRAW_AUTO to the algorithm will let the algorithm choose between Direct3D or raytracing path
	//  depending on the total number of clusters (less than 200 clusters, it will use Direct3D path otherwise it will
	//  use raytracing path since the raytracing path will be faster than the Direct3D path at that point).
	//
	// Tips: use v2.0 for fast optimization, and v1.2 to further improve the result by mix-matching the calls.
	// **********************************************************************************************************************

	unsigned int clusterCount;

	TootleResult result = TOOTLE_OK;
	switch (algorithmChoice)
	{
		default:
			// wrong algorithm choice
			// break;

		case TOOTLE_VCACHE_ONLY:
			// *******************************************************************************************************************
			// Perform Vertex Cache Optimization ONLY
			// *******************************************************************************************************************

			// Optimize vertex cache
			if ((result = TootleOptimizeVCache(
				inVertexIndices,
				faceCount,
				positionsCount,
				cacheSize,
				outVertexIndices,
				nullptr, // faceMap
				optimizerCache)) != TOOTLE_OK)
			{
				break;
			}

			break;

		case TOOTLE_VCACHE_OVERDRAW:
			// *******************************************************************************************************************
			// Perform Vertex Cache Optimization ONLY
			// *******************************************************************************************************************

			// Assign cluster 0 to all faces
			for (unsigned int f = 0; f < faceCount; ++f)
				outFaceClusters[f] = 0;

			// The last entry of of outVertexClusters store the total number of clusters.
			clusterCount = outFaceClusters[faceCount] = 1;

			// Perform vertex cache optimization on the clustered mesh.
			if ((result = TootleVCacheClusters(
				tmpVertexIndices,
				faceCount,
				positionsCount,
				cacheSize,
				outFaceClusters,
				tmpVertexIndices,
				NULL,
				optimizerCache)) != TOOTLE_OK)
			{
				break;
			}

			// Optimize the draw order (using v1.2 path: TOOTLE_OVERDRAW_AUTO, the default path is from v2.0--SIGGRAPH version).
			if ((result = TootleOptimizeOverdraw(
				inVertexPositions,
				tmpVertexIndices,
				positionsCount,
				faceCount,
				positionStride,
				viewpointPositions,
				viewpointCount,
				windingOrder,
				outFaceClusters,
				outVertexIndices,
				NULL,
				optimizerDraw)) != TOOTLE_OK)
			{
				break;
			}

			break;

		case TOOTLE_CLUSTER_VCACHE_OVERDRAW:
			// *******************************************************************************************************************
			// An example of calling clustermesh, vcacheclusters and optimize overdraw individually.
			// This case demonstrate mix-matching v1.2 clustering with v2.0 overdraw optimization.
			// *******************************************************************************************************************

			if ((optimizerDraw != TOOTLE_OVERDRAW_FAST) ||
			    (outFaceClusters != nullptr))
			{
				// Cluster the mesh, and sort faces by cluster.
				if ((result = TootleClusterMesh(
					inVertexPositions,
					inVertexIndices,
					positionsCount,
					faceCount,
					positionStride,
					clusterLimit,
					tmpVertexIndices,
					outFaceClusters,
					NULL)) != TOOTLE_OK)
				{
					break;
				}

				// The last entry of of outVertexClusters store the total number of clusters.
				clusterCount = outFaceClusters[faceCount];

				// Perform vertex cache optimization on the clustered mesh.
				if ((result = TootleVCacheClusters(
					tmpVertexIndices,
					faceCount,
					positionsCount,
					cacheSize,
					outFaceClusters,
					tmpVertexIndices,
					NULL,
					optimizerCache)) != TOOTLE_OK)
				{
					break;
				}

				// Optimize the draw order (using v1.2 path: TOOTLE_OVERDRAW_AUTO, the default path is from v2.0--SIGGRAPH version).
				if ((result = TootleOptimizeOverdraw(
					inVertexPositions,
					tmpVertexIndices,
					positionsCount,
					faceCount,
					positionStride,
					viewpointPositions,
					viewpointCount,
					windingOrder,
					outFaceClusters,
					outVertexIndices,
					NULL,
					optimizerDraw)) != TOOTLE_OK)
				{
					break;
				}
			}

			// *******************************************************************************************************************
			// An example of using a single utility function to perform v1.2 optimizations.
			// *******************************************************************************************************************

			else
			{
				// This function will compute the entire optimization (cluster mesh, vcache per cluster, and optimize overdraw).
				// It will use TOOTLE_OVERDRAW_FAST as the default overdraw optimization
				if ((result = TootleOptimize(
					inVertexPositions,
					inVertexIndices,
					positionsCount,
					faceCount,
					positionStride,
					cacheSize,
					viewpointPositions,
					viewpointCount,
					windingOrder,
					outVertexIndices,
					&clusterCount,
					optimizerCache)) != TOOTLE_OK)
				{
					break;
				}
			}

			break;

		case TOOTLE_APPROX_CLUSTER_VCACHE_OVERDRAW:
			// *******************************************************************************************************************
			// An example of calling v2.0 optimize vertex cache and clustering mesh with v1.2 overdraw optimization.
			// *******************************************************************************************************************

			if ((optimizerDraw != TOOTLE_OVERDRAW_FAST) ||
			    (outFaceClusters != nullptr))
			{
				// Optimize vertex cache and create cluster
				// The algorithm from SIGGRAPH combine the vertex cache optimization and clustering mesh into a single step
				if ((result = TootleFastOptimizeVCacheAndClusterMesh(
					inVertexIndices,
					faceCount,
					positionsCount,
					cacheSize,
					tmpVertexIndices,
					outFaceClusters,
					&clusterCount,
					TOOTLE_DEFAULT_ALPHA)) != TOOTLE_OK)
				{
					break;
				}

				// In this example, we use TOOTLE_OVERDRAW_AUTO to show that we can mix-match the clustering and
				//  vcache computation from the new library with the overdraw optimization from the old library.
				//  TOOTLE_OVERDRAW_AUTO will choose between using Direct3D or CPU raytracing path.  This path is
				//  much slower than TOOTLE_OVERDRAW_FAST but usually produce 2x better results.
				if ((result = TootleOptimizeOverdraw(
					inVertexPositions,
					tmpVertexIndices,
					positionsCount,
					faceCount,
					positionStride,
					NULL,
					0,
					windingOrder,
					outFaceClusters,
					outVertexIndices,
					NULL,
					optimizerDraw)) != TOOTLE_OK)
				{
					break;
				}
			}


			// *******************************************************************************************************************
			// An example of using a single utility function to perform v2.0 optimizations.
			// *******************************************************************************************************************

			else
			{
				// This function will compute the entire optimization (optimize vertex cache, cluster mesh, and optimize overdraw).
				// It will use TOOTLE_OVERDRAW_FAST as the default overdraw optimization
				if ((result = TootleFastOptimize(
					inVertexPositions,
					inVertexIndices,
					positionsCount,
					faceCount,
					positionStride,
					cacheSize,
					windingOrder,
					outVertexIndices,
					&clusterCount,
					TOOTLE_DEFAULT_ALPHA)) != TOOTLE_OK)
				{
					break;
				}
			}

			break;
	}

	if (result != TOOTLE_OK)
	{
		return false;
	}

	if (true)
	{
		// measure output VCache efficiency
		if (TootleMeasureCacheEfficiency(
			outVertexIndices,
			faceCount,
			cacheSize,
			&fVCacheOut) != TOOTLE_OK)
		{
			return false;
		}
	}

	if (true)
	{
		// measure output overdraw
		if (TootleMeasureOverdraw(
			inVertexPositions,
			outVertexIndices,
			positionsCount,
			faceCount,
			positionStride,
			viewpointPositions,
			viewpointCount,
			windingOrder,
			&fOverdrawOut,
			&fMaxOverdrawOut) != TOOTLE_OK)
		{
			return false;
		}
	}

	return true;
}

bool TootleFaceReorderer::reorderVertices(
	const size_t indexCount,
	const unsigned int* const inVertexIndices,
	const size_t positionsCount, // equates max(index)
	const void* const inVertexPositions,
	unsigned int* const outVertexIndices,
	void* const outVertexPositions,
	unsigned int* const outVertexMap,
	const TootleFaceWinding windingOrder,
	const unsigned int cacheSize)
{
	const unsigned int faceCount = indexCount / 3;
	const unsigned int positionStride = 3 * sizeof(float);

	//-----------------------------------------------------------------------------------------------------
	// PERFORM VERTEX MEMORY OPTIMIZATION (rearrange memory layout for vertices based on the final indices
	//  to exploit vertex cache prefetch).
	//  We want to optimize the vertex memory locations based on the final optimized index buffer that will
	//  be in the output file.
	//  Thus, in this sample code, we recompute a copy of the indices that point to the original vertices
	//  (inVertexIndicesTmp) to be passed into the function TootleOptimizeVertexMemory.  If we use the array inVertexIndices, we
	//  will optimize for the wrong result since the array inVertexIndices is based on the rehashed vertex location created
	//  by the function ObjLoader.
	//-----------------------------------------------------------------------------------------------------

	TootleResult result = TOOTLE_OK;
	if (true)
	{
		// For this sample code, we are just going to use vertexRemapping array result.  This is to support general obj
		//  file input and output.
		//  In fact, we are sending the wrong vertex buffer here (it should be based on the original file instead of the
		//  rehashed vertices).  But, it is ok because we do not request the reordered vertex buffer as an output.
		if (TootleOptimizeVertexMemory(
			inVertexPositions,
			inVertexIndices,
			positionsCount,
			faceCount,
			positionStride,
			outVertexPositions,
			outVertexIndices,
			outVertexMap) != TOOTLE_OK)
		{
			return false;
		}
	}

	if (result != TOOTLE_OK)
	{
		return false;
	}

	return true;
}

#endif
#endif

// eof
