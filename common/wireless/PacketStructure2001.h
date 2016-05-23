#ifndef PACKET_STRUCTURE_2001_H
#define PACKET_STRUCTURE_2001_H

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
 
Byte 1		- Wheel 1 (Left) velocity magnitude 		(Higher 8 bits of the magnitude)
Byte 2		- Wheel 2 (Right) velocity magnitude		(Higher 8 bits of the magnitude)
Byte 3		- Wheel 3 (Forward) velocity magnitude		(Higher 8 bits of the magnitude)
Byte 4

	bit 7 	- Wheel 3 velocity sign
	bit 6 	- Wheel 2 velocity sign
	bit 5 	- wheel 1 velocity sign
	bit 4	- Horizontal Dribbler
	bit 3	- Vertical Dribbler
	bit 2	- Kicker
	bit 1 	- Unused (for now - will be used for kicker)
	bit 0	- Beep

Bit Numbering: 76543210

*/

class PacketStructure2001 {
public:
  char PACKET_DIVE;
  char PACKET_KICK;
  char PACKET_V_DRIBBLE;
  char PACKET_DRIBBLE;
  char PACKET_WHEEL_ONE;
  char PACKET_WHEEL_TWO;
  char PACKET_WHEEL_THREE;

  PacketStructure2001() {
    PACKET_DIVE           = (char)(0x01); // 00000001
    PACKET_KICK           = (char)(0x04); // 00000100
    PACKET_V_DRIBBLE      = (char)(0x08); // 00010000
    PACKET_DRIBBLE        = (char)(0x10); // 00001000
    PACKET_WHEEL_ONE      = (char)(0x80); // 10000000
    PACKET_WHEEL_TWO      = (char)(0x40); // 01000000
    PACKET_WHEEL_THREE    = (char)(0x20); // 00100000
  }
};



//-----------------------------------------------------
// ROBOTS - 2001
//-----------------------------------------------------
static float gear2001             =    12.2f;
static float diameter2001         =     0.040f;  //meters
static float quadrature2001       =     4.0f;
static float counts_per_rev2001   =   100.0f;
static float sample_time_8mhz2001 =     0.000256f;
static float PIDconst2001         = 65536.0f;

static float linearSpeedModifier2001;

static int bitsToIgnore2001       = 11;

static float velocityScalingFactor2001 = 1.00f;  // tested empirically
//-----------------------------------------------------



//-----------------------------------------------------
// ROBOTS - 2000
//-----------------------------------------------------
static float gear2000             =     5.5385f;
//static float diameter2000         =     0.0072f / 0.0039f;  //centimeters
static float diameter2000         =     0.019f;  //meters
static float quadrature2000       =     4.0f;
static float counts_per_rev2000   =   512.0f;
static float sample_time_8mhz2000 =     0.000256f;
static float PIDconst2000         = 65536.0f;

static float linearSpeedModifier2000;

static int bitsToIgnore2000       = 11;

static float velocityScalingFactor2000 = 1.00f;  // tested empirically
//-----------------------------------------------------


#endif