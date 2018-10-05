////////////////////////////////////////////////////////////////////////////////
// Copyright 2017 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not
// use this file except in compliance with the License.  You may obtain a copy
// of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
// License for the specific language governing permissions and limitations
// under the License.
////////////////////////////////////////////////////////////////////////////////

#include "TransformedMeshOGL.h"



TransformedMeshOGL::TransformedMeshOGL()
	: mNumVertices(0),
	  mNumIndices(0),
	  mNumTriangles(0),
	  mpVertices(NULL),
	  mpIndices(NULL)
{
	mpXformedPos[0] = mpXformedPos[1] = NULL;
}

TransformedMeshOGL::~TransformedMeshOGL()
{

}

void TransformedMeshOGL::CreateOccluderSet(Vertex *vertices, UINT *indices, int numIndices) {
	for (int i = 0; i < numIndices; ++i) {
		OccluderSetMesh.push_back(vertices[indices[i]]);
	}
}

void TransformedMeshOGL::Initialize(CPUTMeshDX11* pMesh)
{
	mNumVertices = pMesh->GetDepthVertexCount();
	mNumIndices  = pMesh->GetIndexCount();
	mNumTriangles = pMesh->GetTriangleCount();
	mpVertices   = pMesh->GetDepthVertices();
	mpIndices    = pMesh->GetDepthIndices();

	numIndexPerObjectMesh = mNumIndices;
	CreateOccluderSet(mpVertices, mpIndices, mNumIndices);
}

//-------------------------------------------------------------------
// Trasforms the occluder vertices to screen space once every frame
//-------------------------------------------------------------------
void TransformedMeshOGL::TransformVertices(const float4x4 &cumulativeMatrix, 
										      UINT start, 
										      UINT end,
											  UINT idx)
{
	for(UINT i = start; i <= end; i++)
	{
		float4 xform = TransformCoords(mpVertices[i].pos, cumulativeMatrix);
		float4 projected = xform / xform.w; 

		//set to all 0s if clipped by near clip plane
		mpXformedPos[idx][i] = xform.z <= xform.w ? projected : float4(0.0, 0.0, 0.0, 0.0);
	}
}

void TransformedMeshOGL::Gather(float4 pOut[3], UINT triId, UINT idx)
{
	for(UINT i = 0; i < 3; i++)
	{
		UINT index = mpIndices[(triId * 3) + i];
		pOut[i] = mpXformedPos[idx][index];	
	}
}

//--------------------------------------------------------------------------------
// Bin the screen space transformed triangles into tiles. For single threaded version
//--------------------------------------------------------------------------------
void TransformedMeshOGL::BinTransformedTrianglesST(UINT taskId,
													  UINT modelId,
													  UINT meshId,
													  UINT start,
													  UINT end,
													  UINT* pBin,
												      USHORT* pBinModel,
													  USHORT* pBinMesh,
													  USHORT* pNumTrisInBin,
													  UINT idx)
{
	// working on one triangle at a time
	for(UINT index = start; index <= end; index++)
	{
		float4 xformedPos[3];
		Gather(xformedPos, index, idx);

		int fxPtX[3], fxPtY[3];
		for(int i = 0; i < 3; i++)
		{
			fxPtX[i] = (int)(xformedPos[i].x + 0.5);
			fxPtY[i] = (int)(xformedPos[i].y + 0.5);
		}
		
		// Compute triangle area
		int triArea = (fxPtX[1] - fxPtX[0]) * (fxPtY[2] - fxPtY[0]) - (fxPtX[0] - fxPtX[2]) * (fxPtY[0] - fxPtY[1]);
				
		// Find bounding box for screen space triangle in terms of pixels
		int startX = max(min(min(fxPtX[0], fxPtX[1]), fxPtX[2]), 0); 
        int endX   = min(max(max(fxPtX[0], fxPtX[1]), fxPtX[2]), SCREENW - 1);

        int startY = max(min(min(fxPtY[0], fxPtY[1]), fxPtY[2]), 0 );
        int endY   = min(max(max(fxPtY[0], fxPtY[1]), fxPtY[2]), SCREENH - 1);

		// Skip triangle if area is zero 
		if(triArea <= 0) continue;
		// Dont bin screen-clipped triangles
		if(endX < startX || endY < startY) continue;
			
		// Reject the triangle if any of its verts is behind the nearclip plane
		if(xformedPos[0].w > 0.0f && xformedPos[1].w > 0.0f && xformedPos[2].w > 0.0f) 
		{
			// Convert bounding box in terms of pixels to bounding box in terms of tiles
			int startXx = max(startX/TILE_WIDTH_IN_PIXELS, 0);
			int endXx   = min(endX/TILE_WIDTH_IN_PIXELS, SCREENW_IN_TILES-1);
		
			int startYy = max(startY/TILE_HEIGHT_IN_PIXELS, 0);
			int endYy   = min(endY/TILE_HEIGHT_IN_PIXELS, SCREENH_IN_TILES-1);

			// Add triangle to the tiles or bins that the bounding box covers
			int row, col;
			for(row = startYy; row <= endYy; row++)
			{
				int offset1 = YOFFSET1_ST * row;
				int offset2 = YOFFSET2_ST * row;
				for(col = startXx; col <= endXx; col++)
				{
					int idx1 = offset1 + (XOFFSET1_ST * col) + taskId;
					int idx2 = offset2 + (XOFFSET2_ST * col) + (taskId * MAX_TRIS_IN_BIN_ST) + pNumTrisInBin[idx1];
					pBin[idx2] = index;
					pBinModel[idx2] = modelId;
					pBinMesh[idx2] = meshId;
					pNumTrisInBin[idx1] += 1;
				}
			}
		}
	}
}

