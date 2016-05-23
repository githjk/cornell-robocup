#ifndef PARAMETER_OPERATIONS_H
#define PARAMETER_OPERATIONS_H

#include <fstream.h>

///A base class for all parameter classes to extend.
///Contains functions useful in all such classes.
class ParameterOperations
{
public:

  //the default load values method does nothing, must be overwritten :)
  void loadValues() { }

  void readString(char* buffer, const char* expectedLabel, ifstream& file);
  void readInt(int* value, const char* expectedLabel, ifstream& file);
  void readFloat(float* value, const char* expectedLabel, ifstream& file);
};


///Operations for reading parameters from a file.  These macros both put 
///the parameter into the specified location and check the label:
#define READ_FLOAT(param) readFloat(&param, #param, mainFile);
#define READ_FLOAT_TO_OBJECT(param, object) readFloat(&object.param, #param, mainFile);
#define READ_TEMP_FLOAT(param, var) readFloat(&var, #param, mainFile);

#define READ_INT(param) readInt(&param, #param, mainFile);
#define READ_INT_TO_OBJECT(param, object) readInt(&object.param, #param, mainFile);
#define READ_TEMP_INT(param, var) readInt(&var, #param, mainFile);

#define READ_STRING(param) readString(param, #param, mainFile);
#define READ_STRING_TO_OBJECT(param, object) readString(&object.param, #param, mainFile);
#define READ_TEMP_STRING(param, var) readString(&var, #param, mainFile);

#define READ_TRAJ_FLOAT(param) readFloat(&param, #param, trajFile);
#define READ_TRAJ_INT(param) readInt(&param, #param, trajFile);
#define READ_TRAJ_STRING(param) readString(&param, #param, trajFile);




#endif //PARAMETER_OPERATIONS_H
