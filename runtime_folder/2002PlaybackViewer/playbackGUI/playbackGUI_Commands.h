
//------------------------------
//Basic GUI_Commands definitions
//------------------------------
//Basic Tcl/Tk headers
#include "tcl.h"
#include "tk.h"

//Include basic GUI Error methods
#include "playbackGUI.h"

//Access all values
#include "../parser/file_reader.h"

#ifndef GUI_Commands_DEF
#define GUI_Commands_DEF

//==========================================================================
int toggleTeam_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                     
	     int argc,		                           
	     char *argv[]);
//==========================================================================

//==========================================================================
//############################
//#### Our robot messages ####
//############################
//==========================================================================
int GetOurRobotFound_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                     
	     int argc,		                           
	     char *argv[]);	                         
//--------------------------------------------------------------------------
int GetRobotPosition_cmd(ClientData clientData,
	     Tcl_Interp *interp,                     
	     int argc,		                           
	     char *argv[]);	                         
//==========================================================================
int GetRobotMessage_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                     
	     int argc,		                           
	     char *argv[]);	                         
//==========================================================================
int GetOurRobotX_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                
	     int argc,		                      
	     char *argv[]);	                    
//--------------------------------------------------------------------------
int GetOurRobotY_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                  
	     int argc,		                        
	     char *argv[]);	                      
//==========================================================================
int GetOurRobotRawX_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                
	     int argc,		                      
	     char *argv[]);	                    
//--------------------------------------------------------------------------
int GetOurRobotRawY_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                  
	     int argc,		                        
	     char *argv[]);	                      
//--------------------------------------------------------------------------
int GetOurRobotSentXVel_cmd(ClientData clientData,
	     Tcl_Interp *interp,                
	     int argc,		                      
	     char *argv[]);	                    
//--------------------------------------------------------------------------
int GetOurRobotSentYVel_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                     
	     int argc,		                           
	     char *argv[]);	                         
//--------------------------------------------------------------------------
int GetOurRobotRotation_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                
	     int argc,		                      
	     char *argv[]);	                    
//--------------------------------------------------------------------------
int GetOurRobotRawRotation_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                
	     int argc,		                      
	     char *argv[]);	                    
//--------------------------------------------------------------------------
int GetOurRobotSentRotation_cmd(ClientData clientData, 
	     Tcl_Interp *interp,               
	     int argc,		                     
	     char *argv[]);	      
//--------------------------------------------------------------------------
int GetRobotKick_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                   
	     int argc,		                         
	     char *argv[]);	                       
//==========================================================================
int GetRobotDribble_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                     
	     int argc,		                           
	     char *argv[]);	                         
//==========================================================================
int GetFriendlyRobotHasPossesion_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                     
	     int argc,		                           
	     char *argv[]);	                         
//==========================================================================
int GetOpponentRobotHasPossesion_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                     
	     int argc,		                           
	     char *argv[]);	                         
//==========================================================================
//##############################
//#### Their robot messages ####
//##############################
//==========================================================================
int GetTheirRobotFound_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                     
	     int argc,		                           
	     char *argv[]);	                         
//--------------------------------------------------------------------------
int GetTheirRobotX_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                    
	     int argc,		                          
	     char *argv[]);	                        
//--------------------------------------------------------------------------
int GetTheirRobotY_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                    
	     int argc,		                          
	     char *argv[]);	                        
//--------------------------------------------------------------------------
int GetTheirRobotRawX_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                    
	     int argc,		                          
	     char *argv[]);	                        
//--------------------------------------------------------------------------
int GetTheirRobotRawY_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                    
	     int argc,		                          
	     char *argv[]);	                        
//--------------------------------------------------------------------------
int GetTheirRobotXVel_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                     
	     int argc,		                           
	     char *argv[]);	                         
//--------------------------------------------------------------------------
int GetTheirRobotYVel_cmd(ClientData clientData,
	     Tcl_Interp *interp,                    
	     int argc,		                          
	     char *argv[]);	                        
