#include "back_pass.h"

bool trapBackPass(const VisionData& field,
                  const RobocupStrategyData& sd,
                  //robots Position
                  RobotIndex robotID,
                  Destination* sendTo)
{
  // FIXME
	cout << "trapBackPass unimplemented." << endl;
	return false;
}

void evaluateBackPass(const VisionData& field, 
                      const RobocupStrategyData& GS,
                      Pass* passData)
{
  // FIXME
	// cout << "evaluateBackPass unimplemented." << endl;
  passData->setPassValue(0);
}
