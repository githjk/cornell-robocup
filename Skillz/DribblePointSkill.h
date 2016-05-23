#ifndef DRIBBLE_POINT_SKILL_H
#define DRIBBLE_POINT_SKILL_H

#include "skill.h"

class DribblePointSkill : public Skill
{
public:
  
  static int skillNum;

  /**@name    public functions  */
  //@{
    //--------------------------------------------------------------------------
  ///Constructor.  Calls the base class Skill constructor
  DribblePointSkill(SystemParameters *sp,
                    VisionModule *vm, 
                    RoboCupStrategyModule *strat,
                    RobotIndex id, 
                    SkillSet* set);
  //--------------------------------------------------------------------------
  /**Distroys list of ignore id's*/
  ~DribblePointSkill();
  //--------------------------------------------------------------------------
  /**Call this function to chec kthe prerequisites of the skill.  This will return 
  a bool indicating whether or not the skill is ciable in the present situation.*/
  bool isValid();
  //--------------------------------------------------------------------------
  //always call the more specific init function below
  void initialize() { }
  //--------------------------------------------------------------------------
    /**Initializes skill*/
  void initialize(Pair goalDestination, 
                  RobotIndex* friendlyIgnoreIDs,
                  int newNumIgnoreIDs);
  //--------------------------------------------------------------------------
  /**Indicates if the skill is still viable (whether it is frustrated or not).
  True = skill is okay.  False = skill is frustrated.*/
  bool evaluate();
  //--------------------------------------------------------------------------
  /**For tweaking the skill.  You may want to change local parameters or behave 
  differently to adapt to any situation that is frustrating the skill.*/
  void update();
  //--------------------------------------------------------------------------
  ///always false
  bool isFinished(float tolerance);
  //--------------------------------------------------------------------------
  ///Reads in parameters for this skill
  void loadValues();
  //--------------------------------------------------------------------------
  ///Returns the skill's name
  char* getName() const 
  { 
	  return "DribblePointSkill"; 
  }
  //}@

protected:
  //--------------------------------------------------------------------------
  /**Execute the skill.  This is the main part of the skill, where you tell the
  robot how to perform the skill.*/
  void execute();
  //--------------------------------------------------------------------------
  /**Given an array of friendly robots to ignore, fills array
  with all obstacle positions.*/
  void createObstacleArray();
  //--------------------------------------------------------------------------
  /**Given an array of obstacles, this robot location and the goal destination,
  rotates all obstacles such that in the rotated space the vector from the
  dribbling robot to the goal dest has a slope of 0.*/
  void rotateObstacleArray();
  //--------------------------------------------------------------------------
  /**Based on placement of search box, determines beginning 
  croppings such that no point will be chosen off the field.*/
  void determineSearchBounds();
  //--------------------------------------------------------------------------
  /**Provided search box size and resolution based on loaded parameters,
  creates new matrix.*/
  void createSearchBox();  
  //--------------------------------------------------------------------------
  /**Given an array of rotated obstacles, clears search box matrix and
  stamps obstacles onto it.*/
  void fillSearchBox();
  //--------------------------------------------------------------------------
  /**Finds open most area in search box
  matrix and sets bounds to be that most open unit.*/
  void findOpenArea();
  //--------------------------------------------------------------------------
  /**Given open area point, dribbling robot and goal desitnation, unrotates
  open area point and shifts onto real coordinate plane.*/
  Pair realOpenAreaPoint();
  //--------------------------------------------------------------------------
  /**Calls various methods above to draw search box between robot and 
  goal destination, uses find open area to calculate best point,
  and returns that point in the real coordinate plane.*/
  Pair dribblePointBox();
  //--------------------------------------------------------------------------
  /**Nudges search box coordinates while determining offset of entire search box takes place.*/
  void nudgeCorners(float xDiff, float yDiff);
  //--------------------------------------------------------------------------
  /**Checks if a corner is off the field, if is calls nudge corners to nudge this corner and all others
  so they all are on the field.*/
  void checkCorner(Pair corner);
  //--------------------------------------------------------------------------
  //has skill be initialized?

  //screening robots ID
  RobotIndex ID;

  //location of dribbling robot
  Pair dribblingRobot;

  //goal desitination
  Pair goalDestination;

  //id's of robots to ignore
  RobotIndex* friendlyIgnoreIDs;

  //num of id's being ignored
  int numIgnoreIDs;

  //array that holds obstacle positions
  Pair obstacles[2*NUM_ROBOTS];

  //search box
  int* searchBox;
  int searchBoxWidth;
  int searchBoxHeight;

  //used for (un)shifting objects
  Pair trueCloseRightCorner;

  //searchBox offsets to ensure on field
  float searchBoxXOffset, searchBoxYOffset;

  //specify dimentions of search box
  Pair farLeftCorner, farRightCorner, closeLeftCorner, closeRightCorner;
  Pair farEdge, closeEdge;

    //bounds being searched
  int leftBound;
  int rightBound;
  int upperBound;
  int lowerBound;

  //used when determing bounds for searching search box
  #define NO_INTERSECTION -1
  #define USE_CLOSE 0
  #define USE_FAR 1
  
  //params which define size and resolution of search box
  float SEARCH_BOX_WIDTH;
  float SEARCH_BOX_HEIGHT;
  float SEARCH_BOX_RESOLUTION;
  float SEARCH_BOX_FORWARD_OFFSET;
  float MAX_TURN_ANGLE;
  float MAX_TURN_ANGLE_THRESHOLD;
  
  bool turning;
  bool empty;
};

#endif //DRIBBLE_POINT_SKILL_H
