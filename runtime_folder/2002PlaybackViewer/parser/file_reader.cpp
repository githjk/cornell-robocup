#include <iostream.h>
#include <fstream.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include "file_reader.h"

//-------------------------------------
//default initial values
playbackSpec playbackSequenceInfo;
frame* firstFrame = NULL;
frame* lastFrame = NULL;
frame** framePointers = NULL;
crop* firstCrop = NULL;

crop* lastCrop = NULL;

int tempCropLeft = -32000;

int tempCropRight = -32000;

bool playbackExists = false;
int currentFrameNumber = 0;
int numFrames = 0;
//-------------------------------------

//#################################
//Destroy information when all done
//#################################
void destructSequence()
{
  if(playbackExists)
  {
    playbackExists = false;

	while(firstFrame)
	{
		frame* tempFrameHandle = firstFrame->nextFrame;
		delete [] firstFrame->team0Balls;
		delete [] firstFrame->team1Balls;
		delete [] firstFrame->team0Robots;
		delete [] firstFrame->team1Robots;
		delete firstFrame;
		firstFrame = tempFrameHandle;
	}

    delete [] framePointers;
    framePointers = NULL;

    //delete all outstanding crops
	while(firstCrop)
	{
		crop* nextCrop = firstCrop->nextCrop;
		delete firstCrop;
		firstCrop = nextCrop;
    }
  }

  numFrames = 0;
}

//###############################
//Create gigantic array of frame
//data and initializes it all =-)
//###############################
void initializeSequence()
{
  if(framePointers)
  {
	    delete [] framePointers;
  }

	framePointers = new frame*[numFrames];
	frame* curFrame = firstFrame;
  int i=0;
  while(curFrame)
	{
		framePointers[i] = curFrame;
		i++;
		curFrame = curFrame->nextFrame;
	}

  playbackSequenceInfo.numberOfFrames = i;

  if(playbackSequenceInfo.thisTeam == 0)
  {
		playbackSequenceInfo.otherTeam = 1;
  }
	else
  {
		playbackSequenceInfo.otherTeam = 0;
  }

}

//######################################################################################
//######################################################################################
//######################################################################################
//###############################
//Initialized all data in a frame
//###############################
void initializeFrame(frame* aFrame)
{
  //create two arrays of robots
  aFrame->team0Robots = new robot[playbackSequenceInfo.numberOfRobotsOnTeam0];
  aFrame->team1Robots = new robot[playbackSequenceInfo.numberOfRobotsOnTeam1];

  //initialize all robot data
  for(int i=0; i<playbackSequenceInfo.numberOfRobotsOnTeam0; i++)
  { 
	  aFrame->team0Robots[i].location.x = 0.0f;
    aFrame->team0Robots[i].location.y = 0.0f;
	  aFrame->team0Robots[i].rawLocation.x = 0.0f;
    aFrame->team0Robots[i].rawLocation.y = 0.0f;

    aFrame->team0Robots[i].velocity.x = 0.0f;
    aFrame->team0Robots[i].velocity.y = 0.0f;
    aFrame->team0Robots[i].rotation = 0.0f;
    aFrame->team0Robots[i].rawRotation = 0.0f;

    aFrame->team0Robots[i].rotationVelocity = 0.0f;
    aFrame->team0Robots[i].destination.x = 0.0f;
    aFrame->team0Robots[i].destination.y = 0.0f;
  	aFrame->team0Robots[i].destinationRot = 0.0f;

    aFrame->team0Robots[i].passToLocation.x = 0.0f;
    aFrame->team0Robots[i].passToLocation.y = 0.0f;
    aFrame->team0Robots[i].passToMe = false;
    aFrame->team0Robots[i].found = false;
    aFrame->team0Robots[i].kick = false;
    aFrame->team0Robots[i].dribble = false;
    aFrame->team0Robots[i].hasPossession = false;
    strcpy(aFrame->team0Robots[i].position, "Unspecified");
    strcpy(aFrame->team0Robots[i].debugMessage, "Robot data not set from file");
  }

  for(i=0; i<playbackSequenceInfo.numberOfRobotsOnTeam1; i++)
  { 
    aFrame->team1Robots[i].location.x = 0.0f;
    aFrame->team1Robots[i].location.y = 0.0f;
    aFrame->team1Robots[i].rawLocation.x = 0.0f;
    aFrame->team1Robots[i].rawLocation.y = 0.0f;
    aFrame->team1Robots[i].velocity.x = 0.0f;
    aFrame->team1Robots[i].velocity.y = 0.0f;
    aFrame->team1Robots[i].rotation = 0.0f;
    aFrame->team1Robots[i].rawRotation = 0.0f;
    aFrame->team1Robots[i].rotationVelocity = 0.0f;
    aFrame->team1Robots[i].destination.x = -32000.0f;
    aFrame->team1Robots[i].destination.y = -32000.0f;
	  aFrame->team0Robots[i].destinationRot = 0.0f;

    aFrame->team1Robots[i].passToLocation.x = -32000.0f;
    aFrame->team1Robots[i].passToLocation.y = -32000.0f;
    aFrame->team1Robots[i].passToMe = false;
    aFrame->team1Robots[i].found = false;
    aFrame->team1Robots[i].kick = false;
    aFrame->team1Robots[i].dribble = false;
    aFrame->team1Robots[i].hasPossession = false;
    strcpy(aFrame->team1Robots[i].position, "Unspecified");
    strcpy(aFrame->team1Robots[i].debugMessage, "Robot data not set from file");
  }

  //create two arrays of balls
  aFrame->team0Balls = new ball[playbackSequenceInfo.numberOfTeam0Balls];
  aFrame->team1Balls = new ball[playbackSequenceInfo.numberOfTeam1Balls];

  //initialize all ball data
  for(i=0; i<playbackSequenceInfo.numberOfTeam0Balls; i++)
  { 
    aFrame->team0Balls[i].location.x = 0.0f;
    aFrame->team0Balls[i].location.y = 0.0f;
    aFrame->team0Balls[i].rawLocation.x = 0.0f;
    aFrame->team0Balls[i].rawLocation.y = 0.0f;
    aFrame->team0Balls[i].velocity.x = 0.0f;
    aFrame->team0Balls[i].velocity.y = 0.0f;
    aFrame->team0Balls[i].found = false;
  }

  for(i=0; i<playbackSequenceInfo.numberOfTeam1Balls; i++)
  { 
    aFrame->team1Balls[i].location.x = 0.0f;
    aFrame->team1Balls[i].location.y = 0.0f;
    aFrame->team1Balls[i].rawLocation.x = 0.0f;
    aFrame->team1Balls[i].rawLocation.y = 0.0f;
    aFrame->team1Balls[i].velocity.x = 0.0f;
    aFrame->team1Balls[i].velocity.y = 0.0f;
    aFrame->team1Balls[i].found = false;
  }
 
  //Initialize all system messages
  strcpy(aFrame->systemMessages.play, "Play not loaded from file...");

  aFrame->nextFrame = NULL;
}
//######################################################################################
//######################################################################################
//######################################################################################

