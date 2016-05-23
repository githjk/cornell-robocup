# Microsoft Developer Studio Project File - Name="robocupAI" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=robocupAI - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "robocupAI.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "robocupAI.mak" CFG="robocupAI - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "robocupAI - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "robocupAI - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "robocupAI - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\runtime_folder"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /Zi /O2 /Ob2 /I "../" /I "..\\" /I "..\common\util" /I "..\common\modules" /I "..\common\datatypes" /I "..\common\control" /I "..\common\vision" /I "c:\Program Files\Tcl\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 winmm.lib tcl84.lib tclstub84.lib tk84.lib tkstub84.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /libpath:"C:\Program Files\Tcl\lib\\"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "robocupAI - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\runtime_folder"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /I "..\\" /I "..\common\util" /I "..\common\modules" /I "..\common\datatypes" /I "..\common\control" /I "..\common\vision" /I "c:\Program Files\Tcl\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS /NODEFAULTLIB:library" /FR /FD /I /I /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i "C:\Program Files\Tcl\lib\\" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"robocupAI.bsc"
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib tcl84.lib tclstub84.lib tk84.lib tkstub84.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"..\runtime_folder/robocupAI-debug.exe" /libpath:"C:\Program Files\Tcl\lib\\"
# SUBTRACT LINK32 /profile /nodefaultlib

!ENDIF 

# Begin Target

# Name "robocupAI - Win32 Release"
# Name "robocupAI - Win32 Debug"
# Begin Group "common"

# PROP Default_Filter ""
# Begin Group "datatypes"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\datatypes\ActionCodes.h
# End Source File
# Begin Source File

SOURCE=..\common\datatypes\ai.h
# End Source File
# Begin Source File

SOURCE=..\common\datatypes\base_types.h
# End Source File
# Begin Source File

SOURCE=..\common\datatypes\ControlParams.cpp
# End Source File
# Begin Source File

SOURCE=..\common\datatypes\ControlParams.h
# End Source File
# Begin Source File

SOURCE=..\common\datatypes\FieldObjects.h
# End Source File
# Begin Source File

SOURCE=..\common\datatypes\FieldParams.cpp
# End Source File
# Begin Source File

SOURCE=..\common\datatypes\FieldParams.h
# End Source File
# Begin Source File

SOURCE=..\common\datatypes\GeneralParams.cpp
# End Source File
# Begin Source File

SOURCE=..\common\datatypes\GeneralParams.h
# End Source File
# Begin Source File

SOURCE=..\common\datatypes\obstacle.h
# End Source File
# Begin Source File

SOURCE=..\common\datatypes\ParameterOperations.cpp
# End Source File
# Begin Source File

SOURCE=..\common\datatypes\ParameterOperations.h
# End Source File
# Begin Source File

SOURCE=..\common\datatypes\passData.h
# End Source File
# Begin Source File

SOURCE=..\common\datatypes\path.h
# End Source File
# Begin Source File

SOURCE=..\common\datatypes\robocup_strategy_data.cpp
# End Source File
# Begin Source File

SOURCE=..\common\datatypes\robocup_strategy_data.h
# End Source File
# Begin Source File

SOURCE=..\common\datatypes\StrategyParams2002.cpp
# End Source File
# Begin Source File

SOURCE=..\common\datatypes\StrategyParams2002.h
# End Source File
# Begin Source File

SOURCE=..\common\datatypes\SystemParameters.cpp
# End Source File
# Begin Source File

SOURCE=..\common\datatypes\SystemParameters.h
# End Source File
# Begin Source File

SOURCE=..\common\datatypes\vision_globals.h
# End Source File
# End Group
# Begin Group "modules"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\modules\FilteredObject.cpp
# End Source File
# Begin Source File

SOURCE=..\common\modules\FilteredObject.h
# End Source File
# Begin Source File

SOURCE=..\common\modules\hires_timer.cpp
# End Source File
# Begin Source File

SOURCE=..\common\modules\hires_timer.h
# End Source File
# Begin Source File

SOURCE=..\common\modules\StandardModule.cpp
# End Source File
# Begin Source File

SOURCE=..\common\modules\StandardModule.h
# End Source File
# Begin Source File

SOURCE=..\common\modules\StrategyModule.cpp
# End Source File
# Begin Source File

SOURCE=..\common\modules\StrategyModule.h
# End Source File
# Begin Source File

SOURCE=..\common\modules\TrajectoryAndControl.h
# End Source File
# End Group
# Begin Group "network"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\network\Client.cpp
# End Source File
# Begin Source File

SOURCE=..\common\network\Client.h
# End Source File
# Begin Source File

SOURCE=..\common\network\CommType.h
# End Source File
# Begin Source File

SOURCE=..\common\network\Server.cpp
# End Source File
# Begin Source File

SOURCE=..\common\network\Server.h
# End Source File
# Begin Source File

SOURCE=..\common\network\Socket.cpp
# End Source File
# Begin Source File

SOURCE=..\common\network\Socket.h
# End Source File
# End Group
# Begin Group "util"

# PROP Default_Filter ""
# Begin Group "action"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\util\action\action.cpp
# End Source File
# Begin Source File

SOURCE=..\common\util\action\action.h
# End Source File
# Begin Source File

SOURCE=..\common\util\action\dribble.cpp
# End Source File
# Begin Source File

SOURCE=..\common\util\action\dribble.h
# End Source File
# End Group
# Begin Group "annoy"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\util\annoy\screen.cpp
# End Source File
# Begin Source File

SOURCE=..\common\util\annoy\screen.h
# End Source File
# End Group
# Begin Group "info"

# PROP Default_Filter ""
# Begin Group "ball"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\util\info\ball\ball.cpp
# End Source File
# Begin Source File

SOURCE=..\common\util\info\ball\ball.h
# End Source File
# Begin Source File

SOURCE=..\common\util\info\ball\possession.cpp
# End Source File
# Begin Source File

SOURCE=..\common\util\info\ball\possession.h
# End Source File
# End Group
# Begin Group "robot"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\util\info\robot\robot.cpp
# End Source File
# Begin Source File

SOURCE=..\common\util\info\robot\robot.h
# End Source File
# End Group
# End Group
# Begin Group "intersect"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\util\intersect\intersect.cpp
# End Source File
# Begin Source File

SOURCE=..\common\util\intersect\intersect.h
# End Source File
# End Group
# Begin Group "math"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\util\math\angle.h
# End Source File
# Begin Source File

SOURCE=..\common\util\math\cmath.cpp
# End Source File
# Begin Source File

SOURCE=..\common\util\math\cmath.h
# End Source File
# Begin Source File

SOURCE=..\common\util\math\coordinate_transform.cpp
# End Source File
# Begin Source File

SOURCE=..\common\util\math\coordinate_transform.h
# End Source File
# Begin Source File

SOURCE=..\common\util\math\dist.cpp
# End Source File
# Begin Source File

SOURCE=..\common\util\math\dist.h
# End Source File
# Begin Source File

SOURCE=..\common\util\math\find_open_space.cpp
# End Source File
# Begin Source File

