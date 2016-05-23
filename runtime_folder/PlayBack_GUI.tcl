###################
### Big Red GUI ###
###################

##############
set PLAYBACK_CONTROLS { \
                        "Start Playback                (return)"   \
                        "Stop Playback                 (spacebar)" \
                        "Rest to frame 0               (r)"        \
                        "Increase Speed                (+)"        \
                        "Decrease Speed                (-)"        \
                        "Toggle Displaying Orientation (c)"        \
                        "Toggle auto-loop              (l)"        \
                        "Toggle Team #                 (t)"        \
						"Toggle Data Source            (v)" \
					  }
###############
set PLAYBACK_CONTROLS_COMMANDS { \
                                  startPlayBack            \
                                  stopPlayBack             \
                                  resetToFrameZero         \
                                  increaseSpeed            \
                                  decreaseSpeed            \
                                  toggleDrawOurOrientation \
                                  toggleAutoLoop           \
                                  toggleTeam_cmd           \
								  toggleDataSource         \
								}
###############
set EDITING_TOOLS { \
                        "Show Editing Tools      (e)" \
                        "Remove Editing Tools    (d)" \
                        "Append playback         (a)" \
                  }
##############
set EDITING_TOOLS_COMMANDS { \
                             showEditTools   \
                             deleteEditTools \
                             doappend        \
                           }
###############

set FORMATION_START 10
set SPECIALTY_START 15

set toolsshown 0
set useRawData 0
set visionSource "Predicted Filtered"

set xxx 0
set yyy 0

set CANVAS_WIDTH                 500
set CANVAS_HEIGHT                635
set FIELD_X [expr $CANVAS_WIDTH / 2]
set FIELD_Y [expr $CANVAS_HEIGHT / 2]
set FIELD_FACTOR 2
set METER_CONVERSION 100
set NO_OF_ROBOTS 5
set REFRESH_DELAY 5
set OUR_ROBOT_RADIUS 9.0
set THEIR_ROBOT_RADIUS 9.0

set aiStatus ""
set wirelessStatus ""
set visionStatus ""
set sideValue ""
set teamNumber 0
set frameNumber 0
set playMode 0 
set maxFrame 1
set demoMode 0

#if a robot sets it's pass value, then display it
set automaticDisplayPassDest 1

# Initially File is not loaded
set fileLoaded 1
# Initially display only every 5-th frame
set frameOffset 2

# GUI_mode = 0 for AI mode
# GUI_mode = 1 for Playback mode
set GUI_mode 0

for {set i 0} { $i <= 4} {incr i} {
    set ourXPos$i ""
    set ourYPos$i ""
    set ourAngPos$i ""
    set ourXCommand$i ""
    set ourYCommand$i ""
    set ourAngCommand$i ""
    set dribber$i ""
    set ourKicker$i ""
    set ourRobotName$i ""
    set ourRobotFound$i ""
    set ourMessage$i ""
    set ourRobotDestX$i ""
    set ourRobotDestY$i ""
    set ourRobotPassDestX$i ""
    set ourRobotPassDestY$i ""
    set ourRobotPassValue$i ""
    set displayRobotDestination$i 1
    set displayRobotPassDestination$i ""

    set theirXPos$i ""
    set theirYPos$i ""
    set theirXVel$i ""
    set theirYVel$i ""
    set theirAngPos$i ""
    set theirRobotFound$i ""
}
set ballXPos ""
set ballYPos ""
set ballXVel ""
set ballYVel ""
set ballFound ""

set tempLeftCrop -32000
set tempRightCrop -32000

set drawOurOrientation 1

bind . <minus>    { decreaseSpeed }
bind . <equal>    { increaseSpeed }
bind . <plus>     { increaseSpeed }
bind . <r>        { resetToFrameZero }
bind . <Return>   { startPlayBack }
bind . <space>    { stopPlayBack }
bind . <t>        { toggleTeam_cmd  }
bind . <e>        { showEditTools }
bind . <d>        { deleteEditTools }
bind . <a>        { doappend }
bind . <c>        { toggleDrawOurOrientation }
bind . <l>        { toggleAutoLoop }
bind . <Left>     { rewind }
bind . <Right>    { fastforward }
bind . <v>        { toggleDataSource }
############################################
####### FUNCTION DECLARATIONS BEGINS #######
############################################
###
proc toggleDataSource {} \
{
  global useRawData
  global visionSource

  if { $useRawData == 0 } \
  {
    set useRawData 1
    set visionSource "Raw Vision"
  } \
  else \
  {
    set useRawData 0
    set visionSource "Predicted Filtered"
  }
}
###
proc rewind {} \
{
  global fileLoaded
  global frameNumber
  global frameOffset

  if { $fileLoaded == 1 } \
  {

    set frameNumber [expr $frameNumber - $frameOffset]

    if { $frameNumber < 0 } \
    {

      set frameNumber 0

    }

  }

}

###

