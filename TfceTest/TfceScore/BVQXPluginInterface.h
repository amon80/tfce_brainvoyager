// BVQXPluginInterface.h : general header file for BrainVoyagerQX plugin support
//

#ifndef BVQXPLUGININTERFACE_H
#define BVQXPLUGININTERFACE_H

#include "Plugin_FMR_Header.h"
#include "Plugin_DMR_Header.h"
#include "Plugin_VMR_Header.h"
#include "Plugin_VTC_Header.h"
#include "Plugin_NR_VMP_Header.h"
#include "Plugin_AR_VMP_Header.h"
#include "Plugin_PRT_Header.h"
#include "Plugin_VOI_Header.h"
#include "Plugin_GLM_Header.h"
#include "Plugin_SRF_Header.h"
#include "Plugin_MTC_Header.h"
#include "Plugin_SMP_Header.h"
#include "Plugin_SSM_Header.h"
#include "Plugin_POI_Header.h"

#include "global.h"


// make sure that "MAKE_PLUGIN" is defined here or as a preprocessor definition (this is only important on Windows)
//
#define MAKE_PLUGIN

#if defined OS_WIN32
#ifdef MAKE_PLUGIN
#define PLUGIN_ACCESS __declspec(dllexport)
#else
#define PLUGIN_ACCESS __declspec(dllimport)
#endif
#else
// for all other platforms PLUGIN_ACCESS is empty (not needed)
#define PLUGIN_ACCESS
#endif


#if defined(OS_WIN32)
#include <windows.h>
#include <malloc.h>
#endif
#include <stdio.h>
#include <stdlib.h>

#ifndef OS_WIN32
#include <dlfcn.h>
#endif

#ifdef OS_WIN32
#define GETSYMADDRESS GetProcAddress
#else
#define GETSYMADDRESS dlsym
#endif

class PLUGIN_ACCESS BVQXPluginInterface
{
public:	 
	BVQXPluginInterface() {};
	virtual ~BVQXPluginInterface() {};

	// use this method to execute the plugin (your essential code)
	//
	virtual bool execute() = 0;

	// this method has been introduced with BVQX 2.1 for GUI plugin support: this function will be called in case of GUI plugins once, the execute() method may be called repeatedly from GUI script
	//
	virtual bool initPlugin() = 0;
};


//
// These are typedef's for dynamic access of QX functions - version with "static" definitions
//

// general functions
typedef void (*GetVersionOfBrainVoyagerQXPrototype)(int *, int *, int *);
static GetVersionOfBrainVoyagerQXPrototype qxGetVersionOfBrainVoyagerQX;

typedef void (*GetBrainVoyagerQXPathPrototype)(char *);
static GetBrainVoyagerQXPathPrototype qxGetBrainVoyagerQXPath;

typedef void (*GetPluginsPathPrototype)(char *);
static GetPluginsPathPrototype qxGetPluginsPath;


// input / output
typedef void (*ShowLogPanePrototype)();
static ShowLogPanePrototype qxShowLogPane;

typedef void (*ShowLogTabPrototype)();
static ShowLogTabPrototype qxShowLogTab;

typedef void (*LogTextPrototype)(const char *);
static LogTextPrototype qxLogText;

typedef void (*MessageBoxPrototype)(const char *);
static MessageBoxPrototype qxMessageBox;

typedef int (*YesNoMessageBoxPrototype)(const char *);
static YesNoMessageBoxPrototype qxYesNoMessageBox;

typedef int (*GetIntegerInputPrototype)(const char *, int, int, int);
static GetIntegerInputPrototype qxGetIntegerInput;

typedef float (*GetFloatInputPrototype)(const char *, float, float, float);
static GetFloatInputPrototype qxGetFloatInput;

typedef int (*GetTextInputPrototype)(const char *, const char *, char *);
static GetTextInputPrototype qxGetTextInput;

typedef int (*GetOpenFileNamePrototype)(const char *, const char *, const char *, char *);
static GetOpenFileNamePrototype qxGetOpenFileName;

typedef int (*GetSaveFileNamePrototype)(const char *, const char *, const char *, char *);
static GetSaveFileNamePrototype qxGetSaveFileName;


// GUI
typedef void (*UpdateActiveWindowPrototype)();
static UpdateActiveWindowPrototype qxUpdateActiveWindow;

typedef void (*ShowBusyCursorPrototype)();
static ShowBusyCursorPrototype qxShowBusyCursor;

typedef void (*StopBusyCursorPrototype)();
static StopBusyCursorPrototype qxStopBusyCursor;


// Docs
typedef int (*GetNumberOfDocsInWorkspacePrototype)();
static GetNumberOfDocsInWorkspacePrototype qxGetNumberOfDocsInWorkspace;

typedef int (*GetDocumentTypePrototype)(int);
static GetDocumentTypePrototype qxGetDocumentType;

typedef char * (*GetDocumentTypeNamePrototype)(int);
static GetDocumentTypeNamePrototype qxGetDocumentTypeName;

typedef int (*GetIDOfCurrentDocumentPrototype)();
static GetIDOfCurrentDocumentPrototype qxGetIDOfCurrentDocument;

typedef void (*SetAsCurrentDocumentPrototype)(int);
static SetAsCurrentDocumentPrototype qxSetAsCurrentDocument;

typedef int (*CalculateIAMatrixPrototype)(struct FMR_ScannerPosition_Header *, struct VMR_ScannerPosition_Header *, struct VMR_PastTransformations_Header *, float *, float *, float *, float *, float *, float *, float *, float *);
static CalculateIAMatrixPrototype qxCalculateIAMatrix;

typedef int (*GetTRFParametersFromVolumeToolsDlgPrototype)(float *, float *, float *, float *, float *, float *, float *, float *, float *);
static GetTRFParametersFromVolumeToolsDlgPrototype qxGetTRFParametersFromVolumeToolsDlg;


// FMR-STC
typedef int (*GetHeaderOfCurrentFMRPrototype)(struct FMR_Header *);
static GetHeaderOfCurrentFMRPrototype qxGetHeaderOfCurrentFMR;

typedef int (*SetHeaderOfCurrentFMRPrototype)(struct FMR_Header *);
static SetHeaderOfCurrentFMRPrototype qxSetHeaderOfCurrentFMR;

typedef float ** (*GetSTCFloatDataOfCurrentFMRPrototype)();
static GetSTCFloatDataOfCurrentFMRPrototype qxGetSTCFloatDataOfCurrentFMR;   // NEW in BVQX v2.0 - use to get float ptr to whole STC data

typedef short ** (*GetSTCIntDataOfCurrentFMRPrototype)();
static GetSTCIntDataOfCurrentFMRPrototype qxGetSTCIntDataOfCurrentFMR;       // NEW in BVQX v2.0 - use to get short int ptr to whole STC data

typedef short ** (*GetSTCDataOfCurrentFMRPrototype)();
static GetSTCDataOfCurrentFMRPrototype qxGetSTCDataOfCurrentFMR;             // deprecated in BVQX 2.0 -> use new fns for int or float

typedef int (*SaveFMRPrototype)(const char *);
static SaveFMRPrototype qxSaveFMR;

typedef int (*SaveFMRAndSTCPrototype)(const char *, const char *);
static SaveFMRAndSTCPrototype qxSaveFMRAndSTC;

