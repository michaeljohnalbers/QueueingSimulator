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
  EXPECT_NO_THROW(QS::BehaviorSetDefinition behaviorSet("", {""}));
}

GTEST_TEST(BehaviorSetDefinition, testCopyMove)
{
  QS::BehaviorSetDefinition behaviorsetDef("StandardBehaviorSet",
                                           {"Behavior1", "Behavior2"});

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
  QS::BehaviorSetDefinition behaviorsetAssignCopy{"", {""}};
  behaviorsetAssignCopy = behaviorsetDef;
  EXPECT_EQ(behaviorsetDef.getName(), behaviorsetAssignCopy.getName());
  EXPECT_EQ(behaviorsetDef.getBehaviors(),
            behaviorsetAssignCopy.getBehaviors());

  // Test move assignment operator
  QS::BehaviorSetDefinition behaviorsetAssignMove{"", {""}};
  behaviorsetAssignMove = behaviorsetDef;
  EXPECT_EQ(behaviorsetDef.getName(), behaviorsetAssignMove.getName());
  EXPECT_EQ(behaviorsetDef.getBehaviors(),
            behaviorsetAssignMove.getBehaviors());
}

GTEST_TEST(BehaviorSetDefinition, testGetters)
{
  std::vector<std::string> behaviorSets{"Behavior1", "Behavior2"};
  QS::BehaviorSetDefinition behaviorsetDef("StandardBehaviorSet", behaviorSets);
  EXPECT_EQ("StandardBehaviorSet", behaviorsetDef.getName());
  EXPECT_EQ(behaviorSets, behaviorsetDef.getBehaviors());
}

GTEST_TEST(BehaviorSetDefinition, testEqualityOperator)
{
  QS::BehaviorSetDefinition behaviorsetDef("StandardBehaviorSet",
                                           {"Behavior1", "Behavior2"});
  auto behaviorsetCopy(behaviorsetDef);
  EXPECT_TRUE(behaviorsetDef == behaviorsetCopy);
  EXPECT_TRUE(behaviorsetCopy == behaviorsetDef);

  // Test behaviorset with a different name.
  {
    QS::BehaviorSetDefinition behaviorsetDiff("Standardbehaviorset",
                                              {"Behavior1", "Behavior2"});
    EXPECT_FALSE(behaviorsetDef == behaviorsetDiff);
    EXPECT_FALSE(behaviorsetDiff == behaviorsetDef);
  }

  // Test behaviorset with a different number of behaviors
  {
    QS::BehaviorSetDefinition behaviorsetDiff("StandardBehaviorSet",
                                              {"Behavior1"});
    EXPECT_FALSE(behaviorsetDef == behaviorsetDiff);
    EXPECT_FALSE(behaviorsetDiff == behaviorsetDef);
  }

  // Test behaviorset with a different behavior names
  {
    QS::BehaviorSetDefinition behaviorsetDiff("StandardBehaviorSet",
                                              {"behavior1", "Behavior2"});
    EXPECT_FALSE(behaviorsetDef == behaviorsetDiff);
    EXPECT_FALSE(behaviorsetDiff == behaviorsetDef);
  }
}
