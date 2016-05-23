//We need to be able to access the standard 
//module with the GUI so declare it in header

#ifndef MAIN_H
#define MAIN_H

#include "RoboCup_modules/RoboCupStandardModule.h"

//GUI_commands uses this pointer to interact with the AI
extern RoboCupModule* sm;

#endif //MAIN_H