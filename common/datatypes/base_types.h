/***************************
 * Data types: Mathematics *
 ***************************/

#ifndef DATATYPES_BASE_TYPES_H
#define DATATYPES_BASE_TYPES_H

//use a forward declaration of Pair so that math.h can reference it.
class Pair;

#include "math/cmath.h"

/*********************
 * Pair              *
 *********************/
class Pair
{
 public:
  /* constructors */
  // initialize to 0,0.
  Pair() {
    x = y = 0;
  }
  
  Pair (const Pair& p) {
    this->x=p.x; 
    this->y=p.y;
  }

  Pair (float a, float b) {
    x=a; y=b;
  }

  // get/set functions for the pair
  inline float getX (void) const { return x; }
  inline float getY (void) const { return y; }
  inline void setX (float f) { x = f; }
  inline void setY (float f) { y = f; }
  inline void set(const Pair& pair) {
	  this->x = pair.x;
	  this->y = pair.y;
  }
  inline void set(float x_, float y_) {
	  this->x = x_;
	  this->y = y_;
  }

  float distanceTo (const Pair &other) const {
    return distanceTo(other.getX(), other.getY());
  }

  float distanceTo (float otherX, float otherY) const {
    float dy = y - otherY;
    float dx = x - otherX;
    return SQRT( (dy*dy) + (dx*dx) );
  }

  float squareDistanceTo (const Pair &other) const {

    float dy = y - other.y;
    float dx = x - other.x;
    return static_cast<float>(dy*dy + dx*dx);
  }

  // returns the distance between (x, y) and the origin.
  // (e.g. if the pair repreasents velocity, this returns the scalar speed)
  float magnitude() const {
    return SQRT(x*x + y*y);
  }
  
  /// Swaps with another pair
  void swap(Pair &pair) {
    Pair temp;
    temp.set(this->x,this->y);
    this->x = pair.getX();
    this->y = pair.getY();
    pair.set(temp);
  }

  // returns true if pair falls on line between
  // two other points
  bool isBetween(const Pair &a, const Pair&b)
  {
    float distance1 = a.distanceTo(x,y);
    float distance2 = b.distanceTo(x,y);
    float realSum = a.distanceTo(b);
    return (distance1 <= realSum &&
            distance2 <= realSum);
  }

  bool operator==(const Pair& other) const {
    return (this->x == other.x) && (this->y == other.y);
  }
  bool operator!=(const Pair& other) const {
    return (this->x != other.x) || (this->y != other.y);
  }

 protected:
  float x, y;
};

/*********************
 * Line              * 
 *********************/

class Line
{
 public:
  /* constructors */
  // by default, initialize both pairs to (0,0)
  Line () :
    pointA(0,0), pointB(0,0)
    { }
  
  Line (const Pair &p, const Pair &q) :
    pointA(p), pointB(q) // use default copy constructor 
    { }
 
  Line (float x1, float y1, float x2, float y2) :
    pointA(x1, y1),
    pointB(x2, y2)
    { }

  /* get/set functions */
  // return value because otherwise, can modify
  // value by mangling w/pointer
  Pair getA (void)  const { return pointA; }
  Pair getB (void)  const { return pointB; }
  void setA (const Pair &p) { pointA.set(p); }
  void setB (const Pair &p) { pointB.set(p); }

  float getAposX(void) const { return pointA.getX(); }
  float getAposY(void) const { return pointA.getY(); }
  float getBposX(void) const { return pointB.getX(); }
  float getBposY(void) const { return pointB.getY(); }
  void setAposX(float posX) { pointA.setX(posX); }
  void setAposY(float posY) { pointA.setY(posY); }
  void setBposX(float posX) { pointB.setX(posX); }
  void setBposY(float posY) { pointB.setY(posY); }

  float getSlope(void) const {
    return (pointA.getX() == pointB.getX()) 
      ? 1000000.0f //don't divide by zero
      : (pointA.getY() - pointB.getY())/(pointA.getX() - pointB.getX());
  }
  
  float getYIntercept(void) const {
    return pointA.getY() - pointA.getX() * getSlope();
  }

 private:
  Pair pointA, pointB;

};

/*******
 * Box *
 *******/
class Box
{
  public:
  /* constructors */
  //default init left and right to 0, top and bottom to 0
  Box()
  {
    left = 0; 
    right = 0;
    top = 0;
    bottom = 0;
  }

  Box(float sleft, 
      float sright, 
      float stop, 
      float sbottom)
  {
    left = sleft;
    right = sright;
    top = stop; //stop. ha! pretty funny. not planned
    bottom = sbottom;
  }
  
  void boxAt (const Pair & loc)
  {
    left = loc.getX();
    right = loc.getX();
    top = loc.getY();
    bottom = loc.getY();
  }

  void pad (const float & thickness)
  {
    left -= thickness;
    right += thickness;
    top += thickness;
    bottom -= thickness;
  }

  bool contains (Pair loc)
  {
    if (loc.getX() < left)          return false;
    else if (loc.getX() > right)    return false;
    else if (loc.getY() < bottom)   return false;
    else if (loc.getY() > top)      return false;
    else                            return true;

  }

  //setters and getters
  void setLeft (const float& sleft) { left = sleft; }
  void setRight (const float& sright) { right = sright; }
  void setTop (const float& stop) { top = stop; }
  void setBottom (const float& sbottom) { bottom = sbottom; }

  float getLeft (void) const { return left; }
  float getRight (void) const { return right; }
  float getTop (void) const { return top; }
  float getBottom (void) const { return bottom; }

  private:
    float left, right, top, bottom;
};


/*********************
 * Polygon3          *
 *********************/

class Polygon3
{
 public:
  /* constructors */
  // by default, initialize all pairs to (0,0)
  Polygon3 () :
    a(), b(), c()
    { }
  
  Polygon3 (const Pair &p,
	    const Pair &q,
	    const Pair &r) :
    a(p), b(q), c(r)
    { }
 
  /* get/set functions */
  Pair getA(void)  const { return a; }
  Pair getB(void)  const { return b; }
  Pair getC(void)  const { return c; }
  void setA(const Pair &p) { a = p; }
  void setB(const Pair &p) { b = p; }
  void setC(const Pair &p) { c = p; }  

  // does polygon contain a point?
  // WARNING: not yet implemented
 // bool contains(const Pair &p) {}
  
 private:
  Pair a, b, c;

};


/*********************
 * Polygon4          *
 *********************/

class Polygon4: public Polygon3
{
 public:
  /* constructors */
  // by default, initializes all pairs to (0,0)
  Polygon4 () :
    a(), b(), c(), d()
    { }
  
  Polygon4 (const Pair &p,
	    const Pair &q,
	    const Pair &r,
	    const Pair &s) :
    // use default copy constructor
	   a(p), b(q), c(r), d(s)
    { }
 
  /* get/set functions */
  // this class inherits all funcs for a, b, c from Polygon3
  Pair getD (void)  const { return d; }
  void setD (const Pair &p) { d = p; }  

  // does polygon contain a point?
  // WARNING: not yet implemented
  //bool contains(const Pair &p) {}

 private:
  Pair a, b, c, d;

};

#endif // DATATYPES_BASE_TYPES_H







