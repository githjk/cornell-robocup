#include "Cloud.h"
#include "iostream.h"

//========================================
Cloud::Cloud(Position* positions, int num)
{
  this->positions = new Position[num+1];
  this->numPositions = num;

  for(int i=0; i<num; i++)
  {
    this->positions[i] = positions[i];
  }

  this->positions[num] = (Position)NULL;
}
//========================================
Cloud::~Cloud()
{
  delete[] positions;
  positions = NULL;
}
//========================================
bool Cloud::positionInCloud(Position position)
{
  for(int i=0; i<numPositions; i++)
  {
    if(position == positions[i])
      return true;
  }
  return false;
}
//========================================

