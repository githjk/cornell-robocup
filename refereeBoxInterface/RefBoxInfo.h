#ifndef REFBOXINFO_H
#define REFBOXINFO_H


/**
   This is a enumeration of the Commands that the RefBoxInterface 
   returns.  Each of these commands are given with respect to the 
   team color specified in the instantiation of the team color.  
*/
typedef enum refBoxCommand {
  EMPTY = 0,
  START,
  STOP,
  BEGIN_FIRST_HALF,
  BEGIN_HALF_TIME,
  BEGIN_SECOND_HALF,
  OVERTIME,
  PENALTY_SHOOTOUT,
  NEUTRAL_RESTART,
  KICK_OFF_US,
  KICK_OFF_THEM,
  PENALTY_US,
  PENALTY_THEM,
  FREE_KICK_US,
  FREE_KICK_THEM,
  TIMEOUT_US,
  TIMEOUT_THEM,
  TIMEOUT_END,
  GOAL_US,
  GOAL_THEM,
  CONNECTED
} refBoxCommand_t;

/**
   This is the structure that holds the command received and the time 
   stamp of when it was received.  Time right now is set to the 
   a counter from the first instruction received.
 */
struct RefBoxInfo {
  refBoxCommand command;
  long time;


};

#endif