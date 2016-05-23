#ifndef PACKET_STRUCTURE_2002_H
#define PACKET_STRUCTURE_2002_H

/**
2000, 2001 Packet Format 

1st Packet

Byte  0 	- Packet Identifier.  Bit 8 - '0'  (identifies this as first of 2 packets).
Byte  1:4	- Robot0  (4 bytes)
Byte  5:8	- Robot1  (4 bytes)
Byte  9:12	- Robot2  (4 bytes)
Byte 13:16	- Robot3  (4 bytes)
Byte 17:20	- Robot4  (4 bytes)
Byte 21:24	- Robot5  (4 bytes)

2nd Packet

Byte  0 	- Packet Identifier.  Bit 8 - '1'  (identifies this as second of 2 packets).
Byte  1:4	- Robot6  (4 bytes)
Byte  5:8	- Robot7  (4 bytes)
Byte  9:12	- Robot8  (4 bytes)
Byte 13:16	- Robot9  (4 bytes)
Byte 17:20	- Robot10 (4 bytes)
Byte 21:24	- Robot11 (4 bytes)


For each robot
 
Byte 1		- X Velocity
Byte 2		- Y Velocity
Byte 3		- Angular Velocity Magnitude
Byte 4

	bit 7 	- Angular Velocity sign
	bit 6 	- Dive
	bit 5 	- Vertical Dribbler
	bit 4	  - Horizontal Dribbler
	bit 3	  - Horizontal Dribbler
	bit 2	  - Kick
	bit 1 	- Kick
	bit 0	  - Kick

Bit Numbering: 76543210

*/

class PacketStructure2002 {
public:
  char PACKET_ANGULAR_VELOCITY;
  char PACKET_DIVE;
  char PACKET_V_DRIBBLE;
  char PACKET_DRIBBLE_FAST;
  char PACKET_DRIBBLE_MID;
  char PACKET_DRIBBLE_SLOW;
  char PACKET_KICK_FAST;
  char PACKET_KICK_PASS_1;
  char PACKET_KICK_PASS_2;
  char PACKET_KICK_PASS_3;
  char PACKET_KICK_PASS_4;
  char PACKET_KICK_PASS_5;
  char PACKET_KICK_SLOW;

  float MAX_VELOCITY;
  float MAX_ANGULAR_VELOCITY;

  PacketStructure2002() {
    PACKET_ANGULAR_VELOCITY     = (char)(0x80); // 10000000
    PACKET_DIVE                 = (char)(0x40); // 01000000
    PACKET_V_DRIBBLE            = (char)(0x20); // 00110000
    PACKET_DRIBBLE_FAST         = (char)(0x18); // 00011000
    PACKET_DRIBBLE_MID          = (char)(0x10); // 00010000
    PACKET_DRIBBLE_SLOW         = (char)(0x08); // 00001000
    PACKET_KICK_FAST            = (char)(0x07); // 00000111
    PACKET_KICK_PASS_5          = (char)(0x06); // 00000110
    PACKET_KICK_PASS_4          = (char)(0x05); // 00000101
    PACKET_KICK_PASS_3          = (char)(0x04); // 00000100
    PACKET_KICK_PASS_2          = (char)(0x03); // 00000011
    PACKET_KICK_PASS_1          = (char)(0x02); // 00000010
    PACKET_KICK_SLOW            = (char)(0x01); // 00000001

    MAX_VELOCITY            = 2.5f;
    MAX_ANGULAR_VELOCITY    = 10.0f;
  }
};

#endif