//################################
//Reads in a playbacksequence from
//the specified filename
//################################
bool readSequence(char* filename)
{
  //-----------------------------
  //attempt to find and load file
  //-----------------------------

  ifstream inputstream(filename);
  if(!inputstream.is_open())
  {
    cout << "ERROR! Unable to find or open " << filename << "!\n";
	cout.flush();
	return false;
  }
  
  //-----------------------------
  //ok, found file, read contents :)
  //-----------------------------
  cout << "-Successfully opened " << filename << "\n";
  cout.flush();
  
  //first destroy all old data
  destructSequence();

  numFrames = 0;



  //the longest any line in the file can be.
  const int maxCharacters = 1024;

  //this will hold the contents of the file as we
  //read it line by line
  char fileLine[maxCharacters];
 
  while(inputstream)
  {
    //get the next line of the file
    inputstream.getline(fileLine, maxCharacters);

    //set true below if we find first character is a #
    bool comment_out = false;

    //if first character is a # symbol, skip line
    for (int i=0; i< maxCharacters; i++)
    {  
      //if we hit a #, comment out
      if(fileLine[i] == '#')
      {
        comment_out = true;
        break;
      }
 
      //if we hit a space, keep searching...
      else if(fileLine[i] == ' ')
        continue;
  
      //else, nonwhitespace character so don't comment out
      break;
    }

    //if line commented out, loop and get next line
    if(comment_out)
      continue;
 
    //ok, line not commented out. depending
    //on what kind of tag we have we parse it differently
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    //this is more symbolic than anything, always
    //begin a file with this tag
    if(strcmp(fileLine, "<brml>") == 0)
      continue;
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    //once the end tag is seen, the parser quits.
    if(strcmp(fileLine, "</brml>") == 0)
    {
      cout << "-End of stream found. Quiting parser.\n";
   	  cout.flush();
      break;
    }
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    //if we find the specification tag, parse it and 
    //not only set the spec, but also intialize the sequence
    if(strncmp(fileLine, " <specification", 15) == 0)
    {
      cout << "-Specification found.\n";
   	  cout.flush();
      
      char* chunk; //arbitrary place we put chunks in from file
      char* tag; //name of tag, like frameRate

      //we have a twostep process, read
      //tag, then value, then tag, then value...
      //in order to parse properly, we need to
      //know which one we are looking for and thus how
      //to grab next chunk using which delimeters...

      //we always start by reading a tag
      bool readingTag = true;
   
      //do the initial strtok call
      //this grabes the tag name, in this case, 
      //"specification"
      //Subsequent calls will
      //return good chunks =-)
      chunk = strtok(fileLine, "<> ='\"");

      //get first tag
      chunk = strtok(NULL,"<> ='\"");
     
      //================================================
  	  //default values - overwritten when real values read
      //allows backwards compatibility
  	  playbackSequenceInfo.thisTeam = 0;      
      playbackSequenceInfo.leftSideLine         = 0.7560f;
      playbackSequenceInfo.rightSideLine        = -0.7560f;
      playbackSequenceInfo.leftDefenseZone      = 0.506f;
      playbackSequenceInfo.rightDefenseZone     = -0.506f;
      playbackSequenceInfo.leftGoalPost         = 0.2540f;
      playbackSequenceInfo.rightGoalPost        = -0.2540f;
      playbackSequenceInfo.ourGoalieBox         = -1.1550f;
      playbackSequenceInfo.theirGoalieBox       = 1.1550f;
      playbackSequenceInfo.ourGoalLine          = -1.370f;
      playbackSequenceInfo.theirGoalLine        = 1.370f;
      playbackSequenceInfo.goalDepth            = 0.20f;
      playbackSequenceInfo.leftGoalieBox        = 0.5f;
      playbackSequenceInfo.rightGoalieBox       = -0.5f;
      playbackSequenceInfo.goalieBoxDepth       = 0.20f;

      //0.5 was width until 2002, starting in 2002 is was 0.7, but then we
      //started logging it
      playbackSequenceInfo.goalWidth            = 0.5f;
      playbackSequenceInfo.goalDepth            = 0.18f;
      //only started usage in 2002, so previously not used,
      //shoudln't even be displayed
      playbackSequenceInfo.deathZoneLine        = -32000.0f;
      playbackSequenceInfo.defenseZoneLine      = -32000.0f;
      playbackSequenceInfo.offensiveZoneLine    = -32000.0f;
      playbackSequenceInfo.killZoneLine         = -32000.0f;
      playbackSequenceInfo.leftMiddleSector     = -32000.0f;
      playbackSequenceInfo.rightMiddleSector    = -32000.0f;
      playbackSequenceInfo.leftSweetSector      = -32000.0f;
      playbackSequenceInfo.rightSweetSector     = -32000.0f;
      //================================================
      //while we are still parsing the spec...
      while(chunk !=NULL)
      {
        //if we have a value, then store it in the permanent location
        //else, store chunk value, which is a tag
        if(readingTag)
        {
          //save tag 
          tag = chunk;
 
          //get next chunk
          chunk = strtok(NULL,"='>\"");
        }

        else
        {
          //store value associate with tag
          //====================================
          if(strcmp(tag, "frameRate") == 0)
            playbackSequenceInfo.frameRate = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "robotsOnTeam0") == 0)
            playbackSequenceInfo.numberOfRobotsOnTeam0 = atoi(chunk);
          //====================================
          else if(strcmp(tag, "robotsOnTeam1") == 0)
            playbackSequenceInfo.numberOfRobotsOnTeam1 = atoi(chunk);
          //====================================
          else if(strcmp(tag, "ballsOnTeam0") == 0)
            playbackSequenceInfo.numberOfTeam0Balls = atoi(chunk);
          //====================================
          else if(strcmp(tag, "ballsOnTeam1") == 0)
            playbackSequenceInfo.numberOfTeam1Balls = atoi(chunk);
          //====================================
          else if(strcmp(tag, "thisTeam") == 0)
            playbackSequenceInfo.thisTeam = atoi(chunk);
          //====================================

          else if(strcmp(tag, "leftSideLine") == 0)
            playbackSequenceInfo.leftSideLine = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "rightSideLine") == 0)
            playbackSequenceInfo.rightSideLine = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "leftDefenseZone") == 0)
            playbackSequenceInfo.leftDefenseZone = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "rightDefenseZone") == 0)
            playbackSequenceInfo.rightDefenseZone = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "leftGoalPost") == 0)
            playbackSequenceInfo.leftGoalPost = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "rightGoalPost") == 0)
            playbackSequenceInfo.rightGoalPost = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "ourGoalieBox") == 0)
            playbackSequenceInfo.ourGoalieBox = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "theirGoalieBox") == 0)
            playbackSequenceInfo.theirGoalieBox = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "ourGoalLine") == 0)
            playbackSequenceInfo.ourGoalLine = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "theirGoalLine") == 0)
            playbackSequenceInfo.theirGoalLine = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "goalboxDepth") == 0)
            playbackSequenceInfo.goalboxDepth = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "goalWidth") == 0)
            playbackSequenceInfo.goalWidth = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "goalDepth") == 0)
            playbackSequenceInfo.goalDepth = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "leftGoalieBox") == 0)
            playbackSequenceInfo.leftGoalieBox = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "rightGoalieBox") == 0)
            playbackSequenceInfo.rightGoalieBox = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "goalieBoxDepth") == 0)
            playbackSequenceInfo.goalieBoxDepth = (float)atof(chunk);
          //====================================          
          else if(strcmp(tag, "deathZoneLine") == 0)
            playbackSequenceInfo.deathZoneLine = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "defenseZoneLine") == 0)
            playbackSequenceInfo.defenseZoneLine = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "offensiveZoneLine") == 0)
            playbackSequenceInfo.offensiveZoneLine = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "killZoneLine") == 0)
            playbackSequenceInfo.killZoneLine = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "leftMiddleSector") == 0)
            playbackSequenceInfo.leftMiddleSector = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "rightMiddleSector") == 0)
            playbackSequenceInfo.rightMiddleSector = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "leftSweetSector") == 0)
            playbackSequenceInfo.leftSweetSector = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "rightSweetSector") == 0)
            playbackSequenceInfo.rightSweetSector = (float)atof(chunk);
          //====================================
          //get next chunk
          chunk = strtok(NULL," ='\"");  
        }

        //next value will be opposite
        readingTag = !readingTag;
      }      
      continue;
	}
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    //if we see a frame tag, create a new frame and insert it at the end of the list
    if(strncmp(fileLine, " <frame", 7) == 0)
    {
		frame* newFrame = new frame();
		initializeFrame(newFrame);
		if(!firstFrame)
		{
			firstFrame = newFrame;
			lastFrame = newFrame;
		}
		else
		{
			lastFrame->nextFrame = newFrame;
			lastFrame = newFrame;
		}
		numFrames++;

    if((numFrames % 1000) == 1)
    {
      cout << "-Loading frame #" << numFrames << "\n";
    }

		cout.flush();


		continue;
	}
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    //this is more symbolic than anything, always
    //end a frame with this tag
    if(strcmp(fileLine, " </frame>") == 0)
      continue;
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    //if we find a robot tag, parse it
    if(strncmp(fileLine, "  <robot", 8) == 0)
    {
      char* chunk; //arbitrary place we put chunks in from file
      char* tag; //name of tag, like frameRate

      //we have a twostep process, read
      //tag, then value, then tag, then value...
      //in order to parse properly, we need to
      //know which one we are looking for and thus how
      //to grab next chunk using which delimeters...

      //we always start by reading a tag
      bool readingTag = true;
   
      //we don't know which order tags will come so write down
      //the robot id and team number in temp ints, and the
      //rest of the values in a temp robot. then copy over
      //to real data structure at very end.
      int tempRobotID;
      int tempTeamNumber;
      robot tempRobot;

  	  tempRobot.location.x = -32000.0f;
      tempRobot.location.y = -32000.0f;
  	  tempRobot.rawLocation.x = -32000.0f;
      tempRobot.rawLocation.y = -32000.0f;
      tempRobot.velocity.x = -32000.0f;
      tempRobot.velocity.y = -32000.0f;
      tempRobot.rotation = 0.0f;
      tempRobot.rawRotation = 0.0f;
      tempRobot.rotationVelocity = 0.0f;
      tempRobot.destination.x = -32000.0f;
      tempRobot.destination.y = -32000.0f;
      tempRobot.destinationRot = -32000.0f;

      tempRobot.passToLocation.x = -32000.0f;
      tempRobot.passToLocation.y = -32000.0f;
      tempRobot.passToMe = false;
      tempRobot.found = false;
      tempRobot.kick = false;
      tempRobot.dribble = false;
      strcpy(tempRobot.position, "Unspecified");
      strcpy(tempRobot.debugMessage, "Robot data not set from file");

      //do the initial strtok call
      //this grabes the tag name, in this case, 
      //"robot"
      //Subsequent calls will
      //return good chunks =-)
      chunk = strtok(fileLine, "<> ='\"");

      //get first tag
      chunk = strtok(NULL,"<> ='\"");
     
      //while we are still parsing the spec...
      while(chunk !=NULL)
      {
        //if we have a value, then store it in the permanent location
        //else, store chunk value, which is a tag
        if(readingTag)
        {
          //save tag 
          tag = chunk;
 
          //get next chunk
          chunk = strtok(NULL,"'\"");
        }

        else
        {
          //store value associate with tag
          //====================================
          if(strcmp(tag, "team") == 0)
            tempTeamNumber = atoi(chunk);
          //====================================
          else if(strcmp(tag, "id") == 0)
            tempRobotID = atoi(chunk);
          //====================================
          else if(strcmp(tag, "x") == 0)
            tempRobot.location.x = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "y") == 0)
            tempRobot.location.y = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "rot") == 0)
            tempRobot.rotation = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "rawX") == 0)
            tempRobot.rawLocation.x = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "rawY") == 0)
            tempRobot.rawLocation.y = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "rawRot") == 0)
            tempRobot.rawRotation = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "xVel") == 0)
            tempRobot.velocity.x = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "yVel") == 0)
            tempRobot.velocity.y = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "rotVel") == 0)
            tempRobot.rotationVelocity = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "destX") == 0)
            tempRobot.destination.x = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "destY") == 0)
            tempRobot.destination.y = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "destRot") == 0)
            tempRobot.destinationRot = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "passToX") == 0)
            tempRobot.passToLocation.x = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "passToY") == 0)
            tempRobot.passToLocation.y = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "passToMe") == 0)
            tempRobot.passToMe = (atoi(chunk) != 0);
          //====================================
          else if(strcmp(tag, "found") == 0)
            tempRobot.found = (atoi(chunk) != 0);
          //====================================
          else if(strcmp(tag, "kick") == 0)
            tempRobot.kick = (atoi(chunk) != 0);
          //====================================
          else if(strcmp(tag, "dribble") == 0)
            tempRobot.dribble = (atoi(chunk) != 0);
          //====================================
          else if(strcmp(tag, "hasPossession") == 0)
            tempRobot.hasPossession = (atoi(chunk) != 0);
          //====================================
          else if(strcmp(tag, "position") == 0)
            strcpy(tempRobot.position, chunk);
          //====================================
          else if(strcmp(tag, "debugMessage") == 0)
            strcpy(tempRobot.debugMessage, chunk);
          //====================================

          //get next chunk
          chunk = strtok(NULL," ='\"");  
        }

        //next value will be opposite
        readingTag = !readingTag;
      }      

      //finished reading in robot
      //write into into arrays...

      //get a pointer to this robot =-)
      robot* thisRobot;          
      if(tempTeamNumber == 0)
        thisRobot = &lastFrame->team0Robots[tempRobotID];
      else
        thisRobot = &lastFrame->team1Robots[tempRobotID];

      //copy all data over...
      memcpy(thisRobot, &tempRobot, sizeof(robot));
     
      continue;
    }
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    //if we find a ball tag, parse it
    if(strncmp(fileLine, "  <ball", 7) == 0)
    {
      char* chunk; //arbitrary place we put chunks in from file
      char* tag; //name of tag, like frameRate

      //we have a twostep process, read
      //tag, then value, then tag, then value...
      //in order to parse properly, we need to
      //know which one we are looking for and thus how
      //to grab next chunk using which delimeters...

      //we always start by reading a tag
      bool readingTag = true;
   
      //we don't know which order tags will come so write down
      //the ball id and team number in temp ints, and the
      //rest of the values in a temp ball. then copy over
      //to real data structure at very end.
      int tempBallID;
      int tempTeamNumber;
      ball tempBall;

      //do the initial strtok call
      //this grabes the tag name, in this case, 
      //"ball"
      //Subsequent calls will
      //return good chunks =-)
      chunk = strtok(fileLine, "<> ='\"");

      //get first tag
      chunk = strtok(NULL,"<> ='\"");
     
      //while we are still parsing the spec...
      while(chunk !=NULL)
      {
        //if we have a value, then store it in the permanent location
        //else, store chunk value, which is a tag
        if(readingTag)
        {
          //save tag 
          tag = chunk;
 
          //get next chunk
          chunk = strtok(NULL,"'\"");
        }

        else
        {
          //store value associate with tag
          //====================================
          if(strcmp(tag, "team") == 0)
            tempTeamNumber = atoi(chunk);
          //====================================
          else if(strcmp(tag, "id") == 0)
            tempBallID = atoi(chunk);
          //====================================
          else if(strcmp(tag, "x") == 0)
            tempBall.location.x = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "y") == 0)
            tempBall.location.y = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "rawX") == 0)
            tempBall.rawLocation.x = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "rawY") == 0)
            tempBall.rawLocation.y = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "xVel") == 0)
            tempBall.velocity.x = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "yVel") == 0)
            tempBall.velocity.y = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "found") == 0)
            tempBall.found = (atoi(chunk) != 0);
          //====================================

          //get next chunk
          chunk = strtok(NULL," ='\"");  
        }

        //next value will be opposite
        readingTag = !readingTag;
      }      

      //finished reading in ball
      //write into into arrays...

      //get a pointer to this ball =-)
      ball* thisBall;          
      if(tempTeamNumber == 0)
        thisBall = &lastFrame->team0Balls[tempBallID];
      else
        thisBall = &lastFrame->team1Balls[tempBallID];

      //copy all data over...
      thisBall->location = tempBall.location;
      thisBall->rawLocation = tempBall.rawLocation;
      thisBall->velocity = tempBall.velocity;
      thisBall->found = tempBall.found;
     
      continue;
    }
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    //if we see a system tag, write down the play for that frame.
    if(strncmp(fileLine, "  <system", 9) == 0)
    {
      char* chunk; //arbitrary place we put chunks in from file

      //this splits off all info before the frame #
      chunk = strtok(fileLine, "=");

      //get play for that frame
      chunk = strtok(NULL,"'>\"");
     
      //store it =-)
      strcpy(lastFrame->systemMessages.play, chunk);
 
      continue;
    }
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    cout << "Hrmm, don't know what to do with: " << fileLine << endl;
    cout.flush();
  }    

  //close file of course =-)
  inputstream.close();
        
  currentFrameNumber = 0;
  playbackExists = true;

  initializeSequence();
  
  return true;
}

