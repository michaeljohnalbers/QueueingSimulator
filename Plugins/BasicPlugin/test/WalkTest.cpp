/**
 * @file WalkTest.cpp
 * @brief Unit tests for Walk behavior class
 *
 * @author Michael Albers
 */

#include "gtest/gtest.h"
#include "EigenHelper.h"
#include "Walk.h"

GTEST_TEST(WalkTest, testWalk)
{
  QS::PluginEntity::Properties properties;
  ASSERT_NO_THROW(QS::Walk temp(properties, ""));

  QS::Walk walkBehavior(properties, "");
  float oneSecond = 1.0;
  Eigen::Vector2f expectedMotionVector(1.0, 0);
  auto actualMotionVector = walkBehavior.evaluate(nullptr, oneSecond);
  EXPECT_EQ(expectedMotionVector, actualMotionVector);

  float oneMillisecond = 0.001;
  expectedMotionVector << 0.001, 0;
  actualMotionVector = walkBehavior.evaluate(nullptr, oneMillisecond);
  EXPECT_FLOAT_EQ(expectedMotionVector.x(), actualMotionVector.x())
    << "Actual X: " << actualMotionVector.x() << std::endl;
  EXPECT_FLOAT_EQ(expectedMotionVector.y(), actualMotionVector.y())
    << "Actual Y: " << actualMotionVector.y() << std::endl;

  // Test at a movie-type frame rate (think batch mode).
  float twentyFourFramesPerSecond = 1.0/24.0;
  expectedMotionVector << 0.0416666, 0;
  actualMotionVector = walkBehavior.evaluate(nullptr,
                                             twentyFourFramesPerSecond);
  EXPECT_NEAR(expectedMotionVector.x(), actualMotionVector.x(),
              QS::FLOAT_TOLERANCE)
    << "Actual X: " << actualMotionVector.x() << std::endl;
  EXPECT_FLOAT_EQ(expectedMotionVector.y(), actualMotionVector.y())
    << "Actual Y: " << actualMotionVector.y() << std::endl;


  // Test using the speed property.
  properties["speed"] = "2.5";
  QS::Walk walkBehavior2(properties, "");
  expectedMotionVector << 2.5, 0;
  actualMotionVector = walkBehavior2.evaluate(nullptr, oneSecond);
  EXPECT_EQ(expectedMotionVector, actualMotionVector);

  // Test using invalid speed property.
  properties["speed"] = "r1.2";
  EXPECT_THROW(QS::Walk(properties, ""), std::logic_error);
}
