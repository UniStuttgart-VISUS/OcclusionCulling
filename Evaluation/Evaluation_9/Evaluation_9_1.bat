SET currentEvaluationPath=%~dp0
SET evaluationPath=%currentEvaluationPath:~0,-13%
SET cullingPath=%currentEvaluationPath:~0,-25%
SET evaluationNr=Evaluation_9
SET OC_exe=OcclusionCullingR.exe

SET evaluationName=OGL_0
SET type=OGL_TYPE                       &:: default: SSE_TYPE   Values: [SCALAR_TYPE | SSE_TYPE | AVX_TYPE | MASK_AVX_TYPE | OGL_TYPE]
SET culling=true                        &:: default: true       Values: [true | false]
SET frustumCulling=true                 &:: default: true       Values: [true | false]
SET depthBuffer=false                   &:: default: false      Values: [true | false]
SET boundingBox=false                   &:: default: false      Values: [true | false]
SET multithreading=true                 &:: default: true       Values: [true | false]
SET pipeline=false                      &:: default: false      Values: [true | false]
SET cameraPosX=38.6                     &:: default: [38.6, 52.5, 166.6, 73.5, 5.0, 55.0]
SET cameraPosY=52.5
SET cameraPosZ=166.6
SET cameraLookX=73.5
SET cameraLookY=5.0
SET cameralookZ=55.0
SET occluderSizeThreshold=1.5           &:: default: 1.5        Values: [0.0 - 5.0]
SET occludeeSizeThreshold=0.01          &:: default: 0.01       Values: [0.0 - 0.1]
SET depthTestTasks=20                   &:: default: 20         Values: [1 - 50]
SET cameraTour=1                        &:: default: 0          Values: [1]
SET framesForEvaluation=100             &:: default: 0          Values: [int]
SET DBResolution=3                      &:: default: 3          Values: [0-5]
SET occludeePackage=0                   &:: default: 0          Values: [int]

START /WAIT /D %cullingPath% %OC_exe% -type %type% -depthBuffer %depthBuffer% -culling %culling% -frustumCulling %frustumCulling% -boundingBox %boundingBox% -multithreading %multithreading% -pipeline %pipeline% -camera %cameraPosX% %cameraPosY% %cameraPosZ% %cameraLookX% %cameraLookY% %cameraLookZ% -occluderSizeThreshold %occluderSizeThreshold% -occludeeSizeThreshold %occludeeSizeThreshold% -depthTestTasks %depthTestTasks% -cameraTour %cameraTour% -DBResolution %DBResolution% -occludeePackage %occludeePackage% -evaluationName %evaluationName% -evaluationNr %evaluationNr% -framesForEvaluation %framesForEvaluation%
python %evaluationPath%/create_diagram_for_value.py %evaluationName% %currentEvaluationPath% %framesForEvaluation% %*



SET evaluationName=OGL_1
SET type=OGL_TYPE                       &:: default: SSE_TYPE   Values: [SCALAR_TYPE | SSE_TYPE | AVX_TYPE | MASK_AVX_TYPE | OGL_TYPE]
SET culling=true                        &:: default: true       Values: [true | false]
SET frustumCulling=true                 &:: default: true       Values: [true | false]
SET depthBuffer=false                   &:: default: false      Values: [true | false]
SET boundingBox=false                   &:: default: false      Values: [true | false]
SET multithreading=true                 &:: default: true       Values: [true | false]
SET pipeline=false                      &:: default: false      Values: [true | false]
SET cameraPosX=38.6                     &:: default: [38.6, 52.5, 166.6, 73.5, 5.0, 55.0]
SET cameraPosY=52.5
SET cameraPosZ=166.6
SET cameraLookX=73.5
SET cameraLookY=5.0
SET cameralookZ=55.0
SET occluderSizeThreshold=1.5           &:: default: 1.5        Values: [0.0 - 5.0]
SET occludeeSizeThreshold=0.01          &:: default: 0.01       Values: [0.0 - 0.1]
SET depthTestTasks=20                   &:: default: 20         Values: [1 - 50]
SET cameraTour=1                        &:: default: 0          Values: [1]
SET framesForEvaluation=100             &:: default: 0          Values: [int]
SET DBResolution=3                      &:: default: 3          Values: [0-5]
SET occludeePackage=1                   &:: default: 0          Values: [int]

START /WAIT /D %cullingPath% %OC_exe% -type %type% -depthBuffer %depthBuffer% -culling %culling% -frustumCulling %frustumCulling% -boundingBox %boundingBox% -multithreading %multithreading% -pipeline %pipeline% -camera %cameraPosX% %cameraPosY% %cameraPosZ% %cameraLookX% %cameraLookY% %cameraLookZ% -occluderSizeThreshold %occluderSizeThreshold% -occludeeSizeThreshold %occludeeSizeThreshold% -depthTestTasks %depthTestTasks% -cameraTour %cameraTour% -DBResolution %DBResolution% -occludeePackage %occludeePackage% -evaluationName %evaluationName% -evaluationNr %evaluationNr% -framesForEvaluation %framesForEvaluation%
python %evaluationPath%/create_diagram_for_value.py %evaluationName% %currentEvaluationPath% %framesForEvaluation% %*