//######################################################################################
//######################################################################################
//######################################################################################

void writePlaybackSequence(char* fileName)
{
  //======================================================
  //create a file pointer
  FILE *playbackSequence;
  //======================================================
  //attempt to create the file based on the filename provided
  //if unsuccessful, bail out
  playbackSequence = fopen(fileName,"w");
  if (playbackSequence == NULL)
  {
    printf("Error creating playback sequence file :( \n");
    return;
  }
  //======================================================
  //write friendly message at the top that tells the user 
  //how to get the spec of brml =-)
  fprintf(playbackSequence, "###############################\n");
  fprintf(playbackSequence, "# This is a playback sequence #\n");
  fprintf(playbackSequence, "# Specification: <brml>       #\n");
  fprintf(playbackSequence, "# (Big Red Markup Language)   #\n");
  fprintf(playbackSequence, "#-----------------------------#\n");
  fprintf(playbackSequence, "# RoboCup Log Data:           #\n");
  fprintf(playbackSequence, "# => cropped by the editor    #\n");
  fprintf(playbackSequence, "#-----------------------------#\n");
  fprintf(playbackSequence, "# Please see specification at #\n");
  fprintf(playbackSequence, "# end of file.                #\n");
  fprintf(playbackSequence, "#    -Your Friendly Big Red   #\n");
  fprintf(playbackSequence, "#     RoboCup AI Members...   #\n");
  fprintf(playbackSequence, "###############################\n");
  //======================================================
  //file open, write header of file with specification for
  //the sequence
  fprintf(playbackSequence, "<brml>\n");
  fprintf(playbackSequence, " <specification ");
  fprintf(playbackSequence, " leftSideLine='");
  fprintf(playbackSequence, "%f'", playbackSequenceInfo.leftSideLine);
  fprintf(playbackSequence, " rightSideLine='");
  fprintf(playbackSequence, "%f'", playbackSequenceInfo.rightSideLine);
  fprintf(playbackSequence, " leftDefenseZone='");
  fprintf(playbackSequence, "%f'", playbackSequenceInfo.leftDefenseZone);
  fprintf(playbackSequence, " rightDefenseZone='");
  fprintf(playbackSequence, "%f'", playbackSequenceInfo.rightDefenseZone);
  fprintf(playbackSequence, " leftGoalPost='");
  fprintf(playbackSequence, "%f'", playbackSequenceInfo.leftGoalPost);
  fprintf(playbackSequence, " rightGoalPost='");
  fprintf(playbackSequence, "%f'", playbackSequenceInfo.rightGoalPost);
  fprintf(playbackSequence, " ourGoalieBox='");
  fprintf(playbackSequence, "%f'", playbackSequenceInfo.ourGoalieBox);
  fprintf(playbackSequence, " theirGoalieBox='");
  fprintf(playbackSequence, "%f'", playbackSequenceInfo.theirGoalieBox);
  fprintf(playbackSequence, " ourGoalLine='");
  fprintf(playbackSequence, "%f'", playbackSequenceInfo.ourGoalLine);
  fprintf(playbackSequence, " theirGoalLine='");
  fprintf(playbackSequence, "%f'", playbackSequenceInfo.theirGoalLine);
  fprintf(playbackSequence, " goalboxDepth='");
  fprintf(playbackSequence, "%f'", playbackSequenceInfo.goalboxDepth);
  fprintf(playbackSequence, " goalWidth='");
  fprintf(playbackSequence, "%f'", playbackSequenceInfo.goalWidth);
  fprintf(playbackSequence, " goalDepth='");
  fprintf(playbackSequence, "%f'", playbackSequenceInfo.goalDepth);
  fprintf(playbackSequence, " deathZoneLine='");
  fprintf(playbackSequence, "%f'", playbackSequenceInfo.defenseZoneLine);
  fprintf(playbackSequence, " leftGoalieBox='");
  fprintf(playbackSequence, "%f'", playbackSequenceInfo.leftGoalieBox);
  fprintf(playbackSequence, " rightGoalieBox='");
  fprintf(playbackSequence, "%f'", playbackSequenceInfo.rightGoalieBox);
  fprintf(playbackSequence, " goalieBoxDepth='");
  fprintf(playbackSequence, "%f'", playbackSequenceInfo.goalieBoxDepth);
  fprintf(playbackSequence, " defenseZoneLine='");
  fprintf(playbackSequence, "%f'", playbackSequenceInfo.defenseZoneLine);
  fprintf(playbackSequence, " offensiveZoneLine='");
  fprintf(playbackSequence, "%f'", playbackSequenceInfo.offensiveZoneLine);
  fprintf(playbackSequence, " killZoneLine='");
  fprintf(playbackSequence, "%f'", playbackSequenceInfo.killZoneLine);
  fprintf(playbackSequence, " leftMiddleSector='");
  fprintf(playbackSequence, "%f'", playbackSequenceInfo.leftMiddleSector);
  fprintf(playbackSequence, " rightMiddleSector='");
  fprintf(playbackSequence, "%f'", playbackSequenceInfo.rightMiddleSector);
  fprintf(playbackSequence, " leftSweetSector='");
  fprintf(playbackSequence, "%f'", playbackSequenceInfo.leftSweetSector);
  fprintf(playbackSequence, " rightSweetSector='");
  fprintf(playbackSequence, "%f'", playbackSequenceInfo.rightSweetSector);
  fprintf(playbackSequence, "robotsOnTeam0='");
  fprintf(playbackSequence, "%i", playbackSequenceInfo.numberOfRobotsOnTeam0);
  fprintf(playbackSequence, "' robotsOnTeam1='");
  fprintf(playbackSequence, "%i", playbackSequenceInfo.numberOfRobotsOnTeam1);
  fprintf(playbackSequence, "' ballsOnTeam0='");
  fprintf(playbackSequence, "%i", playbackSequenceInfo.numberOfTeam0Balls);
  fprintf(playbackSequence, "' ballsOnTeam1='");
  fprintf(playbackSequence, "%i", playbackSequenceInfo.numberOfTeam1Balls);
  fprintf(playbackSequence, "' thisTeam='%i'>\n",playbackSequenceInfo.thisTeam);
  //======================================================
  //write all frame data to file
  int dispFrameNum = -1;

  for(int frame=0; frame<playbackSequenceInfo.numberOfFrames; frame++)
  {
    cout << "attemping to write frame #" << frame << "...";
	  
	bool skipThisFrame = false;
	crop* tempCrop = firstCrop;
	while(tempCrop)
	{
		if(tempCrop->leftFrame <= frame && tempCrop->rightFrame >= frame)
        {
			skipThisFrame = true;
			break;
		}

		tempCrop = tempCrop->nextCrop;
    }

    //if this frame has been cut, then skip it!
    if(skipThisFrame)
	{
        cout << "skipped...\n";
		continue;
	}

    cout << "used \n";
	dispFrameNum++;

    //beginning of frame
    fprintf(playbackSequence, "########################\n");
    fprintf(playbackSequence, " <frame number='");
    fprintf(playbackSequence, "%i", dispFrameNum);
    fprintf(playbackSequence, "'>\n");

    for(int robot=0; robot<playbackSequenceInfo.numberOfRobotsOnTeam0; robot++)
    {
      fprintf(playbackSequence, "  <robot team='");       
      fprintf(playbackSequence, "%i", 0);
      fprintf(playbackSequence, "' id='");
      fprintf(playbackSequence, "%i", robot);
      fprintf(playbackSequence, "' x='");
      fprintf(playbackSequence, "%1.3f", framePointers[frame]->team0Robots[robot].location.x);
      fprintf(playbackSequence, "' y='");
      fprintf(playbackSequence, "%1.3f", framePointers[frame]->team0Robots[robot].location.y);
      fprintf(playbackSequence, "' rot='");
      fprintf(playbackSequence, "%1.3f", framePointers[frame]->team0Robots[robot].rotation);
      fprintf(playbackSequence, "' xVel='");
      fprintf(playbackSequence, "%1.3f", framePointers[frame]->team0Robots[robot].velocity.x);
      fprintf(playbackSequence, "' yVel='");
      fprintf(playbackSequence, "%1.3f", framePointers[frame]->team0Robots[robot].velocity.y);
      fprintf(playbackSequence, "' rotVel='");
      fprintf(playbackSequence, "%1.3f", framePointers[frame]->team0Robots[robot].rotationVelocity);

      if(framePointers[frame]->team0Robots[robot].destination.x != -32000)
      {
	      fprintf(playbackSequence, "' destX='");
        fprintf(playbackSequence, "%1.3f", framePointers[frame]->team0Robots[robot].destination.x);
        fprintf(playbackSequence, "' destY='");
        fprintf(playbackSequence, "%1.3f", framePointers[frame]->team0Robots[robot].destination.y);
        fprintf(playbackSequence, "' destRot='");
        fprintf(playbackSequence, "%1.3f", framePointers[frame]->team0Robots[robot].destinationRot);
        fprintf(playbackSequence, "' passToX='");
        fprintf(playbackSequence, "%1.3f", framePointers[frame]->team0Robots[robot].passToLocation.x);
        fprintf(playbackSequence, "' passToY='");
        fprintf(playbackSequence, "%1.3f", framePointers[frame]->team0Robots[robot].passToLocation.y);
        fprintf(playbackSequence, "' passToMe='");
        fprintf(playbackSequence, "%i", framePointers[frame]->team0Robots[robot].passToMe);
        fprintf(playbackSequence, "' found='");
        fprintf(playbackSequence, "%i", framePointers[frame]->team0Robots[robot].found);
        fprintf(playbackSequence, "' kick='");
        fprintf(playbackSequence, "%i", framePointers[frame]->team0Robots[robot].kick);
        fprintf(playbackSequence, "' dribble='");
        fprintf(playbackSequence, "%i", framePointers[frame]->team0Robots[robot].dribble);
        fprintf(playbackSequence, "' hasPossession='");
        fprintf(playbackSequence, "%i", framePointers[frame]->team0Robots[robot].hasPossession);
        fprintf(playbackSequence, "' position='");    
        fprintf(playbackSequence, "%s", framePointers[frame]->team0Robots[robot].position);
        fprintf(playbackSequence, "' debugMessage='");
        fprintf(playbackSequence, "%s", framePointers[frame]->team0Robots[robot].debugMessage);    
      }
      fprintf(playbackSequence, "'>\n");
	  }
	  
    for(robot=0; robot<playbackSequenceInfo.numberOfRobotsOnTeam1; robot++)
    {
      fprintf(playbackSequence, "  <robot team='");       
      fprintf(playbackSequence, "%i", 1);
      fprintf(playbackSequence, "' id='");
      fprintf(playbackSequence, "%i", robot);
      fprintf(playbackSequence, "' x='");
      fprintf(playbackSequence, "%1.3f", framePointers[frame]->team1Robots[robot].location.x);
      fprintf(playbackSequence, "' y='");
      fprintf(playbackSequence, "%1.3f", framePointers[frame]->team1Robots[robot].location.y);
      fprintf(playbackSequence, "' rot='");
      fprintf(playbackSequence, "%1.3f", framePointers[frame]->team1Robots[robot].rotation);
      fprintf(playbackSequence, "' xVel='");
      fprintf(playbackSequence, "%1.3f", framePointers[frame]->team1Robots[robot].velocity.x);
      fprintf(playbackSequence, "' yVel='");
      fprintf(playbackSequence, "%1.3f", framePointers[frame]->team1Robots[robot].velocity.y);
      fprintf(playbackSequence, "' rotVel='");
      fprintf(playbackSequence, "%1.3f", framePointers[frame]->team1Robots[robot].rotationVelocity);

      if(framePointers[frame]->team1Robots[robot].destination.x != -32000)
      {
	      fprintf(playbackSequence, "' destX='");
        fprintf(playbackSequence, "%1.3f", framePointers[frame]->team1Robots[robot].destination.x);
        fprintf(playbackSequence, "' destY='");
        fprintf(playbackSequence, "%1.3f", framePointers[frame]->team1Robots[robot].destination.y);
        fprintf(playbackSequence, "' destRot='");
        fprintf(playbackSequence, "%1.3f", framePointers[frame]->team1Robots[robot].destinationRot);
    	  fprintf(playbackSequence, "' passToX='");
        fprintf(playbackSequence, "%1.3f", framePointers[frame]->team1Robots[robot].passToLocation.x);
        fprintf(playbackSequence, "' passToY='");
        fprintf(playbackSequence, "%1.3f", framePointers[frame]->team1Robots[robot].passToLocation.y);
        fprintf(playbackSequence, "' passToMe='");
        fprintf(playbackSequence, "%i", framePointers[frame]->team1Robots[robot].passToMe);
        fprintf(playbackSequence, "' found='");
        fprintf(playbackSequence, "%i", framePointers[frame]->team1Robots[robot].found);
        fprintf(playbackSequence, "' kick='");
        fprintf(playbackSequence, "%i", framePointers[frame]->team1Robots[robot].kick);
        fprintf(playbackSequence, "' dribble='");
        fprintf(playbackSequence, "%i", framePointers[frame]->team1Robots[robot].dribble);
        fprintf(playbackSequence, "' position='");    
        fprintf(playbackSequence, "%s", framePointers[frame]->team1Robots[robot].position);
        fprintf(playbackSequence, "' debugMessage='");
        fprintf(playbackSequence, "%s", framePointers[frame]->team1Robots[robot].debugMessage);    
      }
      fprintf(playbackSequence, "'>\n");
    }
    
    //our one pathetic lonely robocup ball
    fprintf(playbackSequence, "  #----BALL DATA----\n");
    for(int ball=0; ball<playbackSequenceInfo.numberOfTeam0Balls; ball++)
    {
	    fprintf(playbackSequence, "  <ball id='");
      fprintf(playbackSequence, "%i' team='", ball); 
      fprintf(playbackSequence, "%i", 0);
      fprintf(playbackSequence, "' x='"); 
      fprintf(playbackSequence, "%1.3f", framePointers[frame]->team0Balls[ball].location.x);
      fprintf(playbackSequence, "' y='"); 
      fprintf(playbackSequence, "%1.3f", framePointers[frame]->team0Balls[ball].location.y);
      fprintf(playbackSequence, "' xVel='"); 
      fprintf(playbackSequence, "%1.3f", framePointers[frame]->team0Balls[ball].velocity.x);
      fprintf(playbackSequence, "' yVel='"); 
      fprintf(playbackSequence, "%1.3f", framePointers[frame]->team0Balls[ball].velocity.y);
      fprintf(playbackSequence, "' found='");     
      fprintf(playbackSequence, "%i", framePointers[frame]->team0Balls[ball].found);
      fprintf(playbackSequence, "'>\n");
    }
    for(ball=0; ball<playbackSequenceInfo.numberOfTeam1Balls; ball++)
    {
	    fprintf(playbackSequence, "  <ball id='");
      fprintf(playbackSequence, "%i' team='", ball); 
      fprintf(playbackSequence, "%i", 1);
      fprintf(playbackSequence, "' x='"); 
      fprintf(playbackSequence, "%1.3f", framePointers[frame]->team1Balls[ball].location.x);
      fprintf(playbackSequence, "' y='"); 
      fprintf(playbackSequence, "%1.3f", framePointers[frame]->team1Balls[ball].location.y);
      fprintf(playbackSequence, "' xVel='"); 
      fprintf(playbackSequence, "%1.3f", framePointers[frame]->team1Balls[ball].velocity.x);
      fprintf(playbackSequence, "' yVel='"); 
      fprintf(playbackSequence, "%1.3f", framePointers[frame]->team1Balls[ball].velocity.y);
      fprintf(playbackSequence, "' found='");     
      fprintf(playbackSequence, "%i", framePointers[frame]->team1Balls[ball].found);
      fprintf(playbackSequence, "'>\n");
    }

    //system frame info
    fprintf(playbackSequence, "  #----SYSTEM FRAME INFO----\n");
    fprintf(playbackSequence, "  <system play='");
    fprintf(playbackSequence, "%s", framePointers[frame]->systemMessages.play);
    fprintf(playbackSequence, "'>\n");

    //end the frame
    fprintf(playbackSequence, " </frame>\n");
  }

  //after all the frames put one last frame delimeter
  fprintf(playbackSequence, "########################\n");
  
   //======================================================
  //write end of file, closing the playbacksequence
  fprintf(playbackSequence, "</brml>\n");
      
  //======================================================
  //write the spec for brml
  fprintf(playbackSequence, "################################\n");
  fprintf(playbackSequence, "# BRML is easy. Put a # sign   #\n");
  fprintf(playbackSequence, "# at the beginning of a line,  #\n");
  fprintf(playbackSequence, "# and all characters after it  #\n");
  fprintf(playbackSequence, "# are ignored. Just make sure  #\n");
  fprintf(playbackSequence, "# # is the first character :)  #\n");
  fprintf(playbackSequence, "#                              #\n");
  fprintf(playbackSequence, "# When making a tag, write all #\n");
  fprintf(playbackSequence, "# flags on the same line!      #\n");
  fprintf(playbackSequence, "#                              #\n");
  fprintf(playbackSequence, "# BRML is actually written for #\n");
  fprintf(playbackSequence, "# you, so this should be good  #\n");
  fprintf(playbackSequence, "# enough for now. =-)          #\n");
  fprintf(playbackSequence, "################################\n");
  //======================================================
  //close file :)
  fclose(playbackSequence);
  //======================================================
}



