
#ifndef PLUGIN_GLM_HEADER_H
#define PLUGIN_GLM_HEADER_H

// header for ONE VMP
struct GLM_Header
{
	int   GLMDataType; // FMR, VTC, MTC

	int   RFXGLM;
	int   NrOfSubjectsRFXGLM;
	int   NrOfSubjectPredictorsRFXGLM;

	int   CorrectedForSerialCorrelations;

	int   XStart;  // do NOT change right now: if these 6 values are changed, you MUST also change the allocated memory of this and all other VMPs !
	int   XEnd;
	int   YStart;
	int   YEnd;
	int   ZStart;
	int   ZEnd;
	int   Resolution; // normally "3", but can be also "1" or "2"

	int   NrOfTimePoints;     // overall nr of time points used to compute GLM
	int   NrOfPredictors;     // nr of all predictors including confounds
	int   NrOfStudies;        // overall number of analyzed studies (sum of runs across all subjects)
	int   SeparatePredictors; // "0" no separate preds, "1" separate studies, "2" separate subjects
	int   PSCTransformation;  // "1" percent signal change transformation has been computed for each study
	int   ZTransformation;    // "1" z transformation has been computed for each study ("2" baseline-only z transformation)

	int   CortexBasedStatistics; // "1" a mask was used
	int   NrOfVoxelsForBonfCorrection; // either TAL space voxels of voxels in mask
	char  CortexMaskFile[260];

	float **DesignMatrix;     // pointer to design matrix: X[NrOfTimePoints][NrOfPredictors]
	float **iXX;              // pointer to inverted square matrix X transposed * X: iXX[NrOfPredictors][NrOfPredictors]

	// Note that X/Y/Z/End values are exclusive (as VTCs), compute dims as:
	// dim_x = (XEndVTC - XStartVTC)/Resolution;
	// dim_y = (YEndVTC - YStartVTC)/Resolution;
	// dim_z = (ZEndVTC - ZStartVTC)/Resolution;
};


#endif // PLUGIN_GLM_HEADER_H