proc fastforward {} \
{
  global fileLoaded
  global frameNumber
  global frameOffset
  global maxFrame

  if { $fileLoaded == 1 } \
  {
    set frameNumber [expr $frameNumber + $frameOffset]
    if { $frameNumber > $maxFrame } \
    {
      set frameNumber $maxFrame
    }
  }
}

###

proc toggleAutoLoop {} \
{
  global demoMode
  if { $demoMode == 1 } \
  {
     set demoMode 0
  } \
  else \
  {
     set demoMode 1
  }
}

###
proc toggleDrawOurOrientation {} \
{
  global drawOurOrientation
  if { $drawOurOrientation == 1 } \
  {
    set drawOurOrientation 0
  } \
  else \
  {
    set drawOurOrientation 1
  }
}

###
proc resetToFrameZero {} \
{ 
   global frameNumber
   global toolsshown
   set frameNumber 0; 
   SetFrame_cmd 0;
   if { $toolsshown == 1 } \
   {
      set x [expr [expr [expr $frameNumber + 0.01] / [expr [NumberOfFrames_cmd] + 0.01]] * 333]
      .playbackEdit.editMap delete curFrame
      .playbackEdit.editMap create rectangle [expr $x + 3] 0 [expr $x + 7] 14 -width 0 -tags curFrame -fill white
    }
}
###
proc startPlayBack {} \
{
  global playMode
  set playMode 1
}
###
proc stopPlayBack {} \
{
  global playMode
  set playMode 0
}
###

