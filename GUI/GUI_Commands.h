//------------------------------
//Basic GUI_Commands definitions
//------------------------------
//Basic Tcl/Tk headers
#include "tcl.h"
#include "tk.h"

//Include basic GUI Error methods
#include "StandardModule.h"
#include "GUI.h"

#include "VisionModule.h"

//Access all values via the 
//standard module cleared in this file
#include "main.h"

#include "logger/playback_writer.h"

#ifndef GUI_Commands_DEF
#define GUI_Commands_DEF
//==========================================================================
//==========================================================================
int Default_Color_Scheme_cmd(ClientData clientData,
							 Tcl_Interp *interp,            
							 int argc,		                  
							 char *argv[]);	                
//==========================================================================
//==========================================================================
int Get_Complex_Setting_cmd(ClientData clientData,
       	                    Tcl_Interp *interp,            
							int argc,		                  
							char *argv[]);	                
//==========================================================================
//==========================================================================
int Toggle_AI_cmd(ClientData clientData,
	                Tcl_Interp *interp,            
                  int argc,		                  
                  char *argv[]);	                
//==========================================================================
//==========================================================================
int Set_AI_cmd(ClientData clientData, 
	             Tcl_Interp *interp,            
               int argc,		                  
               char *argv[]);	                
//==========================================================================
//==========================================================================
int Toggle_VISION_cmd(ClientData clientData,
	                    Tcl_Interp *interp,                 
                      int argc,		                      
                      char *argv[]);	                    
//==========================================================================
//==========================================================================
int Set_VISION_cmd(ClientData clientData, 
	                 Tcl_Interp *interp,                
                   int argc,		                      
                   char *argv[]);	                    
//==========================================================================
//==========================================================================
int Toggle_WIRELESS_cmd(ClientData clientData,
	                      Tcl_Interp *interp,                    
                        int argc,		                          
                        char *argv[]);	                        
//==========================================================================
//==========================================================================
int Set_WIRELESS_cmd(ClientData clientData, 
	                   Tcl_Interp *interp,                
                     int argc,		                      
                     char *argv[]);	                    
//==========================================================================
//==========================================================================
int GetRobotName_cmd(ClientData clientData, 
	                   Tcl_Interp *interp,                  
                     int argc,		                        
                     char *argv[]);	                      
//==========================================================================
//==========================================================================
int GetRobotType_cmd(ClientData clientData, 
	                   Tcl_Interp *interp,                    
                     int argc,		                          
                     char *argv[]);	                        
//==========================================================================
//==========================================================================
int SetRobotType_cmd(ClientData clientData, 
	                   Tcl_Interp *interp,                    
                     int argc,		                          
                     char *argv[]);	                        
//==========================================================================
//==========================================================================
int setAllType2000_cmd(ClientData clientData, 
	                   Tcl_Interp *interp,                    
                     int argc,		                          
                     char *argv[]);	                        
//==========================================================================
//==========================================================================
int setAllType2001_cmd(ClientData clientData, 
	                   Tcl_Interp *interp,                    
                     int argc,		                          
                     char *argv[]);	                        
//==========================================================================
//==========================================================================
int setAllType2002_cmd(ClientData clientData, 
	                   Tcl_Interp *interp,                    
                     int argc,		                          
                     char *argv[]);	                        
//==========================================================================
//==========================================================================
int GetRobotPosition_cmd(ClientData clientData, 
	                       Tcl_Interp *interp,                    
                         int argc,		                          
                         char *argv[]);	                        
//==========================================================================
//==========================================================================
int GetRobotMessage_cmd(ClientData clientData,
	                      Tcl_Interp *interp,                    
                        int argc,		                          
                        char *argv[]);	                        
//==========================================================================
//==========================================================================
int GetSystemStatus_cmd(ClientData clientData,
	                      Tcl_Interp *interp,                    
                        int argc,		                          
                        char *argv[]);	                        