//--------------------------------------------------------------------------
//#######################
//#### Ball messages ####
//#######################
//--------------------------------------------------------------------------
int GetRoboCupBallFound_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                     
	     int argc,		                           
	     char *argv[]);	                         
//===========================================================
int GetBallRawX_cmd(ClientData clientData, 
	     Tcl_Interp *interp,               
	     int argc,		                     
	     char *argv[]);	                   
//--------------------------------------------------------------------------
int GetBallRawY_cmd(ClientData clientData, 
	     Tcl_Interp *interp,              
	     int argc,		                    
	     char *argv[]);	                  
//===========================================================
int GetBallX_cmd(ClientData clientData, 
	     Tcl_Interp *interp,               
	     int argc,		                     
	     char *argv[]);	                   
//--------------------------------------------------------------------------
int GetBallY_cmd(ClientData clientData, 
	     Tcl_Interp *interp,              
	     int argc,		                    
	     char *argv[]);	                  
//--------------------------------------------------------------------------
int GetBallXVel_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                 
	     int argc,		                       
	     char *argv[]);	                     
//--------------------------------------------------------------------------
int GetBallYVel_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                 
	     int argc,		                       
	     char *argv[]);	                     
//==========================================================================
//#########################
//#### System messages ####
//#########################
//--------------------------------------------------------------------------
int GetCurrentFrame_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                    
	     int argc,		                          
	     char *argv[]);	                        
//==========================================================================
int GetCurrentPlayName_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                     
	     int argc,		                           
	     char *argv[]);	                         
//==========================================================================
//##########################
//#### Playback buttons ####
//##########################
//--------------------------------------------------------------------------
int ForwardFrame_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                    
	     int argc,		                          
	     char *argv[]);	                        
//==========================================================================
int BackwardFrame_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                    
	     int argc,		                          
	     char *argv[]);	                        
//==========================================================================
int SetFrame_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                    
	     int argc,		                          
	     char *argv[]);	                        
//==========================================================================
int NumberOfFrames_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                    
	     int argc,		                          
	     char *argv[]);	                        
//==========================================================================
int LoadFile_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                    
	     int argc,		                          
	     char *argv[]);	                        
//==========================================================================
int SaveFile_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                    
	     int argc,		                          
	     char *argv[]);	                        
//==========================================================================
int AppendFile_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                    
	     int argc,		                          
	     char *argv[]);	                        
//==========================================================================
int GUIMode_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                    
	     int argc,		                          
	     char *argv[]);	                        
//==========================================================================
int GetOurRobotDestinationX_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                
	     int argc,		                      
	     char *argv[]);    
//--------------------------------------------------------------------------
int GetOurRobotDestinationY_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                  
	     int argc,		                        
	     char *argv[]);
//--------------------------------------------------------------------------
int GetOurRobotDestinationRot_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                  
	     int argc,		                        
	     char *argv[]);
//==========================================================================
int GetOurRobotPassValue_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                
	     int argc,		                      
	     char *argv[]);    
//==========================================================================
int GetOurRobotPassDestinationX_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                
	     int argc,		                      
	     char *argv[]);    
//--------------------------------------------------------------------------
int GetOurRobotPassDestinationY_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                  
	     int argc,		                        
	     char *argv[]);

//--------------------------------------------------------------------------
int SetCropLeft_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                  
	     int argc,		                        
	     char *argv[]);
//--------------------------------------------------------------------------
int SetCropRight_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                  
	     int argc,		                        
	     char *argv[]);
//--------------------------------------------------------------------------
int SetCrop_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                  
	     int argc,		                        
	     char *argv[]);
//--------------------------------------------------------------------------
int GetLeftMostCropIncludingFrame_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                  
	     int argc,		                        
	     char *argv[]);
//--------------------------------------------------------------------------
int GetRightMostCropIncludingFrame_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                  
	     int argc,		                        
	     char *argv[]);
//--------------------------------------------------------------------------
int DeleteAllCropsIncludingFrame_cmd(ClientData clientData, 
	     Tcl_Interp *interp,                  
	     int argc,		                        
	     char *argv[]);
