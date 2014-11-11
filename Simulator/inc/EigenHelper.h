#ifndef _EigenHelper_h_
#define _EigenHelper_h_

/**
 * @file EigenHelper.h
 * @brief Provides some static helper functions for using the Eigen matrix
 * library.
 *
 * @author Michael Albers
 */

#include <string>

#include <Eigen/Core>

class EigenHelper
{
  // ************************************************************
  // Public
  // ************************************************************
  public:

  /**
   * Calculates the distance between the two points.
   * @param theFirstPoint Point
   * @param theSecondPoint Point
   * @return Distance between the two points
   */
  static float distance(const Eigen::Vector2f &theFirstPoint,
                        const Eigen::Vector2f &theSecondPoint);

  /**
   * Prints the vector in a nicer form than the Eigen library provides.
   */
  static std::string print(const Eigen::Vector2f &theVector);

  /**
   * Ensures the given vector's magnitude is not larger than the
   * given magnitude. If so returns a vector scaled down to that magnitude.
   * @param theVector Vector to truncate
   * @param theMaximumMagnitude Maximum magnitude for theVector
   * @return Scaled vector (If magnitude is less than theMaximumMagnitude
   * the vector will not be scaled.)
   */
  static Eigen::Vector2f truncate(const Eigen::Vector2f &theVector,
                                  float theMaximumMagnitude);

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
