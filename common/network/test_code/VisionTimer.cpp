/***************************************
 *                                     *
 * Vision Timer                        *
 *                                     *
 * Tests the maximum possible speed of *
 * the vision server.                  *
 *                                     *
 * 6/26/2001   mth13                   *
 **************************************/


#include "..\VisionClient.h"
#include <iostream.h>
#include <iomanip.h>
#include <conio.h>
#include "..\..\datatypes\fieldObjects.h"
#include "..\..\modules\hires_timer.h"
#include "..\..\util\math\cmath.h"
#include "PlayBack_writer.h"

const char helpString[] = "Vision Test Client.\n"
                          "VisionTimer.exe [ <Server IP> <Team #> [/log] ]\n"
                          "  use /log to dump the vision data to a playback file.\n";


int main(int argc, char** argv)
{
	Timer timer;
	char* visionServerName = NULL;	
	int team = 0;
    PlaybackLogger* logger = NULL;

	if(argc >= 2 && argv[1][1] == '?'){ //first arg is -? or /?  
		cout << helpString;
		return 1;
	}
	
	if(argc < 2){
		cout << "No vision server specified; using 127.0.0.1" << endl;
		visionServerName = "127.0.0.1";
	}
	else{
		visionServerName = argv[1];
		cout << "Vision server is \"" << visionServerName << "\"" << endl;
		if(argc < 3){
			cout << "No team specified; assuming team 0." << endl;
		}
		else{
			team = atoi(argv[2]);
			cout << "Team number is " << team  << endl;

            int logTeam=team;
            if(argc > 4)
				logTeam = atoi(argv[3]);

			if ((argc >= 5) && (stricmp(argv[4], "/log") == 0)) {
                logger = new PlaybackLogger(logTeam);
				cout << "Logging data to " << logger->getFileName()
					 << "\nWarning:  this will cause a lot of disk activity.  Don't run the AI on\n"
					 << "the same computer during competition unless you know that it's safe.\n" << endl;
			}
			else if(argc >= 4) {
				cout << helpString;
				return 1;
			}
		}
	}
	cout << "Vision port is 4545.  If you don't like it, tough noogies." << endl;
	const int VISION_PORT = 4545;

	RoboNetwork::VisionClient visionClient(visionServerName, VISION_PORT, team);

	if(visionClient.connect() != 0) {
		cout << "Failed to connect to vision system.  Sayonara..." << endl;
		return 1;
	}
	cout << "Connected to vision system.  Let's rock 'n' roll!\n" << endl;


	FieldData visionData;
	int frame = 0;
	cout << "Receiving the first 20 frames (these won't count in the stats)" << endl;
	for(; frame < 20; frame++){
		visionClient.receive(&visionData);
        if(logger != NULL) {
          logger->addFrame(visionData, frame);
        }
		cout << '.' << flush;
	}
	timer.markStartTime();
	cout<< "\n\n(All times in milliseconds *** Press 'q' to stop gathering data.)\n\n"
		<< "          Vision    |      Running totals:\n"
		<< "Frame#:   time:     |   MIN       MAX       AVE" << endl;
	cout.setf(ios::fixed, ios::floatfield); // set fixed floating format
	cout.precision(3);

	double min = 1e99;
	double max = 0;
	double total = 0;
	double timeThisIter;

	while (true){
		frame++;
		visionClient.receive(&visionData);
		timeThisIter = timer.getElapsedAndReset() * 1000.0;
		min = MIN(min, timeThisIter);
		max = MAX(max, timeThisIter);
		total += timeThisIter;
		cout<< setw(6)  << frame
			<< setw(10) << timeThisIter
			<< setw(13) << min
			<< setw(10) << max
			<< setw(10) << total/(frame-20)
			<< endl;

        if(logger != NULL) {
          logger->addFrame(visionData, frame);
        }

		if(kbhit()){
			int keyHit = getch();
			if((keyHit == 'q') || (keyHit == 'Q'))
				break;
		}
	}

	visionClient.disconnect();

    if(logger != NULL){
      delete logger;
    }

	return 0;
}