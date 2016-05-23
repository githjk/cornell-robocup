#ifndef JUKE_AROUND_OPPONENT_SKILL_H
#define JUKE_AROUND_OPPONENT_SKILL_H

#include "skill.h"

class JukeAroundOpponentSkill : public Skill
{
public:
  static int skillNum;

  /**@name    public functions  */
  //@{
  //========================================================
  ///Constructor.  Calls the base class Skill constructor
  JukeAroundOpponentSkill(SystemParameters *sp,  
                          VisionModule *vm, 
            			        RoboCupStrategyModule *strat,
                          RobotIndex id, 
                          SkillSet* set);
  //========================================================
  ~JukeAroundOpponentSkill()  
  {
  }
  //========================================================
  /**Call this function to chec kthe prerequisites of the skill.  This will return 
  a bool indicating whether or not the skill is ciable in the present situation.*/
  bool isValid();
  //========================================================
  ///Perform any initializations for the skill, such as reaseting the timer.
  void initialize() {}
  //========================================================
  //takes in an ultimate destination, used to determine
  //if an opponent is in your way
  void initialize(Pair newDest);
  //========================================================
  /**Indicates if the skill is still viable (whether it is frustrated or not).
  True = skill is okay.  False = skill is frustrated.*/
  bool evaluate();
  //========================================================
  /**For tweaking the skill.  You may want to change local parameters or behave 
  differently to adapt to any situation that is frustrating the skill.*/
  void update();
  //========================================================
  ///determine if we're finished yet
  bool isFinished(float tolerance);
  //========================================================
 ///Reads in parameters for this skill
  void loadValues();
  //========================================================
  ///Returns the skill's name
  char* getName() const { 
	  return "JukeAroundOpponentSkill"; 
  }
  //}@

//========================================================
protected:
  /**Execute the skill.  This is the main part of the skill, where you tell the
  robot how to perform the skill.*/
  void execute();
  //-----------------
  //Private Variables
  //-----------------
  //-----------
  //which technique are we using to get arond the opponent
  typedef enum
  {
    NO_TECHNIQUE = -1,
    PUSH_THROUGH = 0,
    MOVE_AROUND,
    FAKE_AROUND,
    NUM_TECHNIQUES
  };
  //-----------
  typedef enum
  {
    STEP_0 = 0,
    STEP_1,
    STEP_2,
    STEP_3,
    STEP_4,
    STEP_5,
  };
  //-----------
  //structs used by PosCalculate and VelCalculate
  struct translation
  {
    float Radius;
    float Frequency;
    float Phase;
  };
  
  struct rotation
  {
    float Frequency;
    float Phase;
  };
  //-----------
  //the goal we are striving to reach ultimately
  Pair dest;
  //-----------
  //which technique we are currently using
  //aka push_through,move_around, or fake_around
  int currentTechnique;
  //-----------
  //are we frustrated with the current technique?
  bool frustrated;
  bool finished;
  //-----------
  //state within the technique, aka which step if 
  //there are a series of steps
  //like turning then moving
  int techniqueStep;
  //-----------
  //opponent who we are juking around's ID
  RobotIndex opponentID;
  //-----------
  //params loaded from file
  float Biases[NUM_TECHNIQUES];
  float DELTA_BIAS;
  float MAX_TIME_PER_TECHNIQUE;
  //--
  float PUSH_THROUGH_AVOID_DISTANCE;
  float PUSH_THROUGH_DISTANCE;
  float PUSH_THROUGH_ANGLE_THRESHHOLD;
  //--
  float MOVE_AROUND_AVOID_DISTANCE;
  float MOVE_AROUND_ANGLE_THRESHHOLD;
  float MOVE_AROUND_DELTA;
  float MOVE_AROUND_ROTATION_RATE;
  //--
  float FAKE_AROUND_AVOID_DISTANCE;
  float FAKE_AROUND_DISTANCE;
  float FAKE_AROUND_ANGLE_THRESHHOLD;
  //-----------
  //variabes that hold some state for each of the techniques
  bool goAroundLeft;
  //--  
  Pair pushToPoint;
  //--
  Pair moveThroughPoint;
  Pair moveAroundLookToPoint;
  //--
  Pair fakeToPoint;
//  Pair fakeBackPoint;
  Pair fakeThroughPoint;
  //-----------

  //---------------
  //Private Methods
  //---------------
  //picks a new technique, if passed in lastTechnique is valid it updates
  //the bias's
  void pickNewTechnique(int lastTechnique = -1);  
  //========================================================
  //these methods are the various techniques that can be executed.
  void pushThrough();
  void moveAround();
  void fakeAround();
  //========================================================
  //algorithm sets sidePoint to the side of the obstacle that is closest to you
  void pickSide(Pair us, 
                Pair goalDest, 
                Pair obstacle, 
                float avoidDistance, 
                Pair& sidePoint);
  //========================================================
};

#endif //JUKE_AROUND_OPPONENT_SKILL_H
