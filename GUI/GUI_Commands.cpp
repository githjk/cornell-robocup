
//Declare GUI_Commands information before defining it
#include "GUI_Commands.h"
#include "GUI_Internal_Includes.h"
#include "plays/plays.h"
#include "plays/noplay.h"
#include "plays/otherplays/LED_latency_test.h"
#include "plays/formations/penalty_shot_us.h"
#include "common/wireless/WirelessSender.h"
#include <float.h>
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int Default_Color_Scheme_cmd(ClientData clientData, 
	                         Tcl_Interp *interp,               
                             int argc,		                     
                             char *argv[])	                   
{ 
  int res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->GetDefaultColor();

  sprintf(result, "%d",res);
  interp->result = result;
  return TCL_OK;

}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int Get_Complex_Setting_cmd(ClientData clientData, 
	                        Tcl_Interp *interp,               
							int argc,		                     
							char *argv[])	                   
{ 
  int res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->GetComplexSetting();

  sprintf(result, "%d",res);
  interp->result = result;
  return TCL_OK;

}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
//This command is used to toggle the AI status...
int Toggle_AI_cmd(ClientData clientData, 
	                Tcl_Interp *interp,               
                  int argc,		                     
                  char *argv[])	                   
{ 
  int res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  GUI_Record.toggleAI = true;
  res = !sm->AIStatus();

  sprintf(result, "%d",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
//This command is used to set the AI status...
int Set_AI_cmd(ClientData clientData, 
        	     Tcl_Interp *interp,            
               int argc,		                  
               char *argv[])	                
{ 
  int res;
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  //If new value is not equal to old value, toggle
  if(
     (atoi(argv[1]) && !sm->AIStatus()) ||
     (!atoi(argv[1]) && sm->AIStatus())
    )
    GUI_Record.toggleAI = true;  
  res = atoi(argv[1]);
    
  sprintf(result, "%d",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
//This command is used to toggle the VISION status...
int Toggle_VISION_cmd(ClientData clientData, 
                	    Tcl_Interp *interp,                   
                      int argc,		                         
                      char *argv[])	                       
{ 
  int res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  GUI_Record.toggleVision = true;
  res = !sm->VisionStatus();
 
  sprintf(result, "%d",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
//This command is used to set the VISION status...
int Set_VISION_cmd(ClientData clientData, 
	                 Tcl_Interp *interp,                
                   int argc,		                      
                   char *argv[])	                    
{ 
  int res;
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  //If new value is not equal to old value, toggle
  if(
     (atoi(argv[1]) && !sm->VisionStatus()) ||
     (!atoi(argv[1]) && sm->VisionStatus())
    )
    GUI_Record.toggleVision = true;  

  res = atoi(argv[1]);

  sprintf(result, "%d",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
//This command is used to toggle the WIRELESS status...
int Toggle_WIRELESS_cmd(ClientData clientData, 
                 	      Tcl_Interp *interp,                     
                        int argc,		                           
                        char *argv[])	                         
{ 
  int res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  GUI_Record.toggleWireless = true;
  res = !sm->WirelessRunning();

  sprintf(result, "%d",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
//This command is used to set the WIRELESS status...
int Set_WIRELESS_cmd(ClientData clientData, 
	                   Tcl_Interp *interp,                
                     int argc,		                      
                     char *argv[])	                    
{ 
  int res;
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  //If new value is not equal to old value, toggle
  if(
     (atoi(argv[1]) && !sm->WirelessRunning()) ||
     (!atoi(argv[1]) && sm->WirelessRunning()) 
    )     
    GUI_Record.toggleWireless = true;  
  res = atoi(argv[1]);
  
  sprintf(result, "%d",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
//yes, this is the plague, or "lice", or "super mario man", or "ghandi"
int GetRobotName_cmd(ClientData clientData, 
             	       Tcl_Interp *interp,                  
                     int argc,		                        
                     char *argv[])                        
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  strcpy(result, sm->getPositionName(sm->GetRobotPosition((RobotIndex)atoi(argv[1]))));

  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetRobotType_cmd(ClientData clientData, 
	                   Tcl_Interp *interp,                     
                     int argc,		                           
                     char *argv[])                         
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  strcpy(result, sm->getRobotType(atoi(argv[1])));
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int SetRobotType_cmd(ClientData clientData, 
	                   Tcl_Interp *interp,                     
                     int argc,		                           
                     char *argv[])                         
{
  // if incorrect # of arguments, return error			
  if (argc !=3) 
    return ArgError(argv[0], interp);

  sm->setRobotType(atoi(argv[1]), atoi(argv[2]));

  sprintf(result, "set type");
  interp->result = result;
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int setAllType2000_cmd(ClientData clientData, 
	                   Tcl_Interp *interp,                     
                     int argc,		                           
                     char *argv[])                         
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  for(int i=0; i<NUM_ROBOTS; i++)
  {
    sm->setRobotType(i, 2000);
  }
  sprintf(result, "set type");
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int setAllType2001_cmd(ClientData clientData, 
	                   Tcl_Interp *interp,                     
                     int argc,		                           
                     char *argv[])                         
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  for(int i=0; i<NUM_ROBOTS; i++)
  {
    sm->setRobotType(i, 2001);
  }


  sprintf(result, "set type");
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int setAllType2002_cmd(ClientData clientData, 
	                   Tcl_Interp *interp,                     
                     int argc,		                           
                     char *argv[])                         
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  for(int i=0; i<NUM_ROBOTS; i++)
  {
    sm->setRobotType(i, 2002);
  }
  sprintf(result, "set type");
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetRobotPosition_cmd(ClientData clientData, 
	                       Tcl_Interp *interp,                     
                         int argc,		                           
                         char *argv[])                         
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  strcpy(result, sm->getPositionText(sm->GetRobotPosition((RobotIndex)atoi(argv[1]))));
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetRobotMessage_cmd(ClientData clientData, 
	                      Tcl_Interp *interp,                     
                        int argc,		                           
                        char *argv[])                         
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  sprintf(result, "%s",sm->GetRobotMessageBuffer((RobotIndex)atoi(argv[1])));
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetSystemStatus_cmd(ClientData clientData, 
	                      Tcl_Interp *interp,                     
                        int argc,		                           
                        char *argv[])                         
{
  char* res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->GetSystemStatus();
  
  sprintf(result, "%s",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetOurRobotX_cmd(ClientData clientData, 
	                   Tcl_Interp *interp,                
                     int argc,		                      
                     char *argv[])	                    
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  res = sm->GetOurRobotX(atoi(argv[1]));
  //--------------------------------
  //ensure the value is valid
  if(
       _isnan(res) || //not a number
       !_finite(res)  //infinite
    )
  {
    res = -64000.0f; //this is the error value
  }
  //--------------------------------
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//=========================================================================
//==========================================================================
//=========================================================================
int GetOurRobotY_cmd(ClientData clientData, 
	                   Tcl_Interp *interp,                     
                     int argc,		                           
                     char *argv[])	                         
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  res = sm->GetOurRobotY(atoi(argv[1]));
  //--------------------------------
  //ensure the value is valid
  if(
       _isnan(res) || //not a number
       !_finite(res)  //infinite
    )
  {
    res = -64000.0f; //this is the error value
  }
  //--------------------------------
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetOurRobotXVel_cmd(ClientData clientData, 
	                      Tcl_Interp *interp,                
                        int argc,		                     
                        char *argv[])	                    
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  res = sm->GetOurRobotXVel(atoi(argv[1]));
  //--------------------------------
  //ensure the value is valid
  if(
       _isnan(res) || //not a number
       !_finite(res)  //infinite
    )
  {
    res = -64000.0f; //this is the error value
  }
  //--------------------------------
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetOurRobotYVel_cmd(ClientData clientData, 
	                      Tcl_Interp *interp,                     
                        int argc,		                           
                        char *argv[])	                         
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  res = sm->GetOurRobotYVel(atoi(argv[1]));
  //--------------------------------
  //ensure the value is valid
  if(
       _isnan(res) || //not a number
       !_finite(res)  //infinite
    )
  {
    res = -64000.0f; //this is the error value
  }
  //--------------------------------
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetOurRobotRotation_cmd(ClientData clientData, 
                      	    Tcl_Interp *interp,                
                            int argc,		                      
                            char *argv[])	                    
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  res = sm->GetOurRobotRotation(atoi(argv[1]));
  //--------------------------------
  //ensure the value is valid
  if(
       _isnan(res) || //not a number
       !_finite(res)  //infinite
    )
  {
    res = -64000.0f; //this is the error value
  }
  //--------------------------------
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetOurRobotSentXVel_cmd(ClientData clientData, 
                 	          Tcl_Interp *interp,  
                            int argc,		        
                            char *argv[])	      
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  res = sm->GetOurRobotSentXVel(atoi(argv[1]));
  //--------------------------------
  //ensure the value is valid
  if(
       _isnan(res) || //not a number
       !_finite(res)  //infinite
    )
  {
    res = -64000.0f; //this is the error value
  }
  //--------------------------------
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetOurRobotSentYVel_cmd(ClientData clientData, 
	                          Tcl_Interp *interp,              
                            int argc,		                    
                            char *argv[])	                  
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  res = sm->GetOurRobotSentYVel(atoi(argv[1]));
  //--------------------------------
  //ensure the value is valid
  if(
       _isnan(res) || //not a number
       !_finite(res)  //infinite
    )
  {
    res = -64000.0f; //this is the error value
  }
  //--------------------------------
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetOurRobotSentRotation_cmd(ClientData clientData, 
	                              Tcl_Interp *interp,  
                                int argc,		        
                                char *argv[])	      
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  res = sm->GetOurRobotSentRotation(atoi(argv[1]));
  //--------------------------------
  //ensure the value is valid
  if(
       _isnan(res) || //not a number
       !_finite(res)  //infinite
    )
  {
    res = -64000.0f; //this is the error value
  }
  //--------------------------------
  
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetOurRobotDestinationX_cmd(ClientData clientData, 
	                              Tcl_Interp *interp,                
                                int argc,		                      
                                char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  float res = sm->GetOurRobotDestX(atoi(argv[1]));
  //--------------------------------
  //ensure the value is valid
  if(
       _isnan(res) || //not a number
       !_finite(res)  //infinite
    )
  {
    res = -64000.0f; //this is the error value
  }
  //--------------------------------

  sprintf(result, "%f", res );
      interp->result = result;
  return TCL_OK;
}      
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetOurRobotDestinationY_cmd(ClientData clientData, 
	                              Tcl_Interp *interp,                  
                                int argc,		                        
                                char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  float res = sm->GetOurRobotDestY(atoi(argv[1]));
  //--------------------------------
  //ensure the value is valid
  if(
       _isnan(res) || //not a number
       !_finite(res)  //infinite
    )
  {
    res = -64000.0f; //this is the error value
  }
  //--------------------------------

  sprintf(result, "%f", res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetOurRobotDestinationRot_cmd(ClientData clientData, 
	                              Tcl_Interp *interp,                
                                int argc,		                      
                                char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  float res = sm->GetOurRobotDestRot(atoi(argv[1]));
  //--------------------------------
  //ensure the value is valid
  if(
       _isnan(res) || //not a number
       !_finite(res)  //infinite
    )
  {
    res = -64000.0f; //this is the error value
  }
  //--------------------------------

  sprintf(result, "%f", res );
      interp->result = result;
  return TCL_OK;
}      
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetOurRobotPassValue_cmd(ClientData clientData, 
	                           Tcl_Interp *interp,                
                             int argc,		                      
                             char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  int res = sm->GetRobotPassValue(atoi(argv[1]));

  sprintf(result, "%i", res);

  interp->result = result;
  return TCL_OK;
}      
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetOurRobotPassDestinationX_cmd(ClientData clientData, 
	                                  Tcl_Interp *interp,                
                                    int argc,		                      
                                    char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  float res = sm->GetRobotPassX(atoi(argv[1]));
  //--------------------------------
  //ensure the value is valid
  if(
       _isnan(res) || //not a number
       !_finite(res)  //infinite
    )
  {
    res = -64000.0f; //this is the error value
  }
  //--------------------------------
  sprintf(result, "%f", res);
  interp->result = result;
  return TCL_OK;
}      
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetOurRobotPassDestinationY_cmd(ClientData clientData, 
	                                  Tcl_Interp *interp,                  
                                    int argc,		                        
                                    char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  float res = sm->GetRobotPassY(atoi(argv[1]));
  //--------------------------------
  //ensure the value is valid
  if(
       _isnan(res) || //not a number
       !_finite(res)  //infinite
    )
  {
    res = -64000.0f; //this is the error value
  }
  //--------------------------------
  sprintf(result, "%f", res);
  interp->result = result;
  return TCL_OK;
}            
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetTheirRobotX_cmd(ClientData clientData, 
                	     Tcl_Interp *interp,                    
                       int argc,		                          
                       char *argv[])	                        
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  res = sm->GetTheirRobotX(atoi(argv[1]));
  //--------------------------------
  //ensure the value is valid
  if(
       _isnan(res) || //not a number
       !_finite(res)  //infinite
    )
  {
    res = -64000.0f; //this is the error value
  }
  //--------------------------------
  
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetTheirRobotY_cmd(ClientData clientData, 
	                     Tcl_Interp *interp,                    
                       int argc,		                          
                       char *argv[])	                        
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  res = sm->GetTheirRobotY(atoi(argv[1]));
  //--------------------------------
  //ensure the value is valid
  if(
       _isnan(res) || //not a number
       !_finite(res)  //infinite
    )
  {
    res = -64000.0f; //this is the error value
  }
  //--------------------------------
  
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetTheirRobotXVel_cmd(ClientData clientData, 
	                        Tcl_Interp *interp,                     
                          int argc,		                           
                          char *argv[])	                         
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  res = sm->GetTheirRobotXVel(atoi(argv[1]));
  //--------------------------------
  //ensure the value is valid
  if(
       _isnan(res) || //not a number
       !_finite(res)  //infinite
    )
  {
    res = -64000.0f; //this is the error value
  }
  //--------------------------------
  
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetTheirRobotYVel_cmd(ClientData clientData, 
	                        Tcl_Interp *interp,                     
                          int argc,		                           
                          char *argv[])	                         
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  res = sm->GetTheirRobotYVel(atoi(argv[1]));
  //--------------------------------
  //ensure the value is valid
  if(
       _isnan(res) || //not a number
       !_finite(res)  //infinite
    )
  {
    res = -64000.0f; //this is the error value
  }
  //--------------------------------
  
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetBallX_cmd(ClientData clientData, 
	               Tcl_Interp *interp,               
                 int argc,		                          
                 char *argv[])	                        
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->GetBallX();
  //--------------------------------
  //ensure the value is valid
  if(
       _isnan(res) || //not a number
       !_finite(res)  //infinite
    )
  {
    res = -64000.0f; //this is the error value
  }
  //--------------------------------
  
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetBallY_cmd(ClientData clientData, 
	               Tcl_Interp *interp,              
                 int argc,		                    
                 char *argv[])	                  
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->GetBallY();
  //--------------------------------
  //ensure the value is valid
  if(
       _isnan(res) || //not a number
       !_finite(res)  //infinite
    )
  {
    res = -64000.0f; //this is the error value
  }
  //--------------------------------
  
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetBallRawX_cmd(ClientData clientData, 
	               Tcl_Interp *interp,               
                 int argc,		                          
                 char *argv[])	                        
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->GetBallRawX();
  //--------------------------------
  //ensure the value is valid
  if(
       _isnan(res) || //not a number
       !_finite(res)  //infinite
    )
  {
    res = -64000.0f; //this is the error value
  }
  //--------------------------------
  
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetBallRawY_cmd(ClientData clientData, 
	               Tcl_Interp *interp,              
                 int argc,		                    
                 char *argv[])	                  
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);
  res = sm->GetBallRawY();
  //--------------------------------
  //ensure the value is valid
  if(
       _isnan(res) || //not a number
       !_finite(res)  //infinite
    )
  {
    res = -64000.0f; //this is the error value
  }
  //--------------------------------
  
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetBallXVel_cmd(ClientData clientData, 
	                  Tcl_Interp *interp,                 
                    int argc,		                       
                    char *argv[])	                     
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->GetBallXVel();
  //--------------------------------
  //ensure the value is valid
  if(
       _isnan(res) || //not a number
       !_finite(res)  //infinite
    )
  {
    res = -64000.0f; //this is the error value
  }
  //--------------------------------
  
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetBallYVel_cmd(ClientData clientData, 
	                  Tcl_Interp *interp,                 
                    int argc,		                       
                    char *argv[])	                     
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->GetBallYVel();
  //--------------------------------
  //ensure the value is valid
  if(
       _isnan(res) || //not a number
       !_finite(res)  //infinite
    )
  {
    res = -64000.0f; //this is the error value
  }
  //--------------------------------
  
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetAIStatus_cmd(ClientData clientData, 
	                  Tcl_Interp *interp,                 
                    int argc,		                       
                    char *argv[])	                     
{
  int res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->AIStatus();
  
  sprintf(result, "%d",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetVisionStatus_cmd(ClientData clientData, 
	                      Tcl_Interp *interp,                     
                        int argc,		                           
                        char *argv[])	                         
{
  int res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->VisionStatus();
  
  sprintf(result, "%d",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetWirelessStatus_cmd(ClientData clientData, 
	                        Tcl_Interp *interp,                     
                          int argc,		                           
                          char *argv[])	                         
{
  int res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->WirelessRunning();
  
  sprintf(result, "%d",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetTeamNumber_cmd(ClientData clientData, 
	                    Tcl_Interp *interp,                   
                      int argc,		                         
                      char *argv[])	                       
{
  int res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->CurrentTeam();
  
  sprintf(result, "%d",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetSidesFlippedStatus_cmd(ClientData clientData, 
	                            Tcl_Interp *interp,                     
                              int argc,		                           
                              char *argv[])	                         
{
  int res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->SidesFlipped();
  
  sprintf(result, "%d",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetCurrentFrame_cmd(ClientData clientData, 
                  	    Tcl_Interp *interp,                     
                        int argc,		                           
                        char *argv[])	                         
{
  int res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->ReturnFrame();
  
  sprintf(result, "%d",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetOurRobotFound_cmd(ClientData clientData, 
                  	     Tcl_Interp *interp,                     
                         int argc,		                           
                         char *argv[])	                         
{
  int res;
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  res = sm->OurRobotFound(atoi(argv[1]));
  
  sprintf(result, "%d",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetOurRobotHavePossession_cmd(ClientData clientData, 
                  	              Tcl_Interp *interp,                     
                                  int argc,		                           
                                  char *argv[])	                         
{
  int res;
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  res = sm->GetFriendlyHasPossession(atoi(argv[1]));
  
  sprintf(result, "%d",res);
  interp->result = result;
  return TCL_OK;
}//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetTheirRobotHavePossession_cmd(ClientData clientData, 
                  	              Tcl_Interp *interp,                     
                                  int argc,		                           
                                  char *argv[])	                         
{
  int res;
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  res = sm->GetOpponentHasPossession(atoi(argv[1]));
  
  sprintf(result, "%d",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetTheirRobotFound_cmd(ClientData clientData, 
                    	     Tcl_Interp *interp,                     
                           int argc,		                           
                           char *argv[])	                         
{
  int res;
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  res = sm->TheirRobotFound(atoi(argv[1]));
  
  sprintf(result, "%d",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetRoboCupBallFound_cmd(ClientData clientData,
	                          Tcl_Interp *interp,                     
                            int argc,		                           
                            char *argv[])	                         
{
  int res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->RoboCupBallFound();
  
  sprintf(result, "%d",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetRobotKick_cmd(ClientData clientData, 
	                   Tcl_Interp *interp,                  
                     int argc,		                        
                     char *argv[])	                      
{
  int res;
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  res = sm->RobotKick(atoi(argv[1]));
  
  sprintf(result, "%d",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetRobotDribble_cmd(ClientData clientData, 
	                      Tcl_Interp *interp,                     
                        int argc,		                           
                        char *argv[])	                         
{
  int res;
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  res = sm->RobotDribble(atoi(argv[1]));
  
  sprintf(result, "%d",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int SetPlay_cmd(ClientData clientData, 
	              Tcl_Interp *interp,             
                int argc,		                   
                char *argv[])	                 
{
  int res;
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  GUI_Record.newPlay = atoi(argv[1]); 
  res = sm->getPlay();

  sprintf(result, "%d",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetCurrentPlayName_cmd(ClientData clientData,    
	                         Tcl_Interp *interp,                 
                           int argc,		                       
                           char *argv[])	                     
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  strcpy(result,sm->getPlayName());

  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int CurrentPlayActive_cmd(ClientData clientData,    
	                         Tcl_Interp *interp,                 
                           int argc,		                       
                           char *argv[])	                     
{
  int res;

  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->currentPlayActive();

  sprintf(result, "%d",res);

  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int PlayIsGamePlay_cmd(ClientData clientData,
	                         Tcl_Interp *interp,                  
                           int argc,		                        
                           char *argv[])
{
  int res;

  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->currentPlayActive();

  sprintf(result, "%d",res);

  interp->result = result;
  return TCL_OK;  
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetPlayName_cmd(ClientData clientData, 
	                  Tcl_Interp *interp,                 
                    int argc,		                       
                    char *argv[])	                     
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  strcpy(result,sm->getPlayName((Play)atoi(argv[1])) );

  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetSetCmd_cmd(ClientData clientData, 
	                Tcl_Interp *interp,               
                  int argc,		                     
                  char *argv[])	                   
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  sprintf(result,"SetPlay_cmd %s",argv[1]);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetSetPosition_cmd(ClientData clientData, 
	                     Tcl_Interp *interp,                    
                       int argc,		                          
                       char *argv[])	                       
{
  // if incorrect # of arguments, return error			
  if (argc !=3) 
    return ArgError(argv[0], interp);

  sprintf(result,"SetRobotPosition_cmd %s %s",argv[1],argv[2]);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int Initialize2002Positions_cmd(ClientData clientData, 
	                             Tcl_Interp *interp,                    
                               int argc,		                          
                               char *argv[])	                        
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);
  sm->Initialize2002Positions();
  sprintf(result,"Positions set.");
  interp->result = result;

  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int ResetPositions_cmd(ClientData clientData, 
	                     Tcl_Interp *interp,                    
                       int argc,		                          
                       char *argv[])	                        
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);
  sm->ResetPositions();
  sprintf(result,"Positions reset.");
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int SetRobotPosition_cmd(ClientData clientData, 
                  	     Tcl_Interp *interp,                     
                         int argc,		                           
                         char *argv[])	                         
{
  // if incorrect # of arguments, return error			
  if (argc !=3) 
    return ArgError(argv[0], interp);

  sm->SetRobotPosition(atoi(argv[1]), atoi(argv[2]));
  sm->resetPlay();
  sprintf(result,"Position set.");
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetPositionName_cmd(ClientData clientData,
	                      Tcl_Interp *interp,                    
                        int argc,		                          
                        char *argv[])                        
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  strcpy(result, sm->getPositionText( (Position)atoi(argv[1]) ));
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetNoPosition_cmd(ClientData clientData, 
	                   Tcl_Interp *interp,                     
                       int argc,		                           
                       char *argv[])                         
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sprintf(result, "%d", NO_POSITION);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetNumPositions_cmd(ClientData clientData, 
	                      Tcl_Interp *interp,                     
                        int argc,		                           
                        char *argv[])                         
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sprintf(result, "%d", NUM_POSITIONS);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetNumRobots_cmd(ClientData clientData, 
	                   Tcl_Interp *interp,                  
                     int argc,		                        
                     char *argv[])                        
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sprintf(result, "%d", NUM_ROBOTS);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int ToggleTeamNumber_cmd(ClientData clientData, 
	                       Tcl_Interp *interp,                     
                         int argc,		                          
                         char *argv[])                         
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sm->ToggleTeamNumber();
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int TogglePlayingSide_cmd(ClientData clientData, 
	                        Tcl_Interp *interp,                     
                          int argc,		                           
                          char *argv[])                         
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sm->TogglePlayingSide();
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int ReloadParameters_cmd(ClientData clientData, 
	                       Tcl_Interp *interp,                     
                         int argc,		                          
                         char *argv[])                         
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sm->reloadParameters();
  interp->result = result;

  //display message in gui debug window that params have been reloaded...
  GUI_Record.debuggingInfo.addDebugMessage("Reloaded All Parameters");
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int ReloadTrajectoryParameters_cmd(ClientData clientData, 
	                       Tcl_Interp *interp,                     
                         int argc,		                          
                         char *argv[])                         
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sm->reloadTrajectoryParameters();
  interp->result = result;

  //display message in gui debug window that params have been reloaded...
  GUI_Record.debuggingInfo.addDebugMessage("Reloaded Trajectory Parameters");
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int SaveLog_cmd(ClientData clientData,
	              Tcl_Interp *interp,            
                int argc,		                  
                char *argv[])                  
{

  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  writePlaybackSequence("-", true);
  sm->resetFrame();

  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int SaveLogByName_cmd(ClientData clientData, 
	                    Tcl_Interp *interp,                   
                      int argc,		                         
                      char *argv[])                         
{

  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  writePlaybackSequence(argv[1], false);
  sm->resetFrame();

  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int ShutDown_cmd(ClientData clientData, 
	               Tcl_Interp *interp,              
                 int argc,		                    
                 char *argv[])                    
{

  // if incorrect # of arguments, return error			
  if (argc !=1)
    return ArgError(argv[0], interp);

  sm->shutDown();

  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int RunLatencyTest_cmd(ClientData clientData, 
	                     Tcl_Interp *interp,                    
                       int argc,		                          
                       char *argv[])                         
{
  // if incorrect # of arguments, return error			
  if (argc !=1)
    return ArgError(argv[0], interp);

  //first set old mean to -1
  sm->setMeanLatency(-1.0f);

  //set play to latency test to run the latency test =-)
  sm->setPlay(LED_LatencyTest::playNum);

  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int IsLatencyTestDone_cmd(ClientData clientData, 
                    	    Tcl_Interp *interp,                     
                          int argc,		                           
                          char *argv[])                         
{
  int res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->getLatencyTestDone();
 
  sprintf(result, "%i",res);
  interp->result = result;

  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetMinLatency_cmd(ClientData clientData, 
	                    Tcl_Interp *interp,                   
                      int argc,		                         
                      char *argv[])                         
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->getMinLatency();
  //--------------------------------
  //ensure the value is valid
  if(
       _isnan(res) || //not a number
       !_finite(res)  //infinite
    )
  {
    res = -64000.0f; //this is the error value
  }
  //--------------------------------
 
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;

}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetMaxLatency_cmd(ClientData clientData, 
              	      Tcl_Interp *interp,                   
                      int argc,		                         
                      char *argv[])                         
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->getMaxLatency();
  //--------------------------------
  //ensure the value is valid
  if(
       _isnan(res) || //not a number
       !_finite(res)  //infinite
    )
  {
    res = -64000.0f; //this is the error value
  }
  //--------------------------------
 
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;

}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetMeanLatency_cmd(ClientData clientData, 
	                     Tcl_Interp *interp,                    
                       int argc,		                       
                       char *argv[])                         
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->getMeanLatency();
  //--------------------------------
  //ensure the value is valid
  if(
       _isnan(res) || //not a number
       !_finite(res)  //infinite
    )
  {
    res = -64000.0f; //this is the error value
  }
  //--------------------------------
 
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;

}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetFrameRate_cmd(ClientData clientData,
	                   Tcl_Interp *interp,                 
                     int argc,		                       
                     char *argv[])                       
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->getParameters()->general.FRAME_RATE;
  //--------------------------------
  //ensure the value is valid
  if(
       _isnan(res) || //not a number
       !_finite(res)  //infinite
    )
  {
    res = -64000.0f; //this is the error value
  }
  //--------------------------------

  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;

}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int SampleFrameRate_cmd(ClientData clientData, 
	                      Tcl_Interp *interp,                     
                        int argc,		                           
                        char *argv[])                         
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  GUI_Record.sampleFrameRate = true;
 
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetFirst2002Play_cmd(ClientData clientData, 
	                     Tcl_Interp *interp,                     
                         int argc,		                           
                         char *argv[])	                         
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sprintf(result,"%d",START_2002_PLAYS);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetLast2002Play_cmd(ClientData clientData, 
	                     Tcl_Interp *interp,                     
                         int argc,		                           
                         char *argv[])	                         
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sprintf(result,"%d",END_2002_PLAYS);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetFirst2002PenaltyPlay_cmd(ClientData clientData, 
	                     Tcl_Interp *interp,                     
                         int argc,		                           
                         char *argv[])	                         
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sprintf(result,"%d",START_2002_PENALTY_PLAYS);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetLast2002PenaltyPlay_cmd(ClientData clientData, 
	                     Tcl_Interp *interp,                     
                         int argc,		                           
                         char *argv[])	                         
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sprintf(result,"%d",END_2002_PENALTY_PLAYS);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetFirstFormationPlay_cmd(ClientData clientData, 
	                        Tcl_Interp *interp,                     
                          int argc,		                           
                          char *argv[])	                         
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sprintf(result,"%d",START_FORMATION_PLAYS);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetLastFormationPlay_cmd(ClientData clientData, 
	                        Tcl_Interp *interp,                     
                          int argc,		                           
                          char *argv[])	                         
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sprintf(result,"%d",END_FORMATION_PLAYS);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetFirstEvanKuhnSkillTestPlay_cmd(ClientData clientData, 
	                      Tcl_Interp *interp,                      
                        int argc,		                           
                        char *argv[])	                         
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sprintf(result,"%d",START_EVAN_KUHN_SKILL_TEST_PLAYS);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetLastEvanKuhnSkillTestPlay_cmd(ClientData clientData, 
	                      Tcl_Interp *interp,                      
                        int argc,		                           
                        char *argv[])	                         
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sprintf(result,"%d",END_EVAN_KUHN_SKILL_TEST_PLAYS);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetFirstEvanMaloneSkillTestPlay_cmd(ClientData clientData, 
	                      Tcl_Interp *interp,                      
                        int argc,		                           
                        char *argv[])	                         
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sprintf(result,"%d",START_EVAN_MALONE_SKILL_TEST_PLAYS);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetLastEvanMaloneSkillTestPlay_cmd(ClientData clientData, 
	                      Tcl_Interp *interp,                      
                        int argc,		                           
                        char *argv[])	                         
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sprintf(result,"%d",END_EVAN_MALONE_SKILL_TEST_PLAYS);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetFirstJoelSkillTestPlay_cmd(ClientData clientData, 
	                      Tcl_Interp *interp,                      
                        int argc,		                           
                        char *argv[])	                         
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sprintf(result,"%d",START_JOEL_SKILL_TEST_PLAYS);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetLastJoelSkillTestPlay_cmd(ClientData clientData, 
	                      Tcl_Interp *interp,                      
                        int argc,		                           
                        char *argv[])	                         
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sprintf(result,"%d",END_JOEL_SKILL_TEST_PLAYS);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetFirstNiravSkillTestPlay_cmd(ClientData clientData, 
	                      Tcl_Interp *interp,                      
                        int argc,		                           
                        char *argv[])	                         
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sprintf(result,"%d",START_NIRAV_SKILL_TEST_PLAYS);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetLastNiravSkillTestPlay_cmd(ClientData clientData, 
	                      Tcl_Interp *interp,                      
                        int argc,		                           
                        char *argv[])	                         
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sprintf(result,"%d",END_NIRAV_SKILL_TEST_PLAYS);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetFirstWillSkillTestPlay_cmd(ClientData clientData, 
	                      Tcl_Interp *interp,                      
                        int argc,		                           
                        char *argv[])	                         
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sprintf(result,"%d",START_WILL_SKILL_TEST_PLAYS);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetLastWillSkillTestPlay_cmd(ClientData clientData, 
	                      Tcl_Interp *interp,                      
                        int argc,		                           
                        char *argv[])	                         
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sprintf(result,"%d",END_WILL_SKILL_TEST_PLAYS);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetFirstIndividualPlay_cmd(ClientData clientData, 
	                      Tcl_Interp *interp,                      
                        int argc,		                           
                        char *argv[])	                         
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sprintf(result,"%d",START_INDIVIDUAL_PLAYS);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetLastIndividualPlay_cmd(ClientData clientData, 
	                      Tcl_Interp *interp,                      
                        int argc,		                           
                        char *argv[])	                         
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sprintf(result,"%d",END_INDIVIDUAL_PLAYS);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetFirstIntimidationPlay_cmd(ClientData clientData, 
	                      Tcl_Interp *interp,                      
                        int argc,		                           
                        char *argv[])	                         
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sprintf(result,"%d",START_INTIMIDATION_PLAYS);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetLastIntimidationPlay_cmd(ClientData clientData, 
	                      Tcl_Interp *interp,                      
                        int argc,		                           
                        char *argv[])	                         
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sprintf(result,"%d",END_INTIMIDATION_PLAYS);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetFirstOtherPlay_cmd(ClientData clientData, 
	                      Tcl_Interp *interp,                      
                        int argc,		                           
                        char *argv[])	                         
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sprintf(result,"%d",START_OTHER_PLAYS);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetLastOtherPlay_cmd(ClientData clientData, 
	                      Tcl_Interp *interp,                      
                        int argc,		                           
                        char *argv[])	                         
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sprintf(result,"%d",END_OTHER_PLAYS);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetNoPlay_cmd(ClientData clientData, 
	              Tcl_Interp *interp,                     
                  int argc,		                           
                  char *argv[])	                         
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sprintf(result,"%d",NoPlay::playNum);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetLatencyPlay_cmd(ClientData clientData, 
	                     Tcl_Interp *interp,                    
                       int argc,		                          
                       char *argv[])	                        
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sprintf(result,"%d",LED_LatencyTest::playNum);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetPenaltyShotUsFormationPlay_cmd(ClientData clientData, 
	                                    Tcl_Interp *interp,                    
                                      int argc,		                          
                                      char *argv[])	                        
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sprintf(result,"%d",PenaltyShotUs::playNum);
  interp->result = result;
  return TCL_OK;
}//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int GetNumPlays_cmd(ClientData clientData, 
	                  Tcl_Interp *interp,                 
                    int argc,		                       
                    char *argv[])	                     
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sprintf(result,"%d",NUM_PLAYS);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int SetRobotLocation_cmd(ClientData clientData,
                         Tcl_Interp *interp,
                         int argc,
                         char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=4) 
    return ArgError(argv[0], interp);

  int specifiedObject = atoi(argv[1]);
  float specifiedObjectA = (float)atof(argv[2]);
  float specifiedObjectB = (float)atof(argv[3]);

  if(sm->CurrentTeam() == 0)
  {
    if(specifiedObject == 0) 
    {
      GUI_Placement.friendlyRobotX[0] = specifiedObjectA;
      GUI_Placement.friendlyRobotY[0] = specifiedObjectB;
    }
    else if(specifiedObject == 1)
    {
      GUI_Placement.friendlyRobotX[1] = specifiedObjectA;
      GUI_Placement.friendlyRobotY[1] = specifiedObjectB;
    }
    else if(specifiedObject == 2)
    {
      GUI_Placement.friendlyRobotX[2] = specifiedObjectA;
      GUI_Placement.friendlyRobotY[2] = specifiedObjectB;
    }
    else if(specifiedObject == 3)
    {
      GUI_Placement.friendlyRobotX[3] = specifiedObjectA;
      GUI_Placement.friendlyRobotY[3] = specifiedObjectB;
    }
    else if(specifiedObject == 4)
    {
      GUI_Placement.friendlyRobotX[4] = specifiedObjectA;
      GUI_Placement.friendlyRobotY[4] = specifiedObjectB;
    }
    else if(specifiedObject == 11)
    {
      GUI_Placement.opponentRobotX[0] = specifiedObjectA;
      GUI_Placement.opponentRobotY[0] = specifiedObjectB;
    }
    else if(specifiedObject == 12)
    {
      GUI_Placement.opponentRobotX[1] = specifiedObjectA;
      GUI_Placement.opponentRobotY[1] = specifiedObjectB;
    }
    else if(specifiedObject == 13)
    {
      GUI_Placement.opponentRobotX[2] = specifiedObjectA;
      GUI_Placement.opponentRobotY[2] = specifiedObjectB;
    }
    else if(specifiedObject == 14)
    {
      GUI_Placement.opponentRobotX[3] = specifiedObjectA;
      GUI_Placement.opponentRobotY[3] = specifiedObjectB;
    }
    else if(specifiedObject == 15)
    {
      GUI_Placement.opponentRobotX[4] = specifiedObjectA;
      GUI_Placement.opponentRobotY[4] = specifiedObjectB;
    }
  }
  else
  {
    if(specifiedObject == 0) 
    {
      GUI_Placement.opponentRobotX[0] = specifiedObjectA;
      GUI_Placement.opponentRobotY[0] = specifiedObjectB;
    }
    else if(specifiedObject == 1)
    {
      GUI_Placement.opponentRobotX[1] = specifiedObjectA;
      GUI_Placement.opponentRobotY[1] = specifiedObjectB;
    }
    else if(specifiedObject == 2)
    {
      GUI_Placement.opponentRobotX[2] = specifiedObjectA;
      GUI_Placement.opponentRobotY[2] = specifiedObjectB;
    }
    else if(specifiedObject == 3)
    {
      GUI_Placement.opponentRobotX[3] = specifiedObjectA;
      GUI_Placement.opponentRobotY[3] = specifiedObjectB;
    }
    else if(specifiedObject == 4)
    {
      GUI_Placement.opponentRobotX[4] = specifiedObjectA;
      GUI_Placement.opponentRobotY[4] = specifiedObjectB;
    }
    else if(specifiedObject == 11)
    {
      GUI_Placement.friendlyRobotX[0] = specifiedObjectA;
      GUI_Placement.friendlyRobotY[0] = specifiedObjectB;
    }
    else if(specifiedObject == 12)
    {
      GUI_Placement.friendlyRobotX[1] = specifiedObjectA;
      GUI_Placement.friendlyRobotY[1] = specifiedObjectB;
    }
    else if(specifiedObject == 13)
    {
      GUI_Placement.friendlyRobotX[2] = specifiedObjectA;
      GUI_Placement.friendlyRobotY[2] = specifiedObjectB;
    }
    else if(specifiedObject == 14)
    {
      GUI_Placement.friendlyRobotX[3] = specifiedObjectA;
      GUI_Placement.friendlyRobotY[3] = specifiedObjectB;
    }
    else if(specifiedObject == 15)
    {
      GUI_Placement.friendlyRobotX[4] = specifiedObjectA;
      GUI_Placement.friendlyRobotY[4] = specifiedObjectB;
    }    
  }
  
  if(specifiedObject == 10)
  {
    GUI_Placement.ballX[0][0] = specifiedObjectA;
    GUI_Placement.ballY[0][0] = specifiedObjectB;
  }

  return 0;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int SetRobotRotation_cmd(ClientData clientData,
                         Tcl_Interp *interp,
                         int argc,
                         char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=3) 
    return ArgError(argv[0], interp);

  int specifiedObject = atoi(argv[1]);
  float specifiedObjectRotation = (float)atof(argv[2]);

  if(specifiedObject == 5)
  {
    GUI_Placement.friendlyRobotRotation[0] = specifiedObjectRotation;
    GUI_Placement.opponentRobotRotation[0] = specifiedObjectRotation;
  }
  else if(specifiedObject == 6)
  {
    GUI_Placement.friendlyRobotRotation[1] = specifiedObjectRotation;
    GUI_Placement.opponentRobotRotation[1] = specifiedObjectRotation;
  }
  else if(specifiedObject == 7)
  {
    GUI_Placement.friendlyRobotRotation[2] = specifiedObjectRotation;
    GUI_Placement.opponentRobotRotation[2] = specifiedObjectRotation;
  }
  else if(specifiedObject == 8)
  {
    GUI_Placement.friendlyRobotRotation[3] = specifiedObjectRotation;
    GUI_Placement.opponentRobotRotation[3] = specifiedObjectRotation;
  }
  else if(specifiedObject == 9)
  {
    GUI_Placement.friendlyRobotRotation[4] = specifiedObjectRotation;
    GUI_Placement.opponentRobotRotation[4] = specifiedObjectRotation;
  }

 return 0;

}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
//Gets X coordinate of a debug point
int getDebugPointX(ClientData clientData, 
	               Tcl_Interp *interp,               
                   int argc,		                     
                   char *argv[])	                   
{ 
  // if incorrect # of arguments, return error			
  if (argc !=3) 
    return ArgError(argv[0], interp);

  float res = GUI_Record.debuggingInfo.getDebugPointX(atoi(argv[1]),
                                                      atoi(argv[2]));
  //--------------------------------
  //ensure the value is valid
  if(
       _isnan(res) || //not a number
       !_finite(res)  //infinite
    )
  {
    res = -64000.0f; //this is the error value
  }
  //--------------------------------
  sprintf(result, "%f", res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
//Gets Y coordinate of a debug point
int getDebugPointY(ClientData clientData, 
	               Tcl_Interp *interp,               
                   int argc,		                     
                   char *argv[])	                   
{ 
  // if incorrect # of arguments, return error			
  if (argc !=3) 
    return ArgError(argv[0], interp);

  float res = GUI_Record.debuggingInfo.getDebugPointY(atoi(argv[1]),
                                                      atoi(argv[2]));
  //--------------------------------
  //ensure the value is valid
  if(
       _isnan(res) || //not a number
       !_finite(res)  //infinite
    )
  {
    res = -64000.0f; //this is the error value
  }
  //--------------------------------
  sprintf(result, "%f", res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
//Gets the number of debug points
int getNumDebugPoints(ClientData clientData, 
	                    Tcl_Interp *interp,               
                      int argc,		                     
                      char *argv[])	                   
{ 
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sprintf(result, "%d", NUM_DEBUG_POINTS);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
//Gets the number of debug messages to print to debug dialog box
int getNumDebugMessages_cmd(ClientData clientData, 
	                          Tcl_Interp *interp,               
                            int argc,		                     
                            char *argv[])	                   
{ 
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sprintf(result, "%d", GUI_Record.debuggingInfo.getNumDebugMessages());
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
//Gets specific debug messsage
int getDebugMessage(ClientData clientData, 
	                  Tcl_Interp *interp,               
                    int argc,		                     
                    char *argv[])	                   
{ 
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sprintf(result, "%s", GUI_Record.debuggingInfo.popDebugMessage());
  interp->result = result;

  return TCL_OK;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
int LEFT_SIDE_LINE_cmd(ClientData clientData, 

                	   Tcl_Interp *interp,                    

                       int argc,		                          

                       char *argv[])	                        

{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->getParameters()->field.LEFT_SIDE_LINE;
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
int RIGHT_SIDE_LINE_cmd(ClientData clientData, 
                	   Tcl_Interp *interp,                    
                       int argc,		                          
                       char *argv[])	                        
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->getParameters()->field.RIGHT_SIDE_LINE;
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
int OUR_GOAL_LINE_cmd(ClientData clientData, 
                	   Tcl_Interp *interp,                    
                       int argc,		                          
                       char *argv[])	                        
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->getParameters()->field.OUR_GOAL_LINE;
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
int THEIR_GOAL_LINE_cmd(ClientData clientData, 
                	   Tcl_Interp *interp,                    
                       int argc,		                          
                       char *argv[])	                        
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->getParameters()->field.THEIR_GOAL_LINE;
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
int GOAL_DEPTH_cmd(ClientData clientData, 
                	   Tcl_Interp *interp,                    
                       int argc,		                          
                       char *argv[])	                        
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->getParameters()->field.GOAL_DEPTH;
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
int LEFT_GOAL_POST_cmd(ClientData clientData, 
                	   Tcl_Interp *interp,                    
                       int argc,		                          
                       char *argv[])	                        
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->getParameters()->field.LEFT_GOAL_POST;
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
int RIGHT_GOAL_POST_cmd(ClientData clientData, 
               	   Tcl_Interp *interp,                    
                      int argc,		                          
                       char *argv[])	                        
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->getParameters()->field.RIGHT_GOAL_POST;
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
int LEFT_GOALIE_BOX_cmd(ClientData clientData, 
               	   Tcl_Interp *interp,                    
                      int argc,		                          
                       char *argv[])	                        
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->getParameters()->field.LEFT_GOALIE_BOX;
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
int RIGHT_GOALIE_BOX_cmd(ClientData clientData, 
               	   Tcl_Interp *interp,                    
                      int argc,		                          
                       char *argv[])	                        
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->getParameters()->field.RIGHT_GOALIE_BOX;
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
int GOALIE_BOX_DEPTH_cmd(ClientData clientData, 
               	   Tcl_Interp *interp,                    
                      int argc,		                          
                       char *argv[])	                        
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->getParameters()->field.GOALIEBOX_DEPTH;
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
int CENTER_CIRCLE_RADIUS_cmd(ClientData clientData, 
               	   Tcl_Interp *interp,                    
                      int argc,		                          
                       char *argv[])	                        
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);
  res = sm->getParameters()->field.CENTER_CIRCLE_RADIUS;
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
int FREE_KICK_X_OFFSET_cmd(ClientData clientData, 
               	   Tcl_Interp *interp,                    
                      int argc,		                          
                       char *argv[])	                        
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);
  res = sm->getParameters()->field.FREE_KICK_X_OFFSET;
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
int FREE_KICK_Y_OFFSET_cmd(ClientData clientData, 
               	   Tcl_Interp *interp,                    
                      int argc,		                          
                       char *argv[])	                        
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);
  res = sm->getParameters()->field.FREE_KICK_Y_OFFSET;
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
int PENALTY_KICK_X_OFFSET_cmd(ClientData clientData, 
               	   Tcl_Interp *interp,                    
                      int argc,		                          
                       char *argv[])	                        
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);
  res = sm->getParameters()->field.PENALTY_KICK_X_OFFSET;
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
int DEATH_ZONE_LINE_cmd(ClientData clientData, 
	                    Tcl_Interp *interp,               
                        int argc,		                     
                        char *argv[])
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->getParameters()->field.DEATH_ZONE_LINE;
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
int DEFENSE_ZONE_LINE_cmd(ClientData clientData, 
	                    Tcl_Interp *interp,               
                        int argc,		                     
                        char *argv[])
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->getParameters()->field.DEFENSE_ZONE_LINE;
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}						
//==========================================================================
//==========================================================================
int KILL_ZONE_LINE_cmd(ClientData clientData, 
	                    Tcl_Interp *interp,               
                        int argc,		                     
                        char *argv[])
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->getParameters()->field.KILL_ZONE_LINE;
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}						
//==========================================================================
//==========================================================================
int OFFENSE_ZONE_LINE_cmd(ClientData clientData, 
	                    Tcl_Interp *interp,               
                        int argc,		                     
                        char *argv[])
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->getParameters()->field.OFFENSE_ZONE_LINE;
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
int LEFT_MIDDLE_SECTOR_cmd(ClientData clientData, 
	                    Tcl_Interp *interp,               
                        int argc,		                     
                        char *argv[])
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->getParameters()->field.LEFT_MIDDLE_SECTOR;
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}						
//==========================================================================
//==========================================================================
int RIGHT_MIDDLE_SECTOR_cmd(ClientData clientData, 
	                    Tcl_Interp *interp,               
                        int argc,		                     
                        char *argv[])
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->getParameters()->field.RIGHT_MIDDLE_SECTOR;
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
//==========================================================================
int LEFT_SWEET_SECTOR_cmd(ClientData clientData, 
	                    Tcl_Interp *interp,               
                        int argc,		                     
                        char *argv[])
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->getParameters()->field.LEFT_SWEET_SECTOR;
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}						
//==========================================================================
//==========================================================================
int RIGHT_SWEET_SECTOR_cmd(ClientData clientData, 
	                    Tcl_Interp *interp,               
                        int argc,		                     
                        char *argv[])
{
  float res;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  res = sm->getParameters()->field.RIGHT_SWEET_SECTOR;
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}						
//==========================================================================
//==========================================================================


//==========================================================================
//System Quality Test Commaands
//-----------------------------------------------------
int get_VISION_EXECUTE_TIME(ClientData clientData, 
	                          Tcl_Interp *interp,               
                            int argc,		                     
                            char *argv[])
{
  if (argc !=1) 
    return ArgError(argv[0], interp);

  double res = sm->visionExecuteTime();
  sprintf(result, "%9.3lf",res);
  interp->result = result;
  return TCL_OK;
}
//-----------------------------------------------------
int get_VISION_GET_TIME(ClientData clientData, 
	                          Tcl_Interp *interp,               
                            int argc,		                     
                            char *argv[])	                  
{
  if (argc !=1) 
    return ArgError(argv[0], interp);
  double res = sm->visionGetTime();
  sprintf(result, "%9.3lf",res);
  interp->result = result;
  return TCL_OK;
}
//-----------------------------------------------------
int get_WIRELESS_EXECUTE_TIME(ClientData clientData, 
	                          Tcl_Interp *interp,               
                            int argc,		                     
                            char *argv[])	                   
{
  if (argc !=1) 
    return ArgError(argv[0], interp);
  double res = sm->wirelessExecuteTime();
  sprintf(result, "%9.3lf",res);
  interp->result = result;
  return TCL_OK;
}
//-----------------------------------------------------
int get_SIMULATOR_EXECUTE_TIME(ClientData clientData, 
	                             Tcl_Interp *interp,               
                               int argc,		                     
                               char *argv[])	                   
{
  if (argc !=1) 
    return ArgError(argv[0], interp);
  double res = sm->simulatorExecuteTime();
  sprintf(result, "%9.3lf",res);
  interp->result = result;
  return TCL_OK;
}
//-----------------------------------------------------
int get_STRATEGY_EXECUTE_TIME(ClientData clientData, 
	                          Tcl_Interp *interp,               
                            int argc,		                     
                            char *argv[])	                   
{
  if (argc !=1) 
    return ArgError(argv[0], interp);
  double res = sm->strategyExecuteTime();
  sprintf(result, "%9.3lf",res);
  interp->result = result;
  return TCL_OK;
}
//-----------------------------------------------------
int get_TRAJECTORY_EXECUTE_TIME(ClientData clientData, 
	                          Tcl_Interp *interp,               
                            int argc,		                     
                            char *argv[])	                   
{
  if (argc !=1) 
    return ArgError(argv[0], interp);
  double res = sm->trajectoryExecuteTime();
  sprintf(result, "%9.3lf",res);
  interp->result = result;
  return TCL_OK;
}
//-----------------------------------------------------
int get_TOTAL_EXECUTE_TIME(ClientData clientData, 
	                          Tcl_Interp *interp,               
                            int argc,		                     
                            char *argv[])	                   
{
  if (argc !=1) 
    return ArgError(argv[0], interp);
  double res = sm->totalExecuteTime();
  sprintf(result, "%9.3lf",res);
  interp->result = result;
  return TCL_OK;
}
//-----------------------------------------------------
int get_AI_FPS(ClientData clientData, 
	             Tcl_Interp *interp,               
               int argc,		                     
               char *argv[])	                   
{
  if (argc !=1) 
    return ArgError(argv[0], interp);
  double res = sm->aiFPS();
  sprintf(result, "%9.3lf",res);
  interp->result = result;
  return TCL_OK;
}
//-----------------------------------------------------
int get_VISION_FPS(ClientData clientData, 
	                 Tcl_Interp *interp,               
                   int argc,		                     
                   char *argv[])	                   
{
  if (argc !=1) 
    return ArgError(argv[0], interp);
  double res = sm->visionFPS();
  sprintf(result, "%9.3lf",res);
  interp->result = result;
  return TCL_OK;
}
//-----------------------------------------------------
int TogglePlayTransitions_cmd(ClientData clientData, 
	                            Tcl_Interp *interp,                  
                              int argc,		                        
                              char *argv[])	                      
{
  int res = 0;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);


  if(sm->togglePlayTransitions())
    GUI_Record.debuggingInfo.addDebugMessage("Turning ON Play Transitioning");
  else
    GUI_Record.debuggingInfo.addDebugMessage("Turning OFF Play Transitioning");

  sprintf(result, "%d",res);
  interp->result = result;
  return TCL_OK;
}
//-----------------------------------------------------
int TogglePositionSwitching_cmd(ClientData clientData, 
	                              Tcl_Interp *interp,                  
                                int argc,		                        
                                char *argv[])	                      
{
  int res = 0;
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  if(sm->togglePositionSwitching())
    GUI_Record.debuggingInfo.addDebugMessage("Turning ON Position Switching");
  else
    GUI_Record.debuggingInfo.addDebugMessage("Turning OFF Position Switching");
  
  sprintf(result, "%d",res);
  interp->result = result;
  return TCL_OK;
}
//-----------------------------------------------------
int RESET_GOALIE_BOX_DEPTH(ClientData clientData, 
	                            Tcl_Interp *interp,               
                              int argc,		                     
                              char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sm->getParameters()->field.GOALIEBOX_DEPTH = sm->GetBallRawX() - 
                                         sm->getParameters()->field.OUR_GOAL_LINE;
  sprintf(result, "set");
  interp->result = result;
  return TCL_OK;
}
//-----------------------------------------------------
int RESET_OUR_GOAL_LINE(ClientData clientData, 
	                      Tcl_Interp *interp,                  
                        int argc,		                        
                        char *argv[])	                      
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sm->getParameters()->field.OUR_GOAL_LINE = sm->GetBallRawX();
  sprintf(result, "set");
  interp->result = result;
  return TCL_OK;
}
//-----------------------------------------------------
int RESET_THEIR_GOAL_LINE(ClientData clientData, 
	                      Tcl_Interp *interp,                  
                        int argc,		                        
                        char *argv[])	                      
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sm->getParameters()->field.THEIR_GOAL_LINE = sm->GetBallRawX();
  sprintf(result, "set");
  interp->result = result;
  return TCL_OK;
}
//-----------------------------------------------------
int RESET_OUR_LEFT_GOAL_WALL(ClientData clientData, 
	                      Tcl_Interp *interp,                  
                        int argc,		                        
                        char *argv[])	                      
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sm->getParameters()->field.OUR_LEFT_GOAL_WALL = sm->GetBallRawX();
  sprintf(result, "set");
  interp->result = result;
  return TCL_OK;
}
//-----------------------------------------------------         
int RESET_OUR_RIGHT_GOAL_WALL(ClientData clientData, 
	                      Tcl_Interp *interp,                  
                        int argc,		                        
                        char *argv[])	                      
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sm->getParameters()->field.OUR_RIGHT_GOAL_WALL = sm->GetBallRawX();
  sprintf(result, "set");
  interp->result = result;
  return TCL_OK;
}
//-----------------------------------------------------         
int RESET_THEIR_LEFT_GOAL_WALL(ClientData clientData, 
	                      Tcl_Interp *interp,                  
                        int argc,		                        
                        char *argv[])	                      
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sm->getParameters()->field.THEIR_LEFT_GOAL_WALL = sm->GetBallRawX();
  sprintf(result, "set");
  interp->result = result;
  return TCL_OK;
}
//-----------------------------------------------------
int RESET_THEIR_RIGHT_GOAL_WALL(ClientData clientData, 
	                      Tcl_Interp *interp,                  
                        int argc,		                        
                        char *argv[])	                      
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sm->getParameters()->field.THEIR_RIGHT_GOAL_WALL = sm->GetBallRawX();
  sprintf(result, "set");
  interp->result = result;
  return TCL_OK;
}
//-----------------------------------------------------
int RESET_RIGHT_SIDE_LINE(ClientData clientData, 
	                      Tcl_Interp *interp,                  
                        int argc,		                        
                        char *argv[])	                      
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sm->getParameters()->field.RIGHT_SIDE_LINE = sm->GetBallRawY();
  sprintf(result, "set");
  interp->result = result;
  return TCL_OK;
}
//-----------------------------------------------------
int RESET_RIGHT_GOALIE_BOX(ClientData clientData, 
	                      Tcl_Interp *interp,                  
                        int argc,		                        
                        char *argv[])	                      
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sm->getParameters()->field.RIGHT_GOALIE_BOX = sm->GetBallRawY();
  sprintf(result, "set");
  interp->result = result;
  return TCL_OK;
}
//-----------------------------------------------------
int RESET_RIGHT_GOAL_POST(ClientData clientData, 
	                      Tcl_Interp *interp,                  
                        int argc,		                        
                        char *argv[])	                      
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sm->getParameters()->field.RIGHT_GOAL_POST = sm->GetBallRawY();
  sprintf(result, "set");
  interp->result = result;
  return TCL_OK;
}
//-----------------------------------------------------
int RESET_LEFT_GOAL_POST(ClientData clientData, 
	                      Tcl_Interp *interp,                  
                        int argc,		                        
                        char *argv[])	                      
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sm->getParameters()->field.LEFT_GOAL_POST = sm->GetBallRawY();
  sprintf(result, "set");
  interp->result = result;
  return TCL_OK;
}
//-----------------------------------------------------
int RESET_LEFT_GOALIE_BOX(ClientData clientData, 
	                      Tcl_Interp *interp,                  
                        int argc,		                        
                        char *argv[])	                      
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sm->getParameters()->field.LEFT_GOALIE_BOX = sm->GetBallRawY();
  sprintf(result, "set");
  interp->result = result;
  return TCL_OK;
}
//-----------------------------------------------------               
int RESET_LEFT_SIDE_LINE(ClientData clientData, 
	                      Tcl_Interp *interp,                  
                        int argc,		                        
                        char *argv[])	                      
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sm->getParameters()->field.LEFT_SIDE_LINE = sm->GetBallRawY();
  sprintf(result, "set");
  interp->result = result;
  return TCL_OK;
}
//-----------------------------------------------------               
int RESET_OUR_LEFT_CORNER(ClientData clientData, 
	                      Tcl_Interp *interp,                  
                        int argc,		                        
                        char *argv[])	                      
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sm->getParameters()->field.OUR_LEFT_X = sm->GetBallRawX();
  sm->getParameters()->field.OUR_LEFT_Y = sm->GetBallRawY();
  sprintf(result, "set");
  interp->result = result;
  return TCL_OK;
}
//-----------------------------------------------------               
int RESET_OUR_RIGHT_CORNER(ClientData clientData, 
	                      Tcl_Interp *interp,                  
                        int argc,		                        
                        char *argv[])	                      
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sm->getParameters()->field.OUR_RIGHT_X = sm->GetBallRawX();
  sm->getParameters()->field.OUR_RIGHT_Y = sm->GetBallRawY();
  sprintf(result, "set");
  interp->result = result;
  return TCL_OK;
}
//-----------------------------------------------------               
int RESET_THEIR_LEFT_CORNER(ClientData clientData, 
	                      Tcl_Interp *interp,                  
                        int argc,		                        
                        char *argv[])	                      
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sm->getParameters()->field.THEIR_LEFT_X = sm->GetBallRawX();
  sm->getParameters()->field.THEIR_LEFT_Y = sm->GetBallRawY();
  sprintf(result, "set");
  interp->result = result;
  return TCL_OK;
}
//-----------------------------------------------------               
int RESET_THEIR_RIGHT_CORNER(ClientData clientData, 
	                           Tcl_Interp *interp,                  
                             int argc,		                        
                             char *argv[])	                      
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sm->getParameters()->field.THEIR_RIGHT_X = sm->GetBallRawX();
  sm->getParameters()->field.THEIR_RIGHT_Y = sm->GetBallRawY();
  sprintf(result, "set");
  interp->result = result;
  return TCL_OK;
}
//-----------------------------------------------------               
int RESET_CENTER(ClientData clientData, 
	                           Tcl_Interp *interp,                  
                             int argc,		                        
                             char *argv[])	                      
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sm->getParameters()->field.CENTER_X = sm->GetBallRawX();
  sm->getParameters()->field.CENTER_Y = sm->GetBallRawY();
  sprintf(result, "set");
  interp->result = result;
  return TCL_OK;
}
//-----------------------------------------------------
int SAVE_FIELD_PARAMETERS(ClientData clientData, 
	                      Tcl_Interp *interp,                  
                        int argc,		                        
                        char *argv[])	                      
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sm->getParameters()->field.saveValues();
  sprintf(result, "saved");
  interp->result = result;
  return TCL_OK;
}
//-----------------------------------------------------
