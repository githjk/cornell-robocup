###################
### Big Red GUI ###
###################
set fieldCalibrationMode -1

############################################################
set MainSystemCommandsText { 
                             "(F1) Turn AI On"
                             "(F1) Turn AI Off"
                             "(Ctrl-a) Toggle AI" 
                             "(Ctrl-v) Toggle VISION" 
                             "(Ctrl-w) Toggle WIRELESS" 
                               "(Ctrl-s) Switch Side" 
                             "(Ctrl-t) Switch Team"
                             "(Ctrl-f) Sample Vision Framerate"
                           }
set MainSystemCommands {
                         "Set_AI_cmd 1"
                         "Set_AI_cmd 0"
                         Toggle_AI_cmd 
                         Toggle_VISION_cmd 
                         Toggle_WIRELESS_cmd 
                         TogglePlayingSide_cmd 
                         "ToggleTeamNumber_cmd; resetManuallyPlacedObjects"
                         SampleFrameRate_cmd
                       }
set MinorSystemCommandsText {
                              "(F5) Save Log File" 
                              "(Ctrl-R) Reload All Parameter Files (Including Skill Params)" 
                              "(Ctrl-E) Reload Trajectory + Basic System Parameter Files" 
                              "Exit" 
                            }
set MinorSystemCommands {
                          doSave 
                          reloadAllParameters
                          reloadTrajectoryParameters
                          doQuit 
                        }
############################################################
set HELPLIST1 { 
                "About BigRed 2002 AI" 
                "Running AI Instructions" 
              }

set HELPLIST2 { 
                helpAboutAI 
                helpRunningInstructions 
              }
############################################################
set TOOLSLIST1 { 
				"Calibrate Field"
				"Calibrate Left Side Line"
				"Calibrate Left Goalie Box"
				"Calibrate Left Goal Post"
				"Calibrate Our Goal Line"
				"Calibrate Our Front Goalie Box"
				"Calibrate Right Goal Post"
				"Calibrate Right Goalie Box"
				"Calibrate Right Side Line"
				"Calibrate Their Left Goal Post"
				"Calibrate Their Goal Line"
				"Calibrate Their Right Goal Post"
				"Calibrate Our Left Corner"
				"Calibrate Our Right Corner"
				"Calibrate Their Left Corner"
				"Calibrate Their Right Corner"
				"Calibrate Center of Field"
                "System Quality Test" 
               }

set TOOLSLIST2 { 
				"set fieldCalibrationStep 1; set fieldCalibrationMode 0; calibrateField"
				"set fieldCalibrationStep 1; set fieldCalibrationMode 1; calibrateLine"
				"set fieldCalibrationStep 1; set fieldCalibrationMode 2; calibrateLine"
				"set fieldCalibrationStep 1; set fieldCalibrationMode 3; calibrateLine"
				"set fieldCalibrationStep 1; set fieldCalibrationMode 4; calibrateLine"
				"set fieldCalibrationStep 1; set fieldCalibrationMode 5; calibrateLine"
				"set fieldCalibrationStep 1; set fieldCalibrationMode 6; calibrateLine"
				"set fieldCalibrationStep 1; set fieldCalibrationMode 7; calibrateLine"
				"set fieldCalibrationStep 1; set fieldCalibrationMode 8; calibrateLine"
				"set fieldCalibrationStep 1; set fieldCalibrationMode 9; calibrateLine"
				"set fieldCalibrationStep 1; set fieldCalibrationMode 10; calibrateLine"
				"set fieldCalibrationStep 1; set fieldCalibrationMode 11; calibrateLine"
				"set fieldCalibrationStep 1; set fieldCalibrationMode 12; calibrateLine"
				"set fieldCalibrationStep 1; set fieldCalibrationMode 13; calibrateLine"
				"set fieldCalibrationStep 1; set fieldCalibrationMode 14; calibrateLine"
				"set fieldCalibrationStep 1; set fieldCalibrationMode 15; calibrateLine"
				"set fieldCalibrationStep 1; set fieldCalibrationMode 16; calibrateLine"
                "set DONE 1; visionQualityInfo" 
               }


############################################################

#------------------------------------------------------
set NO_POSITION                  [GetNoPosition_cmd]
set NO_PLAY                      [GetNoPlay_cmd]
#------------------------------------------------------
set START_2002_PLAYS             [GetFirst2002Play_cmd]
set END_2002_PLAYS               [GetLast2002Play_cmd]
#------------------------------------------------------
set START_2002_PENALTY_PLAYS          [GetFirst2002PenaltyPlay_cmd]
set END_2002_PENALTY_PLAYS            [GetLast2002PenaltyPlay_cmd]
#------------------------------------------------------
set START_FORMATION_PLAYS        [GetFirstFormationPlay_cmd]
set END_FORMATION_PLAYS          [GetLastFormationPlay_cmd]
#------------------------------------------------------
set START_EVAN_MALONE_SKILL_TEST_PLAYS       [GetFirstEvanMaloneSkillTestPlay_cmd]
set END_EVAN_MALONE_SKILL_TEST_PLAYS         [GetLastEvanMaloneSkillTestPlay_cmd]
#------------------------------------------------------
set START_EVAN_KUHN_SKILL_TEST_PLAYS       [GetFirstEvanKuhnSkillTestPlay_cmd]
set END_EVAN_KUHN_SKILL_TEST_PLAYS         [GetLastEvanKuhnSkillTestPlay_cmd]
#------------------------------------------------------
set START_JOEL_SKILL_TEST_PLAYS       [GetFirstJoelSkillTestPlay_cmd]
set END_JOEL_SKILL_TEST_PLAYS         [GetLastJoelSkillTestPlay_cmd]
#------------------------------------------------------
set START_NIRAV_SKILL_TEST_PLAYS       [GetFirstNiravSkillTestPlay_cmd]
set END_NIRAV_SKILL_TEST_PLAYS         [GetLastNiravSkillTestPlay_cmd]
#------------------------------------------------------
set START_WILL_SKILL_TEST_PLAYS       [GetFirstWillSkillTestPlay_cmd]
set END_WILL_SKILL_TEST_PLAYS         [GetLastWillSkillTestPlay_cmd]
#------------------------------------------------------
set START_INDIVIDUAL_PLAYS       [GetFirstIndividualPlay_cmd]
set END_INDIVIDUAL_PLAYS         [GetLastIndividualPlay_cmd]
#------------------------------------------------------
set START_INTIMIDATION_PLAYS     [GetFirstIntimidationPlay_cmd]
set END_INTIMIDATION_PLAYS       [GetLastIntimidationPlay_cmd]
#------------------------------------------------------
set START_OTHER_PLAYS            [GetFirstOtherPlay_cmd]
set END_OTHER_PLAYS              [GetLastOtherPlay_cmd]
#------------------------------------------------------
set CANVAS_WIDTH                 500
set CANVAS_HEIGHT                635

set FIELD_X                      [expr $CANVAS_WIDTH / 2]
set FIELD_Y                      [expr $CANVAS_HEIGHT / 2]
set FIELD_FACTOR                 2
set METER_CONVERSION             100
set REFRESH_DELAY                5
set THEIR_ROBOT_RADIUS           8.0
set BALL_RADIUS                  2.0

set aiStatus                     ""
set wirelessStatus               ""
set visionStatus                 ""
set sideValue                    ""
set teamNumber                   ""
set frameNumber                  ""

set saveFile                     "log.txt"

set minLatency                   ""
set maxLatency                   ""
set meanLatency                  ""

set frameRate                    ""

set framesAnalized               30
set DONE                         0
set updateTimingData             1
set currentVisionQualityFrame    0
set currentframeAnalized         [visionQualityFrame_cmd]

#if a robot sets it's pass value, then display it
set automaticDisplayPassDest     1

#set to to display zone/sector lines
set displayZoneSectorLines 1
set displayFieldLines 1
set complexMode [Get_Complex_Setting_cmd]
set toggleComplexity             0

for {set i 0} { $i < [GetNumRobots_cmd]} {incr i} \
{
  set ourXPos$i                     ""
  set ourYPos$i                     ""
  set ourAngPos$i                   ""
  set ourXVel$i                     ""
  set ourYVel$i                     ""
  set ourXCommand$i                 ""
  set ourYCommand$i                 ""
  set ourAngCommand$i               ""
  set dribber$i                     ""
  set ourKicker$i                   ""
  set ourRobotFound$i               ""
  set ourMessage$i                  ""
  set ourRobotDestX$i               ""
  set ourRobotDestY$i               ""
  set ourRobotPassDestX$i           ""
  set ourRobotPassDestY$i           ""
  set ourRobotPassValue$i           ""
  set displayRobotDestination$i     1
  set displayRobotPassDestination$i ""
  set theirXPos$i                   ""
  set theirYPos$i                   ""
  set theirXVel$i                   ""
  set theirYVel$i                   ""
  set theirAngPos$i                 ""
  set theirRobotFound$i             ""
}

#---------------
#---------------
#---------------
# Used to manually specify the location of a friendly or opponent robot
set objectPositionSpecified        -1
set objectPositionSpecifiedEnabled -1
set objectConverted                -1
set objectSpecifiedX                0
set objectSpecifiedY                0
set dragOffset                      0
set xActual                         0
set yActual                         0
#---------------
#these variables are used to set a robot's
#location when vision and ai is turned off
for {set ID 0} { $ID < [GetNumRobots_cmd]} {incr ID} \
{
  set friendlySetPositionX$ID        -32000
  set friendlySetPositionY$ID        -32000
  set friendlySetPositionRotation$ID -32000

  set opponentSetPositionX$ID        -32000
  set opponentSetPositionY$ID        -32000

}
  set ballSetPositionX            -32000
  set ballSetPositionY            -32000    
#---------------
#---------------
#---------------

set fieldCalibrationStep 0
set fieldCalibrateMode 0
set normalBallMode 1
set flipBack 0
set ballXPos ""
set ballYPos ""
set ballXVel ""
set ballYVel ""

#intercept the close window function
wm protocol . WM_DELETE_WINDOW { doQuit }

bind . <space>        { Set_AI_cmd 0 }
bind . <Return>       { calibrateField }
bind . <F1>           { Set_AI_cmd 1 }
bind . <Control-o>    { SetPlay_cmd 1 }

bind . <Control-r>    { reloadAllParameters }
bind . <Control-e>    { reloadTrajectoryParameters }

bind . <Control-a>    { Toggle_AI_cmd }
bind . <Control-v>    { Toggle_VISION_cmd }
bind . <Control-w>    { Toggle_WIRELESS_cmd }
bind . <Control-s>    { TogglePlayingSide_cmd; eraseField; drawField }

bind . <Control-F5>   { doSave }
bind . <F5>           { doQuickSave }
bind . <Control-t>    { ToggleTeamNumber_cmd; resetManuallyPlacedObjects }

bind . <F8>           { ResetPositions_cmd }

# These keys reserved for SampleVisionSpeed
# and maybe LatencyTest
bind . <F11>          { offenseCommand }
bind . <F12>          { defenseCommand }

bind . <Control-l>    { SetPlay_cmd [GetLatencyPlay_cmd] }
bind . <Control-p>    { SetPlay_cmd [GetPenaltyShotUsFormationPlay_cmd]; Set_AI_cmd 1; }

bind . <Control-f>    { SampleFrameRate_cmd }

#beware! F10 acts funky if you bind it to something


bind . <q> { set objectPositionSpecified "Set Friendly Robot 0 Location (q)" }
bind . <w> { set objectPositionSpecified "Set Friendly Robot 1 Location (w)" }
bind . <e> { set objectPositionSpecified "Set Friendly Robot 2 Location (e)" }
bind . <r> { set objectPositionSpecified "Set Friendly Robot 3 Location (r)" }
bind . <t> { set objectPositionSpecified "Set Friendly Robot 4 Location (t)" }
bind . <a> { set objectPositionSpecified "Set Friendly Robot 0 Rotation (a)" }
bind . <s> { set objectPositionSpecified "Set Friendly Robot 1 Rotation (s)" }
bind . <d> { set objectPositionSpecified "Set Friendly Robot 2 Rotation (d)" }
bind . <f> { set objectPositionSpecified "Set Friendly Robot 3 Rotation (f)" }
bind . <g> { set objectPositionSpecified "Set Friendly Robot 4 Rotation (g)" }

bind . <z> { set objectPositionSpecified "Set Opponent Robot 0 Location (z)" }
bind . <x> { set objectPositionSpecified "Set Opponent Robot 1 Location (x)" }
bind . <c> { set objectPositionSpecified "Set Opponent Robot 2 Location (c)" }
bind . <v> { set objectPositionSpecified "Set Opponent Robot 3 Location (v)" }
bind . <b> { set objectPositionSpecified "Set Opponent Robot 4 Location (b)" }

bind . <`> { set objectPositionSpecified "Set Ball Location (`)" }

bind . <Alt-f> \
{ 
  global displayFieldLines
  if { $displayFieldLines == 1 } \
  { \
     set displayFieldLines 0
  } \
  else \
  {
     set displayFieldLines 1   
  }
  eraseField
  drawField
}

bind . <Alt-s> \
{
  global displayZoneSectorLines
  if { $displayZoneSectorLines == 1 } \
  { \
     set displayZoneSectorLines 0
  } \
  else \
  {
     set displayZoneSectorLines 1  
  }
  eraseField
  drawField
}

bind . <Alt-t> \
{
  global toggleComplexity
  if { $toggleComplexity == 1 } \
  { \
     set toggleComplexity 0
  } \
  else \
  {
     set toggleComplexity 1  
  }
}

############################################
####### FUNCTION DECLARATIONS BEGINS #######
############################################

####### MENU COMMANDS ##################
proc reloadAllParameters {} \
{
  ReloadParameters_cmd
  eraseField
  drawField
}
proc reloadTrajectoryParameters {} \
{
  ReloadTrajectoryParameters_cmd
  drawField
}

# This procedure calculates number^index
proc offenseCommand {} \
{
  puts "offenseCommand"
}

proc defenseCommand {} \
{
  puts "defenseCommand"
}

proc goalieBallCommand {} \
{
  puts "goalieBallCommand"
}

