#ifndef _Seek_h_
#define _Seek_h_

/**
 * @file Seek.h
 * @brief Defines the Seek steering behavior.
 *
 * @author Michael Albers
 */

#include <Eigen/Core>

/**
 * This class defines the 'Seek' steering behavior. This behavior
 * simply causes the Individual to move to a specific point.
 * Typically this is a fixed point in space.
 */
class Seek
{
  // ************************************************************
  // Public
  // ************************************************************
  public:

  /**
   * Default constructor.
   */
  Seek() = delete;

  /**
   * Destructor.
   */
  ~Seek() = delete;

  /**
   * Drives an Individual towards a fixed point in space.
   * @param theIndividual Individual being steered.
   * @param theTarget Target being seeked.
   * @return Force vector
   */
  static Eigen::Vector2f calculateForce(const Individual &theIndividual,
                                        const Eigen::Vector2f *theTarget);

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