//==========================================================================
//==========================================================================
int GetOurRobotX_cmd(ClientData clientData, 
	                   Tcl_Interp *interp,               
                     int argc,		                     
                     char *argv[]);	                   
//==========================================================================
//==========================================================================
int GetOurRobotY_cmd(ClientData clientData, 
	                   Tcl_Interp *interp,                     
                     int argc,		                           
                     char *argv[]);	                         
//==========================================================================
//==========================================================================
int GetOurRobotXVel_cmd(ClientData clientData, 
	                      Tcl_Interp *interp,               
                        int argc,		                     
                        char *argv[]);	                   
//==========================================================================
//==========================================================================
int GetOurRobotYVel_cmd(ClientData clientData, 
	                      Tcl_Interp *interp,                     
                        int argc,		                           
                        char *argv[]);	                         
//==========================================================================
//==========================================================================
int GetOurRobotRotation_cmd(ClientData clientData, 
	                          Tcl_Interp *interp,                
                            int argc,		                      
                            char *argv[]);	                    
//==========================================================================
//==========================================================================
int GetOurRobotSentXVel_cmd(ClientData clientData, 
	                          Tcl_Interp *interp,                     
                            int argc,		                           
                            char *argv[]);	                         
//==========================================================================
//==========================================================================
int GetOurRobotSentYVel_cmd(ClientData clientData, 
	                          Tcl_Interp *interp,                
                            int argc,		                      
                            char *argv[]);	                    
//==========================================================================
//==========================================================================
int GetOurRobotSentRotation_cmd(ClientData clientData, 
	                              Tcl_Interp *interp,                
                                int argc,		                      
                                char *argv[]);	                    
//==========================================================================
//==========================================================================
int GetTheirRobotX_cmd(ClientData clientData, 
	                     Tcl_Interp *interp,                    
                       int argc,		                          
                       char *argv[]);	                        
//==========================================================================
//==========================================================================
int GetTheirRobotY_cmd(ClientData clientData, 
	                     Tcl_Interp *interp,                    
                       int argc,		                          
                       char *argv[]);	                        
//==========================================================================
//==========================================================================
int GetTheirRobotXVel_cmd(ClientData clientData,
	                        Tcl_Interp *interp,                    
                          int argc,		                          
                          char *argv[]);	                        
//==========================================================================
//==========================================================================
int GetTheirRobotYVel_cmd(ClientData clientData,
	                        Tcl_Interp *interp,              
                          int argc,		                    
                          char *argv[]);	                  
//==========================================================================
//==========================================================================
int GetBallX_cmd(ClientData clientData, 
	               Tcl_Interp *interp,              
                 int argc,		                    
                 char *argv[]);	                  
//==========================================================================
//==========================================================================
int GetBallY_cmd(ClientData clientData, 
	               Tcl_Interp *interp,                 
                 int argc,		                       
                 char *argv[]);	                     
//==========================================================================
//==========================================================================
int GetBallRawX_cmd(ClientData clientData, 
	               Tcl_Interp *interp,              
                 int argc,		                    
                 char *argv[]);	                  
//==========================================================================
//==========================================================================
int GetBallRawY_cmd(ClientData clientData, 
	               Tcl_Interp *interp,                 
                 int argc,		                       
                 char *argv[]);	                     
//==========================================================================
//==========================================================================
int GetBallXVel_cmd(ClientData clientData, 
	                  Tcl_Interp *interp,                 
                    int argc,		                       
                    char *argv[]);	                     
//==========================================================================
//==========================================================================
int GetBallYVel_cmd(ClientData clientData, 
	                  Tcl_Interp *interp,                 
                    int argc,		                       
                    char *argv[]);	                     
//==========================================================================
//==========================================================================
int GetAIStatus_cmd(ClientData clientData, 
	                  Tcl_Interp *interp,                     
                    int argc,		                           
                    char *argv[]);	                         
