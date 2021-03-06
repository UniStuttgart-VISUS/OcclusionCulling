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

#include "AABBoxRasterizerScalar.h"

AABBoxRasterizerScalar::AABBoxRasterizerScalar()
	: mNumModels(0),
	  mpTransformedAABBox(NULL),
	  mpModels(NULL),
	  mpNumTriangles(NULL),
	  mOccludeeSizeThreshold(0.0f),
	  mNumDepthTestTasks(0),
	  mTimeCounter(0),
	  mEnableFCulling(true)
{
	mpCamera[0] = mpCamera[1] = NULL;
	mpVisible[0] = mpVisible[1] = NULL;
	mpInsideFrustum[0] = mpInsideFrustum[1] = NULL;

	mpRenderTargetPixels[0] = NULL;
	mpRenderTargetPixels[1] = NULL;
	
	mNumCulled[0] = mNumCulled[1] =NULL;
	for(UINT i = 0; i < AVG_COUNTER; i++)
	{
		mDepthTestTime[i] = 0.0;
	}
}

AABBoxRasterizerScalar::~AABBoxRasterizerScalar()
{
	for(UINT i = 0; i < mNumModels; i++)
	{
		mpModels[i]->Release();
	}
	SAFE_DELETE_ARRAY(mpVisible[0]);
	SAFE_DELETE_ARRAY(mpVisible[1]);
	SAFE_DELETE_ARRAY(mpTransformedAABBox);
	SAFE_DELETE_ARRAY(mpInsideFrustum[0]);
	SAFE_DELETE_ARRAY(mpInsideFrustum[1]);
	SAFE_DELETE_ARRAY(mpNumTriangles);
	SAFE_DELETE_ARRAY(mpModels);
}

//--------------------------------------------------------------------
// * Go through the asset set and determine the model count in it
// * Create data structures for all the models in the asset set
// * For each model create the axis aligned bounding box triangle 
//   vertex and index list
//--------------------------------------------------------------------
void AABBoxRasterizerScalar::CreateTransformedAABBoxes(CPUTAssetSet **pAssetSet, UINT numAssetSets)
{
	for(UINT assetId = 0; assetId < numAssetSets; assetId++)
	{
		for(UINT nodeId = 0; nodeId < pAssetSet[assetId]->GetAssetCount(); nodeId++)
		{
			CPUTRenderNode* pRenderNode = NULL;
			CPUTResult result = pAssetSet[assetId]->GetAssetByIndex(nodeId, &pRenderNode);
			ASSERT((CPUT_SUCCESS == result), _L ("Failed getting asset by index")); 
			if(pRenderNode->IsModel())
			{
				mNumModels++;		
			}
			pRenderNode->Release();
		}
	}

	mpVisible[0] = new bool[mNumModels];
	mpVisible[1] = new bool[mNumModels];
	mpTransformedAABBox = new TransformedAABBoxScalar[mNumModels];
	mpModels = new CPUTModelDX11 *[mNumModels];
	mpInsideFrustum[0] = new bool[mNumModels];
	mpInsideFrustum[1] = new bool[mNumModels];

	mpNumTriangles = new UINT[mNumModels];
	
	for(UINT assetId = 0, modelId = 0; assetId < numAssetSets; assetId++)
	{
		for(UINT nodeId = 0; nodeId < pAssetSet[assetId]->GetAssetCount(); nodeId++)
		{
			CPUTRenderNode* pRenderNode = NULL;
			CPUTResult result = pAssetSet[assetId]->GetAssetByIndex(nodeId, &pRenderNode);
			ASSERT((CPUT_SUCCESS == result), _L ("Failed getting asset by index")); 
			if(pRenderNode->IsModel())
			{
				CPUTModelDX11 *pModel = (CPUTModelDX11*)pRenderNode;
				pModel = (CPUTModelDX11*)pRenderNode;
		
				mpModels[modelId] = pModel;
				pModel->AddRef();

				mpTransformedAABBox[modelId].CreateAABBVertexIndexList(pModel);
				mpNumTriangles[modelId] = 0;
				for(int meshId = 0; meshId < pModel->GetMeshCount(); meshId++)
				{
					mpNumTriangles[modelId] += pModel->GetMesh(meshId)->GetTriangleCount();
				}
				modelId++;
			}
			pRenderNode->Release();
		}
	}
}

void AABBoxRasterizerScalar::SetViewProjMatrix(float4x4 *viewMatrix, float4x4 *projMatrix, UINT idx)
{
	mViewMatrix[idx] = *viewMatrix;
	mProjMatrix[idx] = *projMatrix;
}

//------------------------------------------------------------------------
// Go through the list of models in the asset set and render only those 
// models that are marked as visible by the software occlusion culling test
//-------------------------------------------------------------------------
void AABBoxRasterizerScalar::RenderVisible(CPUTAssetSet **pAssetSet,
										   CPUTRenderParametersDX &renderParams,
										   UINT numAssetSets,
										   UINT idx)
{
	int count = 0;
	for(UINT modelId = 0; modelId < mNumModels; modelId++)
	{
		if(mpVisible[idx][modelId])
		{
			mpModels[modelId]->Render(renderParams);
			count++;
		}
	}

	mNumCulled[idx] =  mNumModels - count;
}


//------------------------------------------------------------------------
// Go through the list of models in the asset set and render only those 
// models that are marked as visible by the software occlusion culling test
//-------------------------------------------------------------------------
void AABBoxRasterizerScalar::Render(CPUTAssetSet **pAssetSet,
									CPUTRenderParametersDX &renderParams,
									UINT numAssetSets, 
									UINT idx)
{
	int count = 0;
	int triCount = 0;

	CPUTModelDX11::ResetFCullCount();

	BoxTestSetupScalar setup;
	setup.Init(mViewMatrix[idx], mProjMatrix[idx], viewportMatrix, mpCamera[idx], mOccludeeSizeThreshold);

	float4x4 cumulativeMatrix;

	for(UINT assetId = 0, modelId = 0; assetId < numAssetSets; assetId++)
	{
		for(UINT nodeId = 0; nodeId< pAssetSet[assetId]->GetAssetCount(); nodeId++)
		{
			CPUTRenderNode* pRenderNode = NULL;
			CPUTResult result = pAssetSet[assetId]->GetAssetByIndex(nodeId, &pRenderNode);
			ASSERT((CPUT_SUCCESS == result), _L ("Failed getting asset by index")); 
			if(pRenderNode->IsModel())
			{
				if(!mpTransformedAABBox[modelId].IsTooSmall(setup, cumulativeMatrix))
				{
					CPUTModelDX11* model = (CPUTModelDX11*)pRenderNode;
					ASSERT((model != NULL), _L("model is NULL"));

					model = (CPUTModelDX11*)pRenderNode;
					model->Render(renderParams);
					count++;
					triCount += mpNumTriangles[modelId];
				}
				modelId++;			
			}
			pRenderNode->Release();
		}
	}
	mNumFCullCount = CPUTModelDX11::GetFCullCount();
	mNumCulled[idx] =  mNumModels - count;
	mNumTrisRendered = triCount;
}