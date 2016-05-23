#include "DribblePointSkill.h"
#include "RoboCup_modules/RoboCupStrategyModule.h"
#include "GUI/GUI.h"
#include "common/util/pair/pair.h"
#include "common/util/shoot/shoot.h"
#include "common/util/math/angle.h"
#include "common/util/intersect/intersect.h"
#include "Skillz/ManeuverSkill.h"

int DribblePointSkill::skillNum = -1;

//=====================================================
//Obstacle Stamp
static int Stamp[37][3] = 
{
  //(X, Y, Value)
  //The 2's
  {-1, 3, 2}, 
  {0, 3, 2},
  {1, 3, 2},
  {2, 2, 2},
  {3, 1, 2},
  {3, 0, 2},
  {3, -1, 2},
  {2, -2, 2},
  {1, -3, 2},
  {0, -3, 2},
  {-1, -3, 2},
  {-2, -2, 2},
  {-3, -1, 2},
  {-3, 0, 2},
  {-3, 1, 2},
  {-2, 2, 2},
  //The 5's
  {-1, 2, 5},
  {0, 2, 5},
  {1, 2, 5},
  {2, 1, 5},
  {2, 0, 5},
  {2, -1, 5},
  {1, -2, 5},
  {0, -2, 5},
  {-1, -2, 5},
  {-2, -1, 5},
  {-2, 0, 5},
  {-2, 1, 5},
  //The 7's
  {-1, 1, 7},
  {0, 1, 7},
  {1, 1, 7},
  {1, 0, 7},
  {1, -1, 7},
  {0, -1, 7},
  {-1, -1, 7},
  {-1, 0, 7},
  //The 9 (the center)
  {0,0, 9}
};
//=====================================================
///Constructor.  Calls the base class Skill constructor
DribblePointSkill::DribblePointSkill(SystemParameters *sp,
                                     VisionModule *vm, 
                                     RoboCupStrategyModule *strat,
                                     RobotIndex id, 
                                     SkillSet* set) : Skill(sp,vm,strat,id,set) 
{
  initialized = false;
  ID = id;
  friendlyIgnoreIDs = NULL;
  searchBox = NULL;
  
  //load parameters, also creates search box
  loadValues();
}
//=====================================================
///Distructor, distroys array of ignore id's
DribblePointSkill::~DribblePointSkill()
{
  delete[] friendlyIgnoreIDs;
  friendlyIgnoreIDs = NULL;
  
  delete[] searchBox;
  searchBox = NULL;
}  
//=====================================================
///Call this function to check the prerequisites of the skill.  This will return 
///a bool indicating whether or not the skill is ciable in the present situation.
bool DribblePointSkill::isValid() 
{ 
  return true;
}
//=====================================================
///Perform any initializations for the skill, such as reseting the timer.
void DribblePointSkill::initialize(Pair newGoalDestination, 
                                   RobotIndex* newFriendlyIgnoreIDs,
                                   int newNumIgnoreIDs)
{
  //------
  //set goal destination
  goalDestination.set(newGoalDestination);
  //------
  //reset timer
  timer->resetTimer();
  //------
  //skill has been initialized
  initialized = true;
  //------
  //set list of friendly robots to ignore
  if(newFriendlyIgnoreIDs == NULL)
    friendlyIgnoreIDs = NULL;
  else
  {
    if(friendlyIgnoreIDs != NULL)
    {
      delete[] friendlyIgnoreIDs;
      friendlyIgnoreIDs = NULL;
    }
    
    friendlyIgnoreIDs = new RobotIndex[newNumIgnoreIDs];
    for(int i=0; i<newNumIgnoreIDs; i++)
    {
      friendlyIgnoreIDs[i] = newFriendlyIgnoreIDs[i];
    }
  }
  
  numIgnoreIDs = newNumIgnoreIDs;

  //by default dribbling
  turning = false;
}
//=====================================================
///Execute the skill.  This is the main part of the skill, where you tell the
///robot how to perform the skill.
void DribblePointSkill::execute()
{      
  ///If not active, dont do anything!
  if(!initialized)
  {
    return;  
  }
  else
  {
    //--------------------------------------------------------
    //get the current location of the dribbling robot
    dribblingRobot.set(getLocation(robotID, (*currentVisionData), (*sp))); 

    Pair optimalPoint;
    optimalPoint.set(dribblePointBox());
    
    //if search box empty, dribble straight toward goal desitnation
    if(empty)
    {
      optimalPoint.set(goalDestination);
    }

    //--------------------------------------------------------
    //determine which mode to be in
    if(!turning)
    {
      if(ABS(angleDifference(
                              getRotation(robotID, 
                                          (*currentVisionData), 
                                          (*sp)
                                         ),
                              angleBetween(dribblingRobot, optimalPoint)
                            )) > MAX_TURN_ANGLE + MAX_TURN_ANGLE_THRESHOLD)
        turning = true;
    }
    else
    {
      if(ABS(angleDifference(
                              getRotation(robotID, 
                                          (*currentVisionData), 
                                          (*sp)
                                         ),
                              angleBetween(dribblingRobot, optimalPoint)
                            )) < MAX_TURN_ANGLE - MAX_TURN_ANGLE_THRESHOLD)
        turning = false;
    }
    //--------------------------------------------------------
    //sharp angle, maneuver around ball first to avoid losing posession
    if(turning)
    {
      ManeuverWithBallSkill* maneuverSkill = (ManeuverWithBallSkill*)(skillSet->getSkill(ManeuverWithBallSkill::skillNum));

      Pair lookToPoint;
      extendPoint(dribblingRobot, optimalPoint, 1.0f, lookToPoint);

      maneuverSkill->initialize(optimalPoint, lookToPoint);
      maneuverSkill->run();  

      strategy->getCurrentFrame()->setMessage(robotID, "Angle sharp, MANEUVER with ball.");  
    }
    //else go directly for point
    else
    {
      if(empty)
      {
        strategy->getCurrentFrame()->setMessage(robotID, "Dribling without SB");  
      }
      else
      {
        strategy->getCurrentFrame()->setMessage(robotID, "Dribling using SEARCH BOX");  
      }

      command->setPos(optimalPoint);
      command->setRotation(angleBetween(getLocation(robotID, *currentVisionData, *sp), 
                                        optimalPoint));     
      command->setDribble(DRIBBLE_DEFAULT);
      command->setVerticalDribble(V_DRIBBLE_DEFAULT);
      command->setControl(OMNI_FAST);

    }
    //--------------------------------------------------------  }
  }
}
//=====================================================
bool DribblePointSkill::evaluate() 
{
  return true;
}
//=====================================================
///For tweaking the skill.  You may want to change local parameters or behave 
///differently to adapt to any situation that is frustrating the skill
void DribblePointSkill::update() 
{
  return;
}
//=====================================================
bool DribblePointSkill::isFinished(float tolerance)
{
  return false;
}
//=====================================================
///Reads in the values of the parameters from the skill's parameter text file.
void DribblePointSkill::loadValues()
{
  ///Open the parameter file:
  ///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
  ifstream mainFile;
  mainFile.open("Params/Skill_Params/DribblePointSkillParams.txt", ios::nocreate);  
  ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");
  
  //Load parameter values.  See params.h and params.cpp for good examples.
  //################  ADD PARAMETERS BELOW ################
  
  //size,resolution, and forward offset when placed of search box
  READ_FLOAT(SEARCH_BOX_WIDTH);
  READ_FLOAT(SEARCH_BOX_HEIGHT);
  READ_FLOAT(SEARCH_BOX_RESOLUTION);
  READ_FLOAT(SEARCH_BOX_FORWARD_OFFSET);

  //include robot radius in forward offset
  SEARCH_BOX_FORWARD_OFFSET+=sp->general.PLAYER_RADIUS;

  READ_FLOAT(MAX_TURN_ANGLE);
  READ_FLOAT(MAX_TURN_ANGLE_THRESHOLD);
  //################  ADD PARAMETERS ABOVE ################
  mainFile.close();  
  
  //recreate search box
  createSearchBox();
  
}
//=====================================================
/**Given an array of friendly robots to ignore, fills array
with all obstacle positions.*/
void DribblePointSkill::createObstacleArray()
{
  //----
  //Fill array and return handle
  //----
  //our robots
  for(RobotIndex i=(RobotIndex)0; i<(RobotIndex)NUM_ROBOTS; i++)
  {
    //----
    //Check to see if this robot is ignored
    bool ignored = false;
    
    //we are not an obstacle to ourself
    if(i == robotID)
      ignored = true;
    
    for(int j=0; j<numIgnoreIDs; j++)
    {
      if(i == friendlyIgnoreIDs[j])
      {
        ignored = true;
        break;
      }
    }
    //----
    //If robot not ignored, get it's location and insert it into the array
    if(ignored)
      obstacles[i].set(-32000.0f, -32000.0f);
    else
      obstacles[i].set(getLocation(i, (*currentVisionData), (*sp)));
    //----
  }
  //----
  //opponent robots
  for(i=(RobotIndex)0; i<(RobotIndex)NUM_ROBOTS; i++)
  {
    obstacles[i+NUM_ROBOTS].set(getTheirRobotLocation(i, (*currentVisionData), (*sp)));
  }
  //----
}
//=====================================================
/**Given an array of obstacles, this robot location and the goal destination,
rotates all obstacles such that in the rotated space the vector from the
dribbling robot to the goal dest has a slope of 0.*/
void DribblePointSkill::rotateObstacleArray()
{
  //find the angle between dribbling robot and goal dest
  float angle = angleBetween(dribblingRobot, goalDestination);
  
  for(int i=0; i<2*NUM_ROBOTS; i++)
  {
    //if obstacle defined
    if(obstacles[i].getX() != -32000.0f)
    {
      rotateAboutPoint(obstacles[i],
        dribblingRobot,
        -angle,
        obstacles[i]);
    }
  }
}
//=====================================================
/**Based on placement of search box, determines beginning 
croppings such that no point will be chosen off the field.*/
void DribblePointSkill::determineSearchBounds()
{
    /*
    --------------------------------------------------
            G

     A      B      C
  
     D      E      F
     
            U
      
        U = dribblingRobot (us!)
        G = goal Destination
        DF = close edge line, close edge pair is E
        AC = far edge line, far edge pair is B
        A = far left corner
        C = far right corner
        D = close left corner
        F = close rigth corner
  --------------------------------------------------
  */
  searchBoxXOffset = searchBoxYOffset = 0.0f;

  //close edge is SEARCH_BOX_FORWARD_OFFSET away
  //from dribbling robot location along
  //vector from dribbling robot to goal destination.
  extendPoint(goalDestination, 
              dribblingRobot, 
              -SEARCH_BOX_FORWARD_OFFSET,
              closeEdge);
  
  //far edge is an additional SEARCH_BOX_HEIGHT distance away
  //from the close edge.
  extendPoint(goalDestination, 
              dribblingRobot, 
              -(SEARCH_BOX_FORWARD_OFFSET + SEARCH_BOX_HEIGHT),
              farEdge);
  
  //calculate far left/right corners
  extendPoint(goalDestination,dribblingRobot,-(SEARCH_BOX_WIDTH/2 + SEARCH_BOX_FORWARD_OFFSET + SEARCH_BOX_HEIGHT),farLeftCorner);
  farRightCorner.set(farLeftCorner);
  rotateAboutPoint(farLeftCorner, farEdge, PI/2, farLeftCorner);  
  rotateAboutPoint(farRightCorner, farEdge, -PI/2, farRightCorner);  
  
  //calculate close left/right corners
  extendPoint(goalDestination, dribblingRobot,-(SEARCH_BOX_WIDTH/2 + SEARCH_BOX_FORWARD_OFFSET),closeLeftCorner);
  closeRightCorner.set(closeLeftCorner);
  rotateAboutPoint(closeLeftCorner, closeEdge, PI/2, closeLeftCorner);  
  rotateAboutPoint(closeRightCorner, closeEdge, -PI/2, closeRightCorner);  

  //calculate true close right corner
  trueCloseRightCorner.set(dribblingRobot.getX() + (SEARCH_BOX_WIDTH/2 + SEARCH_BOX_FORWARD_OFFSET),
                           dribblingRobot.getY());
  Pair forwardPoint(dribblingRobot.getX() + SEARCH_BOX_FORWARD_OFFSET, 
                    dribblingRobot.getY());
  rotateAboutPoint(trueCloseRightCorner, forwardPoint, -PI/2, trueCloseRightCorner);  

  //========================
  //check if any points are off the field, if so nudge entire set
  
  //close left corner
  checkCorner(closeLeftCorner);

  //close right corner
  checkCorner(closeRightCorner);

  //far left corner
  checkCorner(farLeftCorner);

  //far right corner
  checkCorner(farRightCorner);
  //========================
  GUI_Record.debuggingInfo.setDebugPoint(robotID, 0, closeLeftCorner);
  GUI_Record.debuggingInfo.setDebugPoint(robotID, 1, farLeftCorner);
  GUI_Record.debuggingInfo.setDebugPoint(robotID, 2, farEdge);
  GUI_Record.debuggingInfo.setDebugPoint(robotID, 3, farRightCorner);
  GUI_Record.debuggingInfo.setDebugPoint(robotID, 4, closeRightCorner);
  GUI_Record.debuggingInfo.setDebugPoint(robotID, 5, closeEdge);
  GUI_Record.debuggingInfo.setDebugPoint(robotID, 9, goalDestination);
  //========================
  //set the bounds for searching
  leftBound = 0;
  rightBound = searchBoxWidth-1;
  lowerBound = 0;
  upperBound = searchBoxHeight-1;
}
//=====================================================
/**Nudges search box coordinates while determining offset of entire search box takes place.*/
void DribblePointSkill::nudgeCorners(float xDiff, float yDiff)
{
  searchBoxXOffset+=xDiff;
  searchBoxYOffset+=yDiff;
  farLeftCorner.set(farLeftCorner.getX() + xDiff,
	                farLeftCorner.getY() + yDiff);

  farRightCorner.set(farRightCorner.getX() + xDiff,
	                 farRightCorner.getY() + yDiff);

  closeLeftCorner.set(closeLeftCorner.getX() + xDiff,
	                  closeLeftCorner.getY() + yDiff);

  closeRightCorner.set(closeRightCorner.getX() + xDiff,
	                   closeRightCorner.getY() + yDiff);

  farEdge.set(farEdge.getX() + xDiff,
	          farEdge.getY() + yDiff);

  closeEdge.set(closeEdge.getX() + xDiff,
	            closeEdge.getY() + yDiff);

  trueCloseRightCorner.set(trueCloseRightCorner.getX() + xDiff,
	                       trueCloseRightCorner.getY() + yDiff);

}
//=====================================================
/**Checks if a corner is off the field, if is calls nudge corners to nudge this corner and all others
so they all are on the field.*/
void DribblePointSkill::checkCorner(Pair corner)
{
  //left side line
  if(corner.getY() > sp->field.LEFT_SIDE_LINE)
    nudgeCorners(0.0f, -(corner.getY() - sp->field.LEFT_SIDE_LINE));

  //right side line
  if(corner.getY() < sp->field.RIGHT_SIDE_LINE)
    nudgeCorners(0.0f, sp->field.RIGHT_SIDE_LINE - corner.getY());

  //opponent goal line
  if(corner.getX() > sp->field.THEIR_GOAL_LINE)
    nudgeCorners(-(corner.getX() - sp->field.THEIR_GOAL_LINE), 0.0f);

  //our goal line
  if(corner.getX() < sp->field.OUR_GOAL_LINE)
    nudgeCorners(sp->field.OUR_GOAL_LINE - corner.getX(), 0.0f);
}
//=====================================================
/**Provided search box size and resolution based on loaded parameters,
creates new matrix.*/
void DribblePointSkill::createSearchBox()
{
  //if search box already exists, free it
  if(searchBox)
    delete[] searchBox;
  
  //calculate size necessary for search box and create it
  searchBoxWidth = (int)(SEARCH_BOX_WIDTH / SEARCH_BOX_RESOLUTION);
  searchBoxHeight = (int)(SEARCH_BOX_HEIGHT / SEARCH_BOX_RESOLUTION);
  searchBox = new int[searchBoxWidth * searchBoxHeight];
}
//=====================================================
/**Given an array of rotated obstacles, clears search box matrix and
stamps obstacles onto it.*/
void DribblePointSkill::fillSearchBox()
{
  //assume nobody in search box
  empty = true;

  //clear box
  for(int i=0; i<searchBoxWidth*searchBoxHeight; i++)
  {
    searchBox[i] = 0;
  }  
  
  //create array of obstacles based on current vision data
  createObstacleArray();
  
  //rotate obstacles to the new search space, 
  //where our search box will directly above the
  //dribbling robot.
  rotateObstacleArray();
  
  //find the angle between dribbling robot and goal dest
  float angle = angleBetween(dribblingRobot, goalDestination);
    
  //iterate over each obstacle, stamping it on the search box
  for(i=0; i<2*NUM_ROBOTS; i++)
  {
    //if obstacle defined stamp it
    if(obstacles[i].getX() != -32000.0f)
    {
      //handle offset so in 0-(max-1) range
      float shiftedX = obstacles[i].getX() - trueCloseRightCorner.getX();
      float shiftedY = obstacles[i].getY() - trueCloseRightCorner.getY();
      
      //scale into matrix coords
      int matrixX = (int)(shiftedX/SEARCH_BOX_RESOLUTION);
      int matrixY = (int)(shiftedY/SEARCH_BOX_RESOLUTION);
      
      //iterate over entire stamp, for each unit that is on matrix stamp it's value
      for(int point = 0; point < 37; point++)
      {
        int x = matrixX + Stamp[point][0];
        int y = matrixY + Stamp[point][1];
        int value = Stamp[point][2];
        
        //if unit is on search box, place it
        if(x >= 0 && 
          x < searchBoxHeight &&
           y >= 0 &&
           y < searchBoxWidth)
        {
          searchBox[x*searchBoxWidth + y]+=value;
          empty = false;
        }        
      }
    }
  }
}
//=====================================================
/**Given a search box matrix, finds open most area in
matrix and returns its coordinates.*/
void DribblePointSkill::findOpenArea()
{
  //since there are multiple ways to quickly back out and be done,
  //we'll loop and break out of it on internal tests
  while(true)
  {
    //if bounds specify a single sqaure, we are done!
    if(rightBound-leftBound == 0 &&
       upperBound-lowerBound == 0)
      break;
    //else, split along longer side, sum up each half
    int left1, left2, left3,
        right1, right2, right3,
        lower1, lower2, lower3, 
        upper1, upper2, upper3;
      
    //wider than tall
    if(rightBound - leftBound > upperBound - lowerBound)
    {
      //split along width
        
      //first half
      left1 = leftBound;
      right1 = (rightBound + leftBound)/2;
        
      //second half
      left2 = right1 + 1;
      right2 = rightBound;
        
      //third second is halfway between 
      //first and second half, it
      //overlaps onto each
      left3 = (left1 + left2)/2;
      right3 = (right1 + right2)/2;
        
      //upper and lower bounds are the same
      lower1 = lowerBound;
      lower2 = lowerBound;
      lower3 = lowerBound;
      upper1 = upperBound;
      upper2 = upperBound;
      upper3 = upperBound;
        
    }
    //taller than wide
    else
    {
      //split along height
        
      //first half
      lower1 = lowerBound;
      upper1 = (upperBound + lowerBound)/2;
        
      //second half
      lower2 = upper1 + 1;
      upper2 = upperBound;
        
      //third second is halfway between 
      //first and second half, it
      //overlaps onto each
      lower3 = (lower1 + lower2)/2;
      upper3 = (upper1 + upper2)/2;
      
      //left and right bounds are the same
      left1 = leftBound;
      left2 = leftBound;
      left3 = leftBound;
      right1 = rightBound;
      right2 = rightBound;
      right3 = rightBound;
    }
      
    //next sum up all three portions
    int sum1, sum2, sum3;
    sum1 = sum2 = sum3 = 0;
      
    for(int y = left1; y<=right1; y++)
    {
      for(int x = lower1; x<=upper1; x++)
      {
        sum1+= searchBox[x*searchBoxWidth + y];
      }
    }
      
    for(y = left2; y<=right2; y++)
    {
      for(int x = lower2; x<=upper2; x++)
      {
        sum2+= searchBox[x*searchBoxWidth + y];
      }
    }
      
    for(y = left3; y<=right3; y++)
    {
      for(int x = lower3; x<=upper3; x++)
      {
        sum3+= searchBox[x*searchBoxWidth + y];
      }
    }
      
    //now, if entire search box is empty, set bounds to be the middle and quit
    if(sum1 + sum2 == 0)
    {
      leftBound = (rightBound+leftBound)/2;
      rightBound = leftBound;
      upperBound = (upperBound+lowerBound)/2;
      lowerBound = upperBound;
      break;
    }
    //else, pick new smaller bounds that resulted in lowest sum and loop
    else
    {
      int smallest = sum1;
      int smallestIndex = 1;
      if(sum2 < smallest)
      {
        smallest = sum2;
        smallestIndex = 2;
      }
      if(sum3 < smallest)
      {
        smallest = sum3;
        smallestIndex = 3;
      }
   
      if(smallestIndex == 1)
      {
        leftBound = left1;
        rightBound = right1;
        lowerBound = lower1;
        upperBound = upper1;
      }
      else if(smallestIndex == 2)
      {
        leftBound = left2;
        rightBound = right2;
        lowerBound = lower2;
        upperBound = upper2;
      }
      else if(smallestIndex == 3)
      {
        leftBound = left3;
        rightBound = right3;
        lowerBound = lower3;
        upperBound = upper3;
      }
    }
  }
}
//=====================================================
/**Given open area point, dribbling robot and goal desitnation, unrotates
open area point and shifts onto real coordinate plane.*/
Pair DribblePointSkill::realOpenAreaPoint()
{
  //first, unshift open area point
  Pair result(lowerBound*SEARCH_BOX_RESOLUTION + trueCloseRightCorner.getX(),
              leftBound*SEARCH_BOX_RESOLUTION + trueCloseRightCorner.getY());

  //next, unrotate point to be in real space

  //find the angle between dribbling robot and goal dest
  float angle = angleBetween(dribblingRobot, goalDestination);

  Pair shiftedDribblingRobot(dribblingRobot.getX() + searchBoxXOffset,
	                         dribblingRobot.getY() + searchBoxYOffset);

  rotateAboutPoint(result,
                   shiftedDribblingRobot,
                   angle,
                   result);
  return result;
}
//=====================================================
/**Calls various methods above to draw search box between robot and 
goal destination, uses find open area to calculate best point,
and returns that point in the real coordinate plane.*/
Pair DribblePointSkill::dribblePointBox()
{
  //determine starting search bounds
  determineSearchBounds();

  //clear search box and fill it with obstacles
  fillSearchBox();

  //run findOpenArea algorithm until best point chosen
  findOpenArea();

  //unshift and unrotate result into real coordinate plane and return
  return realOpenAreaPoint();
}
//=====================================================
