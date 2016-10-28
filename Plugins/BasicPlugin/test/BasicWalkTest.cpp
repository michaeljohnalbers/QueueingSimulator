/**
 * @file BasicWalkTest.cpp
 * @brief Unit tests for BasicWalk behavior set class
 *
 * @author Michael Albers
 */

#include "gtest/gtest.h"
#include "BasicWalk.h"
#include "EigenHelper.h"
#include "Sensable.h"
#include "Walk.h"

GTEST_TEST(BasicWalkTest, testBasicWalk)
{
  // BasicWalk just returns the value of Walk.
  QS::PluginEntity::Properties properties;
  ASSERT_NO_THROW(QS::BasicWalk temp(properties, ""));

  QS::BasicWalk basicWalk(properties, "");
  QS::Walk walk(properties, "");
  QS::EntityDependency<QS::Behavior> basicWalkDependencies{"Walk", &walk, ""};
  basicWalk.setDependencies({basicWalkDependencies});

  {
    float oneSecond = 1.0;
    QS::Sensable sensable({}, oneSecond);
    Eigen::Vector2f expectedMotionVector(1.0, 0);
    auto actualMotionVector = basicWalk.evaluate(nullptr, sensable);
    EXPECT_FLOAT_EQ(expectedMotionVector.x(), actualMotionVector.x())
      << "Actual X: " << actualMotionVector.x() << std::endl;
    EXPECT_FLOAT_EQ(expectedMotionVector.y(), actualMotionVector.y())
      << "Actual Y: " << actualMotionVector.y() << std::endl;
  }

  {
    float oneMillisecond = 0.001;
    QS::Sensable sensable({}, oneMillisecond);
    Eigen::Vector2f expectedMotionVector(0.001, 0);
    auto actualMotionVector = basicWalk.evaluate(nullptr, sensable);
    EXPECT_FLOAT_EQ(expectedMotionVector.x(), actualMotionVector.x())
      << "Actual X: " << actualMotionVector.x() << std::endl;
    EXPECT_FLOAT_EQ(expectedMotionVector.y(), actualMotionVector.y())
      << "Actual Y: " << actualMotionVector.y() << std::endl;
  }

  {
    // Test at a movie-type frame rate (think batch mode).
    float twentyFourFramesPerSecond = 1.0/24.0;
    QS::Sensable sensable({}, twentyFourFramesPerSecond);
    Eigen::Vector2f expectedMotionVector(0.041666, 0);
    auto actualMotionVector = basicWalk.evaluate(nullptr, sensable);
    EXPECT_NEAR(expectedMotionVector.x(), actualMotionVector.x(),
                QS::FLOAT_TOLERANCE)
      << "Actual X: " << actualMotionVector.x() << std::endl;
    EXPECT_FLOAT_EQ(expectedMotionVector.y(), actualMotionVector.y())
      << "Actual Y: " << actualMotionVector.y() << std::endl;
  }
}
