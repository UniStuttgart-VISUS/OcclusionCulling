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
#include "OcclusionCulling.h"


void ParseCommandLine()
{
	LPTSTR commandLine = GetCommandLineW();
	int numArgs = 0;
	LPWSTR* argv = CommandLineToArgvW(commandLine, &numArgs);

	/*
	Command Line Input:

	-type [SCALAR_TYPE | SSE_TYPE | AVX_TYPE | MASK_AVX_TASK | OGL_TYPE]
	-culling [true | false]
	-frustumCulling [true | false]
	-depthBuffer [true | false]
	-boundingBox [true | false]
	-multithreading [true | false]
	-pipeline [true | false]
	-camera [X Pos | Y Pos | Z Pos | X LookAt | Y LookAt | Z LookAt]           => 38.6, 52.5, 166.6, 73.5, 5.0, 55.0
	-occluderSizeThreshold [0.0 - 5.0]
	-occludeeSizeThreshold [0.0 - 0.1]
	-depthTestTasks [1 - 50]
	-framesForEvaluation [int]
	-DBResolution [0-5]
	-evaluationName [string]
	-evaluationNr [string]
	*/

	for(int i = 1; i < numArgs; i += 2) 
	{
		if(!_wcsicmp(argv[i], L"-type"))
		{
			if(!_wcsicmp(argv[i+1], L"SCALAR_TYPE"))
			{
				gSOCType = SCALAR_TYPE;
			}
			else if (!_wcsicmp(argv[i + 1], L"SSE_TYPE"))
			{
				gSOCType = SSE_TYPE;
			}
			else if (!_wcsicmp(argv[i + 1], L"AVX_TYPE"))
			{
				gSOCType = AVX_TYPE;
			}
			else if (!_wcsicmp(argv[i + 1], L"MASK_AVX_TYPE"))
			{
				gSOCType = MASK_AVX_TYPE;
			}
			else if (!_wcsicmp(argv[i + 1], L"OGL_TYPE"))
			{
				gSOCType = OGL_TYPE;
			}
		}
		else if (!_wcsicmp(argv[i], L"-culling"))
		{
			if (!_wcsicmp(argv[i + 1], L"true"))
			{
				gEnableCullingDX11 = true;
			}
			else
			{
				gEnableCullingDX11 = false;
			}
		}
		else if (!_wcsicmp(argv[i], L"-frustumCulling"))
		{
			if (!_wcsicmp(argv[i + 1], L"true"))
			{
				gEnableFCullingDX11 = true;
			}
			else
			{
				gEnableFCullingDX11 = false;
			}
		}
		else if (!_wcsicmp(argv[i], L"-depthBuffer"))
		{
			if (!_wcsicmp(argv[i + 1], L"true"))
			{
				gViewDepthBufferDX11 = true;
			}
			else
			{
				gViewDepthBufferDX11 = false;
			}
		}
		else if (!_wcsicmp(argv[i], L"-boundingBox"))
		{
			if (!_wcsicmp(argv[i + 1], L"true"))
			{
				gViewBoundingBoxDX11 = true;
			}
			else
			{
				gViewBoundingBoxDX11 = false;
			}
		}
		else if (!_wcsicmp(argv[i], L"-multithreading"))
		{
			if (!_wcsicmp(argv[i + 1], L"true"))
			{
				gEnableTasksDX11 = true;
			}
			else
			{
				gEnableTasksDX11 = false;
			}
		}
		else if (!_wcsicmp(argv[i], L"-pipeline"))
		{
			if (!_wcsicmp(argv[i + 1], L"true"))
			{
				gPipelineDX11 = true;
			}
			else
			{
				gPipelineDX11 = false;
			}
		}
		else if (!_wcsicmp(argv[i], L"-camera"))
		{
			gCameraDX11[0] = (float)_wtof(argv[i + 1]);
			gCameraDX11[1] = (float)_wtof(argv[i + 2]);
			gCameraDX11[2] = (float)_wtof(argv[i + 3]);
			gCameraDX11[3] = (float)_wtof(argv[i + 4]);
			gCameraDX11[4] = (float)_wtof(argv[i + 5]);
			gCameraDX11[5] = (float)_wtof(argv[i + 6]);
			i += 5;
		}
		else if(!_wcsicmp(argv[i], L"-occluderSizeThreshold"))
		{
			gOccluderSizeThresholdDX11 = (float)_wtof(argv[i+1]);
		}
		else if(!_wcsicmp(argv[i], L"-occludeeSizeThreshold"))
		{
			gOccludeeSizeThresholdDX11 = (float)_wtof(argv[i+1]);
		}
		else if(!_wcsicmp(argv[i], L"-depthTestTasks"))
		{
			gDepthTestTasksDX11 = (int)_wtof(argv[i + 1]);
		}
		else if (!_wcsicmp(argv[i], L"-cameraTour"))
		{
			gCameraTourDX11 = (int)_wtof(argv[i + 1]);
		}
		else if (!_wcsicmp(argv[i], L"-framesForEvaluation"))
		{
			gFramesForEvaluationDX11 = (int)_wtof(argv[i + 1]);
		}
		else if (!_wcsicmp(argv[i], L"-DBResolution"))
		{
			gDBResolutionDX11 = (int)_wtof(argv[i + 1]);
		}
		else if (!_wcsicmp(argv[i], L"-evaluationName"))
		{
			gEvaluationNameDX11 = std::wstring(argv[i + 1]);
		}
		else if (!_wcsicmp(argv[i], L"-evaluationNr"))
		{
			gEvaluationNrDX11 = std::wstring(argv[i + 1]);
		}
		else if (!_wcsicmp(argv[i], L"-occludeePackage"))
		{
			gOccludeePackageDX11 = (int)_wtof(argv[i + 1]);
		}
	}
}

