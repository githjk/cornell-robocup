typedef enum refBoxCommand {
  START = 0,
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

struct RefBoxInfo {
  refBoxCommand command;
  unsigned long time;

};