//######################################################################################
//######################################################################################
//######################################################################################

//################################
//Reads in a playbacksequence from
//the specified filename
//################################
bool appendSequence(char* filename)
{
  //-----------------------------  
  //attempt to find and load file
  //-----------------------------

  ifstream inputstream(filename);
  if(!inputstream.is_open())
  {
    cout << "ERROR! Unable to find or open " << filename << "!\n";
	cout.flush();
	return false;
  }
  
  //-----------------------------
  //ok, found file, read contents :)
  //-----------------------------
  cout << "-Successfully opened " << filename << "\n";
  cout.flush();
  
  //the longest any line in the file can be.
  const int maxCharacters = 1024;

  //this will hold the contents of the file as we
  //read it line by line
  char fileLine[maxCharacters];

  bool error = false;

  while(inputstream && !error)
  {
    //get the next line of the file
    inputstream.getline(fileLine, maxCharacters);

    //set true below if we find first character is a #
    bool comment_out = false;

    //if first character is a # symbol, skip line
    for (int i=0; i< maxCharacters; i++)
    {  
      //if we hit a #, comment out
      if(fileLine[i] == '#')
      {
        comment_out = true;
        break;
      }
 
      //if we hit a space, keep searching...
      else if(fileLine[i] == ' ')
        continue;
  
      //else, nonwhitespace character so don't comment out
      break;
    }

    //if line commented out, loop and get next line
    if(comment_out)
      continue;
 
    //ok, line not commented out. depending
    //on what kind of tag we have we parse it differently
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    //this is more symbolic than anything, always
    //begin a file with this tag
    if(strcmp(fileLine, "<brml>") == 0)
      continue;
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    //once the end tag is seen, the parser quits.
    if(strcmp(fileLine, "</brml>") == 0)
    {
      cout << "-End of stream found. Quiting parser.\n";
   	  cout.flush();
      break;
    }
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    //if we find the specification tag, parse it and
    //not only set the spec, but also intialize the sequence
    if(strncmp(fileLine, " <specification", 15) == 0)
    {
      cout << "-Specification found.\n";
   	  cout.flush();
      
      char* chunk; //arbitrary place we put chunks in from file
      char* tag; //name of tag, like frameRate

      //we have a twostep process, read
      //tag, then value, then tag, then value...
      //in order to parse properly, we need to
      //know which one we are looking for and thus how
      //to grab next chunk using which delimeters...

      //we always start by reading a tag
      bool readingTag = true;
   
      //do the initial strtok call
      //this grabes the tag name, in this case, 
      //"specification"
      //Subsequent calls will
      //return good chunks =-)
      chunk = strtok(fileLine, "<> ='");

      //get first tag
      chunk = strtok(NULL,"<> ='");
     
      //while we are still parsing the spec...
      while(chunk !=NULL)
      {
        //if we have a value, then store it in the permanent location
        //else, store chunk value, which is a tag
        if(readingTag)
        {
          //save tag 
          tag = chunk;
 
          //get next chunk
          chunk = strtok(NULL,"='>");
        }

        else
        {
          //store value associate with tag
          //====================================
          if(strcmp(tag, "frameRate") == 0)
          {
			  if(playbackSequenceInfo.frameRate != (float)atof(chunk))
				  error = true;
		  }
          //====================================
          else if(strcmp(tag, "robotsOnTeam0") == 0)
          {
			  if(playbackSequenceInfo.numberOfRobotsOnTeam0 != (float)atof(chunk))
				  error = true;
		  }
          //====================================
          else if(strcmp(tag, "robotsOnTeam1") == 0)
          {
			  if(playbackSequenceInfo.numberOfRobotsOnTeam1 != (float)atof(chunk))
				  error = true;
		  }
          //====================================
          else if(strcmp(tag, "ballsOnTeam0") == 0)
          {
			  if(playbackSequenceInfo.numberOfTeam0Balls != (float)atof(chunk))
				  error = true;
		  }
          //====================================
          else if(strcmp(tag, "ballsOnTeam1") == 0)
          {
			  if(playbackSequenceInfo.numberOfTeam1Balls != (float)atof(chunk))
				  error = true;
		  }
          //====================================
          else if(strcmp(tag, "thisTeam") == 0)
          {
			  if(playbackSequenceInfo.thisTeam != (float)atof(chunk))
				  error = true;
		  }
          //====================================
          //get next chunk
          chunk = strtok(NULL," ='");  
        }

        //next value will be opposite
        readingTag = !readingTag;
      }      
      continue;
	}
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    //if we see a frame tag, create a new frame and insert it at the end of the list
    if(strncmp(fileLine, " <frame", 7) == 0)
    {
		frame* newFrame = new frame();
		initializeFrame(newFrame);
		if(!firstFrame)
		{
			firstFrame = newFrame;
			lastFrame = newFrame;
		}
		else
		{
			lastFrame->nextFrame = newFrame;
			lastFrame = newFrame;
		}
		numFrames++;	
        cout << "appending frame #" << numFrames << "\n";
		cout.flush();
		continue;
	}
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    //this is more symbolic than anything, always
    //end a frame with this tag
    if(strcmp(fileLine, " </frame>") == 0)
      continue;
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    //if we find a robot tag, parse it
    if(strncmp(fileLine, "  <robot", 8) == 0)
    {
      char* chunk; //arbitrary place we put chunks in from file
      char* tag; //name of tag, like frameRate

      //we have a twostep process, read
      //tag, then value, then tag, then value..
      //in order to parse properly, we need t
      //know which one we are looking for and thus how
      //to grab next chunk using which delimeters...
      //we always start by reading a tag
      bool readingTag = true;
   
      //we don't know which order tags will come so write down
      //the robot id and team number in temp ints, and the
      //rest of the values in a temp robot. then copy over
      //to real data structure at very end.
      int tempRobotID;
      int tempTeamNumber;
      robot tempRobot;

  	  tempRobot.location.x = 0.0f;
      tempRobot.location.y = 0.0f;
  	  tempRobot.rawLocation.x = 0.0f;
      tempRobot.rawLocation.y = 0.0f;
      tempRobot.velocity.x = 0.0f;
      tempRobot.velocity.y = 0.0f;
      tempRobot.rotation = 0.0f;
      tempRobot.rawRotation = 0.0f;
      tempRobot.rotationVelocity = 0.0f;
      tempRobot.destination.x = -32000.0f;
      tempRobot.destination.y = -32000.0f;
      tempRobot.destinationRot = -32000.0f;
      tempRobot.passToLocation.x = -32000.0f;
      tempRobot.passToLocation.y = -32000.0f;
      tempRobot.passToMe = false;
      tempRobot.found = false;
      tempRobot.kick = false;
      tempRobot.dribble = false;
      strcpy(tempRobot.position, "Unspecified");
      strcpy(tempRobot.debugMessage, "Robot data not set from file");

      //do the initial strtok call
      //this grabes the tag name, in this case, 
      //"robot"
      //Subsequent calls will
      //return good chunks =-)
      chunk = strtok(fileLine, "<> ='");

      //get first tag
      chunk = strtok(NULL,"<> ='");
     
      //while we are still parsing the spec...
      while(chunk !=NULL)
      {
        //if we have a value, then store it in the permanent location
        //else, store chunk value, which is a tag
        if(readingTag)
        {
          //save tag 
          tag = chunk;
 
          //get next chunk
          chunk = strtok(NULL,"'");
        }

        else
        {
          //store value associate with tag
          //====================================
          if(strcmp(tag, "team") == 0)
            tempTeamNumber = atoi(chunk);
          //====================================
          else if(strcmp(tag, "id") == 0)
            tempRobotID = atoi(chunk);
          //====================================
          else if(strcmp(tag, "x") == 0)
            tempRobot.location.x = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "y") == 0)
            tempRobot.location.y = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "rawX") == 0)
            tempRobot.rawLocation.x = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "rawY") == 0)
            tempRobot.rawLocation.y = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "rawRot") == 0)
            tempRobot.rawRotation = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "xVel") == 0)
            tempRobot.velocity.x = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "yVel") == 0)
            tempRobot.velocity.y = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "rotVel") == 0)
            tempRobot.rotationVelocity = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "destX") == 0)
            tempRobot.destination.x = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "destY") == 0)
            tempRobot.destination.y = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "destRot") == 0)
            tempRobot.destinationRot = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "passToX") == 0)
            tempRobot.passToLocation.x = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "passToY") == 0)
            tempRobot.passToLocation.y = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "passToMe") == 0)
            tempRobot.passToMe = (atoi(chunk) != 0);
          //====================================
          else if(strcmp(tag, "found") == 0)
            tempRobot.found = (atoi(chunk) != 0);
          //====================================
          else if(strcmp(tag, "kick") == 0)
            tempRobot.kick = (atoi(chunk) != 0);
          //====================================
          else if(strcmp(tag, "dribble") == 0)
            tempRobot.dribble = (atoi(chunk) != 0);
          //====================================
          else if(strcmp(tag, "position") == 0)
            strcpy(tempRobot.position, chunk);
          //====================================
          else if(strcmp(tag, "debugMessage") == 0)
            strcpy(tempRobot.debugMessage, chunk);
          //====================================

          //get next chunk
          chunk = strtok(NULL," ='");  
        }

        //next value will be opposite
        readingTag = !readingTag;
      }      

      //finished reading in robot
      //write into into arrays...

      //get a pointer to this robot =-)
      robot* thisRobot;          
      if(tempTeamNumber == 0)
        thisRobot = &lastFrame->team0Robots[tempRobotID];
      else
        thisRobot = &lastFrame->team1Robots[tempRobotID];

      //copy all data over...
      memcpy(thisRobot, &tempRobot, sizeof(robot));
     
      continue;
    }
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    //if we find a ball tag, parse it
    if(strncmp(fileLine, "  <ball", 7) == 0)
    {
      char* chunk; //arbitrary place we put chunks in from file
      char* tag; //name of tag, like frameRate

      //we have a twostep process, read
      //tag, then value, then tag, then value...
      //in order to parse properly, we need to
      //know which one we are looking for and thus how
      //to grab next chunk using which delimeters...

      //we always start by reading a tag
      bool readingTag = true;
   
      //we don't know which order tags will come so write down
      //the ball id and team number in temp ints, and the
      //rest of the values in a temp ball. then copy over
      //to real data structure at very end.
      int tempBallID;
      int tempTeamNumber;
      ball tempBall;

      //do the initial strtok call
      //this grabes the tag name, in this case, 
      //"ball"
      //Subsequent calls will
      //return good chunks =-)
      chunk = strtok(fileLine, "<> ='");

      //get first tag
      chunk = strtok(NULL,"<> ='");
     
      //while we are still parsing the spec...
      while(chunk !=NULL)
      {
        //if we have a value, then store it in the permanent location
        //else, store chunk value, which is a tag
        if(readingTag)
        {
          //save tag 
          tag = chunk;
 
          //get next chunk
          chunk = strtok(NULL,"'");
        }

        else
        {
          //store value associate with tag
          //====================================
          if(strcmp(tag, "team") == 0)
            tempTeamNumber = atoi(chunk);          
          //====================================
          else if(strcmp(tag, "id") == 0)
            tempBallID = atoi(chunk);
          //====================================
          else if(strcmp(tag, "x") == 0)
            tempBall.location.x = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "y") == 0)
            tempBall.location.y = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "rawX") == 0)
            tempBall.rawLocation.x = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "rawY") == 0)
            tempBall.rawLocation.y = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "xVel") == 0)
            tempBall.velocity.x = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "yVel") == 0)
            tempBall.velocity.y = (float)atof(chunk);
          //====================================
          else if(strcmp(tag, "found") == 0)
            tempBall.found = (atoi(chunk) != 0);
          //====================================

          //get next chunk
          chunk = strtok(NULL," ='");  
        }

        //next value will be opposite
        readingTag = !readingTag;
      }      

      //finished reading in ball
      //write into into arrays...

      //get a pointer to this ball =-)
      ball* thisBall;          
      if(tempTeamNumber == 0)
        thisBall = &lastFrame->team0Balls[tempBallID];
      else
        thisBall = &lastFrame->team1Balls[tempBallID];

      //copy all data over...
      thisBall->location = tempBall.location;
      thisBall->rawLocation = tempBall.rawLocation;
      thisBall->velocity = tempBall.velocity;
      thisBall->found = tempBall.found;
     
      continue;
    }
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    //if we see a system tag, write down the play for that frame.
    if(strncmp(fileLine, "  <system", 9) == 0)
    {
      char* chunk; //arbitrary place we put chunks in from file

      //this splits off all info before the frame #
      chunk = strtok(fileLine, "=");

      //get play for that frame
      chunk = strtok(NULL,"'>");
     
      //store it =-)
      strcpy(lastFrame->systemMessages.play, chunk);
 
      continue;
    }
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    //--------------------------------------------
    cout << "Hrmm, don't know what to do with: " << fileLine << endl;
    cout.flush();
  }    

  //close file of course =-)
  inputstream.close();
        
  currentFrameNumber = 0;
  playbackExists = true;

  initializeSequence();
  
  return true;
}

//######################################################################################
//######################################################################################
//######################################################################################
