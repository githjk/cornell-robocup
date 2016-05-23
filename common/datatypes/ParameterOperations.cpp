#include "ParameterOperations.h"
#include <fstream.h>
#include "assert.h"
#include <string.h>

const int LABEL_BUFFER_SIZE = 255;

///Reads a string from a file
void ParameterOperations::readString(char* buffer, const char* expectedLabel, ifstream& file)
{
  static char label[LABEL_BUFFER_SIZE];

  //If the first character is a '#', ignore the entire line.
  file.eatwhite();
  while (file.peek() == '#'){
    file.ignore(255, '\n');
    file.eatwhite();
  }

  file >> label >> buffer;
  ASSERT(strcmp(label, expectedLabel) == 0, "Error in parameters file: read "<<label<<" when expecting "<<expectedLabel);
}

///Reads a float from a file
void ParameterOperations::readFloat(float* value, const char* expectedLabel, ifstream& file)
{
  static char label[LABEL_BUFFER_SIZE];

  //If the first character is a '#', ignore the entire line.
  file.eatwhite();
  while (file.peek() == '#'){
    file.ignore(255, '\n');
    file.eatwhite();
  }

  file >> label >> *value;
  ASSERT(strcmp(label, expectedLabel) == 0, "Error in parameters file: read "<<label<<" when expecting "<<expectedLabel);
}

///Reads an integer from a file
void ParameterOperations::readInt(int* value, const char* expectedLabel, ifstream& file)
{
  static char label[LABEL_BUFFER_SIZE];

  //If the first character is a '#', ignore the entire line.
  file.eatwhite();
  while (file.peek() == '#'){
    file.ignore(255, '\n');
    file.eatwhite();
  }

  file >> label >> *value;
  ASSERT(strcmp(label, expectedLabel) == 0, "Error in parameters file: read "<<label<<" when expecting "<<expectedLabel);
}
