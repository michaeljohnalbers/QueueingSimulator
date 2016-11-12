/**
 * @file BasicWalkTest.cpp
 * @brief Unit tests for BasicWalk behavior set class
 *
 * @author Michael Albers
 */

#include "gtest/gtest.h"
#include "Actor.h"
#include "BasicWalk.h"
#include "EigenHelper.h"
#include "Sensable.h"
#include "TestUtils.h"
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

  QS::Actor actor(QS::TestUtils::getMinimalActorProperties(), "");
  actor.setVelocity({0.0, 0.0});

  QS::Sensable sensable(nullptr, {}, {}, 1.0);
  // Using 2.0 due to mass correction in Walk.
  Eigen::Vector2f expectedMotionVector(2.0, 0);
  auto actualMotionVector = basicWalk.evaluate(&actor, sensable);
  EXPECT_EQ(expectedMotionVector, actualMotionVector)
    << ", Actual: "
    << actualMotionVector.format(QS::EigenHelper::prettyPrint);
}
