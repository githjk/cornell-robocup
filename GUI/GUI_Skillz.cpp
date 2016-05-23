#include "GUI_Skillz.h"
#include <string>
//#include "plays/playbook.h"


#define MAX_DEBUG_MESSAGES 100

GUI_Debug_Message::GUI_Debug_Message(char* message)
{
  strcpy(thisMessage, message);
  nextMessage = NULL;
}

void GUI_Debug_Message::setNextMessage(GUI_Debug_Message* theNextMessage)
{
  nextMessage = theNextMessage;
}


debug::debug()
{
  reset();
  //reset manual placement points
  for(int i=0; i<NUM_ROBOTS; i++)
  {
    for(int j=0; j<NUM_DEBUG_POINTS; j++)
    {
      realDebugPoints[i*NUM_DEBUG_POINTS + j].set(-32000.0f, -32000.0f);
    }
  }  

  //reset dialog debug messages
  firstDebugMessage = NULL;
  lastDebugMessage = NULL;
  numDebugMessages = 0;
}

void debug::reset()
{
  //reset manual placement points
  for(int i=0; i<NUM_ROBOTS; i++)
  {
    for(int j=0; j<NUM_DEBUG_POINTS; j++)
    {
      workingDebugPoints[i*NUM_DEBUG_POINTS + j].set(-32000.0f, -32000.0f);
    }
  }  
}

void debug::syncPoints() 
{
  for(int i=0; i<NUM_ROBOTS; i++)
  {
    for(int j=0; j<NUM_DEBUG_POINTS; j++)
    {
      realDebugPoints[i*NUM_DEBUG_POINTS + j].set(
        workingDebugPoints[i*NUM_DEBUG_POINTS + j]
        );    
    }
  }
}

float debug::getDebugPointX(int robotID, int debugPointNumber)
{
  if( 
    robotID >= 0 &&
    robotID < NUM_ROBOTS &&
    debugPointNumber >= 0 &&
    debugPointNumber < NUM_DEBUG_POINTS
    )
  {
    return realDebugPoints[robotID*NUM_DEBUG_POINTS + debugPointNumber].getX();
  }
  else
  {
    return -32000.0f;
  }
}

float debug::getDebugPointY(int robotID, int debugPointNumber)
{
  if( 
    robotID >= 0 &&
    robotID < NUM_ROBOTS &&
    debugPointNumber >= 0 &&
    debugPointNumber < NUM_DEBUG_POINTS
    )
  {
    return realDebugPoints[robotID*NUM_DEBUG_POINTS + debugPointNumber].getY();
  }
  else
  {
    return -32000.0f;
  }
}

void debug::setDebugPoint(int robotID, int debugPointNumber, Pair pos)
{
  if( 
    robotID >= 0 &&
    robotID < NUM_ROBOTS &&
    debugPointNumber >= 0 &&
    debugPointNumber < NUM_DEBUG_POINTS
    )
  {
    workingDebugPoints[robotID*NUM_DEBUG_POINTS + debugPointNumber].set(pos);
  }
}

void debug::setDebugPoint(int robotID, int debugPointNumber, float xPos, float yPos)
{
  if( 
    robotID >= 0 &&
    robotID < NUM_ROBOTS &&
    debugPointNumber >= 0 &&
    debugPointNumber < NUM_DEBUG_POINTS
    )
  {
    workingDebugPoints[robotID*NUM_DEBUG_POINTS + debugPointNumber].setX(xPos);
    workingDebugPoints[robotID*NUM_DEBUG_POINTS + debugPointNumber].setY(yPos);
  }
}

void debug::addDebugMessage(char* newMessage)
{
  if(numDebugMessages < MAX_DEBUG_MESSAGES)
  {
    if(firstDebugMessage == NULL)
    {
      firstDebugMessage = new GUI_Debug_Message(newMessage);
      lastDebugMessage = firstDebugMessage;
    }
    else
    {
      GUI_Debug_Message* temp = new GUI_Debug_Message(newMessage);
      if(temp != NULL)
      {
        lastDebugMessage->setNextMessage(temp);
        lastDebugMessage = temp;
        
        //if while this was going on the head was deleted, set the head to this node
        if(firstDebugMessage == NULL)
          firstDebugMessage = lastDebugMessage;
      }
    }
    numDebugMessages++;
  }
}

int debug::getNumDebugMessages()
{
  return numDebugMessages;
}

char* debug::popDebugMessage()
{

  if(numDebugMessages <= 0)
    return "no messages...";
  else
  {
    numDebugMessages--;
    
    strcpy(debugMessageString,firstDebugMessage->thisMessage);

    GUI_Debug_Message* temp = firstDebugMessage;
    firstDebugMessage = firstDebugMessage->nextMessage;
    delete temp;
    temp = NULL;

    return debugMessageString;
  }   
}



GUI_Diary::GUI_Diary()
{
  reset(); //reset all values to nonSet
  
  debuggingInfo.reset();
}

void GUI_Diary::reset()
{
  newPlay = -1;
  
  for(RobotIndex ID=ROBOT0; ID<NUM_ROBOTS; ID++)
    newPosition[ID] = notSet;
  
  toggleAI = false;
  toggleVision = false;
  toggleWireless = false;
  toggleSimulator = false;
  toggleSides = false;
  toggleTeam = false;
  sampleFrameRate = false;
}

bool GUI_Diary::newPlaySet()
{
  return (newPlay != -1);
}

char debugMessageString[400];

