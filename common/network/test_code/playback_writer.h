#ifndef PLAYBACK_WRITER_DEFINITION_H
#define PLAYBACK_WRITER_DEFINITION_H

#include "..\..\datatypes\FieldObjects.h"

//function that writes all data to a logfile
class PlaybackLogger {

public:
    PlaybackLogger(int team=0);
	~PlaybackLogger();

	void addFrame(const FieldData& vision, int frame);

    const char* getFileName() { return fileName; }
private:
  FILE *playbackSequence;
  char fileName[128];
  int thisTeam;
};

#endif //PLAYBACK_WRITER_DEFINITION_H

