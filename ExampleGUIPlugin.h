// ExampleGUIPlugin.h

#ifndef EXAMPLEGUIPLUGIN_H
#define EXAMPLEGUIPLUGIN_H

#include "BVQXPluginInterface.h"

/*
 * This class implements a plugin by deriving it from the generic PluginInterface
 */
class ExampleGUIPlugin : public BVQXPluginInterface
{
public:
	
	PLUGIN_ACCESS ExampleGUIPlugin();
	PLUGIN_ACCESS ~ExampleGUIPlugin();

	PLUGIN_ACCESS bool initPlugin();
	PLUGIN_ACCESS bool execute();

private:
	bool resetVMR();
	bool invertVMR(int min_intensity, int max_intensity);
	int  countVoxels(int min_intensity, int max_intensity);

	unsigned char *OrigVMR;
	int OrigVMRDimX, OrigVMRDimY, OrigVMRDimZ;
};

// To avoid C++ name mangling problems, we need to bootstrap access of plugin via C functions
//
extern "C" {
	PLUGIN_ACCESS BVQXPluginInterface *createPlugin();
	PLUGIN_ACCESS const char *getPluginName();
	PLUGIN_ACCESS const char *getPluginDescription();
	PLUGIN_ACCESS const char *getAuthor();
	PLUGIN_ACCESS const char *getHelpFile();
	PLUGIN_ACCESS const char *getPluginVersion();
	PLUGIN_ACCESS const char *getGUIScript();
}


#endif // EXAMPLEGUIPLUGIN_H