proc Update_Screen {} \
{
  printDebugMessages

  convertSetVariable

  ####################
  global toggleComplexity
  global complexMode

  if { $toggleComplexity == 1 } \
  {
     if { $complexMode == 1 } \
     {
        set complexMode 0
     } \
     else \
     {
        set complexMode 1
     }
     set toggleComplexity 0
     destroy .mbar
     drawMenus
  }
  ###########################
  global objectPositionSpecified
  global objectPositionSpecifiedEnabled
  global objectConverted
  global objectSpecifiedX
  global objectSpecifiedY
  ###########################
  #these variables are used to set a robot's
  #location when vision and ai is turned off
  for {set ID 0} { $ID < [GetNumRobots_cmd]} {incr ID} \
  {
    global friendlySetPositionX$ID        
    global friendlySetPositionY$ID     
    global friendlySetPositionRotation$ID

    global opponentSetPositionX$ID      
    global opponentSetPositionY$ID     

    global ballSetPositionX       
    global ballSetPositionY
  }
  global FIELD_X
  global FIELD_Y
  global FIELD_FACTOR
  global xActual
  global yActual
  global dragOffset
  set xActual [expr [expr [expr $objectSpecifiedY + $dragOffset - $FIELD_Y] / [expr -1.0 * $FIELD_FACTOR] ] / 100.0]
  set yActual [expr [expr [expr $objectSpecifiedX - $dragOffset - $FIELD_X] / [expr -1.0 * $FIELD_FACTOR] ] / 100.0]
  ###########################
  #in order to calculate the rotation, first grab this robots x and y position
  #by default
  set manualRotation 0
  if { $objectConverted >= 5 && $objectConverted < 10} \
  {
    #calculate this robots ID
    set theID [expr $objectConverted - 5]

    #x1/y1 is the robot's position
    global ourXPos$theID
    global ourYPos$theID
    set x1 ourXPos$theID
    set y1 ourYPos$theID


    #x2/y2 is the circles position
    set x2 $xActual
    set y2 $yActual

    set A [expr $x2 - $$x1]
    set B [expr $y2 - $$y1]

    set manualRotation [expr atan2($B, $A)]
  }
  ###########################
  #if we are specifying an object position or rotation, then set it
  if { $objectConverted != -1 } \
  {
     if { $objectConverted > 4 &&
          $objectConverted < 10 } \
     {
        SetRobotRotation_cmd $objectConverted $manualRotation
     } \
     else \
     {
        SetRobotLocation_cmd $objectConverted $xActual $yActual
     }
  }
  ###########################

  #######
  global BACKGROUNDCOLOR
  global NO_OF_ROBOTS
  global framesAnalized
  global DONE
  global currentVisionQualityFrame

  for {set i 0} { $i < [GetNumRobots_cmd]} {incr i} \
  {
    global ourXPos$i
      global ourYPos$i
      global ourAngPos$i
      global ourXVel$i
      global ourYVel$i
      global ourXCommand$i
      global ourYCommand$i
      global ourAngCommand$i
      global ourDribbler$i
      global ourKicker$i
      global ourRobotFound$i
      global ourMessage$i
      global ourRobotRole$i
      global ourRobotType$i
    global ourRobotName$i
      global ourRobotDestX$i
      global ourRobotDestY$i
      global ourRobotPassDestX$i
      global ourRobotPassDestY$i
    global ourRobotPassValue$i
    global displayRobotDestination$i
    global displayRobotPassDestination$i

      global theirXPos$i
      global theirYPos$i
      global theirXVel$i
      global theirYVel$i
      global theirAngPos$i
      global theirRobotFound$i
  }

  global automaticDisplayPassDest

  global playName
  global normalBallMode
  global ballXPos
  global ballYPos
  global ballXVel
  global ballYVel
  global METER_CONVERSION
  global aiStatus
  global visionStatus
  global wirelessStatus
  global sideValue
  global teamNumber
  global frameNumber
  global minLatency
  global maxLatency
  global meanLatency
  global frameRate

  #update all robots
  for {set i 0} { $i < [GetNumRobots_cmd]} {incr i} \
  {
    set ourXPos$i              [GetOurRobotX_cmd $i]
    set ourYPos$i              [GetOurRobotY_cmd $i]
    set ourAngPos$i            [GetOurRobotRotation_cmd $i]
    set ourXCommand$i          [GetOurRobotSentXVel_cmd $i]
    set ourYCommand$i          [GetOurRobotSentYVel_cmd $i]
    set ourAngCommand$i        [GetOurRobotSentRotation_cmd $i]
    set ourMessage$i           [GetRobotMessage_cmd $i]
    set ourRobotRole$i         [GetRobotPosition_cmd $i]
	set ourRobotType$i         [GetRobotType_cmd $i]
    set ourRobotName$i         [GetRobotName_cmd $i]
    set ourRobotDestX$i        [GetOurRobotDestinationX_cmd $i]
    set ourRobotDestY$i        [GetOurRobotDestinationY_cmd $i]
    set ourRobotDestRot$i        [GetOurRobotDestinationRot_cmd $i]
    set ourRobotPassDestX$i    [GetOurRobotPassDestinationX_cmd $i]
    set ourRobotPassDestY$i    [GetOurRobotPassDestinationY_cmd $i]
    set ourRobotPassValue$i    [GetOurRobotPassValue_cmd $i]

    set theirXPos$i            [GetTheirRobotX_cmd $i]
      set theirYPos$i            [GetTheirRobotY_cmd $i]
      set theirXVel$i            [GetTheirRobotXVel_cmd $i]
      set theirYVel$i            [GetTheirRobotYVel_cmd $i]
      set theirAngPos$i          0
      set ourDribbler$i          [booleanText [GetRobotDribble_cmd $i]]
      set ourKicker$i            [booleanText [GetRobotKick_cmd $i]]



    ###################################################################
    # set background of current play
    ###################################################################
    if { [CurrentPlayActive_cmd] == 1 } \
    {
        .playFrame.top.l1 configure -bg "green"
        .playFrame.top.l2 configure -bg "green"
    } \
    elseif { [CurrentPlayActive_cmd] == 2 } \
    {
        .playFrame.top.l1 configure -bg "darkgreen"
        .playFrame.top.l2 configure -bg "darkgreen"
    } \
    elseif { [CurrentPlayActive_cmd] == 3 } \
    {
        .playFrame.top.l1 configure -bg "red"
        .playFrame.top.l2 configure -bg "red"
    } \
    elseif { [CurrentPlayActive_cmd] == 4 } \
    {
        .playFrame.top.l1 configure -bg "darkred"
        .playFrame.top.l2 configure -bg "darkred"
    }
    .playFrame.top.l1 configure -fg "white"
    .playFrame.top.l2 configure -fg "white"
    ###################################################################

    ###################################################################
    ### SET BACKGROUND TO RED IF VALUE WAS NOT FOUND!!!
    ###################################################################
    ###################################################################
    if { [GetOurRobotFound_cmd $i] == 1 } \
    {
        .bigRedFrame.robot$i.titleFrame.l4 configure -bg $BACKGROUNDCOLOR
        .bigRedFrame.robot$i.titleFrame.l6 configure -bg $BACKGROUNDCOLOR
      } \
    else \
    {
          .bigRedFrame.robot$i.titleFrame.l4 configure -bg [booleanColor 0]
          .bigRedFrame.robot$i.titleFrame.l6 configure -bg [booleanColor 0]
          set ourMessage$i "Not Found"
    }

    if { [GetRoboCupBallFound_cmd] == 0 } \
    {
       .ballFrame.l2 configure -bg [booleanColor 0]
       .ballFrame.l4 configure -bg [booleanColor 0]
    } \
    else \
    {
       .ballFrame.l2 configure -bg $BACKGROUNDCOLOR
       .ballFrame.l4 configure -bg $BACKGROUNDCOLOR
    }

    if { [GetTheirRobotFound_cmd $i] == 0 } \
    {
          .adversaryFrame.robot$i.l2 configure -bg [booleanColor 0]
          .adversaryFrame.robot$i.l4 configure -bg [booleanColor 0]
    } \
    else \
    {
          .adversaryFrame.robot$i.l2 configure -bg $BACKGROUNDCOLOR
          .adversaryFrame.robot$i.l4 configure -bg $BACKGROUNDCOLOR 
    }

    if {[GetRobotDribble_cmd $i] == 1 } \
    {
          .bigRedFrame.robot$i.positionFrame.l8 configure -bg [booleanColor 1]
    } \
    else \
    {
          .bigRedFrame.robot$i.positionFrame.l8 configure -bg [booleanColor 0]
      }

    if {[GetRobotKick_cmd $i] == 1} \
    {
          .bigRedFrame.robot$i.sendFrame.l8 configure -bg [booleanColor 1]
      } \
    else \
    {
          .bigRedFrame.robot$i.sendFrame.l8 configure -bg [booleanColor 0]
      }
    ###################################################################
  
    ###################################################################
    ### SET BACKGROUND TO YELLOW IF VALUE WAS INVALID!!!
    ###################################################################
    set tempString ourXPos
    if { [expr $$tempString[expr $i]] == -64000 } \
    {
       .bigRedFrame.robot$i.positionFrame.l2 configure -bg [booleanColor 3]
    } \
    else \
    {
       .bigRedFrame.robot$i.positionFrame.l2 configure -bg $BACKGROUNDCOLOR
    }
    set tempString ourYPos
    if { [expr $$tempString[expr $i]] == -64000 } \
    {
       .bigRedFrame.robot$i.positionFrame.l4 configure -bg [booleanColor 3]
    } \
    else \
    {
       .bigRedFrame.robot$i.positionFrame.l4 configure -bg $BACKGROUNDCOLOR
    }
    set tempString ourAngPos
    if { [expr $$tempString[expr $i]] == -64000 } \
    {
       .bigRedFrame.robot$i.positionFrame.l6 configure -bg [booleanColor 3]
    } \
    else \
    {
       .bigRedFrame.robot$i.positionFrame.l6 configure -bg $BACKGROUNDCOLOR
    }
    set tempString ourXCommand
    if { [expr $$tempString[expr $i]] == -64000 } \
    {
       .bigRedFrame.robot$i.sendFrame.l2 configure -bg [booleanColor 3]
    } \
    else \
    {
       .bigRedFrame.robot$i.sendFrame.l2 configure -bg $BACKGROUNDCOLOR
    }
    set tempString ourYCommand
    if { [expr $$tempString[expr $i]] == -64000 } \
    {
       .bigRedFrame.robot$i.sendFrame.l4 configure -bg [booleanColor 3]
    } \
    else \
    {
       .bigRedFrame.robot$i.sendFrame.l4 configure -bg $BACKGROUNDCOLOR
    }
    set tempString ourAngCommand
    if { [expr $$tempString[expr $i]] == -64000 } \
    {
       .bigRedFrame.robot$i.sendFrame.l6 configure -bg [booleanColor 3]
    } \
    else \
    {
       .bigRedFrame.robot$i.sendFrame.l6 configure -bg $BACKGROUNDCOLOR
    }
    ###################################################################
    if { $ballXPos == -64000 } \
    {
       .ballFrame.l2 configure -bg [booleanColor 3]
    } 
    if { $ballYPos == -64000 } \
    {
       .ballFrame.l4 configure -bg [booleanColor 3]
    } 
    if { $ballXVel == -64000 } \
    {
       .ballFrame.l6 configure -bg [booleanColor 3]
    } \
    else \
    {
       .ballFrame.l6 configure -bg $BACKGROUNDCOLOR
    } 
    if { $ballYVel == -64000 } \
    {
       .ballFrame.l8 configure -bg [booleanColor 3]
    } \
    else \
    {
       .ballFrame.l8 configure -bg $BACKGROUNDCOLOR
    } 
    ###################################################################
    set tempString theirXPos
    if { [expr $$tempString[expr $i]] == -64000 } \
    {
       .adversaryFrame.robot$i.l2 configure -bg [booleanColor 3]
    } 
    set tempString theirYPos
    if { [expr $$tempString[expr $i]] == -64000 } \
    {
       .adversaryFrame.robot$i.l4 configure -bg [booleanColor 3]
    } 
    set tempString theirXVel
    if { [expr $$tempString[expr $i]] == -64000 } \
    {
       .adversaryFrame.robot$i.l6 configure -bg [booleanColor 3]
    } \
    else \
    {
       .adversaryFrame.robot$i.l6 configure -bg $BACKGROUNDCOLOR
    } 
    set tempString theirYVel
    if { [expr $$tempString[expr $i]] == -64000 } \
    {
       .adversaryFrame.robot$i.l8 configure -bg [booleanColor 3]
    } \
    else \
    {
       .adversaryFrame.robot$i.l8 configure -bg $BACKGROUNDCOLOR
    } 
    
  }


  .cc delete OUR_ROBOT0
  drawOurRobot [expr $METER_CONVERSION * $ourXPos0] \
               [expr $METER_CONVERSION * $ourYPos0] \
               $ourAngPos0 \
               [expr $METER_CONVERSION * $ourRobotDestX0] \
               [expr $METER_CONVERSION * $ourRobotDestY0] \
               $ourRobotDestRot0 \
               [expr $METER_CONVERSION * $ourRobotPassDestX0] \
               [expr $METER_CONVERSION * $ourRobotPassDestY0] \
               OUR_ROBOT0 \
               0 \
               $displayRobotDestination0 \
               $displayRobotPassDestination0 \
               $ourRobotPassValue0

  .cc delete OUR_ROBOT1
  drawOurRobot [expr $METER_CONVERSION * $ourXPos1] \
               [expr $METER_CONVERSION * $ourYPos1] \
               $ourAngPos1 \
               [expr $METER_CONVERSION * $ourRobotDestX1] \
               [expr $METER_CONVERSION * $ourRobotDestY1] \
               $ourRobotDestRot1 \
               [expr $METER_CONVERSION * $ourRobotPassDestX1] \
               [expr $METER_CONVERSION * $ourRobotPassDestY1] \
               OUR_ROBOT1 \
               1 \
               $displayRobotDestination1 \
               $displayRobotPassDestination1 \
               $ourRobotPassValue1

  .cc delete OUR_ROBOT2
  drawOurRobot [expr $METER_CONVERSION * $ourXPos2] \
               [expr $METER_CONVERSION * $ourYPos2] \
               $ourAngPos2 \
               [expr $METER_CONVERSION * $ourRobotDestX2] \
               [expr $METER_CONVERSION * $ourRobotDestY2] \
               $ourRobotDestRot2 \
               [expr $METER_CONVERSION * $ourRobotPassDestX2] \
               [expr $METER_CONVERSION * $ourRobotPassDestY2] \
               OUR_ROBOT2 \
               2 \
               $displayRobotDestination2 \
               $displayRobotPassDestination2 \
               $ourRobotPassValue2

  .cc delete OUR_ROBOT3
  drawOurRobot [expr $METER_CONVERSION * $ourXPos3] \
               [expr $METER_CONVERSION * $ourYPos3] \
               $ourAngPos3 \
               [expr $METER_CONVERSION * $ourRobotDestX3] \
               [expr $METER_CONVERSION * $ourRobotDestY3] \
               $ourRobotDestRot3 \
               [expr $METER_CONVERSION * $ourRobotPassDestX3] \
               [expr $METER_CONVERSION * $ourRobotPassDestY3] \
               OUR_ROBOT3 \
               3 \
               $displayRobotDestination3 \
               $displayRobotPassDestination3 \
               $ourRobotPassValue3

  .cc delete OUR_ROBOT4
  drawOurRobot [expr $METER_CONVERSION * $ourXPos4] \
               [expr $METER_CONVERSION * $ourYPos4] \
               $ourAngPos4 \
               [expr $METER_CONVERSION * $ourRobotDestX4] \
               [expr $METER_CONVERSION * $ourRobotDestY4] \
               $ourRobotDestRot4 \
               [expr $METER_CONVERSION * $ourRobotPassDestX4] \
               [expr $METER_CONVERSION * $ourRobotPassDestY4] \
               OUR_ROBOT4 \
               4 \
               $displayRobotDestination4 \
               $displayRobotPassDestination4 \
               $ourRobotPassValue4

  .cc delete THEIR_ROBOT0
  drawTheirRobot [expr $METER_CONVERSION*$theirXPos0] [expr $METER_CONVERSION*$theirYPos0] THEIR_ROBOT0 5
  .cc delete THEIR_ROBOT1
  drawTheirRobot [expr $METER_CONVERSION*$theirXPos1] [expr $METER_CONVERSION*$theirYPos1] THEIR_ROBOT1 6
  .cc delete THEIR_ROBOT2
  drawTheirRobot [expr $METER_CONVERSION*$theirXPos2] [expr $METER_CONVERSION*$theirYPos2] THEIR_ROBOT2 7
  .cc delete THEIR_ROBOT3
  drawTheirRobot [expr $METER_CONVERSION*$theirXPos3] [expr $METER_CONVERSION*$theirYPos3] THEIR_ROBOT3 8
  .cc delete THEIR_ROBOT4
  drawTheirRobot [expr $METER_CONVERSION*$theirXPos4] [expr $METER_CONVERSION*$theirYPos4] THEIR_ROBOT4 9

  # update the ball
  if { $normalBallMode == 1 } \
  {
    set ballXPos [GetBallX_cmd]
    set ballYPos [GetBallY_cmd]
  } \
  else \
  {
    set ballXPos [GetBallRawX_cmd]
    set ballYPos [GetBallRawY_cmd]
  }
  
  set ballXVel [GetBallXVel_cmd]
  set ballYVel [GetBallYVel_cmd]
  .cc delete BALL
  drawOneBall [expr $METER_CONVERSION*$ballXPos] [expr $METER_CONVERSION*$ballYPos]

  #update all debug points
  .cc delete DEBUG_POINT
  for {set i 0} { $i < [GetNumRobots_cmd]} {incr i} \
  {
    for {set j 0} { $j < [getNumDebugPoints]} {incr j} \
    {
      set debugPointX [getDebugPointX $i $j]
      set debugPointY [getDebugPointY $i $j]
      if { \
           $debugPointX != -32000 && \
           $debugPointY != -32000 \
         } \
      {
        drawDebugPoint [expr $METER_CONVERSION*$debugPointX] \
                       [expr $METER_CONVERSION*$debugPointY] \
                       $i \
                       $j
      }
    }
  }

  # update system status
  set aiStatus [booleanText [GetAIStatus_cmd]]
  set wirelessStatus [booleanText [GetWirelessStatus_cmd]]
  set visionStatus [booleanText [GetVisionStatus_cmd]]
  set sideValue [GetSidesFlippedStatus_cmd]
  set teamNumber [expr [GetTeamNumber_cmd] + 1]
  set playName [GetCurrentPlayName_cmd]

  set minLatency [GetMinLatency_cmd]
  set maxLatency [GetMaxLatency_cmd]
  set meanLatency [GetMeanLatency_cmd]
  

  set frameRate [GetFrameRate_cmd]

  set frameNumber [GetCurrentFrame_cmd]

  #################################################
  if { $aiStatus == "On"} \
  {
      .statusFrame.top.l2 configure -bg [booleanColor 1]
  } \
  else \
  {
      .statusFrame.top.l2 configure -bg [booleanColor 0]
  }
  #################################################
  if { $wirelessStatus == "On"} \
  {
      .statusFrame.middle.l2 configure -bg [booleanColor 1]
  } \
  else \
  {
      .statusFrame.middle.l2 configure -bg [booleanColor 0] 
  }
  #################################################
  if { $visionStatus == "On"} \
  {
      .statusFrame.bottom.l2 configure -bg [booleanColor 1]
  } \
  else \
  {
      .statusFrame.bottom.l2 configure -bg [booleanColor 0]
  }
  #################################################
  #if latency test is done, display results
  if { [IsLatencyTestDone_cmd] == 1 } \
  {
    displayLatencyTestResults
  }
  ##################################################
  #if object toggled on, but circle not displayed yet, and
  #vision turned off, set object pointer and default position

  if { \
       $objectPositionSpecified != $objectPositionSpecifiedEnabled && \
       $visionStatus == "Off" \
     } \
  {
    #if the current object location is not -32000,-32000, then reuse it!
    convertSetVariable

    set ourX ourXPos
    set ourY ourYPos
    set theirX theirXPos
    set theirY theirYPos

    global objectConverted

######################
  global FIELD_X
  global FIELD_Y
  global FIELD_FACTOR
  global dragOffset

  if { $objectConverted >= 0 && $objectConverted <= 4} \
  {
     ###offset for set our robot position
     set dragOffset 12
  } \
  elseif { $objectConverted >= 5 && $objectConverted <= 9} \
  {
     ###offset for set our robot rotation
     set dragOffset 0
  } \
  elseif { $objectConverted >= 11 && $objectConverted <= 15} \
  {
     ###offset for set their robot position
     set dragOffset 9
  } \
  elseif { $objectConverted == 10 } \
  {
     ###offset for set our ball position
     set dragOffset 5
  } 

  set defaultX [expr 200 + $dragOffset]
  set defaultY [expr 314 - $dragOffset]
   
  
  ######
  ## If our robot is already placed, use it's last position as the new position
  #####
  if { $objectConverted >= 0 && \
       $objectConverted <= 4 && \
       [expr $$ourX$objectConverted] != -32000} \
  {
    set defaultX [expr [expr -100.0 * $FIELD_FACTOR * [expr $$ourY$objectConverted]] + $FIELD_X + $dragOffset]
    set defaultY [expr [expr -100.0 * $FIELD_FACTOR * [expr $$ourX$objectConverted]] + $FIELD_Y - $dragOffset]
  }

  ######
  ## If opponent robot is already placed, use it's last position as the new position
  #####
  if { $objectConverted >= 11 && \
       $objectConverted <= 15 && \
       [expr $$theirX[expr $objectConverted - 11]] != -32000 } \
  {
    set defaultX [expr [expr -100.0 * $FIELD_FACTOR * [expr $$theirY[expr $objectConverted - 11]]] + $FIELD_X + $dragOffset]
    set defaultY [expr [expr -100.0 * $FIELD_FACTOR * [expr $$theirX[expr $objectConverted - 11]]] + $FIELD_Y - $dragOffset]
  }  

  ######
  ## If ball is already placed, use it's last position as the new position
  #####
  set ballXPos [GetBallX_cmd]
  set ballYPos [GetBallY_cmd]
  if { $objectConverted == 10 && \
       [GetRoboCupBallFound_cmd] != 0 } \
  {
    ##############################
    #I can't get this to work. :(
    ##############################
    #set defaultX [expr [expr -100.0 * $FIELD_FACTOR * $ballYPos] + $FIELD_X + $dragOffset]
    #set defaultY [expr [expr -100.0 * $FIELD_FACTOR * $ballXPos] + $FIELD_Y - $dragOffset]
  }  


######################

    #set default position of the object
    set objectSpecifiedX $defaultX
    set objectSpecifiedY $defaultY

    #if object does not already exist
    if { $objectPositionSpecifiedEnabled != -1 } \
    {
      .cc delete theOval
    }

    createOval
    #set pointer to correct object
    set objectPositionSpecifiedEnabled $objectPositionSpecified  
  }

  #if the vision system is turned on, quickly delete the
  #object if it exists, and if we are trying to turn it on, quickly avoid that!
  if { \
        $visionStatus == "On" \
     } \
  {
    if { $objectPositionSpecifiedEnabled != -1 } \
    {
      .cc delete theOval
    }

    set objectPositionSpecifiedEnabled -1
    set objectPositionSpecified -1
  }

##################################################
####END proc Update_Screen #######################
##################################################

#Sleep, then do it all again =-)

#if running a vision quality test, slow down this update procedure and allow the other one to be fast
if { $DONE == 0 } \
{
  after 300 Update_Screen
}

}

proc booleanText {a_integer} \
{
    set bool ""
    if {$a_integer == 1} \
  {
        set bool On
    } \
  else \
  {
        set bool Off
    }
    set bool
}

proc booleanColor {a_integer} \
{
    set bool ""
    if {$a_integer == 0} \
  {
        set bool red
    } \
  elseif {$a_integer == 1} \
  {
        set bool green
    } \
  elseif {$a_integer == 2} \
  {
        set bool blue
    } \
  else \
  {
        set bool yellow
    }
    set bool
}

###### DRAWING COMMANDS #################
# These procedures take x and y coordinates
# traverses or rotates them about angle
proc traverse_x {x_coord x_delta} \
{
    expr $x_coord + $x_delta
}

proc traverse_y {y_coord y_delta} \
{
    expr $y_coord + $y_delta
}

proc rotate_x {x_coord y_coord degAngle {x_coord_center 0} {y_coord_center 0}} \
{
    expr [expr cos($degAngle)] * ($x_coord-$x_coord_center) - \
           [expr sin($degAngle)] * ($y_coord-$y_coord_center) - \
       $x_coord_center
}

proc rotate_y {x_coord y_coord degAngle {x_coord_center 0} {y_coord_center 0}} \
{
    expr [expr sin($degAngle)] * ($x_coord-$x_coord_center) + \
           [expr cos($degAngle)] * ($y_coord-$y_coord_center) - \
       $y_coord_center
}

# This procedure actually creates the picture elements in the canvas
# depending on the value of the drawtype
proc dodraw {drawtype tagName wid x1 y1 x2 y2 color} \
{
  set X_SHIFT -1
  set x1 [expr $x1 + $X_SHIFT]
  set x2 [expr $x2 + $X_SHIFT]

  global FIELD_X
  global FIELD_Y
  global FIELD_FACTOR

  switch $drawtype \
  {
    line { \
            .cc create line [expr $FIELD_FACTOR * -$y1 + $FIELD_X] \
                            [expr $FIELD_FACTOR * -$x1 + $FIELD_Y] \
                            [expr $FIELD_FACTOR * -$y2 + $FIELD_X] \
                            [expr $FIELD_FACTOR * -$x2 + $FIELD_Y] \
                            -width $wid \
                            -tags $tagName \
                            -fill $color \
         }

    dribbler { \
               .cc create line [expr $FIELD_FACTOR * -$y1 + $FIELD_X] \
                               [expr $FIELD_FACTOR * -$x1 + $FIELD_Y] \
                                   [expr $FIELD_FACTOR * -$y2 + $FIELD_X] \
                               [expr $FIELD_FACTOR * -$x2 + $FIELD_Y] \
                               -width $wid \
                               -tags $tagName \
                               -fill $color \
             }

    rect { \
            .cc create rectangle [expr 2* -$y1+$FIELD_X] \
                                 [expr 2 * -$x1+$FIELD_Y] \
                                     [expr $FIELD_FACTOR * -$y2 + $FIELD_X] \
                                 [expr $FIELD_FACTOR* -$x2+$FIELD_Y] \
                                 -width $wid \
                                 -tags $tagName \
                                 -fill $color \
         }

    ball { \
           .cc create oval [expr 2* -$y1+$FIELD_X] \
                           [expr $FIELD_FACTOR* -$x1+$FIELD_Y] \
                               [expr $FIELD_FACTOR* -$y2+ $FIELD_X] \
                           [expr $FIELD_FACTOR* -$x2+$FIELD_Y] \
                           -width $wid \
                           -tags $tagName \
                           -outline red \
                           -fill $color \
         }
    debugPoint { \
           .cc create oval [expr 2* -$y1+$FIELD_X] \
                           [expr $FIELD_FACTOR* -$x1+$FIELD_Y] \
                           [expr $FIELD_FACTOR* -$y2+ $FIELD_X] \
                           [expr $FIELD_FACTOR* -$x2+$FIELD_Y] \
                           -width $wid \
                           -tags $tagName \
                           -outline blue \
                           -fill $color \
         }

     oval { \
             .cc create oval [expr 2* -$y1+$FIELD_X] \
                             [expr $FIELD_FACTOR* -$x1+$FIELD_Y] \
                                 [expr $FIELD_FACTOR* -$y2+ $FIELD_X] \
                             [expr $FIELD_FACTOR* -$x2+$FIELD_Y] \
                             -width $wid \
                             -tags $tagName \
                             -outline $color
          }
  }
}
########################################
# This procedure erases the field
proc eraseField {} \
{
  .cc delete FIELD_BOUNDARY
  .cc delete LEFT_SIDE_LINE
  .cc delete RIGHT_SIDE_LINE
  .cc delete OUR_SIDE_LEFT
  .cc delete OUR_SIDE_RIGHT
  .cc delete THEIR_SIDE_LEFT
  .cc delete THEIR_SIDE_RIGHT
  .cc delete OUR_GOAL_POST
  .cc delete OUR_GOAL_BOX_LEFT
  .cc delete OUR_GOAL_BOX_RIGHT
  .cc delete OUR_GOAL_BOX_BACK
  .cc delete THEIR_GOAL_BOX
  .cc delete THEIR_GOAL_BOX_LEFT
  .cc delete THEIR_GOAL_BOX_RIGHT
  .cc delete THEIR_GOAL_BOX_BACK
  .cc delete HALF_LINE
  .cc delete CENTER_CIRCLE
  .cc delete OUR_GOAL_BOX_FRONT
  .cc delete THEIR_GOAL_BOX_FRONT
  .cc delete OUR_DEFENCE_ZONE_LEFT
  .cc delete OUR_DEFENCE_ZONE_RIGHT
  .cc delete OUR_DEFENCE_ZONE_FRONT
  .cc delete OUR_DEFENCE_ZONE_LEFT
  .cc delete OUR_DEFENCE_ZONE_RIGHT
  .cc delete OUR_DEFENCE_ZONE_FRONT
  .cc delete OFFENSE_ZONE_LINE
  .cc delete KILL_ZONE_LINE
  .cc delete DEFENSE_ZONE_LINE
  .cc delete DEATH_ZONE_LINE
  .cc delete LEFT_SWEET_SECTOR
  .cc delete RIGHT_SWEET_SECTOR
  .cc delete LEFT_MIDDLE_SECTOR
  .cc delete RIGHT_MIDDLE_SECTOR
  .cc delete OUR_LEFT_FREE_KICK_SPOT
  .cc delete OUR_RIGHT_FREE_KICK_SPOT
  .cc delete THEIR_LEFT_FREE_KICK_SPOT
  .cc delete THEIR_RIGHT_FREE_KICK_SPOT
  .cc delete OUR_PENALTY_KICK_SPOT
  .cc delete THEIR_PENALTY_KICK_SPOT
}

####################################
# debug output window
####################################
text .debugOutput -relief sunken -yscrollcommand ".debugOutput.scroll set"
scrollbar .debugOutput.scroll -command ".debugOutput yview"
pack .debugOutput.scroll -side right -fill y
####################################


####ADD DEBUG MESSAGE################
proc addDebugMessage { message } \
{
    .debugOutput insert end $message
    .debugOutput insert end \n
    .debugOutput yview end
}

########################################