SOURCE=..\common\util\math\find_open_space.h
# End Source File
# Begin Source File

SOURCE=..\common\util\math\perpendicular.cpp
# End Source File
# Begin Source File

SOURCE=..\common\util\math\perpendicular.h
# End Source File
# Begin Source File

SOURCE=..\common\util\math\tangent.cpp
# End Source File
# Begin Source File

SOURCE=..\common\util\math\tangent.h
# End Source File
# End Group
# Begin Group "pass"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\util\pass\back_pass.cpp
# End Source File
# Begin Source File

SOURCE=..\common\util\pass\back_pass.h
# End Source File
# Begin Source File

SOURCE=..\common\util\pass\best_pass_position.cpp
# End Source File
# Begin Source File

SOURCE=..\common\util\pass\best_pass_position.h
# End Source File
# Begin Source File

SOURCE=..\common\util\pass\fast_pass.cpp
# End Source File
# Begin Source File

SOURCE=..\common\util\pass\fast_pass.h
# End Source File
# Begin Source File

SOURCE=..\common\util\pass\find_pass.cpp
# End Source File
# Begin Source File

SOURCE=..\common\util\pass\find_pass.h
# End Source File
# Begin Source File

SOURCE=..\common\util\pass\forward_pass.cpp
# End Source File
# Begin Source File

SOURCE=..\common\util\pass\forward_pass.h
# End Source File
# Begin Source File

SOURCE=..\common\util\pass\pass.cpp
# End Source File
# Begin Source File

SOURCE=..\common\util\pass\pass.h
# End Source File
# Begin Source File

SOURCE=..\common\util\pass\receive_pass.cpp
# End Source File
# Begin Source File

SOURCE=..\common\util\pass\receive_pass.h
# End Source File
# End Group
# Begin Group "shoot"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\util\shoot\shoot.cpp
# End Source File
# Begin Source File

SOURCE=..\common\util\shoot\shoot.h
# End Source File
# Begin Source File

SOURCE=..\common\util\shoot\shotQuality.cpp
# End Source File
# Begin Source File

SOURCE=..\common\util\shoot\shotQuality.h
# End Source File
# End Group
# Begin Group "corner"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\util\corner\corner.cpp
# End Source File
# Begin Source File

SOURCE=..\common\util\corner\corner.h
# End Source File
# End Group
# Begin Group "lane"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\util\lane\lane.cpp
# End Source File
# Begin Source File

SOURCE=..\common\util\lane\lane.h
# End Source File
# End Group
# Begin Group "pair"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\util\pair\pair.cpp
# End Source File
# Begin Source File

SOURCE=..\common\util\pair\pair.h
# End Source File
# End Group
# Begin Group "defending"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\util\defense\defending.cpp
# End Source File
# Begin Source File

SOURCE=..\common\util\defense\defending.h
# End Source File
# End Group
# Begin Group "2002Strategy"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\util\2002Strategy\2002Strategy.cpp
# End Source File
# Begin Source File

SOURCE=..\common\util\2002Strategy\2002Strategy.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\common\util\assert.h
# End Source File
# Begin Source File

SOURCE=..\common\util\static_vars.h
# End Source File
# End Group
# Begin Group "wireless"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\wireless\PacketStructure2001.h
# End Source File
# Begin Source File

SOURCE=..\common\wireless\PacketStructure2002.h
# End Source File
# Begin Source File

SOURCE=..\common\wireless\RCSerialPortInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\common\wireless\RCSerialPortInterface.h
# End Source File
# Begin Source File

SOURCE=..\common\wireless\RobotCommand.h
# End Source File
# Begin Source File

SOURCE=..\common\wireless\WheelConversion2000.cpp
# End Source File
# Begin Source File

SOURCE=..\common\wireless\WheelConversion2000.h
# End Source File
# Begin Source File

SOURCE=..\common\wireless\WheelConversion2001.cpp
# End Source File
# Begin Source File

SOURCE=..\common\wireless\WheelConversion2001.h
# End Source File
# Begin Source File

SOURCE=..\common\wireless\WheelConversion2002.cpp
# End Source File
# Begin Source File

SOURCE=..\common\wireless\WheelConversion2002.h
# End Source File
# Begin Source File

SOURCE=..\common\wireless\WirelessSender.cpp
# End Source File
# Begin Source File

SOURCE=..\common\wireless\WirelessSender.h
# End Source File
# End Group
# Begin Group "control"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\control\ControlData.cpp
# End Source File
# Begin Source File

SOURCE=..\common\control\ControlData.h
# End Source File
# Begin Source File

SOURCE=..\common\control\ControlModule.cpp
# End Source File
# Begin Source File

SOURCE=..\common\control\ControlModule.h
# End Source File
# Begin Source File

SOURCE=..\common\control\ObstacleAvoidance.cpp
# End Source File
# Begin Source File

SOURCE=..\common\control\ObstacleAvoidance.h
# End Source File
# Begin Source File

SOURCE=..\common\control\Trajectory.cpp
# End Source File
# Begin Source File

SOURCE=..\common\control\Trajectory.h
# End Source File
# Begin Source File

SOURCE=..\common\control\TrajectoryStructs.cpp
# End Source File
# Begin Source File

SOURCE=..\common\control\TrajectoryStructs.h
# End Source File
# Begin Source File

SOURCE=..\common\control\TrajectorySupport.cpp
# End Source File
# Begin Source File

SOURCE=..\common\control\TrajectorySupport.h
# End Source File
# Begin Source File

SOURCE=..\common\control\WallAvoidance.cpp
# End Source File
# Begin Source File

SOURCE=..\common\control\WallAvoidance.h
# End Source File
# End Group
# Begin Group "vision"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\vision\RawVisionData.cpp
# End Source File
# Begin Source File

SOURCE=..\common\vision\RawVisionData.h
# End Source File
# Begin Source File

SOURCE=..\common\vision\VisionConstants.h
# End Source File
# Begin Source File

SOURCE=..\common\vision\VisionData.cpp
# End Source File
# Begin Source File

SOURCE=..\common\vision\VisionData.h
# End Source File
# Begin Source File

SOURCE=..\common\vision\VisionModule.h
# End Source File
# Begin Source File

SOURCE=..\common\vision\VisionModuleBasicFunctions.cpp
# End Source File
# Begin Source File

SOURCE=..\common\vision\VisionModuleMainFunctions.cpp
# End Source File
# Begin Source File

SOURCE=..\common\vision\VisionModulePrediction.cpp
# End Source File
# Begin Source File

SOURCE=..\common\vision\VisionModulePredictionError.cpp
# End Source File
# Begin Source File

SOURCE=..\common\vision\VisionPacket.cpp
# End Source File
# Begin Source File

SOURCE=..\common\vision\VisionPacket.h
# End Source File
# Begin Source File

SOURCE=..\common\vision\visionTimer.cpp
# End Source File
# Begin Source File

SOURCE=..\common\vision\visionTimer.h
# End Source File
# End Group
# End Group
# Begin Group "GUI"

# PROP Default_Filter ".cpp .h"
# Begin Source File

