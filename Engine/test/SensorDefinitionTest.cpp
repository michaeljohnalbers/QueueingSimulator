/**
 * @file SensorDefinitionTest.cpp
 * @brief Unit test of SensorDefinition class
 *
 * @author Michael Albers
 */

#include "SensorDefinition.h"
#include "gtest/gtest.h"

GTEST_TEST(SensorDefinitionTest, testConstruction)
{
  EXPECT_NO_THROW(QS::SensorDefinition sensor(""));
}

GTEST_TEST(SensorDefinition, testCopyMove)
{
  QS::SensorDefinition sensorDef("StandardSensor");

  // Test copy constructor
  QS::SensorDefinition sensorCopy(sensorDef);
  EXPECT_EQ(sensorDef.getName(), sensorCopy.getName());

  // Test move constructor
  QS::SensorDefinition sensorMove(std::move(sensorCopy));
  EXPECT_EQ(sensorDef.getName(), sensorMove.getName());

  // Test copy assignment operator
  QS::SensorDefinition sensorAssignCopy{""};
  sensorAssignCopy = sensorDef;
  EXPECT_EQ(sensorDef.getName(), sensorAssignCopy.getName());

  // Test move assignment operator
  QS::SensorDefinition sensorAssignMove{""};
  sensorAssignMove = sensorDef;
  EXPECT_EQ(sensorDef.getName(), sensorAssignMove.getName());
}

GTEST_TEST(SensorDefinition, testGetters)
{
  QS::SensorDefinition sensorDef("StandardSensor");
  EXPECT_EQ("StandardSensor", sensorDef.getName());
}

GTEST_TEST(SensorDefinition, testEqualityOperator)
{
  QS::SensorDefinition sensorDef("StandardSensor");
  auto sensorCopy(sensorDef);
  EXPECT_TRUE(sensorDef == sensorCopy);
  EXPECT_TRUE(sensorCopy == sensorDef);

  // Test sensor with a different name.
  {
    QS::SensorDefinition sensorDiff("Standardsensor");
    EXPECT_FALSE(sensorDef == sensorDiff);
    EXPECT_FALSE(sensorDiff == sensorDef);
  }
}