# This procedure draws the field
proc drawField {} \
{
  set GOAL_DEPTH                     [expr 100 * [GOAL_DEPTH_cmd]]
  set LEFT_SIDE_LINE                 [expr 100 * [LEFT_SIDE_LINE_cmd]]
  set RIGHT_SIDE_LINE                [expr 100 * [RIGHT_SIDE_LINE_cmd]]
  set OUR_GOAL_LINE                  [expr 100 * [OUR_GOAL_LINE_cmd]]
  set OUR_BACK_GOAL_LINE             [expr $OUR_GOAL_LINE - $GOAL_DEPTH]
  set THEIR_GOAL_LINE                [expr 100 * [THEIR_GOAL_LINE_cmd]]
  set THEIR_BACK_GOAL_LINE           [expr $THEIR_GOAL_LINE + $GOAL_DEPTH]
  set LEFT_GOAL_POST                 [expr 100 * [LEFT_GOAL_POST_cmd]]
  set RIGHT_GOAL_POST                [expr 100 * [RIGHT_GOAL_POST_cmd]]
  set HALF_LINE                      [expr [expr $OUR_GOAL_LINE + $THEIR_GOAL_LINE] / 2]
  set SPLIT_LINE                     [expr [expr $LEFT_SIDE_LINE + $RIGHT_SIDE_LINE] / 2]

  set LEFT_DEFENCE_ZONE              [expr 100 * [LEFT_GOALIE_BOX_cmd]]
  set RIGHT_DEFENCE_ZONE             [expr 100 * [RIGHT_GOALIE_BOX_cmd]]
  set DEFENCE_ZONE_DEPTH             [expr 100 * [GOALIE_BOX_DEPTH_cmd]]

  set OUR_DEFENCE_LINE               [expr $OUR_GOAL_LINE + $DEFENCE_ZONE_DEPTH]
  set THEIR_DEFENCE_LINE             [expr $THEIR_GOAL_LINE - $DEFENCE_ZONE_DEPTH]

  set CENTER_CIRCLE_RADIUS           [expr 100 * [CENTER_CIRCLE_RADIUS_cmd]]
  set FREE_KICK_X_OFFSET             [expr 100 * [FREE_KICK_X_OFFSET_cmd]]
  set FREE_KICK_Y_OFFSET             [expr 100 * [FREE_KICK_Y_OFFSET_cmd]]
  set PENALTY_KICK_X_OFFSET          [expr 100 * [PENALTY_KICK_X_OFFSET_cmd]]
 
  #############################
  #draw field, rectangle puts in the green
  dodraw "rect" "FIELD_BOUNDARY"    0    $OUR_GOAL_LINE    $RIGHT_SIDE_LINE    $THEIR_GOAL_LINE    $LEFT_SIDE_LINE green
  #######
  #these lines put in the perimeter, excluding the goals
  #######
  dodraw "line" "LEFT_SIDE_LINE"    2    $OUR_GOAL_LINE    $LEFT_SIDE_LINE    $THEIR_GOAL_LINE    $LEFT_SIDE_LINE black
  dodraw "line" "RIGHT_SIDE_LINE"   2    $OUR_GOAL_LINE    $RIGHT_SIDE_LINE    $THEIR_GOAL_LINE    $RIGHT_SIDE_LINE black
  dodraw "line" "OUR_SIDE_LEFT"     2    $OUR_GOAL_LINE    $LEFT_SIDE_LINE    $OUR_GOAL_LINE    $LEFT_GOAL_POST black
  dodraw "line" "OUR_SIDE_RIGHT"    2    $OUR_GOAL_LINE    $RIGHT_SIDE_LINE    $OUR_GOAL_LINE    $RIGHT_GOAL_POST black
  dodraw "line" "THEIR_SIDE_LEFT"     2    $THEIR_GOAL_LINE    $LEFT_SIDE_LINE    $THEIR_GOAL_LINE    $LEFT_GOAL_POST black
  dodraw "line" "THEIR_SIDE_RIGHT"    2    $THEIR_GOAL_LINE    $RIGHT_SIDE_LINE    $THEIR_GOAL_LINE    $RIGHT_GOAL_POST black
  #############################

  #############################
  #draw our goalbox, rectangle puts in the green

  dodraw "rect" "OUR_GOAL_POST"     0    $OUR_BACK_GOAL_LINE    $RIGHT_GOAL_POST      $OUR_GOAL_LINE    $LEFT_GOAL_POST green
  #######
  #these lines show the boundries. make the front line very thing (1) so we
  #can easily when the ball (or robot) crosses over
  #######
  dodraw "line" "OUR_GOAL_BOX_LEFT" 2    $OUR_BACK_GOAL_LINE    $RIGHT_GOAL_POST      $OUR_GOAL_LINE         $RIGHT_GOAL_POST black
  dodraw "line" "OUR_GOAL_BOX_RIGHT" 2   $OUR_BACK_GOAL_LINE    $LEFT_GOAL_POST       $OUR_GOAL_LINE         $LEFT_GOAL_POST black
  dodraw "line" "OUR_GOAL_BOX_BACK" 2    $OUR_BACK_GOAL_LINE    $LEFT_GOAL_POST       $OUR_BACK_GOAL_LINE    $RIGHT_GOAL_POST black
  #############################

  #############################
  #draw their goalbox, rectangle puts in the green
  dodraw "rect" "THEIR_GOAL_BOX"     0    $THEIR_BACK_GOAL_LINE    $RIGHT_GOAL_POST      $THEIR_GOAL_LINE    $LEFT_GOAL_POST green
  #######
  #these lines show the boundries. make the front line very thing (1) so we
  #can easily when the ball (or robot) crosses over
  #######
  dodraw "line" "THEIR_GOAL_BOX_LEFT" 2    $THEIR_BACK_GOAL_LINE    $RIGHT_GOAL_POST      $THEIR_GOAL_LINE         $RIGHT_GOAL_POST black
  dodraw "line" "THEIR_GOAL_BOX_RIGHT" 2   $THEIR_BACK_GOAL_LINE    $LEFT_GOAL_POST      $THEIR_GOAL_LINE         $LEFT_GOAL_POST black
  dodraw "line" "THEIR_GOAL_BOX_BACK" 2    $THEIR_BACK_GOAL_LINE    $LEFT_GOAL_POST      $THEIR_BACK_GOAL_LINE    $RIGHT_GOAL_POST black
  #############################

  global displayFieldLines
  if { $displayFieldLines == 1 } \
  {
  #############################
  #draw center line and half circle in middle
  #######
  dodraw "line"           \
         "HALF_LINE"      \
         1                \
		 $HALF_LINE       \
		 $LEFT_SIDE_LINE  \
		 $HALF_LINE       \
		 $RIGHT_SIDE_LINE \
		 black

  dodraw "oval"                     \
         "CENTER_CIRCLE"            \
		 1                          \
		 [expr $HALF_LINE - $CENTER_CIRCLE_RADIUS]  \
		 [expr $SPLIT_LINE - $CENTER_CIRCLE_RADIUS] \
		 [expr $HALF_LINE + $CENTER_CIRCLE_RADIUS]   \
		 [expr $SPLIT_LINE + $CENTER_CIRCLE_RADIUS]  \
		 black
  #############################

  #############################
  #draw goal lines
  #######
  dodraw "line" "OUR_GOAL_BOX_FRONT" 1   $OUR_GOAL_LINE         $LEFT_GOAL_POST       $OUR_GOAL_LINE         $RIGHT_GOAL_POST black
  dodraw "line" "THEIR_GOAL_BOX_FRONT" 1   $THEIR_GOAL_LINE         $LEFT_GOAL_POST      $THEIR_GOAL_LINE         $RIGHT_GOAL_POST black
  #############################

  #############################
  #draw our defence zone
  #######
  dodraw "line"                  \
         "OUR_DEFENCE_ZONE_RIGHT" \
		 1                       \
         $OUR_GOAL_LINE          \
		 $RIGHT_DEFENCE_ZONE     \
		 $OUR_DEFENCE_LINE       \
		 $RIGHT_DEFENCE_ZONE     \
		 black

  dodraw "line" "OUR_DEFENCE_ZONE_LEFT" 1    $OUR_GOAL_LINE    $LEFT_DEFENCE_ZONE     $OUR_DEFENCE_LINE    $LEFT_DEFENCE_ZONE black
  dodraw "line" "OUR_DEFENCE_ZONE_FRONT" 1    $OUR_DEFENCE_LINE $LEFT_DEFENCE_ZONE     $OUR_DEFENCE_LINE    $RIGHT_DEFENCE_ZONE black
  #############################

  #############################
  #draw their defence zone
  #######
  dodraw "line" "OUR_DEFENCE_ZONE_LEFT"  1    $THEIR_GOAL_LINE     $RIGHT_DEFENCE_ZONE    $THEIR_DEFENCE_LINE     $RIGHT_DEFENCE_ZONE black
  dodraw "line" "OUR_DEFENCE_ZONE_RIGHT" 1    $THEIR_GOAL_LINE     $LEFT_DEFENCE_ZONE     $THEIR_DEFENCE_LINE     $LEFT_DEFENCE_ZONE black
  dodraw "line" "OUR_DEFENCE_ZONE_FRONT" 1    $THEIR_DEFENCE_LINE  $RIGHT_DEFENCE_ZONE    $THEIR_DEFENCE_LINE     $LEFT_DEFENCE_ZONE black
  #############################

  #############################
  #draw free kick spots
  #######
  dodraw "oval" \
         "OUR_LEFT_FREE_KICK_SPOT" \
		 2   \
		 [expr $OUR_GOAL_LINE + $FREE_KICK_X_OFFSET] \
		 [expr $LEFT_SIDE_LINE - $FREE_KICK_Y_OFFSET] \
		 [expr $OUR_GOAL_LINE + $FREE_KICK_X_OFFSET] \
		 [expr $LEFT_SIDE_LINE - $FREE_KICK_Y_OFFSET] \
		 black

  dodraw "oval" \
         "OUR_RIGHT_FREE_KICK_SPOT" \
		 2   \
		 [expr $OUR_GOAL_LINE + $FREE_KICK_X_OFFSET] \
		 [expr $RIGHT_SIDE_LINE + $FREE_KICK_Y_OFFSET] \
		 [expr $OUR_GOAL_LINE + $FREE_KICK_X_OFFSET] \
		 [expr $RIGHT_SIDE_LINE + $FREE_KICK_Y_OFFSET] \
		 black

  dodraw "oval" \
         "THEIR_LEFT_FREE_KICK_SPOT" \
		 2   \
		 [expr $THEIR_GOAL_LINE - $FREE_KICK_X_OFFSET] \
		 [expr $LEFT_SIDE_LINE - $FREE_KICK_Y_OFFSET] \
		 [expr $THEIR_GOAL_LINE - $FREE_KICK_X_OFFSET] \
		 [expr $LEFT_SIDE_LINE - $FREE_KICK_Y_OFFSET] \
		 black


  dodraw "oval" \
         "THEIR_RIGHT_FREE_KICK_SPOT" \
		 2   \
		 [expr $THEIR_GOAL_LINE - $FREE_KICK_X_OFFSET] \
		 [expr $RIGHT_SIDE_LINE + $FREE_KICK_Y_OFFSET] \
		 [expr $THEIR_GOAL_LINE - $FREE_KICK_X_OFFSET] \
		 [expr $RIGHT_SIDE_LINE + $FREE_KICK_Y_OFFSET] \
		 black
  #############################

  #############################
  #draw penalty kick spots
  #######
  dodraw "oval"                                     \
         "OUR_PENALTY_KICK_SPOT"                    \
		 2                                          \
         [expr $OUR_GOAL_LINE + $PENALTY_KICK_X_OFFSET] \
		 $SPLIT_LINE                                \
         [expr $OUR_GOAL_LINE + $PENALTY_KICK_X_OFFSET] \
		 $SPLIT_LINE                                \
		 black

  dodraw "oval"                    \
         "THEIR_PENALTY_KICK_SPOT" \
		 2                                         \
         [expr $THEIR_GOAL_LINE - $PENALTY_KICK_X_OFFSET] \
		 $SPLIT_LINE                                \
         [expr $THEIR_GOAL_LINE - $PENALTY_KICK_X_OFFSET] \
		 $SPLIT_LINE                                \
		 black
  #############################
  }

  global displayZoneSectorLines
  if { $displayZoneSectorLines == 1 } \
  {

  #############################
  #draw zone lines
  #######
  set OFFENSE_ZONE_LINE         [expr 100 * [OFFENSE_ZONE_LINE_cmd]]
  set KILL_ZONE_LINE            [expr 100 * [KILL_ZONE_LINE_cmd]]
  set DEFENSE_ZONE_LINE         [expr 100 * [DEFENSE_ZONE_LINE_cmd]]
  set DEATH_ZONE_LINE           [expr 100 * [DEATH_ZONE_LINE_cmd]]
  #######
  dodraw "line" "OFFENSE_ZONE_LINE"  1    $OFFENSE_ZONE_LINE     $LEFT_SIDE_LINE    $OFFENSE_ZONE_LINE     $RIGHT_SIDE_LINE blue
  dodraw "line" "KILL_ZONE_LINE"  1    $KILL_ZONE_LINE     $LEFT_SIDE_LINE    $KILL_ZONE_LINE     $RIGHT_SIDE_LINE blue
  dodraw "line" "DEFENSE_ZONE_LINE"  1    $DEFENSE_ZONE_LINE     $LEFT_SIDE_LINE    $DEFENSE_ZONE_LINE     $RIGHT_SIDE_LINE blue
  dodraw "line" "DEATH_ZONE_LINE"  1    $DEATH_ZONE_LINE     $LEFT_SIDE_LINE    $DEATH_ZONE_LINE     $RIGHT_SIDE_LINE blue
  #############################

  #############################
  #draw sector lines
  #######
  set LEFT_SWEET_SECTOR         [expr 100 * [LEFT_SWEET_SECTOR_cmd]]
  set RIGHT_SWEET_SECTOR        [expr 100 * [RIGHT_SWEET_SECTOR_cmd]]
  set LEFT_MIDDLE_SECTOR        [expr 100 * [LEFT_MIDDLE_SECTOR_cmd]]
  set RIGHT_MIDDLE_SECTOR       [expr 100 * [RIGHT_MIDDLE_SECTOR_cmd]]
  #######
  dodraw "line" "LEFT_SWEET_SECTOR"  1    $OUR_GOAL_LINE     $LEFT_SWEET_SECTOR    $THEIR_GOAL_LINE  $LEFT_SWEET_SECTOR blue
  dodraw "line" "RIGHT_SWEET_SECTOR"  1    $OUR_GOAL_LINE     $RIGHT_SWEET_SECTOR    $THEIR_GOAL_LINE  $RIGHT_SWEET_SECTOR blue
  dodraw "line" "LEFT_MIDDLE_SECTOR"  1    $OUR_GOAL_LINE     $LEFT_MIDDLE_SECTOR    $THEIR_GOAL_LINE  $LEFT_MIDDLE_SECTOR blue
  dodraw "line" "RIGHT_MIDDLE_SECTOR"  1    $OUR_GOAL_LINE     $RIGHT_MIDDLE_SECTOR    $THEIR_GOAL_LINE  $RIGHT_MIDDLE_SECTOR blue
  #############################
  }
}
######
# This procedure draws a robot at the specified location
# with the specified orientation
# "orientation" is the counter-clockwise angle between the positive x axis
proc drawOurRobot { \
                    x_coord \
                    y_coord \
                    orientation \
                    destX \
                    destY \
                    destRot \
                    passdestX \
                    passdestY \
                    tagName \
                    robotId \
                    displayDest \
                    displayPassDest \
                    robotsPassValue \
                  } \
{
  for {set i 0} { $i < [GetNumRobots_cmd]} {incr i} \
  {
    global ourRobotPassValue$i
  }
    
    global automaticDisplayPassDest
    global FIELD_X
    global FIELD_Y

  global aiStatus

    set x_coord    [expr -1 * $x_coord]
    set y_coord    [expr -1 * $y_coord]
    set destX      [expr -1 * $destX]
    set destY      [expr -1 * $destY]
    set passdestX  [expr -1 * $passdestX]
    set passdestY  [expr -1 * $passdestY]

    global FIELD_FACTOR
    set OFFSET [expr 4* $FIELD_FACTOR]
    set x_max [expr $x_coord + $OFFSET]
    set y_max [expr $y_coord + $OFFSET]
    set x_min [expr $x_coord - $OFFSET]
    set y_min [expr $y_coord - $OFFSET]
    
  global objectConverted

  #if robot lost, set x-coord and y-coord to -32000.0f
  if { [GetOurRobotFound_cmd $robotId] != 1 } \
  {
    set x_coord -32000
    set y_coord -32000
  }



  #display the robotId
  .cc create text [expr $FIELD_FACTOR * $y_coord + $FIELD_X] \
                  [expr $FIELD_FACTOR * $x_coord + $FIELD_Y] \
                  -text $robotId \
                  -tags $tagName \
                  -font {Courier 20 bold} \
                  -fill red

  global objectPositionSpecified
  .cc bind $tagName <1> "set objectPositionSpecified $robotId"
  .cc bind $tagName <3> "set objectPositionSpecified [expr $robotId + 5]"
  .cc bind $tagName <Control-1> "removeObject $robotId"
 
  # display the robot's destination
  if { $displayDest == 1 && \
       $aiStatus == "On"  && \
       [GetOurRobotFound_cmd $robotId] == 1 } \
  {
    .cc create text [expr $FIELD_FACTOR * $destY + $FIELD_X] \
                    [expr $FIELD_FACTOR * $destX + $FIELD_Y] \
                    -text $robotId \
                    -tags $tagName \
                    -font {Courier 20 bold} \
                    -fill yellow

    #destination rotation arrow
    set color yellow
    dodraw "line" \
         $tagName \
         3 \
         [rotate_x [expr $x_coord + $OFFSET] $y_coord $destRot $x_coord $y_coord] \
         [rotate_y [expr $x_coord + $OFFSET] $y_coord $destRot $x_coord $y_coord] \
         [rotate_x [expr $x_coord + $OFFSET + 16] $y_coord $destRot $x_coord $y_coord] \
         [rotate_y [expr $x_coord + $OFFSET + 16] $y_coord $destRot $x_coord $y_coord] \
         $color

    dodraw "line" \
         $tagName \
         3 \
         [rotate_x [expr $x_coord + $OFFSET + 7] [expr $y_coord - 7] $destRot $x_coord $y_coord] \
         [rotate_y [expr $x_coord + $OFFSET + 7] [expr $y_coord - 7] $destRot $x_coord $y_coord] \
         [rotate_x [expr $x_coord + $OFFSET + 16] $y_coord $destRot $x_coord $y_coord] \
         [rotate_y [expr $x_coord + $OFFSET + 16] $y_coord $destRot $x_coord $y_coord] \
         $color

    dodraw "line" \
         $tagName \
         3 \
         [rotate_x [expr $x_coord + $OFFSET + 7] [expr $y_coord + 7] $destRot $x_coord $y_coord] \
         [rotate_y [expr $x_coord + $OFFSET + 7] [expr $y_coord + 7] $destRot $x_coord $y_coord] \
         [rotate_x [expr $x_coord + $OFFSET + 16] $y_coord $destRot $x_coord $y_coord] \
         [rotate_y [expr $x_coord + $OFFSET + 16] $y_coord $destRot $x_coord $y_coord] \
         $color


  }

  # display the robot's pass destination IFF
    # we set the robot to display it OR we have set
    # automatic displaying on and this robot has set
    # his pass value to a non-zero value (as in he's saying pass to me!)

  set doWePass 0

  if { $displayPassDest == 1 && \
       $aiStatus == "On" && \
       [GetOurRobotFound_cmd $robotId] == 1 } \
  {
       set doWePass 1
    } 
    
    if { $robotsPassValue == 1 && $automaticDisplayPassDest == 1} \
  {
       set doWePass 1
    }

    if { $doWePass == 1  } \
  {
    .cc create text [expr $FIELD_FACTOR * $passdestY + $FIELD_X] \
                    [expr $FIELD_FACTOR * $passdestX + $FIELD_Y] \
                    -text $robotId \
                    -tags $tagName \
                    -font {Courier 20 bold} \
                    -fill pink
  }
    

    # draw five lines to represent one robot
  set color black
  if { [GetOurRobotHavePossession_cmd $robotId] == 1 } \
  {
    set color yellow
  }

    dodraw "line" \
         $tagName \
         2 \
         [rotate_x $x_max $y_max $orientation $x_coord $y_coord] \
         [rotate_y $x_max $y_max $orientation $x_coord $y_coord] \
         [rotate_x $x_max $y_min $orientation $x_coord $y_coord] \
         [rotate_y $x_max $y_min $orientation $x_coord $y_coord] \
         $color
     
    dodraw "line" \
         $tagName \
         2 \
         [rotate_x $x_max $y_min $orientation $x_coord $y_coord] \
         [rotate_y $x_max $y_min $orientation $x_coord $y_coord] \
         [rotate_x $x_min $y_min $orientation $x_coord $y_coord] \
         [rotate_y $x_min $y_min $orientation $x_coord $y_coord] \
         $color

    dodraw "line" \
         $tagName \
         2 \
         [rotate_x $x_min $y_min $orientation $x_coord $y_coord] \
         [rotate_y $x_min $y_min $orientation $x_coord $y_coord] \
         [rotate_x $x_min $y_max $orientation $x_coord $y_coord] \
         [rotate_y $x_min $y_max $orientation $x_coord $y_coord] \
         $color

    dodraw "line" \
         $tagName \
         2 \
         [rotate_x $x_min $y_max $orientation $x_coord $y_coord] \
         [rotate_y $x_min $y_max $orientation $x_coord $y_coord] \
         [rotate_x $x_max $y_max $orientation $x_coord $y_coord] \
         [rotate_y $x_max $y_max $orientation $x_coord $y_coord] \
         $color

    dodraw "dribbler" \
         $tagName \
         2 \
         [rotate_x [expr $x_max+1] $y_max $orientation $x_coord $y_coord] \
         [rotate_y [expr $x_max+1] $y_max $orientation $x_coord $y_coord] \
         [rotate_x [expr $x_max+1] $y_min $orientation $x_coord $y_coord] \
         [rotate_y [expr $x_max+1] $y_min $orientation $x_coord $y_coord] \
         red
}
####################################
### END proc drawOurRobot ##########
####################################

############
# This procedure draws opponent robots
############
proc drawTheirRobot {x_coord y_coord tagName robotId} \
{
  global FIELD_FACTOR
  global FIELD_X
  global FIELD_Y
  global THEIR_ROBOT_RADIUS
  
  #if robot lost, set x-coord and y-coord to -32000.0f
  if { [GetTheirRobotFound_cmd [expr $robotId-5]] == 0 } \
  {
    set x_coord -32000
    set y_coord -32000
  }

  
  # display the robotId
  .cc create text [expr $FIELD_FACTOR * -1 *$y_coord + $FIELD_X] \
                  [expr $FIELD_FACTOR * -1 * $x_coord + $FIELD_Y] \
                  -text [expr $robotId-5] \
                  -tags $tagName \
                  -font {Courier 20 bold} \
                  -fill blue

  global objectPositionSpecified
  .cc bind $tagName <1> "set objectPositionSpecified [expr $robotId + 6]"
  .cc bind $tagName <Control-1> "removeObject [expr $robotId + 6]"

  set color black
  if { [GetTheirRobotHavePossession_cmd [expr $robotId - 5]] == 1 } \
  {
    set color yellow
  }

    dodraw "oval" \
         $tagName \
         2 \
         [expr $x_coord+$THEIR_ROBOT_RADIUS] \
         [expr $y_coord+$THEIR_ROBOT_RADIUS] \
                 [expr $x_coord-$THEIR_ROBOT_RADIUS] \
         [expr $y_coord-$THEIR_ROBOT_RADIUS] \
         $color
}
############
# This procedures draws a ball given the location
proc drawOneBall {x_coord y_coord} \
{
  #if robot lost, set x-coord and y-coord to -32000.0f
  if { [GetRoboCupBallFound_cmd] != 1 } \
  {
    set x_coord -32000
    set y_coord -32000
  }

    global BALL_RADIUS
    dodraw "ball" \
         BALL \
         3 \
         [expr $x_coord+$BALL_RADIUS] \
         [expr $y_coord+$BALL_RADIUS] \
         [expr $x_coord-$BALL_RADIUS] \
         [expr $y_coord-$BALL_RADIUS] \
         red
    global objectPositionSpecified
   .cc bind BALL <1> "set objectPositionSpecified 10"
   .cc bind BALL <Control-1> "removeObject 10"
}
############
# This procedures draws a debug point given the location
proc drawDebugPoint {x_coord y_coord robotID debugPointID} \
{
    global FIELD_FACTOR
    global FIELD_X
    global FIELD_Y

    dodraw "debugPoint" \
         DEBUG_POINT \
         1 \
         [expr $x_coord+3] \
         [expr $y_coord+3] \
         [expr $x_coord-3] \
         [expr $y_coord-3] \
         blue

    .cc create text [expr $FIELD_FACTOR * -1 * $y_coord + $FIELD_X + 1] \
                    [expr $FIELD_FACTOR * -1 * $x_coord + $FIELD_Y + 1] \
                    -text [expr $robotID]:[expr $debugPointID] \
                    -tags DEBUG_POINT \
                    -font {Courier 11 bold} \
                    -fill white
}

