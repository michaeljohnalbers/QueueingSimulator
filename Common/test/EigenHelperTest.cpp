/**
 * @file EigenHelperTest.cpp
 * @brief Unit tests for EigenHelper class
 *
 * @author Michael Albers
 */

#include "gtest/gtest.h"
#include "EigenHelper.h"


GTEST_TEST(EigenHelperTest, truncate)
{
  // No truncation, length is less than zero.
  {
    Eigen::Vector2f vector(3.5, 3.5);
    auto newVector = QS::EigenHelper::truncate(vector, -0.1);
    EXPECT_EQ(vector, newVector)
      << "newVector == "<< newVector.format(QS::EigenHelper::prettyPrint);
  }

  // No truncation, length is greater than vector length
  {
    Eigen::Vector2f vector(3.5, 3.5);
    float maxLength = vector.norm();
    auto newVector = QS::EigenHelper::truncate(vector, maxLength);
    EXPECT_EQ(vector, newVector)
      << "newVector == "<< newVector.format(QS::EigenHelper::prettyPrint);
  }

  // Tuncation to zero.
  {
    Eigen::Vector2f vector(3.5, 3.5);
    auto newVector = QS::EigenHelper::truncate(vector, 0);
    EXPECT_EQ(Eigen::Vector2f(0,0), newVector)
      << "newVector == "<< newVector.format(QS::EigenHelper::prettyPrint);
  }

  // Tuncation to non-zero length.
  {
    Eigen::Vector2f vector(3.5, 3.5);
    auto newVector = QS::EigenHelper::truncate(vector, 3.0);
    auto val = std::sqrt(4.5f);
    EXPECT_EQ(Eigen::Vector2f(val,val), newVector)
      << "newVector == "<< newVector.format(QS::EigenHelper::prettyPrint);
  }
}

