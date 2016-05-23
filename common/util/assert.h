/*
 *	assert.h
 */

#ifndef ASSERT_H
#define ASSERT_H

#include <iostream.h>

#define ASSERT(condition, message) \
  if (!(condition)) { \
  cout << "\nError: Assertion \"" << #condition << "\" failed in file\n" \
         << __FILE__ << ", line " << __LINE__ << "\n--> " \
         << message \
         << endl; \
  }


// This test is used by our test code.  in addition to printing an error message, the
// boolean variable errorFlag is set to true;
#define TEST_ASSERT(condition, errorFlag) \
  if (!(condition)) { \
    cout << "\n*Test failed! Assertion \"" << #condition << "\" failed in file\n* " \
         << __FILE__ << ", line " << __LINE__ \
         << endl; \
    errorFlag = true; \
  }


#endif //ASSERT_H