typedef unsigned int (*GetScannerPositionHeaderOfCurrentFMRPrototype)(struct FMR_ScannerPosition_Header *);
static GetScannerPositionHeaderOfCurrentFMRPrototype qxGetScannerPositionHeaderOfCurrentFMR;

typedef unsigned int (*SetScannerPositionHeaderOfCurrentFMRPrototype)(struct FMR_ScannerPosition_Header *);
static SetScannerPositionHeaderOfCurrentFMRPrototype qxSetScannerPositionHeaderOfCurrentFMR;


// DMR-DWI
typedef int (*GetHeaderOfCurrentDMRPrototype)(struct DMR_Header *);
static GetHeaderOfCurrentDMRPrototype qxGetHeaderOfCurrentDMR;

typedef int (*SetHeaderOfCurrentDMRPrototype)(struct DMR_Header *);
static SetHeaderOfCurrentDMRPrototype qxSetHeaderOfCurrentDMR;

typedef float ** (*GetDWIFloatDataOfCurrentDMRPrototype)();
static GetDWIFloatDataOfCurrentDMRPrototype qxGetDWIFloatDataOfCurrentDMR;   // NEW in BVQX v2.0 - use to get float ptr to whole STC data

typedef short ** (*GetDWIIntDataOfCurrentDMRPrototype)();
static GetDWIIntDataOfCurrentDMRPrototype qxGetDWIIntDataOfCurrentDMR;       // NEW in BVQX v2.0 - use to get short int ptr to whole STC data

typedef short ** (*GetDWIDataOfCurrentDMRPrototype)();
static GetDWIDataOfCurrentDMRPrototype qxGetDWIDataOfCurrentDMR;             // deprecated in BVQX 2.0 -> use new fns for int or float

typedef int (*ConvertDWIFormatOfCurrentDMRPrototype)(int, int);
static ConvertDWIFormatOfCurrentDMRPrototype qxConvertDWIFormatOfCurrentDMR; // works for both int and float data type

typedef int (*SaveDMRPrototype)(const char *);
static SaveDMRPrototype qxSaveDMR;

typedef int (*SaveDMRAndDWIPrototype)(const char *, const char *);
static SaveDMRAndDWIPrototype qxSaveDMRAndDWI;


// VMR
typedef unsigned char * (*GetCurrentVMRPrototype)(int *, int *, int *);
static GetCurrentVMRPrototype qxGetCurrentVMR;

typedef unsigned int (*GetMainHeaderOfCurrentVMRPrototype)(struct VMR_Header *);
static GetMainHeaderOfCurrentVMRPrototype qxGetMainHeaderOfCurrentVMR;

typedef unsigned int (*SetMainHeaderOfCurrentVMRPrototype)(struct VMR_Header *);
static SetMainHeaderOfCurrentVMRPrototype qxSetMainHeaderOfCurrentVMR;

typedef unsigned int (*GetScannerPositionHeaderOfCurrentVMRPrototype)(struct VMR_ScannerPosition_Header *);
static GetScannerPositionHeaderOfCurrentVMRPrototype qxGetScannerPositionHeaderOfCurrentVMR;

typedef unsigned int (*SetScannerPositionHeaderOfCurrentVMRPrototype)(struct VMR_ScannerPosition_Header *);
static SetScannerPositionHeaderOfCurrentVMRPrototype qxSetScannerPositionHeaderOfCurrentVMR;

typedef unsigned int (*GetPastTransformationsHeaderOfCurrentVMRPrototype)(struct VMR_PastTransformations_Header *);
static GetPastTransformationsHeaderOfCurrentVMRPrototype qxGetPastTransformationsHeaderOfCurrentVMR;

typedef unsigned int (*SetPastTransformationsHeaderOfCurrentVMRPrototype)(struct VMR_PastTransformations_Header *);
static SetPastTransformationsHeaderOfCurrentVMRPrototype qxSetPastTransformationsHeaderOfCurrentVMR;

typedef unsigned int (*ReframeCurrentVMRPrototype)(int, int, int);
static ReframeCurrentVMRPrototype qxReframeCurrentVMR;

typedef int (*GetCrossPositionOfCurrentVMRPrototype)(int *, int *, int *);
static GetCrossPositionOfCurrentVMRPrototype qxGetCrossPositionOfCurrentVMR;


// VMP - native resolution
typedef float ** (*GetNRVMPsOfCurrentVMRPrototype)(struct NR_VMPs_Header *);
static GetNRVMPsOfCurrentVMRPrototype qxGetNRVMPsOfCurrentVMR;

typedef float * (*GetNRVMPOfCurrentVMRPrototype)(int, struct NR_VMP_Header *);
static GetNRVMPOfCurrentVMRPrototype qxGetNRVMPOfCurrentVMR;

typedef int (*SetNRVMPParametersOfCurrentVMRPrototype)(int, struct NR_VMP_Header *);
static SetNRVMPParametersOfCurrentVMRPrototype qxSetNRVMPParametersOfCurrentVMR;

typedef int (*LoadNRVMPsToCurrentVMRPrototype)(const char *);
static LoadNRVMPsToCurrentVMRPrototype qxLoadNRVMPsToCurrentVMR;

typedef void (*SaveNRVMPsOfCurrentVMRPrototype)(const char *);
static SaveNRVMPsOfCurrentVMRPrototype qxSaveNRVMPsOfCurrentVMR;

typedef int (*CreateNRVMPsForCurrentVMRPrototype)(int, int, int, struct NR_VMPs_Header *);
static CreateNRVMPsForCurrentVMRPrototype qxCreateNRVMPsForCurrentVMR;

typedef void (*DeleteNRVMPsOfCurrentVMRPrototype)();
static DeleteNRVMPsOfCurrentVMRPrototype qxDeleteNRVMPsOfCurrentVMR;

typedef int (*SetNRVMPsParamsDisplayRangePrototype)(struct NR_VMPs_Header *vmps_header);
static SetNRVMPsParamsDisplayRangePrototype qxSetNRVMPsParamsDisplayRange;


// VMP - anatomical resolution
typedef float ** (*GetARVMPsOfCurrentVMRPrototype)(struct AR_VMPs_Header *);
static GetARVMPsOfCurrentVMRPrototype qxGetARVMPsOfCurrentVMR;

typedef float * (*GetARVMPOfCurrentVMRPrototype)(int, struct AR_VMP_Header *);
static GetARVMPOfCurrentVMRPrototype qxGetARVMPOfCurrentVMR;

typedef int (*SetARVMPParametersOfCurrentVMRPrototype)(int, struct AR_VMP_Header *);
static SetARVMPParametersOfCurrentVMRPrototype qxSetARVMPParametersOfCurrentVMR;

typedef int (*LoadARVMPsToCurrentVMRPrototype)(const char *);
static LoadARVMPsToCurrentVMRPrototype qxLoadARVMPsToCurrentVMR;

typedef void (*SaveARVMPsOfCurrentVMRPrototype)(const char *);
static SaveARVMPsOfCurrentVMRPrototype qxSaveARVMPsOfCurrentVMR;

typedef int (*CreateARVMPsForCurrentVMRPrototype)(int, struct AR_VMPs_Header *);
static CreateARVMPsForCurrentVMRPrototype qxCreateARVMPsForCurrentVMR;

