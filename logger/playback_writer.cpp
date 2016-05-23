#include <iostream.h>
#include <fstream.h>
#include <stdio.h>
#include <string>
#include "playback_writer.h"
#include <time.h>
#include "common/datatypes/robocup_strategy_data.h"

void writePlaybackSequence(char* alternateFileName, 
                           bool fileNameIsDate)
{

  //-------------------------------  
  //Get actual system time and date
  //-------------------------------  
  struct tm *newtime;
  time_t aclock;
  time( &aclock );                   //get time in seconds
  newtime = localtime( &aclock );    //convert to tm form 
  //------------------------------   
  //Get the year as a 4-digit #
  //------------------------------   
  int year = newtime->tm_year + 1900;
  //------------------------------   
  //Get the month as a 2-digit #
  //------------------------------   
  int mn = newtime->tm_mon + 1;
  char month[3];
  if(mn < 10)
    sprintf(month, "%c%d", '0', mn);
  else
    sprintf(month, "%d", mn);
  //------------------------------   
  //Get the day as a 2-digit #
  //------------------------------   
  int dy = newtime->tm_mday;
  char day[3];
  if(dy < 10)
    sprintf(day, "%c%d", '0', dy);    
  else
    sprintf(day, "%d", dy);
  //------------------------------   
  //Get the hour as a 2-digit #
  //------------------------------   
  int hr = newtime->tm_hour;
  char hour[3];
  if(hr < 10)
    sprintf(hour, "%c%d", '0', hr);    
  else
    sprintf(hour, "%d", hr);
  //------------------------------   
  //Get the minute as a 2-digit #
  //------------------------------   
  int mi = newtime->tm_min;
  char minute[3];
  if(mi < 10)
    sprintf(minute, "%c%d", '0', mi);    
  else
    sprintf(minute, "%d", mi);
  //------------------------------   
  //Get the second as a 2-digit #
  //------------------------------   
  int se = newtime->tm_sec;
  char second[3];
  if(se < 10)
    sprintf(second, "%c%d", '0', se);    
  else
    sprintf(second, "%d", se);
  //------------------------------   
  //A filename might look like this:
  //RoboCup_Log_Data_2001_04_07_at_05:23:17.txt
  //If you count it, there are always 43 characters in the filename
  char fileName[100];

  //if we want the filename to just be the default time+date
  if(fileNameIsDate)
  {
    sprintf(fileName, "playback_log_%d_%s_%s_at_%s_%s_%s.brml",
            year,
            month,
            day,
            hour,
            minute,
            second);    
  }
  //else if we are specifying the filename, just copy this over
  else
  {
    sprintf(fileName, "%s", alternateFileName);
  }

  
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
  //file open, write header of file with specification for
  //the sequence
  int numberOfFrames = sm->ReturnFrame() + 1;
  //======================================================
  fprintf(playbackSequence, "<brml>\n");
  //======================================================
  //write friendly message at the top that tells the user 
  //how to get the spec of brml =-)
  fprintf(playbackSequence, "###############################\n");
  fprintf(playbackSequence, "# This is a playback sequence #\n");
  fprintf(playbackSequence, "# Specification: <!--brml-->  #\n");
  fprintf(playbackSequence, "# (Big Red Markup Language)   #\n");
  fprintf(playbackSequence, "#-----------------------------#\n");
  fprintf(playbackSequence, "# RoboCup Log Data:           #\n");
  fprintf(playbackSequence, "# %s/%s/%d at %s:%s:%s      #\n", 
                            month, day, year, hour, minute, second);
  fprintf(playbackSequence, "#-----------------------------#\n");
  fprintf(playbackSequence, "# Please see specification at #\n");
  fprintf(playbackSequence, "# end of file.                #\n");
  fprintf(playbackSequence, "#    -Your Friendly Big Red   #\n");
  fprintf(playbackSequence, "#     RoboCup AI Members...   #\n");
  fprintf(playbackSequence, "###############################\n");
  //======================================================
  fprintf(playbackSequence, " <specification");
  //--  
  fprintf(playbackSequence, " brmlVersion='");
  fprintf(playbackSequence, "%f'", 2.1f);
  //--  
  fprintf(playbackSequence, " numberFrames='");
  fprintf(playbackSequence, "%i'", numberOfFrames);
  //--  
  fprintf(playbackSequence, " frameRate='");
  fprintf(playbackSequence, "%2.2f'", sm->getParameters()->general.FRAME_RATE);
  //--  
  fprintf(playbackSequence, " robotsOnTeam0='");
  fprintf(playbackSequence, "%i'", NUM_ROBOTS);
  //--  
  fprintf(playbackSequence, " robotsOnTeam1='");
  fprintf(playbackSequence, "%i'", NUM_ROBOTS);
  //--  
  fprintf(playbackSequence, " ballsOnTeam0='");
  fprintf(playbackSequence, "%i'", 1);
  //--  
  fprintf(playbackSequence, " ballsOnTeam1='");
  fprintf(playbackSequence, "%i'", 0);
  //--  
  fprintf(playbackSequence, " leftSideLine='");
  fprintf(playbackSequence, "%f'", sm->getParameters()->field.LEFT_SIDE_LINE);
  fprintf(playbackSequence, " rightSideLine='");
  fprintf(playbackSequence, "%f'", sm->getParameters()->field.RIGHT_SIDE_LINE);
  fprintf(playbackSequence, " leftDefenseZone='");
  fprintf(playbackSequence, "%f'", sm->getParameters()->field.LEFT_GOALIE_BOX);
  fprintf(playbackSequence, " rightDefenseZone='");
  fprintf(playbackSequence, "%f'", sm->getParameters()->field.RIGHT_GOALIE_BOX);
  fprintf(playbackSequence, " leftGoalPost='");
  fprintf(playbackSequence, "%f'", sm->getParameters()->field.LEFT_GOAL_POST);
  fprintf(playbackSequence, " rightGoalPost='");
  fprintf(playbackSequence, "%f'", sm->getParameters()->field.RIGHT_GOAL_POST);
  fprintf(playbackSequence, " ourGoalieBox='");
  fprintf(playbackSequence, "%f'", sm->getParameters()->field.OUR_GOALIE_BOX);
  fprintf(playbackSequence, " theirGoalieBox='");
  fprintf(playbackSequence, "%f'", sm->getParameters()->field.THEIR_GOALIE_BOX);
  fprintf(playbackSequence, " ourGoalLine='");
  fprintf(playbackSequence, "%f'", sm->getParameters()->field.OUR_GOAL_LINE);
  fprintf(playbackSequence, " theirGoalLine='");
  fprintf(playbackSequence, "%f'", sm->getParameters()->field.THEIR_GOAL_LINE);
  fprintf(playbackSequence, " goalDepth='");
  fprintf(playbackSequence, "%f'", sm->getParameters()->field.GOAL_DEPTH);
  fprintf(playbackSequence, " goalWidth='");
  fprintf(playbackSequence, "%f'", sm->getParameters()->field.GOAL_WIDTH);
  fprintf(playbackSequence, " goalDepth='");
  fprintf(playbackSequence, "%f'", sm->getParameters()->field.GOAL_WIDTH);

  fprintf(playbackSequence, " leftGoalieBox='");
  fprintf(playbackSequence, "%f'", sm->getParameters()->field.LEFT_GOALIE_BOX);
  fprintf(playbackSequence, " rightGoalieBox='");
  fprintf(playbackSequence, "%f'", sm->getParameters()->field.RIGHT_GOALIE_BOX);
  fprintf(playbackSequence, " goalieBoxDepth='");
  fprintf(playbackSequence, "%f'", sm->getParameters()->field.GOALIEBOX_DEPTH);

  fprintf(playbackSequence, " deathZoneLine='");
  fprintf(playbackSequence, "%f'", sm->getParameters()->field.DEATH_ZONE_LINE);
  fprintf(playbackSequence, " defenseZoneLine='");
  fprintf(playbackSequence, "%f'", sm->getParameters()->field.DEFENSE_ZONE_LINE);
  fprintf(playbackSequence, " offensiveZoneLine='");
  fprintf(playbackSequence, "%f'", sm->getParameters()->field.OFFENSE_ZONE_LINE);
  fprintf(playbackSequence, " killZoneLine='");
  fprintf(playbackSequence, "%f'", sm->getParameters()->field.KILL_ZONE_LINE);
  fprintf(playbackSequence, " leftMiddleSector='");
  fprintf(playbackSequence, "%f'", sm->getParameters()->field.LEFT_MIDDLE_SECTOR);
  fprintf(playbackSequence, " rightMiddleSector='");
  fprintf(playbackSequence, "%f'", sm->getParameters()->field.RIGHT_MIDDLE_SECTOR);
  fprintf(playbackSequence, " leftSweetSector='");
  fprintf(playbackSequence, "%f'", sm->getParameters()->field.LEFT_SWEET_SECTOR);
  fprintf(playbackSequence, " rightSweetSector='");
  fprintf(playbackSequence, "%f'", sm->getParameters()->field.RIGHT_SWEET_SECTOR);
  //======  
  fprintf(playbackSequence, "/>\n");
  //======================================================
  //write all frame data to file
  for(int frame=0; frame<numberOfFrames; frame++)
  {
    //beginning of frame
    fprintf(playbackSequence, "########################\n");
    fprintf(playbackSequence, " <frame number='");
    fprintf(playbackSequence, "%i", frame);
    fprintf(playbackSequence, "'>\n");

    //our robots
    fprintf(playbackSequence, "  #-----OUR ROBOTS----\n");
    for(int robot=0; robot<NUM_ROBOTS; robot++)
    {
      fprintf(playbackSequence, "  <robot team='");       
      fprintf(playbackSequence, "%i", 0);
      fprintf(playbackSequence, "' id='");
      fprintf(playbackSequence, "%i", robot);
      fprintf(playbackSequence, "' x='");
      fprintf(playbackSequence, "%1.3f", sm->GetOurRobotX(robot, frame));
      fprintf(playbackSequence, "' y='");
      fprintf(playbackSequence, "%1.3f", sm->GetOurRobotY(robot, frame));
      fprintf(playbackSequence, "' rot='");
      fprintf(playbackSequence, "%1.3f", sm->GetOurRobotRotation(robot, frame));

      fprintf(playbackSequence, "' rawX='");
      fprintf(playbackSequence, "%1.3f", sm->GetOurRobotRawX(robot, frame));
      fprintf(playbackSequence, "' rawY='");
      fprintf(playbackSequence, "%1.3f", sm->GetOurRobotRawY(robot, frame));
      fprintf(playbackSequence, "' rawRot='");
      fprintf(playbackSequence, "%1.3f", sm->GetOurRobotRawRotation(robot, frame));

      fprintf(playbackSequence, "' xVel='");
      fprintf(playbackSequence, "%1.3f", sm->GetOurRobotSentXVel(robot, frame));
      fprintf(playbackSequence, "' yVel='");
      fprintf(playbackSequence, "%1.3f", sm->GetOurRobotSentYVel(robot, frame));
      fprintf(playbackSequence, "' rotVel='");
      fprintf(playbackSequence, "%1.3f", sm->GetOurRobotSentRotation(robot, frame));
      fprintf(playbackSequence, "' destX='");
      fprintf(playbackSequence, "%1.3f", sm->GetRobotDestinationX(robot, frame));
      fprintf(playbackSequence, "' destY='");
      fprintf(playbackSequence, "%1.3f", sm->GetRobotDestinationY(robot, frame));
      fprintf(playbackSequence, "' destRot='");
      fprintf(playbackSequence, "%1.3f", sm->GetRobotDestinationRotation(robot, frame));
      fprintf(playbackSequence, "' passToX='");
      fprintf(playbackSequence, "%1.3f", sm->GetRobotPassX(robot, frame));
      fprintf(playbackSequence, "' passToY='");
      fprintf(playbackSequence, "%1.3f", sm->GetRobotPassY(robot, frame));
      fprintf(playbackSequence, "' passToMe='");
      fprintf(playbackSequence, "%i", sm->GetRobotPassValue(robot, frame));
      fprintf(playbackSequence, "' found='");   
      fprintf(playbackSequence, "%i", sm->OurRobotFound(robot, frame));
      fprintf(playbackSequence, "' hasPossession='");   
      fprintf(playbackSequence, "%i", (sm->FriendlyHasPossession(robot, frame) ? 1 : 0));
      fprintf(playbackSequence, "' kick='");
      fprintf(playbackSequence, "%i", (sm->RobotKick(robot, frame) ? 1 : 0));
      fprintf(playbackSequence, "' dribble='");
      fprintf(playbackSequence, "%i", (sm->RobotDribble(robot, frame) ? 1 : 0));
      fprintf(playbackSequence, "' position='");    
      fprintf(playbackSequence, "%s", sm->getPositionText( sm->GetRobotPosition((RobotIndex)robot, frame) ));
      fprintf(playbackSequence, "' debugMessage='");
      fprintf(playbackSequence, "%s", sm->GetRobotMessageBuffer(robot, frame));        
      

  float getNormalizedTranslationalError(int robot);
  float getNormalizedRotationalError(int robot);
  bool getResetTranslationalVelocity(int robot);
  bool getResetRotationalVelocity(int robot);
  float getDistanceTravelled(int robot);
  float getRotationTravelled(int robot);

      fprintf(playbackSequence, "' normalizedTranslationalError='");
      fprintf(playbackSequence, "%1.3f", sm->getNormalizedTranslationalError(robot, frame));    
      fprintf(playbackSequence, "' normalizedRotationalError='");
      fprintf(playbackSequence, "%1.3f", sm->getNormalizedRotationalError(robot, frame));    
      fprintf(playbackSequence, "' resetTranslationalVelocity='");
      fprintf(playbackSequence, "%i", 
        (sm->getResetTranslationalVelocity(robot, frame) ? 1 : 0));
      fprintf(playbackSequence, "' resetRotationalVelocity='");
      fprintf(playbackSequence, "%i", 
        (sm->getResetRotationalVelocity(robot, frame) ? 1 : 0));
     fprintf(playbackSequence, "' distanceTravelled='");
      fprintf(playbackSequence, "%1.3f", sm->getDistanceTravelled(robot, frame));    
      fprintf(playbackSequence, "' rotationTravelled='");
      fprintf(playbackSequence, "%1.3f", sm->getRotationTravelled(robot, frame));            
      
      fprintf(playbackSequence, "'/>\n");
    }

    //their robots
    fprintf(playbackSequence, "  #----THEIR ROBOTS----\n");
    for(robot=0; robot<NUM_ROBOTS; robot++)
    {
      fprintf(playbackSequence, "  <robot team='");       
      fprintf(playbackSequence, "%i", 1);
      fprintf(playbackSequence, "' id='");
      fprintf(playbackSequence, "%i", robot);
      fprintf(playbackSequence, "' x='");
      fprintf(playbackSequence, "%1.3f", sm->GetTheirRobotX(robot, frame));
      fprintf(playbackSequence, "' y='");
      fprintf(playbackSequence, "%1.3f", sm->GetTheirRobotY(robot, frame));

      fprintf(playbackSequence, "' rawX='");
      fprintf(playbackSequence, "%1.3f", sm->GetTheirRobotRawX(robot, frame));
      fprintf(playbackSequence, "' rawY='");
      fprintf(playbackSequence, "%1.3f", sm->GetTheirRobotRawY(robot, frame));

      fprintf(playbackSequence, "' xVel='");
      fprintf(playbackSequence, "%1.3f", sm->GetTheirRobotXVel(robot, frame));
      fprintf(playbackSequence, "' yVel='");
      fprintf(playbackSequence, "%1.3f", sm->GetTheirRobotYVel(robot, frame));
      fprintf(playbackSequence, "' found='");
      fprintf(playbackSequence, "%i", sm->TheirRobotFound(robot, frame));
      fprintf(playbackSequence, "' hasPossession='");      
      fprintf(playbackSequence, "%i", (sm->OpponentHasPossession(robot, frame) ? 1 : 0));
      fprintf(playbackSequence, "'/>\n");
    }
    
    //our one pathetic lonely robocup ball
    fprintf(playbackSequence, "  #----BALL DATA----\n");
    fprintf(playbackSequence, "  <ball id='0' team='"); 
    fprintf(playbackSequence, "%i", 0);
    fprintf(playbackSequence, "' x='"); 
    fprintf(playbackSequence, "%1.3f", sm->GetBallX(frame));
    fprintf(playbackSequence, "' y='"); 
    fprintf(playbackSequence, "%1.3f", sm->GetBallY(frame));

    fprintf(playbackSequence, "' rawX='"); 
    fprintf(playbackSequence, "%1.3f", sm->GetBallRawX(frame));
    fprintf(playbackSequence, "' rawY='"); 
    fprintf(playbackSequence, "%1.3f", sm->GetBallRawY(frame));

    fprintf(playbackSequence, "' xVel='"); 
    fprintf(playbackSequence, "%1.3f", sm->GetBallXVel(frame));
    fprintf(playbackSequence, "' yVel='"); 
    fprintf(playbackSequence, "%1.3f", sm->GetBallYVel(frame));
    fprintf(playbackSequence, "' found='");     
    fprintf(playbackSequence, "%i", sm->RoboCupBallFound(frame));
    fprintf(playbackSequence, "' source='");     
    fprintf(playbackSequence, "%d", sm->RoboCupBallSource(frame));
    fprintf(playbackSequence, "'/>\n");

    //system frame info
    fprintf(playbackSequence, "  #----SYSTEM FRAME INFO----\n");
    fprintf(playbackSequence, "  <system play='");
    fprintf(playbackSequence, "%s", sm->getPlayName(sm->getPlay(frame)));
    fprintf(playbackSequence, "'/>\n");

    //end the frame
    fprintf(playbackSequence, " </frame>\n");
  }

  //after all the frames put one last frame delimeter
  fprintf(playbackSequence, "########################\n");
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
  //write end of file, closing the playbacksequence
  fprintf(playbackSequence, "</brml>\n");
  //======================================================
  //close file :)
  fclose(playbackSequence);
  //======================================================

  printf("Playback sequence saved to file %s \n", fileName);

}
