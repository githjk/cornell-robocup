/*



 *	simple_kick_test.h



 */



#ifndef PLAYS_SIMPLE_KICK_TEST_H



#define PLAYS_SIMPLE_KICK_TEST_H







#include "../baseplay.h"



#include "../basic_actions.h"



#include "Skillz/SkillSet.h"











class SimpleKickTest : public BasePlay 



{



public:



  static Play playNum;

  SimpleKickTest(VisionModule *vm);


  //====================================



  Play getID() const 



  {



	  return playNum;



  }



  //====================================



  char* getName() const 



  { 



	  return "Simple Kick Test"; 



  }



  //====================================



  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);



  //====================================



  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);



  //====================================



  virtual ActionFunction getActionFunction(Position pos); 



  //====================================



private:



  //Index of robot that will be screening



  RobotIndex ID;







  //Handle on skillset for the screening robot



  SkillSet* skills;




};







#endif //PLAYS_KICK_TEST_H