SOURCE=..\GUI\GUI.cpp
# End Source File
# Begin Source File

SOURCE=..\GUI\GUI.h
# End Source File
# Begin Source File

SOURCE=..\GUI\GUI_Commands.cpp
# End Source File
# Begin Source File

SOURCE=..\GUI\GUI_Commands.h
# End Source File
# Begin Source File

SOURCE=..\GUI\GUI_Internal_Includes.h
# End Source File
# Begin Source File

SOURCE=..\GUI\GUI_Skillz.cpp
# End Source File
# Begin Source File

SOURCE=..\GUI\GUI_Skillz.h
# End Source File
# End Group
# Begin Group "logger"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\logger\playback_writer.cpp
# End Source File
# Begin Source File

SOURCE=..\logger\playback_writer.h
# End Source File
# End Group
# Begin Group "plays"

# PROP Default_Filter ""
# Begin Group "skillPlays"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\plays\skillPlays\AcquirePossessionSkill_Test.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\AcquirePossessionSkill_Test.h
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\AdvanceBallUpfieldSkill_Test.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\AdvanceBallUpfieldSkill_Test.h
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\AggressorCageDefenseSkill_Test.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\AggressorCageDefenseSkill_Test.h
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\BlockerGainPossessionSkill_Test.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\BlockerGainPossessionSkill_Test.h
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\CreatorLookForwardPassSkill_Test.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\CreatorLookForwardPassSkill_Test.h
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\CrossCreasePassSkill_Test.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\CrossCreasePassSkill_Test.h
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\CutGoalSkill_Test.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\CutGoalSkill_Test.h
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\DefenderCageDefenseSkill_Test.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\DefenderCageDefenseSkill_Test.h
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\DribblePointSkill_Test.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\DribblePointSkill_Test.h
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\InterceptBallSkill_test.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\InterceptBallSkill_test.h
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\JamAndShootSkill_Test.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\JamAndShootSkill_Test.h
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\JukeAroundOpponentSkill_Test.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\JukeAroundOpponentSkill_Test.h
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\LookForChipKickSkill_Test.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\LookForChipKickSkill_Test.h
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\LooseDefenseSkill_Test.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\LooseDefenseSkill_Test.h
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\LooseDSpecOpSkill_Test.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\LooseDSpecOpSkill_Test.h
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\ManeuverWithBallSkill_Test.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\ManeuverWithBallSkill_Test.h
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\MidReboundOffenseSkill_Test.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\MidReboundOffenseSkill_Test.h
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\MoveAroundPickSkill_Test.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\MoveAroundPickSkill_Test.h
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\MoveToBall_test.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\MoveToBall_test.h
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\OppositeReboundOffenseSkill_Test.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\OppositeReboundOffenseSkill_Test.h
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\PickSkill_Test.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\PickSkill_Test.h
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\PullBallOffCornerSkill_test.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\PullBallOffCornerSkill_test.h
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\PullBallOffWallSkill_test.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\PullBallOffWallSkill_test.h
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\ReceivePassSkill_Test.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\ReceivePassSkill_Test.h
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\SimpleDefenseSkill_Test.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\SimpleDefenseSkill_Test.h
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\SimpleScreenSkill_Test.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\SimpleScreenSkill_Test.h
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\SpecialOpCageDefenseSkill_Test.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\SpecialOpCageDefenseSkill_Test.h
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\SpinAroundBallSkill_Test.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\SpinAroundBallSkill_Test.h
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\StiffarmOffenseSkill_Test.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\StiffarmOffenseSkill_Test.h
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\StripBallSkill_Test.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\StripBallSkill_Test.h
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\TestSkill_test.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\TestSkill_test.h
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\TightDefenseBlockerSkill_Test.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\TightDefenseBlockerSkill_Test.h
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\TightDefenseDefenderSkill_Test.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\TightDefenseDefenderSkill_Test.h
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\TightDefenseSpecialOpDSkill_Test.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\TightDefenseSpecialOpDSkill_Test.h
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\TurnAndKickSkill_Test.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\skillPlays\TurnAndKickSkill_Test.h
# End Source File
# End Group
# Begin Group "2002plays"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\plays\2002plays\backPass.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\backPass.h
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\cageDefense.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\cageDefense.h
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\creatorAssistOffense.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\creatorAssistOffense.h
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\CreatorPass.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\CreatorPass.h
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\crossCreasePass.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\crossCreasePass.h
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\doublePickOffense.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\doublePickOffense.h
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\freeKick.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\freeKick.h
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\jamAndShoot.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\jamAndShoot.h
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\kickUpfield.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\kickUpfield.h
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\looseDefense.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\looseDefense.h
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\penaltyShotThemDiveLeft2002.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\penaltyShotThemDiveLeft2002.h
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\penaltyShotThemDiveRight2002.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\penaltyShotThemDiveRight2002.h
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\penaltyShotThemRush2002.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\penaltyShotThemRush2002.h
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\penaltyShotThemSmartRush2002.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\penaltyShotThemSmartRush2002.h
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\penaltyShotUsOpenLoop2002.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\penaltyShotUsOpenLoop2002.h
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\penaltyShotUsPivot2002.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\penaltyShotUsPivot2002.h
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\penaltyShotUsSideStep2002.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\penaltyShotUsSideStep2002.h
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\pongDefense.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\pongDefense.h
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\puntKick.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\puntKick.h
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\removeBallDefense.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\removeBallDefense.h
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\singleAssistOffense.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\singleAssistOffense.h
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\ThreeManDefense.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\ThreeManDefense.h
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\tightDefense.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\tightDefense.h
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\touchShoot.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\touchShoot.h
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\upfieldDefense.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\2002plays\upfieldDefense.h
# End Source File
# End Group
# Begin Group "individualPlays"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\plays\individualPlays\hollywood_play.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\individualPlays\hollywood_play.h
# End Source File
# Begin Source File

SOURCE=..\plays\individualPlays\joel_play.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\individualPlays\joel_play.h
# End Source File
# Begin Source File

SOURCE=..\plays\individualPlays\nirav_play.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\individualPlays\nirav_play.h
# End Source File
# Begin Source File

SOURCE=..\plays\individualPlays\tamas_play.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\individualPlays\tamas_play.h
# End Source File
# Begin Source File

SOURCE=..\plays\individualPlays\will_play.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\individualPlays\will_play.h
# End Source File
# End Group
# Begin Group "intimidationPlays"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\plays\intimidationPlays\cones_play.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\intimidationPlays\cones_play.h
# End Source File
# Begin Source File

SOURCE=..\plays\intimidationPlays\ellipses.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\intimidationPlays\ellipses.h
# End Source File
# Begin Source File

SOURCE=..\plays\intimidationPlays\figure_eight.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\intimidationPlays\figure_eight.h
# End Source File
# Begin Source File

SOURCE=..\plays\intimidationPlays\five_man_star.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\intimidationPlays\five_man_star.h
# End Source File
# Begin Source File

SOURCE=..\plays\intimidationPlays\radar_play.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\intimidationPlays\radar_play.h
# End Source File
# Begin Source File

