
#include "WirelessSender.h"
#include "WheelConversion2002.h"
#include "WheelConversion2001.h"
#include "WheelConversion2000.h"
#include "iostream.h"
#include "PacketStructure2001.h"
#include "PacketStructure2002.h"
#include "math.h"

static const int BYTES_PER_ROBOT = 4;

static float PI = 3.1459f;




static float wheelOne, wheelTwo, wheelThree;

static WheelConversion2002* robots2002;
static WheelConversion2001* robots2001;
static WheelConversion2000* robots2000;

static PacketStructure2002* packet2002;
static PacketStructure2001* packet2001;

//-----------------------------------------------------
WirelessSender::WirelessSender(char* portName){

  robots2002 = new WheelConversion2002();
  robots2001 = new WheelConversion2001();
  robots2000 = new WheelConversion2000();

  packet2002 = new PacketStructure2002();
  packet2001 = new PacketStructure2001();

  // Calculate Linear Speed Modifier
  linearSpeedModifier2001 = (gear2001 / (PI * diameter2001))
      * counts_per_rev2001    // counts per revolution
      * sample_time_8mhz2001  // sample period
      * PIDconst2001          // fit for PID
      * quadrature2001;       // for quadrature encoders

  // Calculate 2000 Linear Speed Modifier
  linearSpeedModifier2000 = (gear2000 / (PI * diameter2000))
      * counts_per_rev2000    // counts per revolution
      * sample_time_8mhz2000  // sample period
      * PIDconst2000          // fit for PID
      * quadrature2000;       // for quadrature encoders

  int status;
  // Open connection
  status = RCSPort.openConnection(portName);

  if (status == RCS_INTERFACE_CANNOT_SET_COM_STATE)
     cout << "Cannot open port " << portName << endl;
  else {
    cout << "Wireless Transmission on " << portName << endl;
  }


  for (int i=0; i<NUM_AGENTS; i++){


    // Default robot type
    robotType[i] = ROBOT_2002;

    // Initialize 'rotations' array
    rotations[i] = 0.0f;

    // Initialize 'commands' array
    commands[i].xVel = 0.0f;
    commands[i].yVel = 0.0f;
    commands[i].rotVel = 0.0f;

    commands[i].kick = 0;
    commands[i].dribble = 0;
    commands[i].vDribble = 0;

    commands[i].dive = false;

    // Initialize 'wheels' array
    wheels[i].xVel = 0.0f;
    wheels[i].yVel = 0.0f;
    wheels[i].rotVel = 0.0f;

    wheels[i].kick = 0;
    wheels[i].dribble = 0;
    wheels[i].vDribble = 0;

    wheels[i].dive = false;
  }

  // Initialize data packet
  for (int j=0; j<TOTAL_PACKET_SIZE; j++){
    dataPacket[j] = 0x00;
  }

  // Add identity bit for second set of packets
  dataPacket[PACKET_SIZE] = 0x80;
   
}


//-----------------------------------------------------
WirelessSender::~WirelessSender()
{
  RCSPort.closeConnection();
}
//-----------------------------------------------------
void WirelessSender::setBoard(int type)
{
  wirelessBoard = type;
  RCSPort.setBoard(type);
}
//-----------------------------------------------------
int WirelessSender::sendCommands(bool secondPacket)
{
  int status;  

  if (secondPacket) {
    status = RCSPort.sendFirstPacket((char*)dataPacket);
    status = RCSPort.sendPacket((char*)(&dataPacket[PACKET_SIZE]));
  }
  else {
    status = RCSPort.sendPacket((char*)dataPacket);
  }

  return status;
}

//-----------------------------------------------------
void WirelessSender::setRobotCommand(int index, RobotCommand* command)
{
  if ((index < 0) || (index >= NUM_AGENTS)) return;

  memcpy( &commands[index], command, sizeof(RobotCommand));
}


//-----------------------------------------------------
void WirelessSender::setRobotVelocity(int index, float xVel, float yVel, float rotVel)
{
  commands[index].xVel    = xVel;
  commands[index].yVel    = yVel;
  commands[index].rotVel  = rotVel;
}

