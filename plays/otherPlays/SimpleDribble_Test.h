/*



 *	simple_Dribble_test.h



 */



#ifndef PLAYS_SIMPLE_Dribble_TEST_H



#define PLAYS_SIMPLE_Dribble_TEST_H







#include "../baseplay.h"



#include "../basic_actions.h"



#include "Skillz/SkillSet.h"











class SimpleDribbleTest : public BasePlay 



{



public:



  static Play playNum;

  SimpleDribbleTest(VisionModule *vm);


  //====================================



  Play getID() const 



  {



	  return playNum;



  }



  //====================================



  char* getName() const 



  { 



	  return "Simple Dribble Test"; 



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







#endif //PLAYS_Dribble_TEST_H







