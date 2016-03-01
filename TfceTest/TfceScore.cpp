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
	qxGetStringParameter("Command", task_name); // in this variable, we code the task we should perform
	
	char InfoString[501];
	sprintf(InfoString, "Plugin>  Command received: %s", task_name);
	qxLogText(InfoString);

	if( !strcmp(task_name, "Calculate") ){		
		bool b_voxels;
		b_voxels = CalculateTFCE();
		return b_voxels;
	}

	sprintf(InfoString, "Plugin>  Received unknown command: \"%s\".", task_name);
	qxLogText(InfoString);

	return true;
}

int TfceScore::CalculateTFCE()
{
	char buffer[100];
	float **vmp, *vv;
	bool log = 1;
	float min, max, range;
	struct VMR_Header vmr_header;
	struct NR_VMPs_Header vmps_header;
	struct NR_VMP_Header vmp_header;

	if (!qxGetMainHeaderOfCurrentVMR(&vmr_header)){
		qxLogText("Plugin>  Problem Get resolution voxel");
		return false;
	}

	if ((vmp = qxGetNRVMPsOfCurrentVMR(&vmps_header)) != NULL) {
		int dimX = (vmps_header.XEnd - vmps_header.XStart) / vmps_header.Resolution;
		int dimY = (vmps_header.YEnd - vmps_header.YStart) / vmps_header.Resolution;
		int dimZ = (vmps_header.ZEnd - vmps_header.ZStart) / vmps_header.Resolution;
		vv = qxGetNRVMPOfCurrentVMR(0, &vmp_header);

		try{
			qxLogText("Plugin> Starting to calculate TFCE...");
			float * scores = tfce_score(vv, dimX, dimY, dimZ, 0, 0.5, 2, 0.1);
			findMinMax(scores, dimX*dimY*dimZ, &min, &max, &range);
		}
		catch (std::exception& e){
			qxLogText(e.what());
		}
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
static const char *PluginDescription = "This very simple plugin retrieve various information about the voxel selected on the current VMR document.<br>";

PLUGIN_ACCESS const char *getPluginDescription() 
{
	return PluginDescription;
}

// get credit for your work!
// fuck you goebel
PLUGIN_ACCESS const char *getAuthor()
{
	return "<i>Marco Mecchia e Luigi Giugliano</i>";
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