//-----------------------------------------------------
void WirelessSender::setRobotActions (int index, int kick, int dribble, int vDribble)
{
  commands[index].kick      = kick;
  commands[index].dribble   = dribble;
  commands[index].vDribble  = vDribble;
}

//-----------------------------------------------------
void WirelessSender::setRobotRotation(int index, float rotation)
{
  if ((index < 0) || (index >= NUM_AGENTS)) return;

  rotations[index] = rotation;
}

//-----------------------------------------------------
void WirelessSender::setRobotType(int index, RobotType newRobotType)
{
  robotType[index] = newRobotType;
}

RobotType WirelessSender::getRobotType(int index)
{
  return robotType[index];
}



//-----------------------------------------------------
void WirelessSender::getWheelCommands(int index, RobotCommand* wheel)
{
  if ((index < 0) || (index >= NUM_AGENTS)) return;

  memcpy( wheel, &wheels[index], sizeof(RobotCommand));
}
//-----------------------------------------------------
void WirelessSender::getWheelCommands(int index, 
                                      float &xVel, 
                                      float &yVel, 
                                      float &rotVel)
{
  if ((index < 0) || (index >= NUM_AGENTS)) return;

  xVel = wheels[index].xVel;
  yVel = wheels[index].yVel;
  rotVel = wheels[index].rotVel;
}
//-----------------------------------------------------
void WirelessSender::getPacket(char packet[TOTAL_PACKET_SIZE])
{
  memcpy(packet, dataPacket, TOTAL_PACKET_SIZE);
}

//-----------------------------------------------------
void WirelessSender::buildPacket()
{

  // Clear data packet
  for (int i=0; i<TOTAL_PACKET_SIZE; i++){
    dataPacket[i] = 0x00;
  }

  // Add identity bit for second set of packets
  dataPacket[PACKET_SIZE] = 0x80;

  int offsetByte = 1;

  // Build packets for each robot
  for (int robot=0; robot<NUM_AGENTS; robot++){

    if (NUM_AGENTS/2 == robot) offsetByte++;

    switch (robotType[robot]){
    case ROBOT_2000:
      buildRobot2000(robot, offsetByte);
      break;
    case ROBOT_2001:
      buildRobot2001(robot, offsetByte);
      break;
    case ROBOT_2002:
      buildRobot2002(robot, offsetByte);
      break;
    default:
      // send nothing
      stopRobot(robot, offsetByte);
    }
  }

}

//-----------------------------------------------------

void WirelessSender::stopRobot(int robot, int offsetByte)
{
  dataPacket[offsetByte + BYTES_PER_ROBOT * robot + 0] = 0x00;
  dataPacket[offsetByte + BYTES_PER_ROBOT * robot + 1] = 0x00;
  dataPacket[offsetByte + BYTES_PER_ROBOT * robot + 2] = 0x00;
  dataPacket[offsetByte + BYTES_PER_ROBOT * robot + 3] = 0x00;
}

//-----------------------------------------------------

