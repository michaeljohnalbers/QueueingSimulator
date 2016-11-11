/**
 * @file OrderedActor.cpp
 * @brief Unit tests for OrderedActor class
 *
 * @author Michael Albers
 */

#include <stdexcept>
#include "gtest/gtest.h"
#include "OrderedActor.h"
#include "TestUtils.h"

GTEST_TEST(OrderedActorTest, construction)
{
  auto baseProperties{QS::TestUtils::getMinimalActorProperties()};

  QS::PluginEntity::Properties orderedActorProperties{baseProperties};
  orderedActorProperties["rank"] = "4";

  // Test no properties
  EXPECT_THROW(QS::OrderedActor({}, ""), std::invalid_argument);
  // Test only base properties (missing required OrderedActor properties)
  EXPECT_THROW(QS::OrderedActor(baseProperties, ""), std::invalid_argument);

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
  QS::PluginEntity::Properties orderedActorProperties{
    QS::TestUtils::getMinimalActorProperties()};
  orderedActorProperties["rank"] = "4";

  QS::OrderedActor actor(orderedActorProperties, "");
  auto rank = actor.getRank();
  EXPECT_EQ(4u, rank);
}
