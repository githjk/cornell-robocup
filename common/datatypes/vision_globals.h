
#ifndef GLOBALS_H
#define GLOBALS_H

//define unsigned types;
typedef unsigned char ubyte;
typedef signed char sbyte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

const char CLIENT_ONE	  = 0;
const char CLIENT_TWO	  = 1;
const char CLIENT_THREE = 2;
const char CLIENT_FOUR  = 3;

const int NUM_TEAMS		= 2;	//Number of teams (of course 2)
const int NUM_CLIENTS	= 4;	


//Client Names
const int TEAM_ONE	=	CLIENT_ONE;
const int	TEAM_TWO	=	CLIENT_TWO;
const int	VISION		=	CLIENT_THREE;
const int	DISPLAY		=	CLIENT_FOUR;
const int SIMULATOR	=	VISION;		//Simlator should be able to emulate Vision ((char) 2)
const int NET_SERVER=		VISION;		//Also poses as vision

//Commands for AI to send to Vision/Simulation
const int	NODATAAVAILABLE =	-1;
const int NOOP			      =  0;
const int ROBOTCOMMANDS	  =  5;
const int VIEWNORMAL		  = 10;
const int VIEWLEFT		    = 11;
const int VIEWRIGHT		    = 12;

//Commands for Vision to send to AI
const int VISION_NO_OP =	0;
const int KICKERS_ON	 =	20;
const int KICKERS_OFF	 =	21;

//Status
const int UNINITIALIZED	= -32000;	//Has not be initialized yet
const int INFINITY		  = 64000;	//Just a really high number (ie, out of range of field)

//===============================================================================
// Error Codes
//===============================================================================
//Uncategorized Errors
#define	VIAC_NO_ERROR	0	//Nothing is wrong
#define	MEMORYEXISTS	1	//Memory mapped file already created
#define ALLOCATIONERROR	2	//Could not allocate memory for the view
#define	MAPPINGERROR	3	//Could not map a view to the file
#define OPENINGERROR	4	//Could not open the file
#define MUTEX_ERROR		5	//Could not create Mutex/Semaphore

//Network Errors
#define ERR_OPEN_SOCK	10	//Error opening socket
#define ERR_BIND_SOCK	11	//Unable to bind socket
#define ERR_PACK_LEN	12	//Packet length mismatch
#define ERR_CALL_SELECT	13	//Error in call to select
#define ERR_SOCK_READ	14	//Socket not ready to read
#define ERR_PACKET		15	//unexpected packet
#define ERR_NET_TIMEOUT	16	//Network timeout

//Thread Errors
#define ERR_THREAD_CLASS	20	//Thread priority class
#define ERR_THREAD_PRI		21	//Thread priority
#define ERR_MUTEX_LOCK		22	//Could not obtain lock
#define ERR_MUTEX_ABANDON	23	//Semaphore/Mutex abandoned
#define ERR_EVENT_TIMEOUT	24	//Timed out waiting for event
#define ERR_MUTEX_TIMEOUT	25	//Timed out waiting for event

//Data Storing Results
#define	ERR_DATA_READ	30	//Error reading data
#define ERR_DATA_WRITE	31	//Error writing data

#endif