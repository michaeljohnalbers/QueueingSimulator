#ifndef _Seek_h_
#define _Seek_h_

/**
 * @file Seek.h
 * @brief Defines the Seek steering behavior.
 *
 * @author Michael Albers
 */

#include <Eigen/Core>
#include <SteeringBehavior.h>


/**
 */
class Seek : public SteeringBehavior
{
  // ************************************************************
  // Public
  // ************************************************************
  public:

  /**
   * Default constructor.
   */
  Seek();

  /**
   * Destructor.
   */
  virtual ~Seek();

  /**
   * Drives an Individual towards a fixed point in space.
   * @param theIndividual Individual being steered.
   * @param theNeighbors Not needed, ignored.
   * @param theTarget Target being seeked.
   * @return Force vector
   */
  virtual Eigen::Vector2f calculateForce(const Individual &theIndividual,
                                         const NearestN *theNeighbors,
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