####### MENU COMMANDS ##################
proc Update_Screen {} \
{
  global NO_OF_ROBOTS
  for {set i 0} { $i <= 4} {incr i} \
  {
    global ourXPos$i
    global ourYPos$i
    global ourAngPos$i
    global ourXCommand$i
    global ourYCommand$i
    global ourAngCommand$i
    global ourDribbler$i
    global ourKicker$i
    global ourRobotFound$i
    global ourMessage$i
    global ourRobotRole$i
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
  global ballXPos
  global ballYPos
  global ballXVel
  global ballYVel
  global ballFound
  global METER_CONVERSION
  global sideValue
  global teamNumber
  global GUI_mode
  global frameNumber
  global playMode
  global fileLoaded
  global maxFrame
  global frameOffset
  global demoMode
  global useRawData
  # update all robots
  for {set i 0} { $i < $NO_OF_ROBOTS} {incr i} \
  {
    if { $useRawData == 1 } \
	{
      set ourXPos$i [GetOurRobotRawX_cmd $i]
      set ourYPos$i [GetOurRobotRawY_cmd $i]
      set ourAngPos$i [GetOurRobotRawRotation_cmd $i]
	} \
	else \
	{
      set ourXPos$i [GetOurRobotX_cmd $i]
      set ourYPos$i [GetOurRobotY_cmd $i]
      set ourAngPos$i [GetOurRobotRotation_cmd $i]
	}
    
    set ourXCommand$i [GetOurRobotSentXVel_cmd $i]
    set ourYCommand$i [GetOurRobotSentYVel_cmd $i]
    set ourAngCommand$i [GetOurRobotSentRotation_cmd $i]
    set ourMessage$i [GetRobotMessage_cmd $i]
    set ourRobotRole$i [GetRobotPosition_cmd $i]
    set ourRobotDestX$i [GetOurRobotDestinationX_cmd $i]
    set ourRobotDestY$i [GetOurRobotDestinationY_cmd $i]
    set ourRobotPassDestX$i [GetOurRobotPassDestinationX_cmd $i]
    set ourRobotPassDestY$i [GetOurRobotPassDestinationY_cmd $i]
    set ourRobotPassValue$i [GetOurRobotPassValue_cmd $i]

    if { $useRawData == 1 } \
	{
      set theirXPos$i [GetTheirRobotRawX_cmd $i]
      set theirYPos$i [GetTheirRobotRawY_cmd $i]
	} \
	else \
	{
      set theirXPos$i [GetTheirRobotX_cmd $i]
      set theirYPos$i [GetTheirRobotY_cmd $i]
	}

    set theirXVel$i [GetTheirRobotXVel_cmd $i]
    set theirYVel$i [GetTheirRobotYVel_cmd $i]
    set theirAngPos$i 0
    set ourDribbler$i [booleanText [GetRobotDribble_cmd $i]]
    set ourKicker$i [booleanText [GetRobotKick_cmd $i]]

    set ox ourXPos
    if { [expr $$ox$i] != -32000 } \
	{
      .bigRedFrame.robot$i.titleFrame.l2 configure -bg grey
      .bigRedFrame.robot$i.titleFrame.l4 configure -bg grey
    } \
    else \
    {
      .bigRedFrame.robot$i.titleFrame.l2 configure -bg [booleanColor 0]
      .bigRedFrame.robot$i.titleFrame.l4 configure -bg [booleanColor 0]
      set ourMessage$i "Not Found"
    }

    set tx theirXPos
    if { [expr $$tx$i] != -32000 } \
    {
      .adversaryFrame.robot$i.l2 configure -bg [booleanColor 0]
      .adversaryFrame.robot$i.l4 configure -bg [booleanColor 0]
    } \
    else \
    {
      .adversaryFrame.robot$i.l2 configure -bg grey 
      .adversaryFrame.robot$i.l4 configure -bg grey 
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
  }

  .cc delete OUR_ROBOT0
  drawOurRobot [expr $METER_CONVERSION *$ourXPos0] [expr $METER_CONVERSION*$ourYPos0] $ourAngPos0 [expr $METER_CONVERSION *$ourRobotDestX0] [expr $METER_CONVERSION *$ourRobotDestY0] [expr $METER_CONVERSION *$ourRobotPassDestX0] [expr $METER_CONVERSION *$ourRobotPassDestY0] OUR_ROBOT0 0 $displayRobotDestination0 $displayRobotPassDestination0 $ourRobotPassValue0
  .cc delete OUR_ROBOT1
  drawOurRobot [expr $METER_CONVERSION*$ourXPos1] [expr $METER_CONVERSION*$ourYPos1] $ourAngPos1 [expr $METER_CONVERSION *$ourRobotDestX1] [expr $METER_CONVERSION *$ourRobotDestY1] [expr $METER_CONVERSION *$ourRobotPassDestX1] [expr $METER_CONVERSION *$ourRobotPassDestY1] OUR_ROBOT1 1 $displayRobotDestination1 $displayRobotPassDestination1 $ourRobotPassValue1
  .cc delete OUR_ROBOT2
  drawOurRobot [expr $METER_CONVERSION*$ourXPos2] [expr $METER_CONVERSION*$ourYPos2] $ourAngPos2 [expr $METER_CONVERSION *$ourRobotDestX2] [expr $METER_CONVERSION *$ourRobotDestY2] [expr $METER_CONVERSION *$ourRobotPassDestX2] [expr $METER_CONVERSION *$ourRobotPassDestY2] OUR_ROBOT2 2 $displayRobotDestination2 $displayRobotPassDestination2 $ourRobotPassValue2
  .cc delete OUR_ROBOT3
  drawOurRobot [expr $METER_CONVERSION*$ourXPos3] [expr $METER_CONVERSION*$ourYPos3] $ourAngPos3 [expr $METER_CONVERSION *$ourRobotDestX3] [expr $METER_CONVERSION *$ourRobotDestY3] [expr $METER_CONVERSION *$ourRobotPassDestX3] [expr $METER_CONVERSION *$ourRobotPassDestY3] OUR_ROBOT3 3 $displayRobotDestination3 $displayRobotPassDestination3 $ourRobotPassValue3
  .cc delete OUR_ROBOT4
  drawOurRobot [expr $METER_CONVERSION*$ourXPos4] [expr $METER_CONVERSION*$ourYPos4] [expr $ourAngPos4] [expr $METER_CONVERSION *$ourRobotDestX4] [expr $METER_CONVERSION *$ourRobotDestY4] [expr $METER_CONVERSION *$ourRobotPassDestX4] [expr $METER_CONVERSION *$ourRobotPassDestY4] OUR_ROBOT4 4 $displayRobotDestination4 $displayRobotPassDestination4 $ourRobotPassValue4
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
  if { $useRawData == 1 } \
  {
    set ballXPos [GetBallRawX_cmd]
    set ballYPos [GetBallRawY_cmd]
  } \
  else \
  {
    set ballXPos [GetBallX_cmd]
    set ballYPos [GetBallY_cmd]
  }

  set ballXVel [GetBallXVel_cmd]
  set ballYVel [GetBallYVel_cmd]
  set ballFound [GetRoboCupBallFound_cmd]

  if { $ballFound == 1 } \
  {
    .ballFrame.l2 configure -bg grey
    .ballFrame.l4 configure -bg grey
    .ballFrame.l6 configure -bg grey
	.ballFrame.l8 configure -bg grey
  } \
  else \
  {
    .ballFrame.l2 configure -bg [booleanColor 0]
    .ballFrame.l4 configure -bg [booleanColor 0]
    .ballFrame.l6 configure -bg [booleanColor 0]
    .ballFrame.l8 configure -bg [booleanColor 0]
  }

  .cc delete BALL
  drawOneBall [expr $METER_CONVERSION*$ballXPos] [expr $METER_CONVERSION*$ballYPos]

  # update system status
  set aiStatus ""
  set wirelessStatus ""
  set visionStatus ""
  set sideValue ""
  set teamNumber ""
  set playName [GetCurrentPlayName_cmd]

  # Play back specific features
  SetFrame_cmd $frameNumber
  set frameNumber [GetCurrentFrame_cmd]
  set maxFrame [expr [NumberOfFrames_cmd]-1]
  if {$frameNumber >= $maxFrame} \
  {
    if {$demoMode == 1} \
    {
      set frameNumber 0
      SetFrame_cmd $frameNumber
    } \
    else \
    {          
      set demoMode 0
    }
  }

  global toolsshown
  if { $toolsshown == 1 } \
  {
     set x [expr [expr [expr $frameNumber + 0.01] / [expr [NumberOfFrames_cmd] + 0.01]] * 333]
    .playbackEdit.editMap delete curFrame
    .playbackEdit.editMap create rectangle [expr $x + 3] 0 [expr $x + 7] 14 -width 0 -tags curFrame -fill white
  }

  # Actually playing the playback
  if {$fileLoaded == 1} \
  {
    #update more frequently
    .playbackFrame.frameSelect configure -to $maxFrame
  } \
  else \
  {
    set playMode 0
  }

  if {$playMode == 1} \
  {
    set frameNumber [expr $frameNumber + $frameOffset]
    global toolsshown
    if { $toolsshown == 1 } \
    {
       set x [expr [expr [expr $frameNumber + 0.01] / [expr [NumberOfFrames_cmd] + 0.01]] * 333]
      .playbackEdit.editMap delete curFrame
      .playbackEdit.editMap create rectangle [expr $x + 3] 0 [expr $x + 7] 14 -width 0 -tags curFrame -fill white
    }
    after 1 Update_Screen
  } \
  else \
  {
    after 500 Update_Screen 
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
  if {$a_integer == 1} \
  {
    set bool green
  } \
  else \
  {
    set bool red
  }
  set bool
}

proc increaseSpeed {} \
{
  global frameOffset
  incr frameOffset
}

proc decreaseSpeed {} \
{
  global frameOffset
  if {$frameOffset > 1} \
  {
    set  frameOffset [expr $frameOffset - 1]
  } \
  else \
  {
    set frameOffset 1 
  }
}

proc setCropLeft {} \
{
  if { [NumberOfFrames_cmd] != 0 } \
  {
    global frameNumber
    global tempLeftCrop
    global tempRightCrop
    set tempLeftCrop $frameNumber
    if { $tempRightCrop < $frameNumber && $tempRightCrop != -32000 } \
    {
       set tempRightCrop $frameNumber
       .playbackEdit.editMap delete rightCrop
       SetCropRight_cmd $frameNumber
    }

    SetCropLeft_cmd $frameNumber
    set x [expr [expr [expr $frameNumber + 0.01] / [expr [NumberOfFrames_cmd] + 0.01]] * 333]
    .playbackEdit.editMap delete leftCrop
    .playbackEdit.editMap create rectangle [expr $x + 3] 0 [expr $x + 7] 14 -width 0 -tags leftCrop -fill red
    .playbackEdit.editMap delete cropSelection

    if { $tempLeftCrop != $tempRightCrop && $tempLeftCrop != -32000 && $tempRightCrop != -32000 } \
    {
      set x1 [expr [expr [expr $tempLeftCrop + 0.01] / [expr [NumberOfFrames_cmd] + 0.01]] * 333]
      set x2 [expr [expr [expr $tempRightCrop + 0.01] / [expr [NumberOfFrames_cmd] + 0.01]] * 333]
      .playbackEdit.editMap create rectangle [expr $x1 + 5] 0 [expr $x2 + 5] 14 -width 0 -tags cropSelection -fill yellow
    }
  }
}

proc setCropRight {} \
{
  if { [NumberOfFrames_cmd] != 0 } \
  {
    global frameNumber
    global tempLeftCrop
    global tempRightCrop
    set tempRightCrop $frameNumber
    if { $tempLeftCrop > $frameNumber && $tempLeftCrop != -32000 } \
    {
       set tempLeftCrop $frameNumber
       .playbackEdit.editMap delete leftCrop
       SetCropLeft_cmd $frameNumber
    }

    SetCropRight_cmd $frameNumber
    set x [expr [expr [expr $frameNumber + 0.01] / [expr [NumberOfFrames_cmd] + 0.01]] * 333]
    .playbackEdit.editMap delete rightCrop
    .playbackEdit.editMap create rectangle [expr $x + 3] 0 [expr $x + 7] 14 -width 0 -tags rightCrop -fill blue
    .playbackEdit.editMap delete cropSelection

    if { $tempLeftCrop != $tempRightCrop && $tempLeftCrop != -32000 && $tempRightCrop != -32000 } \
    {
       set x1 [expr [expr [expr $tempLeftCrop + 0.01] / [expr [NumberOfFrames_cmd] + 0.01]] * 333]
       set x2 [expr [expr [expr $tempRightCrop + 0.01] / [expr [NumberOfFrames_cmd] + 0.01]] * 333]
      .playbackEdit.editMap create rectangle [expr $x1 + 5] 0 [expr $x2 + 5] 14 -width 0 -tags cropSelection -fill yellow
    }
  }
}

proc setCrop {} \
{
  SetCrop_cmd
  global tempLeftCrop
  global tempRightCrop
  .playbackEdit.editMap delete leftCrop
  .playbackEdit.editMap delete rightCrop
  .playbackEdit.editMap delete cropSelection

   if { $tempLeftCrop != $tempRightCrop && $tempLeftCrop != -32000 && $tempRightCrop != -32000 } \
   {
     set x1 [expr [expr [expr $tempLeftCrop + 0.01] / [expr [NumberOfFrames_cmd] + 0.01]] * 333]
     set x2 [expr [expr [expr $tempRightCrop + 0.01] / [expr [NumberOfFrames_cmd] + 0.01]] * 333]
     .playbackEdit.editMap create rectangle [expr $x1 + 5] 0 [expr $x2 + 5] 14 -width 0 -tags cropSelectionFinal -fill red
   }

   set tempLeftCrop -32000
   set tempRightCrop -32000
}

proc delCrop {} \
{
   global leftCrop
   global rightCrop
   set leftCrop -32000
   set rightCrop -32000
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
  expr [expr cos($degAngle)]*($x_coord-$x_coord_center) - [expr sin($degAngle)]*($y_coord-$y_coord_center) - $x_coord_center
}

proc rotate_y {x_coord y_coord degAngle {x_coord_center 0} {y_coord_center 0}} \
{
  expr [expr sin($degAngle)]*($x_coord-$x_coord_center) + [expr cos($degAngle)]*($y_coord-$y_coord_center) - $y_coord_center
}

# This procedure actually creates the picture elements in the canvas
# depending on the value of the drawtype
proc dodraw {drawtype tagName wid x1 y1 x2 y2 color} \
{

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
}

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
  set GOALIE_BOX_DEPTH               [expr 100 * [GOALIE_BOX_DEPTH_cmd]]

  set OUR_DEFENCE_LINE               [expr $OUR_GOAL_LINE + $GOALIE_BOX_DEPTH]
  set THEIR_DEFENCE_LINE             [expr $THEIR_GOAL_LINE - $GOALIE_BOX_DEPTH]
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

  set displayFieldLines 1
  if { $displayFieldLines == 1 } \
  {
  #############################
  #draw center line and half circle in middle
  #######
  dodraw "line" "HALF_LINE"         1       $HALF_LINE     $LEFT_SIDE_LINE      $HALF_LINE   $RIGHT_SIDE_LINE black
  dodraw "oval" "CENTER_CIRCLE"     1     [expr -22.5 + $HALF_LINE]   [expr -22.5 + $SPLIT_LINE]        [expr 22.5 + $HALF_LINE]    [expr 22.5 + $SPLIT_LINE] black
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
  }

  set displayZoneSectorLines 1
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
#####
# This procedure draws a robot at the specified location
# with the specified orientation
# "orientation" is the counter-clockwise angle between the positive x axis
proc drawOurRobot { \
                    x_coord \
                    y_coord \
                    orientation \
                    destX \
                    destY \
                    passdestX \
                    passdestY \
                    tagName \
                    robotId \
                    displayDest \
                    displayPassDest \
                    robotsPassValue \
                  } \
{
    for {set i 0} { $i < 5} {incr i} \
    {
      global ourRobotPassValue$i
    }
    
    global automaticDisplayPassDest
    global FIELD_X
    global FIELD_Y
    global FIELD_FACTOR
    global OUR_ROBOT_RADIUS

    global drawOurOrientation
    global aiStatus

    set x_coord    [expr -1 * $x_coord]
    set y_coord    [expr -1 * $y_coord]
    set destX      [expr -1 * $destX]
    set destY      [expr -1 * $destY]
    set passdestX  [expr -1 * $passdestX]
    set passdestY  [expr -1 * $passdestY]

    set OFFSET [expr 4* $FIELD_FACTOR]
    set x_max [expr $x_coord + $OFFSET]
    set y_max [expr $y_coord + $OFFSET]
    set x_min [expr $x_coord - $OFFSET]
    set y_min [expr $y_coord - $OFFSET]

    if { $x_coord != 32000 } \
    {   
    # display the robotId
    .cc create text [expr $FIELD_FACTOR * $y_coord + $FIELD_X] \
                    [expr $FIELD_FACTOR * $x_coord + $FIELD_Y] \
                    -text $robotId \
                    -tags $tagName \
                    -font {Courier 20 bold} \
                    -fill red
    }

    # display the robot's destination
    if { $displayDest == 1 && \
         [GetOurRobotFound_cmd $robotId] == 1 } \
    {
    .cc create text [expr $FIELD_FACTOR * $destY + $FIELD_X] \
                    [expr $FIELD_FACTOR * $destX + $FIELD_Y] \
                    -text $robotId \
                    -tags $tagName \
                    -font {Courier 20 bold} \
                    -fill yellow

    set destinationRotation [GetOurRobotDestinationRot_cmd $robotId]
    if { $destinationRotation != -32000 &&
         $destinationRotation != -64000 &&
         $destinationRotation != 32000 &&
         $destinationRotation != 64000 } \
    {


    #destination rotation arrow
    set color yellow
    dodraw "line" \
         $tagName \
         3 \
         [rotate_x [expr $x_coord + $OFFSET] $y_coord [GetOurRobotDestinationRot_cmd $robotId] $x_coord $y_coord] \
         [rotate_y [expr $x_coord + $OFFSET] $y_coord [GetOurRobotDestinationRot_cmd $robotId] $x_coord $y_coord] \
         [rotate_x [expr $x_coord + $OFFSET + 16] $y_coord [GetOurRobotDestinationRot_cmd $robotId] $x_coord $y_coord] \
         [rotate_y [expr $x_coord + $OFFSET + 16] $y_coord [GetOurRobotDestinationRot_cmd $robotId] $x_coord $y_coord] \
         $color

    dodraw "dribbler" \
         $tagName \
         2 \
         [rotate_x [expr $x_coord + $OFFSET + 7] [expr $y_coord - 7] [GetOurRobotDestinationRot_cmd $robotId] $x_coord $y_coord] \
         [rotate_y [expr $x_coord + $OFFSET + 7] [expr $y_coord - 7] [GetOurRobotDestinationRot_cmd $robotId] $x_coord $y_coord] \
         [rotate_x [expr $x_coord + $OFFSET + 16] $y_coord [GetOurRobotDestinationRot_cmd $robotId] $x_coord $y_coord] \
         [rotate_y [expr $x_coord + $OFFSET + 16] $y_coord [GetOurRobotDestinationRot_cmd $robotId] $x_coord $y_coord] \
         yellow

    dodraw "dribbler" \
         $tagName \
         2 \
         [rotate_x [expr $x_coord + $OFFSET + 7] [expr $y_coord + 7] [GetOurRobotDestinationRot_cmd $robotId] $x_coord $y_coord] \
         [rotate_y [expr $x_coord + $OFFSET + 7] [expr $y_coord + 7] [GetOurRobotDestinationRot_cmd $robotId] $x_coord $y_coord] \
         [rotate_x [expr $x_coord + $OFFSET + 16] $y_coord [GetOurRobotDestinationRot_cmd $robotId] $x_coord $y_coord] \
         [rotate_y [expr $x_coord + $OFFSET + 16] $y_coord [GetOurRobotDestinationRot_cmd $robotId] $x_coord $y_coord] \
         yellow

     }
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
  if { [GetFriendlyRobotHasPossesion_cmd $robotId] == 1 } \
  {
    set color yellow 
  } \
  else \
  {
    set color black
  }

  if { $drawOurOrientation == 1 } \
  {

    if { $x_coord != -32000 } \
    {
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
  } \
  else \
  {
    dodraw "oval" \
         $tagName \
         2 \
         [expr -1*$x_coord+$OUR_ROBOT_RADIUS] \
         [expr -1*$y_coord+$OUR_ROBOT_RADIUS] \
         [expr -1*$x_coord-$OUR_ROBOT_RADIUS] \
         [expr -1*$y_coord-$OUR_ROBOT_RADIUS] \
         black
  }
}
############
# This procedure draws opponent robots
############
proc drawTheirRobot {x_coord y_coord tagName robotId} \
{
    global FIELD_FACTOR
    global FIELD_X
    global FIELD_Y
    global THEIR_ROBOT_RADIUS

    #only draw robot if found
    if { $x_coord != -32000 } \
    {
      # display the robotId
      .cc create text [expr $FIELD_FACTOR * -1 *$y_coord + $FIELD_X] \
                      [expr $FIELD_FACTOR * -1 * $x_coord + $FIELD_Y] \
                      -text [expr $robotId-5] \
                      -tags $tagName \
                      -font {Courier 20 bold} \
                      -fill blue
      # draw five lines to represent one robot
      if { [GetOpponentRobotHasPossesion_cmd $robotId] == 1 } \
      {
        set color yellow 
      } \
      else \
      {
        set color black
      }

      # draw actual robot
      dodraw "oval" \
             $tagName \
             2 \
             [expr $x_coord+$THEIR_ROBOT_RADIUS] \
             [expr $y_coord+$THEIR_ROBOT_RADIUS] \
             [expr $x_coord-$THEIR_ROBOT_RADIUS] \
             [expr $y_coord-$THEIR_ROBOT_RADIUS] \
             $color
     }
}
############
# This procedures draws a ball given the location
proc drawOneBall {x_coord y_coord} \
{
  if { [GetRoboCupBallFound_cmd] == 1 } \
  {
    dodraw "ball" BALL 3 \
           [expr $x_coord+1] \
           [expr $y_coord+1] \
           [expr $x_coord-1] \
           [expr $y_coord-1] \
           red
  }
}

#######################################
proc disply {{x 0} {y 0} } \
{
  global FIELD_X
  global FIELD_Y
  global FIELD_FACTOR
  global xxx
  global yyy

  # X-axis in the canvas corresponds to the actual Y-axis
  # Y-axis in the canvas corresponds to the actual X-axis
  set xxx [expr -1 * [expr $y - $FIELD_Y] / $FIELD_FACTOR]
  set yyy [expr -1 * [expr $x - $FIELD_X] / $FIELD_FACTOR]
}
########################################

########################################
###### WIDGET DECLARATION BEGINS #######
########################################

##### TOP LEVEL WINDOW #################
# toplevel window declaration
wm title . "Big Red - Taking AI to new levels..." 
# specify window size
wm geometry . 1020x760
wm maxsize  . 1020 760
wm minsize  . 1020 760

focus .
# toplevel window declaration ends
#############################

###### MENU ############
# menu button begins
# create menu with file option
frame .mbar ;# frame for menubar
menubutton .mbar.file -text File -underline 0 -menu .mbar.file.menu
menu .mbar.file.menu
.mbar.file.menu add command -label Load -command {doload}

menubutton .mbar.playbackControls \
           -text "Playback Controls" \
           -underline 0 \
           -menu .mbar.playbackControls.menu

menu .mbar.playbackControls.menu


foreach {x} $PLAYBACK_CONTROLS \
        {y} $PLAYBACK_CONTROLS_COMMANDS \
{

  .mbar.playbackControls.menu add command \
                              -label $x \
                              -command $y 

}



menubutton .mbar.editingTools \
           -text "Editing Tools" \
           -underline 0 \
           -menu .mbar.editingTools.menu



menu .mbar.editingTools.menu



foreach {x} $EDITING_TOOLS {y} $EDITING_TOOLS_COMMANDS \
{

  .mbar.editingTools.menu add command -label $x -command $y

}



tk_menuBar .mbar .mbar.file

focus .mbar

pack .mbar -side top -fill x

pack .mbar.file -side left

pack .mbar.playbackControls -side left

pack .mbar.editingTools -side left



# menu onclick ends

#############################



#############################

####### CANVAS ############

canvas .cc -width $CANVAS_WIDTH -height $CANVAS_HEIGHT -relief ridge -bd 2
#bind .cc <Button-1> {disply %x %y}
drawField
drawOurRobot -10 +10 0 -10 +10 0 0 OUR_ROBOT0 0 0 0 0
drawOurRobot -20 -20 0 -20 -20 0 0 OUR_ROBOT1 1 0 0 0
drawOurRobot  10 10 0 10 10 0 0 OUR_ROBOT2 2 10 0 0
drawOurRobot  20 -20 0 20 -20 0 0 OUR_ROBOT3 3 0 0 0
drawOurRobot -20  20 0 -20  20 0 0 OUR_ROBOT4 4 0 0 0
drawTheirRobot -10 +10 THEIR_ROBOT0 5 
drawTheirRobot -20 -20 THEIR_ROBOT1 6 
drawTheirRobot  10 10 THEIR_ROBOT2 7 
drawTheirRobot  20 -20 THEIR_ROBOT3 8 
drawTheirRobot -20  20 THEIR_ROBOT4 9
# canvas ends

#############################

##########################################
####### Big Red Robot Display ############
##########################################
frame .bigRedFrame -relief ridge -bd 0 -width $CANVAS_WIDTH
global NO_OF_ROBOTS
for {set i 0} {$i < $NO_OF_ROBOTS} {incr i} \
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

for {set i 0} {$i < $NO_OF_ROBOTS} {incr i} \
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
frame .statusFrame -relief ridge -bd 2 -width $CANVAS_WIDTH
frame .statusFrame.top
label .statusFrame.top.l1 -text "Playback Speed = " -height 0
label .statusFrame.top.l2 -textvariable frameOffset -width 3 -height 0
label .statusFrame.top.l3 -text "Vision Source = " -height 0
label .statusFrame.top.l4 -textvariable visionSource -width 15 -height 0

pack .statusFrame.top.l1 \
     .statusFrame.top.l2 \
     .statusFrame.top.l3 \
     .statusFrame.top.l4 \
	  -in .statusFrame.top \
	  -side left

frame .statusFrame.lower_bottom
label .statusFrame.lower_bottom.l1 -text "The Current Play = " -width 20 -height 0
label .statusFrame.lower_bottom.l2 -textvariable playName -width 20 -height 0
label .statusFrame.lower_bottom.l3 -text "Autoloop: " -width 10 -height 0
checkbutton .statusFrame.lower_bottom.l4 -variable demoMode -height 0
pack .statusFrame.lower_bottom.l1 .statusFrame.lower_bottom.l2  .statusFrame.lower_bottom.l3 .statusFrame.lower_bottom.l4 -in .statusFrame.lower_bottom -side left

pack .statusFrame.top .statusFrame.lower_bottom -side top
####################################

####### PLAYBACK FRAME ##########
set minFrame 0
set maxFrame 1
frame .playbackFrame -relief ridge -bd 2 -width $CANVAS_WIDTH
button .playbackFrame.playButton -text ">" -width 3 -command {set playMode 1}
button .playbackFrame.pauseButton -text "| |" -width 3 -command {set playMode 0}
button .playbackFrame.reverseButton -text "<<" -width 3 -command {BackwardFrame_cmd; set frameNumber [GetCurrentFrame_cmd]}
button .playbackFrame.forwardButton -text ">>" -width 3 -command {ForwardFrame_cmd; set frameNumber [GetCurrentFrame_cmd]}
scale  .playbackFrame.frameSelect \
      -orient horizontal \
      -digit 0 \
      -from $minFrame \
      -to $maxFrame \
      -showvalue 0 \
      -length 8c \
      -variable frameNumber

entry  .playbackFrame.frameSet \
       -width 8 \
       -textvariable frameNumber
pack   .playbackFrame.playButton \
       .playbackFrame.pauseButton \
       .playbackFrame.reverseButton \
       .playbackFrame.forwardButton \
       .playbackFrame.frameSelect \
       .playbackFrame.frameSet \
       -in .playbackFrame \
       -side left

#################################
proc deleteEditTools {} \
{
  global toolsshown
  destroy .playbackEdit
  set toolsshown 0
}
#################################

#################################
proc showEditTools {} \
{
global toolsshown
global CANVAS_WIDTH
set toolsshown 1
frame .playbackEdit -relief ridge -bd 2 -width $CANVAS_WIDTH
button .playbackEdit.setLeft -text "L" -width 3 -command {setCropLeft;}
button .playbackEdit.setRight -text "R" -width 3 -command {setCropRight;}
button .playbackEdit.setCut -text "Cut" -width 3 -command {setCrop;}
button .playbackEdit.setCancel -text "Save" -width 3 -command {dosave;}
canvas .playbackEdit.editMap -width 335 -height 10 -relief ridge -bd 2 -cursor hand2
.playbackEdit.editMap create rectangle 0 0 339 14 -width 0 -tags useAll -fill green
pack .playbackEdit.setLeft \
     .playbackEdit.setRight \
     .playbackEdit.setCut \
     .playbackEdit.setCancel \
     .playbackEdit.editMap \
     -in .playbackEdit \
     -side left
place .playbackEdit -x 10 -y 670
}
#################################

########################################
###### WIDGET PLACEMENT BEGIN #########
########################################
# POSITIONING MENU, LABELS AND CANVAS
place .cc -x 510 -y 26 -anchor nw
place .bigRedFrame -x 0 -y 26
place .ballFrame -x 70 -y 335
place .adversaryFrame -x 65 -y 380
place .statusFrame -x 80 -y 560 -anchor sw
place .playbackFrame -x 10 -y 635
########################################






######### LOAD FILE ####################
# Load the specified file
proc doload {} \
{
  global fileLoaded
  global frameNumber
  global SetFrame_cmd
  global playMode
  global toolsshown
  set frameNumber 0
  SetFrame_cmd 0
  set playMode 0 
  # get the filename specifed in the common dialog box
  set act [tk_getOpenFile]
  # if a filename is specified
  if {$act != ""} \
  {
    eraseField
    LoadFile_cmd $act
    set fileLoaded 1
    #set the window name to display the current log file name
    #split filename up by path components and get last one (filename)
    set simpleFileName [lindex [file split $act] end]
    wm title . "Big Red - $simpleFileName" 
    drawField
  }
  if {$toolsshown == 1 } \
  {
    deleteEditTools
    showEditTools
  }
}

#############################

######### APPEND FILE ####################

# Load the specified file

proc doappend {} {

  global fileLoaded

  global frameNumber

  global SetFrame_cmd

  global playMode

  global toolsshown



  set frameNumber 0

  SetFrame_cmd 0

  set playMode 0 



  # get the filename specifed in the common dialog box

  set act [tk_getOpenFile]

  # if a filename is specified

  if {$act != ""} {



    AppendFile_cmd $act

    set fileLoaded 1

  }



  if {$toolsshown == 1 } \
  {

  deleteEditTools

  showEditTools

  }

}

#############################







######### SAVE FILE ####################

# Save the specified file

proc dosave {} \
{

  # get the filename specifed in the common dialog box

  set act [tk_getSaveFile]

  # if a filename is specified

  if {$act != ""} \
  {

    SaveFile_cmd $act

  }

}

#############################





global toolsshown

if { $toolsshown == 1 } \
{

  set x [expr [expr [expr $frameNumber + 0.01] / [expr [NumberOfFrames_cmd] + 0.01]] * 333]

  .playbackEdit.editMap delete curFrame

  .playbackEdit.editMap create rectangle [expr $x + 3] 0 [expr $x + 7] 14 -width 0 -tags curFrame -fill white

}





Update_Screen

