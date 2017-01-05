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
#include <omp.h>
#include <valarray>

int find_index(float * vetmax, int n, int start_index, int end_index, float actual_score){
	if (start_index == end_index)
		return n;
	int middle = (start_index + end_index) / 2;
	int middle_plus_one = middle + 1;
	if (middle_plus_one == n)
		return 0;
	if (actual_score > vetmax[middle]){
		if (actual_score <= vetmax[middle_plus_one])
			return n - middle - 1;
		return find_index(vetmax, n, middle, end_index, actual_score);
	}
	return find_index(vetmax, n, start_index, middle, actual_score);
}


void threshold_matrix(float * vector, int dim, float min, float max){
	int i;

	for (i = 0; i < dim; i++){
		vector[i] = (vector[i] - min) / (max - min);
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
	char string_neg_or_pos[101];

	qxGetStringParameter("Command", task_name); 
	qxGetStringParameter("H", string_h);
	qxGetStringParameter("E", string_e);
	qxGetStringParameter("dh", string_dh);
	qxGetStringParameter("neg", string_neg_or_pos);

	float H, E, dh;
	int pos_or_neg;

	H = atof(string_h);
	E = atof(string_e);
	dh = atof(string_dh);
	pos_or_neg = atoi(string_neg_or_pos);

	char InfoString[501];

	if( !strcmp(task_name, "Calculate") ){		
		bool b_voxels;
		//getting the parameters.
		b_voxels = CalculateTFCE(E, H, dh, pos_or_neg);
		return b_voxels;
	}

	sprintf(InfoString, "Plugin>  Received unknown command: \"%s\".", task_name);
	qxLogText(InfoString);

	return true;
}

int TfceScore::CalculateTFCE(float E, float H, float dh, int pos_or_neg)
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
	int dim;

	if (!qxGetMainHeaderOfCurrentVMR(&vmr_header)){
		qxLogText("Plugin>  Problem Get resolution voxel");
		return false;
	}

	if ((vmp = qxGetNRVMPsOfCurrentVMR(&vmps_header)) != NULL) {
		int dimX = (vmps_header.XEnd - vmps_header.XStart) / vmps_header.Resolution;
		int dimY = (vmps_header.YEnd - vmps_header.YStart) / vmps_header.Resolution;
		int dimZ = (vmps_header.ZEnd - vmps_header.ZStart) / vmps_header.Resolution;

		dim = dimX*dimY*dimZ;

		//this for loop should select currently overlayed vmp sub map
		int num_of_maps = vmps_header.NrOfMaps;
		
		for (overlayed_vmp_index = 0; overlayed_vmp_index < num_of_maps; overlayed_vmp_index++){
			vv = qxGetNRVMPOfCurrentVMR(overlayed_vmp_index, &vmp_header);
			if (vmp_header.OverlayMap)
				break;
		}

		//pos_or_neg check and action here
		if (pos_or_neg){//positives only
			for (int i = 0; i < dim; i++){
				if (vv[i] < 0){
					vv[i] = 0;
				}
			}
		}
		else{ // negatives only
			for (int i = 0; i < dim; i++){
				if (vv[i] > 0){
					vv[i] = 0;
				}
				if (vv[i] < 0){
					vv[i] = -vv[i];
				}
			}
		}
		
		//vv = qxGetNRVMPOfCurrentVMR(0, &vmp_header);
		qxLogText("Plugin> Starting to calculate TFCE...");
		omp_set_num_threads(omp_get_num_procs());
		float * scores = tfce_score(vv, dimX, dimY, dimZ, E, H, dh);
		findMinMax(scores, dim, &min, &max, &range);
		sprintf(buffer, "Score minimo: %f Score massimo: %f\n", min, max);
		qxLogText(buffer);
		
		/*
		float f = Fwhm(vv, dimX, dimY, dimZ);
		//f *= 2.355;
		sprintf(buffer, "Plugin> Estimated FWHM %f", f);
		qxLogText(buffer);
		*/
		
		/*if (check){
			qxLogText("Plugin> Finished TFCE, starting permutation test...");
			applicaPermutazioni(vv, dimX, dimY, dimZ, E, H, dh, rep, scores);
		}*/
		
		//threshold_matrix(scores, dim, min, max);
		memcpy(vv, scores, sizeof(float)* dimX*dimY*dimZ);
		findMinMax(scores, dimX*dimY*dimZ, &min, &max, &range);
		delete[] scores;
		//sprintf(buffer, "Score minimo sogliato: %f Score massimo sogliato: %f\n", min, max);
		//qxLogText(buffer);
		//Computing visualization bounds
		
		float max_t = max;
		float min_t = max - (max*50) / 100;

		//sprintf(buffer, "Score minimo visualizzato: %f Score massimo visualizzato: %f", min_t, max_t);
		//qxLogText(buffer);

		//Refreshing vmp header
		vmp_header.ThreshMax = max_t;
		vmp_header.ThreshMin = min_t;
		vmp_header.UseClusterSize = 0;
		vmp_header.ShowPosOrNegOrBoth = 1;
		qxSetNRVMPParametersOfCurrentVMR(overlayed_vmp_index, &vmp_header);
		//Refresh
		qxUpdateActiveWindow();
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
