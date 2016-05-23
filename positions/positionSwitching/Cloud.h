#ifndef CLOUD_H
#define CLOUD_H

class Cloud;

#include "common/datatypes/robocup_strategy_data.h"

class Cloud
{
private:
  Position* positions;  
  int numPositions;

public:
  Cloud(Position* positions, int num);
  ~Cloud();

  bool positionInCloud(Position position);

};

#endif //CLOUD_H

