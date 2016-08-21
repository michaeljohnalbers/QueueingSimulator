/**
 * @file Behavior.cpp
 * @brief Unit tests for Behavior class
 *
 * @author Michael Albers
 */

#include <stdexcept>

#include "Behavior.h"
#include "gtest/gtest.h"

class TestBehavior : public QS::Behavior
{
  public:
  TestBehavior(const std::vector<std::string> &theInputSensorTypes) :
    Behavior(theInputSensorTypes)
  {
  }

  virtual Eigen::Vector2f evaluate(
    const std::vector<const QS::Sensor*> theSensors)
  {
    return {0.0, 0.0};
  }
};

GTEST_TEST(BehaviorTest, testConstruction)
{
  // Test nominal construction
  EXPECT_NO_THROW(TestBehavior behavior({""}));
  EXPECT_NO_THROW(TestBehavior behavior({"Type1", "Type2"}));
}

GTEST_TEST(BehaviorTest, testGetInputSensorTypes)
{
  std::vector<std::string> sensorTypes{"Type1", "Type2"};
  TestBehavior behavior(sensorTypes);
  EXPECT_EQ(sensorTypes, behavior.getInputSensorTypes());
}