void WirelessSender::buildRobot2002(int robot, int offsetByte)
{
  //**********************************************************
  // Add parameter bits to packet
  unsigned char result;
  result = 0x00;
  
  // Set dive enable command
  if (commands[robot].dive)
    result |= packet2002->PACKET_DIVE;
  
  // Set kick enable command
  switch (commands[robot].kick) 
  {
  case 0:   break;
  case 1:   result |= packet2002->PACKET_KICK_SLOW;
            break;
  case 2:   result |= packet2002->PACKET_KICK_PASS_1;
            break;
  case 3:   result |= packet2002->PACKET_KICK_PASS_2;
            break;
  case 4:   result |= packet2002->PACKET_KICK_PASS_3;
            break;
  case 5:   result |= packet2002->PACKET_KICK_PASS_4;
            break;
  case 6:   result |= packet2002->PACKET_KICK_PASS_5;
            break;
  case 7:
  default:  result |= packet2002->PACKET_KICK_FAST;
            break;
  }

  // Set dribble command
  switch (commands[robot].dribble) 
  {
  case 0:   break;
  case 1:   result |= packet2002->PACKET_DRIBBLE_SLOW;
            break;
  case 2:   result |= packet2002->PACKET_DRIBBLE_MID;
            break;
  case 3:
  default:  result |= packet2002->PACKET_DRIBBLE_FAST;
            break;
  }
  
  // Set verticle dribble command
  if (commands[robot].vDribble > 0)     result |= packet2002->PACKET_V_DRIBBLE;

  dataPacket[offsetByte + BYTES_PER_ROBOT * robot + 3] = result;


  //**********************************************************
  //Convert from global (field coordinates)
  //to local (robot) coordinates
  robots2002->convertWheelVelocities(commands[robot].xVel,
                                     commands[robot].yVel,
                                     commands[robot].rotVel,
                                     rotations[robot],
                                     wheels[robot].xVel,
                                     wheels[robot].yVel,
                                     wheels[robot].rotVel);

  //**********************************************************
  // Convert results into packet velocities

  //NOTE THAT THE xVel and yVel are sent as 7+1 bit magnitude sign,
  //signed values.  The rotVel is sent as a 8+1 bit magnitude sign
  
  unsigned char temp_byte;

  //---------------------------------
  // xVelocity, converted to sign magnitude form
  if (wheels[robot].xVel < 0.0f) {
    temp_byte = (char)( 0x7F * (-1 * wheels[robot].xVel) / packet2002->MAX_VELOCITY);
    wheels[robot].xVel = -(packet2002->MAX_VELOCITY * temp_byte) / 0x7F;

    if ((-1 * wheels[robot].xVel) > packet2002->MAX_VELOCITY) {
      wheels[robot].xVel = -packet2002->MAX_VELOCITY;
      temp_byte = 0x7F;
    }
    // Add signed bit
    temp_byte |= 0x80; 
  }
  else {
   temp_byte = (char)( 0x7F * wheels[robot].xVel / packet2002->MAX_VELOCITY);
   wheels[robot].xVel = (packet2002->MAX_VELOCITY * temp_byte) / 0x7F;
   
   if (wheels[robot].xVel > packet2002->MAX_VELOCITY) {
     wheels[robot].xVel = packet2002->MAX_VELOCITY;
     temp_byte = 0x7F;
   }
  }
    
  dataPacket[offsetByte + BYTES_PER_ROBOT * robot + 0] = temp_byte;
  

  //---------------------------------
  // yVelocity, converted to sign magnitude form
  if (wheels[robot].yVel < 0.0f) {
    temp_byte = (char)( 0x7F * (-1 * wheels[robot].yVel) / packet2002->MAX_VELOCITY);
    wheels[robot].yVel = -(packet2002->MAX_VELOCITY * temp_byte) / 0x7F;

    if ((-1 * wheels[robot].yVel) > packet2002->MAX_VELOCITY) {
      wheels[robot].yVel = -packet2002->MAX_VELOCITY;
      temp_byte = 0x7F;
    }
    // Add signed bit
    temp_byte |= 0x80; 
  }
  else {
    temp_byte = (char)( 0x7F * wheels[robot].yVel / packet2002->MAX_VELOCITY);
    wheels[robot].yVel = (packet2002->MAX_VELOCITY * temp_byte) / 0x7F;

    if (wheels[robot].yVel > packet2002->MAX_VELOCITY) {
      wheels[robot].yVel = packet2002->MAX_VELOCITY;
      temp_byte = 0x7F;
    }
  }

  dataPacket[offsetByte + BYTES_PER_ROBOT * robot + 1] = temp_byte;

  //---------------------------------
  // Rotational Velocity
  if (wheels[robot].rotVel < 0.0f) {
    temp_byte = (char)( 0xFF * (-1*wheels[robot].rotVel) / packet2002->MAX_ANGULAR_VELOCITY);
    wheels[robot].rotVel = -(packet2002->MAX_ANGULAR_VELOCITY * temp_byte) / 0xFF;

    if ((-1 * wheels[robot].rotVel) > packet2002->MAX_ANGULAR_VELOCITY) {
      wheels[robot].rotVel = -packet2002->MAX_ANGULAR_VELOCITY;
      temp_byte = 0xFF;
    }
    // Add signed bit
    dataPacket[offsetByte + BYTES_PER_ROBOT * robot + 3] |= packet2002->PACKET_ANGULAR_VELOCITY;
  }
  else {
    temp_byte = (char)( 0xFF * (wheels[robot].rotVel) / packet2002->MAX_ANGULAR_VELOCITY);
    wheels[robot].rotVel = (packet2002->MAX_ANGULAR_VELOCITY * temp_byte) / 0xFF;

    if (wheels[robot].rotVel > packet2002->MAX_ANGULAR_VELOCITY) {
      wheels[robot].rotVel = packet2002->MAX_ANGULAR_VELOCITY;
      temp_byte = 0xFF;
    }
  }
  dataPacket[offsetByte + BYTES_PER_ROBOT * robot + 2] = temp_byte;

  //-----------------------------------
  // Rotate back and store so that prediction works better
  float newX = wheels[robot].xVel;
  float newY = wheels[robot].yVel;
  wheels[robot].xVel = (float)(cos(rotations[robot])*newX + sin(rotations[robot])*newY);
  wheels[robot].yVel = (float)(-sin(rotations[robot])*newX + cos(rotations[robot])*newY);
  //-----------------------------------
}

