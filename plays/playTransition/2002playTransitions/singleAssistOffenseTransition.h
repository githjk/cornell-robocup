#ifndef SINGLE_ASSIST_OFFENSE_TRANSITION_H
#define SINGLE_ASSIST_OFFENSE_TRANSITION_H

#include "robocup_strategy_data.h"
#include "..\transition.h"
#include "common/vision/visionTimer.h"

/*****************************
* SingleAssistOffense play Transition    *
*****************************/
class SingleAssistOffenseTransition : public Transition
{
public:
  SingleAssistOffenseTransition::SingleAssistOffenseTransition(VisionModule *vm) : Transition(vm)
	{
      loadValues();
	}

	void executePlayTransition(VisionData& vision, RobocupStrategyData* rsd, BasePlay* currentPlay);
	void loadValues();
  bool twoOpponentsFound(const VisionData& field, RobocupStrategyData* rsd); 
  bool manOnCreator(const VisionData& field, 
                    RobocupStrategyData* rsd, 
                    Pair aggressorLocation,
                    Pair creatorLocation);

protected:
  // The max time allowed for the play
  float MAX_ELAPSED_TIME;
  float PASSING_LINE_THEIR_GOALLINE_OFFSET;
  float DOUBLE_PICK_OFFSET; 
  float KICK_LINE_OFFSET; 
  float PASSING_LINE;
  float KICK_LINE;
  float DOUBLE_PICK_LINE;
  float EDGE_DIST;
  float EXTEND_OFFSET;
  float KICK_UPFIELD_LANE_WIDTH;
  float KICK_UPFIELD_ANGLE;
  float DOUBLE_PICK_DIST;
  float MIN_SEP_DIST;
  float MAN_ON_CONE_ANGLE;
  float PASSING_DANGER_ZONE;
  bool timerOn;
};

#endif // SINGLE_ASSIST_OFFENSE_TRANSITION_H