########################################
###### WIDGET DECLARATION BEGINS #######
########################################

##### TOP LEVEL WINDOW #################
# toplevel window declaration
wm title . "Big Red RoboCup 2002 - oh yeah..." 
# specify window size
wm geometry . 1020x760
wm maxsize  . 1020 760
wm minsize  . 1020 760
focus .
# toplevel window declaration ends
#############################


proc drawMenus {} \
{
  global complexMode
  global toggleComplexity
  global HELPLIST1
  global HELPLIST2
  global TOOLSLIST1
  global TOOLSLIST2
  global MainSystemCommandsText
  global MainSystemCommands 
  global MinorSystemCommandsText 
  global MinorSystemCommands 
  global NO_POSITION              
  global NO_PLAY                  
  global START_2002_PLAYS        
  global END_2002_PLAYS        
  global START_2002_PENALTY_PLAYS        
  global END_2002_PENALTY_PLAYS        
  global START_FORMATION_PLAYS     
  global END_FORMATION_PLAYS   
  global START_EVAN_KUHN_SKILL_TEST_PLAYS  
  global END_EVAN_KUHN_SKILL_TEST_PLAYS    
  global START_EVAN_MALONE_SKILL_TEST_PLAYS
  global END_EVAN_MALONE_SKILL_TEST_PLAYS
  global START_JOEL_SKILL_TEST_PLAYS  
  global END_JOEL_SKILL_TEST_PLAYS    
  global START_NIRAV_SKILL_TEST_PLAYS  
  global END_NIRAV_SKILL_TEST_PLAYS    
  global START_WILL_SKILL_TEST_PLAYS  
  global END_WILL_SKILL_TEST_PLAYS    
  global START_INDIVIDUAL_PLAYS  
  global END_INDIVIDUAL_PLAYS    
  global START_INTIMIDATION_PLAYS 
  global END_INTIMIDATION_PLAYS  
  global START_OTHER_PLAYS       
  global END_OTHER_PLAYS         
  global CANVAS_WIDTH            
  global CANVAS_HEIGHT           
  global FIELD_X                 
  global FIELD_Y                 
  global FIELD_FACTOR            
  global METER_CONVERSION        
  global REFRESH_DELAY           
  global THEIR_ROBOT_RADIUS      
###### MENU ############
# menu button begins
# create menu with file option
frame .mbar ;# frame for menubar

####################################################################
menubutton .mbar.systemControl \
            -text "Main Control" \
            -menu .mbar.systemControl.menu

menu .mbar.systemControl.menu

foreach {x} $MainSystemCommandsText {y} $MainSystemCommands \
{
  .mbar.systemControl.menu add command -label $x -command $y
}

.mbar.systemControl.menu add separator

foreach {x} $MinorSystemCommandsText {y} $MinorSystemCommands \
{
  .mbar.systemControl.menu add command -label $x -command $y
}

pack .mbar.systemControl -side left;
####################################################################
menubutton .mbar.robotPosition \
            -text "Robot Position" \
            -menu .mbar.robotPosition.menu
menu .mbar.robotPosition.menu -tearoff 0
.mbar.robotPosition.menu add command -label "Initialize 2002" -command Initialize2002Positions_cmd
.mbar.robotPosition.menu add command -label "Reset Positions (F8)" -command ResetPositions_cmd
.mbar.robotPosition.menu add separator

for {set robot 0} { $robot < [GetNumRobots_cmd]} {incr robot} \
{
  menu .mbar.robotPosition.menu.positionMenu$robot -tearoff 0
 .mbar.robotPosition.menu add cascade -label "Robot $robot" -menu .mbar.robotPosition.menu.positionMenu$robot
  for {set position 0} { $position < [GetNumPositions_cmd]} {incr position} \
  {
    .mbar.robotPosition.menu.positionMenu$robot add command -label [GetPositionName_cmd $position] -command [GetSetPosition_cmd $robot $position]
  }
 .mbar.robotPosition.menu.positionMenu$robot add command -label [GetPositionName_cmd $NO_POSITION] -command [GetSetPosition_cmd $robot $NO_POSITION]
}
pack .mbar.robotPosition -side left;
####################################################################
menubutton .mbar.robotType \
            -text "Robot Type" \
            -menu .mbar.robotType.menu
menu .mbar.robotType.menu -tearoff 0
.mbar.robotType.menu add command -label "Set All 2000" -command setAllType2000_cmd
.mbar.robotType.menu add command -label "Set All 2001" -command setAllType2001_cmd
.mbar.robotType.menu add command -label "Set All 2002" -command setAllType2002_cmd
.mbar.robotType.menu add separator

for {set robot 0} { $robot < [GetNumRobots_cmd]} {incr robot} \
{
  menu .mbar.robotType.menu.typeMenu$robot -tearoff 0
 .mbar.robotType.menu add cascade -label "Robot $robot" -menu .mbar.robotType.menu.typeMenu$robot
 .mbar.robotType.menu.typeMenu$robot add command -label "2000 Omni" -command "SetRobotType_cmd $robot 2000"
 .mbar.robotType.menu.typeMenu$robot add command -label "2001 Omni" -command "SetRobotType_cmd $robot 2001"
 .mbar.robotType.menu.typeMenu$robot add command -label "2002 Omni" -command "SetRobotType_cmd $robot 2002"
}
pack .mbar.robotType -side left;
####################################################################
####################################################################
menubutton .mbar.setPlay \
            -text "Set Play" \
            -menu .mbar.setPlay.menu
menu .mbar.setPlay.menu -tearoff 0
####################################################################
menu .mbar.setPlay.menu.formation -tearoff 0
.mbar.setPlay.menu add cascade -label "Formations" -menu .mbar.setPlay.menu.formation
for {set iii [expr $START_FORMATION_PLAYS + 1]} { $iii < $END_FORMATION_PLAYS} {incr iii} \
{
  .mbar.setPlay.menu.formation add command -label [GetPlayName_cmd $iii] -command [GetSetCmd_cmd $iii] 
}
####################################################################
menu .mbar.setPlay.menu.intimidation -tearoff 0
.mbar.setPlay.menu add cascade -label "Intimidation" -menu .mbar.setPlay.menu.intimidation
for {set iii [expr $START_INTIMIDATION_PLAYS + 1]} { $iii < $END_INTIMIDATION_PLAYS} {incr iii} \
{
  .mbar.setPlay.menu.intimidation add command -label [GetPlayName_cmd $iii] -command [GetSetCmd_cmd $iii] 
}
####################################################################
.mbar.setPlay.menu add separator
####################################################################
menu .mbar.setPlay.menu.2002play -tearoff 0
.mbar.setPlay.menu add cascade -label "2002 Play" -menu .mbar.setPlay.menu.2002play  
for {set iii [expr $START_2002_PLAYS + 1]} { $iii < $END_2002_PLAYS} {incr iii} \
{
  .mbar.setPlay.menu.2002play add command -label [GetPlayName_cmd $iii] -command [GetSetCmd_cmd $iii] 
}
####################################################################
menu .mbar.setPlay.menu.2002penaltyPlay -tearoff 0
.mbar.setPlay.menu add cascade -label "2002 Penalty Play" -menu .mbar.setPlay.menu.2002penaltyPlay
for {set iii [expr $START_2002_PENALTY_PLAYS + 1]} { $iii < $END_2002_PENALTY_PLAYS} {incr iii} \
{
  .mbar.setPlay.menu.2002penaltyPlay add command -label [GetPlayName_cmd $iii] -command [GetSetCmd_cmd $iii] 
}
####################################################################
if { $complexMode == 1 } \
{
  .mbar.setPlay.menu add separator
}
####################################################################
if { $complexMode == 1 } \
{
  ###############
  menu .mbar.setPlay.menu.evanKuhnSkillz -tearoff 0
  .mbar.setPlay.menu add cascade \
                                  -label "Evan Kuhn's Skillz" \
                                  -menu .mbar.setPlay.menu.evanKuhnSkillz
  for {set iii [expr $START_EVAN_KUHN_SKILL_TEST_PLAYS + 1]} { $iii < $END_EVAN_KUHN_SKILL_TEST_PLAYS} {incr iii} \
  {
    .mbar.setPlay.menu.evanKuhnSkillz add command -label [GetPlayName_cmd $iii] -command [GetSetCmd_cmd $iii] 
  }
  ###############
  menu .mbar.setPlay.menu.evanMaloneSkillz -tearoff 0
 .mbar.setPlay.menu add cascade \
                                  -label "Evan Malone's Skillz" \
                                  -menu .mbar.setPlay.menu.evanMaloneSkillz
  for {set iii [expr $START_EVAN_MALONE_SKILL_TEST_PLAYS + 1]} { $iii < $END_EVAN_MALONE_SKILL_TEST_PLAYS} {incr iii} \
  {
    .mbar.setPlay.menu.evanMaloneSkillz add command -label [GetPlayName_cmd $iii] -command [GetSetCmd_cmd $iii] 
  }
  ###############
  menu .mbar.setPlay.menu.joelSkillz -tearoff 0
 .mbar.setPlay.menu add cascade \
                                  -label "Joel's Skillz" \
                                  -menu .mbar.setPlay.menu.joelSkillz
  for {set iii [expr $START_JOEL_SKILL_TEST_PLAYS + 1]} { $iii < $END_JOEL_SKILL_TEST_PLAYS} {incr iii} \
  {
    .mbar.setPlay.menu.joelSkillz add command -label [GetPlayName_cmd $iii] -command [GetSetCmd_cmd $iii] 
  }
  ###############
  menu .mbar.setPlay.menu.niravSkillz -tearoff 0
 .mbar.setPlay.menu add cascade \
                                  -label "Nirav's Skillz" \
                                  -menu .mbar.setPlay.menu.niravSkillz
  for {set iii [expr $START_NIRAV_SKILL_TEST_PLAYS + 1]} { $iii < $END_NIRAV_SKILL_TEST_PLAYS} {incr iii} \
  {
    .mbar.setPlay.menu.niravSkillz add command -label [GetPlayName_cmd $iii] -command [GetSetCmd_cmd $iii] 
  }
  ###############
  menu .mbar.setPlay.menu.willSkillz -tearoff 0
 .mbar.setPlay.menu add cascade \
                                  -label "Will's Skillz" \
                                  -menu .mbar.setPlay.menu.willSkillz
  for {set iii [expr $START_WILL_SKILL_TEST_PLAYS + 1]} { $iii < $END_WILL_SKILL_TEST_PLAYS} {incr iii} \
  {
    .mbar.setPlay.menu.willSkillz add command -label [GetPlayName_cmd $iii] -command [GetSetCmd_cmd $iii] 
  }
  ###############
}
####################################################################
if { $complexMode == 1 } \
{
  .mbar.setPlay.menu add separator
}
####################################################################
if { $complexMode == 1 } \
{
  menu .mbar.setPlay.menu.individual -tearoff 0
  .mbar.setPlay.menu add cascade -label "Individual Play" -menu .mbar.setPlay.menu.individual
  for {set iii [expr $START_INDIVIDUAL_PLAYS + 1]} { $iii < $END_INDIVIDUAL_PLAYS} {incr iii} \
  {
    .mbar.setPlay.menu.individual add command -label [GetPlayName_cmd $iii] -command [GetSetCmd_cmd $iii] 
  }
}
####################################################################
if { $complexMode == 1 } \
{
  menu .mbar.setPlay.menu.other -tearoff 0
  .mbar.setPlay.menu add cascade -label "Other Play" -menu .mbar.setPlay.menu.other
  for {set iii [expr $START_OTHER_PLAYS + 1]} { $iii < $END_OTHER_PLAYS} {incr iii} \
  {
    .mbar.setPlay.menu.other add command -label [GetPlayName_cmd $iii] -command [GetSetCmd_cmd $iii] 
  }
}
####################################################################
.mbar.setPlay.menu add separator
####################################################################
.mbar.setPlay.menu add command -label [GetPlayName_cmd $NO_PLAY] -command [GetSetCmd_cmd $NO_PLAY] 
pack .mbar.setPlay -side left;
####################################################################
if { $complexMode == 1 } \
{

menubutton .mbar.setObjectPositions -text "Set Object Positions" -menu .mbar.setObjectPositions.menu

menu .mbar.setObjectPositions.menu

set fl fletter
set ol oletter

set fletter0 q
set fletter1 w
set fletter2 e
set fletter3 r
set fletter4 t

set fletter5 a
set fletter6 s
set fletter7 d
set fletter8 f
set fletter9 g

set oletter0 z
set oletter1 x
set oletter2 c
set oletter3 v
set oletter4 b


for {set ID 0} { $ID < [GetNumRobots_cmd]} {incr ID} \
{
  .mbar.setObjectPositions.menu add radio -label "Set Friendly Robot $ID Location ([expr $$fl$ID])" -variable objectPositionSpecified  
  .mbar.setObjectPositions.menu add radio -label "Set Friendly Robot $ID Rotation ([expr $$fl[expr $ID + 5]])" -variable objectPositionSpecified  
  .mbar.setObjectPositions.menu add command -label "Remove Friendly Robot $ID" -command "removeObject $ID"
  .mbar.setObjectPositions.menu add separator
}

.mbar.setObjectPositions.menu add radio -label "Set Ball Location (`)" -variable objectPositionSpecified
.mbar.setObjectPositions.menu add command -label "Remove Ball" -command "removeObject 10"

.mbar.setObjectPositions.menu add separator

for {set ID 0} { $ID < [GetNumRobots_cmd]} {incr ID} \
{
  .mbar.setObjectPositions.menu add radio -label "Set Opponent Robot $ID Location ([expr $$ol$ID])" -variable objectPositionSpecified
  .mbar.setObjectPositions.menu add command -label "Remove Opponent Robot $ID" -command "removeObject [expr $ID + 11]"
  .mbar.setObjectPositions.menu add separator
}

.mbar.setObjectPositions.menu add command -label "Hide Oval" -command ".cc delete theOval;set objectPositionSpecifiedEnabled -1;set objectPositionSpecified -1"
.mbar.setObjectPositions.menu add command -label "Remove all Placed Objects" -command resetManuallyPlacedObjects

pack .mbar.setObjectPositions -side left;

}
####################################################################
menubutton .mbar.preferences -text "Preferences" -menu .mbar.preferences.menu
menu .mbar.preferences.menu -tearoff 0
.mbar.preferences.menu add cascade \
                           -label "Set Color Scheme" \
                           -menu .mbar.preferences.menu.setColor 

menu .mbar.preferences.menu.setColor -tearoff 0
.mbar.preferences.menu.setColor add command \
                                    -label "Black on Light Grey" \
                                    -command "set BACKGROUNDCOLOR lightgrey; set FOREGROUNDCOLOR black; set HIGHLIGHTCOLOR black; configureColors" \
                                    -background lightgrey \
                                    -foreground black
.mbar.preferences.menu.setColor add command \
                                    -label "Black on Grey" \
                                    -command "set BACKGROUNDCOLOR grey; set FOREGROUNDCOLOR black; set HIGHLIGHTCOLOR black; configureColors" \
                                    -background grey \
                                    -foreground black
.mbar.preferences.menu.setColor add command \
                                    -label "Green on Black" \
                                    -command "set BACKGROUNDCOLOR black; set FOREGROUNDCOLOR green; set HIGHLIGHTCOLOR white; configureColors" \
                                    -background black \
                                    -foreground green
.mbar.preferences.menu.setColor add command \
                                    -label "Black on White" \
                                    -command "set BACKGROUNDCOLOR white; set FOREGROUNDCOLOR black; set HIGHLIGHTCOLOR white; configureColors" \
                                    -background white \
                                    -foreground black
.mbar.preferences.menu.setColor add command \
                                    -label "(Fake) OS X Colors" \
                                    -command "set BACKGROUNDCOLOR DodgerBlue; set FOREGROUNDCOLOR black; set HIGHLIGHTCOLOR black; configureColors" \
                                    -background DodgerBlue \
                                    -foreground black
.mbar.preferences.menu.setColor add command \
                                    -label "White on Black" \
                                    -command "set BACKGROUNDCOLOR black; set FOREGROUNDCOLOR white; set HIGHLIGHTCOLOR black; configureColors" \
                                    -background black \
                                    -foreground white
.mbar.preferences.menu.setColor add command \
                                    -label "Wizard Colors" \
                                    -command "set BACKGROUNDCOLOR white; set FOREGROUNDCOLOR white; set HIGHLIGHTCOLOR black; configureColors" \
                                    -background white \
                                    -foreground red
###################################################
menu .mbar.preferences.menu.toggleDest -tearoff 0
.mbar.preferences.menu add cascade \
                           -label "Display Destination" \
                           -menu .mbar.preferences.menu.toggleDest 

for {set robot 0} { $robot < [GetNumRobots_cmd]} {incr robot} \
{
   set destVariable displayRobotDestination$robot
   .mbar.preferences.menu.toggleDest add check \
                                     -label "Display Robot $robot Destination" \
                                     -variable $destVariable
}
#######################################################################################################################
menu .mbar.preferences.menu.toggleForcePassDest -tearoff 0
.mbar.preferences.menu add cascade \
                           -label "Display Pass Destination" \
                           -menu .mbar.preferences.menu.toggleForcePassDest

for {set robot 0} { $robot < [GetNumRobots_cmd]} {incr robot} \
{
   set forcePassDestVariable displayRobotPassDestination$robot
   .mbar.preferences.menu.toggleForcePassDest add check \
                                     -label "Force Display Robot $robot Pass Destination" \
                                     -variable $forcePassDestVariable
}
#######################################################################################################################
.mbar.preferences.menu add check \
                       -label "Automatic Pass Destinations" \
                       -variable automaticDisplayPassDest
.mbar.preferences.menu add check \
                       -label "(Alt-f) Toggle Field Lines" \
                       -variable displayFieldLines \
                       -command "drawField"
.mbar.preferences.menu add check \
                       -label "(Alt-s) Toggle Zone/Sector Lines" \
                       -variable displayZoneSectorLines \
                       -command "drawField"
#######################################################################################################################
.mbar.preferences.menu add command \
                       -label "(Alt-t) Toggle Complexity" \
                       -command "set toggleComplexity 1;"


#######################################################################################################################
.mbar.preferences.menu add command \
                       -label "Toggle Position Switching" \
                       -command "TogglePositionSwitching_cmd;"
#######################################################################################################################
.mbar.preferences.menu add command \
                       -label "Toggle Play Transitions" \
                       -command "TogglePlayTransitions_cmd;"
#######################################################################################################################
pack .mbar.preferences -side left
#######################################################################################################################
menubutton .mbar.tools -text "Tools" -menu .mbar.tools.menu
menu .mbar.tools.menu -tearoff 0
foreach {x} $TOOLSLIST1 {y} $TOOLSLIST2 \
{
  .mbar.tools.menu add command -label $x -command $y
}
pack .mbar.tools -side left
####################################################################
menubutton .mbar.help -text "Help" -menu .mbar.help.menu
menu .mbar.help.menu -tearoff 0
foreach {x} $HELPLIST1 {y} $HELPLIST2 \
{
  .mbar.help.menu add command -label $x -command $y
}
pack .mbar.help -side right
#######################################################################################################################
tk_menuBar .mbar .mbar.play
focus .mbar
pack .mbar -side top -fill x
#############################

}

drawMenus

#############################
####### CANVAS ##############
canvas .cc -width $CANVAS_WIDTH -height $CANVAS_HEIGHT
drawField
drawOurRobot    -10  +10   0            -10   +10   0 0   0   OUR_ROBOT0   0    0   0   0
drawOurRobot    -20  -20   0            -20   -20   0 0   0   OUR_ROBOT1   1    0   0   0
drawOurRobot     10   10   0             10    10   0 0   0   OUR_ROBOT2   2   10   0   0
drawOurRobot     20  -20   0             20   -20   0 0   0   OUR_ROBOT3   3    0   0   0
drawOurRobot    -20   20   0            -20    20   0 0   0   OUR_ROBOT4   4    0   0   0
drawTheirRobot  -10  +10   THEIR_ROBOT0   5
drawTheirRobot  -20  -20   THEIR_ROBOT1   6
drawTheirRobot   10   10   THEIR_ROBOT2   7
drawTheirRobot   20  -20   THEIR_ROBOT3   8
drawTheirRobot  -20   20   THEIR_ROBOT4   9
# canvas ends
#############################

##########################################
####### Big Red Robot Display ############
##########################################
frame .bigRedFrame -relief ridge -bd 0 -width $CANVAS_WIDTH