//-----------------------------------------------------

void WirelessSender::buildRobot2001(int robot, int offsetByte)
{
  //**********************************************************
  // Add parameter bits to packet
  unsigned char result;
  result = 0x00;
  if (commands[robot].dive)             result |= packet2001->PACKET_DIVE;
  if (commands[robot].kick > 0)         result |= packet2001->PACKET_KICK;
  if (commands[robot].dribble > 0)      result |= packet2001->PACKET_DRIBBLE;
  if (commands[robot].vDribble > 0)     result |= packet2001->PACKET_V_DRIBBLE;

  dataPacket[offsetByte + BYTES_PER_ROBOT * robot + 3] = result;

  
  //**********************************************************
  // Convert robot velocities into wheel velocities
  robots2001->convertWheelVelocities( commands[robot].xVel,
                          commands[robot].yVel,
                          commands[robot].rotVel,
                          rotations[robot],
                          wheels[robot].xVel,
                          wheels[robot].yVel,
                          wheels[robot].rotVel);


  //**********************************************************
  // Convert wheel velocities into packet velocities

  wheelOne    = velocityScalingFactor2001 * linearSpeedModifier2001 * wheels[robot].xVel;
  wheelTwo    = velocityScalingFactor2001 * linearSpeedModifier2001 * wheels[robot].yVel;
  wheelThree  = velocityScalingFactor2001 * linearSpeedModifier2001 * wheels[robot].rotVel;

  // Set sign bits:
  // 0x80 for wheel one
  // 0x40 for wheel two
  // 0x20 for wheel three
  if (wheelOne < 0) {
    dataPacket[offsetByte + BYTES_PER_ROBOT * robot + 3] |= packet2001->PACKET_WHEEL_ONE;
    wheelOne *= -1;
  }
  if (wheelTwo < 0) {
    dataPacket[offsetByte + BYTES_PER_ROBOT * robot + 3] |= packet2001->PACKET_WHEEL_TWO;
    wheelTwo *= -1;
  }
  if (wheelThree < 0) {
    dataPacket[offsetByte + BYTES_PER_ROBOT * robot + 3] |= packet2001->PACKET_WHEEL_THREE;
    wheelThree *= -1;
  }

  // Build 8 bit velocity
  unsigned short wheelCommand;
  unsigned char temp_byte;

  // Wheel One
  wheelCommand = ((int) wheelOne ) >> bitsToIgnore2001;
  if( wheelCommand > 0x3FF ) wheelCommand = 0x3FF;

  temp_byte = wheelCommand >> 2 ;
  dataPacket[offsetByte + BYTES_PER_ROBOT * robot + 0] = temp_byte;

  // Wheel Two
  wheelCommand = ((int) wheelTwo ) >> bitsToIgnore2001;
  if( wheelCommand > 0x3FF ) wheelCommand = 0x3FF;

  temp_byte = wheelCommand >> 2 ;
  dataPacket[offsetByte + BYTES_PER_ROBOT * robot + 1] = temp_byte;

  // Wheel Three
  wheelCommand = ((int) wheelThree ) >> bitsToIgnore2001;
  if( wheelCommand > 0x3FF ) wheelCommand = 0x3FF;

  temp_byte = wheelCommand >> 2 ;
  dataPacket[offsetByte + BYTES_PER_ROBOT * robot + 2] = temp_byte;

}

