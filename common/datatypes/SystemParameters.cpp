#include "SystemParameters.h"
//==================================================================
//==================================================================
//when objecst are created values loaded automatically so constructor does nothing
SystemParameters::SystemParameters()
{

}
//==================================================================
//==================================================================
//reloads all values
void SystemParameters::loadValues()
{
  control.loadValues();
  field.loadValues();
  strategy2002.loadValues();
  general.loadValues();
}
//==================================================================
//==================================================================
