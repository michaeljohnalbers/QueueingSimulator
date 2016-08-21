/**
 * @file ActorDefinitionTest.cpp
 * @brief Unit test of ActorDefinition class
 *
 * @author Michael Albers
 */

#include "ActorDefinition.h"
#include "gtest/gtest.h"

GTEST_TEST(ActorDefinitionTest, testConstruction)
{
  EXPECT_NO_THROW(QS::ActorDefinition actorDef("a", {"a"}));
}

GTEST_TEST(ActorDefinition, testCopyMove)
{
  QS::ActorDefinition actorDef("StandardActor",
                               {"BehaviorSet1", "BehaviorSet2"});

  // Test copy constructor
  QS::ActorDefinition actorCopy(actorDef);
  EXPECT_EQ(actorDef.getName(), actorCopy.getName());
  EXPECT_EQ(actorDef.getBehaviorSets(), actorCopy.getBehaviorSets());

  // Test move constructor
  QS::ActorDefinition actorMove(std::move(actorCopy));
  EXPECT_EQ(actorDef.getName(), actorMove.getName());
  EXPECT_EQ(actorDef.getBehaviorSets(), actorMove.getBehaviorSets());

  // Test copy assignment operator
  QS::ActorDefinition actorAssignCopy{"", {""}};
  actorAssignCopy = actorDef;
  EXPECT_EQ(actorDef.getName(), actorAssignCopy.getName());
  EXPECT_EQ(actorDef.getBehaviorSets(), actorAssignCopy.getBehaviorSets());

  // Test move assignment operator
  QS::ActorDefinition actorAssignMove{"", {""}};
  actorAssignMove = actorDef;
  EXPECT_EQ(actorDef.getName(), actorAssignMove.getName());
  EXPECT_EQ(actorDef.getBehaviorSets(), actorAssignMove.getBehaviorSets());
}

GTEST_TEST(ActorDefinition, testGetters)
{
  std::vector<std::string> behaviorSets{"BehaviorSet1", "BehaviorSet2"};
  QS::ActorDefinition actorDef("StandardActor", behaviorSets);
  EXPECT_EQ("StandardActor", actorDef.getName());
  EXPECT_EQ(behaviorSets, actorDef.getBehaviorSets());
}

GTEST_TEST(ActorDefinition, testEqualityOperator)
{
  QS::ActorDefinition actorDef("StandardActor",
                               {"BehaviorSet1", "BehaviorSet2"});
  auto actorCopy(actorDef);
  EXPECT_TRUE(actorDef == actorCopy);
  EXPECT_TRUE(actorCopy == actorDef);

  // Test actor with a different name.
  {
    QS::ActorDefinition actorDiff("Standardactor",
                                  {"BehaviorSet1", "BehaviorSet2"});
    EXPECT_FALSE(actorDef == actorDiff);
    EXPECT_FALSE(actorDiff == actorDef);
  }

  // Test actor with a different number of behaviorSets
  {
    QS::ActorDefinition actorDiff("StandardActor", {"BehaviorSet1"});
    EXPECT_TRUE(actorDef == actorDiff);
    EXPECT_TRUE(actorDiff == actorDef);
  }

  // Test actor with a different behaviorSet names
  {
    QS::ActorDefinition actorDiff("StandardActor",
                                  {"behaviorSet1", "BehaviorSet2"});
    EXPECT_TRUE(actorDef == actorDiff);
    EXPECT_TRUE(actorDiff == actorDef);
  }
}
