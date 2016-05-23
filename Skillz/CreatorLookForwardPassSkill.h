#ifndef CREATOR_LOOK_FORWARD_PASS_SKILL_H
#define CREATOR_LOOK_FORWARD_PASS_SKILL_H

#include "skill.h"
class CreatorLookForwardPassSkill : public Skill
{
public:
 static int skillNum;

/**@name	public functions  */
  //@{
  ///Constructor.  Calls the base class Skill constructor
  CreatorLookForwardPassSkill(SystemParameters *sp,
                              VisionModule *vm, 
                              RoboCupStrategyModule *strat,
                              RobotIndex id, 
                              SkillSet* set);

  /**Call this function to chec kthe prerequisites of the skill.  This will return 
  a bool indicating whether or not the skill is ciable in the present situation.*/
  bool isValid();

  ///always call the more specific init function below
 void initialize();
  
 /**Indicates if the skill is still viable (whether it is frustrated or not).
  True = skill is okay.  False = skill is frustrated.*/
  bool evaluate();

  /**For tweaking the skill.  You may want to change local parameters or behave 
  differently to adapt to any situation that is frustrating the skill.*/
  void update();

  ///always false
  bool isFinished(float tolerance);

  ///Reads in parameters for this skill
  void loadValues();

  ///Returns the skill's name
  char* getName() const { 
	  return "CreatorLookForwardPassSkill"; 
  }
  //}@
protected:
/**Execute the skill.  This is the main part of the skill, where you tell the
  robot how to perform the skill.*/
  void execute();

  int xzone, yzone;
  float zone1Line, zone2Line, zone3Line, yoffset;
  float hyster;
  int PASSING;


  float y_line_rand, x,y;
  //Randomization
  float LINE_CHANCE;

  //line placement randomization
  float Y_ZONE_WIDTH;

  //messing up defense spot and randomization
  float X_POS;
  float Y_POS;
  float SPOT_SIZE;
  float FACTOR;

  //mode variable
  bool line_mode;

};

#endif //CREATOR_LOOK_FORWARD_PASS_SKILL_H