//==========================================================================
//==========================================================================
int GetWirelessStatus_cmd(ClientData clientData, 
	                        Tcl_Interp *interp,                     
                          int argc,		                           
                          char *argv[]);	                         
//==========================================================================
//==========================================================================
int GetVisionStatus_cmd(ClientData clientData, 
	                      Tcl_Interp *interp,                   
                        int argc,		                         
                        char *argv[]);	                       
//==========================================================================
//==========================================================================
int GetSidesFlippedStatus_cmd(ClientData clientData, 
	                            Tcl_Interp *interp,                  
                              int argc,		                        
                              char *argv[]);	                      
//==========================================================================
//==========================================================================
int GetTeamNumber_cmd(ClientData clientData,
	                    Tcl_Interp *interp,                  
                      int argc,		                        
                      char *argv[]);	                      
//==========================================================================
//==========================================================================
int GetCurrentFrame_cmd(ClientData clientData, 
	                      Tcl_Interp *interp,                     
                        int argc,		                           
                        char *argv[]);	                         
//==========================================================================
//==========================================================================
int GetOurRobotFound_cmd(ClientData clientData,
	                       Tcl_Interp *interp,                     
                         int argc,		                           
                         char *argv[]);	                         
//==========================================================================
//==========================================================================
int GetOurRobotHavePossession_cmd(ClientData clientData,
	                                Tcl_Interp *interp,                     
          	      							  int argc,		                           
								                  char *argv[]);	                         
//==========================================================================
//==========================================================================
int GetTheirRobotHavePossession_cmd(ClientData clientData,
     	                              Tcl_Interp *interp,                     
                                    int argc,		                           
                                    char *argv[]);	                         
//==========================================================================
//==========================================================================
int GetTheirRobotFound_cmd(ClientData clientData, 
	                         Tcl_Interp *interp,                    
                           int argc,		                          
                           char *argv[]);	                        
//==========================================================================
//==========================================================================
int GetRoboCupBallFound_cmd(ClientData clientData, 
	                          Tcl_Interp *interp,                  
                            int argc,		                        
                            char *argv[]);	                      
//==========================================================================
//==========================================================================
int GetRobotKick_cmd(ClientData clientData, 
	                   Tcl_Interp *interp,                     
                     int argc,		                           
                     char *argv[]);	                         
//==========================================================================
//==========================================================================
int GetRobotDribble_cmd(ClientData clientData, 
	                      Tcl_Interp *interp,             
                        int argc,		                   
                        char *argv[]);	                 
//==========================================================================
//==========================================================================
int SetPlay_cmd(ClientData clientData, 
	              Tcl_Interp *interp,                    
                int argc,		                          
                char *argv[]);	                        
//==========================================================================
//==========================================================================
int GetCurrentPlayName_cmd(ClientData clientData,
	                         Tcl_Interp *interp,                  
                           int argc,		                        
                           char *argv[]);	                      
//==========================================================================
//==========================================================================
int CurrentPlayActive_cmd(ClientData clientData,
	                        Tcl_Interp *interp,                  
                          int argc,		                        
                          char *argv[]);	                      
//==========================================================================
//==========================================================================
int GetPlayName_cmd(ClientData clientData,  
	                  Tcl_Interp *interp,                
                    int argc,		                      
                    char *argv[]);	                    
//==========================================================================
//==========================================================================
int GetSetCmd_cmd(ClientData clientData, 
	                Tcl_Interp *interp,                    
                  int argc,		                          
                  char *argv[]);	                        
//==========================================================================
//==========================================================================
int GetSetPosition_cmd(ClientData clientData, 
	                     Tcl_Interp *interp,                     
                       int argc,		                           
                       char *argv[]);	                         
//==========================================================================
//==========================================================================
int Initialize2002Positions_cmd(ClientData clientData, 
	                             Tcl_Interp *interp,                     
                               int argc,		                           
                               char *argv[]);	                         
//==========================================================================
//==========================================================================
int SetRobotPosition_cmd(ClientData clientData,
	                       Tcl_Interp *interp,                     
                         int argc,		                           
                         char *argv[]);	                         
