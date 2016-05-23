
#ifndef DATATYPES_PASS_H
#define DATATYPES_PASS_H

#include "base_types.h"


/**************************
 * Pass:                  *
 * Allows easy setting of *
 * pass position and      *
 * associated pass value  *
 **************************/
class Pass
{
  public:
    //constructor
    Pass() 
    {
      initialize();
    }

   //Initializes all desination data
   void initialize (void)
   {    
     //Default destinatinon position
     passPos.setX(0.0f);
     passPos.setY(0.0f);

     //Default pass value (not valid)
     passValue = 0;
   }

   void degrade()
   {
     if(passValue >=1)
     {
       passValue--;
     }
   }

   //Setter's and getter's
   Pair getPassPos (void) const { return passPos; }
   float getPassPosX (void) const { return passPos.getX(); }
   float getPassPosY (void) const { return passPos.getY(); }

   void setPassPos (Pair pos) { passPos = pos; }
   void setPassPosX (float x) { passPos.setX(x); }
   void setPassPosY (float y) { passPos.setY(y); }

   bool getPassValue()
   {
     return (passValue >=1);
   }

   void setPassValue(bool val)
   {
      if(val)
      {
        passValue = 2;
      }
      else
      {
        passValue = 0;
      }
   }

  private:
     Pair passPos;
     int passValue;
};



#endif // DATATYPES_DESTINATION_H
