/*********************************************************
 *                                                       *
 *  Open Loop Testing Code                               *
 *  ----------------------                               *
 *                                                       *
 * Uses the WirelessModule to control the 2000           *
 * or 2001 robots. Reads in a text file with velocities  *
 * and dibble/kick commands. The text file has the       *
 * following format:                                     *
 *                                                       *
 * xVelocity  yVelocity  rotVelocity  Dribble Kick       *
 *                                                       *
 * The velocities are in m/s, and the dribble/kick       *
 * flags are just one or zero. The x-axis is in the      *
 * direction that the robot is initially facing. All     *
 * values are floats                                     *
 *                                                       *
 * Example:                                              *
 * .9 .4 0 1 0                                           *
 * This moves the robot forward at .9 m/s and left at    *
 * .4 m/s without turning. The dribbler is on, but the   *
 * kick is not armed.                                    *
 *                                                       *
 * June 19, 2001 -- Kent Cseh                            *
 *********************************************************/

#include <time.h>

#include "OpenLoop.h"


int main(int argc, char **argv) {
  //These parameters are used by the Wireless module
  SystemParameters pseudoParams;
  pseudoParams.TEAM = 0;
  pseudoParams.OTHER_TEAM = 1;
  pseudoParams.GOALIE_INDEX = 4;  //only used by 2000 robots.
  pseudoParams.PARALLEL_PORT_BASE_ADDR = DEFAULT_PARALLEL_PORT_BASE_ADDR;
  pseudoParams.USE_2000_BOTS = 0;

  if((argc >= 2) && (strcmp(argv[1], "2000") == 0))
  {
    pseudoParams.USE_2000_BOTS = 1;
  }

  OpenLoop openLoopTest;
  openLoopTest.run(&pseudoParams);
  return 0;
}

void OpenLoop::run(SystemParameters* pseudoParams) {

  WirelessModule wireless(pseudoParams, &log);
  Timer timer;
  clock_t ticks1, ticks2;

//  Only uses robot Zero
  RobotIndex robot = ROBOT0;

  static Destination tempDest;


  numCommands = readCommands();
  printf("Press ENTER to run.\n");
	

	MovingObject team[5];
	float angle = 0.0f;

	// send empty packets for all other robots
/*	for (int robot_number = 0; robot_number < 5; robot_number++) {

		// Assume zero rotation
		team[robot_number].setRotation(0);

		command[robot_number].xVel = 0; //left_wheel_vel
		command[robot_number].yVel = 0; //right_wheel_vel
		command[robot_number].thetaVel = 0; //front_wheel_vel
		command[robot_number].parameter = 0;

		KickerEnabled[robot_number] = false;
		PulseKickTime[robot_number] = 0;
	}
*/
  // Boolean: Should we send non-zero velocities?
  bool transmitCommands = false;

	// Boolean: Should we send RPC commands?
	bool RPC_Running = true;
//	RPC_Running = false;
//	ResetToStatus();
	
	
	if (RPC_Running)
		wireless.rpcReset();

/*	int robot[4];
	for (j = 0; j < 4; j++)
		robot[j] = j;
*/
//	float ang_vel, normx, normy;
	bool drib = false, kick = false, vDrib=false;
	bool turnL = false, turnR = false;

	char user_input;
	bool keepRunning = true;
//	printStatus("Press any key to begin.\n");
//	getch();

  int curCommand=0;
  ticks1 = clock();
//	while ( keepRunning && curCommand<FRAME_LIMIT ) {
	while ( keepRunning ) {

		if (kbhit()) {

			user_input = getch();
			switch (user_input)
			{

			// SELECT ROBOTS
/*			case (0x30):
				break;
			case (0x31):
				break;
			case (0x32):
				break;
			case (0x33):
				break;
			case (0x34):
				break;
*/

			case (VK_RETURN):
        printf("Transmitting commands.\n");
        transmitCommands = true;
        curCommand = 0;
        ticks1 = clock();
				break;
      case ('r'):
//				printStatus("Resetting wireless.");
				printf("Resetting wireless...  ");
        RPC_Running = true;
				wireless.rpcReset();
        printf("Done.\n");
				break;
      case ('f'):
        printf("Reloading command file.\n");
        numCommands = readCommands();
        break;
/*
			case ('t'):
			case ('T'):
        pseudoParams.USE_2000_BOTS = !pseudoParams.USE_2000_BOTS;
        clearHeaderBox(pseudoParams.USE_2000_BOTS);
				break;
*/
			case (VK_SPACE):
        printf("Stopping robots.\n");
        transmitCommands = false;
				break;
			case (VK_ESCAPE):
				keepRunning = false;
				break;
			default:
				break;

			}// switch

		} // end if (kbhit())

		//******************************************

//===============================
//  Convert input Velocities to Destinations.
    ControlData controlData;
    
    tempDest.initialize();
    if(transmitCommands)
    {
      tempDest.setXVel((float) commands[curCommand].xVel );
      tempDest.setYVel((float) commands[curCommand].yVel );
      tempDest.setRotVel((float) commands[curCommand].rotVel );
      
      if(commands[curCommand].kick)
      {
        tempDest.setKick(KICK_SHOT);
      }
      if(commands[curCommand].dribble)
      {
        tempDest.setDribble(DRIBBLE_DEFAULT);
      }
      if(commands[curCommand].vDribble)
      {
        tempDest.setVerticalDribble(V_DRIBBLE_DEFAULT);
      }
      curCommand++;
    }
    //else leave tempDest as "stop"
    
    controlData.setRobotDataManually(robot, tempDest);
    
    
    if (curCommand==numCommands-1) {
      ticks2 = clock();
      int numTicks = (int)ticks2-(int)ticks1;
      float totalTime = (float)(numTicks/1000.0f);
      float fps=0;
      if (totalTime != 0) {
        fps = numCommands/totalTime;
      }
      printf("Took %f seconds send %d commands -- %f frames/sec.\n",totalTime,numCommands,fps);
      curCommand = 0;
      transmitCommands = false;
    }
      
    //send wireless packets exactly once every 25 milliseconds.
    double loopTime = timer.ensureLoopInterval_ms(1000/NUM_ITERATIONS_PER_SECOND);
      
  //send the wireless packets
    bool RPCresult = wireless.sendCommands(&controlData, team, RPC_Running);
/*  if(!RPC_Running)
  {
		printStatus(" RPC disabled.  Press ENTER to start it.");
  }
  else if(!RPCresult)
  {
		printError(" Error! RPC not responding.");
  }
  else
  {
		printStatus(" RPC OK."); 
  }
*/
  //Warn the user if the trip through the loop took too long.  This isn't the end of the world,
  // but if accurate acceleration is improtant you should decrease
  // NUM_ITERATIONS_PER_SECOND or make sure no other programs are running.
/*  printLoopTime(loopTime, 1000/NUM_ITERATIONS_PER_SECOND);
 	conprintData(wireless.getData());
  conprintWheelCommands(wireless);
//	ResetToStatus();
*/

  }
  tempDest.initialize();
  ControlData controlData;
  controlData.setRobotDataManually(robot, tempDest);
  bool RPCresult = wireless.sendCommands(&controlData, team, RPC_Running);

  getch();
  return;
}