//==========================================================================
//==========================================================================
int GetPositionName_cmd(ClientData clientData, 
	                      Tcl_Interp *interp,                     
                        int argc,		                           
                        char *argv[]);                         
//==========================================================================
//==========================================================================
int GetNoPosition_cmd(ClientData clientData, 
	                      Tcl_Interp *interp,                     
                        int argc,		                           
                        char *argv[]);	                         
//==========================================================================
//==========================================================================
int GetNumPositions_cmd(ClientData clientData, 
	                      Tcl_Interp *interp,                     
                        int argc,		                           
                        char *argv[]);	                         
//==========================================================================
//==========================================================================
int ResetPositions_cmd(ClientData clientData, 
	                     Tcl_Interp *interp,                    
                       int argc,		                          
                       char *argv[]);	                        
//==========================================================================
//==========================================================================
int GetPositionName_cmd(ClientData clientData,
	                      Tcl_Interp *interp,                    
                        int argc,		                          
                        char *argv[]);                         
//==========================================================================
//==========================================================================
int GetNumRobots_cmd(ClientData clientData, 
	                   Tcl_Interp *interp,                  
                     int argc,		                        
                     char *argv[]);                       
//==========================================================================
//==========================================================================
int ToggleTeamNumber_cmd(ClientData clientData, 
	                       Tcl_Interp *interp,                     
                         int argc,		                           
                         char *argv[]);                        
//==========================================================================
//==========================================================================
int TogglePlayingSide_cmd(ClientData clientData, 
	                        Tcl_Interp *interp,                     
                          int argc,		                           
                          char *argv[]);                         
//==========================================================================
//==========================================================================
int ReloadParameters_cmd(ClientData clientData, 
	                       Tcl_Interp *interp,                     
                         int argc,		                           
                         char *argv[]);                        
//==========================================================================
//==========================================================================
int ReloadTrajectoryParameters_cmd(ClientData clientData, 
	                       Tcl_Interp *interp,                     
                         int argc,		                           
                         char *argv[]);                        
//==========================================================================
//==========================================================================
int SaveLog_cmd(ClientData clientData, 
	              Tcl_Interp *interp,             
                int argc,		                   
                char *argv[]);                  
//==========================================================================
//==========================================================================
int SaveLogByName_cmd(ClientData clientData, 
	                    Tcl_Interp *interp,                   
                      int argc,		                         
                      char *argv[]);                        
//==========================================================================
//==========================================================================
int ShutDown_cmd(ClientData clientData, 
	               Tcl_Interp *interp,              
                 int argc,		                    
                 char *argv[]);                   
//==========================================================================
//==========================================================================
int RunLatencyTest_cmd(ClientData clientData, 
	                     Tcl_Interp *interp,                    
                       int argc,		                          
                       char *argv[]);                         
//==========================================================================
//==========================================================================
int IsLatencyTestDone_cmd(ClientData clientData, 
	                        Tcl_Interp *interp,                     
                          int argc,		                           
                          char *argv[]);                         
//==========================================================================
//==========================================================================
int GetMinLatency_cmd(ClientData clientData, 
	                    Tcl_Interp *interp,                   
                      int argc,		                         
                      char *argv[]);                        
//==========================================================================
//==========================================================================
int GetMaxLatency_cmd(ClientData clientData, 
	                    Tcl_Interp *interp,                   
                      int argc,		                         
                      char *argv[]);                        
//==========================================================================
//==========================================================================
int GetMeanLatency_cmd(ClientData clientData, 
	                     Tcl_Interp *interp,                    
                       int argc,		                          
                       char *argv[]);                         
//==========================================================================
//==========================================================================
int GetFrameRate_cmd(ClientData clientData, 
	                   Tcl_Interp *interp,                  
                     int argc,		                        
                     char *argv[]);                       
