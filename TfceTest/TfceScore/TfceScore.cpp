// TfceScore.cpp

#include "TfceScore.h"


#if defined(OS_WIN32)

#include <windows.h>
#include <malloc.h>
#include <io.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <exception>
#include "Tfce.h"
#include "Utilities.h"
#include "Fwhm.h"
#include "GaussianKernel.h"
#include <omp.h>

void threshold_matrix(float * vector, int dim, float posValue, float negValue){
	int i;

	for (i = 0; i < dim; i++){
		if (vector[i] > 0){
			if (vector[i] > posValue)
				vector[i] -= posValue;
			else
				vector[i] = 0;
		}
		if (vector[i] < 0){
			if (vector[i] < negValue)
				vector[i] -= negValue;
			else
				vector[i] = 0;
		}
	}
}

int mycompare(const void * v1, const void * v2){
	float real_value1, real_value2;

	real_value1 = *((float *)v1);
	real_value2 = *((float *)v2);

	if (real_value1 < real_value2)
		return -1;
	else if (real_value1 == real_value2)
		return 0;
	else
		return 1;
}

void applicaPermutazioni(float * vv, int dimX, int dimY, int dimZ, float z_threshold, float E, float H, float dh, int rep, float * scores){
	int dim = dimX * dimY * dimZ;
	float * matrix = new float[dim]; 
	float min, max, range;
	char buffer[100];

	float *vetmax = new float[rep];
	float *vetmin = new float[rep];

	/*
	sprintf(buffer, "Plugin> Producing gaussian kernel...");
	qxLogText(buffer);
	float *** ker = produce3dGaussianKernel(3, f);

	sprintf(buffer, "Plugin> Done!!");
	qxLogText(buffer);
	*/


	for (int i = 0; i < rep; i++){
		//printf("Main: %d\n",vetThread[i]);
		copyMatrix(matrix, vv, dim);
		shuffle(matrix, dim);
		//sprintf(buffer, "Plugin> Applying gaussian filter!!");
		//qxLogText(buffer);
		//float * smoothed_map = apply3DGaussianFilter(ker, matrix, x, y, z);
		//sprintf(buffer, "Plugin> Done!!");
		//qxLogText(buffer);
		float * tfce_score_matrix = tfce_score(matrix, dimX, dimY, dimZ, z_threshold, E, H, dh);
		//fflush(stdout);
		findMinMax(tfce_score_matrix, dim, &min, &max, &range);
		vetmax[i] = max;
		vetmin[i] = min;
		sprintf(buffer, "Plugin> Finished permutation %d", i);
		qxLogText(buffer);
		delete[] tfce_score_matrix;
	}

	int indexPerc;
	int indexNegPerc;
	if (rep == 1){
		indexPerc = 0;
		indexNegPerc = 0;
	}
	else{
		qsort(vetmax, rep, sizeof(float), mycompare);
		qsort(vetmin, rep, sizeof(float), mycompare);
		indexPerc = (int)(rep * 95 / 100) - 1;
		indexNegPerc = rep - indexPerc;
		if (indexNegPerc < 0)
			indexNegPerc = 0;
	}

	sprintf(buffer, "Indice 95 percentile positivi %d\n", indexPerc);
	qxLogText(buffer);

	sprintf(buffer, "Indice 95 percentile negativi %d\n", indexNegPerc);
	qxLogText(buffer);

	sprintf(buffer, "Valore 95 percentile massimi %f\n", vetmax[indexPerc]);
	qxLogText(buffer);

	sprintf(buffer, "Valore 95 percentile minimi %f\n", vetmin[indexNegPerc]);
	qxLogText(buffer);

	if (vetmin[indexNegPerc]<0)
		threshold_matrix(scores, dim, vetmax[indexPerc], vetmin[indexNegPerc]);
	else
		threshold_matrix(scores, dim, vetmax[indexPerc], 0);
	delete[] matrix;
}

// constructor of your GUI plugin class
//
TfceScore::TfceScore()
{
	// initialize values here or in "initPlugin()"
}

