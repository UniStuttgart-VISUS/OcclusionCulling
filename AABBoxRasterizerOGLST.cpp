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

#include "AABBoxRasterizerOGLST.h"

AABBoxRasterizerOGLST::AABBoxRasterizerOGLST()
	: AABBoxRasterizerOGL()
{
	mpModelIds[0] = new UINT[27025];
	mpModelIds[1] = new UINT[27025];
	mModelCounts[0] = mModelCounts[1] = 0;
}

AABBoxRasterizerOGLST::~AABBoxRasterizerOGLST()
{
	SAFE_DELETE_ARRAY(mpModelIds[0]);
	SAFE_DELETE_ARRAY(mpModelIds[1]);
}

//------------------------------------------------------------------------------
// * Altered function for project use
//
// For each occludee model
// * Determine if the occludee model AABox is within the viewing frustum 
// * Transform the AABBox to screen space
// * Rasterize the triangles that make up the AABBox
// * Depth test the raterized triangles against the CPU rasterized depth buffer
//-----------------------------------------------------------------------------
void AABBoxRasterizerOGLST::TransformAABBoxAndDepthTestOGL(CPUTCamera *pCamera, UINT idx, const std::unique_ptr<OSMesaPipeline> &mesa)
{
	QueryPerformanceCounter(&mStartTime[idx][0]);

	mpCamera[idx] = pCamera;
	if (mEnableFCulling)
	{
		for (UINT i = 0; i < mNumModels; i++)
		{
			// 
			mpInsideFrustum[idx][i] = mpTransformedAABBox[i].IsInsideViewFrustum(mpCamera[idx]);
		}
	}

	BoxTestSetupScalar setup;
	setup.Init(mViewMatrix[idx], mProjMatrix[idx], viewportMatrix, mpCamera[idx], mOccludeeSizeThreshold);

	float4 xformedPos[AABB_VERTICES];
	float4x4 cumulativeMatrix;

	if (mesa->InitAllOccludeeBB)
	{
		for (UINT i = 0; i < mNumModels; i++)
		{
			// new cumulativeMatrix only world
			// needed for proper opengl transformation
			//cumulativeMatrix = mpTransformedAABBox[i].GetWorldMatrix();

			// take cumulative matrix out of parameter list of TransformAABBox function
			cumulativeMatrix = float4x4Identity();

			// or gather all vertices from xformedPos here, send them to the mesa context and go through all at once
			// !!! order of vertices is essential here !!!
			// if order gets scrambled, a wrong result will be given back to the intel context
			// get all AABBs and start ALL queries afterwards at once
			mpTransformedAABBox[i].TransformAABBox(xformedPos, cumulativeMatrix);
			mesa->GatherAllAABBs(xformedPos, mpTransformedAABBox[i].GetWorldMatrix());
		}

		mesa->UploadOccludeeAABBs();
		mesa->InitAllOccludeeBB = false;
	}

	std::vector<UINT> ModelIds;
	
	int ModelCount = 0;
	int current = 0;

	for (UINT i = 0; i < mNumModels; i++)
	{
		mpVisible[idx][i] = false;
		

		if (mpInsideFrustum[idx][i] && !mpTransformedAABBox[i].IsTooSmall(setup, cumulativeMatrix))
		{
			//ModelIds.push_back(i);
			mpModelIds[mSwap][current++] = i;
			++ModelCount;
		}
	}
	
	mModelCounts[mSwap] = ModelCount;
	mesa->SartOcclusionQueries(mpModelIds[mSwap], mModelCounts[mSwap], mViewMatrix[idx], mProjMatrix[idx], mSwap);

	// if all Queries finished, get the results
	for (int i = 0; i < mModelCounts[!mSwap]; ++i)
	{
		// invert results to see all occluded objects
		//mpVisible[idx][ModelIds[i]] = mesa->AABBVisibility[i];
		mpVisible[idx][mpModelIds[!mSwap][i]] = mesa->AABBVisible[!mSwap][i];
	}

	mSwap = !mSwap;

	QueryPerformanceCounter(&mStopTime[idx][0]);
	mDepthTestTime[mTimeCounter++] = ((double)(mStopTime[idx][0].QuadPart - mStartTime[idx][0].QuadPart)) / ((double)glFrequency.QuadPart);
	mTimeCounter = mTimeCounter >= AVG_COUNTER ? 0 : mTimeCounter;
}

//------------------------------------------------------------------------------
// For each occludee model
// * Determine if the occludee model AABox is within the viewing frustum 
// * Transform the AABBox to screen space
// * Rasterize the triangles that make up the AABBox
// * Depth test the raterized triangles against the CPU rasterized depth buffer
//-----------------------------------------------------------------------------
void AABBoxRasterizerOGLST::TransformAABBoxAndDepthTest(CPUTCamera *pCamera, UINT idx)
{
	QueryPerformanceCounter(&mStartTime[idx][0]);

	mpCamera[idx] = pCamera;
	if(mEnableFCulling)
	{
		for(UINT i = 0; i < mNumModels; i++)
		{
			// 
			mpInsideFrustum[idx][i] = mpTransformedAABBox[i].IsInsideViewFrustum(mpCamera[idx]);
		}
	}

	BoxTestSetupScalar setup;
	setup.Init(mViewMatrix[idx], mProjMatrix[idx], viewportMatrix, mpCamera[idx], mOccludeeSizeThreshold);

	float4 xformedPos[AABB_VERTICES];
	float4x4 cumulativeMatrix;

	for(UINT i = 0; i < mNumModels; i++)
	{
		mpVisible[idx][i] = false;
		
		if(mpInsideFrustum[idx][i] && !mpTransformedAABBox[i].IsTooSmall(setup, cumulativeMatrix))
		{
			if(mpTransformedAABBox[i].TransformAABBox(xformedPos, cumulativeMatrix))
			{
				mpVisible[idx][i] = mpTransformedAABBox[i].RasterizeAndDepthTestAABBox(mpRenderTargetPixels[idx], xformedPos, idx);
			}
			else
			{
				mpVisible[idx][i] = true;
			}
		}		
	}

	QueryPerformanceCounter(&mStopTime[idx][0]);
	mDepthTestTime[mTimeCounter++] = ((double)(mStopTime[idx][0].QuadPart - mStartTime[idx][0].QuadPart)) / ((double)glFrequency.QuadPart);
	mTimeCounter = mTimeCounter >= AVG_COUNTER ? 0 : mTimeCounter;
}

void AABBoxRasterizerOGLST::WaitForTaskToFinish(UINT idx)
{
}

void AABBoxRasterizerOGLST::ReleaseTaskHandles(UINT idx)
{
}