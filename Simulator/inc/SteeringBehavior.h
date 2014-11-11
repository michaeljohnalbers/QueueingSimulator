#ifndef _SteeringBehavior_h_
#define _SteeringBehavior_h_

/**
 * @file SteeringBehavior.h
 * @brief Base class used to define the interface for all steering behviors.
 *
 * @author Michael Albers
 */

#include <Eigen/Core>

class Individual;
class NearestN;

/**
 * SteeringBehavior defines an interface which all specific steering behaviors
 * will implement.
 */
class SteeringBehavior
{
  // ************************************************************
  // Public
  // ************************************************************
  public:

  /**
   * Destructor.
   */
  virtual ~SteeringBehavior() {}

  /**
   * Calculates the force vector to apply to the Individual to acheive
   * the behavior's goal.
   * @param theIndividual Individual being steered.
   * @param theNeighbors Neighbors of theIndividual. Not necessarily needed
   *        for all behviors. In those cases a null pointer should be provided.
   * @param theTarget Target of the behavior. Not necessarily needed
   *        for all behviors. In those cases a null pointer should be provided.
   * @return Force vector
   */
  virtual Eigen::Vector2f calculateForce(const Individual &theIndividual,
                                         const NearestN *theNeighbors,
                                         const Eigen::Vector2f *theTarget) = 0;

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