//--------------------------------------------------------------------------------
// Bin the screen space transformed triangles into tiles. For multi threaded version
//--------------------------------------------------------------------------------
void TransformedMeshOGL::BinTransformedTrianglesMT(UINT taskId,
													  UINT modelId,
													  UINT meshId,
													  UINT start,
													  UINT end,
													  UINT* pBin,
												      USHORT* pBinModel,
													  USHORT* pBinMesh,
													  USHORT* pNumTrisInBin,
													  UINT idx)
{
	// working on 4 triangles at a time
	for(UINT index = start; index <= end; index++)
	{
		float4 xformedPos[3];
		Gather(xformedPos, index, idx);
		
		int fxPtX[3], fxPtY[3];
		for(int i = 0; i < 3; i++)
		{
			fxPtX[i] = (int)(xformedPos[i].x + 0.5);
			fxPtY[i] = (int)(xformedPos[i].y + 0.5);
		}

		// Compute triangle area
		int triArea = (fxPtX[1] - fxPtX[0]) * (fxPtY[2] - fxPtY[0]) - (fxPtX[0] - fxPtX[2]) * (fxPtY[0] - fxPtY[1]);
		
		// Skip triangle if area is zero 
		if(triArea <= 0) continue;
		
		// Find bounding box for screen space triangle in terms of pixels
		int startX = max(min(min(fxPtX[0], fxPtX[1]), fxPtX[2]), 0);
        int endX   = min(max(max(fxPtX[0], fxPtX[1]), fxPtX[2]), SCREENW-1 );

        int startY = max(min(min(fxPtY[0], fxPtY[1]), fxPtY[2]), 0);
        int endY   = min(max(max(fxPtY[0], fxPtY[1]), fxPtY[2]), SCREENH-1);
		// Dont bin screen-clipped triangles
		if(endX < startX || endY < startY) continue;
				
		// Reject the triangle if any of its verts is behind the nearclip plane
		if(xformedPos[0].w > 0.0f && xformedPos[1].w > 0.0f && xformedPos[2].w > 0.0f) 
		{
			// Convert bounding box in terms of pixels to bounding box in terms of tiles
			int startXx = max(startX/TILE_WIDTH_IN_PIXELS, 0);
			int endXx   = min(endX/TILE_WIDTH_IN_PIXELS, SCREENW_IN_TILES-1);

			int startYy = max(startY/TILE_HEIGHT_IN_PIXELS, 0);
			int endYy   = min(endY/TILE_HEIGHT_IN_PIXELS, SCREENH_IN_TILES-1);

			// Add triangle to the tiles or bins that the bounding box covers
			int row, col;
			for(row = startYy; row <= endYy; row++)
			{
				int offset1 = YOFFSET1_MT * row;
				int offset2 = YOFFSET2_MT * row;
				for(col = startXx; col <= endXx; col++)
				{
					int idx1 = offset1 + (XOFFSET1_MT * col) + (TOFFSET1_MT * taskId);
					int idx2 = offset2 + (XOFFSET2_MT * col) + (taskId * MAX_TRIS_IN_BIN_MT) + pNumTrisInBin[idx1];
					pBin[idx2] = index;
					pBinModel[idx2] = modelId;
					pBinMesh[idx2] = meshId;
					pNumTrisInBin[idx1] += 1;
				}
			}
		}
	}
}

void TransformedMeshOGL::GetOneTriangleData(float* xformedPos, UINT triId, UINT idx)
{
	float4* pOut = (float4*) xformedPos;
	for(int i = 0; i < 3; i++)
	{
		UINT index = mpIndices[(triId * 3) + i];
		*(pOut + i) = mpXformedPos[idx][index];
	}
}