//--------------------------------------------------------------------------
//==========================================================================
//==========================================================================
int LEFT_SIDE_LINE_cmd(ClientData clientData, 
	                    Tcl_Interp *interp,               
                      int argc,		                     
                      char *argv[]);	                   
//==========================================================================
//==========================================================================
int RIGHT_SIDE_LINE_cmd(ClientData clientData, 
	                    Tcl_Interp *interp,               
                      int argc,		                     
                      char *argv[]);	                   
//==========================================================================
//==========================================================================
int OUR_GOAL_LINE_cmd(ClientData clientData, 
	                    Tcl_Interp *interp,               
                      int argc,		                     
                      char *argv[]);	                   
//==========================================================================
//==========================================================================
int THEIR_GOAL_LINE_cmd(ClientData clientData, 
	                    Tcl_Interp *interp,               
                      int argc,		                     
                      char *argv[]);	                   
//==========================================================================
//==========================================================================
int GOAL_DEPTH_cmd(ClientData clientData, 
	                    Tcl_Interp *interp,               
                      int argc,		                     
                      char *argv[]);	                   
//==========================================================================
//==========================================================================
int LEFT_GOAL_POST_cmd(ClientData clientData, 
	                    Tcl_Interp *interp,               
                      int argc,		                     
                      char *argv[]);	                   
//==========================================================================
//==========================================================================
int RIGHT_GOAL_POST_cmd(ClientData clientData, 
	                    Tcl_Interp *interp,               
                      int argc,		                     
                      char *argv[]);	                   
//==========================================================================
//==========================================================================
int LEFT_GOALIE_BOX_cmd(ClientData clientData, 
	                    Tcl_Interp *interp,               
                      int argc,		                     
                      char *argv[]);	                   
//==========================================================================
//==========================================================================
int RIGHT_GOALIE_BOX_cmd(ClientData clientData, 
	                    Tcl_Interp *interp,               
                      int argc,		                     
                      char *argv[]);	                   
//==========================================================================
//==========================================================================
int GOALIE_BOX_DEPTH_cmd(ClientData clientData, 
	                       Tcl_Interp *interp,               
                         int argc,		                     
                         char *argv[]);	                   
//==========================================================================
//==========================================================================
int DEATH_ZONE_LINE_cmd(ClientData clientData, 
	                    Tcl_Interp *interp,               
                        int argc,		                     
                        char *argv[]);	                   
//==========================================================================
//==========================================================================
int DEFENSE_ZONE_LINE_cmd(ClientData clientData, 
	                    Tcl_Interp *interp,               
                        int argc,		                     
                        char *argv[]);	                   
//==========================================================================
//==========================================================================
int KILL_ZONE_LINE_cmd(ClientData clientData, 
	                    Tcl_Interp *interp,               
                        int argc,		                     
                        char *argv[]);	                   
//==========================================================================
//==========================================================================
int OFFENSE_ZONE_LINE_cmd(ClientData clientData, 
	                    Tcl_Interp *interp,               
                        int argc,		                     
                        char *argv[]);	                   
//==========================================================================
//==========================================================================
int LEFT_MIDDLE_SECTOR_cmd(ClientData clientData, 
	                    Tcl_Interp *interp,               
                        int argc,		                     
                        char *argv[]);	                   
//==========================================================================
//==========================================================================
int RIGHT_MIDDLE_SECTOR_cmd(ClientData clientData, 
	                    Tcl_Interp *interp,               
                        int argc,		                     
                        char *argv[]);	                   
//==========================================================================
//==========================================================================
int LEFT_SWEET_SECTOR_cmd(ClientData clientData, 
	                    Tcl_Interp *interp,               
                        int argc,		                     
                        char *argv[]);	                   
//==========================================================================
//==========================================================================
int RIGHT_SWEET_SECTOR_cmd(ClientData clientData, 
	                    Tcl_Interp *interp,               
                        int argc,		                     
                        char *argv[]);	                   
//==========================================================================
//==========================================================================




#endif //GUI_Commands_DEF