//-----------------------------------------------------

void WirelessSender::buildRobot2000(int robot, int offsetByte)
{
  //**********************************************************
  // Add parameter bits to packet
  unsigned char result;
  result = 0x00;
  if (commands[robot].dive)             result |= packet2001->PACKET_DIVE;
  if (commands[robot].kick > 0)         result |= packet2001->PACKET_KICK;
  if (commands[robot].dribble > 0)      result |= packet2001->PACKET_DRIBBLE;
  if (commands[robot].vDribble > 0)     result |= packet2001->PACKET_V_DRIBBLE;

  dataPacket[offsetByte + BYTES_PER_ROBOT * robot + 3] = result;

  
  //**********************************************************
  // Convert robot velocities into wheel velocities
  robots2000->convertWheelVelocities( commands[robot].xVel,
                          commands[robot].yVel,
                          commands[robot].rotVel,
                          rotations[robot],
                          wheels[robot].xVel,
                          wheels[robot].yVel,
                          wheels[robot].rotVel);


  //**********************************************************
  // Convert wheel velocities into packet velocities

  wheelOne    = velocityScalingFactor2000 * linearSpeedModifier2000 * wheels[robot].xVel;
  wheelTwo    = velocityScalingFactor2000 * linearSpeedModifier2000 * wheels[robot].yVel;
  wheelThree  = velocityScalingFactor2000 * linearSpeedModifier2000 * wheels[robot].rotVel;

  // Set sign bits:
  // 0x80 for wheel one
  // 0x40 for wheel two
  // 0x20 for wheel three
  if (wheelOne < 0) {
    dataPacket[offsetByte + BYTES_PER_ROBOT * robot + 3] |= packet2001->PACKET_WHEEL_ONE;
    wheelOne *= -1;
  }
  if (wheelTwo < 0) {
    dataPacket[offsetByte + BYTES_PER_ROBOT * robot + 3] |= packet2001->PACKET_WHEEL_TWO;
    wheelTwo *= -1;
  }
  if (wheelThree < 0) {
    dataPacket[offsetByte + BYTES_PER_ROBOT * robot + 3] |= packet2001->PACKET_WHEEL_THREE;
    wheelThree *= -1;
  }

  // Build 8 bit velocity
  unsigned short wheelCommand;
  unsigned char temp_byte;

  // Wheel One
  wheelCommand = ((int) wheelOne ) >> bitsToIgnore2000;
  if( wheelCommand > 0x3FF ) wheelCommand = 0x3FF;

  temp_byte = wheelCommand >> 2 ;
  dataPacket[offsetByte + BYTES_PER_ROBOT * robot + 0] = temp_byte;

  // Wheel Two
  wheelCommand = ((int) wheelTwo ) >> bitsToIgnore2000;
  if( wheelCommand > 0x3FF ) wheelCommand = 0x3FF;

  temp_byte = wheelCommand >> 2 ;
  dataPacket[offsetByte + BYTES_PER_ROBOT * robot + 1] = temp_byte;

  // Wheel Three
  wheelCommand = ((int) wheelThree ) >> bitsToIgnore2000;
  if( wheelCommand > 0x3FF ) wheelCommand = 0x3FF;

  temp_byte = wheelCommand >> 2 ;
  dataPacket[offsetByte + BYTES_PER_ROBOT * robot + 2] = temp_byte;

}