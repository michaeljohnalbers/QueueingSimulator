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
  EXPECT_NO_THROW(QS::ActorDefinition actorDef("a"));
}

GTEST_TEST(ActorDefinition, testCopyMove)
{
  QS::ActorDefinition actorDef("StandardActor");
  actorDef.addBehaviorSet("BehaviorSet", "Source", "Tag");

  // Test copy constructor
  QS::ActorDefinition actorCopy(actorDef);
  EXPECT_EQ(actorDef.getName(), actorCopy.getName());
  EXPECT_EQ(actorDef.getBehaviorSets(), actorCopy.getBehaviorSets());

  // Test move constructor
  QS::ActorDefinition actorMove(std::move(actorCopy));
  EXPECT_EQ(actorDef.getName(), actorMove.getName());
  EXPECT_EQ(actorDef.getBehaviorSets(), actorMove.getBehaviorSets());

  // Test copy assignment operator
  QS::ActorDefinition actorAssignCopy{""};
  actorAssignCopy = actorDef;
  EXPECT_EQ(actorDef.getName(), actorAssignCopy.getName());
  EXPECT_EQ(actorDef.getBehaviorSets(), actorAssignCopy.getBehaviorSets());

  // Test move assignment operator
  QS::ActorDefinition actorAssignMove{""};
  actorAssignMove = actorDef;
  EXPECT_EQ(actorDef.getName(), actorAssignMove.getName());
  EXPECT_EQ(actorDef.getBehaviorSets(), actorAssignMove.getBehaviorSets());
}

GTEST_TEST(ActorDefinition, testBehaviorSets)
{
  std::set<QS::PluginDependencySet> behaviorSets{
    {"BehaviorSet1", "Source1", "Tag1"},
    {"BehaviorSet2", "Source2", "Tag2"}};

  QS::ActorDefinition actorDef("StandardActor");
  for (auto set : behaviorSets)
  {
    actorDef.addBehaviorSet(set.myName, set.mySource, set.myTag);
  }

  EXPECT_EQ(behaviorSets, actorDef.getBehaviorSets());
}

GTEST_TEST(ActorDefinition, testGetName)
{
  QS::ActorDefinition actorDef("StandardActor");
  EXPECT_EQ("StandardActor", actorDef.getName());
}

GTEST_TEST(ActorDefinition, testEqualityOperator)
{
  QS::ActorDefinition actorDef("StandardActor");
  actorDef.addBehaviorSet("BehaviorSet1", "Source", "Tag");
  actorDef.addBehaviorSet("BehaviorSet2", "Source", "Tag");

  auto actorCopy(actorDef);
  EXPECT_TRUE(actorDef == actorCopy);
  EXPECT_TRUE(actorCopy == actorDef);

  // Test actor with a different name.
  {
    QS::ActorDefinition actorDiff("Standardactor");
    actorDiff.addBehaviorSet("BehaviorSet1", "Source", "Tag");
    actorDiff.addBehaviorSet("BehaviorSet2", "Source", "Tag");
    
    EXPECT_FALSE(actorDef == actorDiff);
    EXPECT_FALSE(actorDiff == actorDef);
  }

  // Test actor with a different number of behaviorSets
  {
    QS::ActorDefinition actorDiff("StandardActor");
    actorDiff.addBehaviorSet("BehaviorSet1", "Source", "Tag");
    EXPECT_TRUE(actorDef == actorDiff);
    EXPECT_TRUE(actorDiff == actorDef);
  }

  // Test actor with a different behaviorSet names
  {
    QS::ActorDefinition actorDiff("StandardActor");
    actorDiff.addBehaviorSet("behaviorset1", "source", "Tag");
    actorDiff.addBehaviorSet("Behaviorset2", "source", "Tag");
    EXPECT_TRUE(actorDef == actorDiff);
    EXPECT_TRUE(actorDiff == actorDef);
  }
}