SET evaluationName=OGL_2
SET type=OGL_TYPE                       &:: default: SSE_TYPE   Values: [SCALAR_TYPE | SSE_TYPE | AVX_TYPE | MASK_AVX_TYPE | OGL_TYPE]
SET culling=true                        &:: default: true       Values: [true | false]
SET frustumCulling=true                 &:: default: true       Values: [true | false]
SET depthBuffer=false                   &:: default: false      Values: [true | false]
SET boundingBox=false                   &:: default: false      Values: [true | false]
SET multithreading=true                 &:: default: true       Values: [true | false]
SET pipeline=false                      &:: default: false      Values: [true | false]
SET cameraPosX=38.6                     &:: default: [38.6, 52.5, 166.6, 73.5, 5.0, 55.0]
SET cameraPosY=52.5
SET cameraPosZ=166.6
SET cameraLookX=73.5
SET cameraLookY=5.0
SET cameralookZ=55.0
SET occluderSizeThreshold=1.5           &:: default: 1.5        Values: [0.0 - 5.0]
SET occludeeSizeThreshold=0.01          &:: default: 0.01       Values: [0.0 - 0.1]
SET depthTestTasks=20                   &:: default: 20         Values: [1 - 50]
SET cameraTour=1                        &:: default: 0          Values: [1]
SET framesForEvaluation=100             &:: default: 0          Values: [int]
SET DBResolution=3                      &:: default: 3          Values: [0-5]
SET occludeePackage=2                   &:: default: 0          Values: [int]

START /WAIT /D %cullingPath% %OC_exe% -type %type% -depthBuffer %depthBuffer% -culling %culling% -frustumCulling %frustumCulling% -boundingBox %boundingBox% -multithreading %multithreading% -pipeline %pipeline% -camera %cameraPosX% %cameraPosY% %cameraPosZ% %cameraLookX% %cameraLookY% %cameraLookZ% -occluderSizeThreshold %occluderSizeThreshold% -occludeeSizeThreshold %occludeeSizeThreshold% -depthTestTasks %depthTestTasks% -cameraTour %cameraTour% -DBResolution %DBResolution% -occludeePackage %occludeePackage% -evaluationName %evaluationName% -evaluationNr %evaluationNr% -framesForEvaluation %framesForEvaluation%
python %evaluationPath%/create_diagram_for_value.py %evaluationName% %currentEvaluationPath% %framesForEvaluation% %*



SET evaluationName=OGL_3
SET type=OGL_TYPE                       &:: default: SSE_TYPE   Values: [SCALAR_TYPE | SSE_TYPE | AVX_TYPE | MASK_AVX_TYPE | OGL_TYPE]
SET culling=true                        &:: default: true       Values: [true | false]
SET frustumCulling=true                 &:: default: true       Values: [true | false]
SET depthBuffer=false                   &:: default: false      Values: [true | false]
SET boundingBox=false                   &:: default: false      Values: [true | false]
SET multithreading=true                 &:: default: true       Values: [true | false]
SET pipeline=false                      &:: default: false      Values: [true | false]
SET cameraPosX=38.6                     &:: default: [38.6, 52.5, 166.6, 73.5, 5.0, 55.0]
SET cameraPosY=52.5
SET cameraPosZ=166.6
SET cameraLookX=73.5
SET cameraLookY=5.0
SET cameralookZ=55.0
SET occluderSizeThreshold=1.5           &:: default: 1.5        Values: [0.0 - 5.0]
SET occludeeSizeThreshold=0.01          &:: default: 0.01       Values: [0.0 - 0.1]
SET depthTestTasks=20                   &:: default: 20         Values: [1 - 50]
SET cameraTour=1                        &:: default: 0          Values: [1]
SET framesForEvaluation=100             &:: default: 0          Values: [int]
SET DBResolution=3                      &:: default: 3          Values: [0-5]
SET occludeePackage=3                   &:: default: 0          Values: [int]

START /WAIT /D %cullingPath% %OC_exe% -type %type% -depthBuffer %depthBuffer% -culling %culling% -frustumCulling %frustumCulling% -boundingBox %boundingBox% -multithreading %multithreading% -pipeline %pipeline% -camera %cameraPosX% %cameraPosY% %cameraPosZ% %cameraLookX% %cameraLookY% %cameraLookZ% -occluderSizeThreshold %occluderSizeThreshold% -occludeeSizeThreshold %occludeeSizeThreshold% -depthTestTasks %depthTestTasks% -cameraTour %cameraTour% -DBResolution %DBResolution% -occludeePackage %occludeePackage% -evaluationName %evaluationName% -evaluationNr %evaluationNr% -framesForEvaluation %framesForEvaluation%
python %evaluationPath%/create_diagram_for_value.py %evaluationName% %currentEvaluationPath% %framesForEvaluation% %*



