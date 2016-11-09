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

static QS::PluginEntity::Properties glbExitProperties{
  {"x", "5.0"},
  {"y", "5.0"},
  {"radius", "1.0"},
};

GTEST_TEST(OrderedExitTest, construction)
{
  EXPECT_THROW(QS::OrderedExit({}, ""), std::invalid_argument);
  EXPECT_NO_THROW(QS::OrderedExit(glbExitProperties, ""));
}

GTEST_TEST(OrderedExitTest, canExit)
{
  QS::PluginEntity::Properties baseActorProperties{
    {"radius", "0.1"},
    {"mass", "0.2"},
    {"x", "5.0"},
    {"y", "5.0"}};

  QS::PluginEntity::Properties orderedActorProperties{baseActorProperties};
  orderedActorProperties["rank"] = "4";

  QS::Actor baseActor(baseActorProperties, "");
  orderedActorProperties["rank"] = "0";
  QS::OrderedActor orderedActor1(orderedActorProperties, "");
  orderedActorProperties["rank"] = "1";
  QS::OrderedActor orderedActor2(orderedActorProperties, "");

  {
    QS::OrderedExit exit(glbExitProperties, "");

    EXPECT_TRUE(exit.canActorExit(&baseActor));
    EXPECT_TRUE(exit.canActorExit(&orderedActor1));
    EXPECT_TRUE(exit.canActorExit(&orderedActor2));
  }

  {
    QS::OrderedExit exit(glbExitProperties, "");

    EXPECT_TRUE(exit.canActorExit(&orderedActor1));
    EXPECT_TRUE(exit.canActorExit(&baseActor));
    EXPECT_TRUE(exit.canActorExit(&orderedActor2));
  }

  {
    QS::OrderedExit exit(glbExitProperties, "");

    EXPECT_TRUE(exit.canActorExit(&orderedActor1));
    EXPECT_TRUE(exit.canActorExit(&orderedActor2));
    EXPECT_TRUE(exit.canActorExit(&baseActor));
  }

  {
    QS::OrderedExit exit(glbExitProperties, "");

    EXPECT_TRUE(exit.canActorExit(&baseActor));
    EXPECT_FALSE(exit.canActorExit(&orderedActor2));
    EXPECT_TRUE(exit.canActorExit(&orderedActor1));
    EXPECT_TRUE(exit.canActorExit(&orderedActor2));
  }

  {
    QS::OrderedExit exit(glbExitProperties, "");

    EXPECT_FALSE(exit.canActorExit(&orderedActor2));
    EXPECT_TRUE(exit.canActorExit(&baseActor));
    EXPECT_TRUE(exit.canActorExit(&orderedActor1));
    EXPECT_TRUE(exit.canActorExit(&orderedActor2));
  }

  // Not overlapping
  {
    QS::OrderedExit exit(glbExitProperties, "");

    baseActor.setPosition({0.0, 0.0});
    orderedActor1.setPosition({0.0, 0.0});
    EXPECT_FALSE(exit.canActorExit(&baseActor));
    EXPECT_FALSE(exit.canActorExit(&orderedActor1));
    EXPECT_FALSE(exit.canActorExit(&orderedActor2));
  }
}
