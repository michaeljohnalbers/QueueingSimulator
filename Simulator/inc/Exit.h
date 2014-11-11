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

  static constexpr float EXIT_RADIUS = 1.0;

  static Eigen::Vector2f myExit;
  static int32_t myRankToExit;
};

#endif
