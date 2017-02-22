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
#include <omp.h>
#include <valarray>
#include <set>
#include <utility>
#include <vector>
#include <algorithm>
#include "BinaryString.h"
#include "StatisticalMap3D.h"
#include "mystat.h"

const int MAX_PERMUTATIONS_ALLOWED = 1024;

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

    // perform here allocation of any "state" memory,
    //which you want to use during repeated calls to "execute()" function
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
    char string_single_or_multy[101];


	qxGetStringParameter("Command", task_name); 
	qxGetStringParameter("H", string_h);
	qxGetStringParameter("E", string_e);
	qxGetStringParameter("dh", string_dh);
	qxGetStringParameter("neg", string_neg_or_pos);
    qxGetStringParameter("single", string_single_or_multy);

	float H, E, dh;
    int pos_or_neg, single_or_multy;

	H = atof(string_h);
	E = atof(string_e);
	dh = atof(string_dh);
	pos_or_neg = atoi(string_neg_or_pos);
    single_or_multy = atoi(string_single_or_multy);

	char InfoString[501];

	if( !strcmp(task_name, "Calculate") ){		
		bool b_voxels;
		//getting the parameters.
        b_voxels = CalculateTFCE(E, H, dh, pos_or_neg,single_or_multy);
		return b_voxels;
	}

	sprintf(InfoString, "Plugin>  Received unknown command: \"%s\".", task_name);
	qxLogText(InfoString);

	return true;
}

