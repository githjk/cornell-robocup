#include "find_pass.h"


//This function returns to best (farthest forward) pass destination to a sideline 
//from one robot to another.  It will only pass if the passing lane width is 
//within the defined threshhold.  Returns false if no good pass is found.
bool findSidelinePass(Pair passerPos,
             Pair receiverPos,
             Pair *passDest,
             const VisionData& field,
             const SystemParameters &rp)
{
    float laneAngle;
  //Pass to the sideline nearest to the receiver
  float sidelineY = rp.strategy2002.SIDELINE_Y_RECEIVE_PASS;
  if(receiverPos.getY() <= 0.0f) sidelineY *= -1;

  //We search a line near the sideline for the best pass. We discretize this
    //line when searching.  Find the distance between each search-point on the line:	
  float step = (rp.strategy2002.FAR_FORWARD_LINE - rp.strategy2002.CLEAR_UPFIELD_LINE) / 
            (float)sidelineNumSearchPoints;

  Pair tempPassDest;

  //start from the farthest point on the line and search for the best pass
  for(int i=0; i < sidelineNumSearchPoints; i++) {
      //float x = FAR_FORWARD_LINE - (i * step);
    tempPassDest = Pair( rp.strategy2002.FAR_FORWARD_LINE - (i * step), sidelineY );	
    laneAngle = laneHalfAngle(passerPos,tempPassDest,field,rp);		

    //If the pass lane to the receive point is big enough, use that point
    if(	laneAngle >= rp.strategy2002.PASS_LANE_ANGLE_THRESH ) {  
      *passDest = tempPassDest;
      return true;
    }
  }

  //We can't find a good pass, so return false
  return false;
}







//This function returns to best (farthest forward) pass destination to the center 
//(x-axis) from one robot to another.  It will only pass if the passing lane angle 
//is within the defined threshhold.  Returns false if no good pass is found.
bool findCenterfieldPass(Pair passerPos,
            Pair *passDest,
            const VisionData& field,
            const SystemParameters &rp)
{

  //We search the line x=0 for the best pass. We discretize this line when 
  //searching.  Find the distance between each search-point on the line:
  float step = (rp.strategy2002.FAR_FORWARD_LINE - rp.strategy2002.CLEAR_UPFIELD_LINE) / 
            (float)centerfieldNumSearchPoints;

  Pair tempPassDest;
  float laneAngle;

    //Start from the farthest point on the line and search for the best pass
  for(int i=0; i < centerfieldNumSearchPoints; i++) {
    //We receive on the x-axis line (y=0)	
    tempPassDest = Pair( rp.strategy2002.FAR_FORWARD_LINE - (i * step), rp.field.SPLIT_LINE );		  
    laneAngle = laneHalfAngle(passerPos,tempPassDest,field,rp);

    //If the pass lane to the receive point is big enough, use that point		
    if( laneAngle >= rp.strategy2002.PASS_LANE_ANGLE_THRESH ) {  
      *passDest = tempPassDest;
      return true;
    }
  }

  //We can't find a good pass, so return false
  return false;
}







//Finds up to two upfield pass destinations.
int findUpfieldPass(Pair passerPos,
            Pair *passDest1,
            Pair *passDest2,
            const VisionData& field,
            const SystemParameters &rp)
{
  //Best two pass destinations and their buffer widths.  
  //We'll require that buffer1 >= buffer2 (thus pass1 is better than pass2)
  Pair pass1, pass2;
  float buffer1 = 0.0f, buffer2 = 0.0f;

  //We want to pass the ball farther forward if we have possession farther 
  //back.  Therefore we find the distance from ball to enemy goal, and shoot 
  //forward by a fixed percentage of that distance;

  //The percentage of distance from ball to enemy goal
  const float kickDistPercentage = 0.5f;	

  //Get x-distance from passer to enemy goal line, multiply by kickDistPercentage 
  //to get the pass distance
  float passDist = rp.field.THEIR_GOAL_LINE - passerPos.getX();
  passDist *= kickDistPercentage;

  //We search the first (closest) line parallel to the halfline for a good pass. 
  //We discretize this line when searching.  Find the distance between each 
  //search-point on the line.  Note that we leave a buffer from the sideline to 
  //the pass destination of length "sideBuffer"
  float step = (rp.field.FIELD_WIDTH - (2*sideBuffer)) / (float)halflineNumSearchPoints;

  //Temporary variables used in for-loop
  Pair tempPassDest;	//pass destination point
  float tempBuffer;	//buffer width of the pass

  //Search the closer line for the first best pass. Use this destination later as 
  //an obstacle for finding the second pass.
  for(int i=0; i <= halflineNumSearchPoints; i++) {
    tempPassDest = Pair( passerPos.getX() + passDist,
               rp.field.LEFT_SIDE_LINE - (i * step) - sideBuffer );
    //If the pass lane to the receive point is big enough, store it if it's 
    //buffer angle is bigger than previous stored ones.
    tempBuffer = laneHalfAngle(passerPos,tempPassDest,field,rp);
    if( tempBuffer >= rp.strategy2002.PASS_LANE_ANGLE_THRESH && tempBuffer > buffer1) {
      buffer1 = tempBuffer;
      pass1 = tempPassDest;
    }
  }

  //Find second pass 
  for(i=0; i < halflineNumSearchPoints; i++) {
    tempPassDest = Pair( passerPos.getX() + passDist + passLengthDiff,
               rp.field.LEFT_SIDE_LINE - (i * step) - sideBuffer );		  
      //If the pass lane to the receive point is big enough, store it if it's 
    //buffer angle is bigger than previous stored ones.		
    //Remember that first pass dest is now an obstacle (if we found a first pass)
    if (buffer1 == 0.0f)
      tempBuffer = laneHalfAngle(passerPos,tempPassDest,field,rp);
    else
      tempBuffer = laneHalfAngle(passerPos,tempPassDest,field,rp,&pass1);

    if( tempBuffer >= rp.strategy2002.PASS_LANE_ANGLE_THRESH && tempBuffer > buffer2) {  
      buffer2 = tempBuffer;
      pass2 = tempPassDest;
    }
  }

  //Set pass destinations and return an integer indicating how many passes found.
  //Switch pass1 and pass2 if pass2 was found and pass1 was not
  if(buffer1 == 0.0f && buffer2 > 0.0f) {
    pass1 = pass2;
    buffer1 = buffer2;
    buffer2 = 0.0f;
  }

  if(buffer1 > 0.0f) {
    *passDest1 = pass1;
    //We found at least one good pass.  Check if a second was found
    if(buffer2 > 0.0f) {
      *passDest2 = pass2;
      return 2;    
    }
    return 1; //we only found 1 good pass
  }
  //We can't find a good pass, so return 0
  return 0;
}

