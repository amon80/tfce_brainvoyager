// ExampleGUIPlugin.cpp

#include "ExampleGUIPlugin.h"

#if defined(OS_WIN32)
#include <windows.h>
#include <malloc.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// constructor of your GUI plugin class
//
ExampleGUIPlugin::ExampleGUIPlugin()
{
	// initialize values here or in "initPlugin()" (see below)
}

// destructor of your plugin class - release any memory created on heap in "initPlugin()" or "execute()" 
//
ExampleGUIPlugin::~ExampleGUIPlugin()
{
	// clean any "state" memory
	//
	if(OrigVMR != NULL)
		free(OrigVMR);

	qxLogText("Plugin>  Example GUI Plugin finished!");
	qxLogText(" ");
}


bool ExampleGUIPlugin::initPlugin()
{
	// This function call and check of return value is necessary to check whether access to QX works
	//
	int ret = InitQXAccess();

	// Never continue, if the "InitQXAccess" function fails
	//
	if(!ret)
		return false;

	// Test whether the used QX version is new enough to support the commands used in your plugin
	//
	int Major, Minor, SubMinor;
	qxGetVersionOfBrainVoyagerQX(&Major, &Minor, &SubMinor);

	// we require v2.1.0 for GUI plugins
	if( !checkVersionGreaterEqualVersion(Major, Minor, SubMinor, 2, 1, 0) )
	{
		// we can use message box since this was available with the first QX version with plugin support (v1.3.8)
		qxMessageBox("Can not execute this plugin - BrainVoyagerQX version 2.1 or higher required.");
		return false;
	}

	// this command makes sure that the user sees the "Log" tab. Use this tab to print useful information.
	qxShowLogTab();
	// this command sends a line of text to the "Log" tab.
	qxLogText(" ");
	qxLogText("Plugin>  Running Example GUI Plugin...");
	qxLogText("Plugin>");

	// perform here allocation of any "state" memory, which you want to use during repeated calls to "execute()" function
	//

	// we allocate memory to hold the content of the current VMR - allowing to reset the later modified intensity values to the original state
	int current_doc_index = qxGetIDOfCurrentDocument(); // check for return value < 0
	if(current_doc_index < 0)
	{
		qxMessageBox("No current document available.");
		return false;
	}
	int DocType = qxGetDocumentType(current_doc_index);
	if(DocType != 3)
	{
		qxMessageBox("The currently active document is not a VMR document.");
		return false;
	}
	// Get a pointer to the VMR and its dims
	int DimX, DimY, DimZ;
	unsigned char *VMR = qxGetCurrentVMR(&DimX, &DimY, &DimZ);

	OrigVMR = (unsigned char *)malloc(DimX*DimY*DimZ*sizeof(unsigned char)); // "sizeof" could be dropped since it returns "1" (one byte per voxel)
	memcpy(OrigVMR, VMR, DimX*DimY*DimZ*sizeof(unsigned char)); // perform quick copy (avoiding loop)

	OrigVMRDimX = DimX; OrigVMRDimY = DimY; OrigVMRDimZ = DimZ; // keep original dims to check that VMR (dims) are the same when calling "undo"

	return true; // return "false" (see above) if something goes wrong, then plugin execution is cancelled by BVQX
}

// This function contains the core computational code - it can be called repeatedly in case of GUI plugin
//
bool ExampleGUIPlugin::execute()
{
	// let's get some parameters from GUI script:
	//
	char task_name[101];
	qxGetStringParameter("Command", task_name); // in this variable, we code the task we should perform

	// print obtained command in "Log" tab
	//
	char InfoString[501];
	sprintf(InfoString, "Plugin>  Command received: %s", task_name);
	qxLogText(InfoString);

	int minIntensity = qxGetIntParameter("MinimumIntensity");
	int maxIntensity = qxGetIntParameter("MaximumIntensity");

	if( !strcmp(task_name, "Undo") )
		return resetVMR();
	else if( !strcmp(task_name, "Invert") )
	{
		return invertVMR(minIntensity, maxIntensity);
	}
	else if( !strcmp(task_name, "CountVoxels") )
	{
		int n_voxels = countVoxels(minIntensity, maxIntensity);

		qxSetIntParameter("NumberOfCountedVoxels", n_voxels);

		return true;
	}

	sprintf(InfoString, "Plugin>  Received unknown command: \"%s\".", task_name);
	qxLogText(InfoString);

	return true;
}

