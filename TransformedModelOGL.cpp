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
#include "TransformedModelOGL.h"

TransformedModelOGL::TransformedModelOGL()
	: mpCPUTModel(NULL),
	  mNumMeshes(0),
	  mNumVertices(0),
	  mNumTriangles(0),
	  mpMeshes(NULL)
{
	mInsideViewFrustum[0] = mInsideViewFrustum[1] = false;
	mTooSmall[0] = mTooSmall[1] = false;
	mpXformedPos[0] = mpXformedPos[1] = NULL;

	Osmesa = std::make_unique<OSMesaPipeline>();
}

TransformedModelOGL::~TransformedModelOGL()
{
	SAFE_DELETE_ARRAY(mpMeshes);
}

//--------------------------------------------------------------------
// Create place holder for the transformed meshes for each model
//---------------------------------------------------------------------
void TransformedModelOGL::CreateTransformedMeshes(CPUTModelDX11 *pModel)
{
	mpCPUTModel = pModel;
	mNumMeshes = pModel->GetMeshCount();
	mWorldMatrix = *pModel->GetWorldMatrix();
	
	float3 center, half;
	pModel->GetBoundsObjectSpace(&center, &half);

	mBBCenterOS = center;
	mRadiusSq = half.lengthSq();

	mpMeshes = new TransformedMeshOGL[mNumMeshes];

	for(UINT i = 0; i < mNumMeshes; i++)
	{
		CPUTMeshDX11* pMesh = (CPUTMeshDX11*)pModel->GetMesh(i);
		ASSERT((pMesh != NULL), _L("pMesh is NULL"));

		mpMeshes[i].Initialize(pMesh);
		mNumVertices += mpMeshes[i].GetNumVertices();
		mNumTriangles += mpMeshes[i].GetNumTriangles();
	}
}

void TransformedModelOGL::TooSmall(const BoxTestSetupScalar &setup, UINT idx)
{
	if(mInsideViewFrustum[idx])
	{
		mCumulativeMatrix[idx] = mWorldMatrix * setup.mViewProjViewport;
		
		float w = mBBCenterOS.x * mCumulativeMatrix[idx].r0.w +
				  mBBCenterOS.y * mCumulativeMatrix[idx].r1.w +
				  mBBCenterOS.z * mCumulativeMatrix[idx].r2.w +
				  mCumulativeMatrix[idx].r3.w; 
	
		if(w > 1.0f)
		{
			mTooSmall[idx] = mRadiusSq < w * setup.radiusThreshold; 
		}
		else
		{
			// BB center is behind the near clip plane, making screen-space radius meaningless.
            // Assume visible.  This should be a safe assumption, as the frustum test says the bbox is visible.
            mTooSmall[idx] = false;
		}
	}
}


//------------------------------------------------------------------
// Determine if the occluder model is inside view frustum
// Gather all occluder parameter
//------------------------------------------------------------------
void TransformedModelOGL::InsideViewFrustum(const BoxTestSetupScalar &setup,
											   UINT idx)
{
	mpCPUTModel->GetBoundsWorldSpace(&mBBCenterWS, &mBBHalfWS);
	FrustumModels.push_back(FrustumModel(mWorldMatrix, setup.mViewProjViewport, mBBCenterWS, mBBHalfWS));

	//mInsideViewFrustum[idx] = setup.mpCamera->mFrustum.IsVisible(mBBCenterWS, mBBHalfWS);

	// code below gets cut
	//if(mInsideViewFrustum[idx])
	//{
	//	mCumulativeMatrix[idx] = mWorldMatrix * setup.mViewProjViewport;
	//	
	//	float w = mBBCenterOS.x * mCumulativeMatrix[idx].r0.w +
	//			  mBBCenterOS.y * mCumulativeMatrix[idx].r1.w +
	//			  mBBCenterOS.z * mCumulativeMatrix[idx].r2.w +
	//			  mCumulativeMatrix[idx].r3.w; 
	//
	//	if(w > 1.0f)
	//	{
	//		mTooSmall[idx] = mRadiusSq < w * setup.radiusThreshold; 
	//	}
	//	else
	//	{
	//		// BB center is behind the near clip plane, making screen-space radius meaningless.
 //           // Assume visible.  This should be a safe assumption, as the frustum test says the bbox is visible.
 //           mTooSmall[idx] = false;
	//	}
	//}
}