int TfceScore::CalculateTFCE(float E, float H, float dh, int pos_or_neg, int single_or_multy)
{
	char buffer[100];
	float * vv = NULL;
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

    if (qxGetNRVMPsOfCurrentVMR(&vmps_header) != NULL) {
		int dimX = (vmps_header.XEnd - vmps_header.XStart) / vmps_header.Resolution;
		int dimY = (vmps_header.YEnd - vmps_header.YStart) / vmps_header.Resolution;
		int dimZ = (vmps_header.ZEnd - vmps_header.ZStart) / vmps_header.Resolution;

		dim = dimX*dimY*dimZ;

		//this for loop should select currently overlayed vmp sub map
		int num_of_maps = vmps_header.NrOfMaps;

        if(single_or_multy == 1){
            sprintf(buffer, "Single study for T map");
            qxLogText(buffer);
            for (overlayed_vmp_index = 0; overlayed_vmp_index < num_of_maps; overlayed_vmp_index++){
                vv = qxGetNRVMPOfCurrentVMR(overlayed_vmp_index, &vmp_header);
                if (vmp_header.OverlayMap)
                    break;
            }

            //Remember: Negative maps receive positive scores

            //copying vmp in a object
            StatisticalMap3D tfceMap(vv, dimX, dimY, dimZ);

            //eliminating positives or negatives
            tfceMap.zeroMap(pos_or_neg);

            //flipping if negatives
            if (pos_or_neg == -1){
                tfceMap.flipMap();
            }

            qxLogText("Plugin> Starting to calculate TFCE...");
            //omp_set_num_threads(omp_get_num_procs());
            tfceMap.tfce(E, H, dh);
            tfceMap.findMinMax(min, max, range);
            sprintf(buffer, "Score minimo: %f Score massimo: %f\n", min, max);
            qxLogText(buffer);

            //copying tfce map in visualized map
            for (int i = 0; i < dim; ++i) {
                vv[i] = tfceMap[i];
            }

            //Computing visualization bounds
            float max_t = max;
            float min_t = max - (max*50) / 100;

            sprintf(buffer, "Score minimo visualizzato: %f Score massimo visualizzato: %f", min_t, max_t);
            qxLogText(buffer);

            //Refreshing vmp header
            //Setting up thresholds
            vmp_header.ThreshMax = max_t;
            vmp_header.ThreshMin = min_t;
            //No cluster based threshold
            vmp_header.UseClusterSize = 0;
            //Show only positive scores
            vmp_header.ShowPosOrNegOrBoth = 1;

            //Change visualized map parameters with refreshed vmp header
            qxSetNRVMPParametersOfCurrentVMR(overlayed_vmp_index, &vmp_header);
            //Refresh
            qxUpdateActiveWindow();
            sprintf(buffer, "Finished calculation");
            qxLogText(buffer);
        }else{
            sprintf(buffer, "Multy study on beta maps");
            qxLogText(buffer);
            if(num_of_maps == 1){
                sprintf(buffer, "For multy study on beta maps you need more than one vmp");
                qxLogText(buffer);
                return false;
            }
            int total_num_of_permutations = 1 << num_of_maps;
            int permutations_used;
            std::set<BinaryString> permutations;
            //explicit insertion of original permutation in each case
            BinaryString original_permutation(num_of_maps, 0);
            permutations.insert(original_permutation);
            if(total_num_of_permutations <= MAX_PERMUTATIONS_ALLOWED){
                permutations_used = total_num_of_permutations;
                for (int i = 1; i < permutations_used; ++i) {
                    permutations.insert(BinaryString(num_of_maps, i));
                }
            }else{
                permutations_used = MAX_PERMUTATIONS_ALLOWED;
                for (int i = 1; i < permutations_used; ++i) {
                    while(true){
                        auto x = permutations.insert(BinaryString(num_of_maps, true));
                        if(x.second == false)
                            break;
                    }
                }
            }
            std::vector<float> maps_maxinum(permutations_used);
            float * voxels = new float[num_of_maps];
            float * map = new float[dim];
            int j = 0;
            bool first_permutation = true;
            StatisticalMap3D originalPermutationMap;
            //thanks to < operator, we are sure that the original permutation is the first
            //in any ordering
            for(auto& perm: permutations){
                //for each permutation, we create a map voxel by voxel...
                for (int current_voxel = 0; current_voxel < dim; current_voxel++){
                    //...using ttest on the beta maps
                    for (int i = 0; i < num_of_maps; ++i) {
                        vv = qxGetNRVMPOfCurrentVMR(i, &vmp_header);
                        if(perm[i] == 1)
                            voxels[i] = -vv[current_voxel];
                        else
                            voxels[i] = vv[current_voxel];
                    }
                    ttest1sample(voxels, num_of_maps, 0, &map[current_voxel]);
                }
                StatisticalMap3D currentPermutationMap(map, dimX, dimY, dimZ);
                currentPermutationMap.tfce();
                if(first_permutation){
                    first_permutation = false;
                    originalPermutationMap = currentPermutationMap;
                }
                currentPermutationMap.findMinMax(min, max, range);
                maps_maxinum[j++] = max;
            }
            delete [] map;
            delete [] voxels;
            std::sort(maps_maxinum.begin(), maps_maxinum.end());
            int percentile_index = 0.95 * permutations_used;
            float percentile_threshold = maps_maxinum[percentile_index];
            originalPermutationMap.thresholdMap(percentile_threshold);
            //Due strade possibili:
            //Prima strada:
            //Calcolare la distribuzione dei massimi. Andare a vedere, per ogni permutazione, il massimo. (approccio conservativo)
            //Calcolare i quantili, ordinando i valori. (95 percentile). Sogliare la mappa tramite soglia globale.


            //Finished calculations, showing results
            //Finding actual vmp visualized
            for (overlayed_vmp_index = 0; overlayed_vmp_index < num_of_maps; overlayed_vmp_index++){
                vv = qxGetNRVMPOfCurrentVMR(overlayed_vmp_index, &vmp_header);
                if (vmp_header.OverlayMap)
                    break;
            }

            //copying tfce map in visualized map
            for (int i = 0; i < dim; ++i) {
                vv[i] = originalPermutationMap[i];
            }

            //Computing visualization bounds
            //float max_t = max;
            //float min_t = max - (max*50) / 100;

            //sprintf(buffer, "Score minimo visualizzato: %f Score massimo visualizzato: %f", min_t, max_t);
            //qxLogText(buffer);

            //Refreshing vmp header
            //Setting up thresholds
            //vmp_header.ThreshMax = max_t;
            //vmp_header.ThreshMin = min_t;
            //No cluster based threshold
            //vmp_header.UseClusterSize = 0;
            //Show only positive scores
            //vmp_header.ShowPosOrNegOrBoth = 1;

            //Change visualized map parameters with refreshed vmp header
            //qxSetNRVMPParametersOfCurrentVMR(overlayed_vmp_index, &vmp_header);
            //Refresh
            //qxUpdateActiveWindow();
            sprintf(buffer, "Finished calculation");
            qxLogText(buffer);
        }
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
