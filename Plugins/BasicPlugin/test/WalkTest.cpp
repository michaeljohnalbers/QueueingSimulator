/**
 * @file WalkTest.cpp
 * @brief Unit tests for Walk behavior class
 *
 * @author Michael Albers
 */

#include "gtest/gtest.h"
#include "Actor.h"
#include "EigenHelper.h"
#include "TestUtils.h"
#include "Walk.h"

GTEST_TEST(WalkTest, testWalk)
{
  QS::PluginEntity::Properties properties;
  ASSERT_NO_THROW(QS::Walk temp(properties, ""));

  QS::Actor actor(QS::TestUtils::getMinimalActorProperties(), "");
  actor.setVelocity({0.0, 0.0});

  QS::Walk walkBehavior(properties, "");
  // Using 2.0 due to mass correction in Walk.
  Eigen::Vector2f expectedMotionVector(2.0, 0);
  auto actualMotionVector = walkBehavior.evaluate(&actor);
  EXPECT_EQ(expectedMotionVector, actualMotionVector)
    << ", Actual: "
    << actualMotionVector.format(QS::EigenHelper::prettyPrint);

  // Actor is already moving at desired speed.
  actor.setVelocity({1.0, 0});
  expectedMotionVector << 0.0, 0.0;
  actualMotionVector = walkBehavior.evaluate(&actor);
  EXPECT_EQ(expectedMotionVector, actualMotionVector)
    << ", Actual: "
    << actualMotionVector.format(QS::EigenHelper::prettyPrint);

  // Actor is already moving at greater than desired speed.
  actor.setVelocity({4.25, 0});
  expectedMotionVector << 0.0, 0.0;
  actualMotionVector = walkBehavior.evaluate(&actor);
  EXPECT_EQ(expectedMotionVector, actualMotionVector)
    << ", Actual: "
    << actualMotionVector.format(QS::EigenHelper::prettyPrint);

  // Test using the speed property.
  actor.setVelocity({0.0, 0.0});
  properties["speed"] = "2.5";
  QS::Walk walkBehavior2(properties, "");
  // Actor has a mass of 2.0
  expectedMotionVector << 5.0, 0;
  actualMotionVector = walkBehavior2.evaluate(&actor);
  EXPECT_EQ(expectedMotionVector, actualMotionVector)
    << ", Actual: "
    << actualMotionVector.format(QS::EigenHelper::prettyPrint);

  // Test using invalid speed property.
  properties["speed"] = "r1.2";
  EXPECT_THROW(QS::Walk(properties, ""), std::logic_error);
}
