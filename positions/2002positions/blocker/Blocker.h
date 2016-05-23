#ifndef BLOCKER_H
#define BLOCKER_H
/***********************************************************************************/
//	HEADER FILES
#include "plays/plays.h"
#include "robocup_strategy_data.h"
#include "../../ScratchVars.h"
#include "Skillz/SkillSet.h"
/***********************************************************************************/
//Basic rule of thumb, each play gets it's own action function!!! NOOO exceptions.
//If many plays have position doing same thing, make special action function and
//have the various action functions call that action function. Make debugging much easier.
namespace Blocker 
{
  //=================================================================
  //Play Action Functions
  //-----------------------------------------------------------------
  void cageDefense(RobotIndex ID, 
                   BasePlay* play, 
                   const VisionData& field, 
                   RobocupStrategyData* sd);
  //-----------------------------------------------------------------
  void creatorAssistOffense(RobotIndex ID, 
                            BasePlay* play, 
                            const VisionData& field, 
                            RobocupStrategyData* sd);
  //-----------------------------------------------------------------
  void crossCreasePass(RobotIndex ID, 
                       BasePlay* play, 
                       const VisionData& field, 
                       RobocupStrategyData* sd);
  //-----------------------------------------------------------------
  void doublePickOffense(RobotIndex ID, 
                         BasePlay* play, 
                         const VisionData& field, 
                         RobocupStrategyData* sd);
  //-----------------------------------------------------------------
  void jamAndShoot(RobotIndex ID, 
                   BasePlay* play, 
                   const VisionData& field, 
                   RobocupStrategyData* sd);
  //-----------------------------------------------------------------
  void looseDefense(RobotIndex ID, 
                    BasePlay* play, 
                    const VisionData& field, 
                    RobocupStrategyData* sd);
  //-----------------------------------------------------------------
  void removeBallDefense(RobotIndex ID, 
                         BasePlay* play, 
                         const VisionData& field, 
                         RobocupStrategyData* sd);
  //-----------------------------------------------------------------
  void singleAssistOffense(RobotIndex ID, 
                           BasePlay* play, 
                           const VisionData& field, 
                           RobocupStrategyData* sd);
  //-----------------------------------------------------------------
  void tightDefense(RobotIndex ID, 
                    BasePlay* play, 
                    const VisionData& field, 
                    RobocupStrategyData* sd);
  //-----------------------------------------------------------------
  void touchShoot(RobotIndex ID, 
                  BasePlay* play, 
                  const VisionData& field, 
                  RobocupStrategyData* sd);
  //-----------------------------------------------------------------
  void upfieldDefense(RobotIndex ID, 
                      BasePlay* play, 
                      const VisionData& field, 
                      RobocupStrategyData* sd);
   //-----------------------------------------------------------------
  void kickUpfield(RobotIndex ID, 
                   BasePlay* play, 
                   const VisionData& field, 
                   RobocupStrategyData* sd);
  //-----------------------------------------------------------------
  void threeManDefense(RobotIndex ID, 
                   BasePlay* play, 
                   const VisionData& field, 
                   RobocupStrategyData* sd);
  //-----------------------------------------------------------------  
  void penaltyShotUs2002(RobotIndex ID, 
                         BasePlay* play, 
                         const VisionData& field, 
                         RobocupStrategyData* sd);
  //-----------------------------------------------------------------
  void penaltyShotThemRush2002(RobotIndex ID, 
                         BasePlay* play, 
                         const VisionData& field, 
                         RobocupStrategyData* sd);
  //-----------------------------------------------------------------
  void penaltyShotThemSmartRush2002(RobotIndex ID, 
                                    BasePlay* play, 
                                    const VisionData& field, 
                                    RobocupStrategyData* sd);
  //-----------------------------------------------------------------
  void penaltyShotThemDiveLeft2002(RobotIndex ID, 
                                   BasePlay* play, 
                                   const VisionData& field, 
                                   RobocupStrategyData* sd);
  //-----------------------------------------------------------------
  void penaltyShotThemDiveRight2002(RobotIndex ID, 
                                    BasePlay* play, 
                                    const VisionData& field, 
                                    RobocupStrategyData* sd);
  //-----------------------------------------------------------------
  void puntKick(RobotIndex ID, 
                BasePlay* play, 
                const VisionData& field, 
                RobocupStrategyData* sd);
  //-----------------------------------------------------------------
  void backPass(RobotIndex ID, 
                BasePlay* play, 
                const VisionData& field, 
                RobocupStrategyData* sd);
  //-----------------------------------------------------------------
  float getOpponentAngle(const VisionData& field,
                         const RawVisionData& rawField,
                         RobocupStrategyData* sd,
                         Pair& closestOp);
  //-----------------------------------------------------------------


}
/***********************************************************************************/
#endif	// BLOCKER_H