for {set i 0} {$i < [GetNumRobots_cmd]} {incr i} \
{
  frame .bigRedFrame.robot$i -relief ridge -bd 1
  #
  frame .bigRedFrame.robot$i.titleFrame
  #
  label .bigRedFrame.robot$i.titleFrame.l1 -text "Robot $i" -width 10 -height 1 -font -Adobe-Times-Medium-R-Mormal--*-120-*
  pack .bigRedFrame.robot$i.titleFrame.l1 -in .bigRedFrame.robot$i.titleFrame -side left
  #
  label .bigRedFrame.robot$i.titleFrame.l2 -text "Type: " -width 6 -height 1 -font -Adobe-Times-Medium-R-Mormal--*-120-*
  pack .bigRedFrame.robot$i.titleFrame.l2 -in .bigRedFrame.robot$i.titleFrame -side left
  #
  label .bigRedFrame.robot$i.titleFrame.l3 -textvariable ourRobotType$i -width 4 -height 1 -font -Adobe-Times-Medium-R-Mormal--*-120-*
  pack .bigRedFrame.robot$i.titleFrame.l3 -in .bigRedFrame.robot$i.titleFrame -side left
  #
  label .bigRedFrame.robot$i.titleFrame.l4 -textvariable ourRobotRole$i -width 20 -height 1
  pack .bigRedFrame.robot$i.titleFrame.l4 -in .bigRedFrame.robot$i.titleFrame -side left
  #
  label .bigRedFrame.robot$i.titleFrame.l5 -text "Message: " -width 7 
  pack .bigRedFrame.robot$i.titleFrame.l5 -in .bigRedFrame.robot$i.titleFrame -side left
  #
  label .bigRedFrame.robot$i.titleFrame.l6 -textvariable ourMessage$i -width 30
  pack .bigRedFrame.robot$i.titleFrame.l6 -in .bigRedFrame.robot$i.titleFrame -side left
  #
  frame .bigRedFrame.robot$i.positionFrame

  label .bigRedFrame.robot$i.positionFrame.l1 -text "Position = (" -width 10 -height 1
  label .bigRedFrame.robot$i.positionFrame.l2 -textvariable ourXPos$i -width 8 -height 1
  label .bigRedFrame.robot$i.positionFrame.l3 -text "," -width 3 -height 1
  label .bigRedFrame.robot$i.positionFrame.l4 -textvariable ourYPos$i -width 8 -height 1 
  label .bigRedFrame.robot$i.positionFrame.l5 -text ")   Angle = " -width 10  -height 1 
  label .bigRedFrame.robot$i.positionFrame.l6 -textvariable ourAngPos$i -width 8  -height 1
  label .bigRedFrame.robot$i.positionFrame.l7 -text "Dribbler=" -width 10  -height 1
  label .bigRedFrame.robot$i.positionFrame.l8 -textvariable ourDribbler$i -width 3  -height 1
  #
  frame .bigRedFrame.robot$i.sendFrame
 
  label .bigRedFrame.robot$i.sendFrame.l1 -text "Command=(" -width 10  -height 1
  label .bigRedFrame.robot$i.sendFrame.l2 -textvariable ourXCommand$i -width 8  -height 1
  label .bigRedFrame.robot$i.sendFrame.l3 -text "," -width 3 -height 1
  label .bigRedFrame.robot$i.sendFrame.l4 -textvariable ourYCommand$i -width 8  -height 1   
  label .bigRedFrame.robot$i.sendFrame.l5 -text ")  Theta = " -width 10  -height 1
  label .bigRedFrame.robot$i.sendFrame.l6 -textvariable ourAngCommand$i -width 8  -height 1
  label .bigRedFrame.robot$i.sendFrame.l7 -text "Kicker= " -width 10  -height 1
  label .bigRedFrame.robot$i.sendFrame.l8 -textvariable ourKicker$i -width 3  -height 1  
  #
  pack .bigRedFrame.robot$i.positionFrame.l1 .bigRedFrame.robot$i.positionFrame.l2 .bigRedFrame.robot$i.positionFrame.l3 .bigRedFrame.robot$i.positionFrame.l4 .bigRedFrame.robot$i.positionFrame.l5 .bigRedFrame.robot$i.positionFrame.l6 .bigRedFrame.robot$i.positionFrame.l7 .bigRedFrame.robot$i.positionFrame.l8 -in .bigRedFrame.robot$i.positionFrame -side left
  pack .bigRedFrame.robot$i.sendFrame.l1 .bigRedFrame.robot$i.sendFrame.l2 .bigRedFrame.robot$i.sendFrame.l3 .bigRedFrame.robot$i.sendFrame.l4 .bigRedFrame.robot$i.sendFrame.l5 .bigRedFrame.robot$i.sendFrame.l6 .bigRedFrame.robot$i.sendFrame.l7 .bigRedFrame.robot$i.sendFrame.l8 -in .bigRedFrame.robot$i.sendFrame -side left
  pack .bigRedFrame.robot$i.titleFrame .bigRedFrame.robot$i.positionFrame .bigRedFrame.robot$i.sendFrame -in .bigRedFrame.robot$i -side top
  pack .bigRedFrame.robot$i -in .bigRedFrame -side top -padx 1m -pady 0m
}
### End bigRedFrame ######################
##########################################

####### Play Frame #######################
frame .playFrame -relief ridge -bd 1 -width $CANVAS_WIDTH

frame .playFrame.top

frame .playFrame.latency

frame .playFrame.frameRate

label .playFrame.top.l1 -text "The Current Play = " -width 20 -height 1
label .playFrame.top.l2 -textvariable playName -width 30 -height 1

label .playFrame.frameRate.l1 -text "Last Sampled Frame Rate = " -width 30 -height 1
label .playFrame.frameRate.l2 -textvariable frameRate -width 12 -height 1

pack .playFrame.top.l1 .playFrame.top.l2 -in .playFrame.top -side left
pack .playFrame.frameRate.l1 .playFrame.frameRate.l2 -in .playFrame.frameRate -side left

pack .playFrame.top .playFrame.frameRate -side top

###########################################

####################################
### Ball Frame #####################
frame .ballFrame -relief ridge -bd 1 -width $CANVAS_WIDTH 
#
label .ballFrame.title -text Ball
pack .ballFrame.title -in .ballFrame -side top
#
label .ballFrame.l1 -text "Position = (" -width 10 -height 1
pack .ballFrame.l1 -in .ballFrame -side left
#
label .ballFrame.l2 -textvariable ballXPos -width 8 -height 1
pack .ballFrame.l2 -in .ballFrame -side left
#
label .ballFrame.l3 -text "," -width 3  
pack .ballFrame.l3 -in .ballFrame -side left
#
label .ballFrame.l4 -textvariable ballYPos -width 8 -height 1
pack .ballFrame.l4 -in .ballFrame -side left
#
label .ballFrame.l5 -text ")  Velocity = ( " -width 10 -height 1
pack .ballFrame.l5 -in .ballFrame -side left
#
label .ballFrame.l6 -textvariable ballXVel -width 8
pack .ballFrame.l6 -in .ballFrame -side left
#
label .ballFrame.l7 -text "," -width 3
pack .ballFrame.l7 -in .ballFrame -side left
#
label .ballFrame.l8 -textvariable ballYVel -width 8
pack .ballFrame.l8 -in .ballFrame -side left
#
label .ballFrame.l9 -text ") " -width 1
pack .ballFrame.l9 -in .ballFrame -side left
####################################

####### AdversaryFrame #############
frame .adversaryFrame -relief ridge -bd 1 -width $CANVAS_WIDTH

label .adversaryFrame.title -text Opponents
pack .adversaryFrame.title -in .adversaryFrame 

for {set i 0} {$i < [GetNumRobots_cmd]} {incr i} \
{
  frame .adversaryFrame.robot$i
  #
  label .adversaryFrame.robot$i.l1 -text "Position = (" -width 10 -height 1
  pack .adversaryFrame.robot$i.l1 -in .adversaryFrame.robot$i -side left
  #
  label .adversaryFrame.robot$i.l2 -textvariable theirXPos$i -width 8 -height 1
  pack .adversaryFrame.robot$i.l2 -in .adversaryFrame.robot$i -side left
  #
  label .adversaryFrame.robot$i.l3 -text "," -width 3
  pack .adversaryFrame.robot$i.l3 -in .adversaryFrame.robot$i -side left
  #
  label .adversaryFrame.robot$i.l4 -textvariable theirYPos$i -width 8 -height 1
  pack .adversaryFrame.robot$i.l4 -in .adversaryFrame.robot$i -side left
  #
  label .adversaryFrame.robot$i.l5 -text ")  Velocity = (" -width 10 -height 1
  pack .adversaryFrame.robot$i.l5 -in .adversaryFrame.robot$i -side left
  #
  label .adversaryFrame.robot$i.l6 -textvariable theirXVel$i -width 8 -height 1
  pack .adversaryFrame.robot$i.l6 -in .adversaryFrame.robot$i -side left
  #
  label .adversaryFrame.robot$i.l7 -text "," -width 3
  pack .adversaryFrame.robot$i.l7 -in .adversaryFrame.robot$i -side left
  #
  label .adversaryFrame.robot$i.l8 -textvariable theirYVel$i -width 8 -height 1
  pack .adversaryFrame.robot$i.l8 -in .adversaryFrame.robot$i -side left
  #
  label .adversaryFrame.robot$i.l9 -text ")" -width 1
  pack .adversaryFrame.robot$i.l9 -in .adversaryFrame.robot$i -side left
  #
  pack .adversaryFrame.robot$i -in .adversaryFrame -side top
}
####################################

####### Status Frame ##############################
frame .statusFrame -relief ridge -bd 1 -width $CANVAS_WIDTH

frame .statusFrame.top

frame .statusFrame.middle

frame .statusFrame.bottom

label .statusFrame.top.l1 -text "AI = " -width 10 -height 1
pack .statusFrame.top.l1 -in .statusFrame.top -side left
#
label .statusFrame.top.l2 -textvariable aiStatus -width 10 -height 1 
pack .statusFrame.top.l2 -in .statusFrame.top -side left
#
label .statusFrame.top.l3 -text "Side = " -width 10 -height 1
pack .statusFrame.top.l3 -in .statusFrame.top -side left
#
label .statusFrame.top.l4 -textvariable sideValue -width 10 -height 1
pack .statusFrame.top.l4 -in .statusFrame.top -side left
#
pack .statusFrame.top -in .statusFrame -side top
#
label .statusFrame.middle.l1 -text "Wireless = " -width 10 -height 1
pack .statusFrame.middle.l1 -in .statusFrame.middle -side left
#
label .statusFrame.middle.l2 -textvariable wirelessStatus -width 10 -height 1
pack .statusFrame.middle.l2 -in .statusFrame.middle -side left
#
label .statusFrame.middle.l3 -text "Team No = " -width 10 -height 1
pack .statusFrame.middle.l3 -in .statusFrame.middle -side left
#
label .statusFrame.middle.l4 -textvariable teamNumber -width 10 -height 1
pack .statusFrame.middle.l4 -in .statusFrame.middle -side left
#
pack .statusFrame.middle -in .statusFrame -side top
#
label .statusFrame.bottom.l1 -text "Vision = " -width 10 -height 1
pack .statusFrame.bottom.l1 -in .statusFrame.bottom -side left
#
label .statusFrame.bottom.l2 -textvariable visionStatus -width 10 -height 1
pack .statusFrame.bottom.l2 -in .statusFrame.bottom -side left
#
label .statusFrame.bottom.l3 -text "Frame No = " -width 10 -height 1
pack .statusFrame.bottom.l3 -in .statusFrame.bottom -side left
#
label .statusFrame.bottom.l4 -textvariable frameNumber -width 10 -height 1
pack .statusFrame.bottom.l4 -in .statusFrame.bottom -side left
#
pack .statusFrame.bottom -in .statusFrame -side top
####################################

########################################
####### WIDGET PLACEMENT BEGIN #########
########################################
# POSITIONING MENU, LABELS AND CANVAS
place .bigRedFrame    -x    0    -y   26
place .ballFrame      -x    70    -y   340
place .adversaryFrame -x    70    -y   390
place .playFrame      -x    110    -y   535
place .debugOutput -x 10 -y 710 -anchor sw -height 100 -width 500
place .cc             -x   510    -y   26   -anchor nw
place .statusFrame    -x    610   -y   727  -anchor sw

########################################
########################################

####PRINT DEBUG MESSAGES################
proc printDebugMessages {} \
{
  set num [getNumDebugMessages_cmd]

  for {set i 0} { $i < $num } {incr i} \
  {
    .debugOutput insert end [getDebugMessage]
    .debugOutput insert end \n
    .debugOutput yview end
  }
}
########################################

####CALIBRATE FIELD################
proc calibrateField {} \
{
   global fieldCalibrationStep
   global fieldCalibrationMode
   global normalBallMode
   global flipBack

   ## calibrate left side line only
   if { $fieldCalibrationMode == 0 } \
   {
     ## ignore return when not configing (0)
     if { $fieldCalibrationStep == 1 } \
     {
       addDebugMessage "=-=-=-=-=-=-=-=-=-=-="
       addDebugMessage "= Field Calibrator  ="
       addDebugMessage "=-=-=-=-=-=-=-=-=-=-="
       addDebugMessage "Turning AI Off"
       Set_AI_cmd 0

	   if { $flipBack == 0 } \
	   {
         if { [GetSidesFlippedStatus_cmd] == 1 } \
   	     {
           addDebugMessage "Sides were flipped, unflipping, but will put back when done. :)"
           TogglePlayingSide_cmd; eraseField; drawField;
	       set flipBack 1
	     } \
	     else \
	     {
	       set flipBack 0
  	     }
	   }

       addDebugMessage "Using Raw Ball Data"
       set normalBallMode 0
       addDebugMessage "Place Ball on LEFT SIDE LINE, then hit ENTER"
       set fieldCalibrationStep 2
     } \
     elseif { $fieldCalibrationStep == 2 } \
     {
	   RESET_LEFT_SIDE_LINE
       eraseField; drawField
       addDebugMessage "Place Ball on LEFT GOALIE BOX, then hit ENTER"
       set fieldCalibrationStep 3
     } \
     elseif { $fieldCalibrationStep == 3 } \
     {
       RESET_LEFT_GOALIE_BOX
       eraseField; drawField
       addDebugMessage "Place Ball on LEFT GOAL POST, then hit ENTER"
       set fieldCalibrationStep 4
     } \
     elseif { $fieldCalibrationStep == 4 } \
     {
       RESET_LEFT_GOAL_POST              
  	   RESET_OUR_LEFT_GOAL_WALL
       eraseField; drawField
       addDebugMessage "Place Ball on OUR GOAL LINE, then hit ENTER"
       set fieldCalibrationStep 5
     } \
     elseif { $fieldCalibrationStep == 5 } \
     {
       RESET_OUR_GOAL_LINE              
       eraseField; drawField
       addDebugMessage "Place Ball on OUR GOALIE BOX FRONT, then hit ENTER"
       set fieldCalibrationStep 6
     } \
     elseif { $fieldCalibrationStep == 6 } \
     {
	   RESET_GOALIE_BOX_DEPTH          
       eraseField; drawField 
       addDebugMessage "Place Ball on RIGHT GOAL POST, then hit ENTER"
       set fieldCalibrationStep 7
     } \
     elseif { $fieldCalibrationStep == 7 } \
     {
       RESET_RIGHT_GOAL_POST              
       eraseField; drawField
       addDebugMessage "Place Ball on RIGHT GOALIE BOX, then hit ENTER"
       set fieldCalibrationStep 8
     } \
     elseif { $fieldCalibrationStep == 8 } \
     {
       RESET_RIGHT_GOALIE_BOX             
       eraseField; drawField
       addDebugMessage "Place Ball on RIGHT SIDE LINE, then hit ENTER"
       set fieldCalibrationStep 9
     } \
     elseif { $fieldCalibrationStep == 9 } \
     {
       RESET_RIGHT_SIDE_LINE              
       eraseField; drawField
       addDebugMessage "Place Ball on THEIR LEFT GOAL WALL, then hit ENTER"
       set fieldCalibrationStep 10
     } \
     elseif { $fieldCalibrationStep == 10 } \
     {
       RESET_THEIR_LEFT_GOAL_WALL                
       eraseField; drawField
       addDebugMessage "Place Ball on THEIR GOAL LINE, then hit ENTER"
       set fieldCalibrationStep 11
     } \
     elseif { $fieldCalibrationStep == 11 } \
     {
       RESET_THEIR_GOAL_LINE         
       eraseField; drawField
       addDebugMessage "Place Ball on THEIR RIGHT GOAL WALL, then hit ENTER"
       set fieldCalibrationStep 12
     } \
     elseif { $fieldCalibrationStep == 12 } \
     {
       RESET_THEIR_RIGHT_GOAL_WALL         
       eraseField; drawField
       addDebugMessage "Place Ball on OUR LEFT CORNER, then hit ENTER"
       set fieldCalibrationStep 13
     } \
     elseif { $fieldCalibrationStep == 13 } \
     {
       RESET_OUR_LEFT_CORNER         
       eraseField; drawField
       addDebugMessage "Place Ball on OUR RIGHT CORNER, then hit ENTER"
       set fieldCalibrationStep 14
     } \
     elseif { $fieldCalibrationStep == 14 } \
     {
       RESET_OUR_RIGHT_CORNER         
       eraseField; drawField
       addDebugMessage "Place Ball on THEIR LEFT CORNER, then hit ENTER"
       set fieldCalibrationStep 15
     } \
     elseif { $fieldCalibrationStep == 15 } \
     {
       RESET_THEIR_LEFT_CORNER         
       eraseField; drawField
       addDebugMessage "Place Ball on THEIR RIGHT CORNER, then hit ENTER"
       set fieldCalibrationStep 16
     } \
     elseif { $fieldCalibrationStep == 16 } \
     {
       RESET_THEIR_RIGHT_CORNER         
       eraseField; drawField
       addDebugMessage "Place Ball on CENTER OF FIELD, then hit ENTER"
       set fieldCalibrationStep 17
     } \
     elseif { $fieldCalibrationStep == 17 } \
     {
       RESET_CENTER        
       eraseField; drawField
       addDebugMessage "Saving Field Parameters to File"
       SAVE_FIELD_PARAMETERS
       addDebugMessage "Using Filtered Ball Data"
       set normalBallMode 1
       addDebugMessage "Loading new Field Dimensions"
       reloadAllParameters
       if { $flipBack == 1 } \
  	   {
         addDebugMessage "Flipping back. See, I told you so. ;)"
         TogglePlayingSide_cmd; eraseField; drawField;
	     set flipBack 0
       }
       addDebugMessage "=-=-=-=-=-=-=-=-=-=-=-="
       addDebugMessage "= Ending Calibration  ="
       addDebugMessage "=-=-=-=-=-=-=-=-=-=-=-="
       set fieldCalibrationStep 0
     }
   } \
   else \
   {
     calibrateLine
   }
}
########################################
proc calibrateLine {} \
{
   global fieldCalibrationStep
   global fieldCalibrationMode
   global normalBallMode
   global flipBack

   if { $fieldCalibrationStep == 1 } \
   {
     addDebugMessage "=-=-=-=-=-=-=-=-=-=-="
     addDebugMessage "= Field Calibrator  ="
     addDebugMessage "=-=-=-=-=-=-=-=-=-=-="
     addDebugMessage "Turning AI Off"
     Set_AI_cmd 0
    
     if { $flipBack == 0 } \
     {
  	   if { [GetSidesFlippedStatus_cmd] == 1 } \
	   {
         addDebugMessage "Sides were flipped, unflipping, but will put back when done. :)"
         TogglePlayingSide_cmd; eraseField; drawField;
	     set flipBack 1
	   } \
	   else \
	   {
	     set flipBack 0
	   }
	 }

     addDebugMessage "Using Raw Ball Data"
     set normalBallMode 0

     if { $fieldCalibrationMode == 1 } \
	 {
       addDebugMessage "Place Ball on LEFT SIDE LINE, then hit ENTER"
	 } \
	 elseif { $fieldCalibrationMode == 2} \
	 {
       addDebugMessage "Place Ball on LEFT GOALIE BOX, then hit ENTER"
	 } \
	 elseif { $fieldCalibrationMode == 3} \
	 {
       addDebugMessage "Place Ball on OUR LEFT GOAL POST, then hit ENTER"
	 } \
	 elseif { $fieldCalibrationMode == 4} \
	 {
       addDebugMessage "Place Ball on OUR GOAL LINE, then hit ENTER"
	 } \
	 elseif { $fieldCalibrationMode == 5} \
	 {
       addDebugMessage "Place Ball on OUR FRONT GOALIE BOX, then hit ENTER"
	 } \
	 elseif { $fieldCalibrationMode == 6} \
	 {
       addDebugMessage "Place Ball on OUR RIGHT GOAL POST, then hit ENTER"
	 } \
	 elseif { $fieldCalibrationMode == 7} \
	 {
       addDebugMessage "Place Ball on RIGHT GOALIE BOX, then hit ENTER"
	 } \
	 elseif { $fieldCalibrationMode == 8} \
	 {
       addDebugMessage "Place Ball on RIGHT SIDE LINE, then hit ENTER"
	 } \
	 elseif { $fieldCalibrationMode == 9} \
	 {
       addDebugMessage "Place Ball on THEIR LEFT GOAL POST, then hit ENTER"
	 } \
	 elseif { $fieldCalibrationMode == 10} \
	 {
       addDebugMessage "Place Ball on THEIR GOAL LINE, then hit ENTER"
	 } \
	 elseif { $fieldCalibrationMode == 11} \
	 {
       addDebugMessage "Place Ball on THEIR RIGHT GOAL POST, then hit ENTER"
	 } \
	 elseif { $fieldCalibrationMode == 12} \
	 {
       addDebugMessage "Place Ball on OUR LEFT CORNER, then hit ENTER"
	 } \
	 elseif { $fieldCalibrationMode == 13} \
	 {
       addDebugMessage "Place Ball on OUR RIGHT CORNER, then hit ENTER"
	 } \
	 elseif { $fieldCalibrationMode == 14} \
	 {
       addDebugMessage "Place Ball on THEIR LEFT CORNER, then hit ENTER"
	 } \
	 elseif { $fieldCalibrationMode == 15} \
	 {
       addDebugMessage "Place Ball on THEIR RIGHT CORNER, then hit ENTER"
	 } \
	 elseif { $fieldCalibrationMode == 16} \
	 {
       addDebugMessage "Place Ball on CENTER OF FIELD, then hit ENTER"
	 }

     set fieldCalibrationStep 2
   } \
   elseif { $fieldCalibrationStep == 2 } \
   {
     if { $fieldCalibrationMode == 1 } \
	 { 
       RESET_LEFT_SIDE_LINE	 
       eraseField; drawField
	 } \
	 elseif { $fieldCalibrationMode == 2} \
	 {
       RESET_LEFT_GOALIE_BOX 
       eraseField; drawField
	 } \
	 elseif { $fieldCalibrationMode == 3} \
	 {
       RESET_LEFT_GOAL_POST              
  	   RESET_OUR_LEFT_GOAL_WALL
       eraseField; drawField
	 } \
	 elseif { $fieldCalibrationMode == 4} \
	 {
       RESET_OUR_GOAL_LINE 
       eraseField; drawField
	 } \
	 elseif { $fieldCalibrationMode == 5} \
	 {
       RESET_GOALIE_BOX_DEPTH 
       eraseField; drawField
	 } \
	 elseif { $fieldCalibrationMode == 6} \
	 {
       RESET_RIGHT_GOAL_POST              
  	   RESET_OUR_RIGHT_GOAL_WALL
       eraseField; drawField
	 } \
	 elseif { $fieldCalibrationMode == 7} \
	 {
       RESET_RIGHT_GOALIE_BOX 
       eraseField; drawField
	 } \
	 elseif { $fieldCalibrationMode == 8} \
	 {
       RESET_RIGHT_SIDE_LINE 
       eraseField; drawField
	 } \
	 elseif { $fieldCalibrationMode == 9} \
	 {
       RESET_THEIR_LEFT_GOAL_WALL 
       eraseField; drawField
	 } \
	 elseif { $fieldCalibrationMode == 10} \
	 {
       RESET_THEIR_GOAL_LINE
       eraseField; drawField
	 } \
	 elseif { $fieldCalibrationMode == 11} \
	 {
       RESET_THEIR_RIGHT_GOAL_WALL 
       eraseField; drawField
	 } \
	 elseif { $fieldCalibrationMode == 12} \
	 {
       RESET_OUR_LEFT_CORNER 
       eraseField; drawField
	 } \
	 elseif { $fieldCalibrationMode == 13} \
	 {
       RESET_OUR_RIGHT_CORNER 
       eraseField; drawField
	 } \
	 elseif { $fieldCalibrationMode == 14} \
	 {
       RESET_THEIR_LEFT_CORNER 
       eraseField; drawField
	 } \
	 elseif { $fieldCalibrationMode == 15} \
	 {
       RESET_THEIR_RIGHT_CORNER 
       eraseField; drawField
	 } \
	 elseif { $fieldCalibrationMode == 16} \
	 {
       RESET_CENTER 
       eraseField; drawField
	 }

     addDebugMessage "Saving Field Parameters to File"
     SAVE_FIELD_PARAMETERS
     addDebugMessage "Using Filtered Ball Data"
     set normalBallMode 1
     addDebugMessage "Loading new Field Dimensions"
     reloadAllParameters
     if { $flipBack == 1 } \
	 {
       addDebugMessage "Flipping back. See, I told you so. ;)"
       TogglePlayingSide_cmd; eraseField; drawField; 
	   set flipBack 0
     }

     addDebugMessage "=-=-=-=-=-=-=-=-=-=-=-="
     addDebugMessage "= Ending Calibration  ="
     addDebugMessage "=-=-=-=-=-=-=-=-=-=-=-="
     set fieldCalibrationStep 0
   }
}
########################################