SET evaluationName=OGL_4
SET type=OGL_TYPE                       &:: default: SSE_TYPE   Values: [SCALAR_TYPE | SSE_TYPE | AVX_TYPE | MASK_AVX_TYPE | OGL_TYPE]
SET culling=true                        &:: default: true       Values: [true | false]
SET frustumCulling=true                 &:: default: true       Values: [true | false]
SET depthBuffer=false                   &:: default: false      Values: [true | false]
SET boundingBox=false                   &:: default: false      Values: [true | false]
SET multithreading=true                 &:: default: true       Values: [true | false]
SET pipeline=false                      &:: default: false      Values: [true | false]
SET cameraPosX=38.6                     &:: default: [38.6, 52.5, 166.6, 73.5, 5.0, 55.0]
SET cameraPosY=52.5
SET cameraPosZ=166.6
SET cameraLookX=73.5
SET cameraLookY=5.0
SET cameralookZ=55.0
SET occluderSizeThreshold=1.5           &:: default: 1.5        Values: [0.0 - 5.0]
SET occludeeSizeThreshold=0.01          &:: default: 0.01       Values: [0.0 - 0.1]
SET depthTestTasks=20                   &:: default: 20         Values: [1 - 50]
SET cameraTour=1                        &:: default: 0          Values: [1]
SET framesForEvaluation=100             &:: default: 0          Values: [int]
SET DBResolution=3                      &:: default: 3          Values: [0-5]
SET occludeePackage=5                   &:: default: 0          Values: [int]

START /WAIT /D %cullingPath% %OC_exe% -type %type% -depthBuffer %depthBuffer% -culling %culling% -frustumCulling %frustumCulling% -boundingBox %boundingBox% -multithreading %multithreading% -pipeline %pipeline% -camera %cameraPosX% %cameraPosY% %cameraPosZ% %cameraLookX% %cameraLookY% %cameraLookZ% -occluderSizeThreshold %occluderSizeThreshold% -occludeeSizeThreshold %occludeeSizeThreshold% -depthTestTasks %depthTestTasks% -cameraTour %cameraTour% -DBResolution %DBResolution% -occludeePackage %occludeePackage% -evaluationName %evaluationName% -evaluationNr %evaluationNr% -framesForEvaluation %framesForEvaluation%
python %evaluationPath%/create_diagram_for_value.py %evaluationName% %currentEvaluationPath% %framesForEvaluation% %*



SET evaluationName=OGL_5
SET type=OGL_TYPE                       &:: default: SSE_TYPE   Values: [SCALAR_TYPE | SSE_TYPE | AVX_TYPE | MASK_AVX_TYPE | OGL_TYPE]
SET culling=true                        &:: default: true       Values: [true | false]
SET frustumCulling=true                 &:: default: true       Values: [true | false]
SET depthBuffer=false                   &:: default: false      Values: [true | false]
SET boundingBox=false                   &:: default: false      Values: [true | false]
SET multithreading=true                 &:: default: true       Values: [true | false]
SET pipeline=false                      &:: default: false      Values: [true | false]
SET cameraPosX=38.6                     &:: default: [38.6, 52.5, 166.6, 73.5, 5.0, 55.0]
SET cameraPosY=52.5
SET cameraPosZ=166.6
SET cameraLookX=73.5
SET cameraLookY=5.0
SET cameralookZ=55.0
SET occluderSizeThreshold=1.5           &:: default: 1.5        Values: [0.0 - 5.0]
SET occludeeSizeThreshold=0.01          &:: default: 0.01       Values: [0.0 - 0.1]
SET depthTestTasks=20                   &:: default: 20         Values: [1 - 50]
SET cameraTour=1                        &:: default: 0          Values: [1]
SET framesForEvaluation=100             &:: default: 0          Values: [int]
SET DBResolution=3                      &:: default: 3          Values: [0-5]
SET occludeePackage=10                  &:: default: 0          Values: [int]

