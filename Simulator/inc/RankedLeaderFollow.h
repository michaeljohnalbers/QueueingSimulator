#ifndef _RankedLeaderFollow_h_
#define _RankedLeaderFollow_h_

/**
 * @file RankedLeaderFollow.h
 * @brief Defines the RankedLeaderFollow steering behavior.
 *
 * @author Michael Albers
 */

#include <Eigen/Core>

class Individual;
class NearestN;

/**
 * This class defines the 'RankedLeaderFollow' steering behavior. This behavior
 * causes an Individual to move towards the Individual with the closest,
 * higher rank. If no such Individual is nearby, the Individual will move
 * towards the exit.
 * Based off of description at:
 * http://gamedevelopment.tutsplus.com/tutorials/understanding-steering-behaviors-leader-following--gamedev-10810
 */
class RankedLeaderFollow
{
  // ************************************************************
  // Public
  // ************************************************************
  public:

  /**
   * Default constructor.
   */
  RankedLeaderFollow() = delete;

  /**
   * Destructor.
   */
  ~RankedLeaderFollow() = delete;

  /**
   * Drives an Individual towards a leader Individual.
   * @param theIndividual Individual being steered.
   * @param theNeighbors Neighbors of theIndividual.
   * @return Force vector
   */
  static Eigen::Vector2f calculateForce(const Individual &theIndividual,
                                        const NearestN &theNeighbors);

  // ************************************************************
  // Protected
  // ************************************************************
  protected:

  /**
   * Determines if the Individual is in the way of the leader.
   * @param theIndividualPosition Position of the Individual
   * @param theLeaderPosition Position of the leader
   * @param theLeaderAheadPosition Position some amount ahead of the leader.
   */
  static bool inWayOfLeader(const Eigen::Vector2f &theIndividualPosition,
                            const Eigen::Vector2f &theLeaderPosition,
                            const Eigen::Vector2f &theLeaderAheadPosition);

  // ************************************************************
  // Private
  // ************************************************************
  private:
};

#endif