//==========================================================================
//==========================================================================
int SampleFrameRate_cmd(ClientData clientData, 
	                      Tcl_Interp *interp,                     
                        int argc,		                           
                        char *argv[]);                         
//==========================================================================
//==========================================================================
int GetNoPlay_cmd(ClientData clientData,
	              Tcl_Interp *interp,                     
                  int argc,		                           
                  char *argv[]);                         
//==========================================================================
//==========================================================================
int GetFirst2000Play_cmd(ClientData clientData,
	                     Tcl_Interp *interp,                     
                         int argc,		                           
                         char *argv[]);                         
//==========================================================================
//==========================================================================
int GetLast2000Play_cmd(ClientData clientData,
	                     Tcl_Interp *interp,                     
                         int argc,		                           
                         char *argv[]);                         
//==========================================================================
//==========================================================================
int GetFirst2002Play_cmd(ClientData clientData,
	                     Tcl_Interp *interp,                     
                         int argc,		                           
                         char *argv[]);                         
//==========================================================================
//==========================================================================
int GetLast2002Play_cmd(ClientData clientData,
	                     Tcl_Interp *interp,                     
                         int argc,		                           
                         char *argv[]);                         
//==========================================================================
//==========================================================================
int GetFirst2002PenaltyPlay_cmd(ClientData clientData,
	                     Tcl_Interp *interp,                     
                         int argc,		                           
                         char *argv[]);                         
//==========================================================================
//==========================================================================
int GetLast2002PenaltyPlay_cmd(ClientData clientData,
	                     Tcl_Interp *interp,                     
                         int argc,		                           
                         char *argv[]);                         
//==========================================================================
//==========================================================================
int GetFirstFormationPlay_cmd(ClientData clientData,
	                        Tcl_Interp *interp,                     
                          int argc,		                           
                          char *argv[]);                         
//==========================================================================
//==========================================================================
int GetLastFormationPlay_cmd(ClientData clientData,
	                        Tcl_Interp *interp,                     
                          int argc,		                           
                          char *argv[]);                         
//==========================================================================
//==========================================================================
int GetFirstEvanKuhnSkillTestPlay_cmd(ClientData clientData,
	                      Tcl_Interp *interp,                   
                        int argc,		                         
                        char *argv[]);                        
//==========================================================================
//==========================================================================
int GetLastEvanKuhnSkillTestPlay_cmd(ClientData clientData,
	                      Tcl_Interp *interp,                   
                        int argc,		                         
                        char *argv[]);                        
//==========================================================================
//==========================================================================
int GetFirstEvanMaloneSkillTestPlay_cmd(ClientData clientData,
	                      Tcl_Interp *interp,                   
                        int argc,		                         
                        char *argv[]);                        
//==========================================================================
//==========================================================================
int GetLastEvanMaloneSkillTestPlay_cmd(ClientData clientData,
	                      Tcl_Interp *interp,                   
                        int argc,		                         
                        char *argv[]);                        
//==========================================================================
//==========================================================================
int GetFirstJoelSkillTestPlay_cmd(ClientData clientData,
	                      Tcl_Interp *interp,                   
                        int argc,		                         
                        char *argv[]);                        
//==========================================================================
//==========================================================================
int GetLastJoelSkillTestPlay_cmd(ClientData clientData,
	                      Tcl_Interp *interp,                   
                        int argc,		                         
                        char *argv[]);                        
//==========================================================================
//==========================================================================
int GetFirstNiravSkillTestPlay_cmd(ClientData clientData,
	                      Tcl_Interp *interp,                   
                        int argc,		                         
                        char *argv[]);                        
//==========================================================================
//==========================================================================
int GetLastNiravSkillTestPlay_cmd(ClientData clientData,
	                      Tcl_Interp *interp,                   
                        int argc,		                         
                        char *argv[]);                        
//==========================================================================
//==========================================================================
int GetFirstWillSkillTestPlay_cmd(ClientData clientData,
	                      Tcl_Interp *interp,                   
                        int argc,		                         
                        char *argv[]);                        
