/**
 * @file BehaviorSetDefinitionTest.cpp
 * @brief Unit test of BehaviorSetDefinition class
 *
 * @author Michael Albers
 */

#include "BehaviorSetDefinition.h"
#include "gtest/gtest.h"

GTEST_TEST(BehaviorSetDefinitionTest, testConstruction)
{
  EXPECT_NO_THROW(QS::BehaviorSetDefinition behaviorSet(""));
}

GTEST_TEST(BehaviorSetDefinition, testCopyMove)
{
  QS::BehaviorSetDefinition behaviorsetDef("StandardBehaviorSet");
  behaviorsetDef.addBehavior("Behavior1", "Source", "Tag1");
  behaviorsetDef.addBehavior("Behavior2", "Source", "");

  // Test copy constructor
  QS::BehaviorSetDefinition behaviorsetCopy(behaviorsetDef);
  EXPECT_EQ(behaviorsetDef.getName(), behaviorsetCopy.getName());
  EXPECT_EQ(behaviorsetDef.getBehaviors(),
            behaviorsetCopy.getBehaviors());

  // Test move constructor
  QS::BehaviorSetDefinition behaviorsetMove(std::move(behaviorsetCopy));
  EXPECT_EQ(behaviorsetDef.getName(), behaviorsetMove.getName());
  EXPECT_EQ(behaviorsetDef.getBehaviors(),
            behaviorsetMove.getBehaviors());

  // Test copy assignment operator
  QS::BehaviorSetDefinition behaviorsetAssignCopy{""};
  behaviorsetAssignCopy = behaviorsetDef;
  EXPECT_EQ(behaviorsetDef.getName(), behaviorsetAssignCopy.getName());
  EXPECT_EQ(behaviorsetDef.getBehaviors(),
            behaviorsetAssignCopy.getBehaviors());

  // Test move assignment operator
  QS::BehaviorSetDefinition behaviorsetAssignMove{""};
  behaviorsetAssignMove = behaviorsetDef;
  EXPECT_EQ(behaviorsetDef.getName(), behaviorsetAssignMove.getName());
  EXPECT_EQ(behaviorsetDef.getBehaviors(),
            behaviorsetAssignMove.getBehaviors());
}

GTEST_TEST(BehaviorSetDefinition, testBehaviors)
{
  std::set<QS::PluginDependencySet> behaviors{
    {"Behavior1", "Source1", ""},
    {"Behavior2", "Source2", "Tag2"}};

  QS::BehaviorSetDefinition behaviorSetDef("StandardBehaviorSet");
  for (auto set : behaviors)
  {
    behaviorSetDef.addBehavior(set.myName, set.mySource, set.myTag);
  }

  EXPECT_EQ(behaviors, behaviorSetDef.getBehaviors());
}

GTEST_TEST(BehaviorSetDefinition, testGetName)
{
  QS::BehaviorSetDefinition behaviorSetDef("StandardBehaviorSet");
  EXPECT_EQ("StandardBehaviorSet", behaviorSetDef.getName());
}

GTEST_TEST(BehaviorSetDefinition, testEqualityOperator)
{
  QS::BehaviorSetDefinition behaviorSetDef("StandardBehaviorSet");
  behaviorSetDef.addBehavior("Behavior1", "Source", "");
  behaviorSetDef.addBehavior("Behavior2", "Source", "");

  auto behaviorSetCopy(behaviorSetDef);
  EXPECT_TRUE(behaviorSetDef == behaviorSetCopy);
  EXPECT_TRUE(behaviorSetCopy == behaviorSetDef);

  // Test behaviorSet with a different name.
  {
    QS::BehaviorSetDefinition behaviorSetDiff("Standardbehaviorset");
    behaviorSetDiff.addBehavior("Behavior1", "Source", "");
    behaviorSetDiff.addBehavior("Behavior2", "Source", "Tag2");
    EXPECT_FALSE(behaviorSetDef == behaviorSetDiff);
    EXPECT_FALSE(behaviorSetDiff == behaviorSetDef);
  }

  // Test behaviorSet with a different number of behaviors
  {
    QS::BehaviorSetDefinition behaviorSetDiff("StandardBehaviorset");
    behaviorSetDiff.addBehavior("Behavior1", "Source", "Tag");
    EXPECT_FALSE(behaviorSetDef == behaviorSetDiff);
    EXPECT_FALSE(behaviorSetDiff == behaviorSetDef);
  }

  // Test behaviorSet with a different behavior names
  {
    QS::BehaviorSetDefinition behaviorSetDiff("Standardbehaviorset");
    behaviorSetDiff.addBehavior("behavior1", "Source", "Tag1");
    behaviorSetDiff.addBehavior("Behavior2", "Source", "Tag");
    EXPECT_FALSE(behaviorSetDef == behaviorSetDiff);
    EXPECT_FALSE(behaviorSetDiff == behaviorSetDef);
  }
}
