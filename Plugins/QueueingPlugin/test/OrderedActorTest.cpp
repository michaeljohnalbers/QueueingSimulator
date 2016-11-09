/**
 * @file OrderedActor.cpp
 * @brief Unit tests for OrderedActor class
 *
 * @author Michael Albers
 */

#include <stdexcept>
#include "gtest/gtest.h"
#include "OrderedActor.h"

static QS::PluginEntity::Properties baseActorProperties{
  {"radius", "0.1"},
  {"mass", "0.2"},
  {"x", "0.5"},
  {"y", "0.6"}};

GTEST_TEST(OrderedActorTest, construction)
{
  QS::PluginEntity::Properties orderedActorProperties{baseActorProperties};
  orderedActorProperties["rank"] = "4";

  // Test no properties
  EXPECT_THROW(QS::OrderedActor({}, ""), std::invalid_argument);
  // Test only base properties (missing required OrderedActor properties)
  EXPECT_THROW(QS::OrderedActor(baseActorProperties, ""),
               std::invalid_argument);

  EXPECT_NO_THROW(QS::OrderedActor(orderedActorProperties, ""));

  // Invalid ranks
  orderedActorProperties["rank"] = "NotANumber";
  EXPECT_THROW(QS::OrderedActor(orderedActorProperties, ""),
               std::invalid_argument);
  orderedActorProperties["rank"] = "239502384792374982374983724983749287394873";
  EXPECT_THROW(QS::OrderedActor(orderedActorProperties, ""),
               std::out_of_range);
}

GTEST_TEST(OrderedActorTest, rank)
{
  QS::PluginEntity::Properties orderedActorProperties{baseActorProperties};
  orderedActorProperties["rank"] = "4";

  QS::OrderedActor actor(orderedActorProperties, "");
  auto rank = actor.getRank();
  EXPECT_EQ(4u, rank);
}
