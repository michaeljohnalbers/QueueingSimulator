#ifndef _Exit_h_
#define _Exit_h_

/**
 * @file Exit.h
 * @brief Tracks the location of the exit and the next Individual which
 *        can exit.
 *
 * @author Michael Albers
 */

#include <cstdint>

#include <Eigen/Core>

class Individual;

/**
 * This class defines the exit point of the world. All Individuals are
 * attempting to move towards this point. This class also tracks the
 * rank of the Individual which can currently exit.
 */
class Exit
{
  // ************************************************************
  // Public
  // ************************************************************
  public:

  /**
   * Default constructor.
   */
  Exit() = delete;

  /**
   * Determines if the given individual can exit the world. If so, increments
   * the rank which can exit.
   * @param theIndividual Individual to check.
   * @return True if the individual can exit, false otherwise.
   */
  static bool canExit(const Individual &theIndividual);

  /**
   * Returns the position of the exit.
   * @return Exit position.
   */
  static const Eigen::Vector2f& getPosition();

  /**
   * Returns the exit radius.
   * @return Exit radius (m).
   */
  static float getRadius();

  /**
   * Returns the rank of the next Individual which can exit
   * @return Rank of individual which can exit.
   */
  static int32_t getRankToExit();

  /**
   * Sets the exit position.
   * @param theExit Positon of the exit.
   */
  static void setPosition(const Eigen::Vector2f &theExit);

  // ************************************************************
  // Protected
  // ************************************************************
  protected:

  // ************************************************************
  // Private
  // ************************************************************
  private:

  /** Radius of the exit area. */
  static constexpr float EXIT_RADIUS = 1.0;

  /** Position of the exit. */
  static Eigen::Vector2f myExit;

  /** Rank of the Individual which can exit. */
  static int32_t myRankToExit;
};

#endif
