/**
 * @file BehaviorDefinitionTest.cpp
 * @brief Unit test of BehaviorDefinition class
 *
 * @author Michael Albers
 */

#include "BehaviorDefinition.h"
#include "gtest/gtest.h"

GTEST_TEST(BehaviorDefinitionTest, testConstruction)
{
  EXPECT_NO_THROW(QS::BehaviorDefinition behavior(""));
}

GTEST_TEST(BehaviorDefinition, testCopyMove)
{
  QS::BehaviorDefinition behaviorDef("StandardBehavior");
  behaviorDef.addSensor("Sensor1", "Source", "");
  behaviorDef.addSensor("Sensor2", "Source", "");

  // Test copy constructor
  QS::BehaviorDefinition behaviorCopy(behaviorDef);
  EXPECT_EQ(behaviorDef.getName(), behaviorCopy.getName());
  EXPECT_EQ(behaviorDef.getSensors(), behaviorCopy.getSensors());

  // Test move constructor
  QS::BehaviorDefinition behaviorMove(std::move(behaviorCopy));
  EXPECT_EQ(behaviorDef.getName(), behaviorMove.getName());
  EXPECT_EQ(behaviorDef.getSensors(), behaviorMove.getSensors());

  // Test copy assignment operator
  QS::BehaviorDefinition behaviorAssignCopy{""};
  behaviorAssignCopy = behaviorDef;
  EXPECT_EQ(behaviorDef.getName(), behaviorAssignCopy.getName());
  EXPECT_EQ(behaviorDef.getSensors(), behaviorAssignCopy.getSensors());

  // Test move assignment operator
  QS::BehaviorDefinition behaviorAssignMove{""};
  behaviorAssignMove = behaviorDef;
  EXPECT_EQ(behaviorDef.getName(), behaviorAssignMove.getName());
  EXPECT_EQ(behaviorDef.getSensors(), behaviorAssignMove.getSensors());
}

GTEST_TEST(BehaviorDefinition, testSensors)
{
  std::set<QS::PluginDependencySet> sensors{
    {"Sensor1", "Source1", "Tag1"},
    {"Sensor2", "Source2", "Tag2"}};

  QS::BehaviorDefinition behaviorDef("StandardBehavior");
  for (auto set : sensors)
  {
    behaviorDef.addSensor(set.myName, set.mySource, set.myTag);
  }

  EXPECT_EQ(sensors, behaviorDef.getSensors());
}

GTEST_TEST(BehaviorDefinition, testGetName)
{
  QS::BehaviorDefinition behaviorDef("StandardBehavior");
  EXPECT_EQ("StandardBehavior", behaviorDef.getName());
}

GTEST_TEST(BehaviorDefinition, testEqualityOperator)
{
  QS::BehaviorDefinition behaviorDef("StandardBehavior");
  behaviorDef.addSensor("Sensor1", "Source", "Tag");
  behaviorDef.addSensor("Sensor2", "Source", "Tag");

  auto behaviorCopy(behaviorDef);
  EXPECT_TRUE(behaviorDef == behaviorCopy);
  EXPECT_TRUE(behaviorCopy == behaviorDef);

  // Test behavior with a different name.
  {
    QS::BehaviorDefinition behaviorDiff("Standardbehavior");
    behaviorDiff.addSensor("Sensor1", "Source", "Tag1");
    behaviorDiff.addSensor("Sensor2", "Source", "Tag2");

    EXPECT_FALSE(behaviorDef == behaviorDiff);
    EXPECT_FALSE(behaviorDiff == behaviorDef);
  }

  // Test behavior with a different number of sensors
  {
    QS::BehaviorDefinition behaviorDiff("StandardBehavior");
    behaviorDiff.addSensor("Sensor1", "Source", "Tag");

    EXPECT_FALSE(behaviorDef == behaviorDiff);
    EXPECT_FALSE(behaviorDiff == behaviorDef);
  }

  // Test behavior with a different sensor names
  {
    QS::BehaviorDefinition behaviorDiff("StandardBehavior");
    behaviorDiff.addSensor("sensor1", "Source", "Tag2");
    behaviorDiff.addSensor("Sensor2", "Source", "Tag1");

    EXPECT_FALSE(behaviorDef == behaviorDiff);
    EXPECT_FALSE(behaviorDiff == behaviorDef);
  }
}
