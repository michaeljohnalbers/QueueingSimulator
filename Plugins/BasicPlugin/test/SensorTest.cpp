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
  TestSensor() :
    Sensor(QS::PluginEntity::Properties{})
  {
  }

  virtual void sense(const QS::Sensable &theSensable) override
  {
  }
};

GTEST_TEST(SensorTest, testConstruction)
{
  // Test nominal construction
  EXPECT_NO_THROW(TestSensor sensor);
}
