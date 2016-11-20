/**
 * @file FindExitSensorTest.cpp
 * @brief Unit tests for FindExitSensor class
 *
 * @author Michael Albers
 */

#include "gtest/gtest.h"
#include "Exit.h"
#include "FindExitSensor.h"
#include "Sensable.h"
#include "TestUtils.h"

GTEST_TEST(FindExitSensorTest, class)
{
  QS::FindExitSensor findExitSensor({}, "");

  // Test that FindExitSensor starts with null exit pointer
  const QS::Exit *exit = findExitSensor.getExit();
  EXPECT_EQ(nullptr, exit);

  // Test with a sensable with no exits.
  {
    QS::Sensable emptySensable(nullptr, {}, {}, 0.0);
    findExitSensor.sense(emptySensable);
    exit = findExitSensor.getExit();
    EXPECT_EQ(nullptr, exit);
  }

  // Test with a sensable with multiple exits.
  {
    QS::Exit firstExit(QS::TestUtils::getMinimalExitProperties(), "");
    QS::Exit secondExit(QS::TestUtils::getMinimalExitProperties(), "");
    std::vector<const QS::Exit*> exits{&firstExit, &secondExit};
    // Can't use initializer list for exits argument as this creates a false
    // negative. Looks like maybe a g++ bug?
    QS::Sensable sensable(nullptr, {}, exits, 0.0);
    findExitSensor.sense(sensable);
    exit = findExitSensor.getExit();
    EXPECT_EQ(&firstExit, exit);
  }

  // Test with an empty sensable (to make sure the member pointer resets to
  // null).
  {
    QS::Sensable emptySensable(nullptr, {}, {}, 0.0);
    findExitSensor.sense(emptySensable);
    exit = findExitSensor.getExit();
    EXPECT_EQ(nullptr, exit);
  }
}
