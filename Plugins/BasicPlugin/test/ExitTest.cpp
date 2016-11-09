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

GTEST_TEST(ExitTest, class)
{
  // Assumes tests in TargetTest have been run since Exit inherits from Target.

  // Test invalid radius (== 0)
  EXPECT_THROW(QS::Exit({{"x", "0.5"}, {"y", "1.0"}, {"radius","0.0"}}, ""),
               std::invalid_argument);

  QS::PluginEntity::Properties exitProperties{
    {"x", "3.5"},
    {"y", "4.2"},
    {"radius", "1.0"}
  };
  QS::Exit exit(exitProperties, "");

  QS::PluginEntity::Properties actorProperties{
    {"radius", "0.1"},
    {"mass", "0.2"},
    {"x", "1.0"},
    {"y", "1.0"}};

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
