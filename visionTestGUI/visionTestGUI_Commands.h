//------------------------------
//Basic visionTestGUI_Commands definitions
//------------------------------
//#include "visionTestGUI.h"

//Basic Tcl/Tk headers
#include "tcl.h"
#include "tk.h"

#ifndef vistionTestGUI_Commands_DEF
#define vistionTestGUI_Commands_DEF

//==========================================================================
int friendlyRobotFound_cmd(ClientData clientData, 
             	             Tcl_Interp *interp,                     
                           int argc,		
                           char *argv[]);	                         
//==========================================================================
int friendlyPositionStable_cmd(ClientData clientData, 
             	             Tcl_Interp *interp,                     
                           int argc,		
                           char *argv[]);	                         
//==========================================================================
int friendlyRotationStable_cmd(ClientData clientData, 
             	             Tcl_Interp *interp,                     
                           int argc,		
                           char *argv[]);	                         
//==========================================================================
int opponentFound_cmd(ClientData clientData, 
             	             Tcl_Interp *interp,                     
                           int argc,		
                           char *argv[]);	                         
//==========================================================================
int opponentPositionStable_cmd(ClientData clientData, 
             	             Tcl_Interp *interp,                     
                           int argc,		
                           char *argv[]);	                         
//==========================================================================
int ballFound_cmd(ClientData clientData, 
             	             Tcl_Interp *interp,                     
                           int argc,		
                           char *argv[]);	                         
//==========================================================================
int ballPositionStable_cmd(ClientData clientData, 
             	             Tcl_Interp *interp,                     
                           int argc,		
                           char *argv[]);	                         
//==========================================================================
int visionQualityFrame_cmd(ClientData clientData, 
             	             Tcl_Interp *interp,                     
                           int argc,		
                           char *argv[]);	                         
//==========================================================================





#endif //vistionTestGUI_Commands_DEF