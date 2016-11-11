#pragma once

/**
 * @file EigenHelper.h
 * @brief Miscellaneous helper things for working with Eigen.
 *
 * @author Michael Albers
 */

#include "Eigen/Core"

namespace QS
{
  /**
   * Class containing various items to make using Eigen easier/nicer.
   */
  class EigenHelper
  {
    public:

    /**
     * Format for printing Eigen::Vector2f
     * (v.format(QS::EigenHelper::prettyPrint))
     */
    static const Eigen::IOFormat prettyPrint;

    /** Tolerance for comparisons. See Eigen isApprox function. */
    static const float FLOAT_TOLERANCE;

    /**
     * Truncates the vector so that it's length is no longer than the given
     * maximum. If the maximum length is smaller than the vector, or if it is
     * negative the vector is returned unaltered. This function does not alter
     * the orientation of the vector, just the magnitude.
     *
     * @param theVector
     *          vector to truncate
     * @param theMaximumLength
     *          maximum length to truncate to
     * @return vector no longer than the maximum
     */
    static Eigen::Vector2f truncate(Eigen::Vector2f theVector,
                                    float theMaximumLength) noexcept;
  };
}
