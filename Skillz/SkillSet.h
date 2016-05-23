#ifndef SKILL_SET_H
#define SKILL_SET_H

#include "ai.h"
#include "VisionModule.h"
#include "Skill.h"
#include "common/datatypes/SystemParameters.h"

//begin hack
class Skill;
//end hack

/**
A skill set holds a bag of skillz for a robot to use at any time.
 */
class SkillSet
{

protected:
  /**@name    protected variables  */
  //@{            

  Skill **skillArray;

  //}@

public:  
  /**@name    public functions  */
  //@{
  //Default constructor.  Does nothing.
  SkillSet() {}  
  //======================================
  //Creates an array of skillz pointers and actual 
  //instances of all these lovely skillz
  SkillSet(SystemParameters *sp,
           VisionModule *vm, 
           RoboCupStrategyModule *dest,
           RobotIndex id);
  //======================================
  //Destroys all instances of skillz, then 
  //destroys array of skill pointers
  ~SkillSet();  
  //======================================
  ///This method returns a pointer to a skill in the skill set
  Skill* getSkill(int skillNumber);
  //======================================
  ///Runs evaluate method of all skillz in the skill set
  void evaluateSkillz();
  //======================================
  ///Runs update method of all skillz in the skill set
  void updateSkillz();
  //======================================
  ///Runs loadValues() method of all skillz in the skill set
  void loadSkillParams();
  //======================================
  ///Resets all skills' hasBeenRun variable so they can run next frame
  void resetSkillz();
  //======================================
  ///Uninitializes all skills for this robot
  void unitializeSkillz();
  //======================================
  ///Updates all vision and strategy data pointers for all skills in the skill set
  void updateVisionAndStrategyPointers();
  //======================================
  //}@


};

#endif //SKILL_SET_H