/*
*   playbook.h
*/

#ifndef PLAYBOOK_H
#define PLAYBOOK_H

#include "baseplay.h"
#include "assert.h"

extern Play INVALID_PLAY,
            START_2001_PLAYS, 
            END_2001_PLAYS,
            START_2001_PENALTY_PLAYS,
            END_2001_PENALTY_PLAYS,
            START_2002_PLAYS, 
            END_2002_PLAYS,
            START_2002_PENALTY_PLAYS,
            END_2002_PENALTY_PLAYS,
            START_FORMATION_PLAYS, 
            END_FORMATION_PLAYS,  
            START_EVAN_KUHN_SKILL_TEST_PLAYS,
            END_EVAN_KUHN_SKILL_TEST_PLAYS,
            START_EVAN_MALONE_SKILL_TEST_PLAYS,
            END_EVAN_MALONE_SKILL_TEST_PLAYS,      
            START_JOEL_SKILL_TEST_PLAYS,
            END_JOEL_SKILL_TEST_PLAYS,
            START_NIRAV_SKILL_TEST_PLAYS,
            END_NIRAV_SKILL_TEST_PLAYS,      
            START_WILL_SKILL_TEST_PLAYS,
            END_WILL_SKILL_TEST_PLAYS,
            START_INDIVIDUAL_PLAYS, 
            END_INDIVIDUAL_PLAYS,
            START_INTIMIDATION_PLAYS, 
            END_INTIMIDATION_PLAYS,
            START_OTHER_PLAYS, 
            END_OTHER_PLAYS,
            NUM_PLAYS;

class Playbook {
public:
  
  //constructor
  Playbook(VisionModule* vision);

  //destructor
  ~Playbook() {
    for (int i = 0; i < NUM_PLAYS; i++){
      if (playbook[i] != NULL){
        delete playbook[i];
      }
    }
    delete [] playbook;
  }

  BasePlay* getPlay(Play p) const {
    ASSERT((p != INVALID_PLAY), "Play not yet implemented");
    return playbook[p];
  }

  char* getPlayName(Play p) const {
    if ((p == INVALID_PLAY) || (playbook[p] == NULL))
    {
      return "Unimplemented Play";
    }
    return playbook[p]->getName();
  }

  void enumPlayNames(char* buffer) const;

protected:
  BasePlay** playbook;
};



#endif //PLAYBOOK_H

