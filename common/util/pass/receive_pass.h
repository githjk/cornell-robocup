#ifndef RECEIVE_PASS_H
#define RECEIVE_PASS_H

#include "ai.h"
#include "VisionModule.h"
#include "robocup_strategy_data.h"
#include "common/datatypes/SystemParameters.h"

bool interceptPass(const Pair& ballPos,
                 const Pair& ballVel,
                 const Pair& passTarget,
                 Pair* interceptionPoint,
                 const SystemParameters& params);

#endif //RECEIVE_PASS_H