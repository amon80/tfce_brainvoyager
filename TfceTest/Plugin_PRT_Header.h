
#ifndef PLUGIN_PRT_HEADER_H
#define PLUGIN_PRT_HEADER_H

// header for a protocol with global data
//
struct Protocol_Header
{
	int  NrOfConditions;
	char ProtocolFileName[261]; // 261 chars reserved (empty C string ("") if not provided)
	
	char ExperimentName[301];
	int  TimeResolution;  // "1" -> intervals specified in volume resolution, "2" -> intervals specified in msec resolution

	int  PlotBackgroundColor;
	int  PlotTextColor;
	int  PlotTimeCourseColor;
	int  PlotTimeCoursePenThickness;
	int  PlotReferenceTimeCourseColor;
	int  PlotReferenceTimeCoursePenThickness;
};

// header for ONE condition of the protocol
//
struct ProtocolCondition_Header
{
	char ConditionName[256];
	int  NrOfIntervals;
	int  *IntervalFrom;
	int  *IntervalTo;
	int  Color;
};

#endif // PLUGIN_PRT_HEADER_H
