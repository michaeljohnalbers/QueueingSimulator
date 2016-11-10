/**
 * @file Exit.cpp
 * @brief Unit tests for Exit class
 *
 * @author Michael Albers
 */

#include <stdexcept>
#include "gtest/gtest.h"
#include "Actor.h"
#include "Exit.h"
#include "TestUtils.h"

GTEST_TEST(ExitTest, class)
{
  // Assumes tests in TargetTest have been run since Exit inherits from Target.

  // Test invalid radius (== 0)
  {
    auto properties = QS::TestUtils::getMinimalExitProperties();
    properties["radius"] = "0.0";
    EXPECT_THROW(QS::Exit(properties, ""), std::invalid_argument);
  }

  QS::PluginEntity::Properties exitProperties{
    QS::TestUtils::getMinimalExitProperties()};
  exitProperties["x"] = "3.5";
  exitProperties["y"] = "4.2";
  exitProperties["radius"] = "1.0";
  QS::Exit exit(exitProperties, "");

  QS::PluginEntity::Properties actorProperties{
    QS::TestUtils::getMinimalActorProperties()};
  actorProperties["radius"] = "0.1";
  actorProperties["mass"] = "0.2";
  actorProperties["x"] = "1.0";
  actorProperties["y"] = "1.0";

  QS::Actor actor(actorProperties, "");

  // Returns false when actor is just outside the circle
  actor.setPosition(Eigen::Vector2f(4.6, 4.2));
  EXPECT_FALSE(exit.canActorExit(&actor));

  // Returns false when actor is on the circumference.
  actor.setPosition(Eigen::Vector2f(4.5, 4.2));
  EXPECT_FALSE(exit.canActorExit(&actor));

  // Inside the circle
  actor.setPosition(Eigen::Vector2f(4.4, 4.2));
  EXPECT_TRUE(exit.canActorExit(&actor));
}
