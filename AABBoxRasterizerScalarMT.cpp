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

#include "AABBoxRasterizerScalarMT.h"

AABBoxRasterizerScalarMT::AABBoxRasterizerScalarMT()
	: AABBoxRasterizerScalar()
{
}

AABBoxRasterizerScalarMT::~AABBoxRasterizerScalarMT()
{

}

//-------------------------------------------------------------------------------
// Create mNumDepthTestTasks to tarnsform occludee AABBox, rasterize and depth test 
// to determine if occludee is visible or occluded
//-------------------------------------------------------------------------------
void AABBoxRasterizerScalarMT::TransformAABBoxAndDepthTest(CPUTCamera *pCamera, UINT idx)
{
	mTaskData[idx].idx = idx;
	mTaskData[idx].pAABB = this;

	mpCamera[idx] = pCamera;
	
	gTaskMgr.CreateTaskSet(&AABBoxRasterizerScalarMT::TransformAABBoxAndDepthTest, &mTaskData[idx], mNumDepthTestTasks, &gRasterize[idx], 1, "Xform Vertices", &gAABBoxDepthTest[idx]);
}

void AABBoxRasterizerScalarMT::WaitForTaskToFinish(UINT idx)
{
	// Wait for the task set
	gTaskMgr.WaitForSet(gAABBoxDepthTest[idx]);
}

void AABBoxRasterizerScalarMT::ReleaseTaskHandles(UINT idx)
{
	// Release the task set
	if(mEnableFCulling)
	{
		gTaskMgr.ReleaseHandle(gInsideViewFrustum[idx]);
	}
	else
	{
		gTaskMgr.ReleaseHandle(gTooSmall[idx]);
	}
	gTaskMgr.ReleaseHandle(gActiveModels[idx]);
	gTaskMgr.ReleaseHandle(gXformMesh[idx]);
	gTaskMgr.ReleaseHandle(gBinMesh[idx]);
	gTaskMgr.ReleaseHandle(gSortBins[idx]);
	gTaskMgr.ReleaseHandle(gRasterize[idx]);
	gTaskMgr.ReleaseHandle(gAABBoxDepthTest[idx]);

	gInsideViewFrustum[idx] = gTooSmall[idx] = gActiveModels[idx] = gXformMesh[idx] = gBinMesh[idx] = gSortBins[idx] = gRasterize[idx] = TASKSETHANDLE_INVALID;
	gAABBoxDepthTest[idx] = TASKSETHANDLE_INVALID;

	LARGE_INTEGER startTime = mStartTime[idx][0];
	LARGE_INTEGER stopTime = mStopTime[idx][0];
	for(UINT i = 0; i < mNumDepthTestTasks; i++)
	{
		startTime = startTime.QuadPart > mStartTime[idx][i].QuadPart ? mStartTime[idx][i] : startTime;
		stopTime = stopTime.QuadPart < mStopTime[idx][i].QuadPart? mStopTime[idx][i] : stopTime;
	}
	mDepthTestTime[mTimeCounter++] = ((double)(stopTime.QuadPart - startTime.QuadPart)) / ((double)glFrequency.QuadPart);
	mTimeCounter = mTimeCounter >= AVG_COUNTER ? 0 : mTimeCounter;
}

//--------------------------------------------------------------------------------
// Determine the batch of occludee models each task should work on
// For each occludee model in the batch
// * Transform the AABBox to screen space
// * Rasterize the triangles that make up the AABBox
// * Depth test the raterized triangles against the CPU rasterized depth buffer
//--------------------------------------------------------------------------------
void AABBoxRasterizerScalarMT::TransformAABBoxAndDepthTest(UINT taskId, UINT idx)
{
	QueryPerformanceCounter(&mStartTime[idx][taskId]);
	
	BoxTestSetupScalar setup;
	setup.Init(mViewMatrix[idx], mProjMatrix[idx], viewportMatrix, mpCamera[idx], mOccludeeSizeThreshold);

	float4 xformedPos[AABB_VERTICES];
	float4x4 cumulativeMatrix;

	static const UINT kChunkSize = 64;
	for(UINT base = taskId*kChunkSize; base < mNumModels; base += mNumDepthTestTasks * kChunkSize)
	{
		UINT end = min(base + kChunkSize, mNumModels);
		for(UINT i = base; i < end; i++)
		{
			if(mEnableFCulling)
			{
				// Determine if the occludee model AABBox is within the viewing frustum 
				mpInsideFrustum[idx][i] = mpTransformedAABBox[i].IsInsideViewFrustum(mpCamera[idx]);
			}
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
	}
	QueryPerformanceCounter(&mStopTime[idx][taskId]);
}

void AABBoxRasterizerScalarMT::TransformAABBoxAndDepthTest(VOID* pTaskData, INT context, UINT taskId, UINT taskCount)
{
	PerTaskData *pPerTaskData = (PerTaskData*)pTaskData;
	pPerTaskData->pAABB->TransformAABBoxAndDepthTest(taskId, pPerTaskData->idx);
}