// destructor of your plugin class - release any memory created on heap in "initPlugin()" or "execute()" 
//
TfceScore::~TfceScore()
{
	// clean any "state" memory
	//
	qxLogText("Plugin> Tfce Score finished!");
	qxLogText(" ");
	qxUpdateActiveWindow();
}

bool TfceScore::initPlugin(){

	// This function call and check of return value is necessary to check whether access to QX works
	int ret = InitQXAccess();

	// Never continue, if the "InitQXAccess" function fails
	if(!ret)
		return false;

	// Test whether the used QX version is new enough to support the commands used in your plugin
	int Major, Minor, SubMinor;
	qxGetVersionOfBrainVoyagerQX(&Major, &Minor, &SubMinor);

	// we require v2.1.0 for GUI plugins
	if( !checkVersionGreaterEqualVersion(Major, Minor, SubMinor, 2, 1, 0) )
	{
		// we can use message box since this was available with the first QX version with plugin support (v1.3.8)
		qxMessageBox("Can not execute this plugin - BrainVoyagerQX version 2.1 or higher required.");
		return false;
	}

	qxLogText(" ");
	qxLogText("Plugin>  Running Tfce Score...");
	qxLogText("Plugin>");

	// perform here allocation of any "state" memory, which you want to use during repeated calls to "execute()" function
	//
	int current_doc_index = qxGetIDOfCurrentDocument(); // check for return value < 0
	if(current_doc_index < 0)
	{
		qxMessageBox("No current document available.");
		qxLogText("No current document available.");
		return false;
	}
	int DocType = qxGetDocumentType(current_doc_index);
	if(DocType != 3)
	{
		qxMessageBox("The currently active document is not a VMR document.");
		qxLogText("The currently active document is not a VMR document.");
		return false;
	}

	return true; // return "false" (see above) if something goes wrong, then plugin execution is cancelled by BVQX
}

// This function contains the core computational code - it can be called repeatedly in case of GUI plugin
// Task name comes from controller (javascript)
bool TfceScore::execute(){
	
	char task_name[101];
	char string_e[101];
	char string_h[101];
	char string_dh[101];
	char string_z[101];
	char string_rep[101]; 
	char string_thresh[101];

	qxGetStringParameter("Command", task_name); 
	qxGetStringParameter("H", string_h);
	qxGetStringParameter("E", string_e);
	qxGetStringParameter("Z", string_z);
	qxGetStringParameter("dh", string_dh);
	qxGetStringParameter("check", TfceScore::string_check);
	qxGetStringParameter("rep", string_rep);
	qxGetStringParameter("thresh", string_thresh);

	float H, E, Z, dh;

	H = atof(string_h);
	E = atof(string_e);
	Z = atof(string_z);
	dh = atof(string_dh);
	rep = atoi(string_rep);
	TfceScore::thresh = atof(string_thresh);

	char InfoString[501];
	sprintf(InfoString, "Plugin>  Command received: %s Parameters: H: %s E: %s dh: %s Z:%s Check:%s Rep:%s Thresh:%s", task_name, string_h, string_e, string_dh, string_z, string_check,string_rep, string_thresh);
	qxLogText(InfoString);

	if( !strcmp(task_name, "Calculate") ){		
		bool b_voxels;
		//getting the parameters.
		b_voxels = CalculateTFCE(Z, E, H, dh);
		return b_voxels;
	}

	sprintf(InfoString, "Plugin>  Received unknown command: \"%s\".", task_name);
	qxLogText(InfoString);

	return true;
}

