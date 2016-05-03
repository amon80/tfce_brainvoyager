// TfceScore.h

#ifndef TfceScore_H
#define TfceScore_H

#include "BVQXPluginInterface.h"

/*
 * This class implements a plugin by deriving it from the generic PluginInterface
 */
class TfceScore : public BVQXPluginInterface
{

public:
	
	PLUGIN_ACCESS TfceScore();
	PLUGIN_ACCESS ~TfceScore();

	PLUGIN_ACCESS bool initPlugin();
	PLUGIN_ACCESS bool execute();

private:
	int  CalculateTFCE(float E, float H, float dh, int pos_or_neg);

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


#endif // TfceScore_H
