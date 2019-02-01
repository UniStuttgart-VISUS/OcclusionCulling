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
#ifndef DEPTHBUFFERRASTERIZEROGLST_H
#define DEPTHBUFFERRASTERIZEROGLST_H

#include "DepthBufferRasterizerOGL.h"

class DepthBufferRasterizerOGLST : public DepthBufferRasterizerOGL
{
	public:
		DepthBufferRasterizerOGLST();
		~DepthBufferRasterizerOGLST();

		void TransformModelsAndRasterizeToDepthBuffer(CPUTCamera *pCamera, UINT idx);
		void TransformModelsAndRasterizeToDepthBufferOGL(CPUTCamera *pCamera, UINT idx, const std::unique_ptr<OSMesaPipeline> &mesa);
		void ComputeR2DBTime(UINT idx);
		

	private:
		void ActiveModels(UINT idx);
		void TransformMeshes(UINT idx);
		void BinTransformedMeshes(UINT idx);
		void RasterizeBinnedTrianglesToDepthBuffer(UINT tileId, UINT idx);

		std::vector<float4> mFinalXformedPos;
		std::vector<float4> mAllOccluderxformedPos;
		bool InitAllOccluder = true;
		// std::vector<FrustumModel> mFrustumModels; // not used currently
};

#endif  //DEPTHBUFFERRASTERIZEROGLST_H