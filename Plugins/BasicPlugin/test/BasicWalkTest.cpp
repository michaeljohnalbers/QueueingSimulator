/**
 * @file BasicWalkTest.cpp
 * @brief Unit tests for BasicWalk behavior set class
 *
 * @author Michael Albers
 */

#include "gtest/gtest.h"
#include "BasicWalk.h"
#include "Sensable.h"
#include "Walk.h"

GTEST_TEST(BasicWalkTest, testBasicWalk)
{
  // BasicWalk just returns the value of Walk.
  QS::PluginEntity::Properties properties;
  ASSERT_NO_THROW(QS::BasicWalk temp(properties));

  QS::BasicWalk basicWalk(properties);
  QS::Walk walk(properties);
  QS::EntityDependency<QS::Behavior> basicWalkDependencies{"Walk", &walk, ""};
  basicWalk.setDependencies({basicWalkDependencies});

  {
    std::chrono::milliseconds oneSecond{1000};
    QS::Sensable sensable({}, oneSecond);
    Eigen::Vector2f expectedMotionVector(0.89408, 0);
    auto actualMotionVector = basicWalk.evaluate(nullptr, sensable);
    EXPECT_FLOAT_EQ(expectedMotionVector.x(), actualMotionVector.x())
      << "Actual X: " << actualMotionVector.x() << std::endl;
    EXPECT_FLOAT_EQ(expectedMotionVector.y(), actualMotionVector.y())
      << "Actual Y: " << actualMotionVector.y() << std::endl;
  }

  {
    std::chrono::milliseconds oneMillisecond{1};
    QS::Sensable sensable({}, oneMillisecond);
    Eigen::Vector2f expectedMotionVector(0.00089408, 0);
    auto actualMotionVector = basicWalk.evaluate(nullptr, sensable);
    EXPECT_FLOAT_EQ(expectedMotionVector.x(), actualMotionVector.x())
      << "Actual X: " << actualMotionVector.x() << std::endl;
    EXPECT_FLOAT_EQ(expectedMotionVector.y(), actualMotionVector.y())
      << "Actual Y: " << actualMotionVector.y() << std::endl;
  }

  {
    // Test at a movie-type frame rate (think batch mode). 24 frames per second
    // is actually 41 2/3 milliseconds per frame, but std::chrono::milliseconds
    // only deals in integer numbers. So I rounded up.
    std::chrono::milliseconds twentyFourFramesPerSecond{42};
    QS::Sensable sensable({}, twentyFourFramesPerSecond);
    Eigen::Vector2f expectedMotionVector(0.03755136, 0);
    auto actualMotionVector = basicWalk.evaluate(nullptr, sensable);
    EXPECT_FLOAT_EQ(expectedMotionVector.x(), actualMotionVector.x())
      << "Actual X: " << actualMotionVector.x() << std::endl;
    EXPECT_FLOAT_EQ(expectedMotionVector.y(), actualMotionVector.y())
      << "Actual Y: " << actualMotionVector.y() << std::endl;
  }
}