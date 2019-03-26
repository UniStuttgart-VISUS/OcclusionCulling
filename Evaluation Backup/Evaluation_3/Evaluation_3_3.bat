SET currentEvaluationPath=%~dp0
SET evaluationPath=%currentEvaluationPath:~0,-13%
SET cullingPath=%currentEvaluationPath:~0,-25%


SET evaluation=Evaluation_1_OGL Evaluation_2_OGL "Test" "alt"
SET output=Evaluation_3_Results_Test
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