typedef void (*DeleteARVMPsOfCurrentVMRPrototype)();
static DeleteARVMPsOfCurrentVMRPrototype qxDeleteARVMPsOfCurrentVMR;


// VTC
typedef int (*LinkVTCToCurrentVMRPrototype)(const char *, int);
static LinkVTCToCurrentVMRPrototype qxLinkVTCToCurrentVMR;

typedef int (*CreateVTCForCurrentVMRPrototype)(int, struct VTC_Header *);
static CreateVTCForCurrentVMRPrototype qxCreateVTCForCurrentVMR;             // if NULL ptr to VTC_Header, float data created as default - fill header (data type) for more control

typedef int (*GetHeaderOfVTCOfCurrentVMRPrototype)(struct VTC_Header *);
static GetHeaderOfVTCOfCurrentVMRPrototype qxGetHeaderOfVTCOfCurrentVMR;     // NEW in BVQX v2.0

typedef int (*SetHeaderOfVTCOfCurrentVMRPrototype)(struct VTC_Header *);
static SetHeaderOfVTCOfCurrentVMRPrototype qxSetHeaderOfVTCOfCurrentVMR;     // NEW in BVQX v2.0

typedef int (*SetVTCParametersOfCurrentVMRPrototype)(struct VTC_Header *);
static SetVTCParametersOfCurrentVMRPrototype qxSetVTCParametersOfCurrentVMR; // deprecated -> use fn above

typedef float * (*GetVTCFloatDataOfCurrentVMRPrototype)();
static GetVTCFloatDataOfCurrentVMRPrototype qxGetVTCFloatDataOfCurrentVMR;   // NEW in BVQX v2.0 - use to get float ptr to whole VTC data

typedef short * (*GetVTCIntDataOfCurrentVMRPrototype)();
static GetVTCIntDataOfCurrentVMRPrototype qxGetVTCIntDataOfCurrentVMR;       // NEW in BVQX v2.0 - use to get short int ptr to whole VTC data

typedef short * (*GetVTCOfCurrentVMRPrototype)(struct VTC_Header *);
static GetVTCOfCurrentVMRPrototype qxGetVTCOfCurrentVMR;                     // deprecated in BVQX 2.0 -> use fns above

typedef void (*SaveVTCOfCurrentVMRPrototype)(const char *);
static SaveVTCOfCurrentVMRPrototype qxSaveVTCOfCurrentVMR;

// Multi-VTC
typedef int (*ClearMultiVTCAnalysisPrototype)();
static ClearMultiVTCAnalysisPrototype qxClearMultiVTCAnalysis;

typedef int (*AddVTCRefForMultiVTCAnalysisPrototype)(const char *);
static AddVTCRefForMultiVTCAnalysisPrototype qxAddVTCRefForMultiVTCAnalysis;

typedef int (*GetVoxelTimeCourseOfVTCPrototype)(int , int , float *);                 // CHANGED in BVQX v2.0 - provide float ptr, float time series always returned, even if VTC uses short int data type
static GetVoxelTimeCourseOfVTCPrototype qxGetVoxelTimeCourseOfVTC;

typedef int (*GetVoxelTimeCourseOfVTCsPrototype)(int , float *);                      // CHANGED in BVQX 2.0 - see above
static GetVoxelTimeCourseOfVTCsPrototype qxGetVoxelTimeCourseOfVTCs;

typedef int (*GetVoxelCoordsTimeCourseOfVTCPrototype)(int , int , int, int, float *); // CHANGED in BVQX 2.0 - see above
static GetVoxelCoordsTimeCourseOfVTCPrototype qxGetVoxelCoordsTimeCourseOfVTC;

typedef int (*GetVoxelCoordsTimeCourseOfVTCsPrototype)(int , int, int, float *);      // CHANGED in BVQX 2.0 - see above
static GetVoxelCoordsTimeCourseOfVTCsPrototype qxGetVoxelCoordsTimeCourseOfVTCs;


// GLM
typedef float * (*GetGLMOfCurrentVMRPrototype)(struct GLM_Header *);
static GetGLMOfCurrentVMRPrototype qxGetGLMOfCurrentVMR;


// MSK
typedef int (*LoadMaskToCurrentVMRPrototype)(const char *);
static LoadMaskToCurrentVMRPrototype qxLoadMaskToCurrentVMR;

typedef unsigned char * (*GetMaskOfCurrentVMRPrototype)();
static GetMaskOfCurrentVMRPrototype qxGetMaskOfCurrentVMR;


// SRF
typedef int (*GetScenePrototype)(struct SCENE_Header *);
static GetScenePrototype qxGetScene;

typedef int (*GetCurrentSRFPrototype)(struct SRF_Header *);
static GetCurrentSRFPrototype qxGetCurrentSRF;

typedef void (*ReleaseSRFAccessPrototype)(struct SRF_Header *);
static ReleaseSRFAccessPrototype qxReleaseSRFAccess;

typedef void (*SaveCurrentSRFPrototype)(const char *);
static SaveCurrentSRFPrototype qxSaveCurrentSRF;

typedef int (*SetCurrentSRFPrototype)(int);
static SetCurrentSRFPrototype qxSetCurrentSRF;


// SMP
typedef int (*GetSMPsOfCurrentSRFPrototype)(struct SMPs_Header *);
static GetSMPsOfCurrentSRFPrototype qxGetSMPsOfCurrentSRF;

typedef float * (*GetSMPOfCurrentSRFPrototype)(int, struct SMP_Header *);
static GetSMPOfCurrentSRFPrototype qxGetSMPOfCurrentSRF;

typedef int (*SetSMPParametersOfCurrentSRFPrototype)(int, struct SMP_Header *);
static SetSMPParametersOfCurrentSRFPrototype qxSetSMPParametersOfCurrentSRF;

typedef int (*LoadSMPsToCurrentSRFPrototype)(const char *);
static LoadSMPsToCurrentSRFPrototype qxLoadSMPsToCurrentSRF;

typedef void (*SaveSMPsOfCurrentSRFPrototype)(const char *);
static SaveSMPsOfCurrentSRFPrototype qxSaveSMPsOfCurrentSRF;

typedef int (*CreateSMPForCurrentSRFPrototype)();
static CreateSMPForCurrentSRFPrototype qxCreateSMPForCurrentSRF;

typedef void (*DeleteSMPsOfCurrentSRFPrototype)();
static DeleteSMPsOfCurrentSRFPrototype qxDeleteSMPsOfCurrentSRF;

typedef void (*OverlaySMPsOnSRFPrototype)();
static OverlaySMPsOnSRFPrototype qxOverlaySMPsOnSRF;


// MTC
typedef int (*LinkMTCToCurrentSRFPrototype)(const char *, int);
static LinkMTCToCurrentSRFPrototype qxLinkMTCToCurrentSRF;

typedef int (*CreateMTCForCurrentSRFPrototype)(int);
static CreateMTCForCurrentSRFPrototype qxCreateMTCForCurrentSRF;

typedef float ** (*GetMTCOfCurrentSRFPrototype)(struct MTC_Header *);
static GetMTCOfCurrentSRFPrototype qxGetMTCOfCurrentSRF;

typedef int (*SetMTCParametersOfCurrentSRFPrototype)(struct MTC_Header *);
static SetMTCParametersOfCurrentSRFPrototype qxSetMTCParametersOfCurrentSRF;

