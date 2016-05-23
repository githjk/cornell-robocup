//----------------------------------------------------
//What the heck is this? To make sure
//the GUI cannot modify system variables (like
//a robots assigned position during the middle of a 
//play, we write all GUI commands to a diary, and 
//the main loop looks at this diary at a specific time.
//----------------------------------------------------
//so we know how many robots exist...
#include "ai.h"
#include "plays/plays.h"

#ifndef GUI_Skillz_H
#define GUI_Skillz_H

//If GUI does not set a value...
#define notSet -1


//number of debug points to allocate per robot
#define NUM_DEBUG_POINTS 20

class GUI_Debug_Message
{
public:
  GUI_Debug_Message(char* message);
  
  void setNextMessage(GUI_Debug_Message* theNextMessage);
  
  #define MAX_DEBUG_MESSAGE_LENGTH 400
  char thisMessage[MAX_DEBUG_MESSAGE_LENGTH];
  GUI_Debug_Message* nextMessage;
};

class debug
{
public:
  debug();

  ~debug() {}
  
  void reset();
  
  void syncPoints();
  
  float getDebugPointX(int robotID, int debugPointNumber);
  
  float getDebugPointY(int robotID, int debugPointNumber);
  
  void setDebugPoint(int robotID, int debugPointNumber, Pair pos);
  
  void setDebugPoint(int robotID, int debugPointNumber, float xPos, float yPos);
  
  void addDebugMessage(char* newMessage);
  
  int getNumDebugMessages();
  
  char* popDebugMessage();
  
private:
  Pair realDebugPoints[NUM_ROBOTS*NUM_DEBUG_POINTS];
  Pair workingDebugPoints[NUM_ROBOTS*NUM_DEBUG_POINTS];
  
  GUI_Debug_Message* firstDebugMessage;
  GUI_Debug_Message* lastDebugMessage;
  int numDebugMessages;
  
};



class GUI_Diary
{
public:
  
  GUI_Diary();
  
  void reset();
  
  bool newPlaySet();
  
  //----------
  //actual data
  //----------
  //assign a new play?
  int newPlay;
  //assign a new position to a robot?
  int newPosition[NUM_ROBOTS];
  bool toggleAI, toggleVision, toggleWireless, toggleSimulator;
  bool toggleSides, toggleTeam, sampleFrameRate;
  
  debug debuggingInfo;
};    

extern char debugMessageString[400];


#endif //GUI_Skillz_H