//==========================================================================
//==========================================================================
int GetLastWillSkillTestPlay_cmd(ClientData clientData,
	                      Tcl_Interp *interp,                   
                        int argc,		                         
                        char *argv[]);                        
//==========================================================================
//==========================================================================
int GetFirstIndividualPlay_cmd(ClientData clientData,
	                      Tcl_Interp *interp,                   
                        int argc,		                         
                        char *argv[]);                        
//==========================================================================
//==========================================================================
int GetLastIndividualPlay_cmd(ClientData clientData,
	                      Tcl_Interp *interp,                   
                        int argc,		                         
                        char *argv[]);                        
//==========================================================================
//==========================================================================
int GetFirstIntimidationPlay_cmd(ClientData clientData,
	                      Tcl_Interp *interp,                   
                        int argc,		                         
                        char *argv[]);                        
//==========================================================================
//==========================================================================
int GetLastIntimidationPlay_cmd(ClientData clientData,
	                      Tcl_Interp *interp,                   
                        int argc,		                         
                        char *argv[]);                        
//==========================================================================
//==========================================================================
int GetFirstOtherPlay_cmd(ClientData clientData,
	                      Tcl_Interp *interp,                   
                        int argc,		                         
                        char *argv[]);                        
//==========================================================================
//==========================================================================
int GetLastOtherPlay_cmd(ClientData clientData,
	                      Tcl_Interp *interp,                   
                        int argc,		                         
                        char *argv[]);                        
//==========================================================================
//==========================================================================
int GetLatencyPlay_cmd(ClientData clientData, 
	                     Tcl_Interp *interp,                    
                       int argc,		                          
                       char *argv[]);                         
//==========================================================================
//==========================================================================
int GetPenaltyShotUsFormationPlay_cmd(ClientData clientData, 
	                                    Tcl_Interp *interp,                    
                                      int argc,		                          
                                      char *argv[]);                         
//==========================================================================
//==========================================================================
int GetNumPlays_cmd(ClientData clientData, 
	                  Tcl_Interp *interp,                 
                    int argc,		                       
                    char *argv[]);	                     
//==========================================================================
//==========================================================================
int GetOurRobotDestinationX_cmd(ClientData clientData, 
	                              Tcl_Interp *interp,                
                                int argc,		                      
                                char *argv[]);    
//==========================================================================
//==========================================================================
int GetOurRobotDestinationY_cmd(ClientData clientData, 
	                              Tcl_Interp *interp,                  
                                int argc,		                        
                                char *argv[]);
//==========================================================================
//==========================================================================
int GetOurRobotDestinationRot_cmd(ClientData clientData, 
	                              Tcl_Interp *interp,                  
                                int argc,		                        
                                char *argv[]);
//==========================================================================
//==========================================================================
int GetOurRobotPassValue_cmd(ClientData clientData, 
	                           Tcl_Interp *interp,                
                             int argc,		                      
                             char *argv[]);    
//==========================================================================
//==========================================================================
int GetOurRobotPassDestinationX_cmd(ClientData clientData, 
	                                  Tcl_Interp *interp,                
	                                  int argc,		                      
	                                  char *argv[]);    
//==========================================================================
//==========================================================================
int GetOurRobotPassDestinationY_cmd(ClientData clientData, 
                              	    Tcl_Interp *interp,                  
	                                  int argc,		                        
	                                  char *argv[]);
//==========================================================================
//==========================================================================
int SetRobotLocation_cmd(ClientData clientData,
                         Tcl_Interp *interp,
                         int argc,
                         char *argv[]);
//==========================================================================
//==========================================================================
int SetRobotRotation_cmd(ClientData clientData,
                         Tcl_Interp *interp,
                         int argc,
                         char *argv[]);
//==========================================================================
//==========================================================================
int getDebugPointX(ClientData clientData, 
	               Tcl_Interp *interp,               
                   int argc,		                     
                   char *argv[]);	                   
