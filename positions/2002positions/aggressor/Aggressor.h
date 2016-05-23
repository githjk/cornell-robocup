#ifndef AGGRESSOR_H
#define AGGRESSOR_H
/***********************************************************************************/
//	HEADER FILES
#include "plays/plays.h"
#include "robocup_strategy_data.h"
#include "../../ScratchVars.h"
/***********************************************************************************/
//Basic rule of thumb, each play gets it's own action function!!! NOOO exceptions.
//If many plays have position doing same thing, make special action function and
//have the various action functions call that action function. Make debugging much easier.
namespace Aggressor 
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
  void penaltyShotUsPivot2002(RobotIndex ID, 
                              BasePlay* play, 
                              const VisionData& field, 
                              RobocupStrategyData* sd);
  //-----------------------------------------------------------------
  void penaltyShotUsOpenLoop2002(RobotIndex ID, 
                                 BasePlay* play, 
                                 const VisionData& field, 
                                 RobocupStrategyData* sd);
  //-----------------------------------------------------------------
  void penaltyShotUsSideStep2002(RobotIndex ID, 
                                 BasePlay* play, 
                                 const VisionData& field, 
                                 RobocupStrategyData* sd);
  //-----------------------------------------------------------------
  void penaltyShotThem2002(RobotIndex ID, 
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
}
/***********************************************************************************/
#endif	// AGGRESSOR_H



