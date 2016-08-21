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
  EXPECT_NO_THROW(QS::BehaviorDefinition behavior("", {""}));
}

GTEST_TEST(BehaviorDefinition, testCopyMove)
{
  QS::BehaviorDefinition behaviorDef("StandardBehavior",
                                     {"Sensor1", "Sensor2"});

  // Test copy constructor
  QS::BehaviorDefinition behaviorCopy(behaviorDef);
  EXPECT_EQ(behaviorDef.getName(), behaviorCopy.getName());
  EXPECT_EQ(behaviorDef.getSensors(), behaviorCopy.getSensors());

  // Test move constructor
  QS::BehaviorDefinition behaviorMove(std::move(behaviorCopy));
  EXPECT_EQ(behaviorDef.getName(), behaviorMove.getName());
  EXPECT_EQ(behaviorDef.getSensors(), behaviorMove.getSensors());

  // Test copy assignment operator
  QS::BehaviorDefinition behaviorAssignCopy{"", {""}};
  behaviorAssignCopy = behaviorDef;
  EXPECT_EQ(behaviorDef.getName(), behaviorAssignCopy.getName());
  EXPECT_EQ(behaviorDef.getSensors(), behaviorAssignCopy.getSensors());

  // Test move assignment operator
  QS::BehaviorDefinition behaviorAssignMove{"", {""}};
  behaviorAssignMove = behaviorDef;
  EXPECT_EQ(behaviorDef.getName(), behaviorAssignMove.getName());
  EXPECT_EQ(behaviorDef.getSensors(), behaviorAssignMove.getSensors());
}

GTEST_TEST(BehaviorDefinition, testGetters)
{
  std::vector<std::string> sensors{"Sensor1", "Sensor2"};
  QS::BehaviorDefinition sensorDef("StandardBehavior", sensors);
  EXPECT_EQ("StandardBehavior", sensorDef.getName());
  EXPECT_EQ(sensors, sensorDef.getSensors());
}

GTEST_TEST(BehaviorDefinition, testEqualityOperator)
{
  QS::BehaviorDefinition behaviorDef("StandardBehavior",
                                     {"Sensor1", "Sensor2"});
  auto behaviorCopy(behaviorDef);
  EXPECT_TRUE(behaviorDef == behaviorCopy);
  EXPECT_TRUE(behaviorCopy == behaviorDef);

  // Test behavior with a different name.
  {
    QS::BehaviorDefinition behaviorDiff("Standardbehavior",
                                        {"Sensor1", "Sensor2"});
    EXPECT_FALSE(behaviorDef == behaviorDiff);
    EXPECT_FALSE(behaviorDiff == behaviorDef);
  }

  // Test behavior with a different number of sensors
  {
    QS::BehaviorDefinition behaviorDiff("StandardBehavior", {"Sensor1"});
    EXPECT_FALSE(behaviorDef == behaviorDiff);
    EXPECT_FALSE(behaviorDiff == behaviorDef);
  }

  // Test behavior with a different sensor names
  {
    QS::BehaviorDefinition behaviorDiff("StandardBehavior",
                                        {"sensor1", "Sensor2"});
    EXPECT_FALSE(behaviorDef == behaviorDiff);
    EXPECT_FALSE(behaviorDiff == behaviorDef);
  }
}