bool ExampleGUIPlugin::resetVMR()
{
	// get pointer to VMR and its dims
	int DimX, DimY, DimZ;
	unsigned char *VMR = qxGetCurrentVMR(&DimX, &DimY, &DimZ);

	// check that dims have not changed 
	if((DimX != OrigVMRDimX) || (DimY != OrigVMRDimY) || (DimZ != OrigVMRDimZ))
		return false;

	memcpy(VMR, OrigVMR, DimX*DimY*DimZ*sizeof(unsigned char)); // perform quick copy (avoiding loop)

	qxUpdateActiveWindow();

	qxLogText("Plugin>  VMR restored to original intensity values!");

	return true;
}

bool ExampleGUIPlugin::invertVMR(int min_intensity, int max_intensity)
{
	// get pointer to VMR and its dims
	int DimX, DimY, DimZ;
	unsigned char *VMR = qxGetCurrentVMR(&DimX, &DimY, &DimZ);

	// check that dims have not changed 
	if((DimX != OrigVMRDimX) || (DimY != OrigVMRDimY) || (DimZ != OrigVMRDimZ))
		return false;

	// Now invert the intensities of the VMR
	//
	int x, y, z;
	unsigned char val;

	for(z=0; z<DimZ; z++)
	{
		for(y=0; y<DimY; y++)
		{
			for(x=0; x<DimX; x++)
			{
				val = VMR[z*DimX*DimY + y*DimX + x];

				if((val >= min_intensity) && (val <= max_intensity))
					VMR[z*DimX*DimY + y*DimX + x] = 225 - val;
			}
		}
	}

	qxUpdateActiveWindow(); // To make the change visible, update the active window in QX

	qxLogText("Plugin>  Intensities inverted!");

	return true;
}

int ExampleGUIPlugin::countVoxels(int min_intensity, int max_intensity)
{
	// get pointer to VMR and its dims
	int DimX, DimY, DimZ;
	unsigned char *VMR = qxGetCurrentVMR(&DimX, &DimY, &DimZ);

	// check that dims have not changed 
	if((DimX != OrigVMRDimX) || (DimY != OrigVMRDimY) || (DimZ != OrigVMRDimZ))
		return -1;

	// Now invert the intensities of the VMR
	//
	int x, y, z, counter = 0;
	unsigned char val;

	for(z=0; z<DimZ; z++)
	{
		for(y=0; y<DimY; y++)
		{
			for(x=0; x<DimX; x++)
			{
				val = VMR[z*DimX*DimY + y*DimX + x];
	
				if((val >= min_intensity) && (val <= max_intensity))
					counter++;
			}
		}
	}
	return counter;
}


//
// C access functions (extern "C" in header)
//

// THIS FUNCTION CREATES YOUR PLUGIN CLASS
//
PLUGIN_ACCESS BVQXPluginInterface *createPlugin() 
{
	return new ExampleGUIPlugin();
}

// return a name for your plugin
//
PLUGIN_ACCESS const char *getPluginName() 
{
	return "Example GUI Plugin";
}

// returns a descriptive string for your plugin shown in the QX "Descriptions of Plugins" window
//   it is recommended to use a static C string to describe your plugin
//   (the potentially lengthy string needs then not be created repeatedly in successive calls)
//   Note that you may use simple HTML tags to format the string, such as <b></b>, <i></i> and <br>
//
static const char *PluginDescription = "This simple GUI plugin shows how to exchange information with a GUI script and a simple dialog that allows to perform simple operations (invert intensities in range, count voxels in range, undo) on the current VMR. You may use this source code to get started with writing your own GUI plugins.<br>";

PLUGIN_ACCESS const char *getPluginDescription() 
{
	return PluginDescription;
}

// get credit for your work!
//
PLUGIN_ACCESS const char *getAuthor()
{
	return "<i>Rainer Goebel</i>, Brain Innovation B.V.";
}

// you may provide the name for a HTML file containing online help
//
PLUGIN_ACCESS const char *getHelpFile()
{
	return "ExamplePluginHelp/index.html";
}

// you may provide a version string ("Major.Minor") for (updated) releases of your plugins
//
PLUGIN_ACCESS const char *getPluginVersion()
{
	return "2.0";
}

// NEW: If you want to use GUI dialogs, return a GUI script file, if not, return "" (or don't define this function)
//
PLUGIN_ACCESS const char *getGUIScript()
{
	return "ExampleGUIPlugin.js";
}

// Rainer Goebel 2009
