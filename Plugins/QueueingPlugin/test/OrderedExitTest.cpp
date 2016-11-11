/**
 * @file OrderedExit.cpp
 * @brief Unit tests for OrderedExit class
 *
 * @author Michael Albers
 */

#include <stdexcept>
#include "gtest/gtest.h"
#include "OrderedActor.h"
#include "OrderedExit.h"
#include "TestUtils.h"

GTEST_TEST(OrderedExitTest, construction)
{
  EXPECT_THROW(QS::OrderedExit({}, ""), std::invalid_argument);
  EXPECT_NO_THROW(QS::OrderedExit(QS::TestUtils::getMinimalExitProperties(),
                                  ""));
}

GTEST_TEST(OrderedExitTest, canExit)
{
  QS::PluginEntity::Properties baseActorProperties{
    QS::TestUtils::getMinimalActorProperties()};
  baseActorProperties["x"] = "5.0";
  baseActorProperties["y"] = "5.0";
  baseActorProperties["radius"] = "0.1";

  QS::PluginEntity::Properties orderedActorProperties{baseActorProperties};
  orderedActorProperties["rank"] = "4";

  QS::Actor baseActor(baseActorProperties, "");
  orderedActorProperties["rank"] = "0";
  QS::OrderedActor orderedActor1(orderedActorProperties, "");
  orderedActorProperties["rank"] = "1";
  QS::OrderedActor orderedActor2(orderedActorProperties, "");

  auto exitProperties{QS::TestUtils::getMinimalExitProperties()};
  exitProperties["x"] = "5.0";
  exitProperties["y"] = "5.0";
  exitProperties["radius"] = "1.0";

  // All the Actors overlap with the exit, so these tests ensure that 'rank'
  // is respected.

  {
    QS::OrderedExit exit(exitProperties, "");

    EXPECT_TRUE(exit.canActorExit(&baseActor));
    EXPECT_TRUE(exit.canActorExit(&orderedActor1));
    EXPECT_TRUE(exit.canActorExit(&orderedActor2));
  }

  {
    QS::OrderedExit exit(exitProperties, "");

    EXPECT_TRUE(exit.canActorExit(&orderedActor1));
    EXPECT_TRUE(exit.canActorExit(&baseActor));
    EXPECT_TRUE(exit.canActorExit(&orderedActor2));
  }

  {
    QS::OrderedExit exit(exitProperties, "");

    EXPECT_TRUE(exit.canActorExit(&orderedActor1));
    EXPECT_TRUE(exit.canActorExit(&orderedActor2));
    EXPECT_TRUE(exit.canActorExit(&baseActor));
  }

  {
    QS::OrderedExit exit(exitProperties, "");

    EXPECT_TRUE(exit.canActorExit(&baseActor));
    EXPECT_FALSE(exit.canActorExit(&orderedActor2));
    EXPECT_TRUE(exit.canActorExit(&orderedActor1));
    EXPECT_TRUE(exit.canActorExit(&orderedActor2));
  }

  {
    QS::OrderedExit exit(exitProperties, "");

    EXPECT_FALSE(exit.canActorExit(&orderedActor2));
    EXPECT_TRUE(exit.canActorExit(&baseActor));
    EXPECT_TRUE(exit.canActorExit(&orderedActor1));
    EXPECT_TRUE(exit.canActorExit(&orderedActor2));
  }

  // Not overlapping
  {
    QS::OrderedExit exit(exitProperties, "");

    baseActor.setPosition({0.0, 0.0});
    orderedActor1.setPosition({0.0, 0.0});
    EXPECT_FALSE(exit.canActorExit(&baseActor));
    EXPECT_FALSE(exit.canActorExit(&orderedActor1));
    EXPECT_FALSE(exit.canActorExit(&orderedActor2));
  }
}
