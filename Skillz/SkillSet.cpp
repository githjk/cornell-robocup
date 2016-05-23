#include "SkillSet.h"
#include "SkillSetIncludes.h"
#include "Skill.h"
#include "RegisterSkillz.h"
//===============================================
SkillSet::SkillSet(SystemParameters *sp,
                   VisionModule *vm, 
                   RoboCupStrategyModule *strategy,
            		   RobotIndex id)
{
  skillArray = new Skill*[numSkillz];
  //-----
  //If you ever add a skill to the skillset, add a line to
  //actually create an instance here.
  //Eg:
  //skillArray[DribblePointSkill::skillNum] = new DribblePointSkill(sp, vm, dest, id);
  //-----
  skillArray[TestSkill::skillNum] = new TestSkill(sp, vm, strategy, id, this);
  skillArray[SimpleScreenSkill::skillNum] = new SimpleScreenSkill(sp, vm, strategy, id, this);
  skillArray[SpinAroundBallSkill::skillNum] = new SpinAroundBallSkill(sp, vm, strategy, id, this);
  skillArray[RotateAroundBallSkill::skillNum] = new RotateAroundBallSkill(sp, vm, strategy, id, this);
  skillArray[MoveToBallSkill::skillNum] = new MoveToBallSkill(sp, vm, strategy, id, this);
  skillArray[PickSkill::skillNum] = new PickSkill(sp, vm, strategy, id, this);
  skillArray[JukeAroundOpponentSkill::skillNum] = new JukeAroundOpponentSkill(sp, vm, strategy, id, this);
  skillArray[PullBallOffWallSkill::skillNum] = new PullBallOffWallSkill(sp, vm, strategy, id, this);
  skillArray[PullBallOffCornerSkill::skillNum] = new PullBallOffCornerSkill(sp, vm, strategy, id, this);
  skillArray[ManeuverWithBallSkill::skillNum] = new ManeuverWithBallSkill(sp, vm, strategy, id, this);  
  skillArray[TurnAndKickSkill::skillNum] = new TurnAndKickSkill(sp, vm, strategy, id, this);  
  skillArray[DribblePointSkill::skillNum] = new DribblePointSkill(sp, vm, strategy, id, this);  
  skillArray[MoveAroundPickSkill::skillNum] = new MoveAroundPickSkill(sp, vm, strategy, id, this);  
  skillArray[StripBallSkill::skillNum] = new StripBallSkill(sp, vm, strategy, id, this);  
  skillArray[InterceptBallSkill::skillNum] = new InterceptBallSkill(sp, vm, strategy, id, this);  
  skillArray[DiveSkill::skillNum] = new DiveSkill(sp, vm, strategy, id, this);  
  skillArray[SimpleDefenseSkill::skillNum] = new SimpleDefenseSkill(sp, vm, strategy, id, this); 
  skillArray[CutGoalSkill::skillNum] = new CutGoalSkill(sp, vm, strategy, id, this); 
  skillArray[TandemBlockerSkill::skillNum] = new TandemBlockerSkill(sp, vm, strategy, id, this); 
  skillArray[TandemDefenderSkill::skillNum] = new TandemDefenderSkill(sp, vm, strategy, id, this);
  skillArray[LooseDSpecialOpSkill::skillNum] = new LooseDSpecialOpSkill(sp, vm, strategy, id, this);
  skillArray[CreatorLookForwardPassSkill::skillNum] = new CreatorLookForwardPassSkill(sp, vm, strategy, id, this); 
  skillArray[JamAndShootSkill::skillNum] = new JamAndShootSkill(sp, vm, strategy, id, this); 
  skillArray[TightDefenseBlockerSkill::skillNum] = new TightDefenseBlockerSkill(sp, vm, strategy, id, this); 
  skillArray[TightDefenseDefenderSkill::skillNum] = new TightDefenseDefenderSkill(sp, vm, strategy, id, this); 
  skillArray[TightDefenseSpecialOpDSkill::skillNum] = new TightDefenseSpecialOpDSkill(sp, vm, strategy, id, this); 
  skillArray[PassBlockSkill::skillNum] = new PassBlockSkill(sp, vm, strategy, id, this); 
  skillArray[ShotBlockSkill::skillNum] = new ShotBlockSkill(sp, vm, strategy, id, this); 
  skillArray[BlockerGainPossessionSkill::skillNum] = new BlockerGainPossessionSkill(sp, vm, strategy, id, this); 
  skillArray[AcquirePossessionSkill::skillNum] = new AcquirePossessionSkill(sp, vm, strategy, id, this); 
  skillArray[CageDefenseSkill::skillNum] = new CageDefenseSkill(sp, vm, strategy, id, this); 
  skillArray[SpecialOpCageDefenseSkill::skillNum] = new SpecialOpCageDefenseSkill(sp, vm, strategy, id, this); 
  skillArray[LookForChipKickSkill::skillNum] = new LookForChipKickSkill(sp, vm, strategy, id, this); 
  skillArray[CrossCreasePassSkill::skillNum] = new CrossCreasePassSkill(sp, vm, strategy, id, this); 
  skillArray[OppositeReboundOffenseSkill::skillNum] = new OppositeReboundOffenseSkill(sp, vm, strategy, id, this); 
  skillArray[MidReboundOffenseSkill::skillNum] = new MidReboundOffenseSkill(sp, vm, strategy, id, this); 
  skillArray[BehindScreenOffenseSkill::skillNum] = new BehindScreenOffenseSkill(sp, vm, strategy, id, this); 
  skillArray[AdvanceBallUpfieldSkill::skillNum] = new AdvanceBallUpfieldSkill(sp, vm, strategy, id, this); 
  skillArray[StiffarmOffenseSkill::skillNum] = new StiffarmOffenseSkill(sp, vm, strategy, id, this); 
  skillArray[ReboundOffenseSkill::skillNum] = new ReboundOffenseSkill(sp, vm, strategy, id, this); 
  skillArray[TouchShootSkill::skillNum] = new TouchShootSkill(sp, vm, strategy, id, this); 
  skillArray[RemoveBallSkill::skillNum] = new RemoveBallSkill(sp, vm, strategy, id, this);
  skillArray[RemoveBallScreenSkill::skillNum] = new RemoveBallScreenSkill(sp, vm, strategy, id, this);
  skillArray[DribblePickScoreSkill::skillNum] = new DribblePickScoreSkill(sp, vm, strategy, id, this);
  skillArray[DoublePickBlockSkill::skillNum] = new DoublePickBlockSkill(sp, vm, strategy, id, this);
  skillArray[PassToCreatorSkill::skillNum] = new PassToCreatorSkill(sp, vm, strategy, id, this);
  skillArray[CreatorCreateSkill::skillNum] = new CreatorCreateSkill(sp, vm, strategy, id, this);
  skillArray[SpecialOpBlockSkill::skillNum] = new SpecialOpBlockSkill(sp, vm, strategy, id, this);
  skillArray[ReceivePassSkill::skillNum] = new ReceivePassSkill(sp, vm, strategy, id, this);
  skillArray[CrossCreaseCreatorSkill::skillNum] = new CrossCreaseCreatorSkill(sp, vm, strategy, id, this);
  skillArray[SimpleKickSkill::skillNum] = new SimpleKickSkill(sp, vm, strategy, id, this);
  skillArray[ThreeManBlockerSkill::skillNum] = new ThreeManBlockerSkill(sp, vm, strategy, id, this);
  skillArray[ThreeManSideSkill::skillNum] = new ThreeManSideSkill(sp, vm, strategy, id, this);
  skillArray[RemoveBallSetupSkill::skillNum] = new RemoveBallSetupSkill(sp, vm, strategy, id, this);
  skillArray[RemoveBallHelpSkill::skillNum] = new RemoveBallHelpSkill(sp, vm, strategy, id, this);
  skillArray[PenaltyShotUsAimAndShootSkill::skillNum] = new PenaltyShotUsAimAndShootSkill(sp, vm, strategy, id, this);
  skillArray[CreatorPassSkill::skillNum] = new CreatorPassSkill(sp, vm, strategy, id, this);
  skillArray[BackPassSkill::skillNum] = new BackPassSkill(sp, vm, strategy, id, this);
  skillArray[SupplementThreeManSkill::skillNum] = new SupplementThreeManSkill(sp, vm, strategy, id, this);
  skillArray[FacilatePassSkill::skillNum] = new FacilatePassSkill(sp, vm, strategy, id, this);
  skillArray[FollowDefenderSkill::skillNum] = new FollowDefenderSkill(sp, vm, strategy, id, this);
  skillArray[OppositeCoverSkill::skillNum] = new OppositeCoverSkill(sp, vm, strategy, id, this);
  skillArray[SmartRotateAroundBallSkill::skillNum] = new SmartRotateAroundBallSkill(sp, vm, strategy, id, this);
  skillArray[AggressivePullBallOffWallSkill::skillNum] = new AggressivePullBallOffWallSkill(sp, vm, strategy, id, this);
  skillArray[AggressivePullBallOffCornerSkill::skillNum] = new AggressivePullBallOffCornerSkill(sp, vm, strategy, id, this);
  skillArray[AggressiveJamAndShootSkill::skillNum] = new AggressiveJamAndShootSkill(sp, vm, strategy, id, this);
  skillArray[StupidJamAndShootSkill::skillNum] = new StupidJamAndShootSkill(sp, vm, strategy, id, this);
  skillArray[CarefulStripBallSkill::skillNum] = new CarefulStripBallSkill(sp, vm, strategy, id, this);
  skillArray[PenaltySpinSkill::skillNum] = new PenaltySpinSkill(sp, vm, strategy, id, this);
  skillArray[StrafeSkill::skillNum] = new StrafeSkill(sp, vm, strategy, id, this);
  
  //-----
}
//===============================================
SkillSet::~SkillSet()
{
  //delete each instance of every skill created
  for(int i=0; i<numSkillz; i++)
  {
     //delete this skill instance
     delete skillArray[i];
     skillArray[i] = NULL;
  }

  //delete the array of skill pointers
  delete[] skillArray;
}
//===============================================
Skill* SkillSet::getSkill(int skillNumber)
{
   return skillArray[skillNumber];
}
//===============================================
void SkillSet::evaluateSkillz()
{ 
  //run each skill's evaluate function
  for(int i=0; i<numSkillz; i++)
  {
     skillArray[i]->evaluate();
  }
}
//===============================================
void SkillSet::updateSkillz()
{
  //run each skill's update function
  for(int i=0; i<numSkillz; i++)
  {
     skillArray[i]->update();
  }
}
//===============================================
void SkillSet::loadSkillParams() {
  //run each skill's loadValues() function
  for(int i=0; i<numSkillz; i++)
  {
     skillArray[i]->loadValues();
  }
}
//===============================================
void SkillSet::resetSkillz() 
{
 //reset the hasBeenRun variable
  for(int i=0; i<numSkillz; i++)
  {
     skillArray[i]->resetSkill();
  }
}
//===============================================
//Uninitializes all skills for this robot
void SkillSet::unitializeSkillz()
{  
  //reset the hasBeenRun variable
  for(int i=0; i<numSkillz; i++)
  {
     skillArray[i]->unInitialize();
  }
}
//===============================================
///Updates all vision and strategy data pointers for all skills in the skill set
void SkillSet::updateVisionAndStrategyPointers()
{
  //update pointers
  for(int i=0; i<numSkillz; i++)
  {
     skillArray[i]->updatePointers();
  }
}
//===============================================