SOURCE=..\plays\intimidationPlays\runlaps.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\intimidationPlays\runlaps.h
# End Source File
# End Group
# Begin Group "otherPlays"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\plays\otherPlays\angular_latency_test.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\otherPlays\angular_latency_test.h
# End Source File
# Begin Source File

SOURCE=..\plays\otherPlays\DribbleAngle.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\otherPlays\DribbleAngle.h
# End Source File
# Begin Source File

SOURCE=..\plays\otherPlays\KickingChallengePlay.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\otherPlays\KickingChallengePlay.h
# End Source File
# Begin Source File

SOURCE=..\plays\otherPlays\LED_latency_test.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\otherPlays\LED_latency_test.h
# End Source File
# Begin Source File

SOURCE=..\plays\otherPlays\PassingChallengePlay.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\otherPlays\PassingChallengePlay.h
# End Source File
# Begin Source File

SOURCE=..\plays\otherPlays\prediction_test_play.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\otherPlays\prediction_test_play.h
# End Source File
# Begin Source File

SOURCE=..\plays\otherPlays\random_play.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\otherPlays\random_play.h
# End Source File
# Begin Source File

SOURCE=..\plays\otherPlays\SimpleDribble_Test.h
# End Source File
# Begin Source File

SOURCE=..\plays\otherPlays\SimpleDribbleTest.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\otherPlays\SimpleKick_Test.h
# End Source File
# Begin Source File

SOURCE=..\plays\otherPlays\SimpleKickTest.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\otherPlays\snowflakeTester.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\otherPlays\snowflakeTester.h
# End Source File
# Begin Source File

SOURCE=..\plays\otherPlays\timing_test.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\otherPlays\timing_test.h
# End Source File
# Begin Source File

SOURCE=..\plays\otherPlays\torturetest.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\otherPlays\torturetest.h
# End Source File
# Begin Source File

SOURCE=..\plays\otherPlays\visionTester.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\otherPlays\visionTester.h
# End Source File
# End Group
# Begin Group "formations"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\plays\formations\dribble_formation.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\formations\dribble_formation.h
# End Source File
# Begin Source File

SOURCE=..\plays\formations\formation_tester.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\formations\formation_tester.h
# End Source File
# Begin Source File

SOURCE=..\plays\formations\kick_formation.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\formations\kick_formation.h
# End Source File
# Begin Source File

SOURCE=..\plays\formations\kickoff_them.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\formations\kickoff_them.h
# End Source File
# Begin Source File

SOURCE=..\plays\formations\kickoff_us.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\formations\kickoff_us.h
# End Source File
# Begin Source File

SOURCE=..\plays\formations\penalty_shot_them.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\formations\penalty_shot_them.h
# End Source File
# Begin Source File

SOURCE=..\plays\formations\penalty_shot_us.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\formations\penalty_shot_us.h
# End Source File
# Begin Source File

SOURCE=..\plays\formations\wall_formation.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\formations\wall_formation.h
# End Source File
# End Group
# Begin Group "playTransition"

# PROP Default_Filter ""
# Begin Group "2002playTransitions"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\backPassTransition.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\backPassTransition.h
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\cageDefenseTransition.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\cageDefenseTransition.h
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\creatorAssistOffenseTransition.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\creatorAssistOffenseTransition.h
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\CreatorPassTransition.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\CreatorPassTransition.h
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\crossCreasePassTransition.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\crossCreasePassTransition.h
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\doublePickOffenseTransition.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\doublePickOffenseTransition.h
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\freeKickTransition.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\freeKickTransition.h
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\jamAndShootTransition.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\jamAndShootTransition.h
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\kickUpfieldTransition.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\kickUpfieldTransition.h
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\looseDefenseTransition.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\looseDefenseTransition.h
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\penaltyShotThemDiveLeft2002Transition.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\penaltyShotThemDiveLeft2002Transition.h
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\penaltyShotThemDiveRight2002Transition.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\penaltyShotThemDiveRight2002Transition.h
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\penaltyShotThemRush2002Transition.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\penaltyShotThemRush2002Transition.h
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\penaltyShotThemSmartRush2002Transition.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\penaltyShotThemSmartRush2002Transition.h
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\penaltyShotUsOpenLoop2002Transition.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\penaltyShotUsOpenLoop2002Transition.h
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\penaltyShotUsPivot2002Transition.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\penaltyShotUsPivot2002Transition.h
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\penaltyShotUsSideStep2002Transition.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\penaltyShotUsSideStep2002Transition.h
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\puntKickTransition.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\puntKickTransition.h
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\removeBallDefenseTransition.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\removeBallDefenseTransition.h
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\singleAssistOffenseTransition.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\singleAssistOffenseTransition.h
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\threeManDefenseTransition.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\threeManDefenseTransition.h
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\tightDefenseTransition.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\tightDefenseTransition.h
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\touchShootTransition.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\touchShootTransition.h
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\upfieldDefenseTransition.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\2002playTransitions\upfieldDefenseTransition.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\plays\playTransition\playTransition.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\playTransition.h
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\playTransitionIncludes.h
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\transition.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\playTransition\transition.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\plays\baseplay.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\baseplay.h
# End Source File
# Begin Source File

SOURCE=..\plays\basic_actions.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\basic_actions.h
# End Source File
# Begin Source File

SOURCE=..\plays\noplay.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\noplay.h
# End Source File
# Begin Source File

SOURCE=..\plays\playbook.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\playbook.h
# End Source File
# Begin Source File

SOURCE=..\plays\PlayIncludes.h
# End Source File
# Begin Source File

SOURCE=..\plays\plays.h
# End Source File
# Begin Source File

SOURCE=..\plays\RegisterPlays.cpp
# End Source File
# Begin Source File

SOURCE=..\plays\RegisterPlays.h
# End Source File
# End Group
# Begin Group "positions"

# PROP Default_Filter ""
# Begin Group "2002positions"