// Determines if a robot is currently open for an upfield pass
// Used in clearUpfield
bool openForPass(RobotIndex ID,
            const VisionData& field,
            const SystemParameters &rp)
{

  Pair loc;
  loc.set(getLocation(ID,field,rp));
  
  // if there is an enemy close to the receiver
  for (RobotIndex i=ROBOT0; i<NUM_ROBOTS; i++) {
    if ( theirRobotFound(i,field,rp) ) {
      if ( loc.distanceTo(getTheirRobotLocation(i,field,rp)) < 3*rp.general.PLAYER_RADIUS ) {
        return false;
      }
    }
  }

  // if there is not a lane to the receiver
  if ( laneHalfWidth(getBallLocation(field),loc,field,rp) < rp.strategy2002.PASS_LANE_THRESH ) {
    return false;
  }

  // if the receiver is not halfway to the goal line from the ball
  if (( loc.getX() < ballXLocation(field) + (rp.field.THEIR_GOAL_LINE - ballXLocation(field))*0.5f ) ||
      ( loc.getX() > ballXLocation(field) + (rp.field.THEIR_GOAL_LINE - ballXLocation(field))*0.7f )) {
    return false;
  }
  return true;

}

// Finds an open pass along the sideline on the defensive half of the field.
// Used in GoaliePass
bool findNearSidelinePass(Pair passerPos,
             Pair *passDest,
             const VisionData& field,
             const SystemParameters &rp,
             bool chooseRightSide)
{
  float laneAngle;
  //Pass to the sideline nearest to the receiver
  float sidelineY = rp.strategy2002.SIDELINE_Y_RECEIVE_PASS;
  float maxSidelineX = (rp.field.HALF_LINE - 2*rp.general.PLAYER_RADIUS);
  
  if(chooseRightSide)
    sidelineY *= -1;

  //We search a line near the sideline for the best pass. We discretize this
  //line when searching.  Find the distance between each search-point on the line:	
  float step = ( maxSidelineX - (rp.field.OUR_GOAL_LINE/2) ) / 
                            (float)sidelineNumSearchPoints;

  Pair tempPassDest;
  float maxAngle = 0.2f;

  //start from the farthest point on the line and search for the best pass
  for(int i=0; i < sidelineNumSearchPoints; i++) {
    tempPassDest = Pair( maxSidelineX - (i * step), sidelineY );	

    laneAngle = laneHalfAngle(passerPos,tempPassDest,field,rp);		

    //Save the pass if it is better (has a wider lane) than our previous best pass
    if( laneAngle > maxAngle ) {
      *passDest = tempPassDest;
      maxAngle = laneAngle;
    }
  }
  //We found the best pass, but is it good enough?
  return (maxAngle >= rp.strategy2002.PASS_LANE_ANGLE_THRESH);
}

//finds a location for the passForward during the corner kick play...
bool isGoalBoxPassLocation(const VisionData& field,
						   const SystemParameters &rp,
						   Pair  &passDest,
						   bool leftSide)
{
    
  //Define the initial and final points of the search...
  float initialYPoint, tempYPos;
  bool goodLocation;

  if (leftSide)
    initialYPoint=rp.field.RIGHT_GOAL_POST+0.0f*rp.general.PLAYER_RADIUS;
  else
    initialYPoint=rp.field.LEFT_GOAL_POST-0.0f*rp.general.PLAYER_RADIUS;
  
  //we will search the area of the goal box
  const int numSearchPoints = 20;
	
  //Distance between each position that we consider
  //From the sides of the goal box, 40cm towards the middle...
	float step = (0.40f) / numSearchPoints;
	
  RobotIndex j;

	//Consider the targets from initialPoint to endPoint...
	for(int i=0; i<=numSearchPoints; ++i) 
  {
    //reset the location flag...
    goodLocation=true;

    //check all of their robots...
    j=ROBOT0;
    while (j<NUM_ROBOTS && goodLocation)
    {
      if (leftSide)
        tempYPos=initialYPoint-step*i;
      else
        tempYPos=initialYPoint+step*i;
          
      //if we find an opponent robot near this location, mark the location as BBAAADD.
      if (dist(getTheirRobotLocation(j,field,rp),Pair(rp.field.THEIR_GOALIE_BOX,tempYPos))<(rp.general.OPPONENT_RADIUS+rp.general.PLAYER_RADIUS))
      {
          goodLocation=false;
      }

      j++;
    }

    //if this is a good location, return true and set the pass destination to it...
    if (goodLocation)
    {
      passDest.set(rp.field.THEIR_GOALIE_BOX, tempYPos);
      return true;
    }
	}

  //if none of the locations are good, return false...
  return false;
}