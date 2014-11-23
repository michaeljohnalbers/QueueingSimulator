#ifndef _CollisionAvoidance_h_
#define _CollisionAvoidance_h_

/**
 * @file CollisionAvoidance.h
 * @brief Defines the collision avoidance steering behavior.
 *
 * @author Michael Albers
 */

#include <Eigen/Core>

class Individual;
class NearestN;

/**
 * This class defines the 'Collision Avoidance' steering behavior. This
 * behavior determines the most imminent threat of collision and
 * produces a force to move around it.
 * Based off of description at:
 * http://gamedevelopment.tutsplus.com/tutorials/understanding-steering-behaviors-collision-avoidance--gamedev-7777
 *
 * This is primarily used for avoiding collisions with static objects.
 * It can be used as a rough means to avoid collisions with non-static
 * objects. There are drawbacks to this. Namely, the force produced would
 * not account for the direction of motion of the non-static object.
 */
class CollisionAvoidance
{
  // ************************************************************
  // Public
  // ************************************************************
  public:

  /**
   * Constructor.
   */
  CollisionAvoidance() = delete;

  /**
   * Destructor.
   */
  ~CollisionAvoidance() = delete;

  /**
   * Moves an Individual aware from a potential collision. If no
   * potential collision is detected a null force is returned.
   * @param theIndividual Individual being steered.
   * @param theNeighbors Neighbors of theIndividual.
   * @param theTarget Target being seeked.
   * @return Force vector
   */
  static Eigen::Vector2f calculateForce(const Individual &theIndividual,
                                        const NearestN *theNeighbors);

  // ************************************************************
  // Protected
  // ************************************************************
  protected:

  /**
   * Determines the most imminent thread amongs the nearest neighbors.
   * @param theIndividual Individual search is based around.
   * @param theNeighbors Neighbors of the Individual.
   * @param theLookAhead First look ahead vector
   * @param theLookAhead2 Second look ahead vector (half magnitude of theAhead)
   * @return Individual which poses the largest threat of collision or
   * nullptr if no Individual poses a threat.
   */
  static const Individual* findImminentThreat(
    const Individual &theIndividual,
    const NearestN *theNeighbors,
    const Eigen::Vector2f &theLookAhead,
    const Eigen::Vector2f &theLookAhead2);

  // ************************************************************
  // Private
  // ************************************************************
  private:

};

#endif