# PROP Default_Filter ""
# Begin Group "creator"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\positions\2002positions\creator\C_Back_Pass.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\creator\C_Cage_Defense.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\creator\C_Creator_Assist_Offense.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\creator\C_Creator_Pass.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\creator\C_Cross_Crease_Pass.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\creator\C_Double_Pick_Offense.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\creator\C_Jam_and_Shoot.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\creator\C_Kick_Upfield.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\creator\C_Loose_Defense.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\creator\C_Penalty_Shot_Them_2002.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\creator\C_Penalty_Shot_Us_2002.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\creator\C_Punt_Kick.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\creator\C_Remove_Ball_Defense.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\creator\C_Single_Assist_Offense.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\creator\C_Three_Man_Defense.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\creator\C_Tight_Defense.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\creator\C_Touch_Shoot.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\creator\C_Upfield_Defense.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\creator\Creator.h
# End Source File
# End Group
# Begin Group "defender"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\positions\2002positions\defender\D_Back_Pass.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\defender\D_Cage_Defense.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\defender\D_Creator_Assist_Offense.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\defender\D_Cross_Crease_Pass.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\defender\D_Double_Pick_Offense.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\defender\D_Jam_and_Shoot.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\defender\D_Kick_Upfield.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\defender\D_Loose_Defense.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\defender\D_Penalty_Shot_Them_2002.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\defender\D_Penalty_Shot_Us_2002.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\defender\D_Punt_Kick.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\defender\D_Remove_Ball_Defense.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\defender\D_Single_Assist_Offense.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\defender\D_Three_Man_Defense.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\defender\D_Tight_Defense.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\defender\D_Touch_Shoot.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\defender\D_Upfield_Defense.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\defender\Defender.h
# End Source File
# End Group
# Begin Group "blocker"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\positions\2002positions\blocker\B_Back_Pass.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\blocker\B_Cage_Defense.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\blocker\B_Creator_Assist_Offense.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\blocker\B_Cross_Crease_Pass.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\blocker\B_Double_Pick_Offense.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\blocker\B_Jam_and_Shoot.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\blocker\B_Kick_Upfield.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\blocker\B_Loose_Defense.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\blocker\B_Penalty_Shot_Them_Dive_Left_2002.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\blocker\B_Penalty_Shot_Them_Dive_Right_2002.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\blocker\B_Penalty_Shot_Them_Rush_2002.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\blocker\B_Penalty_Shot_Them_Smart_Rush_2002.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\blocker\B_Penalty_Shot_Us_2002.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\blocker\B_Punt_Kick.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\blocker\B_Remove_Ball_Defense.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\blocker\B_Single_Assist_Offense.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\blocker\B_Three_Man_Defense.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\blocker\B_Tight_Defense.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\blocker\B_Touch_Shoot.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\blocker\B_Upfield_Defense.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\blocker\Blocker.h
# End Source File
# End Group
# Begin Group "aggressor"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\positions\2002positions\aggressor\A_Back_Pass.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\aggressor\A_Cage_Defense.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\aggressor\A_Creator_Assist_Offense.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\aggressor\A_Cross_Crease_Pass.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\aggressor\A_Double_Pick_Offense.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\aggressor\A_Jam_and_Shoot.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\aggressor\A_Kick_Upfield.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\aggressor\A_Loose_Defense.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\aggressor\A_Penalty_Shot_Them_2002.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\aggressor\A_Penalty_Shot_Us_Open_Loop_2002.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\aggressor\A_Penalty_Shot_Us_Pivot_2002.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\aggressor\A_Penalty_Shot_Us_Side_Step_2002.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\aggressor\A_Punt_Kick.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\aggressor\A_Remove_Ball_Defense.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\aggressor\A_Single_Assist_Offense.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\aggressor\A_Three_Man_Defense.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\aggressor\A_Tight_Defense.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\aggressor\A_Touch_Shoot.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\aggressor\A_Upfield_Defense.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\aggressor\Aggressor.h
# End Source File
# End Group
# Begin Group "special_op_creator"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\positions\2002positions\special_op_creator\Special_Op_Creator.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\special_op_creator\Special_Op_Creator.h
# End Source File
# End Group
# Begin Group "special_op_aggressor"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\positions\2002positions\special_op_aggressor\Special_Op_Aggressor.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\special_op_aggressor\Special_Op_Aggressor.h
# End Source File
# End Group
# Begin Group "special_op_defender"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\positions\2002positions\special_op_defender\Special_Op_Defender.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\2002positions\special_op_defender\Special_Op_Defender.h
# End Source File
# End Group
# End Group
# Begin Group "positionSwitching"

# PROP Default_Filter ""
# Begin Group "2002_position_switching"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\positions\positionSwitching\2002_position_switching\BackPassStorm.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\2002_position_switching\BackPassStorm.h
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\2002_position_switching\CageDefenseStorm.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\2002_position_switching\CageDefenseStorm.h
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\2002_position_switching\CreatorAssistOffenseStorm.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\2002_position_switching\CreatorAssistOffenseStorm.h
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\2002_position_switching\CreatorPassStorm.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\2002_position_switching\CreatorPassStorm.h
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\2002_position_switching\CrossCreasePassStorm.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\2002_position_switching\CrossCreasePassStorm.h
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\2002_position_switching\DoublePickOffenseStorm.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\2002_position_switching\DoublePickOffenseStorm.h
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\2002_position_switching\JamAndShootStorm.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\2002_position_switching\JamAndShootStorm.h
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\2002_position_switching\KickUpfieldStorm.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\2002_position_switching\KickUpfieldStorm.h
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\2002_position_switching\LooseDefenseStorm.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\2002_position_switching\LooseDefenseStorm.h
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\2002_position_switching\PuntKickStorm.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\2002_position_switching\PuntKickStorm.h
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\2002_position_switching\RemoveBallDefenseStorm.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\2002_position_switching\RemoveBallDefenseStorm.h
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\2002_position_switching\SingleAssistOffenseStorm.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\2002_position_switching\SingleAssistOffenseStorm.h
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\2002_position_switching\ThreeManDefenseStorm.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\2002_position_switching\ThreeManDefenseStorm.h
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\2002_position_switching\TightDefenseStorm.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\2002_position_switching\TightDefenseStorm.h
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\2002_position_switching\TouchShootStorm.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\2002_position_switching\TouchShootStorm.h
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\2002_position_switching\UpfieldDefenseStorm.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\2002_position_switching\UpfieldDefenseStorm.h
# End Source File
# End Group
# Begin Group "formation_position_switching"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\positions\positionSwitching\formation_position_switching\ShortestDistanceStorm.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\formation_position_switching\ShortestDistanceStorm.h
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\formation_position_switching\WallFormationStorm.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\formation_position_switching\WallFormationStorm.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\positions\positionSwitching\Cloud.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\Cloud.h
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\Storm.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\Storm.h
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\Weather.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\positionSwitching\Weather.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\positions\ScratchVars.cpp
# End Source File
# Begin Source File

SOURCE=..\positions\ScratchVars.h
# End Source File
# End Group
# Begin Group "RoboCup modules"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\RoboCup_modules\RoboCupStandardModule.cpp
# End Source File
# Begin Source File

SOURCE=..\RoboCup_modules\RoboCupStandardModule.h
# End Source File
# Begin Source File

SOURCE=..\RoboCup_modules\RoboCupStrategyModule.cpp
# End Source File
# Begin Source File

SOURCE=..\RoboCup_modules\RoboCupStrategyModule.h
# End Source File
# End Group
# Begin Group "runtime_folder"

# PROP Default_Filter ".tcl .txt"
# Begin Group "Params"

# PROP Default_Filter ""
# Begin Group "Play_Params"

# PROP Default_Filter ""
# Begin Group "2002playsParams"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\runtime_folder\Params\Play_Params\2002plays\BackPassParams.txt
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Play_Params\2002plays\CageDefenseParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Play_Params\2002plays\CreatorAssistOffenseParams.txt"
# End Source File
# Begin Source File

SOURCE=..\runtime_folder\Params\Play_Params\2002plays\CreatorPassParams.txt
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Play_Params\2002plays\CrossCreasePassParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Play_Params\2002plays\DoublePickOffenseParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Play_Params\2002plays\JamAndShootParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Play_Params\2002plays\KickUpfieldParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Play_Params\2002plays\LooseDefenseParams.txt"
# End Source File
# Begin Source File

