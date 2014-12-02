#ifndef _Arrive_h_
#define _Arrive_h_

/**
 * @file Arrive.h
 * @brief Defines the Arrive steering behavior.
 *
 * @author Michael Albers
 */

#include <Eigen/Core>

class Individual;

/**
 * This class defines the 'Arrive' steering behavior. This behavior is
 * similar to 'Seek' except that as the Individual gets closer to the point
 * it is seeking it slows down until the Individual eventually stops.
 * Based off of description at:
 * http://gamedevelopment.tutsplus.com/tutorials/understanding-steering-behaviors-flee-and-arrival--gamedev-1303
 */
class Arrive
{
  // ************************************************************
  // Public
  // ************************************************************
  public:

  /**
   * Default constructor.
   */
  Arrive() = delete;

  /**
   * Destructor.
   */
  ~Arrive() = delete;

  /**
   * Drives an Individual towards a point in space, slowing as the Individual
   * nears the point.
   * @param theIndividual Individual being steered.
   * @param theTarget Target being seeked.
   * @param theSlowingRadius Radius, in meters, from thePosition when
   *        theIndividual should start slowing down.
   * @return Force vector
   */
  static Eigen::Vector2f calculateForce(const Individual &theIndividual,
                                        const Eigen::Vector2f &theTarget,
                                        float theSlowingRadius);

  // ************************************************************
  // Protected
  // ************************************************************
  protected:

  // ************************************************************
  // Private
  // ************************************************************
  private:

};

#endif