######### SAVE FILE ####################
# Saves the log to the specified file
proc doSave {} \
{
  global saveFile

  # get the filename specifed in the common dialog box
  set saveFile [tk_getSaveFile]

  # if a filename is specified
  if {$saveFile != ""} \
  {
    SaveLogByName_cmd $saveFile
  }
}

proc doQuickSave {} \
{
  SaveLog_cmd
}
#############################

proc doQuit {} \
{
 ShutDown_cmd
 exit
}

proc displayLatencyTestResults {} \
{
  global BACKGROUNDCOLOR
  global FOREGROUNDCOLOR

  global minLatency
  global maxLatency
  global meanLatency

  toplevel .latencyResults
  .latencyResults config -bg $BACKGROUNDCOLOR
  wm title .latencyResults "Latency Results:"

  focus .latencyResults
  grab .latencyResults
  
  frame .latencyResults.resultsFrame 
  .latencyResults.resultsFrame config -bg $BACKGROUNDCOLOR

  frame .latencyResults.resultsFrame.line1
  .latencyResults.resultsFrame.line1 config -bg $BACKGROUNDCOLOR
  label .latencyResults.resultsFrame.line1.announcement -text "Latency Test Results:" -width 40 -height 1
  .latencyResults.resultsFrame.line1.announcement config -bg $BACKGROUNDCOLOR
  .latencyResults.resultsFrame.line1.announcement config -fg $FOREGROUNDCOLOR
  pack .latencyResults.resultsFrame.line1.announcement -in .latencyResults.resultsFrame.line1 -side left

  frame .latencyResults.resultsFrame.line2
  .latencyResults.resultsFrame.line2 config -bg $BACKGROUNDCOLOR
  label .latencyResults.resultsFrame.line2.minLatency -text "Min. Frames Latency:  " -width 30 -height 1
  .latencyResults.resultsFrame.line2.minLatency config -bg $BACKGROUNDCOLOR
  .latencyResults.resultsFrame.line2.minLatency config -fg $FOREGROUNDCOLOR
  label .latencyResults.resultsFrame.line2.minLatencyNum -textvariable minLatency -width 10 -height 1
  .latencyResults.resultsFrame.line2.minLatencyNum config -bg $BACKGROUNDCOLOR
  .latencyResults.resultsFrame.line2.minLatencyNum config -fg $FOREGROUNDCOLOR
  pack .latencyResults.resultsFrame.line2.minLatency .latencyResults.resultsFrame.line2.minLatencyNum -in .latencyResults.resultsFrame.line2 -side left
   
  frame .latencyResults.resultsFrame.line3
  .latencyResults.resultsFrame.line3 config -bg $BACKGROUNDCOLOR
  label .latencyResults.resultsFrame.line3.maxLatency -text "Max. Frames Latency:  " -width 30 -height 1
  .latencyResults.resultsFrame.line3.maxLatency config -bg $BACKGROUNDCOLOR
  .latencyResults.resultsFrame.line3.maxLatency config -fg $FOREGROUNDCOLOR
  label .latencyResults.resultsFrame.line3.maxLatencyNum -textvariable maxLatency -width 10 -height 1
  .latencyResults.resultsFrame.line3.maxLatencyNum config -bg $BACKGROUNDCOLOR
  .latencyResults.resultsFrame.line3.maxLatencyNum config -fg $FOREGROUNDCOLOR
  pack .latencyResults.resultsFrame.line3.maxLatency .latencyResults.resultsFrame.line3.maxLatencyNum -in .latencyResults.resultsFrame.line3 -side left

  frame .latencyResults.resultsFrame.line4
  .latencyResults.resultsFrame.line4 config -bg $BACKGROUNDCOLOR
  label .latencyResults.resultsFrame.line4.meanLatency -text "Mean Frames Latency:  " -width 30 -height 1
  .latencyResults.resultsFrame.line4.meanLatency config -bg $BACKGROUNDCOLOR
  .latencyResults.resultsFrame.line4.meanLatency config -fg $FOREGROUNDCOLOR
  label .latencyResults.resultsFrame.line4.meanLatencyNum -textvariable meanLatency -width 10 -height 1
  .latencyResults.resultsFrame.line4.meanLatencyNum config -bg $BACKGROUNDCOLOR
  .latencyResults.resultsFrame.line4.meanLatencyNum config -fg $FOREGROUNDCOLOR
  pack .latencyResults.resultsFrame.line4.meanLatency .latencyResults.resultsFrame.line4.meanLatencyNum -in .latencyResults.resultsFrame.line4 -side left

  pack .latencyResults.resultsFrame.line1 .latencyResults.resultsFrame.line2 .latencyResults.resultsFrame.line3  .latencyResults.resultsFrame.line4 -in  .latencyResults.resultsFrame -side top

  pack .latencyResults.resultsFrame

  label .latencyResults.ok -width 35 -text "Are you happy?" 
  .latencyResults.ok config -bg $BACKGROUNDCOLOR
  .latencyResults.ok config -fg $FOREGROUNDCOLOR
  frame .latencyResults.buttonframe
  .latencyResults.buttonframe config -bg $BACKGROUNDCOLOR  
  button .latencyResults.ok1 -width 10 -text "Yes!" -command {focus .; grab release .latencyResults; destroy .latencyResults }
  button .latencyResults.ok2 -width 10 -text "Yes!" -command {focus .; grab release .latencyResults; destroy .latencyResults }
  pack .latencyResults.ok1 .latencyResults.ok2 -in .latencyResults.buttonframe -side left

  pack .latencyResults.ok .latencyResults.buttonframe

}


proc helpAboutAI {} \
{
  global BACKGROUNDCOLOR
  global FOREGROUNDCOLOR

  toplevel .helpAbout
  .helpAbout config -bg $BACKGROUNDCOLOR
  wm title .helpAbout "Big Red RoboCup AI 2002  -  About AI"

  focus .helpAbout
  
  frame .helpAbout.titleFrame 
  .helpAbout.titleFrame config -bg $BACKGROUNDCOLOR

  frame .helpAbout.messageFrame 
  .helpAbout.messageFrame config -bg $BACKGROUNDCOLOR

  frame .helpAbout.titleFrame.line1
  .helpAbout.titleFrame.line1 config -bg $BACKGROUNDCOLOR

  label .helpAbout.titleFrame.line1.announcement -text "Cornell Big Red RoboCup AI 2002:" -width 80 -height 1
  .helpAbout.titleFrame.line1.announcement config -bg $BACKGROUNDCOLOR
  .helpAbout.titleFrame.line1.announcement config -fg $FOREGROUNDCOLOR
  pack .helpAbout.titleFrame.line1.announcement -in .helpAbout.titleFrame.line1 -side left

  pack .helpAbout.titleFrame.line1 -in  .helpAbout.titleFrame -side top

  frame .helpAbout.messageFrame.line1
  .helpAbout.messageFrame.line1 config -bg $BACKGROUNDCOLOR
  label .helpAbout.messageFrame.line1.announcement -text "-Will Stokes\n \
                                                          -Joel Chestnutt\n \
                                                          -Evan Kuhn\n \
                                                          -Evan Malone \n \
                                                          -Nirav Shah\n \
                                                          -Akosua Kyereme-Tuah\n \
                                                          -Michael Babish"
  .helpAbout.messageFrame.line1.announcement config -bg $BACKGROUNDCOLOR                      
  .helpAbout.messageFrame.line1.announcement config -fg $FOREGROUNDCOLOR                      

  pack .helpAbout.messageFrame.line1.announcement -in .helpAbout.messageFrame.line1 -side left
  
  pack .helpAbout.messageFrame.line1 -in  .helpAbout.messageFrame -side top

  pack .helpAbout.titleFrame .helpAbout.messageFrame
}

proc helpRunningInstructions {} \
{
  global BACKGROUNDCOLOR
  global FOREGROUNDCOLOR

  toplevel .runningInstructions
  .runningInstructions config -bg $BACKGROUNDCOLOR
  wm title .runningInstructions "Big Red RoboCup AI 2002  -  Running Instructions"

  focus .runningInstructions
  
  frame .runningInstructions.titleFrame 
  .runningInstructions.titleFrame config -bg $BACKGROUNDCOLOR

  frame .runningInstructions.messageFrame 
  .runningInstructions.messageFrame config -bg $BACKGROUNDCOLOR

  frame .runningInstructions.titleFrame.line1
  .runningInstructions.titleFrame.line1 config -bg $BACKGROUNDCOLOR

  label .runningInstructions.titleFrame.line1.announcement -text "Instructions for running the artificial intelligence:" -width 80 -height 1
  .runningInstructions.titleFrame.line1.announcement config -bg $BACKGROUNDCOLOR
  .runningInstructions.titleFrame.line1.announcement config -fg $FOREGROUNDCOLOR
  pack .runningInstructions.titleFrame.line1.announcement -in .runningInstructions.titleFrame.line1 -side left

  pack .runningInstructions.titleFrame.line1 -in  .runningInstructions.titleFrame -side top

  frame .runningInstructions.messageFrame.line1
  .runningInstructions.messageFrame.line1 config -bg $BACKGROUNDCOLOR

  label .runningInstructions.messageFrame.line1.announcement -text "1.) Turn on wireless (F3) \n \
                                                          2.) Turn on vision (if not already done) (F2) \n \
                                                          -------------------------- \n \
                                                          3.) Click on Robot->Initalize All to set all vision robots to have a position \n \
                                                          4.) Click on Play and select a play \n \
                                                          5.) Turn on the AI (F1) \n \
                                                          --------------------------- \n \
                                                          To stop robots at any time either stop the AI by pressing spacebar \n \
                                                          or hit F3 to toggle wireless. \n \
                                                          --------------------------- \n \
                                                          Hit F4 to switch sides, \n \
                                                          Hit Control-L to run the latency test \n \
                                                          Hit F5 to log data to a log file in BRML format"
    
  .runningInstructions.messageFrame.line1.announcement config -bg $BACKGROUNDCOLOR
  .runningInstructions.messageFrame.line1.announcement config -fg $FOREGROUNDCOLOR

  pack .runningInstructions.messageFrame.line1.announcement -in .runningInstructions.messageFrame.line1 -side left
  
  pack .runningInstructions.messageFrame.line1 -in  .runningInstructions.messageFrame -side top

  pack .runningInstructions.titleFrame .runningInstructions.messageFrame
}

##########################################################################################
##########################################################################################
##########################################################################################
##########################################################################################
##########################################################################################
##########################################################################################
##########################################################################################

