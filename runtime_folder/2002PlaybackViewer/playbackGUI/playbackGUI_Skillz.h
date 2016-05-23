
//----------------------------------------------------
//What the heck is this? To make sure
//the GUI cannot modify system variables (like
//a robots assigned position during the middle of a 
//play, we write all GUI commands to a diary, and 
//the main loop looks at this diary at a specific time.
//----------------------------------------------------
//so we know how many robots exist...
#ifndef GUI_Skillz_DEF
#define GUI_Skillz_DEF

//If GUI does not set a value...
#define notSet -1

class Skillz
{
  public:

    Skillz()
    {
      reset(); //reset all values to nonSet
    }

    void reset()
    {
      togglePlayPause = false;
      setFrameNumber = false;
      newFrameNumber = 1;
      openFile = false;
      fileName = "default.txt";
    }

    //----------
    //actual data
    //----------
    bool togglePlayPause;
    bool setFrameNumber;
    int newFrameNumber;
    bool openFile;
    char* fileName;  
};    


#endif //GUI_Skillz_DEF
