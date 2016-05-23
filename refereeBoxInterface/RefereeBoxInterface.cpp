// RefereeBoxInterface.cpp : Defines the entry point for the console application.
//
#include "RefBoxInterface.h"
#include <iostream>

int main(int argc, char* argv[])
{
  RefBoxInterface test;

  teamColor color = YELLOW;
  int errorCode;
  errorCode = test.initialize(color, "COM1");

  if (!errorCode)
    test.start();
  refBoxCommand command = EMPTY;

  while (command != PENALTY_THEM) {
    command = test.getEvent();
    if (command != EMPTY)
      cout << "***COMMAND: " << command << endl;
  }

  return 0;

}