######################
# Vision Quality GUI #
######################
proc  visionQualityInfo {} \
{
  global BACKGROUNDCOLOR
  global FOREGROUNDCOLOR
  #how many frames are logged in the C-Code
  global framesAnalized

  #set this variable to 0 when finished running a vision test.
  #setting it to 1 prevent the main window from updating.
  global DONE
  global updateTimingData
    
  #these are used to set the colors being displayed
  set red 0
  set green 1
  set blue 2
  set yellow 3

  ############################
  # Set Window Size and Name #
  ############################
  toplevel  .visionQualityInfo
  .visionQualityInfo config -bg $BACKGROUNDCOLOR
  wm title .visionQualityInfo "Vision Quality Test" 
  wm geometry .visionQualityInfo 1300x800
  wm maxsize .visionQualityInfo 1300 800
  wm minsize .visionQualityInfo 1300 800
  focus .visionQualityInfo

  #####################################
  # Create and Insert the title frame #
  #####################################
  frame .visionQualityInfo.title
  .visionQualityInfo.title config -bg $BACKGROUNDCOLOR

  label .visionQualityInfo.title.text -text "Vision Quality:"
  .visionQualityInfo.title.text configure -bg [booleanColor [expr $yellow] ]
  pack .visionQualityInfo.title.text -in .visionQualityInfo.title -side left
  pack .visionQualityInfo.title -in .visionQualityInfo

  ############################################
  # Create and Insert the friendRobots frame #
  ############################################
  frame .visionQualityInfo.friendRobots
  .visionQualityInfo.friendRobots config -bg $BACKGROUNDCOLOR

  for {set robotID 0} {$robotID < [GetNumRobots_cmd]} {incr robotID} \
  {
    #--------------------------------------------
    frame .visionQualityInfo.friendRobots.friend$robotID  -relief ridge -bd 2
    .visionQualityInfo.friendRobots.friend$robotID config -bg $BACKGROUNDCOLOR
    #--------------------------------------------
    frame .visionQualityInfo.friendRobots.friend$robotID.line1
    .visionQualityInfo.friendRobots.friend$robotID.line1 config -bg $BACKGROUNDCOLOR

    label .visionQualityInfo.friendRobots.friend$robotID.line1.name -text "Friend $robotID" -width 10 -height 1
    .visionQualityInfo.friendRobots.friend$robotID.line1.name config -bg $BACKGROUNDCOLOR
    .visionQualityInfo.friendRobots.friend$robotID.line1.name config -fg $FOREGROUNDCOLOR
    pack .visionQualityInfo.friendRobots.friend$robotID.line1.name -in .visionQualityInfo.friendRobots.friend$robotID.line1 -side left

    label .visionQualityInfo.friendRobots.friend$robotID.line1.text -text "Found: " -width 20 -height 1
    .visionQualityInfo.friendRobots.friend$robotID.line1.text config -bg $BACKGROUNDCOLOR
    .visionQualityInfo.friendRobots.friend$robotID.line1.text config -fg $FOREGROUNDCOLOR
    pack .visionQualityInfo.friendRobots.friend$robotID.line1.text -in .visionQualityInfo.friendRobots.friend$robotID.line1 -side left

    for {set frame 0} {$frame < [expr $framesAnalized]} {incr frame} \
    {
      label .visionQualityInfo.friendRobots.friend$robotID.line1.found$frame -text "-" -width 1 -height 1
      .visionQualityInfo.friendRobots.friend$robotID.line1.found$frame configure -bg [booleanColor 0]
      pack .visionQualityInfo.friendRobots.friend$robotID.line1.found$frame -in .visionQualityInfo.friendRobots.friend$robotID.line1 -side left 
    }

    pack .visionQualityInfo.friendRobots.friend$robotID.line1 -in .visionQualityInfo.friendRobots.friend$robotID 
    #--------------------------------------------
    frame .visionQualityInfo.friendRobots.friend$robotID.line2
    .visionQualityInfo.friendRobots.friend$robotID.line2 config -bg $BACKGROUNDCOLOR

    label .visionQualityInfo.friendRobots.friend$robotID.line2.name -text " " -width 10 -height 1
    .visionQualityInfo.friendRobots.friend$robotID.line2.name config -bg $BACKGROUNDCOLOR
    .visionQualityInfo.friendRobots.friend$robotID.line2.name config -fg $FOREGROUNDCOLOR
    pack .visionQualityInfo.friendRobots.friend$robotID.line2.name -in .visionQualityInfo.friendRobots.friend$robotID.line2 -side left

    label .visionQualityInfo.friendRobots.friend$robotID.line2.text -text "Position Stability: " -width 20 -height 1
    .visionQualityInfo.friendRobots.friend$robotID.line2.text config -bg $BACKGROUNDCOLOR
    .visionQualityInfo.friendRobots.friend$robotID.line2.text config -fg $FOREGROUNDCOLOR
    pack .visionQualityInfo.friendRobots.friend$robotID.line2.text -in .visionQualityInfo.friendRobots.friend$robotID.line2 -side left

    for {set frame 0} {$frame < [expr $framesAnalized]} {incr frame} \
    {
      label .visionQualityInfo.friendRobots.friend$robotID.line2.positionStability$frame -text "-" -width 1 -height 1
      .visionQualityInfo.friendRobots.friend$robotID.line2.positionStability$frame configure -bg [booleanColor [expr $red] ]
      pack .visionQualityInfo.friendRobots.friend$robotID.line2.positionStability$frame -in .visionQualityInfo.friendRobots.friend$robotID.line2 -side left 
    }

    pack .visionQualityInfo.friendRobots.friend$robotID.line2 -in .visionQualityInfo.friendRobots.friend$robotID 
    #--------------------------------------------
    frame .visionQualityInfo.friendRobots.friend$robotID.line3
    .visionQualityInfo.friendRobots.friend$robotID.line3 config -bg $BACKGROUNDCOLOR

    label .visionQualityInfo.friendRobots.friend$robotID.line3.name -text " " -width 10 -height 1
    .visionQualityInfo.friendRobots.friend$robotID.line3.name config -bg $BACKGROUNDCOLOR
    .visionQualityInfo.friendRobots.friend$robotID.line3.name config -fg $FOREGROUNDCOLOR
    pack .visionQualityInfo.friendRobots.friend$robotID.line3.name -in .visionQualityInfo.friendRobots.friend$robotID.line3  -side left

    label .visionQualityInfo.friendRobots.friend$robotID.line3.text -text "Rotation Stability: " -width 20 -height 1
    .visionQualityInfo.friendRobots.friend$robotID.line3.text config -bg $BACKGROUNDCOLOR
    .visionQualityInfo.friendRobots.friend$robotID.line3.text config -fg $FOREGROUNDCOLOR
    pack .visionQualityInfo.friendRobots.friend$robotID.line3.text -in .visionQualityInfo.friendRobots.friend$robotID.line3  -side left

    for {set frame 0} {$frame < [expr $framesAnalized]} {incr frame} \
    {
      label .visionQualityInfo.friendRobots.friend$robotID.line3.rotationStability$frame -text "-" -width 1 -height 1
      .visionQualityInfo.friendRobots.friend$robotID.line3.rotationStability$frame configure -bg [booleanColor [expr $red] ]
      pack .visionQualityInfo.friendRobots.friend$robotID.line3.rotationStability$frame -in .visionQualityInfo.friendRobots.friend$robotID.line3 -side left 
    }

    pack .visionQualityInfo.friendRobots.friend$robotID.line3 -in .visionQualityInfo.friendRobots.friend$robotID 
    #--------------------------------------------
    pack .visionQualityInfo.friendRobots.friend$robotID -in .visionQualityInfo.friendRobots
    #--------------------------------------------
  }
  pack .visionQualityInfo.friendRobots -in .visionQualityInfo

  ####################################
  # Create and Insert the ball frame #
  ####################################
  frame .visionQualityInfo.ball -relief ridge -bd 2
  .visionQualityInfo.ball config -bg $BACKGROUNDCOLOR
  #--------------------------------------------
  frame .visionQualityInfo.ball.line1
  .visionQualityInfo.ball.line1 config -bg $BACKGROUNDCOLOR

  label .visionQualityInfo.ball.line1.name -text "Ball" -width 10 -height 1
  .visionQualityInfo.ball.line1.name config -bg $BACKGROUNDCOLOR
  .visionQualityInfo.ball.line1.name config -fg $FOREGROUNDCOLOR
  pack .visionQualityInfo.ball.line1.name  -in .visionQualityInfo.ball.line1 -side left

  label .visionQualityInfo.ball.line1.text -text "Found: " -width 20 -height 1
  .visionQualityInfo.ball.line1.text config -bg $BACKGROUNDCOLOR
  .visionQualityInfo.ball.line1.text config -fg $FOREGROUNDCOLOR
  pack .visionQualityInfo.ball.line1.text  -in .visionQualityInfo.ball.line1 -side left

  for {set frame 0} {$frame < [expr $framesAnalized]} {incr frame} \
  {
    label .visionQualityInfo.ball.line1.found$frame -text "-" -width 1 -height 1
    .visionQualityInfo.ball.line1.found$frame configure -bg [booleanColor [expr $red] ]
    pack .visionQualityInfo.ball.line1.found$frame -in .visionQualityInfo.ball.line1 -side left
  }
  #--------------------------------------------
  frame .visionQualityInfo.ball.line2
  .visionQualityInfo.ball.line2 config -bg $BACKGROUNDCOLOR

  label .visionQualityInfo.ball.line2.name -text " " -width 10 -height 1
  .visionQualityInfo.ball.line2.name config -bg $BACKGROUNDCOLOR
  .visionQualityInfo.ball.line2.name config -fg $FOREGROUNDCOLOR
  pack .visionQualityInfo.ball.line2.name -in .visionQualityInfo.ball.line2 -side left

  label .visionQualityInfo.ball.line2.text -text "Position Stability: " -width 20 -height 1
  .visionQualityInfo.ball.line2.text config -bg $BACKGROUNDCOLOR
  .visionQualityInfo.ball.line2.text config -fg $FOREGROUNDCOLOR
  pack .visionQualityInfo.ball.line2.text -in .visionQualityInfo.ball.line2 -side left

  for {set frame 0} {$frame < [expr $framesAnalized]} {incr frame} \
  {
    label .visionQualityInfo.ball.line2.positionStability$frame -text "-" -width 1 -height 1
    .visionQualityInfo.ball.line2.positionStability$frame configure -bg [booleanColor [expr $red] ]
    pack .visionQualityInfo.ball.line2.positionStability$frame -in .visionQualityInfo.ball.line2 -side left 
  }
  #--------------------------------------------
  pack .visionQualityInfo.ball.line1 .visionQualityInfo.ball.line2 -in .visionQualityInfo.ball
  pack .visionQualityInfo.ball -in .visionQualityInfo
  #--------------------------------------------

  ##############################################
  # Create and Insert the opponentRobots frame #
  ##############################################
  frame .visionQualityInfo.opponentRobots
  .visionQualityInfo.opponentRobots config -bg $BACKGROUNDCOLOR

  for {set robotID 0} {$robotID < [GetNumRobots_cmd]} {incr robotID} \
  {
    frame .visionQualityInfo.opponentRobots.opponent$robotID  -relief ridge -bd 2
    .visionQualityInfo.opponentRobots.opponent$robotID config -bg $BACKGROUNDCOLOR
    #--------------------------------------------
    frame .visionQualityInfo.opponentRobots.opponent$robotID.line1
    .visionQualityInfo.opponentRobots.opponent$robotID.line1 config -bg $BACKGROUNDCOLOR

    label .visionQualityInfo.opponentRobots.opponent$robotID.line1.name -text "Opponent $robotID" -width 10 -height 1
    .visionQualityInfo.opponentRobots.opponent$robotID.line1.name config -bg $BACKGROUNDCOLOR
    .visionQualityInfo.opponentRobots.opponent$robotID.line1.name config -fg $FOREGROUNDCOLOR
    pack .visionQualityInfo.opponentRobots.opponent$robotID.line1.name -in .visionQualityInfo.opponentRobots.opponent$robotID.line1 -side left

    label .visionQualityInfo.opponentRobots.opponent$robotID.line1.text -text "Found: " -width 20 -height 1
    .visionQualityInfo.opponentRobots.opponent$robotID.line1.text config -bg $BACKGROUNDCOLOR
    .visionQualityInfo.opponentRobots.opponent$robotID.line1.text config -fg $FOREGROUNDCOLOR
    pack .visionQualityInfo.opponentRobots.opponent$robotID.line1.text -in .visionQualityInfo.opponentRobots.opponent$robotID.line1 -side left

    for {set frame 0} {$frame < [expr $framesAnalized]} {incr frame} \
    {
      label .visionQualityInfo.opponentRobots.opponent$robotID.line1.found$frame -text "-" -width 1 -height 1
      .visionQualityInfo.opponentRobots.opponent$robotID.line1.found$frame configure -bg [booleanColor [expr $red] ]
      pack .visionQualityInfo.opponentRobots.opponent$robotID.line1.found$frame -in .visionQualityInfo.opponentRobots.opponent$robotID.line1 -side left 
    }

    pack .visionQualityInfo.opponentRobots.opponent$robotID.line1 -in .visionQualityInfo.opponentRobots.opponent$robotID 
    #--------------------------------------------
    frame .visionQualityInfo.opponentRobots.opponent$robotID.line2
    .visionQualityInfo.opponentRobots.opponent$robotID.line2 config -bg $BACKGROUNDCOLOR

    label .visionQualityInfo.opponentRobots.opponent$robotID.line2.name -text " " -width 10 -height 1
    .visionQualityInfo.opponentRobots.opponent$robotID.line2.name config -bg $BACKGROUNDCOLOR
    .visionQualityInfo.opponentRobots.opponent$robotID.line2.name config -fg $FOREGROUNDCOLOR
    pack .visionQualityInfo.opponentRobots.opponent$robotID.line2.name -in .visionQualityInfo.opponentRobots.opponent$robotID.line2 -side left

    label .visionQualityInfo.opponentRobots.opponent$robotID.line2.text -text "Position Stability: " -width 20 -height 1
    .visionQualityInfo.opponentRobots.opponent$robotID.line2.text config -bg $BACKGROUNDCOLOR
    .visionQualityInfo.opponentRobots.opponent$robotID.line2.text config -fg $FOREGROUNDCOLOR
    pack .visionQualityInfo.opponentRobots.opponent$robotID.line2.text -in .visionQualityInfo.opponentRobots.opponent$robotID.line2 -side left

    for {set frame 0} {$frame < [expr $framesAnalized]} {incr frame} \
    {
      label .visionQualityInfo.opponentRobots.opponent$robotID.line2.positionStability$frame -text "-" -width 1 -height 1
      .visionQualityInfo.opponentRobots.opponent$robotID.line2.positionStability$frame configure -bg [booleanColor [expr $red] ]
      pack .visionQualityInfo.opponentRobots.opponent$robotID.line2.positionStability$frame -in .visionQualityInfo.opponentRobots.opponent$robotID.line2 -side left 
    }

    pack .visionQualityInfo.opponentRobots.opponent$robotID.line2 -in .visionQualityInfo.opponentRobots.opponent$robotID 
    #--------------------------------------------
    pack .visionQualityInfo.opponentRobots.opponent$robotID -in .visionQualityInfo.opponentRobots
  }

  pack .visionQualityInfo.opponentRobots -in .visionQualityInfo

  ########################################
  # Create and Insert the progress frame #
  ########################################
  frame .visionQualityInfo.progress
  .visionQualityInfo.progress config -bg $BACKGROUNDCOLOR
  #--------------------------------------------
  label .visionQualityInfo.progress.name -text "Activity: " -width 31 -height 1
  .visionQualityInfo.progress.name config -bg $BACKGROUNDCOLOR
  .visionQualityInfo.progress.name config -fg $FOREGROUNDCOLOR
  pack .visionQualityInfo.progress.name  -in .visionQualityInfo.progress -side left

  for {set frame 0} {$frame < [expr $framesAnalized]} {incr frame} \
  {
    label .visionQualityInfo.progress.found$frame -text "-" -width 1 -height 1
    .visionQualityInfo.progress.found$frame configure -bg [booleanColor [expr $blue] ]
    pack .visionQualityInfo.progress.found$frame -in .visionQualityInfo.progress -side left
  }
  #--------------------------------------------
  pack .visionQualityInfo.progress -in .visionQualityInfo
  #--------------------------------------------


  ########################################
  # Create and Insert the Frame # and Vision Test Frame No Frame #
  ########################################
  frame .visionQualityInfo.information
  .visionQualityInfo.information config -bg $BACKGROUNDCOLOR

  label .visionQualityInfo.information.frameText -text "Frame No = " -width 10 -height 1
  .visionQualityInfo.information.frameText config -bg $BACKGROUNDCOLOR
  .visionQualityInfo.information.frameText config -fg $FOREGROUNDCOLOR
  label .visionQualityInfo.information.frame -textvariable frameNumber -width 10 -height 1
  .visionQualityInfo.information.frame config -bg $BACKGROUNDCOLOR
  .visionQualityInfo.information.frame config -fg $FOREGROUNDCOLOR
  pack .visionQualityInfo.information.frameText .visionQualityInfo.information.frame -in .visionQualityInfo.information -side left

  label .visionQualityInfo.information.curVisionFrameText -text "Vision Test Frame No = " -width 20 -height 1
  .visionQualityInfo.information.curVisionFrameText config -bg $BACKGROUNDCOLOR
  .visionQualityInfo.information.curVisionFrameText config -fg $FOREGROUNDCOLOR
  label .visionQualityInfo.information.curVisionFrame -textvariable currentframeAnalized -width 10 -height 1
  .visionQualityInfo.information.curVisionFrame config -bg $BACKGROUNDCOLOR
  .visionQualityInfo.information.curVisionFrame config -fg $FOREGROUNDCOLOR
  pack .visionQualityInfo.information.curVisionFrameText .visionQualityInfo.information.curVisionFrame -in .visionQualityInfo.information -side left

  pack .visionQualityInfo.information -in .visionQualityInfo

  ########################################
  # Create and Insert Various Running Time Information
  ########################################
  frame .visionQualityInfo.timing
  frame .visionQualityInfo.timingVals
  .visionQualityInfo.timing config -bg $BACKGROUNDCOLOR
  .visionQualityInfo.timingVals config -bg $BACKGROUNDCOLOR

  #----------------------------
  # Vision Execute Time (ms)
  #----------------------------
  label .visionQualityInfo.timing.frameNumber \
        -text "Frame #" \
        -width 20 \
        -height 1
  .visionQualityInfo.timing.frameNumber config -bg $BACKGROUNDCOLOR
  .visionQualityInfo.timing.frameNumber config -fg $FOREGROUNDCOLOR
  #----------------------------
  # Vision Execute Time (ms)
  #----------------------------
  label .visionQualityInfo.timing.visionExecuteTime \
        -text "Vision Execute Time" \
        -width 20 \
        -height 1
  .visionQualityInfo.timing.visionExecuteTime config -bg $BACKGROUNDCOLOR
  .visionQualityInfo.timing.visionExecuteTime config -fg $FOREGROUNDCOLOR
  #----------------------------
  # Vision Get Time (ms)
  #----------------------------
  label .visionQualityInfo.timing.visionGetTime \
        -text "Vision Get Time" \
        -width 20 \
        -height 1
  .visionQualityInfo.timing.visionGetTime config -bg $BACKGROUNDCOLOR
  .visionQualityInfo.timing.visionGetTime config -fg $FOREGROUNDCOLOR
  #----------------------------
  # Wireless Execute Time (ms)
  #----------------------------
  label .visionQualityInfo.timing.wirelessExecuteTime \
        -text "Wireless Execute Time" \
        -width 20 \
        -height 1
  .visionQualityInfo.timing.wirelessExecuteTime config -bg $BACKGROUNDCOLOR
  .visionQualityInfo.timing.wirelessExecuteTime config -fg $FOREGROUNDCOLOR
  #----------------------------
  # Simulator Execute Time (ms)
  #----------------------------
  label .visionQualityInfo.timing.simulatorExecuteTime \
        -text "Simulator Send Time" \
        -width 20 \
        -height 1
  .visionQualityInfo.timing.simulatorExecuteTime config -bg $BACKGROUNDCOLOR
  .visionQualityInfo.timing.simulatorExecuteTime config -fg $FOREGROUNDCOLOR
  #----------------------------
  # Strategy Execute Time (ms)
  #----------------------------
  label .visionQualityInfo.timing.strategyExecuteTime \
        -text "Strategy Execute Time" \
        -width 20 \
        -height 1
  .visionQualityInfo.timing.strategyExecuteTime config -bg $BACKGROUNDCOLOR
  .visionQualityInfo.timing.strategyExecuteTime config -fg $FOREGROUNDCOLOR
  #----------------------------
  # Trajectory Execute Time (ms)
  #----------------------------
  label .visionQualityInfo.timing.trajectoryExecuteTime \
        -text "Trajectory Execute Time" \
        -width 20 \
        -height 1
  .visionQualityInfo.timing.trajectoryExecuteTime config -bg $BACKGROUNDCOLOR
  .visionQualityInfo.timing.trajectoryExecuteTime config -fg $FOREGROUNDCOLOR
  #----------------------------
  # Total Execute Time (ms)
  #----------------------------
  label .visionQualityInfo.timing.totalExecuteTime \
        -text "Total Execute Time" \
        -width 20 \
        -height 1
  .visionQualityInfo.timing.totalExecuteTime config -bg $BACKGROUNDCOLOR
  .visionQualityInfo.timing.totalExecuteTime config -fg $FOREGROUNDCOLOR
  #----------------------------
  # AI FPS
  #----------------------------
  label .visionQualityInfo.timing.aiFPS \
        -text "AI FPS" \
        -width 20 \
        -height 1
  .visionQualityInfo.timing.aiFPS config -bg $BACKGROUNDCOLOR
  .visionQualityInfo.timing.aiFPS config -fg $FOREGROUNDCOLOR
  #----------------------------
  # Vision FPS
  #----------------------------
  label .visionQualityInfo.timing.visionFPS \
        -text "Vision FPS" \
        -width 20 \
        -height 1
  .visionQualityInfo.timing.visionFPS config -bg $BACKGROUNDCOLOR
  .visionQualityInfo.timing.visionFPS config -fg $FOREGROUNDCOLOR
  #-------------
  pack .visionQualityInfo.timing.frameNumber \
       .visionQualityInfo.timing.visionExecuteTime \
       .visionQualityInfo.timing.visionGetTime \
       .visionQualityInfo.timing.wirelessExecuteTime \
       .visionQualityInfo.timing.simulatorExecuteTime \
       .visionQualityInfo.timing.strategyExecuteTime \
       .visionQualityInfo.timing.trajectoryExecuteTime \
       .visionQualityInfo.timing.totalExecuteTime \
       .visionQualityInfo.timing.aiFPS \
       .visionQualityInfo.timing.visionFPS \
       -in .visionQualityInfo.timing \
       -side left
  #-------------
  pack .visionQualityInfo.timing \
       -in .visionQualityInfo
  #-------------
  # Frame #
  #-------------
  text .visionQualityInfo.timingVals.frameNumber \
       -relief sunken \
       -width 20 \
       -height 10
  #-------------
  # Vision Execute Time Window
  #-------------
  text .visionQualityInfo.timingVals.visionExecuteTime \
       -relief sunken \
       -width 20 \
       -height 10
  #-------------
  # Vision Get Time Window
  #-------------
  text .visionQualityInfo.timingVals.visionGetTime \
       -relief sunken \
       -width 20 \
       -height 10
  #-------------
  # Wireless Execute Time Window
  #-------------
  text .visionQualityInfo.timingVals.wirelessExecuteTime \
       -relief sunken \
       -width 20 \
       -height 10
  #-------------
  # Simulator Execute Time Window
  #-------------
  text .visionQualityInfo.timingVals.simulatorExecuteTime \
       -relief sunken \
       -width 20 \
       -height 10
  #-------------
  # Strategy Execute Time Window
  #-------------
  text .visionQualityInfo.timingVals.strategyExecuteTime \
       -relief sunken \
       -width 20 \
       -height 10
  #-------------
  # Trajectory Execute Time Window
  #-------------
  text .visionQualityInfo.timingVals.trajectoryExecuteTime \
       -relief sunken \
       -width 20 \
       -height 10
  #-------------
  # Total Execute Time Window
  #-------------
  text .visionQualityInfo.timingVals.totalExecuteTime \
       -relief sunken \
       -width 20 \
       -height 10
  #-------------
  # AI FPS Window
  #-------------
  text .visionQualityInfo.timingVals.aiFPS \
       -relief sunken \
       -width 20 \
       -height 10
  #-------------
  # Vision FPS Window
  #-------------
  text .visionQualityInfo.timingVals.visionFPS \
       -relief sunken \
       -width 20 \
       -height 10
  #-------------
  pack .visionQualityInfo.timingVals.frameNumber \
       .visionQualityInfo.timingVals.visionExecuteTime \
       .visionQualityInfo.timingVals.visionGetTime \
       .visionQualityInfo.timingVals.wirelessExecuteTime \
       .visionQualityInfo.timingVals.simulatorExecuteTime \
       .visionQualityInfo.timingVals.strategyExecuteTime \
       .visionQualityInfo.timingVals.trajectoryExecuteTime \
       .visionQualityInfo.timingVals.totalExecuteTime \
       .visionQualityInfo.timingVals.aiFPS \
       .visionQualityInfo.timingVals.visionFPS \
       -in .visionQualityInfo.timingVals \
       -side left
  #-------------
  pack .visionQualityInfo.timingVals \
       -in .visionQualityInfo
  
  #-------------

  #-------------
  # Buttons
  #-------------
  frame .visionQualityInfo.buttons
  button .visionQualityInfo.buttons.quit -text "Close" -command "set DONE 0"
  button .visionQualityInfo.buttons.start -text "Start" -command "set updateTimingData 1"
  button .visionQualityInfo.buttons.stop -text "Stop" -command "set updateTimingData 0"
  pack .visionQualityInfo.buttons.quit \
       .visionQualityInfo.buttons.start \
       .visionQualityInfo.buttons.stop \
       -in .visionQualityInfo.buttons \
       -side left
  pack .visionQualityInfo.buttons -in .visionQualityInfo
  #-------------
  
  set updateTimingData 1

  #######################################################################################
  #######################################################################################
  #######################################################################################
  #######################################################################################
  #Define procedure to update the visionQualityStatus window
    

  proc updateVisionQualityInfo {} \
  {
    global updateTimingData
    global framesAnalized
    global DONE
    global currentframeAnalized
    global frameNumber

    set red 0
    set green 1
    set blue 2
    set yellow 3

    set oldframeAnalized currentframeAnalized
    set currentframeAnalized [visionQualityFrame_cmd]
    set frameNumber [GetCurrentFrame_cmd]

    ###################################################################
    ## update timing data ###
    if { $updateTimingData } \
    {
    .visionQualityInfo.timingVals.frameNumber insert end $frameNumber
    .visionQualityInfo.timingVals.frameNumber insert end \n
    .visionQualityInfo.timingVals.frameNumber yview end

    .visionQualityInfo.timingVals.visionExecuteTime insert end [get_VISION_EXECUTE_TIME]
    .visionQualityInfo.timingVals.visionExecuteTime insert end \n
    .visionQualityInfo.timingVals.visionExecuteTime yview end

    .visionQualityInfo.timingVals.visionGetTime insert end [get_VISION_GET_TIME]
    .visionQualityInfo.timingVals.visionGetTime insert end \n
    .visionQualityInfo.timingVals.visionGetTime yview end

    .visionQualityInfo.timingVals.wirelessExecuteTime insert end [get_WIRELESS_EXECUTE_TIME]
    .visionQualityInfo.timingVals.wirelessExecuteTime insert end \n
    .visionQualityInfo.timingVals.wirelessExecuteTime yview end

    .visionQualityInfo.timingVals.simulatorExecuteTime insert end [get_SIMULATOR_EXECUTE_TIME]
    .visionQualityInfo.timingVals.simulatorExecuteTime insert end \n
    .visionQualityInfo.timingVals.simulatorExecuteTime yview end

    .visionQualityInfo.timingVals.strategyExecuteTime insert end [get_STRATEGY_EXECUTE_TIME]
    .visionQualityInfo.timingVals.strategyExecuteTime insert end \n
    .visionQualityInfo.timingVals.strategyExecuteTime yview end

    .visionQualityInfo.timingVals.trajectoryExecuteTime insert end [get_TRAJECTORY_EXECUTE_TIME]
    .visionQualityInfo.timingVals.trajectoryExecuteTime insert end \n
    .visionQualityInfo.timingVals.trajectoryExecuteTime yview end

    .visionQualityInfo.timingVals.totalExecuteTime insert end [get_TOTAL_EXECUTE_TIME]
    .visionQualityInfo.timingVals.totalExecuteTime insert end \n
    .visionQualityInfo.timingVals.totalExecuteTime yview end

    .visionQualityInfo.timingVals.aiFPS insert end [get_AI_FPS]
    .visionQualityInfo.timingVals.aiFPS insert end \n
    .visionQualityInfo.timingVals.aiFPS yview end

    .visionQualityInfo.timingVals.visionFPS insert end [get_VISION_FPS]
    .visionQualityInfo.timingVals.visionFPS insert end \n
    .visionQualityInfo.timingVals.visionFPS yview end
    } 

    ###################################################################
    #ball found
    if { [ballFound_cmd $currentframeAnalized] == 1 } \
    {
      .visionQualityInfo.ball.line1.found$currentframeAnalized configure -bg [booleanColor [expr $green] ]
    } \
    else \
    {
      .visionQualityInfo.ball.line1.found$currentframeAnalized configure -bg [booleanColor [expr $red] ]
    }
    ###################################################################
    #ball position stable
    if { [ballPositionStable_cmd $currentframeAnalized] == 1 } \
    {
      .visionQualityInfo.ball.line2.positionStability$currentframeAnalized configure -bg [booleanColor [expr $green] ]
    } \
    else \
    {
      .visionQualityInfo.ball.line2.positionStability$currentframeAnalized configure -bg [booleanColor [expr $red] ]
    }
    ###################################################################
    #progress meter
    if { [visionQualityFrame_cmd] == $currentframeAnalized } \
    {
      .visionQualityInfo.progress.found$currentframeAnalized configure -bg [booleanColor [expr $yellow] ]
    } \
    else \
    {
      .visionQualityInfo.progress.found$currentframeAnalized configure -bg [booleanColor [expr $blue] ]
    }

    if { $currentframeAnalized == 0 } \
    {
      set j [expr $framesAnalized - 1]
    } \
    else \
    {
      set j [expr $currentframeAnalized - 1]
    } 

    .visionQualityInfo.progress.found$j configure -bg [booleanColor [expr $blue] ]
    ###################################################################


    for {set ID 0} {$ID < [GetNumRobots_cmd]} {incr ID} \
    {
      ###################################################################
      #friendly robot found
      if { [friendlyRobotFound_cmd $ID $currentframeAnalized] == 1 } \
      {
        .visionQualityInfo.friendRobots.friend$ID.line1.found$currentframeAnalized configure -bg [booleanColor [expr $green] ]
      } \
      else \
      {
        .visionQualityInfo.friendRobots.friend$ID.line1.found$currentframeAnalized configure -bg [booleanColor [expr $red] ] 
      }
      ###################################################################
      #friendly robot position stable
      if { [friendlyPositionStable_cmd $ID $currentframeAnalized] == 1 } \
      {
        .visionQualityInfo.friendRobots.friend$ID.line2.positionStability$currentframeAnalized configure -bg [booleanColor [expr $green] ]
      } \
      else \
      {
        .visionQualityInfo.friendRobots.friend$ID.line2.positionStability$currentframeAnalized configure -bg [booleanColor [expr $red] ]
      }
      ###################################################################
      #friendly robot rotation stable
      if { [friendlyRotationStable_cmd $ID $currentframeAnalized] == 1 } \
      {
        .visionQualityInfo.friendRobots.friend$ID.line3.rotationStability$currentframeAnalized configure -bg [booleanColor [expr $green] ]
      } \
      else \
      {
        .visionQualityInfo.friendRobots.friend$ID.line3.rotationStability$currentframeAnalized configure -bg [booleanColor [expr $red] ]
      }
      ###################################################################
      #opponent found
      if { [opponentFound_cmd $ID $currentframeAnalized] == 1 } \
      {
        .visionQualityInfo.opponentRobots.opponent$ID.line1.found$currentframeAnalized configure -bg [booleanColor [expr $green] ]
      } \
      else \
      {
        .visionQualityInfo.opponentRobots.opponent$ID.line1.found$currentframeAnalized configure -bg [booleanColor [expr $red] ]
      }
      ###################################################################
      #opponent position stable
      if { [opponentPositionStable_cmd $ID $currentframeAnalized] == 1 } \
      {
        .visionQualityInfo.opponentRobots.opponent$ID.line2.positionStability$currentframeAnalized configure -bg [booleanColor [expr $green] ]
      } \
      else \
      {
        .visionQualityInfo.opponentRobots.opponent$ID.line2.positionStability$currentframeAnalized configure -bg [booleanColor [expr $red] ]
      }
      ###################################################################
    }

    #if we have not clicked close, delay, then update again
    #if we clicked on close, then kill the window and don't update
       if { $DONE == 1 } \
    {
       after 1 updateVisionQualityInfo 
    } \
    else \
    {
      destroy .visionQualityInfo 
      Update_Screen
    }
  }

  # end defining the procedure to update the vision quality window
  ##########################################################################################################
  ##########################################################################################################
  ##########################################################################################################
  ##########################################################################################################

  #once window has been created, update it with real values!
  updateVisionQualityInfo

}
##########################################################################################
##########################################################################################
##########################################################################################
##########################################################################################
proc resetManuallyPlacedObjects {} \
{
  for {set ID 0} { $ID < [GetNumRobots_cmd]} {incr ID} \
  {
    global friendlySetPositionX$ID
    set friendlySetPositionX$ID        -32000
    global friendlySetPositionY$ID
    set friendlySetPositionY$ID        -32000
    global friendlySetPositionRotation$ID
    set friendlySetPositionRotation$ID -32000

    global opponentSetPositionX$ID
    set opponentSetPositionX$ID        -32000
    global opponentSetPositionY$ID
    set opponentSetPositionY$ID        -32000

    global ballSetPositionX
    set ballSetPositionX               -32000
    global ballSetPositionY
    set ballSetPositionY               -32000   
  }

  for {set ID 0} { $ID < 5 } {incr ID} \
  {
    SetRobotLocation_cmd $ID -32000 -32000
  }
  for {set ID 5} { $ID < 10 } {incr ID} \
  {
    SetRobotRotation_cmd $ID 0
  }
  for {set ID 10} { $ID < 16 } {incr ID} \
  {
    SetRobotLocation_cmd $ID -32000 -32000
  }

  global objectPositionSpecifiedEnabled
  if { $objectPositionSpecifiedEnabled != -1 } \
  {
    .cc delete theOval
  }

  global objectPositionSpecified
  set objectPositionSpecifiedEnabled -1
  set objectPositionSpecified -1
}
##########################################################################################
##########################################################################################
##########################################################################################
##########################################################################################
proc convertSetVariable {} \
{
   global objectPositionSpecified
   global objectConverted

   if { \
        $objectPositionSpecified == "Set Friendly Robot 0 Location (q)" || \
        $objectPositionSpecified == 0 \
      } \
   {
      set objectConverted 0;
   } \
   elseif { \
             $objectPositionSpecified == "Set Friendly Robot 1 Location (w)" || \
             $objectPositionSpecified == 1 \
          } \
   {
      set objectConverted 1;
   } \
   elseif { \
            $objectPositionSpecified == "Set Friendly Robot 2 Location (e)" || \
            $objectPositionSpecified == 2 \
          } \
   {
      set objectConverted 2;
   } \
   elseif { \
            $objectPositionSpecified == "Set Friendly Robot 3 Location (r)" || \
            $objectPositionSpecified == 3 \
          } \
   {
      set objectConverted 3;
   } \
   elseif { \
            $objectPositionSpecified == "Set Friendly Robot 4 Location (t)" || \
            $objectPositionSpecified == 4 \
          } \
   {
      set objectConverted 4;
   } \
   elseif { \
            $objectPositionSpecified == "Set Friendly Robot 0 Rotation (a)" || \
            $objectPositionSpecified == 5 \
          } \
   {
      set objectConverted 5;
   } \
   elseif { \
            $objectPositionSpecified == "Set Friendly Robot 1 Rotation (s)" || \
            $objectPositionSpecified == 6 \
          } \
   {
      set objectConverted 6;
   } \
   elseif { \
            $objectPositionSpecified == "Set Friendly Robot 2 Rotation (d)" || \
            $objectPositionSpecified == 7 \
          } \
   {
      set objectConverted 7;
   } \
   elseif { \
            $objectPositionSpecified == "Set Friendly Robot 3 Rotation (f)" || \
            $objectPositionSpecified == 8 \
          } \
   {
      set objectConverted 8;
   } \
   elseif { \
            $objectPositionSpecified == "Set Friendly Robot 4 Rotation (g)" || \
            $objectPositionSpecified == 9 \
          } \
   {
      set objectConverted 9;
   } \
   elseif { \
            $objectPositionSpecified == "Set Ball Location (`)" || \
            $objectPositionSpecified == 10 \
          } \
   {
      set objectConverted 10;
   } \
   elseif { \
            $objectPositionSpecified == "Set Opponent Robot 0 Location (z)" || \
            $objectPositionSpecified == 11 \
          } \
   {
      set objectConverted 11;
   } \
   elseif { \
            $objectPositionSpecified == "Set Opponent Robot 1 Location (x)" || \
            $objectPositionSpecified == 12 \
          } \
   {
      set objectConverted 12;
   } \
   elseif { \
            $objectPositionSpecified == "Set Opponent Robot 2 Location (c)" || \
            $objectPositionSpecified == 13 \
          } \
   {
      set objectConverted 13;
   } \
   elseif { \
            $objectPositionSpecified == "Set Opponent Robot 3 Location (v)" || \
            $objectPositionSpecified == 14 \
          } \
   {
      set objectConverted 14;
   } \
   elseif { \
            $objectPositionSpecified == "Set Opponent Robot 4 Location (b)" || \
            $objectPositionSpecified == 15 \
          } \
   {
      set objectConverted 15;
   } \
   else \
   {
      set objectConverted -1
   }
}
##########################################################################################
##########################################################################################
##########################################################################################
##########################################################################################
proc createOval {} \
{
  global objectSpecifiedX
  global objectSpecifiedY
  global dragOffset
  set radius 9
  .cc create oval [expr $objectSpecifiedX - $radius + $dragOffset] \
                  [expr $objectSpecifiedY - $radius - $dragOffset] \
                  [expr $objectSpecifiedX + $radius + $dragOffset] \
                  [expr $objectSpecifiedY + $radius - $dragOffset] \
                  -outline black \
                  -fill green \
                  -tag theOval

  .cc bind theOval \
          <B1-Motion> \
          "plotMove %x %y" 

  .cc bind theOval \
          <Any-Enter> \
          ".cc itemconfig current -fill red"

  .cc bind theOval \
          <Any-Leave> \
          ".cc itemconfig current -fill green"
  .cc bind theOval <3> ".cc delete theOval;set objectPositionSpecifiedEnabled -1;set objectPositionSpecified -1"

}
##########################################################################################
##########################################################################################
##########################################################################################
##########################################################################################
proc plotMove { \
                newX \
                    newY \
              } \
{ 
  global objectSpecifiedX
  global objectSpecifiedY
  .cc move current [expr $newX - $objectSpecifiedX ] [expr $newY - $objectSpecifiedY]
  set objectSpecifiedX $newX
  set objectSpecifiedY $newY
}
##########################################################################################
##########################################################################################
##########################################################################################
##########################################################################################
proc removeObject { \
                    ID \
                  } \
{
  global objectPositionSpecified
  global objectPositionSpecifiedEnabled
  global objectConverted
  global friendlySetPositionX$ID
  global friendlySetPositionY$ID
  global friendlySetPositionRotation$ID
  global opponentSetPositionX$ID
  global opponentSetPositionY$ID
  global ballSetPositionX
  global ballSetPositionY

  if { \
        $objectConverted == $ID ||
        $objectConverted == [expr $ID + 5] \
     } \
  { 
    .cc delete theOval
     set objectPositionSpecifiedEnabled -1
     set objectPositionSpecified -1
  } \

  #always set the object to be lost
  if { $ID < 10 } \
  {
    set friendlySetPositionX$ID -32000
    set friendlySetPositionY$ID -32000 
    set friendlySetPositionRotation$ID 0 
  } \
  elseif { $ID == 10 } \
  {
    set ballSetPositionX -32000
    set ballSetPositionY -32000
  } \
  else \
  {
    set opponentSetPositionX$ID -32000
    set opponentSetPositionY$ID -32000
  }  

  SetRobotLocation_cmd $ID -32000 -32000

  SetRobotRotation_cmd [expr $ID + 5] 0
}
##########################################################################################
##########################################################################################
##########################################################################################
##########################################################################################
proc configureColors {} \
{
  ######################
  global BACKGROUNDCOLOR 
  global FOREGROUNDCOLOR 
  global MENUCOLOR  
  global MENUTEXT 
  global HIGHLIGHTCOLOR
  global complexMode
  ######################
  for {set ID 0} { $ID < [GetNumRobots_cmd]} {incr ID} \
  {
    ################################################################
    .bigRedFrame.robot$ID config -bg $BACKGROUNDCOLOR
    ################################################################
    .bigRedFrame.robot$ID.titleFrame config -bg $BACKGROUNDCOLOR
    ################################################################
    .bigRedFrame.robot$ID.titleFrame.l1 config -bg $BACKGROUNDCOLOR
    ################################################################
    .bigRedFrame.robot$ID.titleFrame.l2 config -bg $BACKGROUNDCOLOR
    ################################################################
    .bigRedFrame.robot$ID.titleFrame.l3 config -bg $BACKGROUNDCOLOR
    ################################################################
    .bigRedFrame.robot$ID.titleFrame.l4 config -bg $BACKGROUNDCOLOR
    ################################################################
    .bigRedFrame.robot$ID.positionFrame config -bg $BACKGROUNDCOLOR
    ################################################################
    .bigRedFrame.robot$ID.positionFrame.l1 config -bg $BACKGROUNDCOLOR
    ################################################################
    .bigRedFrame.robot$ID.positionFrame.l2 config -bg $BACKGROUNDCOLOR  
    ################################################################
    .bigRedFrame.robot$ID.positionFrame.l3 config -bg $BACKGROUNDCOLOR  
    ################################################################
    .bigRedFrame.robot$ID.positionFrame.l4 config -bg $BACKGROUNDCOLOR  
    ################################################################
    .bigRedFrame.robot$ID.positionFrame.l5 config -bg $BACKGROUNDCOLOR  
    ################################################################
    .bigRedFrame.robot$ID.positionFrame.l6 config -bg $BACKGROUNDCOLOR  
    ################################################################
    .bigRedFrame.robot$ID.positionFrame.l7 config -bg $BACKGROUNDCOLOR  
    ################################################################
    .bigRedFrame.robot$ID.positionFrame.l8 config -bg $BACKGROUNDCOLOR  
    ################################################################
    .bigRedFrame.robot$ID.sendFrame config -bg $BACKGROUNDCOLOR
    ################################################################
    .bigRedFrame.robot$ID.sendFrame.l1 config -bg $BACKGROUNDCOLOR  
    ################################################################
    .bigRedFrame.robot$ID.sendFrame.l2 config -bg $BACKGROUNDCOLOR  
    ################################################################
    .bigRedFrame.robot$ID.sendFrame.l3 config -bg $BACKGROUNDCOLOR  
    ################################################################
    .bigRedFrame.robot$ID.sendFrame.l4 config -bg $BACKGROUNDCOLOR  
    ################################################################
    .bigRedFrame.robot$ID.sendFrame.l5 config -bg $BACKGROUNDCOLOR  
    ################################################################
    .bigRedFrame.robot$ID.sendFrame.l6 config -bg $BACKGROUNDCOLOR  
    ################################################################
    .bigRedFrame.robot$ID.sendFrame.l7 config -bg $BACKGROUNDCOLOR  
    ################################################################
    .bigRedFrame.robot$ID.sendFrame.l8 config -bg $BACKGROUNDCOLOR  
    ################################################################
    .adversaryFrame.robot$ID config -bg $BACKGROUNDCOLOR
    ################################################################
    .adversaryFrame.robot$ID.l1 config -bg $BACKGROUNDCOLOR
    ################################################################
    .adversaryFrame.robot$ID.l2 config -bg $BACKGROUNDCOLOR
    ################################################################
    .adversaryFrame.robot$ID.l3 config -bg $BACKGROUNDCOLOR
    ################################################################
    .adversaryFrame.robot$ID.l4 config -bg $BACKGROUNDCOLOR
    ################################################################
    .adversaryFrame.robot$ID.l5 config -bg $BACKGROUNDCOLOR
    ################################################################
    .adversaryFrame.robot$ID.l6 config -bg $BACKGROUNDCOLOR
    ################################################################
    .adversaryFrame.robot$ID.l7 config -bg $BACKGROUNDCOLOR
    ################################################################
    .adversaryFrame.robot$ID.l8 config -bg $BACKGROUNDCOLOR
    ################################################################
    .adversaryFrame.robot$ID.l9 config -bg $BACKGROUNDCOLOR
    ################################################################
   }
  ##############################################
  .mbar config -bg $MENUCOLOR
  ##############################################
  .mbar.systemControl config -bg $MENUCOLOR
  .mbar.systemControl.menu config -bg $MENUCOLOR
  .mbar.systemControl config -fg $MENUTEXT
  .mbar.systemControl.menu config -fg $MENUTEXT
  ##############################################
  .mbar.robotPosition config -bg $MENUCOLOR
  .mbar.robotPosition.menu config -bg $MENUCOLOR
  .mbar.robotPosition config -fg $MENUTEXT
  .mbar.robotPosition.menu config -fg $MENUTEXT
  ##############################################
  for {set ID 0} { $ID < [GetNumRobots_cmd]} {incr ID} \
  {
    .mbar.robotPosition.menu.positionMenu$ID config -bg $MENUCOLOR
    .mbar.robotPosition.menu.positionMenu$ID config -fg $MENUTEXT
  }
  ##############################################

  .adversaryFrame config -bg $BACKGROUNDCOLOR
  .adversaryFrame.title config -bg $BACKGROUNDCOLOR

   ###################################################
  .mbar.setPlay config -bg $MENUCOLOR
  .mbar.setPlay.menu config -bg $MENUCOLOR
   ###################################################
   ###################################################
   ###################################################
   ###################################################
   ###################################################
  .mbar.setPlay.menu.2002play config -bg $MENUCOLOR
  .mbar.setPlay.menu.2002play config -fg $MENUTEXT
   ###################################################
  .mbar.setPlay.menu.2002penaltyPlay config -bg $MENUCOLOR
  .mbar.setPlay.menu.2002penaltyPlay config -fg $MENUTEXT
   ###################################################
  .mbar.setPlay.menu.formation config -bg $MENUCOLOR
  .mbar.setPlay.menu.formation config -fg $MENUTEXT
   ###################################################
  .mbar.setPlay.menu.intimidation config -bg $MENUCOLOR
  .mbar.setPlay.menu.intimidation config -fg $MENUTEXT
   ###################################################
   if { $complexMode == 1 } \
   {
     #################################################
    .mbar.setPlay.menu.willSkillz config -bg $MENUCOLOR
    .mbar.setPlay.menu.willSkillz config -fg $MENUTEXT
     ###################################################
    .mbar.setPlay.menu.niravSkillz config -bg $MENUCOLOR
    .mbar.setPlay.menu.willSkillz config -fg $MENUTEXT
     ###################################################
    .mbar.setPlay.menu.joelSkillz config -bg $MENUCOLOR
    .mbar.setPlay.menu.joelSkillz config -fg $MENUTEXT
     ###################################################
    .mbar.setPlay.menu.evanMaloneSkillz config -bg $MENUCOLOR
    .mbar.setPlay.menu.evanMaloneSkillz config -fg $MENUTEXT
     ###################################################
    .mbar.setPlay.menu.evanKuhnSkillz config -bg $MENUCOLOR
    .mbar.setPlay.menu.evanKuhnSkillz config -fg $MENUTEXT
     ###################################################
    .mbar.setPlay.menu.individual config -bg $MENUCOLOR
    .mbar.setPlay.menu.individual config -fg $MENUTEXT
     ###################################################
    .mbar.setPlay.menu.other config -bg $MENUCOLOR
    .mbar.setPlay.menu.other config -fg $MENUTEXT
     ###################################################
    .mbar.setObjectPositions config -bg $MENUCOLOR
    .mbar.setObjectPositions.menu config -bg $MENUCOLOR
    .mbar.setObjectPositions config -fg $MENUTEXT
    .mbar.setObjectPositions.menu config -fg $MENUTEXT
     #################################################
   }
   ###################################################
   .mbar.preferences config -bg $MENUCOLOR
   .mbar.preferences.menu config -bg $MENUCOLOR
   .mbar.preferences config -fg $MENUTEXT
   .mbar.preferences.menu config -fg $MENUTEXT
    ###################################################
   .mbar.help config -bg $MENUCOLOR
   .mbar.help.menu config -bg $MENUCOLOR
   .mbar.help config -fg $MENUTEXT
   .mbar.help.menu config -fg $MENUTEXT
   ###################################################
   ###################################################
   ###################################################
   ###################################################
   ###################################################
  .cc config -bg $BACKGROUNDCOLOR   
  .bigRedFrame config -bg $BACKGROUNDCOLOR
  .playFrame config -bg $BACKGROUNDCOLOR
  .playFrame.top config -bg $BACKGROUNDCOLOR
  .playFrame.latency config -bg $BACKGROUNDCOLOR
  .playFrame.frameRate config -bg $BACKGROUNDCOLOR
  .playFrame.top.l1 config -bg $BACKGROUNDCOLOR
  .playFrame.top.l2 config -bg $BACKGROUNDCOLOR
  .playFrame.frameRate.l1 config -bg $BACKGROUNDCOLOR
  .playFrame.frameRate.l2 config -bg $BACKGROUNDCOLOR
  .ballFrame config -bg $BACKGROUNDCOLOR
  .ballFrame.title config -bg $BACKGROUNDCOLOR
  .ballFrame.l1 config -bg $BACKGROUNDCOLOR
  .ballFrame.l2 config -bg $BACKGROUNDCOLOR
  .ballFrame.l3 config -bg $BACKGROUNDCOLOR
  .ballFrame.l4 config -bg $BACKGROUNDCOLOR
  .ballFrame.l5 config -bg $BACKGROUNDCOLOR
  .ballFrame.l6 config -bg $BACKGROUNDCOLOR
  .ballFrame.l7 config -bg $BACKGROUNDCOLOR
  .ballFrame.l8 config -bg $BACKGROUNDCOLOR
  .ballFrame.l9 config -bg $BACKGROUNDCOLOR
  .statusFrame config -bg $BACKGROUNDCOLOR
  .statusFrame.top config -bg $BACKGROUNDCOLOR
  .statusFrame.middle config -bg $BACKGROUNDCOLOR
  .statusFrame.bottom config -bg $BACKGROUNDCOLOR
  .statusFrame.top.l1 config -bg $BACKGROUNDCOLOR
  .statusFrame.top.l2 config -bg $BACKGROUNDCOLOR
  .statusFrame.top.l3 config -bg $BACKGROUNDCOLOR
  .statusFrame.top.l4 config -bg $BACKGROUNDCOLOR
  .statusFrame.middle.l1 config -bg $BACKGROUNDCOLOR
  .statusFrame.middle.l2 config -bg $BACKGROUNDCOLOR
  .statusFrame.middle.l3 config -bg $BACKGROUNDCOLOR
  .statusFrame.middle.l4 config -bg $BACKGROUNDCOLOR
  .statusFrame.bottom.l1 config -bg $BACKGROUNDCOLOR
  .statusFrame.bottom.l2 config -bg $BACKGROUNDCOLOR
  .statusFrame.bottom.l3 config -bg $BACKGROUNDCOLOR
  .statusFrame.bottom.l4 config -bg $BACKGROUNDCOLOR
  . config -bg $BACKGROUNDCOLOR
  ##################################################
  .adversaryFrame.title config -fg $FOREGROUNDCOLOR

  .playFrame.top.l1 config -fg $FOREGROUNDCOLOR
  .playFrame.top.l2 config -fg $FOREGROUNDCOLOR
  .playFrame.frameRate.l1 config -fg $FOREGROUNDCOLOR
  .playFrame.frameRate.l2 config -fg $FOREGROUNDCOLOR
  .ballFrame.title config -fg $FOREGROUNDCOLOR
  .ballFrame.l1 config -fg $FOREGROUNDCOLOR
  .ballFrame.l2 config -fg $FOREGROUNDCOLOR
  .ballFrame.l3 config -fg $FOREGROUNDCOLOR
  .ballFrame.l4 config -fg $FOREGROUNDCOLOR
  .ballFrame.l5 config -fg $FOREGROUNDCOLOR
  .ballFrame.l6 config -fg $FOREGROUNDCOLOR
  .ballFrame.l7 config -fg $FOREGROUNDCOLOR
  .ballFrame.l8 config -fg $FOREGROUNDCOLOR
  .ballFrame.l9 config -fg $FOREGROUNDCOLOR
  .statusFrame.top.l1 config -fg $FOREGROUNDCOLOR
  .statusFrame.top.l2 config -fg $HIGHLIGHTCOLOR
  .statusFrame.top.l3 config -fg $FOREGROUNDCOLOR
  .statusFrame.top.l4 config -fg $FOREGROUNDCOLOR
  .statusFrame.middle.l1 config -fg $FOREGROUNDCOLOR
  .statusFrame.middle.l2 config -fg $HIGHLIGHTCOLOR
  .statusFrame.middle.l3 config -fg $FOREGROUNDCOLOR
  .statusFrame.middle.l4 config -fg $FOREGROUNDCOLOR
  .statusFrame.bottom.l1 config -fg $FOREGROUNDCOLOR
  .statusFrame.bottom.l2 config -fg $HIGHLIGHTCOLOR
  .statusFrame.bottom.l3 config -fg $FOREGROUNDCOLOR
  .statusFrame.bottom.l4 config -fg $FOREGROUNDCOLOR
}
##########################################################################################
##########################################################################################
##########################################################################################
##########################################################################################