typedef void (*SaveMTCOfCurrentSRFPrototype)(const char *);
static SaveMTCOfCurrentSRFPrototype qxSaveMTCOfCurrentSRF;

// Multi-MTC (file-based read access)
typedef int (*ClearMultiMTCRefsOfCurrentSRFPrototype)();
static ClearMultiMTCRefsOfCurrentSRFPrototype qxClearMultiMTCRefsOfCurrentSRF;

typedef int (*AddMultiMTCRefToCurrentSRFPrototype)(const char *);
static AddMultiMTCRefToCurrentSRFPrototype qxAddMultiMTCRefToCurrentSRF;

typedef int (*GetVertexTimeCourseOfMTCPrototype)(int , int , float *);
static GetVertexTimeCourseOfMTCPrototype qxGetVertexTimeCourseOfMTC;

typedef int (*GetVertexTimeCourseOfAllMTCsPrototype)(int , float *);
static GetVertexTimeCourseOfAllMTCsPrototype qxGetVertexTimeCourseOfAllMTCs;


// SSM
typedef int * (*GetSSMOfCurrentSRFPrototype)(struct SSM_Header *);
static GetSSMOfCurrentSRFPrototype qxGetSSMOfCurrentSRF;

typedef int (*LoadSSMToCurrentSRFPrototype)(const char *);
static LoadSSMToCurrentSRFPrototype qxLoadSSMToCurrentSRF;

typedef void (*SaveSSMOfCurrentSRFPrototype)(const char *);
static SaveSSMOfCurrentSRFPrototype qxSaveSSMOfCurrentSRF;

typedef int (*CreateSSMForCurrentSRFPrototype)();
static CreateSSMForCurrentSRFPrototype qxCreateSSMForCurrentSRF;

typedef void (*DeleteSSMOfCurrentSRFPrototype)();
static DeleteSSMOfCurrentSRFPrototype qxDeleteSSMOfCurrentSRF;


// PRT
typedef void (*ClearProtocolPrototype)();
static ClearProtocolPrototype qxClearProtocol;

typedef int (*LoadProtocolPrototype)(const char *);
static LoadProtocolPrototype qxLoadProtocol;

typedef int (*SaveProtocolPrototype)(const char *);
static SaveProtocolPrototype qxSaveProtocol;

typedef int (*GetProtocolPrototype)(struct Protocol_Header *);
static GetProtocolPrototype qxGetProtocol;

typedef int (*SetProtocolPrototype)(struct Protocol_Header *);
static SetProtocolPrototype qxSetProtocol;

typedef int (*GetProtocolConditionPrototype)(int, struct ProtocolCondition_Header *);
static GetProtocolConditionPrototype qxGetProtocolCondition;

typedef int (*AddProtocolConditionPrototype)(struct ProtocolCondition_Header *);
static AddProtocolConditionPrototype qxAddProtocolCondition;

typedef int (*AddProtocolConditionIntervalPrototype)(int, int, int);
static AddProtocolConditionIntervalPrototype qxAddProtocolConditionInterval;


// PRED, DM, HRF
typedef int (*ClearDesignMatrixPrototype)();
static ClearDesignMatrixPrototype qxClearDesignMatrix;

typedef int (*AddPredictorPrototype)(const char *);
static AddPredictorPrototype qxAddPredictor;

typedef int (*SetPredictorValuesFromConditionPrototype)(const char *, int, float);
static SetPredictorValuesFromConditionPrototype qxSetPredictorValuesFromCondition;

typedef int (*SetPredictorValuesFromConditionNamePrototype)(const char *, const char *, float);
static SetPredictorValuesFromConditionNamePrototype qxSetPredictorValuesFromConditionName;

typedef int (*ApplyTwoGammaHRFToPredictorPrototype)(int);
static ApplyTwoGammaHRFToPredictorPrototype qxApplyTwoGammaHRFToPredictor;

typedef int (*ApplyTwoGammaHRFToPredictorWithNamePrototype)(const char *);
static ApplyTwoGammaHRFToPredictorWithNamePrototype qxApplyTwoGammaHRFToPredictorWithName;

typedef int (*ApplyBoyntonHRFToPredictorPrototype)(int);
static ApplyBoyntonHRFToPredictorPrototype qxApplyBoyntonHRFToPredictor;

typedef int (*ApplyBoyntonHRFToPredictorWithNamePrototype)(const char *);
static ApplyBoyntonHRFToPredictorWithNamePrototype qxApplyBoyntonHRFToPredictorWithName;

typedef float * (*GetPredictorPrototype)(int);
static GetPredictorPrototype qxGetPredictor;

typedef float * (*GetPredictorByNamePrototype)(const char *);
static GetPredictorByNamePrototype qxGetPredictorByName;

typedef int (*LoadSingleStudyGLMDesignMatrixPrototype)(const char *);
static LoadSingleStudyGLMDesignMatrixPrototype qxLoadSingleStudyGLMDesignMatrix;

typedef int (*SaveSingleStudyGLMDesignMatrixPrototype)(const char *);
static SaveSingleStudyGLMDesignMatrixPrototype qxSaveSingleStudyGLMDesignMatrix;


// VOI
typedef int (*LoadVOIsPrototype)(const char *);
static LoadVOIsPrototype qxLoadVOIs;

typedef int (*SaveVOIsPrototype)(const char *);
static SaveVOIsPrototype qxSaveVOIs;

typedef int (*GetVOIsPrototype)(struct VOIs_Header *);
static GetVOIsPrototype qxGetVOIs;

typedef int * (*GetVOIPrototype)(int, struct VOI_Header *);
static GetVOIPrototype qxGetVOI;


// POI
typedef int (*LoadPOIsPrototype)(const char *);
static LoadPOIsPrototype qxLoadPOIs;

typedef int (*SavePOIsPrototype)(const char *);
static SavePOIsPrototype qxSavePOIs;

typedef int (*GetPOIsPrototype)(struct POIs_Header *);
static GetPOIsPrototype qxGetPOIs;

typedef int * (*GetPOIPrototype)(int, struct POI_Header *);
static GetPOIPrototype qxGetPOI;


// Access to (get, set) parameters for exchange with GUI scripts
typedef int (*GetStringParameterPrototype)(char *, char *);
static GetStringParameterPrototype qxGetStringParameter;

typedef int (*GetIntParameterPrototype)(char *);
static GetIntParameterPrototype qxGetIntParameter;

typedef float (*GetFloatParameterPrototype)(char *);
static GetFloatParameterPrototype qxGetFloatParameter;

typedef void (*SetStringParameterPrototype)(char *, char *);
static SetStringParameterPrototype qxSetStringParameter;

typedef void (*SetIntParameterPrototype)(char *, int);
static SetIntParameterPrototype qxSetIntParameter;

typedef void (*SetFloatParameterPrototype)(char *, float);
static SetFloatParameterPrototype qxSetFloatParameter;


// BVQX dialog access (used only internally, for GUI plugins, use new functions above) - deprecated, will be removed in future plugin version
typedef int (*GetDlgStringParameterPrototype)(int, const char *);
static GetDlgStringParameterPrototype qxGetDlgStringParameter;

typedef int (*GetDlgIntegerParameterPrototype)(int);
static GetDlgIntegerParameterPrototype qxGetDlgIntegerParameter;

