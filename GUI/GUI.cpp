//Declare GUI information before defining it
#include "GUI.h"
#include "GUI_Internal_Includes.h"
#ifdef __cplusplus
extern "C" {
#endif

//Create a skillz diary of all GUI commands.
GUI_Diary GUI_Record;

//This method launches GUI
DWORD WINAPI launchGUI(LPVOID unused)
{
  //Initialize diary so we can record GUI actions (like clicking buttons)
  GUI_Record.reset();

  //Start the actual GUI!
  char* argv[3] = {"robocupAI", "GUI.tcl", 0};
  Tk_Main(2, argv, Tcl_AppInit);
  return 0;
}


//==========================================
//==========================================
//==========================================
int Tcl_AppInit(Tcl_Interp *interp)
{
  //-------------------------------
  //Initialize the Tcl/Tk libraries
  //-------------------------------
  if (Tcl_Init(interp) == TCL_ERROR) 
    return TCL_ERROR;

  if (Tk_Init(interp) == TCL_ERROR)
    return TCL_ERROR;

  //---------------------------------------
 Tcl_StaticPackage(interp, "Tk", Tk_Init, (Tcl_PackageInitProc *) NULL);

  //---------------------------------------
  //Execute the user defined start up script.
  //---------------------------------------

  //---------------------------------------------
  Tcl_CreateCommand(interp, 
                    "Default_Color_Scheme_cmd", 
                    Default_Color_Scheme_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  Tcl_CreateCommand(interp, 
                    "Get_Complex_Setting_cmd", 
                    Get_Complex_Setting_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  //Add command to toggle AI status
  Tcl_CreateCommand(interp, 
                    "Toggle_AI_cmd", 
                    Toggle_AI_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  //Add command to set AI status
  Tcl_CreateCommand(interp, 
                    "Set_AI_cmd", 
                    Set_AI_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  //Add command to set WIRELESS status
  Tcl_CreateCommand(interp, 
                    "Set_WIRELESS_cmd", 
                    Set_WIRELESS_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  //Add command to set VISION status
  Tcl_CreateCommand(interp, 
                    "Set_VISION_cmd", 
                    Set_VISION_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  //Add command to toggle Vision status
  Tcl_CreateCommand(interp, 
                    "Toggle_VISION_cmd", 
                    Toggle_VISION_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  //Add command to toggle Wireless status
  Tcl_CreateCommand(interp, 
                    "Toggle_WIRELESS_cmd", 
                    Toggle_WIRELESS_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  //---------------------------------------------
 Tcl_CreateCommand(interp, 
                   "GetRobotType_cmd", 
                   GetRobotType_cmd,
                   (ClientData) NULL, 
                   (Tcl_CmdDeleteProc *) NULL);
 //---------------------------------------------
 Tcl_CreateCommand(interp, 
                   "SetRobotType_cmd", 
                   SetRobotType_cmd,
                   (ClientData) NULL, 
                   (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
 Tcl_CreateCommand(interp, 
                   "setAllType2000_cmd", 
                   setAllType2000_cmd,
                   (ClientData) NULL, 
                   (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  Tcl_CreateCommand(interp, 
                    "setAllType2001_cmd", 
                    setAllType2001_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  Tcl_CreateCommand(interp, 
                    "setAllType2002_cmd", 
                    setAllType2002_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  //Add command to get a robots position
  Tcl_CreateCommand(interp, 
                    "GetRobotPosition_cmd", 
                    GetRobotPosition_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  //Add command to get a robots name
  Tcl_CreateCommand(interp, 
                    "GetRobotName_cmd", 
                    GetRobotName_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  //Add command to get a robots debug message
  Tcl_CreateCommand(interp, 
                    "GetRobotMessage_cmd", 
                    GetRobotMessage_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  //Add command to get system debug message
  Tcl_CreateCommand(interp, 
                    "GetSystemStatus_cmd", 
                    GetSystemStatus_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  //Add command to get a robots x position
  Tcl_CreateCommand(interp, 
                    "GetOurRobotX_cmd", 
                    GetOurRobotX_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  //Add command to get a robots y position
  Tcl_CreateCommand(interp, 
                    "GetOurRobotY_cmd", 
                    GetOurRobotY_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  //Add command to get a robots x velocity
  Tcl_CreateCommand(interp, 
                    "GetOurRobotXVel_cmd", 
                    GetOurRobotXVel_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  //Add command to get a robots y velocity
  Tcl_CreateCommand(interp, 
                    "GetOurRobotYVel_cmd", 
                    GetOurRobotYVel_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //--------------------------------------------- 
  //Add command to get a robots rotation
  Tcl_CreateCommand(interp, 
                    "GetOurRobotRotation_cmd", 
                    GetOurRobotRotation_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  //Add command to get a robots last sent x velocity
  Tcl_CreateCommand(interp, 
                    "GetOurRobotSentXVel_cmd", 
                    GetOurRobotSentXVel_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  //Add command to get a robots last sent y velocity
  Tcl_CreateCommand(interp, 
                    "GetOurRobotSentYVel_cmd", 
                    GetOurRobotSentYVel_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  //Add command to get a robots last sent y velocity
  Tcl_CreateCommand(interp, 
                    "GetOurRobotSentRotation_cmd", 
                    GetOurRobotSentRotation_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  //Add command to get their robots x position
  Tcl_CreateCommand(interp, 
                    "GetTheirRobotX_cmd", 
                    GetTheirRobotX_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  //Add command to get their robots y position
  Tcl_CreateCommand(interp, 
                    "GetTheirRobotY_cmd", 
                    GetTheirRobotY_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //--------------------------------------------- 
  //Add command to get their robots x velocity
  Tcl_CreateCommand(interp, 
                    "GetTheirRobotXVel_cmd", 
                    GetTheirRobotXVel_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  //Add command to get their robots y velocity
  Tcl_CreateCommand(interp, 
                    "GetTheirRobotYVel_cmd", 
                    GetTheirRobotYVel_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
 //Add command to get ball's x position
 Tcl_CreateCommand(interp, 
                   "GetBallX_cmd", 
                   GetBallX_cmd,
                   (ClientData) NULL, 
                   (Tcl_CmdDeleteProc *) NULL);
 //---------------------------------------------
 //Add command to get ball's y position
 Tcl_CreateCommand(interp, 
                   "GetBallY_cmd", 
                   GetBallY_cmd,
                   (ClientData) NULL, 
                   (Tcl_CmdDeleteProc *) NULL);
 //---------------------------------------------
  //Add command to get ball's raw x position
  Tcl_CreateCommand(interp, 
                    "GetBallRawX_cmd", 
                    GetBallRawX_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  //Add command to get ball's raw y position
  Tcl_CreateCommand(interp, 
                    "GetBallRawY_cmd", 
                    GetBallRawY_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  //Add command to get balls x velocity
  Tcl_CreateCommand(interp, 
                    "GetBallXVel_cmd", 
                    GetBallXVel_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  //Add command to get balls y velocity
  Tcl_CreateCommand(interp, 
                    "GetBallYVel_cmd", 
                    GetBallYVel_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Add command to get balls y velocity
  Tcl_CreateCommand(interp, 
                    "GetAIStatus_cmd", 
                    GetAIStatus_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Add command to get balls y velocity
  Tcl_CreateCommand(interp, 
                    "GetWirelessStatus_cmd", 
                    GetWirelessStatus_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Add command to get balls y velocity
  Tcl_CreateCommand(interp, 
                    "GetVisionStatus_cmd", 
                    GetVisionStatus_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Add command to get balls y velocity
  Tcl_CreateCommand(interp, 
                    "GetSidesFlippedStatus_cmd", 
                    GetSidesFlippedStatus_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Add command to get balls y velocity
  Tcl_CreateCommand(interp, 
                    "GetTeamNumber_cmd", 
                    GetTeamNumber_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Add command to get balls y velocity
  Tcl_CreateCommand(interp, 
                    "GetCurrentFrame_cmd", 
                    GetCurrentFrame_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Add command to find if our robot is found
  Tcl_CreateCommand(interp, 
                    "GetOurRobotFound_cmd", 
                    GetOurRobotFound_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Add command to find if our robot is found
  Tcl_CreateCommand(interp, 
                    "GetOurRobotHavePossession_cmd", 
                    GetOurRobotHavePossession_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Add command to find if our robot is found
  Tcl_CreateCommand(interp, 
                    "GetTheirRobotHavePossession_cmd", 
                    GetTheirRobotHavePossession_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Add command to find if their ball is found
  Tcl_CreateCommand(interp, 
                    "GetTheirRobotFound_cmd", 
                    GetTheirRobotFound_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Add command to find if a robocup ball is found
  Tcl_CreateCommand(interp, 
                    "GetRoboCupBallFound_cmd", 
                    GetRoboCupBallFound_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Add command to find if a robot is kicking
  Tcl_CreateCommand(interp, 
                    "GetRobotKick_cmd", 
                    GetRobotKick_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Add command to find if a robot is dribbling
  Tcl_CreateCommand(interp, 
                    "GetRobotDribble_cmd", 
                    GetRobotDribble_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Add command to set the play
  Tcl_CreateCommand(interp, 
                    "SetPlay_cmd", 
                    SetPlay_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Add command to get the current play name
  Tcl_CreateCommand(interp, 
                    "GetCurrentPlayName_cmd", 
                    GetCurrentPlayName_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Returns true if the current play is an active play state
  Tcl_CreateCommand(interp, 
                    "CurrentPlayActive_cmd", 
                    CurrentPlayActive_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Add command to get the name of a play
  Tcl_CreateCommand(interp, 
                    "GetPlayName_cmd", 
                    GetPlayName_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Hack to get the string "SetPlay_cmd X"
  Tcl_CreateCommand(interp, 
                    "GetSetCmd_cmd", 
                    GetSetCmd_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Hack to get the string "SetPlay_cmd X"
  Tcl_CreateCommand(interp, 
                    "GetSetPosition_cmd", 
                    GetSetPosition_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Initialize2002Positions
  Tcl_CreateCommand(interp, 
                    "Initialize2002Positions_cmd", 
                    Initialize2002Positions_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Set a robots position
  Tcl_CreateCommand(interp, 
                    "SetRobotPosition_cmd", 
                    SetRobotPosition_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Reset all positions
  Tcl_CreateCommand(interp, 
                    "ResetPositions_cmd", 
                    ResetPositions_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Get the name of a certain position
  Tcl_CreateCommand(interp, 
                    "GetPositionName_cmd", 
                    GetPositionName_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Get the number of Positions
  Tcl_CreateCommand(interp, 
                    "GetNoPosition_cmd", 
                    GetNoPosition_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Get the number of Positions
  Tcl_CreateCommand(interp, 
                    "GetNumPositions_cmd", 
                    GetNumPositions_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Get the number of Robots
  Tcl_CreateCommand(interp, 
                    "GetNumRobots_cmd", 
                    GetNumRobots_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Toggle Team Number
  Tcl_CreateCommand(interp, 
                    "ToggleTeamNumber_cmd", 
                    ToggleTeamNumber_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Toggle Playing Side
  Tcl_CreateCommand(interp, 
                    "TogglePlayingSide_cmd", 
                    TogglePlayingSide_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Reload ALL Parameter Files
  Tcl_CreateCommand(interp, 
                    "ReloadParameters_cmd", 
                    ReloadParameters_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Reload Trajectory Parameter File
  Tcl_CreateCommand(interp, 
                    "ReloadTrajectoryParameters_cmd", 
                    ReloadTrajectoryParameters_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Save Log File
  Tcl_CreateCommand(interp, 
                    "SaveLog_cmd", 
                    SaveLog_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Save Log File
  Tcl_CreateCommand(interp, 
                    "SaveLogByName_cmd", 
                    SaveLogByName_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Shut down system (close sockets, etc.)
  Tcl_CreateCommand(interp, 
                    "ShutDown_cmd", 
                    ShutDown_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Run a latency test...
  Tcl_CreateCommand(interp, 
                    "RunLatencyTest_cmd", 
                    RunLatencyTest_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Run a latency test...
  Tcl_CreateCommand(interp, 
                    "IsLatencyTestDone_cmd", 
                    IsLatencyTestDone_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Get min latency of system
  Tcl_CreateCommand(interp, 
                    "GetMinLatency_cmd", 
                    GetMinLatency_cmd,
                   (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Get max latency of system
  Tcl_CreateCommand(interp, 
                    "GetMaxLatency_cmd", 
                    GetMaxLatency_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Get mean latency of system
  Tcl_CreateCommand(interp, 
                    "GetMeanLatency_cmd", 
                    GetMeanLatency_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Get mean latency of system
  Tcl_CreateCommand(interp, 
                    "GetFrameRate_cmd", 
                    GetFrameRate_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Get mean latency of system
  Tcl_CreateCommand(interp, 
                    "SampleFrameRate_cmd", 
                    SampleFrameRate_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "GetNoPlay_cmd", 
                    GetNoPlay_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "GetFirst2002Play_cmd", 
                    GetFirst2002Play_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "GetLast2002Play_cmd", 
                    GetLast2002Play_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "GetFirst2002PenaltyPlay_cmd", 
                    GetFirst2002PenaltyPlay_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "GetLast2002PenaltyPlay_cmd", 
                    GetLast2002PenaltyPlay_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Add commands to get certain play indeces
  Tcl_CreateCommand(interp, 
                    "GetFirstFormationPlay_cmd", 
                    GetFirstFormationPlay_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Add commands to get certain play indeces
  Tcl_CreateCommand(interp, 
                    "GetLastFormationPlay_cmd", 
                    GetLastFormationPlay_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
 Tcl_CreateCommand(interp, 
                   "GetFirstEvanMaloneSkillTestPlay_cmd", 
                   GetFirstEvanMaloneSkillTestPlay_cmd,
                   (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "GetLastEvanMaloneSkillTestPlay_cmd", 
                    GetLastEvanMaloneSkillTestPlay_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "GetFirstEvanKuhnSkillTestPlay_cmd", 
                    GetFirstEvanKuhnSkillTestPlay_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "GetLastEvanKuhnSkillTestPlay_cmd", 
                    GetLastEvanKuhnSkillTestPlay_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "GetFirstJoelSkillTestPlay_cmd", 
                    GetFirstJoelSkillTestPlay_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "GetLastJoelSkillTestPlay_cmd", 
                    GetLastJoelSkillTestPlay_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "GetFirstNiravSkillTestPlay_cmd", 
                    GetFirstNiravSkillTestPlay_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "GetLastNiravSkillTestPlay_cmd", 
                    GetLastNiravSkillTestPlay_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "GetFirstWillSkillTestPlay_cmd", 
                    GetFirstWillSkillTestPlay_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "GetLastWillSkillTestPlay_cmd", 
                    GetLastWillSkillTestPlay_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "GetFirstIndividualPlay_cmd", 
                    GetFirstIndividualPlay_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "GetLastIndividualPlay_cmd", 
                    GetLastIndividualPlay_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "GetFirstIntimidationPlay_cmd", 
                    GetFirstIntimidationPlay_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "GetLastIntimidationPlay_cmd", 
                    GetLastIntimidationPlay_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "GetFirstOtherPlay_cmd", 
                    GetFirstOtherPlay_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "GetLastOtherPlay_cmd", 
                    GetLastOtherPlay_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "GetLatencyPlay_cmd", 
                    GetLatencyPlay_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "GetPenaltyShotUsFormationPlay_cmd", 
                    GetPenaltyShotUsFormationPlay_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "GetNumPlays_cmd", 
                    GetNumPlays_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "GetOurRobotDestinationX_cmd", 
                    GetOurRobotDestinationX_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "GetOurRobotDestinationY_cmd", 
                    GetOurRobotDestinationY_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "GetOurRobotDestinationRot_cmd", 
                    GetOurRobotDestinationRot_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
//==================================
  Tcl_CreateCommand(interp, 
                    "GetOurRobotPassValue_cmd", 
                    GetOurRobotPassValue_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "GetOurRobotPassDestinationX_cmd", 
                    GetOurRobotPassDestinationX_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "GetOurRobotPassDestinationY_cmd", 
                    GetOurRobotPassDestinationY_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Add command to get the current play name
  Tcl_CreateCommand(interp, 
                    "friendlyRobotFound_cmd", 
                    friendlyRobotFound_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Add command to get the current play name
  Tcl_CreateCommand(interp, 
                    "friendlyPositionStable_cmd", 
                    friendlyPositionStable_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Add command to get the current play name
  Tcl_CreateCommand(interp, 
                    "friendlyRotationStable_cmd", 
                    friendlyRotationStable_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Add command to get the current play name
  Tcl_CreateCommand(interp, 
                    "opponentFound_cmd", 
                    opponentFound_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Add command to get the current play name
  Tcl_CreateCommand(interp, 
                    "opponentPositionStable_cmd", 
                    opponentPositionStable_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Add command to get the current play name
  Tcl_CreateCommand(interp, 
                    "ballFound_cmd", 
                    ballFound_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Add command to get the current play name
  Tcl_CreateCommand(interp, 
                    "ballPositionStable_cmd", 
                    ballPositionStable_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  //Add command to get the current play name
  Tcl_CreateCommand(interp, 
                    "visionQualityFrame_cmd", 
                    visionQualityFrame_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "SetRobotLocation_cmd", 
                    SetRobotLocation_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "SetRobotRotation_cmd", 
                    SetRobotRotation_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "getDebugPointX", 
                    getDebugPointX,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "getDebugPointY", 
                    getDebugPointY,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "getNumDebugPoints", 
                    getNumDebugPoints,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "getNumDebugMessages_cmd", 
                    getNumDebugMessages_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "getDebugMessage", 
                    getDebugMessage,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "LEFT_SIDE_LINE_cmd", 
                    LEFT_SIDE_LINE_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "RIGHT_SIDE_LINE_cmd", 
                    RIGHT_SIDE_LINE_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "OUR_GOAL_LINE_cmd", 
                    OUR_GOAL_LINE_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "THEIR_GOAL_LINE_cmd", 
                    THEIR_GOAL_LINE_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "GOAL_DEPTH_cmd", 
                    GOAL_DEPTH_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "GOALIEBOX_DEPTH_cmd", 
                    GOAL_DEPTH_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "LEFT_GOAL_POST_cmd", 
                   LEFT_GOAL_POST_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "RIGHT_GOAL_POST_cmd", 
                    RIGHT_GOAL_POST_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "LEFT_GOALIE_BOX_cmd", 
                    LEFT_GOALIE_BOX_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "RIGHT_GOALIE_BOX_cmd", 
                    RIGHT_GOALIE_BOX_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "GOALIE_BOX_DEPTH_cmd", 
                    GOALIE_BOX_DEPTH_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "CENTER_CIRCLE_RADIUS_cmd", 
                    CENTER_CIRCLE_RADIUS_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "FREE_KICK_X_OFFSET_cmd", 
                    FREE_KICK_X_OFFSET_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "FREE_KICK_Y_OFFSET_cmd", 
                    FREE_KICK_Y_OFFSET_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================
  Tcl_CreateCommand(interp, 
                    "PENALTY_KICK_X_OFFSET_cmd", 
                    PENALTY_KICK_X_OFFSET_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL); 
  //================================== 
  Tcl_CreateCommand(interp, 
                    "DEATH_ZONE_LINE_cmd", 
                    DEATH_ZONE_LINE_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //================================== 
  Tcl_CreateCommand(interp, 
                    "DEFENSE_ZONE_LINE_cmd", 
                    DEFENSE_ZONE_LINE_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //================================== 
  Tcl_CreateCommand(interp, 
                    "KILL_ZONE_LINE_cmd", 
                    KILL_ZONE_LINE_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //================================== 
  Tcl_CreateCommand(interp, 
                    "OFFENSE_ZONE_LINE_cmd", 
                    OFFENSE_ZONE_LINE_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //================================== 
  Tcl_CreateCommand(interp, 
                    "LEFT_MIDDLE_SECTOR_cmd", 
                    LEFT_MIDDLE_SECTOR_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //================================== 
  Tcl_CreateCommand(interp, 
                    "RIGHT_MIDDLE_SECTOR_cmd", 
                    RIGHT_MIDDLE_SECTOR_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //================================== 
  Tcl_CreateCommand(interp, 
                    "LEFT_SWEET_SECTOR_cmd", 
                    LEFT_SWEET_SECTOR_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //================================== 
  Tcl_CreateCommand(interp, 
                    "RIGHT_SWEET_SECTOR_cmd", 
                    RIGHT_SWEET_SECTOR_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //================================== 
  Tcl_CreateCommand(interp, 
                    "get_VISION_EXECUTE_TIME", 
                    get_VISION_EXECUTE_TIME,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //================================== 
  Tcl_CreateCommand(interp, 
                    "get_VISION_GET_TIME", 
                    get_VISION_GET_TIME,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //================================== 
  Tcl_CreateCommand(interp, 
                    "get_WIRELESS_EXECUTE_TIME", 
                    get_WIRELESS_EXECUTE_TIME,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //================================== 
  Tcl_CreateCommand(interp, 
                    "get_SIMULATOR_EXECUTE_TIME", 
                    get_SIMULATOR_EXECUTE_TIME,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //================================== 
  Tcl_CreateCommand(interp, 
                    "get_STRATEGY_EXECUTE_TIME", 
                    get_STRATEGY_EXECUTE_TIME,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //================================== 
  Tcl_CreateCommand(interp, 
                    "get_TRAJECTORY_EXECUTE_TIME", 
                    get_TRAJECTORY_EXECUTE_TIME,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //================================== 
  Tcl_CreateCommand(interp, 
                    "get_TOTAL_EXECUTE_TIME", 
                    get_TOTAL_EXECUTE_TIME,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //================================== 
  Tcl_CreateCommand(interp, 
                    "get_AI_FPS", 
                    get_AI_FPS,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //================================== 
  Tcl_CreateCommand(interp, 
                    "get_VISION_FPS", 
                    get_VISION_FPS,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //================================== 
  Tcl_CreateCommand(interp, 
                    "TogglePositionSwitching_cmd", 
                    TogglePositionSwitching_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //================================== 
  Tcl_CreateCommand(interp, 
                    "TogglePlayTransitions_cmd", 
                    TogglePlayTransitions_cmd,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //==================================   
  Tcl_CreateCommand(interp, 
                    "RESET_GOALIE_BOX_DEPTH", 
                    RESET_GOALIE_BOX_DEPTH,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  Tcl_CreateCommand(interp, 
                    "RESET_OUR_GOAL_LINE", 
                    RESET_OUR_GOAL_LINE,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  Tcl_CreateCommand(interp, 
                    "RESET_THEIR_GOAL_LINE", 
                    RESET_THEIR_GOAL_LINE,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  Tcl_CreateCommand(interp, 
                    "RESET_OUR_LEFT_GOAL_WALL", 
                    RESET_OUR_LEFT_GOAL_WALL,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  Tcl_CreateCommand(interp, 
                    "RESET_OUR_RIGHT_GOAL_WALL", 
                    RESET_OUR_RIGHT_GOAL_WALL,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  Tcl_CreateCommand(interp, 
                    "RESET_THEIR_LEFT_GOAL_WALL", 
                    RESET_THEIR_LEFT_GOAL_WALL,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  Tcl_CreateCommand(interp, 
                    "RESET_THEIR_RIGHT_GOAL_WALL", 
                    RESET_THEIR_RIGHT_GOAL_WALL,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  Tcl_CreateCommand(interp, 
                    "RESET_RIGHT_SIDE_LINE", 
                    RESET_RIGHT_SIDE_LINE,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  Tcl_CreateCommand(interp, 
                    "RESET_RIGHT_GOALIE_BOX", 
                    RESET_RIGHT_GOALIE_BOX,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  Tcl_CreateCommand(interp, 
                    "RESET_RIGHT_GOAL_POST", 
                    RESET_RIGHT_GOAL_POST,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  Tcl_CreateCommand(interp, 
                    "RESET_LEFT_GOAL_POST", 
                    RESET_LEFT_GOAL_POST,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  Tcl_CreateCommand(interp, 
                    "RESET_LEFT_GOALIE_BOX", 
                    RESET_LEFT_GOALIE_BOX,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  Tcl_CreateCommand(interp, 
                    "RESET_LEFT_SIDE_LINE", 
                    RESET_LEFT_SIDE_LINE,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  Tcl_CreateCommand(interp, 
                    "RESET_OUR_LEFT_CORNER", 
                    RESET_OUR_LEFT_CORNER,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  Tcl_CreateCommand(interp, 
                    "RESET_OUR_RIGHT_CORNER", 
                    RESET_OUR_RIGHT_CORNER,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  Tcl_CreateCommand(interp, 
                    "RESET_THEIR_LEFT_CORNER", 
                    RESET_THEIR_LEFT_CORNER,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  Tcl_CreateCommand(interp, 
                    "RESET_THEIR_RIGHT_CORNER", 
                    RESET_THEIR_RIGHT_CORNER,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------
  Tcl_CreateCommand(interp, 
                    "RESET_CENTER", 
                    RESET_CENTER,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------  
  Tcl_CreateCommand(interp, 
                    "SAVE_FIELD_PARAMETERS", 
                    SAVE_FIELD_PARAMETERS,
                    (ClientData) NULL, 
                    (Tcl_CmdDeleteProc *) NULL);
  //---------------------------------------------  
  Tcl_SetVar(interp, 
             "tcl_rcFileName", 
             "~/.wishrc", 
             TCL_GLOBAL_ONLY);
  //==================================

  return TCL_OK;

}
//==========================================
//==========================================
//==========================================

//=======================
//      ArgError         
//=======================
int ArgError(char *command, Tcl_Interp *interp)
{
  /*
   * Provide an error reporting mechanism. If a command is called
   * with the wrong number of arguments, we create a suitable
   * error message and pass back the TCL_ERROR code to show that
   * the command didn't complete properly.
   */
  sprintf(result, "Wrong number of arguments for function: %s",
          command);

  /*
   * This is the method used for passing results back to the
   * wish interpreter
   */
  interp->result = result;
  return TCL_ERROR;
}

//========================
//     ArgvError      
//========================
int ArgvError(char *command, Tcl_Interp *interp)
{
  /*
   * Provide an error reporting mechanism. If a command is called
   * with the invalid arguments, we create a suitable
   * error message and pass back the TCL_ERROR code to show that
   * the command didn't complete properly.
   */
  sprintf(result, "Invalid %s argument",
          command);

  /*
   * This is the method used for passing results back to the
   * wish interpreter
   */
  interp->result = result;
  return TCL_ERROR;
}

//Tcl uses arrays of characters
//to pass around output...
char result[600];

//End extern...
#ifdef __cplusplus
};
#endif





