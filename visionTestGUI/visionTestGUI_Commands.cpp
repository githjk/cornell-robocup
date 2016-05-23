#include "GUI/GUI.h"
#include "GUI/GUI_Internal_Includes.h"
#include "ai.h"
#include "visionTestGUI.h"


//==========================================================================
int friendlyRobotFound_cmd(ClientData clientData, 
             	             Tcl_Interp *interp,                     
                           int argc,		
                           char *argv[])	                         
{
  int res;
  // if incorrect # of arguments, return error			
  if (argc !=3) //robot # (0-4), frame # (0-19) 
    return ArgError(argv[0], interp);

  res = visionQuality.friendlyRobotFound[atoi(argv[1])][atoi(argv[2])];
  
  sprintf(result, "%d",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
int friendlyPositionStable_cmd(ClientData clientData, 
             	             Tcl_Interp *interp,                     
                           int argc,		
                           char *argv[])	                         
{
  int res;
  // if incorrect # of arguments, return error			
  if (argc !=3) //robot # (0-4), frame # (0-19) 
    return ArgError(argv[0], interp);

  res = visionQuality.friendlyRobotStablePosition[atoi(argv[1])][atoi(argv[2])];
  
  sprintf(result, "%d",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
int friendlyRotationStable_cmd(ClientData clientData, 
             	             Tcl_Interp *interp,                     
                           int argc,		
                           char *argv[])	                         
{
  int res;
  // if incorrect # of arguments, return error			
  if (argc !=3) //robot # (0-4), frame # (0-19) 
    return ArgError(argv[0], interp);

  res = visionQuality.friendlyRobotStableRotation[atoi(argv[1])][atoi(argv[2])];
  
  sprintf(result, "%d",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
int opponentFound_cmd(ClientData clientData, 
             	             Tcl_Interp *interp,                     
                           int argc,		
                           char *argv[])	                         
{
  int res;
  // if incorrect # of arguments, return error			
  if (argc !=3) //robot # (0-4), frame # (0-19) 
    return ArgError(argv[0], interp);

  res = visionQuality.opponentRobotFound[atoi(argv[1])][atoi(argv[2])];
  
  sprintf(result, "%d",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
int opponentPositionStable_cmd(ClientData clientData, 
             	             Tcl_Interp *interp,                     
                           int argc,		
                           char *argv[])	                         
{
  int res;
  // if incorrect # of arguments, return error			
  if (argc !=3) //robot # (0-4), frame # (0-19) 
    return ArgError(argv[0], interp);

  res = visionQuality.opponentRobotStablePosition[atoi(argv[1])][atoi(argv[2])];
  
  sprintf(result, "%d",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
int ballFound_cmd(ClientData clientData, 
             	             Tcl_Interp *interp,                     
                           int argc,		
                           char *argv[])	                         
{
  int res;
  // if incorrect # of arguments, return error			
  if (argc !=2) //frame # (0-19) 
    return ArgError(argv[0], interp);

  res = visionQuality.ballFound[atoi(argv[1])];
  
  sprintf(result, "%d",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
int ballPositionStable_cmd(ClientData clientData, 
             	             Tcl_Interp *interp,                     
                           int argc,		
                           char *argv[])	                         
{
  int res;
  // if incorrect # of arguments, return error			
  if (argc !=2) //frame # (0-19) 
    return ArgError(argv[0], interp);

  res = visionQuality.ballStablePosition[atoi(argv[1])];
  
  sprintf(result, "%d",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
int visionQualityFrame_cmd(ClientData clientData, 
             	             Tcl_Interp *interp,                     
                           int argc,		
                           char *argv[])	                         
{
  int res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = visionQuality.visionQualityFrame;
  
  sprintf(result, "%d",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================