int OpenLoop::readCommands() {
// takes care of reading the input file -- returns number of lines read from input

  file.open(INPUT_FILE, ios::nocreate);
  ASSERT(file.fail() == 0, "CAN'T OPEN INPUT FILE!");

  int i=0;
  file.eatwhite();
  while((file.peek() != -1 && file.peek() !=file.eof()) && (i<FRAME_LIMIT)) {
    readFrame(&commands[i]);
//    if (file.peek() == '\n')
      i++;
    file.eatwhite();
  }

  ASSERT(i<FRAME_LIMIT, "TOO MANY FRAMES TO EXECUTE -- LIMIT FRAME_LIMIT");
  file.close();
  return i;

}

void OpenLoop::readFrame(CommandFrame* comLine) {
// reads a line of the text file containing xVel, yVel, rotVel, dribble, kick

  //If the first character is a '#', ignore the entire line.
  file.eatwhite();
  while (file.peek() == '#'){
    file.ignore(255, '\n');
    file.eatwhite();
  }
  printf("%x",file.peek());

  if (USE_DRIBBLE_COMMAND) {
    if (USE_KICK_COMMAND) {
      if (USE_VERTICAL_DRIBBLE_COMMAND) {
        file >> comLine->xVel >> comLine->yVel >> comLine->rotVel >> comLine->dribble >> comLine->kick >> comLine->vDribble;
      } else {
        file >> comLine->xVel >> comLine->yVel >> comLine->rotVel >> comLine->dribble >> comLine->kick;
      }
    } else {
      if (USE_VERTICAL_DRIBBLE_COMMAND) {
        file >> comLine->xVel >> comLine->yVel >> comLine->rotVel >> comLine->dribble >> comLine->vDribble;
      } else {
        file >> comLine->xVel >> comLine->yVel >> comLine->rotVel >> comLine->dribble;
      }
    }
  } else {
    if (USE_KICK_COMMAND) {
      if (USE_VERTICAL_DRIBBLE_COMMAND) {
        file >> comLine->xVel >> comLine->yVel >> comLine->rotVel >> comLine->kick >> comLine->vDribble;
      } else {
        file >> comLine->xVel >> comLine->yVel >> comLine->rotVel >> comLine->kick;
      }
    } else {
      if (USE_VERTICAL_DRIBBLE_COMMAND) {
        file >> comLine->xVel >> comLine->yVel >> comLine->rotVel >> comLine->vDribble;
      } else {
        file >> comLine->xVel >> comLine->yVel >> comLine->rotVel;
      }
    }
  }
  printf("%x",file.peek());
//  ASSERT(strcmp(label, expectedLabel) == 0, "Error in parameters file: read "<<label<<" when expecting "<<expectedLabel);

}


