
/** Class VisionPacket
    Purpose : A dynamic placeholder for vision data to be send over the network.
              The main storage location is just an array of byte, which size is determined
              by the number of obstacles, players in each team, and number of balls per team.
              To replace the old Field Data struct that is being used prior to 2002 which takes
              only fixed size packets.

    The order of the packet is in this form
	  1. timeStamp (long)
	  2. startTime (double)
	  3. finishTime (double)
	  4. instruction (int)
	  5. 2 array of robots information containing found, Xpos(float), YPos(float), and rotation (float).
	  6. 1 array of ball information containing source, found, Xpos(float), Ypos(float)
    
    It is absolutely essential that both the vision computer and ai computer are initialized with
    the same parameters in the constructor in order to communicate to be established.

    If there is a change in this copy, we may need to update the AI's copy too.

    @author Chee Yong Lee
    @version 1.0
    Inspected by author on April 6th 2002.
**/

#if !defined VISIONPACKET_H
#define VISIONPACKET_H


const char MAIN_SOURCE      = 1;
const char AUX_SOURCE       = 2;
const char PREDICTED_SOURCE = 3;

class VisionPacket  
{
public:
	/// construction
	VisionPacket(int robotNum,int ballNum,int obstacleNum);
	
  /// default destructor
	virtual ~VisionPacket();
	
  // //////////////////////////////////////////////////////////////////////////////////
  //  Methods
  // //////////////////////////////////////////////////////////////////////////////////

  /// set the time stamp
	void setTimeStamp(long timeStamp);
	
  /// @return the time stamp
	long getTimeStamp();
	
  /// @return the starting time
	void setStartTime(double startTime);
	
  /// @return the starting time
	double getStartTime();
	
  /// @return  the finishing time
	void setFinishTime(double finishTime);
	
  /// @return the finishing time
	double getFinishTime();

  /// @return the kind of instruction
	void setInstruction(int instruction);

  /// get the kind of instruction
	int getInstruction();

	/// set value for the friendly robot
	void setFriendlyRobot(int index, bool found, float xpos, float ypos, float rotation);
  
  /// @return whether friendly robot is found
  bool isFriendlyRobotFound(int index);

  /// sets whether friendly robot is found
  setFriendlyRobotFound(int index, bool found);

  /// @return friendly robot x position
  float getFriendlyRobotXPos(int index);

  /// sets friendly robot x position
  setFriendlyRobotXPos(int index, float xpos);


  /// @return friendly robot y position
  float getFriendlyRobotYPos(int index);

  /// sets friendly robot y position
  setFriendlyRobotYPos(int index, float ypos);

  /// @return friendly robot rotation
  float getFriendlyRobotRotation(int index);

  /// sets friendly robot rotation
  setFriendlyRobotRotation(int index, float rotation);

  /// @return whether opponent robot is found
  bool isOpponentRobotFound(int index);

  /// sets whether opponent robot is found
  setOpponentRobotFound(int index, bool found);

  /// @return opponent robot x position
  float getOpponentRobotXPos(int index);
  
  /// sets opponent robot x position
  setOpponentRobotXPos(int index, float xpos);

  /// @return opponent robot y position
  float getOpponentRobotYPos(int index);

  /// sets opponent robot y position
  setOpponentRobotYPos(int index, float ypos);

  /// @return opponent robot rotation
  float getOpponentRobotRotation(int index);

  /// sets the opponent robot rotation
  setOpponentRobotRotation(int index, float rotation);

  /// @return whether the ball is found
  bool isBallFound(int index);

  /// sets whether the ball is found
  setBallFound(int index, bool found);

  /// @return the ball x position
  float getBallXPos(int index);

  /// @return the ball y position
  float getBallYPos(int index);

  /// @return the source of the ball
  char getBallSource(int index);

  /// sets the ball x position
  setBallXPos(int index, float xpos);

  /// sets the ball y position
  setBallYPos(int index, float ypos);

  /// sets the source of the ball
  setBallSource(int index, char source);
	
  /// returns when robot is found, and outputs parameters in pointers that is being passed
	bool getFriendlyRobot(int index, bool &found, float &xpos, float &ypos, float &rotation);

	/// set value for the opponent robot
	void setOpponentRobot(int index, bool found, float xpos, float ypos, float rotation);
	
  /// returns when robot is found, and outputs parameters in pointers that is being passed
	bool getOpponentRobot(int index, bool &found , float &xpos, float &ypos, float &rotation);

  /// set value for the opponent robot
	void setObstacle(int index, bool found, float xpos, float ypos, float rotation);
	
  /// returns when robot is found, and outputs parameters in pointers that is being passed
	bool getObstacle(int index, bool &found, float &xpos, float &ypos, float &rotation);

  /// @return obstacle is found
  bool isObstacleFound(int index);

  /// sets whether obstacle is found
  setObstacleFound(int index, bool found);

  /// @return obstacle x position
  float getObstacleXPos(int index);

  /// sets obstacle x position
  setObstacleXPos(int index, float xpos);

  /// @return obstacle x position
  float getObstacleYPos(int index);

  /// set sthe obstacle y position
  setObstacleYPos(int index, float ypos);

  /// @return obstacle rotation
  float getObstacleRotation(int index);

  /// set the obstacle rotation
  setObstacleRotation(int index, float rotation);

  /// returns ball is found, and outputs parameters in pointers that is being passed
	bool getBall(int index, bool &found, float &xpos, float &ypos);

	/// returns ball is found, and outputs parameters in pointers that is being passed
	bool getBall(int index, bool &found, float &xpos, float &ypos, char &source);
	
  /// sets values for the ball
	void setBall(int index, bool found, float xpos, float ypos, char source=MAIN_SOURCE);


  /// return the size of the packet
	int getTotalSize();

	/// sets the packet pointer. Note that this assumes that the required space has already been created through the constructor
  void setPacket(char* packetPointer);

	/// returns the packet address
	char* getPacket();
	

  // //////////////////////////////////////////////////////////////////////////////////
  //  Fields
  // //////////////////////////////////////////////////////////////////////////////////

  // Constants Declaration
  // These robots constants is used to determined the amount of space each object
  // of a particular class will take up in the array to be dispatched over the network.
  
  /// constant for the size of robot - size is for location (x, y) and orientation (theta)
  enum packetConstants1 { packetRobotSize    = sizeof(float)*3+sizeof(bool)};

  // constant for the size of obstacle - size is for location (x, y) and orientation (theta)
  enum packetConstants2 { packetObstacleSize = sizeof(float)*3+sizeof(bool)};

  // constant for the size of ball - size is for location x, y
  enum packetConstants3 { packetBallSize     = sizeof(float)*2+sizeof(bool)+sizeof(char)};

private:
	/// number of robots
	int robotNum;
	/// number of balls
	int ballNum;
  /// number of obstacle
	int obstacleNum;
  /// total size of the packet
	int totalSize;
	/// contains the array to be passed
	char* packet;

	/// Records for the memory offset of each position
	int timeStampOffSet;
	int startTimeOffSet;
	int finishTimeOffSet;
	int instructionOffSet;
	int friendlyRobotOffSet;
	int opponentRobotOffSet;
	int ballOffSet;
  int obstacleOffSet;
	int foundOffSet;
	int xPosOffSet;
	int yPosOffSet;
	int rotationOffSet;
  int sourceOffSet;
};

#endif