typedef float (*GetDlgFloatParameterPrototype)(int);
static GetDlgFloatParameterPrototype qxGetDlgFloatParameter;

typedef void (*SetDlgStringOutputParameterPrototype)(int, const char *);
static SetDlgStringOutputParameterPrototype qxSetDlgStringOutputParameter;

typedef void (*SetDlgIntegerOutputParameterPrototype)(int, int);
static SetDlgIntegerOutputParameterPrototype qxSetDlgIntegerOutputParameter;

typedef void (*SetDlgFloatOutputParameterPrototype)(int, float);
static SetDlgFloatOutputParameterPrototype qxSetDlgFloatOutputParameter;

typedef void (*LaunchICADialogPrototype)();
static LaunchICADialogPrototype qxLaunchICADialog;


// Locally defined functions
//
bool InitQXAccess()
{
#ifdef OS_WIN32

	HMODULE hndl = GetModuleHandleA("BrainVoyagerQX.exe");

	if(hndl == 0)
		return false;

#else

	// handle Mac and Linux
	
	void *hndl = dlopen(NULL, RTLD_LAZY);  // NULL pointer to get handle to main process
	if(hndl == NULL)
		return false;

#endif

	//
	// general functions
	//
	qxGetVersionOfBrainVoyagerQX = (GetVersionOfBrainVoyagerQXPrototype)GETSYMADDRESS( hndl, "qxGetVersionOfBrainVoyagerQX" );
	if(qxGetVersionOfBrainVoyagerQX == NULL) return false;

	// IMPORTANT: If we do not get a pointer, we do not return false
	//  To use specific commands, a) you must check the obtained version string and use only those commands available in that version
	//                         or b) Check whether the function pointers are non-NULL for each command you want to use in your code

	qxGetBrainVoyagerQXPath = (GetBrainVoyagerQXPathPrototype)GETSYMADDRESS( hndl, "qxGetBrainVoyagerQXPath" );  // if(qxGetBrainVoyagerQXPath == NULL) return false;
	qxGetPluginsPath = (GetPluginsPathPrototype)GETSYMADDRESS( hndl, "qxGetPluginsPath" );

	//
	// input and display functions
	//
	qxShowLogPane = (ShowLogPanePrototype)GETSYMADDRESS( hndl, "qxShowLogPane" );  // if(qxShowLogPane == NULL) return false;
	qxShowLogTab = (ShowLogTabPrototype)GETSYMADDRESS( hndl, "qxShowLogTab" );
	qxLogText = (LogTextPrototype)GETSYMADDRESS( hndl, "qxLogText" );
	qxMessageBox = (MessageBoxPrototype)GETSYMADDRESS( hndl, "qxMessageBox" );
	qxYesNoMessageBox = (YesNoMessageBoxPrototype)GETSYMADDRESS( hndl, "qxYesNoMessageBox" );
	qxGetIntegerInput = (GetIntegerInputPrototype)GETSYMADDRESS( hndl, "qxGetIntegerInput" );
	qxGetFloatInput = (GetFloatInputPrototype)GETSYMADDRESS( hndl, "qxGetFloatInput" );
	qxGetTextInput = (GetTextInputPrototype)GETSYMADDRESS( hndl, "qxGetTextInput" );
	qxGetOpenFileName = (GetOpenFileNamePrototype)GETSYMADDRESS( hndl, "qxGetOpenFileName" );
	qxGetSaveFileName = (GetSaveFileNamePrototype)GETSYMADDRESS( hndl, "qxGetSaveFileName" );
	qxUpdateActiveWindow = (UpdateActiveWindowPrototype)GETSYMADDRESS( hndl, "qxUpdateActiveWindow" );
	qxShowBusyCursor = (ShowBusyCursorPrototype)GETSYMADDRESS( hndl, "qxShowBusyCursor" );
	qxStopBusyCursor = (StopBusyCursorPrototype)GETSYMADDRESS( hndl, "qxStopBusyCursor" );

	// Docs
	qxGetNumberOfDocsInWorkspace = (GetNumberOfDocsInWorkspacePrototype)GETSYMADDRESS( hndl, "qxGetNumberOfDocsInWorkspace" );
	qxGetDocumentType = (GetDocumentTypePrototype)GETSYMADDRESS( hndl, "qxGetDocumentType" );
	qxGetDocumentTypeName = (GetDocumentTypeNamePrototype)GETSYMADDRESS( hndl, "qxGetDocumentTypeName" );
	qxGetIDOfCurrentDocument = (GetIDOfCurrentDocumentPrototype)GETSYMADDRESS( hndl, "qxGetIDOfCurrentDocument" );
	qxSetAsCurrentDocument = (SetAsCurrentDocumentPrototype)GETSYMADDRESS( hndl, "qxSetAsCurrentDocument" );
	qxCalculateIAMatrix = (CalculateIAMatrixPrototype)GETSYMADDRESS( hndl, "qxCalculateIAMatrix" );
	qxGetTRFParametersFromVolumeToolsDlg = (GetTRFParametersFromVolumeToolsDlgPrototype)GETSYMADDRESS( hndl, "qxGetTRFParametersFromVolumeToolsDlg" );

	// FMR
	qxGetHeaderOfCurrentFMR = (GetHeaderOfCurrentFMRPrototype)GETSYMADDRESS( hndl, "qxGetHeaderOfCurrentFMR" );
	qxSetHeaderOfCurrentFMR = (SetHeaderOfCurrentFMRPrototype)GETSYMADDRESS( hndl, "qxSetHeaderOfCurrentFMR" );
	qxGetSTCFloatDataOfCurrentFMR = (GetSTCFloatDataOfCurrentFMRPrototype)GETSYMADDRESS( hndl, "qxGetSTCFloatDataOfCurrentFMR" );
	qxGetSTCIntDataOfCurrentFMR = (GetSTCIntDataOfCurrentFMRPrototype)GETSYMADDRESS( hndl, "qxGetSTCIntDataOfCurrentFMR" );
	qxGetSTCDataOfCurrentFMR = (GetSTCDataOfCurrentFMRPrototype)GETSYMADDRESS( hndl, "qxGetSTCDataOfCurrentFMR" );
	qxSaveFMR = (SaveFMRPrototype)GETSYMADDRESS( hndl, "qxSaveFMR" );
	qxSaveFMRAndSTC = (SaveFMRAndSTCPrototype)GETSYMADDRESS( hndl, "qxSaveFMRAndSTC" );
	qxGetScannerPositionHeaderOfCurrentFMR = (GetScannerPositionHeaderOfCurrentFMRPrototype)GETSYMADDRESS( hndl, "qxGetScannerPositionHeaderOfCurrentFMR" );
	qxSetScannerPositionHeaderOfCurrentFMR = (SetScannerPositionHeaderOfCurrentFMRPrototype)GETSYMADDRESS( hndl, "qxSetScannerPositionHeaderOfCurrentFMR" );

	// DMR
	qxGetHeaderOfCurrentDMR = (GetHeaderOfCurrentDMRPrototype)GETSYMADDRESS( hndl, "qxGetHeaderOfCurrentDMR" );
	qxSetHeaderOfCurrentDMR = (SetHeaderOfCurrentDMRPrototype)GETSYMADDRESS( hndl, "qxSetHeaderOfCurrentDMR" );
	qxGetDWIFloatDataOfCurrentDMR = (GetDWIFloatDataOfCurrentDMRPrototype)GETSYMADDRESS( hndl, "qxGetDWIFloatDataOfCurrentDMR" );
	qxGetDWIIntDataOfCurrentDMR = (GetDWIIntDataOfCurrentDMRPrototype)GETSYMADDRESS( hndl, "qxGetDWIIntDataOfCurrentDMR" );
	qxGetDWIDataOfCurrentDMR = (GetDWIDataOfCurrentDMRPrototype)GETSYMADDRESS( hndl, "qxGetDWIDataOfCurrentDMR" );
	qxConvertDWIFormatOfCurrentDMR = (ConvertDWIFormatOfCurrentDMRPrototype)GETSYMADDRESS( hndl, "qxConvertDWIFormatOfCurrentDMR" );
	qxSaveDMR = (SaveDMRPrototype)GETSYMADDRESS( hndl, "qxSaveDMR" );
	qxSaveDMRAndDWI = (SaveDMRAndDWIPrototype)GETSYMADDRESS( hndl, "qxSaveDMRAndDWI" );

	// VMR
	qxGetCurrentVMR = (GetCurrentVMRPrototype)GETSYMADDRESS( hndl, "qxGetCurrentVMR" );
	qxGetMainHeaderOfCurrentVMR = (GetMainHeaderOfCurrentVMRPrototype)GETSYMADDRESS( hndl, "qxGetMainHeaderOfCurrentVMR" );
	qxSetMainHeaderOfCurrentVMR = (SetMainHeaderOfCurrentVMRPrototype)GETSYMADDRESS( hndl, "qxSetMainHeaderOfCurrentVMR" );
	qxGetScannerPositionHeaderOfCurrentVMR = (GetScannerPositionHeaderOfCurrentVMRPrototype)GETSYMADDRESS( hndl, "qxGetScannerPositionHeaderOfCurrentVMR" );
	qxSetScannerPositionHeaderOfCurrentVMR = (SetScannerPositionHeaderOfCurrentVMRPrototype)GETSYMADDRESS( hndl, "qxSetScannerPositionHeaderOfCurrentVMR" );
	qxGetPastTransformationsHeaderOfCurrentVMR = (GetPastTransformationsHeaderOfCurrentVMRPrototype)GETSYMADDRESS( hndl, "qxGetPastTransformationsHeaderOfCurrentVMR" );
	qxSetPastTransformationsHeaderOfCurrentVMR = (SetPastTransformationsHeaderOfCurrentVMRPrototype)GETSYMADDRESS( hndl, "qxSetPastTransformationsHeaderOfCurrentVMR" );
	qxReframeCurrentVMR = (ReframeCurrentVMRPrototype)GETSYMADDRESS( hndl, "qxReframeCurrentVMR" );
	qxGetCrossPositionOfCurrentVMR = (GetCrossPositionOfCurrentVMRPrototype)GETSYMADDRESS( hndl, "qxGetCrossPositionOfCurrentVMR" );

	// nrVMP
	qxGetNRVMPsOfCurrentVMR = (GetNRVMPsOfCurrentVMRPrototype)GETSYMADDRESS( hndl, "qxGetNRVMPsOfCurrentVMR" );
	qxGetNRVMPOfCurrentVMR = (GetNRVMPOfCurrentVMRPrototype)GETSYMADDRESS( hndl, "qxGetNRVMPOfCurrentVMR" );
	qxSetNRVMPParametersOfCurrentVMR = (SetNRVMPParametersOfCurrentVMRPrototype)GETSYMADDRESS( hndl, "qxSetNRVMPParametersOfCurrentVMR" );
	qxLoadNRVMPsToCurrentVMR = (LoadNRVMPsToCurrentVMRPrototype)GETSYMADDRESS( hndl, "qxLoadNRVMPsToCurrentVMR" );
	qxSaveNRVMPsOfCurrentVMR = (SaveNRVMPsOfCurrentVMRPrototype)GETSYMADDRESS( hndl, "qxSaveNRVMPsOfCurrentVMR" );
	qxCreateNRVMPsForCurrentVMR = (CreateNRVMPsForCurrentVMRPrototype)GETSYMADDRESS( hndl, "qxCreateNRVMPsForCurrentVMR" );
	qxDeleteNRVMPsOfCurrentVMR = (DeleteNRVMPsOfCurrentVMRPrototype)GETSYMADDRESS( hndl, "qxDeleteNRVMPsOfCurrentVMR" );
	qxSetNRVMPsParamsDisplayRange = (SetNRVMPsParamsDisplayRangePrototype)GETSYMADDRESS( hndl, "qxSetNRVMPsParamsDisplayRange" );

	// arVMP
	qxGetARVMPsOfCurrentVMR = (GetARVMPsOfCurrentVMRPrototype)GETSYMADDRESS( hndl, "qxGetARVMPsOfCurrentVMR" );
	qxGetARVMPOfCurrentVMR = (GetARVMPOfCurrentVMRPrototype)GETSYMADDRESS( hndl, "qxGetARVMPOfCurrentVMR" );
	qxSetARVMPParametersOfCurrentVMR = (SetARVMPParametersOfCurrentVMRPrototype)GETSYMADDRESS( hndl, "qxSetARVMPParametersOfCurrentVMR" );
	qxLoadARVMPsToCurrentVMR = (LoadARVMPsToCurrentVMRPrototype)GETSYMADDRESS( hndl, "qxLoadARVMPsToCurrentVMR" );
	qxSaveARVMPsOfCurrentVMR = (SaveARVMPsOfCurrentVMRPrototype)GETSYMADDRESS( hndl, "qxSaveARVMPsOfCurrentVMR" );
	qxCreateARVMPsForCurrentVMR = (CreateARVMPsForCurrentVMRPrototype)GETSYMADDRESS( hndl, "qxCreateARVMPsForCurrentVMR" );
	qxDeleteARVMPsOfCurrentVMR = (DeleteARVMPsOfCurrentVMRPrototype)GETSYMADDRESS( hndl, "qxDeleteARVMPsOfCurrentVMR" );

	// VTC
	qxLinkVTCToCurrentVMR = (LinkVTCToCurrentVMRPrototype)GETSYMADDRESS( hndl, "qxLinkVTCToCurrentVMR" );
	qxCreateVTCForCurrentVMR = (CreateVTCForCurrentVMRPrototype)GETSYMADDRESS( hndl, "qxCreateVTCForCurrentVMR" );
	qxGetHeaderOfVTCOfCurrentVMR = (GetHeaderOfVTCOfCurrentVMRPrototype)GETSYMADDRESS( hndl, "qxGetHeaderOfVTCOfCurrentVMR" );
	qxSetHeaderOfVTCOfCurrentVMR = (SetHeaderOfVTCOfCurrentVMRPrototype)GETSYMADDRESS( hndl, "qxSetHeaderOfVTCOfCurrentVMR" );
	qxGetVTCOfCurrentVMR = (GetVTCOfCurrentVMRPrototype)GETSYMADDRESS( hndl, "qxGetVTCOfCurrentVMR" );
	qxGetVTCFloatDataOfCurrentVMR = (GetVTCFloatDataOfCurrentVMRPrototype)GETSYMADDRESS( hndl, "qxGetVTCFloatDataOfCurrentVMR" );
	qxGetVTCIntDataOfCurrentVMR = (GetVTCIntDataOfCurrentVMRPrototype)GETSYMADDRESS( hndl, "qxGetVTCIntDataOfCurrentVMR" );
	qxSetVTCParametersOfCurrentVMR = (SetVTCParametersOfCurrentVMRPrototype)GETSYMADDRESS( hndl, "qxSetVTCParametersOfCurrentVMR" );
	qxSaveVTCOfCurrentVMR = (SaveVTCOfCurrentVMRPrototype)GETSYMADDRESS( hndl, "qxSaveVTCOfCurrentVMR" );

	// Multi-VTC
	qxClearMultiVTCAnalysis = (ClearMultiVTCAnalysisPrototype)GETSYMADDRESS( hndl, "qxClearMultiVTCAnalysis" );
	qxAddVTCRefForMultiVTCAnalysis = (AddVTCRefForMultiVTCAnalysisPrototype)GETSYMADDRESS( hndl, "qxAddVTCRefForMultiVTCAnalysis" );
	qxGetVoxelTimeCourseOfVTC = (GetVoxelTimeCourseOfVTCPrototype)GETSYMADDRESS( hndl, "qxGetVoxelTimeCourseOfVTC" );
	qxGetVoxelTimeCourseOfVTCs = (GetVoxelTimeCourseOfVTCsPrototype)GETSYMADDRESS( hndl, "qxGetVoxelTimeCourseOfVTCs" );
	qxGetVoxelCoordsTimeCourseOfVTC = (GetVoxelCoordsTimeCourseOfVTCPrototype)GETSYMADDRESS( hndl, "qxGetVoxelCoordsTimeCourseOfVTC" );
	qxGetVoxelCoordsTimeCourseOfVTCs = (GetVoxelCoordsTimeCourseOfVTCsPrototype)GETSYMADDRESS( hndl, "qxGetVoxelCoordsTimeCourseOfVTCs" );

	// GLM
	qxGetGLMOfCurrentVMR = (GetGLMOfCurrentVMRPrototype)GETSYMADDRESS( hndl, "qxGetGLMOfCurrentVMR" );

	// MASK
	qxLoadMaskToCurrentVMR = (LoadMaskToCurrentVMRPrototype)GETSYMADDRESS( hndl, "qxLoadMaskToCurrentVMR" );
	qxGetMaskOfCurrentVMR = (GetMaskOfCurrentVMRPrototype)GETSYMADDRESS( hndl, "qxGetMaskOfCurrentVMR" );

	// SRF
	qxGetScene = (GetScenePrototype)GETSYMADDRESS( hndl, "qxGetScene" );
	qxGetCurrentSRF = (GetCurrentSRFPrototype)GETSYMADDRESS( hndl, "qxGetCurrentSRF" );
	qxReleaseSRFAccess = (ReleaseSRFAccessPrototype)GETSYMADDRESS( hndl, "qxReleaseSRFAccess" );
	qxSaveCurrentSRF = (SaveCurrentSRFPrototype)GETSYMADDRESS( hndl, "qxSaveCurrentSRF" );
	qxSetCurrentSRF = (SetCurrentSRFPrototype)GETSYMADDRESS( hndl, "qxSetCurrentSRF" );

	// MTC
	qxLinkMTCToCurrentSRF = (LinkMTCToCurrentSRFPrototype)GETSYMADDRESS( hndl, "qxLinkMTCToCurrentSRF" );
	qxGetMTCOfCurrentSRF = (GetMTCOfCurrentSRFPrototype)GETSYMADDRESS( hndl, "qxGetMTCOfCurrentSRF" );
	qxSetMTCParametersOfCurrentSRF = (SetMTCParametersOfCurrentSRFPrototype)GETSYMADDRESS( hndl, "qxSetMTCParametersOfCurrentSRF" );
	qxCreateMTCForCurrentSRF = (CreateMTCForCurrentSRFPrototype)GETSYMADDRESS( hndl, "qxCreateMTCForCurrentSRF" );
	qxSaveMTCOfCurrentSRF = (SaveMTCOfCurrentSRFPrototype)GETSYMADDRESS( hndl, "qxSaveMTCOfCurrentSRF" );

	// Multi-MTC
	qxClearMultiMTCRefsOfCurrentSRF = (ClearMultiMTCRefsOfCurrentSRFPrototype)GETSYMADDRESS( hndl, "qxClearMultiMTCRefsOfCurrentSRF" );
	qxAddMultiMTCRefToCurrentSRF = (AddMultiMTCRefToCurrentSRFPrototype)GETSYMADDRESS( hndl, "qxAddMultiMTCRefToCurrentSRF" );
	qxGetVertexTimeCourseOfMTC = (GetVertexTimeCourseOfMTCPrototype)GETSYMADDRESS( hndl, "qxGetVertexTimeCourseOfMTC" );
	qxGetVertexTimeCourseOfAllMTCs = (GetVertexTimeCourseOfAllMTCsPrototype)GETSYMADDRESS( hndl, "qxGetVertexTimeCourseOfAllMTCs" );

	// SMP
	qxGetSMPsOfCurrentSRF = (GetSMPsOfCurrentSRFPrototype)GETSYMADDRESS( hndl, "qxGetSMPsOfCurrentSRF" );
	qxGetSMPOfCurrentSRF = (GetSMPOfCurrentSRFPrototype)GETSYMADDRESS( hndl, "qxGetSMPOfCurrentSRF" );
	qxSetSMPParametersOfCurrentSRF = (SetSMPParametersOfCurrentSRFPrototype)GETSYMADDRESS( hndl, "qxSetSMPParametersOfCurrentSRF" );
	qxLoadSMPsToCurrentSRF = (LoadSMPsToCurrentSRFPrototype)GETSYMADDRESS( hndl, "qxLoadSMPsToCurrentSRF" );
	qxSaveSMPsOfCurrentSRF = (SaveSMPsOfCurrentSRFPrototype)GETSYMADDRESS( hndl, "qxSaveSMPsOfCurrentSRF" );
	qxCreateSMPForCurrentSRF = (CreateSMPForCurrentSRFPrototype)GETSYMADDRESS( hndl, "qxCreateSMPForCurrentSRF" );
	qxDeleteSMPsOfCurrentSRF = (DeleteSMPsOfCurrentSRFPrototype)GETSYMADDRESS( hndl, "qxDeleteSMPsOfCurrentSRF" );
	qxOverlaySMPsOnSRF = (OverlaySMPsOnSRFPrototype)GETSYMADDRESS( hndl, "qxOverlaySMPsOnSRF" );

	// SSM
	qxGetSSMOfCurrentSRF = (GetSSMOfCurrentSRFPrototype)GETSYMADDRESS( hndl, "qxGetSSMOfCurrentSRF" );
	qxLoadSSMToCurrentSRF = (LoadSSMToCurrentSRFPrototype)GETSYMADDRESS( hndl, "qxLoadSSMToCurrentSRF" );
	qxSaveSSMOfCurrentSRF = (SaveSSMOfCurrentSRFPrototype)GETSYMADDRESS( hndl, "qxSaveSSMOfCurrentSRF" );
	qxCreateSSMForCurrentSRF = (CreateSSMForCurrentSRFPrototype)GETSYMADDRESS( hndl, "qxCreateSSMForCurrentSRF" );
	qxDeleteSSMOfCurrentSRF = (DeleteSSMOfCurrentSRFPrototype)GETSYMADDRESS( hndl, "qxDeleteSSMOfCurrentSRF" );

	// PRT
	qxClearProtocol = (ClearProtocolPrototype)GETSYMADDRESS( hndl, "qxClearProtocol" );
	qxLoadProtocol = (LoadProtocolPrototype)GETSYMADDRESS( hndl, "qxLoadProtocol" );
	qxSaveProtocol = (SaveProtocolPrototype)GETSYMADDRESS( hndl, "qxSaveProtocol" );
	qxGetProtocol = (GetProtocolPrototype)GETSYMADDRESS( hndl, "qxGetProtocol" );
	qxSetProtocol = (SetProtocolPrototype)GETSYMADDRESS( hndl, "qxSetProtocol" );
	qxGetProtocolCondition = (GetProtocolConditionPrototype)GETSYMADDRESS( hndl, "qxGetProtocolCondition" );
	qxAddProtocolCondition = (AddProtocolConditionPrototype)GETSYMADDRESS( hndl, "qxAddProtocolCondition" );
	qxAddProtocolConditionInterval = (AddProtocolConditionIntervalPrototype)GETSYMADDRESS( hndl, "qxAddProtocolConditionInterval" );

	// Predictor and design matrix modelling functions
	qxClearDesignMatrix = (ClearDesignMatrixPrototype)GETSYMADDRESS( hndl, "qxClearDesignMatrix" );
	qxAddPredictor = (AddPredictorPrototype)GETSYMADDRESS( hndl, "qxAddPredictor" );
	qxSetPredictorValuesFromCondition = (SetPredictorValuesFromConditionPrototype)GETSYMADDRESS( hndl, "qxSetPredictorValuesFromCondition" );
	qxSetPredictorValuesFromConditionName = (SetPredictorValuesFromConditionNamePrototype)GETSYMADDRESS( hndl, "qxSetPredictorValuesFromConditionName" );
	qxApplyTwoGammaHRFToPredictor = (ApplyTwoGammaHRFToPredictorPrototype)GETSYMADDRESS( hndl, "qxApplyTwoGammaHRFToPredictor" );
	qxApplyTwoGammaHRFToPredictorWithName = (ApplyTwoGammaHRFToPredictorWithNamePrototype)GETSYMADDRESS( hndl, "qxApplyTwoGammaHRFToPredictorWithName" );
	qxApplyBoyntonHRFToPredictor = (ApplyBoyntonHRFToPredictorPrototype)GETSYMADDRESS( hndl, "qxApplyBoyntonHRFToPredictor" );
	qxApplyBoyntonHRFToPredictorWithName = (ApplyBoyntonHRFToPredictorWithNamePrototype)GETSYMADDRESS( hndl, "qxApplyBoyntonHRFToPredictorWithName" );
	qxGetPredictor = (GetPredictorPrototype)GETSYMADDRESS( hndl, "qxGetPredictor" );
	qxGetPredictorByName = (GetPredictorByNamePrototype)GETSYMADDRESS( hndl, "qxGetPredictorByName" );
	qxLoadSingleStudyGLMDesignMatrix = (LoadSingleStudyGLMDesignMatrixPrototype)GETSYMADDRESS( hndl, "qxLoadSingleStudyGLMDesignMatrix" );
	qxSaveSingleStudyGLMDesignMatrix = (SaveSingleStudyGLMDesignMatrixPrototype)GETSYMADDRESS( hndl, "qxSaveSingleStudyGLMDesignMatrix" );

	// VOI
	qxLoadVOIs = (LoadVOIsPrototype)GETSYMADDRESS( hndl, "qxLoadVOIs" );
	qxSaveVOIs = (SaveVOIsPrototype)GETSYMADDRESS( hndl, "qxSaveVOIs" );
	qxGetVOIs  = (GetVOIsPrototype)GETSYMADDRESS( hndl, "qxGetVOIs" );
	qxGetVOI   = (GetVOIPrototype)GETSYMADDRESS( hndl, "qxGetVOI" );

	// POI
	qxLoadPOIs = (LoadPOIsPrototype)GETSYMADDRESS( hndl, "qxLoadPOIs" );
	qxSavePOIs = (SavePOIsPrototype)GETSYMADDRESS( hndl, "qxSavePOIs" );
	qxGetPOIs = (GetPOIsPrototype)GETSYMADDRESS( hndl, "qxGetPOIs" );
	qxGetPOI  = (GetPOIPrototype)GETSYMADDRESS( hndl, "qxGetPOI" );

	// get/set parameters between GUI plugin and GUI script
	qxGetStringParameter = (GetStringParameterPrototype)GETSYMADDRESS( hndl, "qxGetStringParameter" );
	qxGetIntParameter    = (GetIntParameterPrototype)GETSYMADDRESS( hndl, "qxGetIntParameter" );
	qxGetFloatParameter  = (GetFloatParameterPrototype)GETSYMADDRESS( hndl, "qxGetFloatParameter" );
	qxSetStringParameter = (SetStringParameterPrototype)GETSYMADDRESS( hndl, "qxSetStringParameter" );
	qxSetIntParameter    = (SetIntParameterPrototype)GETSYMADDRESS( hndl, "qxSetIntParameter" );
	qxSetFloatParameter  = (SetFloatParameterPrototype)GETSYMADDRESS( hndl, "qxSetFloatParameter" );

	// get dlg parameters for special plugins with dialog provided by BrainVoyagerQX
	qxGetDlgStringParameter = (GetDlgStringParameterPrototype)GETSYMADDRESS( hndl, "qxGetDlgStringParameter" );
	qxGetDlgIntegerParameter = (GetDlgIntegerParameterPrototype)GETSYMADDRESS( hndl, "qxGetDlgIntegerParameter" );
	qxGetDlgFloatParameter = (GetDlgFloatParameterPrototype)GETSYMADDRESS( hndl, "qxGetDlgFloatParameter" );
	qxSetDlgStringOutputParameter = (SetDlgStringOutputParameterPrototype)GETSYMADDRESS( hndl, "qxSetDlgStringOutputParameter" );
	qxSetDlgIntegerOutputParameter = (SetDlgIntegerOutputParameterPrototype)GETSYMADDRESS( hndl, "qxSetDlgIntegerOutputParameter" );
	qxSetDlgFloatOutputParameter = (SetDlgFloatOutputParameterPrototype)GETSYMADDRESS( hndl, "qxSetDlgFloatOutputParameter" );
	qxLaunchICADialog = (LaunchICADialogPrototype)GETSYMADDRESS( hndl, "qxLaunchICADialog" );

	return true;
}

bool checkVersionGreaterEqualVersion(int MajorA, int MinorA, int SubMinorA, int MajorB, int MinorB, int SubMinorB)
{
	if(MajorA > MajorB)
		return true;

	if(MajorB > MajorA)
		return false;

	// Major is equal, Minor decides

	if(MinorA > MinorB)
		return true;

	if(MinorB > MinorA)
		return false;

	// Minor is equal, SubMinor decides

	if(SubMinorA >= SubMinorB)
		return true;

	// B smaller
	return false;
}


#endif // BVQXPLUGININTERFACE_H
