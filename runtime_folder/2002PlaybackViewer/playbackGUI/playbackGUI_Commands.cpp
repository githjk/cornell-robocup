#define PI 3.141592653589f

//Declare GUI_Commands information before defining it
#include "playbackGUI_Commands.h"

//==========================================================================
int toggleTeam_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                     
	     int argc,		                           
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "N/A");
    interp->result = result;
    return TCL_OK;
  }
  
  if(playbackSequenceInfo.thisTeam == 0)
  {
	  playbackSequenceInfo.thisTeam = 1;
      playbackSequenceInfo.otherTeam = 0;
  }
  else
  {
	  playbackSequenceInfo.thisTeam = 0;
      playbackSequenceInfo.otherTeam = 1;
  }

  strcpy(result, "-");

  interp->result = result;
  return TCL_OK;
}       
//==========================================================================


//############################
//#### Our robot messages ####
//############################
//==========================================================================
int GetOurRobotFound_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                     
	     int argc,		                           
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "N/A");
    interp->result = result;
    return TCL_OK;
  }
  
  if(playbackSequenceInfo.thisTeam == 0)
	  sprintf(result, "%d", (int)framePointers[currentFrameNumber]->team0Robots[atoi(argv[1])].found);
  else
	  sprintf(result, "%d", (int)framePointers[currentFrameNumber]->team1Robots[atoi(argv[1])].found);
  
  interp->result = result;
  return TCL_OK;
}       
//--------------------------------------------------------------------------
int GetRobotPosition_cmd(ClientData clientData,
	     Tcl_Interp *interp,                     
	     int argc,		                           
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "N/A");
    interp->result = result;
    return TCL_OK;
  }

  if(playbackSequenceInfo.thisTeam == 0)
	  sprintf(result, "%s", framePointers[currentFrameNumber]->team0Robots[atoi(argv[1])].position);
  else
	  sprintf(result, "%s", framePointers[currentFrameNumber]->team1Robots[atoi(argv[1])].position);

  interp->result = result;
  return TCL_OK;
}
//==========================================================================
int GetRobotMessage_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                     
	     int argc,		                           
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "N/A");
    interp->result = result;
    return TCL_OK;
  }

  if(playbackSequenceInfo.thisTeam == 0)
	  sprintf(result, "%s", framePointers[currentFrameNumber]->team0Robots[atoi(argv[1])].debugMessage);
  else
	  sprintf(result, "%s", framePointers[currentFrameNumber]->team1Robots[atoi(argv[1])].debugMessage);

  interp->result = result;
  return TCL_OK;
}           
//==========================================================================
int GetOurRobotX_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                
	     int argc,		                      
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  if(playbackSequenceInfo.thisTeam == 0)
	  sprintf(result, "%f", framePointers[currentFrameNumber]->team0Robots[atoi(argv[1])].location.x);
  else
  	  sprintf(result, "%f", -framePointers[currentFrameNumber]->team1Robots[atoi(argv[1])].location.x);

  interp->result = result;
  return TCL_OK;
}      
//--------------------------------------------------------------------------
int GetOurRobotY_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                  
	     int argc,		                        
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  if(playbackSequenceInfo.thisTeam == 0)
	  sprintf(result, "%f", framePointers[currentFrameNumber]->team0Robots[atoi(argv[1])].location.y);
  else
	  sprintf(result, "%f", -framePointers[currentFrameNumber]->team1Robots[atoi(argv[1])].location.y);

  interp->result = result;
  return TCL_OK;
}            
//==========================================================================
int GetOurRobotRawX_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                
	     int argc,		                      
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  if(playbackSequenceInfo.thisTeam == 0)
	  sprintf(result, "%f", framePointers[currentFrameNumber]->team0Robots[atoi(argv[1])].rawLocation.x);
  else
  	  sprintf(result, "%f", -framePointers[currentFrameNumber]->team1Robots[atoi(argv[1])].rawLocation.x);

  interp->result = result;
  return TCL_OK;
}      
//--------------------------------------------------------------------------
int GetOurRobotRawY_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                  
	     int argc,		                        
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  if(playbackSequenceInfo.thisTeam == 0)
	  sprintf(result, "%f", framePointers[currentFrameNumber]->team0Robots[atoi(argv[1])].rawLocation.y);
  else
	  sprintf(result, "%f", -framePointers[currentFrameNumber]->team1Robots[atoi(argv[1])].rawLocation.y);

  interp->result = result;
  return TCL_OK;
}      
//--------------------------------------------------------------------------
int GetOurRobotSentXVel_cmd(ClientData clientData,
	     Tcl_Interp *interp,                
	     int argc,		                      
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  if(playbackSequenceInfo.thisTeam == 0)
	  sprintf(result, "%f", framePointers[currentFrameNumber]->team0Robots[atoi(argv[1])].velocity.x);
  else
  	  sprintf(result, "%f", -framePointers[currentFrameNumber]->team1Robots[atoi(argv[1])].velocity.x);

  interp->result = result;
  return TCL_OK;
}            
//--------------------------------------------------------------------------
int GetOurRobotSentYVel_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                     
	     int argc,		                           
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  if(playbackSequenceInfo.thisTeam == 0)
	  sprintf(result, "%f", framePointers[currentFrameNumber]->team0Robots[atoi(argv[1])].velocity.y);
  else
	  sprintf(result, "%f", -framePointers[currentFrameNumber]->team1Robots[atoi(argv[1])].velocity.y);
	  
  interp->result = result;
  return TCL_OK;
}          
//--------------------------------------------------------------------------
int GetOurRobotRotation_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                
	     int argc,		                      
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  if(playbackSequenceInfo.thisTeam == 0)
	  sprintf(result, "%f", framePointers[currentFrameNumber]->team0Robots[atoi(argv[1])].rotation);
  else
  {
      float angle = framePointers[currentFrameNumber]->team1Robots[atoi(argv[1])].rotation;
      angle = angle + PI;
	  if(angle > PI)
		  angle = angle - 2*PI;
	  
	  sprintf(result, "%f", angle);
  }
  interp->result = result;
  return TCL_OK;
}          
//--------------------------------------------------------------------------
int GetOurRobotRawRotation_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                
	     int argc,		                      
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  if(playbackSequenceInfo.thisTeam == 0)
	  sprintf(result, "%f", framePointers[currentFrameNumber]->team0Robots[atoi(argv[1])].rawRotation);
  else
  {
      float angle = framePointers[currentFrameNumber]->team1Robots[atoi(argv[1])].rawRotation;
      angle = angle + PI;
	  if(angle > PI)
		  angle = angle - 2*PI;
	  
	  sprintf(result, "%f", angle);
  }
  interp->result = result;
  return TCL_OK;
}     //--------------------------------------------------------------------------
int GetOurRobotSentRotation_cmd(ClientData clientData, 
	     Tcl_Interp *interp,               
	     int argc,		                     
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  if(playbackSequenceInfo.thisTeam == 0)
	  sprintf(result, "%f", framePointers[currentFrameNumber]->team0Robots[atoi(argv[1])].rotationVelocity);
  else
	  sprintf(result, "%f", framePointers[currentFrameNumber]->team1Robots[atoi(argv[1])].rotationVelocity);

  interp->result = result;
  return TCL_OK;
}   
//--------------------------------------------------------------------------
int GetRobotKick_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                   
	     int argc,		                         
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  if(playbackSequenceInfo.thisTeam == 0)
	  sprintf(result, "%d", (int)framePointers[currentFrameNumber]->team0Robots[atoi(argv[1])].kick);
  else
	  sprintf(result, "%d", (int)framePointers[currentFrameNumber]->team1Robots[atoi(argv[1])].kick);

      interp->result = result;
  return TCL_OK;
}          
//==========================================================================
int GetRobotDribble_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                     
	     int argc,		                           
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  if(playbackSequenceInfo.thisTeam == 0)
	  sprintf(result, "%d", (int)framePointers[currentFrameNumber]->team0Robots[atoi(argv[1])].dribble);
  else
	  sprintf(result, "%d", (int)framePointers[currentFrameNumber]->team1Robots[atoi(argv[1])].dribble);

  interp->result = result;
  return TCL_OK;
}   
//==========================================================================
int GetFriendlyRobotHasPossesion_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                     
	     int argc,		                           
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  if(playbackSequenceInfo.thisTeam == 0)
	  sprintf(result, "%d", (int)framePointers[currentFrameNumber]->team0Robots[atoi(argv[1])].hasPossession);
  else
	  sprintf(result, "%d", (int)framePointers[currentFrameNumber]->team1Robots[atoi(argv[1])].hasPossession);

  interp->result = result;
  return TCL_OK;
} //==========================================================================
int GetOpponentRobotHasPossesion_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                     
	     int argc,		                           
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  if(playbackSequenceInfo.thisTeam == 0)
	  sprintf(result, "%d", (int)framePointers[currentFrameNumber]->team1Robots[atoi(argv[1])].hasPossession);
  else
	  sprintf(result, "%d", (int)framePointers[currentFrameNumber]->team0Robots[atoi(argv[1])].hasPossession);

  interp->result = result;
  return TCL_OK;
} 
//--------------------------------------------------------------------------
int GetOurRobotDestinationX_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                
	     int argc,		                      
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  if(playbackSequenceInfo.thisTeam == 0)
	  sprintf(result, "%f", framePointers[currentFrameNumber]->team0Robots[atoi(argv[1])].destination.x);
  else
      sprintf(result, "%f", -framePointers[currentFrameNumber]->team1Robots[atoi(argv[1])].destination.x);

  interp->result = result;
  return TCL_OK;
}      
//--------------------------------------------------------------------------
int GetOurRobotDestinationY_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                  
	     int argc,		                        
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  if(playbackSequenceInfo.thisTeam == 0)
      sprintf(result, "%f", framePointers[currentFrameNumber]->team0Robots[atoi(argv[1])].destination.y);
  else
      sprintf(result, "%f", -framePointers[currentFrameNumber]->team1Robots[atoi(argv[1])].destination.y);

  interp->result = result;
  return TCL_OK;
}            
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

    if(!playbackExists)
	{
      strcpy(result, "0");
      interp->result = result;
      return TCL_OK;
	}

    if(playbackSequenceInfo.thisTeam == 0)
       sprintf(result, "%f", framePointers[currentFrameNumber]->team0Robots[atoi(argv[1])].destinationRot);
    else
       sprintf(result, "%f", -framePointers[currentFrameNumber]->team1Robots[atoi(argv[1])].destinationRot);
  
	interp->result = result;
	return TCL_OK;
}
//--------------------------------------------------------------------------
int GetOurRobotPassValue_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                
	     int argc,		                      
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  if(playbackSequenceInfo.thisTeam == 0)
	  sprintf(result, "%i", (int)(framePointers[currentFrameNumber]->team0Robots[atoi(argv[1])].passToMe));
  else
	  sprintf(result, "%i", (int)(framePointers[currentFrameNumber]->team1Robots[atoi(argv[1])].passToMe));

  interp->result = result;
  return TCL_OK;
}      
//--------------------------------------------------------------------------
int GetOurRobotPassDestinationX_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                
	     int argc,		                      
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  if(playbackSequenceInfo.thisTeam == 0)
	  sprintf(result, "%f", framePointers[currentFrameNumber]->team0Robots[atoi(argv[1])].passToLocation.x);
  else
	  sprintf(result, "%f", -framePointers[currentFrameNumber]->team1Robots[atoi(argv[1])].passToLocation.x);

  interp->result = result;
  return TCL_OK;
}      
//--------------------------------------------------------------------------
int GetOurRobotPassDestinationY_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                  
	     int argc,		                        
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  if(playbackSequenceInfo.thisTeam == 0)
      sprintf(result, "%f", framePointers[currentFrameNumber]->team0Robots[atoi(argv[1])].passToLocation.y);
  else
      sprintf(result, "%f", -framePointers[currentFrameNumber]->team1Robots[atoi(argv[1])].passToLocation.y);

  interp->result = result;
  return TCL_OK;
}            
//--------------------------------------------------------------------------


