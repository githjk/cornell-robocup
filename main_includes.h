//We need to be able to access the standard 
//module with the GUI so declare it in header

#ifndef MAIN_H_INCLUDES
#define MAIN_H_INCLUDES

const bool Cornell_is_Superior = 1;

#include "refereeBoxInterface/RefBoxInterface.h"

//Instance of referee box inteface
RefBoxInterface* refBoxInterface;

//resets working debugging points so they are not displayed unless plays during
//this frame actually set them again
void resetDebuggingPoints();

//copies working debugging points to real debugging points. prevents flickering.
void copyDebuggingPoints();

//This method checks for any changes the GUI is instructing (like toggling AI status)
//and changed them. it is called from the main method rigth AFTER execute frame
void reactToAICommands();

//This method checks for any changes the Referee Box is instructing, and
//changes any state variables to refelct them.
void reactToRefereeCommands();

#endif //MAIN_H_INCLUDES