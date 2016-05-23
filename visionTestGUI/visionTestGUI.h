//---------------------
//Basic visionTestGUI definitions
//---------------------
#ifndef visionTestGUI_DEF
#define visionTestGUI_DEF

//Basic Tcl/Tk headers
#include "tcl.h"
#include "tk.h"

#include "ai.h"

//so I have access to the standard module
#include "main.h"
#include "GUI/GUI.h"

#include "visionTestData.h"
#include "visionTestGUI_Commands.h"

extern dataQuality visionQuality;

int Tcl_AppInit2(Tcl_Interp *interp);

//---------------------------------
//Initialize VisionQualityTest Data
//---------------------------------
void initializeVisionQualityTestData();

//----------------------------------------
//Update vision quality data
//----------------------------------------
void updateVisionQualityData();

#endif //visionTestGUI_DEF