SOURCE=..\runtime_folder\Params\Play_Params\2002plays\PenaltyShotThemDiveLeft2002Params.txt
# End Source File
# Begin Source File

SOURCE=..\runtime_folder\Params\Play_Params\2002plays\PenaltyShotThemDiveRight2002Params.txt
# End Source File
# Begin Source File

SOURCE=..\runtime_folder\Params\Play_Params\2002plays\PenaltyShotThemRush2002Params.txt
# End Source File
# Begin Source File

SOURCE=..\runtime_folder\Params\Play_Params\2002plays\PenaltyShotUsOpenLoop2002Params.txt
# End Source File
# Begin Source File

SOURCE=..\runtime_folder\Params\Play_Params\2002plays\PenaltyShotUsPivot2002Params.txt
# End Source File
# Begin Source File

SOURCE=..\runtime_folder\Params\Play_Params\2002plays\PenaltyShotUsSideStep2002Params.txt
# End Source File
# Begin Source File

SOURCE=..\runtime_folder\Params\Play_Params\2002plays\PongDefenseParams.txt
# End Source File
# Begin Source File

SOURCE=..\runtime_folder\Params\Play_Params\2002plays\PuntKickParams.txt
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Play_Params\2002plays\RemoveBallDefenseParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Play_Params\2002plays\SingleAssistOffenseParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Play_Params\2002plays\ThreeManDefenseParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Play_Params\2002plays\TightDefenseParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Play_Params\2002plays\TouchShootParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Play_Params\2002plays\UpfieldDefenseParams.txt"
# End Source File
# End Group
# Begin Group "other"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\runtime_folder\Params\Play_Params\other\KickingChallengePlayParams.txt
# End Source File
# Begin Source File

SOURCE=..\runtime_folder\Params\Play_Params\other\PassingChallengePlayParams.txt
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Play_Params\other\TimingTestParams.txt"
# End Source File
# End Group
# Begin Group "intimidation"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\runtime_folder\Params\Play_Params\intimidationPlays\FigureEightParams.txt"
# End Source File
# End Group
# Begin Group "individual"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\runtime_folder\Params\Play_Params\individual\AkosuaParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Play_Params\individual\WillParams.txt"
# End Source File
# End Group
# Begin Group "playTransitionParams"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\runtime_folder\Params\Play_Params\playTransition\BackPassTransitionParams.txt
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Play_Params\playTransition\CageDefenseTransitionParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Play_Params\playTransition\CreatorAssistOffenseTransitionParams.txt"
# End Source File
# Begin Source File

SOURCE=..\runtime_folder\Params\Play_Params\playTransition\CreatorPassTransitionParams.txt
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Play_Params\playTransition\CrossCreasePassTransitionParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Play_Params\playTransition\DoublePickOffenseTransitionParams.txt"
# End Source File
# Begin Source File

SOURCE=..\runtime_folder\Params\Play_Params\playTransition\FreeKickTransitionParams.txt
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Play_Params\playTransition\JamAndShootTransitionParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Play_Params\playTransition\LooseDefenseTransitionParams.txt"
# End Source File
# Begin Source File

SOURCE=..\runtime_folder\Params\Play_Params\playTransition\PenaltyShotThemDiveLeft2002TransitionParams.txt
# End Source File
# Begin Source File

SOURCE=..\runtime_folder\Params\Play_Params\playTransition\PenaltyShotThemDiveRight2002TransitionParams.txt
# End Source File
# Begin Source File

SOURCE=..\runtime_folder\Params\Play_Params\playTransition\PenaltyShotThemRush2002TransitionParams.txt
# End Source File
# Begin Source File

SOURCE=..\runtime_folder\Params\Play_Params\playTransition\PenaltyShotThemSmartRush2002TransitionParams.txt
# End Source File
# Begin Source File

SOURCE=..\runtime_folder\Params\Play_Params\playTransition\PenaltyShotUsOpenLoop2002TransitionParams.txt
# End Source File
# Begin Source File

SOURCE=..\runtime_folder\Params\Play_Params\playTransition\PenaltyShotUsPivot2002TransitionParams.txt
# End Source File
# Begin Source File

SOURCE=..\runtime_folder\Params\Play_Params\playTransition\PenaltyShotUsSideStep2002TransitionParams.txt
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Play_Params\playTransition\RemoveBallDefenseTransitionParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Play_Params\playTransition\SingleAssistOffenseTransitionParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Play_Params\playTransition\TightDefenseTransitionParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Play_Params\playTransition\TouchShootTransitionParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Play_Params\playTransition\UpfieldDefenseTransitionParams.txt"
# End Source File
# End Group
# Begin Group "formationParams"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\runtime_folder\Params\Play_Params\formationPlays\KickFormationParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Play_Params\formationPlays\KickoffThemFormationParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Play_Params\formationPlays\KickoffUsFormationParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Play_Params\formationPlays\PenaltyShotThemFormationParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Play_Params\formationPlays\PenaltyShotUsFormationParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Play_Params\formationPlays\WallFormationParams.txt"
# End Source File
# End Group
# End Group
# Begin Group "Skill_Params"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\AcquirePossessionSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\AdvanceBallUpfieldSkillParams.txt"
# End Source File
# Begin Source File

SOURCE=..\runtime_folder\Params\Skill_Params\AggressiveJamAndShootSkillParams.txt
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\AggressorRecoverBallSkillParams.txt"
# End Source File
# Begin Source File

SOURCE=..\runtime_folder\Params\Skill_Params\BackPassSkillParams.txt
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\BehindScreenOffenseSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\BlockerGainPossessionSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\CageDefenseSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\CreatorCreateSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\CreatorLookForwardPassSkillParams.txt"
# End Source File
# Begin Source File

SOURCE=..\runtime_folder\Params\Skill_Params\CreatorPassSkillParams.txt
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\CrossCreaseCreatorSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\CrossCreasePassSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\CutGoalSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\DiveSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\DoublePickBlockSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\DribblePickScoreSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\DribblePointSkillParams.txt"
# End Source File
# Begin Source File

SOURCE=..\runtime_folder\Params\Skill_Params\FacilatePassSkillParams.txt
# End Source File
# Begin Source File

SOURCE=..\runtime_folder\Params\Skill_Params\FollowDefenderSkillParams.txt
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\InterceptBallSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\JamAndShootSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\JukeAroundOpponentSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\KickUpfieldSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\LookForChipKickSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\LooseDSpecialOpSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\ManeuverSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\MidReboundOffenseSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\MoveAroundPickSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\MoveToBallSkillParams.txt"
# End Source File
# Begin Source File

SOURCE=..\runtime_folder\Params\Skill_Params\OppositeCoverSkillParams.txt
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\OppositeReboundOffenseSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\PassBlockSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\PassToCreatorSkillParams.txt"
# End Source File
# Begin Source File