//==========================================================================
//##############################
//#### Their robot messages ####
//##############################
//==========================================================================
int GetTheirRobotFound_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                     
	     int argc,		                           
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  if(playbackSequenceInfo.otherTeam == 1)
	  sprintf(result, "%d", (int)framePointers[currentFrameNumber]->team1Robots[atoi(argv[1])].found);
  else
	  sprintf(result, "%d", (int)framePointers[currentFrameNumber]->team0Robots[atoi(argv[1])].found);

  interp->result = result;
  return TCL_OK;
}   
//--------------------------------------------------------------------------
int GetTheirRobotX_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                    
	     int argc,		                          
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  if(playbackSequenceInfo.otherTeam == 1)
	  sprintf(result, "%f", framePointers[currentFrameNumber]->team1Robots[atoi(argv[1])].location.x);
  else
	  sprintf(result, "%f", -framePointers[currentFrameNumber]->team0Robots[atoi(argv[1])].location.x);

  interp->result = result;
  return TCL_OK;
}          
//--------------------------------------------------------------------------
int GetTheirRobotY_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                    
	     int argc,		                          
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  if(playbackSequenceInfo.otherTeam == 1)
	sprintf(result, "%f", framePointers[currentFrameNumber]->team1Robots[atoi(argv[1])].location.y);
  else
	sprintf(result, "%f", -framePointers[currentFrameNumber]->team0Robots[atoi(argv[1])].location.y);
  
  interp->result = result;
  return TCL_OK;
}         
//--------------------------------------------------------------------------
int GetTheirRobotRawX_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                    
	     int argc,		                          
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  if(playbackSequenceInfo.otherTeam == 1)
	  sprintf(result, "%f", framePointers[currentFrameNumber]->team1Robots[atoi(argv[1])].rawLocation.x);
  else
	  sprintf(result, "%f", -framePointers[currentFrameNumber]->team0Robots[atoi(argv[1])].rawLocation.x);

  interp->result = result;
  return TCL_OK;
}          
//--------------------------------------------------------------------------
int GetTheirRobotRawY_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                    
	     int argc,		                          
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  if(playbackSequenceInfo.otherTeam == 1)
	sprintf(result, "%f", framePointers[currentFrameNumber]->team1Robots[atoi(argv[1])].rawLocation.y);
  else
	sprintf(result, "%f", -framePointers[currentFrameNumber]->team0Robots[atoi(argv[1])].rawLocation.y);
  
  interp->result = result;
  return TCL_OK;
}        
//--------------------------------------------------------------------------
int GetTheirRobotXVel_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                     
	     int argc,		                           
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  if(playbackSequenceInfo.otherTeam == 1)
	  sprintf(result, "%f", framePointers[currentFrameNumber]->team1Robots[atoi(argv[1])].velocity.x);
  else
	  sprintf(result, "%f", -framePointers[currentFrameNumber]->team0Robots[atoi(argv[1])].velocity.x);

      interp->result = result;
  return TCL_OK;
}         
//--------------------------------------------------------------------------
int GetTheirRobotYVel_cmd(ClientData clientData,
	     Tcl_Interp *interp,                    
	     int argc,		                          
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  if(playbackSequenceInfo.otherTeam == 1)
	  sprintf(result, "%f", framePointers[currentFrameNumber]->team1Robots[atoi(argv[1])].velocity.y);
  else
	  sprintf(result, "%f", -framePointers[currentFrameNumber]->team0Robots[atoi(argv[1])].velocity.y);

  interp->result = result;
  return TCL_OK;
}         
//--------------------------------------------------------------------------
//#######################
//#### Ball messages ####
//#######################
//--------------------------------------------------------------------------
int GetRoboCupBallFound_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                     
	     int argc,		                           
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  if(playbackSequenceInfo.thisTeam == 0)
	  sprintf(result, "%d", (int)framePointers[currentFrameNumber]->team0Balls[0].found);
  else
	  sprintf(result, "%d", (int)framePointers[currentFrameNumber]->team1Balls[0].found);

  interp->result = result;
  return TCL_OK;
}  
//===========================================================
int GetBallX_cmd(ClientData clientData, 
	     Tcl_Interp *interp,               
	     int argc,		                     
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  if(playbackSequenceInfo.thisTeam == 0)
	  sprintf(result, "%f", framePointers[currentFrameNumber]->team0Balls[0].location.x);
  else
	  sprintf(result, "%f", -framePointers[currentFrameNumber]->team1Balls[0].location.x);

  interp->result = result;
  return TCL_OK;
}         
//--------------------------------------------------------------------------
int GetBallY_cmd(ClientData clientData, 
	     Tcl_Interp *interp,              
	     int argc,		                    
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  if(playbackSequenceInfo.thisTeam == 0)
	  sprintf(result, "%f", framePointers[currentFrameNumber]->team0Balls[0].location.y);
  else
	  sprintf(result, "%f", -framePointers[currentFrameNumber]->team1Balls[0].location.y);

  interp->result = result;
  return TCL_OK;
}       
//===========================================================
int GetBallRawX_cmd(ClientData clientData, 
	     Tcl_Interp *interp,               
	     int argc,		                     
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  if(playbackSequenceInfo.thisTeam == 0)
	  sprintf(result, "%f", framePointers[currentFrameNumber]->team0Balls[0].rawLocation.x);
  else
	  sprintf(result, "%f", -framePointers[currentFrameNumber]->team1Balls[0].rawLocation.x);

  interp->result = result;
  return TCL_OK;
}         
//--------------------------------------------------------------------------
int GetBallRawY_cmd(ClientData clientData, 
	     Tcl_Interp *interp,              
	     int argc,		                    
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  if(playbackSequenceInfo.thisTeam == 0)
	  sprintf(result, "%f", framePointers[currentFrameNumber]->team0Balls[0].rawLocation.y);
  else
	  sprintf(result, "%f", -framePointers[currentFrameNumber]->team1Balls[0].rawLocation.y);

  interp->result = result;
  return TCL_OK;
}     
//--------------------------------------------------------------------------
int GetBallXVel_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                 
	     int argc,		                       
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  if(playbackSequenceInfo.thisTeam == 0)
	  sprintf(result, "%f", framePointers[currentFrameNumber]->team0Balls[0].velocity.x);
  else
	  sprintf(result, "%f", -framePointers[currentFrameNumber]->team1Balls[0].velocity.x);

  interp->result = result;
  return TCL_OK;
}
//--------------------------------------------------------------------------
int GetBallYVel_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                 
	     int argc,		                       
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  if(playbackSequenceInfo.thisTeam == 0)
	  sprintf(result, "%f", framePointers[currentFrameNumber]->team0Balls[0].velocity.y);
  else
	  sprintf(result, "%f", -framePointers[currentFrameNumber]->team1Balls[0].velocity.y);

  interp->result = result;
  return TCL_OK;
}       
//==========================================================================
//#########################
//#### System messages ####
//#########################
//--------------------------------------------------------------------------
int GetCurrentFrame_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                    
	     int argc,		                          
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  sprintf(result, "%d", currentFrameNumber);
      interp->result = result;
  return TCL_OK;
}       
//==========================================================================
int GetCurrentPlayName_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                     
	     int argc,		                           
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "N/A");
    interp->result = result;
    return TCL_OK;
  }

  sprintf(result, "%s", framePointers[currentFrameNumber]->systemMessages.play);
      interp->result = result;
  return TCL_OK;
}      
//==========================================================================
//##########################
//#### Playback buttons ####
//##########################
//==========================================================================
int ForwardFrame_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                    
	     int argc,		                          
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  if(
     !playbackExists ||
     (currentFrameNumber>=(playbackSequenceInfo.numberOfFrames-1))
    )
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  //ok, increment frame #
  currentFrameNumber++;

  sprintf(result, "%s", "Wow, this worked!"); //yes, cs majors are sick people
      interp->result = result;
  return TCL_OK;
}  
//==========================================================================
int BackwardFrame_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                    
	     int argc,		                          
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  if(
     !playbackExists || 
     (currentFrameNumber==0)
    )
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  //ok, increment frame #
  currentFrameNumber--;

  sprintf(result, "%s", "Wow, this worked!"); //yes, cs majors are sick people
      interp->result = result;
  return TCL_OK;
}         
//==========================================================================
int SetFrame_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                    
	     int argc,		                          
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  if(
     !playbackExists || 
     (atoi(argv[1]) < 0) ||
     (atoi(argv[1]) >=(playbackSequenceInfo.numberOfFrames-1))
    )
  {
    strcpy(result, "00");
    interp->result = result;
    return TCL_OK;
  }

  //ok, increment frame #
  currentFrameNumber = atoi(argv[1]);

  sprintf(result, "%s", "Wow, this worked!"); //yes, cs majors are sick people
      interp->result = result;
  return TCL_OK;
}        
//==========================================================================
int NumberOfFrames_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                    
	     int argc,		                          
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  sprintf(result, "%d", playbackSequenceInfo.numberOfFrames); //yes, cs majors are sick people
      interp->result = result;
  return TCL_OK;
}        
//==========================================================================
int LoadFile_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                    
	     int argc,		                          
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  //attempt to read the file :)
  bool loadOk = readSequence(argv[1]);

  if(loadOk)
    strcpy(result, "File loaded successfully!");
  else
    strcpy(result, "ERROR!");

  interp->result = result;
  return TCL_OK;
}
//==========================================================================
int SaveFile_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                    
	     int argc,		                          
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  writePlaybackSequence(argv[1]);
  strcpy(result, "happy");

  interp->result = result;
  return TCL_OK;
}
//==========================================================================
int AppendFile_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                    
	     int argc,		                          
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  //attempt to read the file :)
  bool loadOk = appendSequence(argv[1]);

  if(loadOk)
    strcpy(result, "File loaded successfully!");
  else
    strcpy(result, "ERROR!");

  interp->result = result;
  return TCL_OK;
}
//==========================================================================
int GUIMode_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                    
	     int argc,		                          
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  sprintf(result, "%d", 1); //yes, cs majors are sick people
      interp->result = result;
  return TCL_OK;
}   
//==========================================================================
int SetCropLeft_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                  
	     int argc,		                        
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  tempCropLeft = atoi(argv[1]);

  sprintf(result, "ok");
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
int SetCropRight_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                  
	     int argc,		                        
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  tempCropRight = atoi(argv[1]);

  sprintf(result, "ok");
  interp->result = result;
  return TCL_OK;
}
//==========================================================================
int SetCrop_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                  
	     int argc,		                        
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=1) 
    return ArgError(argv[0], interp);

  if(!playbackExists || tempCropLeft == -32000 || tempCropRight == -32000)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  crop* newCrop = new crop;
  newCrop->leftFrame = tempCropLeft;
  newCrop->rightFrame = tempCropRight;
  newCrop->nextCrop = NULL;

  if(firstCrop == NULL)
  {
	  firstCrop = newCrop;
	  lastCrop = newCrop;
  }
  else
  {

	  lastCrop->nextCrop = newCrop;
	  lastCrop = newCrop;
  }
 
  tempCropLeft = -32000;
  tempCropRight = -32000;

  sprintf(result, "ok");
  interp->result = result;
  return TCL_OK;
}
//--------------------------------------------------------------------------
int GetLeftMostCropIncludingFrame_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                  
	     int argc,		                        
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  int frame = atoi(argv[1]);
  int leftMostFrame = -32000;
  crop* tempCrop = firstCrop;

  while(tempCrop)
  {
	  if(
		  (
		    tempCrop->leftFrame < leftMostFrame ||
		    (tempCrop->leftFrame != -32000 && leftMostFrame == -32000)
		  ) &&
		  (tempCrop->leftFrame <= frame)
		)
		  leftMostFrame = tempCrop->leftFrame;

	  tempCrop = tempCrop->nextCrop;
  }


  sprintf(result, "%d", leftMostFrame); 
  interp->result = result;
  return TCL_OK;
}
//--------------------------------------------------------------------------
int GetRightMostCropIncludingFrame_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                  
	     int argc,		                        
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  int frame = atoi(argv[1]);
  int rightMostFrame = -32000;
  crop* tempCrop = firstCrop;

  while(tempCrop)
  {
	  if(
		  (
		    tempCrop->rightFrame > rightMostFrame ||
		    (tempCrop->rightFrame != -32000 && rightMostFrame == -32000)
		  ) &&
 		  (tempCrop->rightFrame >= frame)
        )
		  rightMostFrame = tempCrop->rightFrame;

	  tempCrop = tempCrop->nextCrop;
  }


  sprintf(result, "%d", rightMostFrame); 
  interp->result = result;
  return TCL_OK;
}
//--------------------------------------------------------------------------
int DeleteAllCropsIncludingFrame_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                  
	     int argc,		                        
	     char *argv[])
{
  // if incorrect # of arguments, return error			
  if (argc !=2) 
    return ArgError(argv[0], interp);

  if(!playbackExists)
  {
    strcpy(result, "0");
    interp->result = result;
    return TCL_OK;
  }

  int frame = atoi(argv[1]);
  crop* tempCrop = firstCrop;

  while(tempCrop)
  {
      crop* nextCrop = tempCrop->nextCrop;

      if(tempCrop->leftFrame <= frame &&
		  tempCrop->rightFrame >= frame)
      {
		delete tempCrop;
	  }

      tempCrop = nextCrop;
  }

  sprintf(result, "ok");
  interp->result = result;
  return TCL_OK;
}
//--------------------------------------------------------------------------


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

  res = playbackSequenceInfo.leftSideLine;
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

  res = playbackSequenceInfo.rightSideLine;
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

  res = playbackSequenceInfo.ourGoalLine;
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

  res = playbackSequenceInfo.theirGoalLine;
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

  res = playbackSequenceInfo.goalDepth;
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

  res = playbackSequenceInfo.leftGoalPost;
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

  res = playbackSequenceInfo.rightGoalPost;
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

  res = playbackSequenceInfo.leftGoalieBox;
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

  res = playbackSequenceInfo.rightGoalieBox;

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

  res = playbackSequenceInfo.goalieBoxDepth;
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

  res = playbackSequenceInfo.deathZoneLine;
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

  res = playbackSequenceInfo.defenseZoneLine;
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

  res = playbackSequenceInfo.killZoneLine;
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

  res = playbackSequenceInfo.offensiveZoneLine;
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

  res = playbackSequenceInfo.leftMiddleSector;
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

  res = playbackSequenceInfo.rightMiddleSector;
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

  res = playbackSequenceInfo.leftSweetSector;
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

  res = playbackSequenceInfo.rightSweetSector;
  sprintf(result, "%f",res);
  interp->result = result;
  return TCL_OK;
}						
//==========================================================================
//==========================================================================
