/**
 * @file Target.cpp
 * @brief Unit tests for Targ class
 *
 * @author Michael Albers
 */

#include <stdexcept>
#include "gtest/gtest.h"
#include "Target.h"

GTEST_TEST(TargetTest, class)
{
  QS::PluginEntity::Properties goodProperties{
    {"x", "3.5"},
    {"y", "4.2"},
    {"radius", "56.77"}};
  try
  {
    QS::Target target(goodProperties, "");
    EXPECT_EQ(56.77f, target.getRadius());
    Eigen::Vector2f expectedPosition(3.5, 4.2);
    EXPECT_EQ(expectedPosition, target.getPosition());
  }
  catch (...)
  {
    FAIL() << "Unexpectedly threw exception.";
  }

  // Test all zeros.
  EXPECT_NO_THROW(QS::Target({{"x", "0.0"}, {"y", "0.0"}, {"radius", "0.0"}},
                             ""));

  // Test invalid position x value
  EXPECT_THROW(QS::Target({{"x", "-1.0"}, {"y", "1.0"}, {"radius", "1.0"}}, ""),
               std::invalid_argument);
  // Test invalid position y value
  EXPECT_THROW(QS::Target({{"x", "1.0"}, {"y", "-1.0"}, {"radius", "1.0"}}, ""),
                          std::invalid_argument);
  // Test invalid radius value (< 0)
  EXPECT_THROW(QS::Target({{"x", "1.0"}, {"y", "1.0"}, {"radius", "-1.0"}}, ""),
               std::invalid_argument);
}