SOURCE=..\runtime_folder\Params\Skill_Params\PenaltyShotUsAimAndShootSkillParams.txt
# End Source File
# Begin Source File

SOURCE=..\runtime_folder\Params\Skill_Params\PenaltySpinSkillParams.txt
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\PickSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\PullOffCornerSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\PullOffWallSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\ReboundOffenseSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\ReceivePassSkillParams.txt"
# End Source File
# Begin Source File

SOURCE=..\runtime_folder\Params\Skill_Params\RemoveBallHelpSkillParams.txt
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\RemoveBallScreenSkillParams.txt"
# End Source File
# Begin Source File

SOURCE=..\runtime_folder\Params\Skill_Params\RemoveBallSetupSkillParams.txt
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\RemoveBallSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\RotateAroundBallSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\ShotBlockSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\SimpleDefenseSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\SimpleKickSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\SimpleScreenSkillParams.txt"
# End Source File
# Begin Source File

SOURCE=..\runtime_folder\Params\Skill_Params\SmartRotateAroundBallSkillParams.txt
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\SpecialOpBlockSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\SpecialOpCageDefenseSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\SpinSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\StiffarmOffenseSkillParams.txt"
# End Source File
# Begin Source File

SOURCE=..\runtime_folder\Params\Skill_Params\StrafeSkillParams.txt
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\StripBallSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\TandemBlockerSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\TandemDefenderSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\TandemDiveParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\TestSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\ThreeManBlockerSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\ThreeManSideSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\TightDefenseBlockerSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\TightDefenseDefenderSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\TightDefenseSpecialOpDSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\TouchShootSkillParams.txt"
# End Source File
# Begin Source File

SOURCE="..\runtime_folder\Params\Skill_Params\TurnAndKickSkillParams.txt"
# End Source File
# End Group
# Begin Group "TrajectoryParams"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\runtime_folder\Params\TrajectoryParams\RoboCup_2001_TrajectoryParams.txt
# End Source File
# Begin Source File

SOURCE=..\runtime_folder\Params\TrajectoryParams\RoboCup_2002_TrajectoryParams.txt
# End Source File
# End Group
# Begin Group "Position_Params"

# PROP Default_Filter ""
# Begin Group "PositionSwitching No. 1"

# PROP Default_Filter ""
# Begin Group "2002_position_switching No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\runtime_folder\Params\Position_Params\PositionSwitching\2002_postition_switching\ThreeManDefenseStormParams.txt
# End Source File
# End Group
# End Group
# End Group
# Begin Source File

SOURCE=..\runtime_folder\Params\FieldParams.txt
# End Source File
# Begin Source File

SOURCE=..\runtime_folder\Params\GeneralParams.txt
# End Source File
# Begin Source File

SOURCE=..\runtime_folder\Params\StrategyParams2002.txt
# End Source File
# End Group
# Begin Source File

SOURCE=..\runtime_folder\GUI.tcl
# End Source File
# Begin Source File

SOURCE=..\runtime_folder\PlayBack_GUI.tcl
# End Source File
# End Group
# Begin Group "visionTestGUI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\visionTestGUI\visionTestData.h
# End Source File
# Begin Source File

SOURCE=..\visionTestGUI\visionTestGUI.cpp
# End Source File
# Begin Source File

SOURCE=..\visionTestGUI\visionTestGUI.h
# End Source File
# Begin Source File

SOURCE=..\visionTestGUI\visionTestGUI_Commands.cpp
# End Source File
# Begin Source File

SOURCE=..\visionTestGUI\visionTestGUI_Commands.h
# End Source File
# End Group
# Begin Group "Skillz"

# PROP Default_Filter ""
# Begin Group "TestSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\TestSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\TestSkill.h
# End Source File
# End Group
# Begin Group "SimpleScreenSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\SimpleScreenSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\SimpleScreenSkill.h
# End Source File
# End Group
# Begin Group "JukeAroundOpponentSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\JukeAroundOpponentSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\JukeAroundOpponentSkill.h
# End Source File
# End Group
# Begin Group "MoveToBallSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\MoveToBallSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\MoveToBallSkill.h
# End Source File
# End Group
# Begin Group "SpinSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\SpinSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\SpinSkill.h
# End Source File
# End Group
# Begin Group "PullBallOffWallSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\PullBallOffWallSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\PullBallOffWallSkill.h
# End Source File
# End Group
# Begin Group "PickSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\PickSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\PickSkill.h
# End Source File
# End Group
# Begin Group "ManeuverWithBall"

# PROP Default_Filter "cpp h"
# Begin Source File

