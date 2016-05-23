#include "SingleAssistOffenseStorm.h"
#include "info/ball/possession.h"

//========================================
SingleAssistOffenseStorm::SingleAssistOffenseStorm(RoboCupStrategyModule* strategy) : 
                  Storm(strategy)
{
  //------------------------
  //construct various clouds
  //------------------------
  Position pList0[] = {BLOCKER};
  clouds[0] = new Cloud(pList0, 1);

  Position pList1[] = {DEFENDER};
  clouds[1] = new Cloud(pList1, 1);

  Position pList2[] = {AGGRESSOR, 
                       CREATOR,
                       SPECIAL_OP_DEFENDER,
                       SPECIAL_OP_AGGRESSOR,
                       SPECIAL_OP_CREATOR};  
  clouds[2] = new Cloud(pList2, 5);
}
//========================================
void SingleAssistOffenseStorm::preformSwitching(VisionModule* vm,
                                        SystemParameters* params)
{
    //-----
    //Get latest strategy data and vision data required for tests
    RobocupStrategyData* sd = strategy->getCurrentRoboCupFrame();
    VisionData* vision = vm->getCurrentFrame();
    //-----
    //Get Aggressor's Location
    RobotIndex AggressorID = sd->getRobotByPosition(AGGRESSOR);
    Pair aggressorLocation = getLocation(AggressorID,
                                         *vision,
                                         *params);                                                                                  
    //-----
    //Get Creator's Location
    RobotIndex CreatorID = sd->getRobotByPosition(CREATOR);
    Pair creatorLocation = getLocation(CreatorID,
                                         *vision,
                                         *params);                                                                                  
    //-----
    //Get Special Op Aggressor's Location
    RobotIndex SpecialOpAggressorID = sd->getRobotByPosition(SPECIAL_OP_AGGRESSOR);
    Pair specialOpAggressorLocation = getLocation(SpecialOpAggressorID,
                                                 *vision,
                                                 *params);                                                                                  
    //-----
    //Get Ball's Location
    Pair ball = sd->getOffensiveBallLocation();
    //-----
    float historisisFactor = params->general.PLAYER_RADIUS;

    //====================================
    //======= Conditional Switches =======
    //====================================
    //----------------------------------------------------------------
    //ensure the special op is a SPECIAL_OP_AGGRESSOR
    if(sd->getRobotByPosition(SPECIAL_OP_CREATOR) != NO_ROBOT)
      switchPositions(SPECIAL_OP_CREATOR, SPECIAL_OP_AGGRESSOR);
    if(sd->getRobotByPosition(SPECIAL_OP_DEFENDER) != NO_ROBOT)
      switchPositions(SPECIAL_OP_DEFENDER, SPECIAL_OP_AGGRESSOR);
    //----------------------------------------------------------------
    //if the aggressor does not have posession of the ball set man
    //closest to the ball to have posession
    if(sd->getRobotByPosition(AGGRESSOR) == NO_ROBOT ||
       !friendlyHasPossession(sd->getRobotByPosition(AGGRESSOR), *params))
    {
      Position closestPosition = NO_POSITION;
      float closestDistance = -1.0f;
	  //----------------------------------------------------------
      if(AggressorID != NO_ROBOT)
      {
        closestPosition = AGGRESSOR;
        closestDistance = aggressorLocation.distanceTo(ball);
      }
	  //----------------------------------------------------------
      if(CreatorID != NO_ROBOT &&
		   (closestPosition == NO_POSITION ||
		    creatorLocation.distanceTo(ball) < closestDistance - historisisFactor)
	    )
      {
        closestPosition = CREATOR;
        closestDistance = creatorLocation.distanceTo(ball);
      }
	  //----------------------------------------------------------
      if(SpecialOpAggressorID != NO_ROBOT &&
		   (closestPosition == NO_POSITION ||
		    specialOpAggressorLocation.distanceTo(ball) < closestDistance - historisisFactor)
	    )
      {
        closestPosition = SPECIAL_OP_AGGRESSOR;
        closestDistance = specialOpAggressorLocation.distanceTo(ball);
      }
      //----------------------------------------------------------
      if(closestPosition != NO_POSITION)
        switchPositions(AGGRESSOR, closestPosition);
    }  
    //----------------------------------------------------------------
    //kinda complicated. make the creator on the oppsite side of the field as the aggressor.
        
    if(CreatorID != NO_ROBOT &&
       SpecialOpAggressorID != NO_ROBOT)
    {
      //if creator on same side of field as aggressor and special op on opposite side, switch special op
      //creator (to avoid running across).
      //However, do not switch them if one of them is asking for a pass.
      if(
         (
          (aggressorLocation.getY() > params->field.SPLIT_LINE &&
           creatorLocation.getY() >  aggressorLocation.getY() &&
           specialOpAggressorLocation.getY() < aggressorLocation.getY()) ||
          (aggressorLocation.getY() < params->field.SPLIT_LINE &&
           creatorLocation.getY() < aggressorLocation.getY() &&
           specialOpAggressorLocation.getY() > aggressorLocation.getY())
         ) &&
         !sd->getPassValue(CreatorID) &&
         !sd->getPassValue(SpecialOpAggressorID)
        )
      {
         switchPositions(SPECIAL_OP_AGGRESSOR, CREATOR);
      }

      //if creator and special op on same side of field as aggressor...
      else if(   
               (aggressorLocation.getY() > params->field.SPLIT_LINE &&
                creatorLocation.getY() > aggressorLocation.getY() &&
                specialOpAggressorLocation.getY() > aggressorLocation.getY()) ||
               (aggressorLocation.getY() < params->field.SPLIT_LINE &&
                creatorLocation.getY() < aggressorLocation.getY() &&
                specialOpAggressorLocation.getY() < aggressorLocation.getY())
             )
      {
        //if both creator and special op ahead of aggressor, make creator the one farther upfield
        //since he'll have more time to cross in front of aggressor.
        if(specialOpAggressorLocation.getX() > aggressorLocation.getX() + 4*params->general.PLAYER_RADIUS ||
           creatorLocation.getX() > aggressorLocation.getX() + 4*params->general.PLAYER_RADIUS)
        {
           if(creatorLocation.getX() + historisisFactor < specialOpAggressorLocation.getX() &&
              aggressorLocation.getX() < params->field.THEIR_GOALIE_BOX - 4*params->general.PLAYER_RADIUS)
           {
             switchPositions(SPECIAL_OP_AGGRESSOR, CREATOR);
           }
        }
        //else, make creator position behind since he can move around from behind
        else
        {
           if(creatorLocation.getX() > specialOpAggressorLocation.getX() + historisisFactor)
           {
             switchPositions(SPECIAL_OP_AGGRESSOR, CREATOR);
           }
        }
      }
      //if creator and special op on opposite side of field as aggressor...
      else if(   
               (aggressorLocation.getY() > params->field.SPLIT_LINE &&
                creatorLocation.getY() < aggressorLocation.getY() &&
                specialOpAggressorLocation.getY() < aggressorLocation.getY()) ||
               (aggressorLocation.getY() < params->field.SPLIT_LINE &&
                creatorLocation.getY() > aggressorLocation.getY() &&
                specialOpAggressorLocation.getY() > aggressorLocation.getY())
             )
      {
        //----------------------------------------------------------------
        //minimize distances special op aggressor and creator have to travel
        if(SpecialOpAggressorID != NO_ROBOT &&
           CreatorID != NO_ROBOT)
        {
           float dist1 = creatorLocation.squareDistanceTo(sd->getDestination(CreatorID)->getPos()) +
                         specialOpAggressorLocation.squareDistanceTo(sd->getDestination(SpecialOpAggressorID)->getPos());
           float dist2 = specialOpAggressorLocation.squareDistanceTo(sd->getDestination(CreatorID)->getPos()) +
                         creatorLocation.squareDistanceTo(sd->getDestination(SpecialOpAggressorID)->getPos());
           if(dist2 < dist1 - historisisFactor*historisisFactor)
           {
             switchPositions(CREATOR, SPECIAL_OP_AGGRESSOR);
           }
        }
        //----------------------------------------------------------------
        /*
        //make creator the guy farther away
        if(creatorLocation.distanceTo(aggressorLocation) + historisisFactor < 
           specialOpAggressorLocation.distanceTo(aggressorLocation))
        {
           switchPositions(SPECIAL_OP_AGGRESSOR, CREATOR);
        }
        */
      }
    }
    //----------------------------------------------------------------
}
//========================================