// Application entry point.  Execution begins here.
//-----------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	ParseCommandLine();

    // Prevent unused parameter compiler warnings
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(nCmdShow);

#ifdef DEBUG
    // tell VS to report leaks at any exit of the program
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
    CPUTResult result=CPUT_SUCCESS;
    int returnCode=0;

    // create an instance of my sample
    MySample *pSample = new MySample();
    
    // We make the assumption we are running from the executable's dir in
    // the CPUT SampleStart directory or it won't be able to use the relative paths to find the default
    // resources    
	cString ResourceDirectory;
	CPUTOSServices::GetOSServices()->GetWorkingDirectory(&ResourceDirectory);
	ResourceDirectory.append(_L("\\CPUT\\resources\\"));

    // Initialize the system and give it the base CPUT resource directory (location of GUI images/etc)
    // For now, we assume it's a relative directory from the executable directory.  Might make that resource
    // directory location an env variable/hardcoded later
    pSample->CPUTInitialize(ResourceDirectory); 

    // window and device parameters
    CPUTWindowCreationParams params;
    params.deviceParams.refreshRate         = 0; //60;
    params.deviceParams.swapChainBufferCount= 1;
    params.deviceParams.swapChainFormat     = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    params.deviceParams.swapChainUsage      = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	//CPUTOSServices::GetOSServices()->GetDesktopDimensions(&params.windowWidth, &params.windowHeight);

    // parse out the parameter settings or reset them to defaults if not specified
    cString AssetFilename;
    cString CommandLine(lpCmdLine);

    pSample->CPUTParseCommandLine(CommandLine, &params, &AssetFilename);       
	
	params.windowWidth = SCREENW;
	params.windowHeight = SCREENH;

    // create the window and device context
    result = pSample->CPUTCreateWindowAndContext(_L("CPUTWindow DirectX 11"), params);
    ASSERT( CPUTSUCCESS(result), _L("CPUT Error creating window and context.") );

	// initialize the task manager
    gTaskMgr.Init();

    // start the main message loop
    returnCode = pSample->CPUTMessageLoop();

	pSample->DeviceShutdown();

	// shutdown task manage
	gTaskMgr.Shutdown();

    // cleanup resources
    delete pSample;

    return returnCode;
}



