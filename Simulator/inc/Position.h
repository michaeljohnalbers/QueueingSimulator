#ifndef _Position_h_
#define _Position_h_

/**
 * @file Position.h
 * @brief Defines a point in the world.
 *
 * @author Michael Albers
 */


/**
 * Represents a single cartesian coordinate point.
 */
class Position
{
  // ************************************************************
  // Public
  // ************************************************************
  public:

  /**
   * Default constructor.
   */
  Position();

  /**
   * Constructor.
   * @param theX X coordinate.
   */
  Position(float theX, float theY);

  /**
   * Copy constructor.
   * @param thePosition Object to copy.
   */
  Position(const Position &thePosition);

  /**
   * Destructor.
   */
  ~Position();

  /**
   * Assignment operator.
   * @param thePosition Object to copy.
   * @return This object.
   */
  Position& operator=(const Position &thePosition);

  /**
   * Returns the distance between the two points.
   * @param thePosition Other Position for distance calculation.
   * @return Distance between points.
   */
  float distance(const Position &thePosition) const;

  /**
   * Returns the X coordinate.
   * @return X coordinate.
   */
  float getX() const;

  /**
   * Returns the Y coordinate.
   * @return Y coordinate.
   */
  float getY() const;

  /**
   * Sets the X coordinate.
   * @param theX X coordinate.
   */
  void setX(float theX);

  /**
   * Sets the Y coordinate.
   * @param theY Y coordinate.
   */
  void setY(float theY);

  // ************************************************************
  // Protected
  // ************************************************************
  protected:

  // ************************************************************
  // Private
  // ************************************************************
  private:

  /** X coordinate. */
  float myX = 0.0;

  /** Y coordinate. */
  float myY = 0.0;
};

#endif