int TfceScore::CalculateTFCE(float z_threshold, float E, float H, float dh)
{
	char buffer[100];
	float **vmp;
	float * vv = NULL;
	bool log = 1;
	int overlayed_vmp_index = 0;
	float min, max, range;
	struct VMR_Header vmr_header;
	struct NR_VMPs_Header vmps_header;
	struct NR_VMP_Header vmp_header;
	float * matrix;
	int dim;

	if (!qxGetMainHeaderOfCurrentVMR(&vmr_header)){
		qxLogText("Plugin>  Problem Get resolution voxel");
		return false;
	}

	if ((vmp = qxGetNRVMPsOfCurrentVMR(&vmps_header)) != NULL) {
		int dimX = (vmps_header.XEnd - vmps_header.XStart) / vmps_header.Resolution;
		int dimY = (vmps_header.YEnd - vmps_header.YStart) / vmps_header.Resolution;
		int dimZ = (vmps_header.ZEnd - vmps_header.ZStart) / vmps_header.Resolution;
		dim = dimX * dimY * dimZ;

		sprintf(buffer, "Plugin> Linear dimension %d", dim);
		qxLogText(buffer);

		//this for loop should select currently overlayed vmp sub map
		int num_of_maps = vmps_header.NrOfMaps;
		
		for (overlayed_vmp_index = 0; overlayed_vmp_index < num_of_maps; overlayed_vmp_index++){
			vv = qxGetNRVMPOfCurrentVMR(overlayed_vmp_index, &vmp_header);
			if (vmp_header.OverlayMap)
				break;
		}
		
		//vv = qxGetNRVMPOfCurrentVMR(0, &vmp_header);
		qxLogText("Plugin> Starting to calculate TFCE...");
		omp_set_num_threads(omp_get_num_procs());
		float * scores = tfce_score(vv, dimX, dimY, dimZ, z_threshold, E, H, dh);
		
		
		/*
		float f = Fwhm(vv, dimX, dimY, dimZ);
		//f *= 2.355;
		sprintf(buffer, "Plugin> Estimated FWHM %f", f);
		qxLogText(buffer);
		*/
		
		int rep;
		char *checked = TfceScore::string_check;
		if (strcmp(checked, "true") == 0){
			rep = TfceScore::rep;
			qxLogText("Plugin> Finished TFCE, starting permutation test...");
			applicaPermutazioni(vv, dimX, dimY, dimZ, z_threshold, E, H, dh, rep, scores);
		}
		
		findMinMax(scores, dimX*dimY*dimZ, &min, &max, &range);
		memcpy(vv, scores, sizeof(float)* dimX*dimY*dimZ);
		
		float max_t = max - (max*TfceScore::thresh) / 100;
		float min_t = (-min) - ((-min)*TfceScore::thresh) / 100;

		sprintf(buffer, "min_t: %lf max_t: %lf\n", min_t, max_t);
		qxLogText(buffer);

		vmp_header.ThreshMax = max_t;
		vmp_header.ThreshMin = min_t;
		qxSetNRVMPParametersOfCurrentVMR(overlayed_vmp_index, &vmp_header);
		delete[] scores;
		//trying to refresh
		qxUpdateActiveWindow();
		
		sprintf(buffer, "min: %lf max: %lf\n", min, max);
		qxLogText(buffer);
		sprintf(buffer, "Finished calculation");
		qxLogText(buffer);
		return true;
	}
	else{
		qxLogText("Plugin>  VMP not found");
	}
	return false;
}

//
// C access functions (extern "C" in header)
//

PLUGIN_ACCESS BVQXPluginInterface *createPlugin() 
{
	return new TfceScore();
}

// return a name for your plugin
//
PLUGIN_ACCESS const char *getPluginName() 
{
	return "Tfce Score";
}

// returns a descriptive string for your plugin shown in the QX "Descriptions of Plugins" window
//   it is recommended to use a static C string to describe your plugin
//   (the potentially lengthy string needs then not be created repeatedly in successive calls)
//   Note that you may use simple HTML tags to format the string, such as <b></b>, <i></i> and <br>
//
static const char *PluginDescription = "This plugin is the porting of the famous tfce tecnique implemented in fsl.<br>";

PLUGIN_ACCESS const char *getPluginDescription() 
{
	return PluginDescription;
}

// get credit for your work!

PLUGIN_ACCESS const char *getAuthor()
{
	return "<i>Luigi Giugliano & Marco Mecchia</i>";
}

// you may provide the name for a HTML file containing online help
//
PLUGIN_ACCESS const char *getHelpFile()
{
	return "";
}

// you may provide a version string ("Major.Minor") for (updated) releases of your plugins
//
PLUGIN_ACCESS const char *getPluginVersion()
{
	return "0.5";
}

// NEW: If you want to use GUI dialogs, return a GUI script file, if not, return "" (or don't define this function)
//
PLUGIN_ACCESS const char *getGUIScript()
{
	return "TfceScore.js";
}
