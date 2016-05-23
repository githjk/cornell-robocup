//put these libraries in the settings option...
//kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tcl84.lib tclstub84.lib tk84.lib tkstub84.lib
//---------------------
//Basic GUI definitions
//---------------------
#ifndef GUI_DEF_INTERNAL
#define GUI_DEF_INTERNAL

//Basic Tcl/Tk headers
#include "tcl.h"
#include "tk.h"

//So we can used ArgError?
#include "stdlib.h"
#include "math.h"

//In case of error, copy into string buffer
#include <stdio.h>

//Specify Cornell commands to link to c++ code
#include "GUI_Commands.h"
#include "../visionTestGUI/visionTestGUI_Commands.h"

#ifdef __cplusplus
extern "C" {
#endif

//################################################
//### -Initialize the Tcl/Tk libraries         ###
//### -Execute the user defined startup script ###
//################################################
int Tcl_AppInit(Tcl_Interp *interp);
int ArgError(char *command, Tcl_Interp *interp);
int ArgvError(char *command, Tcl_Interp *interp);

//This method launches GUI
DWORD WINAPI launchGUI(LPVOID unused);

//Tcl uses arrays of characters
//to pass around output...
extern char result[600];


//End extern...
#ifdef __cplusplus
};
#endif

#endif //GUI_DEF_INTERNAL


