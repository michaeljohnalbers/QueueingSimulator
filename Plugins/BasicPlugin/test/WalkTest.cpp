/**
 * @file WalkTest.cpp
 * @brief Unit tests for Walk behavior class
 *
 * @author Michael Albers
 */

#include "gtest/gtest.h"
#include "Walk.h"

GTEST_TEST(WalkTest, testWalk)
{
  QS::PluginEntity::Properties properties;
  ASSERT_NO_THROW(QS::Walk temp(properties));

  QS::Walk walkBehavior(properties);
  std::chrono::milliseconds oneSecond{1000};
  Eigen::Vector2f expectedMotionVector(0.89408, 0);
  auto actualMotionVector = walkBehavior.evaluate(nullptr, oneSecond);
  EXPECT_EQ(expectedMotionVector, actualMotionVector);

  std::chrono::milliseconds oneMillisecond{1};
  expectedMotionVector << 0.00089408, 0;
  actualMotionVector = walkBehavior.evaluate(nullptr, oneMillisecond);
  EXPECT_FLOAT_EQ(expectedMotionVector.x(), actualMotionVector.x())
    << "Actual X: " << actualMotionVector.x() << std::endl;
  EXPECT_FLOAT_EQ(expectedMotionVector.y(), actualMotionVector.y())
    << "Actual Y: " << actualMotionVector.y() << std::endl;

  // Test at a movie-type frame rate (think batch mode). 24 frames per second
  // is actually 41 2/3 milliseconds per frame, but std::chrono::milliseconds
  // only deals in integer numbers. So I rounded up.
  std::chrono::milliseconds twentyFourFramesPerSecond{42};
  expectedMotionVector << 0.03755136, 0;
  actualMotionVector = walkBehavior.evaluate(nullptr, twentyFourFramesPerSecond);
  EXPECT_FLOAT_EQ(expectedMotionVector.x(), actualMotionVector.x())
    << "Actual X: " << actualMotionVector.x() << std::endl;
  EXPECT_FLOAT_EQ(expectedMotionVector.y(), actualMotionVector.y())
    << "Actual Y: " << actualMotionVector.y() << std::endl;
}
