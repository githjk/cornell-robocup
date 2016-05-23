
//put these libraries in the settings option...
//kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tcl84.lib tclstub84.lib tk84.lib tkstub84.lib
//---------------------
//Basic GUI definitions
//---------------------
#ifndef GUI_DEF
#define GUI_DEF

//Basic Tcl/Tk headers
#include "tcl.h"
#include "tk.h"

//So we can used ArgError?
#include "math.h"
#include <stdlib.h>

//In case of error, copy into string buffer
#include <stdio.h>

 //Specify Cornell commands to link to c++ code
#include "playbackGUI_Commands.h"

//Specify how to store GUI commands
#include "playbackGUI_Skillz.h"

//Create a skillz diary of all GUI commands.
extern Skillz GUI_Record;

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
void launchGUI(char* fileName);

//Tcl uses arrays of characters
//to pass around output...
extern char result[600];


//End extern...
#ifdef __cplusplus
};
#endif

#endif //GUI_DEF