//==========================================================================
//==========================================================================
int getDebugPointY(ClientData clientData, 
	               Tcl_Interp *interp,               
                   int argc,		                     
                   char *argv[]);	                   
//==========================================================================
//==========================================================================
int getNumDebugPoints(ClientData clientData, 
	                    Tcl_Interp *interp,               
                      int argc,		                     
                      char *argv[]);	                   
//==========================================================================
//==========================================================================
int getNumDebugMessages_cmd(ClientData clientData, 
	                    Tcl_Interp *interp,               
                      int argc,		                     
                      char *argv[]);	                   
//==========================================================================
//==========================================================================
int getDebugMessage(ClientData clientData, 
	                    Tcl_Interp *interp,               
                      int argc,		                     
                      char *argv[]);	                   
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
int GOALIEBOX_DEPTH_cmd(ClientData clientData, 
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
int CENTER_CIRCLE_RADIUS_cmd(ClientData clientData, 
	                       Tcl_Interp *interp,               
                         int argc,		                     
                         char *argv[]);	                   
//==========================================================================
//==========================================================================
int FREE_KICK_X_OFFSET_cmd(ClientData clientData, 
	                       Tcl_Interp *interp,               
                         int argc,		                     
                         char *argv[]);	                   
//==========================================================================
//==========================================================================
int FREE_KICK_Y_OFFSET_cmd(ClientData clientData, 
	                       Tcl_Interp *interp,               
                         int argc,		                     
                         char *argv[]);	                   
//==========================================================================
//==========================================================================
int PENALTY_KICK_X_OFFSET_cmd(ClientData clientData, 
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




//==========================================================================
//System Quality Test Commaands
//-----------------------------------------------------
int get_VISION_EXECUTE_TIME(ClientData clientData, 
	                          Tcl_Interp *interp,               
                            int argc,		                     
                            char *argv[]);	                   
//-----------------------------------------------------
int get_VISION_GET_TIME(ClientData clientData, 
	                          Tcl_Interp *interp,               
                            int argc,		                     
                            char *argv[]);	                   
//-----------------------------------------------------
int get_WIRELESS_EXECUTE_TIME(ClientData clientData, 
	                          Tcl_Interp *interp,               
                            int argc,		                     
                            char *argv[]);	                   
//-----------------------------------------------------
int get_SIMULATOR_EXECUTE_TIME(ClientData clientData, 
	                             Tcl_Interp *interp,               
                               int argc,		                     
                               char *argv[]);	                   
//-----------------------------------------------------
int get_STRATEGY_EXECUTE_TIME(ClientData clientData, 
	                          Tcl_Interp *interp,               
                            int argc,		                     
                            char *argv[]);	                   
//-----------------------------------------------------
int get_TRAJECTORY_EXECUTE_TIME(ClientData clientData, 
	                          Tcl_Interp *interp,               
                            int argc,		                     
                            char *argv[]);	                   
//-----------------------------------------------------
int get_TOTAL_EXECUTE_TIME(ClientData clientData, 
	                          Tcl_Interp *interp,               
                            int argc,		                     
                            char *argv[]);	                   
//-----------------------------------------------------
int get_AI_FPS(ClientData clientData, 
	             Tcl_Interp *interp,               
               int argc,		                     
               char *argv[]);	                   
//-----------------------------------------------------
int get_VISION_FPS(ClientData clientData, 
	                 Tcl_Interp *interp,               
                   int argc,		                     
                   char *argv[]);	                   
//-----------------------------------------------------
int TogglePositionSwitching_cmd(ClientData clientData, 
	                              Tcl_Interp *interp,               
                                int argc,		                     
                                char *argv[]);	                   
//-----------------------------------------------------
int TogglePlayTransitions_cmd(ClientData clientData, 
	                            Tcl_Interp *interp,               
                              int argc,		                     
                              char *argv[]);	                   
//-----------------------------------------------------
int RESET_GOALIE_BOX_DEPTH(ClientData clientData, 
	                            Tcl_Interp *interp,               
                              int argc,		                     
                              char *argv[]);	                   
//-----------------------------------------------------
int RESET_OUR_GOAL_LINE(ClientData clientData, 
	                            Tcl_Interp *interp,               
                              int argc,		                     
                              char *argv[]);	                   
//-----------------------------------------------------
int RESET_THEIR_GOAL_LINE(ClientData clientData, 
	                            Tcl_Interp *interp,               
                              int argc,		                     
                              char *argv[]);	                   
//-----------------------------------------------------
int RESET_OUR_LEFT_GOAL_WALL(ClientData clientData, 
	                            Tcl_Interp *interp,               
                              int argc,		                     
                              char *argv[]);	                   
//-----------------------------------------------------
int RESET_OUR_RIGHT_GOAL_WALL(ClientData clientData, 
	                            Tcl_Interp *interp,               
                              int argc,		                     
                              char *argv[]);	                   
//-----------------------------------------------------
int RESET_THEIR_LEFT_GOAL_WALL(ClientData clientData, 
	                            Tcl_Interp *interp,               
                              int argc,		                     
                              char *argv[]);	                   
//-----------------------------------------------------
int RESET_THEIR_RIGHT_GOAL_WALL(ClientData clientData, 
	                            Tcl_Interp *interp,               
                              int argc,		                     
                              char *argv[]);	                   
//-----------------------------------------------------
int RESET_RIGHT_SIDE_LINE(ClientData clientData, 
	                            Tcl_Interp *interp,               
                              int argc,		                     
                              char *argv[]);	                   
//-----------------------------------------------------
int RESET_RIGHT_GOALIE_BOX(ClientData clientData, 
	                            Tcl_Interp *interp,               
                              int argc,		                     
                              char *argv[]);	                   
//-----------------------------------------------------
int RESET_RIGHT_GOAL_POST(ClientData clientData, 
	                            Tcl_Interp *interp,               
                              int argc,		                     
                              char *argv[]);	                   
//-----------------------------------------------------
int RESET_LEFT_GOAL_POST(ClientData clientData, 
	                            Tcl_Interp *interp,               
                              int argc,		                     
                              char *argv[]);	                   
//-----------------------------------------------------
int RESET_LEFT_GOALIE_BOX(ClientData clientData, 
	                            Tcl_Interp *interp,               
                              int argc,		                     
                              char *argv[]);	                   
//-----------------------------------------------------
int RESET_LEFT_SIDE_LINE(ClientData clientData, 
	                            Tcl_Interp *interp,               
                              int argc,		                     
                              char *argv[]);
//-----------------------------------------------------
int RESET_OUR_LEFT_CORNER(ClientData clientData, 
	                        Tcl_Interp *interp,               
                          int argc,		                     
                          char *argv[]);
//-----------------------------------------------------
int RESET_OUR_RIGHT_CORNER(ClientData clientData, 
	                         Tcl_Interp *interp,               
                           int argc,		                     
                           char *argv[]);
//-----------------------------------------------------
int RESET_THEIR_LEFT_CORNER(ClientData clientData, 
	                          Tcl_Interp *interp,               
                            int argc,		                     
                            char *argv[]);
//-----------------------------------------------------
int RESET_THEIR_RIGHT_CORNER(ClientData clientData, 
	                           Tcl_Interp *interp,               
                             int argc,		                     
                             char *argv[]);
//-----------------------------------------------------
int RESET_CENTER(ClientData clientData, 
	               Tcl_Interp *interp,               
                 int argc,		                     
                 char *argv[]);
//-----------------------------------------------------
int SAVE_FIELD_PARAMETERS(ClientData clientData, 
	                        Tcl_Interp *interp,               
                          int argc,		                     
                          char *argv[]);	                   
//-----------------------------------------------------
#endif //GUI_Commands_DEF

