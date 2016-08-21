/**
 * @file Sensor.cpp
 * @brief Unit tests for Sensor class
 *
 * @author Michael Albers
 */

#include <stdexcept>

#include "Sensor.h"
#include "Sensable.h"
#include "gtest/gtest.h"

class TestSensor : public QS::Sensor
{
  public:
  TestSensor(const std::string &theTypeName) :
    Sensor(theTypeName)
  {
  }

  virtual void sense(const QS::Sensable &theSensable) override
  {
  }
};

GTEST_TEST(SensorTest, testConstruction)
{
  // Test nominal construction
  EXPECT_NO_THROW(TestSensor sensor("Hello"));

  // Verify an exception is thrown on an empty name
  try
  {
    TestSensor sensor("");
    FAIL();
  }
  catch (const std::invalid_argument &e)
  {
    EXPECT_EQ(std::string("Sensor type name cannot be empty "
                          "in Sensor constructor."),
              e.what());
  }
  catch (...)
  {
    FAIL();
  }
}

GTEST_TEST(SensorTest, testGetTypeName)
{
  TestSensor sensor("Hello");
  EXPECT_EQ(std::string("Hello"), sensor.getTypeName());
}