SOURCE=..\Skillz\ManeuverSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\ManeuverSkill.h
# End Source File
# End Group
# Begin Group "PullBallOffCornerSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\PullBallOffCornerSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\PullBallOffCornerSkill.h
# End Source File
# End Group
# Begin Group "TurnAndKickSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\TurnAndKickSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\TurnAndKickSkill.h
# End Source File
# End Group
# Begin Group "MoveAroundPickSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\MoveAroundPickSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\MoveAroundPickSkill.h
# End Source File
# End Group
# Begin Group "DribblePoint"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\DribblePointSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\DribblePointSkill.h
# End Source File
# End Group
# Begin Group "StripBallSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\StripBallSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\StripBallSkill.h
# End Source File
# End Group
# Begin Group "CutGoalSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\CutGoalSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\CutGoalSkill.h
# End Source File
# End Group
# Begin Group "SimpleDefenseSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\SimpleDefenseSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\simpleDefenseSkill.h
# End Source File
# End Group
# Begin Group "TandemDefenderSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\TandemDefenderSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\TandemDefenderSkill.h
# End Source File
# End Group
# Begin Group "TandemBlockerSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\TandemBlockerSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\TandemBlockerSkill.h
# End Source File
# End Group
# Begin Group "LooseDSpecialOpSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\LooseDSpecialOpSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\LooseDSpecialOpSkill.h
# End Source File
# End Group
# Begin Group "CreatorLookForwardPassSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\CreatorLookForwardPassSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\CreatorLookForwardPassSkill.h
# End Source File
# End Group
# Begin Group "TightDefenseBlockerSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\TightDefenseBlockerSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\TightDefenseBlockerSkill.h
# End Source File
# End Group
# Begin Group "TightDefenseDefenderSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\TightDefenseDefenderSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\TightDefenseDefenderSkill.h
# End Source File
# End Group
# Begin Group "TightDefenseSpecialOpDSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\TightDefenseSpecialOpDSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\TightDefenseSpecialOpDSkill.h
# End Source File
# End Group
# Begin Group "PassBlockSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\PassBlockSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\PassBlockSkill.h
# End Source File
# End Group
# Begin Group "JamAndShootSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\JamAndShootSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\JamAndShootSkill.h
# End Source File
# End Group
# Begin Group "BlockerGainPosessionSkill "

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\BlockerGainPossessionSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\BlockerGainPossessionSkill.h
# End Source File
# End Group
# Begin Group "AcquirePossessionSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\AcquirePossessionSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\AcquirePossessionSkill.h
# End Source File
# End Group
# Begin Group "SpecialOpCageDefenseSkill "

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\SpecialOpCageDefenseSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\SpecialOpCageDefenseSkill.h
# End Source File
# End Group
# Begin Group "LookForChipKickSkill "

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\LookForChipKickSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\LookForChipKickSkill.h
# End Source File
# End Group
# Begin Group "CrossCreasePassSkill "

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\CrossCreasePassSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\CrossCreasePassSkill.h
# End Source File
# End Group
# Begin Group "OppositeReboundOffenseSkill "

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\OppositeReboundOffenseSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\OppositeReboundOffenseSkill.h
# End Source File
# End Group
# Begin Group "MidReboundOffenseSkill  "

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\MidReboundOffenseSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\MidReboundOffenseSkill.h
# End Source File
# End Group
# Begin Group "BehindScreenOffenseSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\BehindScreenOffenseSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\BehindScreenOffenseSkill.h
# End Source File
# End Group
# Begin Group "AdvanceBallUpfieldSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\AdvanceBallUpfieldSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\AdvanceBallUpfieldSkill.h
# End Source File
# End Group
# Begin Group "StiffarmOffenseSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\StiffarmOffenseSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\StiffarmOffenseSkill.h
# End Source File
# End Group
# Begin Group "ShotBlockSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\ShotBlockSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\ShotBlockSkill.h
# End Source File
# End Group
# Begin Group "TouchShootSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\TouchShootSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\TouchShootSkill.h
# End Source File
# End Group
# Begin Group "RemoveBallSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\RemoveBallSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\RemoveBallSkill.h
# End Source File
# End Group
# Begin Group "RemoveBallScreenSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\RemoveBallScreenSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\RemoveBallScreenSkill.h
# End Source File
# End Group
# Begin Group "DribblePickScoreSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\DribblePickScoreSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\DribblePickScoreSkill.h
# End Source File
# End Group
# Begin Group "DoublePickBlockSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\DoublePickBlockSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\DoublePickBlockSkill.h
# End Source File
# End Group
# Begin Group "PassToCreatorSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\PassToCreatorSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\PassToCreatorSkill.h
# End Source File
# End Group
# Begin Group "CreatorCreateSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\CreatorCreateSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\CreatorCreateSkill.h
# End Source File
# End Group
# Begin Group "SpecialOpBlockSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\SpecialOpBlockSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\SpecialOpBlockSkill.h
# End Source File
# End Group
# Begin Group "InterceptBallSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\InterceptBallSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\InterceptBallSkill.h
# End Source File
# End Group
# Begin Group "RotateAroundBallSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\RotateAroundBallSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\RotateAroundBallSkill.h
# End Source File
# End Group
# Begin Group "ReceivePassSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\ReceivePassSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\ReceivePassSkill.h
# End Source File
# End Group
# Begin Group "CrossCreaseCreatorSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\CrossCreaseCreatorSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\CrossCreaseCreatorSkill.h
# End Source File
# End Group
# Begin Group "SimpleKickSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\SimpleKickSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\SimpleKickSkill.h
# End Source File
# End Group
# Begin Group "ReboundOffenseSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\ReboundOffenseSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\ReboundOffenseSkill.h
# End Source File
# End Group
# Begin Group "DiveSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\DiveSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\DiveSkill.h
# End Source File
# End Group
# Begin Group "ThreeManBlockerSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\ThreeManBlockerSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\ThreeManBlockerSkill.h
# End Source File
# End Group
# Begin Group "ThreeManSideSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\ThreeManSideSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\ThreeManSideSkill.h
# End Source File
# End Group
# Begin Group "CageDefenseSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\CageDefenseSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\CageDefenseSkill.h
# End Source File
# End Group
# Begin Group "RemoveBallSetupSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\RemoveBallSetupSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\RemoveBallSetupSkill.h
# End Source File
# End Group
# Begin Group "RemoveBallHelpSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\RemoveBallHelpSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\RemoveBallHelpSkill.h
# End Source File
# End Group
# Begin Group "PenaltyShotUsAimAndShoot"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\PenaltyShotUsAimAndShootSkill.h
# End Source File
# Begin Source File

SOURCE=..\Skillz\PenalytShotUsAimAndShootSkill.cpp
# End Source File
# End Group
# Begin Group "CreatorPassSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\CreatorPassSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\CreatorPassSkill.h
# End Source File
# End Group
# Begin Group "SupplmentThreeManSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\SupplementThreeManSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\SupplementThreeManSkill.h
# End Source File
# End Group
# Begin Group "FacilatePassSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\FacilatePassSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\FacilatePassSkill.h
# End Source File
# End Group
# Begin Group "BackPassSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\BackPassSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\BackPassSkill.h
# End Source File
# End Group
# Begin Group "FollowDefenderSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\FollowDefenderSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\FollowDefenderSkill.h
# End Source File
# End Group
# Begin Group "OppositeCoverSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\OppositeCoverSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\OppositeCoverSkill.h
# End Source File
# End Group
# Begin Group "SmartRotateAroundBallSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\SmartRotateAroundBallSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\SmartRotateAroundBallSkill.h
# End Source File
# End Group
# Begin Group "AggressivePullBallOffWallSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\AggressivePullBallOffWallSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\AggressivePullBallOffWallSkill.h
# End Source File
# End Group
# Begin Group "AggressivePullBallOffCornerSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\AggressivePullBallOffCornerSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\AggressivePullBallOffCornerSkill.h
# End Source File
# End Group
# Begin Group "AggressiveJamAndShootSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\AggressiveJamAndShootSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\AggressiveJamAndShootSkill.h
# End Source File
# End Group
# Begin Group "StupidJamAndShootSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\StupidJamAndShootSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\StupidJamAndShootSkill.h
# End Source File
# End Group
# Begin Group "CarefulStripBallSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\CarefulStripBallSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\CarefulStripBallSkill.h
# End Source File
# End Group
# Begin Group "PenaltySpinSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\PenaltySpinSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\PenaltySpinSkill.h
# End Source File
# End Group
# Begin Group "StrafeSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Skillz\StrafeSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\StrafeSkill.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\Skillz\RegisterSkillz.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\RegisterSkillz.h
# End Source File
# Begin Source File

SOURCE=..\Skillz\Skill.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\Skill.h
# End Source File
# Begin Source File

SOURCE=..\Skillz\SkillSet.cpp
# End Source File
# Begin Source File

SOURCE=..\Skillz\SkillSet.h
# End Source File
# Begin Source File

SOURCE=..\Skillz\SkillSetIncludes.h
# End Source File
# End Group
# Begin Group "refereeBoxInterface"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\refereeBoxInterface\RefBoxInfo.h
# End Source File
# Begin Source File

SOURCE=..\refereeBoxInterface\RefBoxInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\refereeBoxInterface\RefBoxInterface.h
# End Source File
# Begin Source File

SOURCE=..\refereeBoxInterface\SerialPortInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\refereeBoxInterface\SerialPortInterface.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\main.cpp
# End Source File
# Begin Source File

SOURCE=..\main.h
# End Source File
# Begin Source File

SOURCE=..\main_includes.h
# End Source File
# End Target
# End Project
