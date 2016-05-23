
#ifndef FILE_READER_DEFINITION_H
#define FILE_READER_DEFINITION_H

//##############################
//Stores into about playback
//sequence stored at top of file
//##############################
struct playbackSpec
{
  int numberOfFrames;
  float frameRate; 
  int numberOfRobotsOnTeam0, numberOfRobotsOnTeam1; 
  int numberOfTeam0Balls, numberOfTeam1Balls;
  int thisTeam, otherTeam;
  float leftSideLine;
  float rightSideLine;
  float leftDefenseZone;
  float rightDefenseZone;
  float leftGoalPost;
  float rightGoalPost;
  float ourGoalieBox;
  float theirGoalieBox;
  float ourGoalLine;
  float theirGoalLine;
  float goalboxDepth;
  float goalWidth;
  float goalDepth;
  float leftGoalieBox;
  float rightGoalieBox;
  float goalieBoxDepth;
  float deathZoneLine;
  float defenseZoneLine;
  float offensiveZoneLine;
  float killZoneLine;
  float leftMiddleSector;
  float rightMiddleSector;
  float leftSweetSector;
  float rightSweetSector;
};

//##############################
//Stores two floats, usually
//for a location or velocity
//##############################
struct pair
{
  float x;
  float y;
};

//##############################
//Stores all the info regarding
//one robot at any time
//##############################
struct robot
{
  pair location, velocity, rawLocation;
  float rotation, rotationVelocity, rawRotation;
  pair destination, passToLocation;
  float destinationRot;
  bool passToMe, found, kick, dribble, hasPossession;
  char position[80]; //aka "Special Op Aggressor"
  char debugMessage[80]; //aka "help me! where is the ball!"
};

//##############################
//Stores all the info regarding 
//one ball at any time
//##############################
struct ball
{
  pair location, velocity, rawLocation;
  bool found;
};

//##############################
//Store system info for a single
//frame.
//##############################
struct systemInfo
{
  char play[80]; //aka "Single Assist Offense"
};

//##############################
//Stoers a single frame worth of
//data.
//##############################
struct frame
{
  robot* team0Robots;
  robot* team1Robots;
  ball* team0Balls;
  ball* team1Balls;
  systemInfo systemMessages;
  frame* nextFrame;	
};

//##################################
//Stores left and right endpoints of
//a crop, and a pointer to the next
//cropped section in the linked list
//##################################
struct crop
{
	int leftFrame;
	int rightFrame;
	crop* nextCrop;
};
//--------------------------------------------------------

//################################
//Reads in a playbacksequence from
//the specified filename
//################################
bool readSequence(char* filename);

//saves an edited playback file
void writePlaybackSequence(char* fileName);

//appends more frames to the current playback
bool appendSequence(char* filename);

//#################################
//Destroy information when all done
//#################################
void destructSequence();

//###############################
//Create gigantic array of frame
//data and initializes it all =-)
//###############################
void initializeSequence();

//###############################
//Initialized all data in a frame
//###############################
void initializeFrame(frame* aFrame);

//--------------------------------------------------------
extern playbackSpec playbackSequenceInfo;
extern frame* firstFrame;
extern frame* lastFrame;
extern frame** framePointers;
extern bool playbackExists;
extern int tempCropLeft;
extern int tempCropRight;
extern crop* firstCrop;
extern crop* lastCrop;
extern int currentFrameNumber;
extern int numFrames;
//--------------------------------------------------------

#endif //MAIN_DEFINITION_H