#########################################################
# Cool colors:                                          #
# ------------                                          #
# white                                                 #
# lightgrey                                             #
# grey                                                  #
# black                                                 #
# yellow                                                #
# red                                                   #
# green                                                 #
# blue                                                  #
# lightblue                                             #
# MediumTurquoise #Actually, this sucks, still looking  #
#                 #for an Acqua like OS X color         #
# DodgerBlue                                            #
#########################################################
set BACKGROUNDCOLOR grey
set FOREGROUNDCOLOR black
set MENUCOLOR lightgrey     
set MENUTEXT black
set HIGHLIGHTCOLOR black


#set specified color scheme if parameter file set's it
if { [Default_Color_Scheme_cmd] == 1 } \
{
  set BACKGROUNDCOLOR lightgrey; 
  set FOREGROUNDCOLOR black; 
  set HIGHLIGHTCOLOR black; 
} \
elseif { [Default_Color_Scheme_cmd] == 2 } \
{
  set BACKGROUNDCOLOR grey; 
  set FOREGROUNDCOLOR black; 
  set HIGHLIGHTCOLOR black; 
} \
elseif { [Default_Color_Scheme_cmd] == 3 } \
{
  set BACKGROUNDCOLOR black; 
  set FOREGROUNDCOLOR green; 
  set HIGHLIGHTCOLOR white; 
} \
elseif { [Default_Color_Scheme_cmd] == 4 } \
{
  set BACKGROUNDCOLOR white; 
  set FOREGROUNDCOLOR black; 
  set HIGHLIGHTCOLOR white; 
} \
elseif { [Default_Color_Scheme_cmd] == 5 } \
{
  set BACKGROUNDCOLOR DodgerBlue; 
  set FOREGROUNDCOLOR black; 
  set HIGHLIGHTCOLOR black; 
} \
elseif { [Default_Color_Scheme_cmd] == 6 } \
{
  set BACKGROUNDCOLOR black; 
  set FOREGROUNDCOLOR white; 
  set HIGHLIGHTCOLOR black; 
} \
elseif { [Default_Color_Scheme_cmd] == 7 } \
{
  set BACKGROUNDCOLOR white; 
  set FOREGROUNDCOLOR white; 
  set HIGHLIGHTCOLOR black; 
} \
#################################
configureColors

.debugOutput insert end "Debug Messages:"
.debugOutput insert end \n
.debugOutput insert end "==============="
.debugOutput insert end \n
.debugOutput yview end

Update_Screen


