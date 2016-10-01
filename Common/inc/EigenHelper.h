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
  /** Format for printing Eigen::Vector2f (v.format(QS::prettyPrint)) */
  static const Eigen::IOFormat prettyPrint(
    8, Eigen::DontAlignCols, "", ",", "", "", "", "");

  /** Tolerance for comparisons. See Eigen isApprox function. */
  static constexpr float FLOAT_TOLERANCE = 0.0001;
}
