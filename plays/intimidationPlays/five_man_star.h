/*
 *	five_man_star.h
 */

#ifndef PLAYS_FIVE_MAN_STAR_H
#define PLAYS_FIVE_MAN_STAR_H

#include "../baseplay.h"
#include "../basic_actions.h"
/*
enum PassState
{
  PASSING_TO_0 = 0,
  PASSING_TO_1,
  PASSING_TO_2,
  PASSING_TO_3,
  PASSING_TO_4,
  RETRIEVING_BALL
};
*/
enum PassState
{
  PASSER_0 = 0,
  PASSER_1,
  PASSER_2,
  PASSER_3,
  PASSER_4,
  RETRIEVING_BALL
};

class FiveManStar : public BasePlay {
public:
  static Play playNum;

  FiveManStar(VisionModule *vm);
  
  Play getID() const {return playNum;}
  char* getName() const { return "Five Man Star"; }
  
  virtual void initializePlay(const VisionData& vision, RobocupStrategyData* rsd);
  
//  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);

  virtual ActionFunction getActionFunction(Position pos);

  virtual void executePlay(VisionData* vision, RobocupStrategyData* rsd);

  void setStarState(RobotIndex ID);
  bool receiveStarPass(RobotIndex passerID,RobotIndex receiverID,
                       const VisionData& vision,RobocupStrategyData* rsd);


//  Pair getStarPos(RobotIndex ID);

  Pair starPos[5];
  PassState starState,prevState;
};

#endif //PLAYS_FIVE_MAN_STAR_H