//---------------------------------------------------------------------------------------------------
// Determine if the occluder size is sufficiently large enough to occlude other object sin the scene
// If so transform the occluder to screen space so that it can be rasterized to the cPU depth buffer
//---------------------------------------------------------------------------------------------------
void TransformedModelOGL::TransformMeshes(UINT start, 
										     UINT end,
											 CPUTCamera* pCamera,
											 UINT idx)
{
	mAllVertices1.clear();

	if(mInsideViewFrustum[idx] && !mTooSmall[idx])
	{
		UINT totalNumVertices = 0;
		for(UINT meshId = 0; meshId < mNumMeshes; meshId++)
		{		
			totalNumVertices +=  mpMeshes[meshId].GetNumVertices();
			if(totalNumVertices < start)
			{
				continue;
			}
			mpMeshes[meshId].TransformVertices(mCumulativeMatrix[idx], start, end, idx);
			std::vector<float4> b = mpMeshes[meshId].GetAllXformedPos();
			mAllVertices1.insert(mAllVertices1.end(), b.begin(), b.end());
		}
	}
}

//------------------------------------------------------------------------------------
// If the occluder is sufficiently large enough to occlude other objects in the scene 
// bin the triangles that make up the occluder into tiles to speed up rateraization
// Single threaded version
//------------------------------------------------------------------------------------
void TransformedModelOGL::BinTransformedTrianglesST(UINT taskId,
													   UINT modelId,
													   UINT start,
													   UINT end,
													   UINT* pBin,
													   USHORT* pBinModel,
													   USHORT* pBinMesh,
													   USHORT* pNumTrisInBin,
													   UINT idx)
{
	if(mInsideViewFrustum[idx] && !mTooSmall[idx])
	{
		UINT totalNumTris = 0;
		for(UINT meshId = 0; meshId < mNumMeshes; meshId++)
		{
			totalNumTris += mpMeshes[meshId].GetNumTriangles();
			if(totalNumTris < start)
			{
				continue;
			}
			mpMeshes[meshId].BinTransformedTrianglesST(taskId, modelId, meshId, start, end, pBin, pBinModel, pBinMesh, pNumTrisInBin, idx);
		}
	}
}

//------------------------------------------------------------------------------------
// If the occluder is sufficiently large enough to occlude other objects in the scene 
// bin the triangles that make up the occluder into tiles to speed up rateraization
// Multi threaded version
//------------------------------------------------------------------------------------
void TransformedModelOGL::BinTransformedTrianglesMT(UINT taskId,
													   UINT modelId,
													   UINT start,
													   UINT end,
													   UINT* pBin,
													   USHORT* pBinModel,
													   USHORT* pBinMesh,
													   USHORT* pNumTrisInBin,
													   UINT idx)
{
	if(mInsideViewFrustum[idx] && !mTooSmall[idx])
	{
		UINT totalNumTris = 0;
		for(UINT meshId = 0; meshId < mNumMeshes; meshId++)
		{
			totalNumTris += mpMeshes[meshId].GetNumTriangles();
			if(totalNumTris < start)
			{
				continue;
			}
			mpMeshes[meshId].BinTransformedTrianglesMT(taskId, modelId, meshId, start, end, pBin, pBinModel, pBinMesh, pNumTrisInBin, idx);
		}
	}
}

void TransformedModelOGL::Gather(float* xformedPos,
									UINT meshId, 
									UINT triId, 
									UINT idx)
{
	mpMeshes[meshId].GetOneTriangleData(xformedPos, triId, idx);
}