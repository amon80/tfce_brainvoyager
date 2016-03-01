
var bvqx = BrainVoyagerQX; // use short symbol to access BrainVoyager QX API

var scriptObj = new Object;

scriptObj.dialogFileName   = "TfceScore.ui";  // specify name of file that will be used by BVQX to create your UI
scriptObj.dialogAccessName = "PluginDialog";         // specify the name you want to use for accessing your UI

scriptObj.initDlg = function()
{
	var dlg = this.PluginDialog; // less to write

	dlg.windowTitle = "TFCE - Threshold Free Cluster Enhancement"; // use to change dialog title
	
	// connec UI elements to script funcctions - you need to know the names of your elements (check your definition)
	dlg.tfce_calculate_button.clicked.connect(this, this.onFindVoxels);
	dlg.dh_textbox.plainText = "0.1";
	dlg.h_textbox.plainText = "0.5";
	dlg.z_textbox.plainText = "0";
	dlg.e_textbox.plainText = "2.0";
}

scriptObj.onFindVoxels = function()
{
	var dlg = this.PluginDialog;

	var dh = dlg.dh_textbox.plainText;
	var E = dlg.e_textbox.plainText;
	var H = dlg.h_textbox.plainText;
	var Z = dlg.z_textbox.plainText;

	bvqx.SetPluginStringParameter("Command", "Calculate");

	bvqx.SetPluginStringParameter("E", E);
	bvqx.SetPluginStringParameter("H", H);
	bvqx.SetPluginStringParameter("Z", Z);
	bvqx.SetPluginStringParameter("dh", dh);

	bvqx.ExecutePlugin();
}

// It is important to return script object containing properties and functions to be called from BrainVoyager QX
// 
returnScriptObj = function()
{
	return scriptObj;
}
returnScriptObj(); // we call this at the end to return created script object to BrainVoyager QX
