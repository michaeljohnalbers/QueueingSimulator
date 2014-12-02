#ifndef _Flee_h_
#define _Flee_h_

/**
 * @file Flee.h
 * @brief Defines the Flee steering behavior.
 *
 * @author Michael Albers
 */

#include <Eigen/Core>

/**
 * This class defines the 'Flee' steering behavior. This behavior
 * simply causes the Individual to move away from a specific point.
 * Based off of description at:
 * http://gamedevelopment.tutsplus.com/tutorials/understanding-steering-behaviors-flee-and-arrival--gamedev-1303
 */
class Flee
{
  // ************************************************************
  // Public
  // ************************************************************
  public:

  /**
   * Default constructor.
   */
  Flee() = delete;

  /**
   * Destructor.
   */
  ~Flee() = delete;

  /**
   * Drives an Individual away from a point in space.
   * @param theIndividual Individual being steered.
   * @param theTarget Target being seeked.
   * @return Force vector
   */
  static Eigen::Vector2f calculateForce(const Individual &theIndividual,
                                        const Eigen::Vector2f &theTarget);

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