START /WAIT /D %cullingPath% %OC_exe% -type %type% -depthBuffer %depthBuffer% -culling %culling% -frustumCulling %frustumCulling% -boundingBox %boundingBox% -multithreading %multithreading% -pipeline %pipeline% -camera %cameraPosX% %cameraPosY% %cameraPosZ% %cameraLookX% %cameraLookY% %cameraLookZ% -occluderSizeThreshold %occluderSizeThreshold% -occludeeSizeThreshold %occludeeSizeThreshold% -depthTestTasks %depthTestTasks% -cameraTour %cameraTour% -DBResolution %DBResolution% -occludeePackage %occludeePackage% -evaluationName %evaluationName% -evaluationNr %evaluationNr% -framesForEvaluation %framesForEvaluation%
python %evaluationPath%/create_diagram_for_value.py %evaluationName% %currentEvaluationPath% %framesForEvaluation% %*



SET evaluationName=OGL_6
SET type=OGL_TYPE                       &:: default: SSE_TYPE   Values: [SCALAR_TYPE | SSE_TYPE | AVX_TYPE | MASK_AVX_TYPE | OGL_TYPE]
SET culling=true                        &:: default: true       Values: [true | false]
SET frustumCulling=true                 &:: default: true       Values: [true | false]
SET depthBuffer=false                   &:: default: false      Values: [true | false]
SET boundingBox=false                   &:: default: false      Values: [true | false]
SET multithreading=true                 &:: default: true       Values: [true | false]
SET pipeline=false                      &:: default: false      Values: [true | false]
SET cameraPosX=38.6                     &:: default: [38.6, 52.5, 166.6, 73.5, 5.0, 55.0]
SET cameraPosY=52.5
SET cameraPosZ=166.6
SET cameraLookX=73.5
SET cameraLookY=5.0
SET cameralookZ=55.0
SET occluderSizeThreshold=1.5           &:: default: 1.5        Values: [0.0 - 5.0]
SET occludeeSizeThreshold=0.01          &:: default: 0.01       Values: [0.0 - 0.1]
SET depthTestTasks=20                   &:: default: 20         Values: [1 - 50]
SET cameraTour=1                        &:: default: 0          Values: [1]
SET framesForEvaluation=100             &:: default: 0          Values: [int]
SET DBResolution=3                      &:: default: 3          Values: [0-5]
SET occludeePackage=15                  &:: default: 0          Values: [int]

START /WAIT /D %cullingPath% %OC_exe% -type %type% -depthBuffer %depthBuffer% -culling %culling% -frustumCulling %frustumCulling% -boundingBox %boundingBox% -multithreading %multithreading% -pipeline %pipeline% -camera %cameraPosX% %cameraPosY% %cameraPosZ% %cameraLookX% %cameraLookY% %cameraLookZ% -occluderSizeThreshold %occluderSizeThreshold% -occludeeSizeThreshold %occludeeSizeThreshold% -depthTestTasks %depthTestTasks% -cameraTour %cameraTour% -DBResolution %DBResolution% -occludeePackage %occludeePackage% -evaluationName %evaluationName% -evaluationNr %evaluationNr% -framesForEvaluation %framesForEvaluation%
python %evaluationPath%/create_diagram_for_value.py %evaluationName% %currentEvaluationPath% %framesForEvaluation% %*



SET evaluation=OGL_0 OGL_1 OGL_2 OGL_3 OGL_4 OGL_5 OGL_6 "0" "1" "2" "3" "5" "10" "15" 
SET output=%evaluationNr%_Results
:: [vale_to_compare] [output_folder] [Evaluation_Run_A].....[Evaluation_Run_Z] [legend_name_A].....[legend_name_Z]
:: [vale_to_compare] [output_folder] [Evaluation_Run_A].....[Evaluation_Run_Z] [legend_name_A].....[legend_name_Z]
python %evaluationPath%/compare_value_in_diagram.py %currentEvaluationPath% "Culled Tries" %output% %evaluation%
python %evaluationPath%/compare_value_in_diagram.py %currentEvaluationPath% "Culling time" %output% %evaluation%
python %evaluationPath%/compare_value_in_diagram.py %currentEvaluationPath% "Depth rasterized models" %output% %evaluation%
python %evaluationPath%/compare_value_in_diagram.py %currentEvaluationPath% "DepthTest Time" %output% %evaluation%
python %evaluationPath%/compare_value_in_diagram.py %currentEvaluationPath% "Draw Calls" %output% %evaluation%
python %evaluationPath%/compare_value_in_diagram.py %currentEvaluationPath% FPS %output% %evaluation%
python %evaluationPath%/compare_value_in_diagram.py %currentEvaluationPath% "Models Culled" %output% %evaluation%
python %evaluationPath%/compare_value_in_diagram.py %currentEvaluationPath% "Models Visible" %output% %evaluation%
python %evaluationPath%/compare_value_in_diagram.py %currentEvaluationPath% "Rasterize Time" %output% %evaluation%
python %evaluationPath%/compare_value_in_diagram.py %currentEvaluationPath% "Visible Tries" %output% %evaluation%
python %evaluationPath%/compare_value_in_diagram.py %currentEvaluationPath% "Percentage culled" %output% %evaluation%