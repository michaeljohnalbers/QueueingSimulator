/**
 * @file ExitSeekTest.cpp
 * @brief Unit tests for ExitSeek class
 *
 * @author Michael Albers
 */

#include "gtest/gtest.h"
#include "Actor.h"
#include "EigenHelper.h"
#include "Exit.h"
#include "ExitSeek.h"
#include "FindExitSensor.h"
#include "Sensable.h"
#include "TestUtils.h"

GTEST_TEST(ExitSeekTest, class)
{
  QS::PluginEntity::Properties exitSeekProperties;

  EXPECT_NO_THROW(QS::ExitSeek(exitSeekProperties, ""));
  QS::ExitSeek exitSeek(exitSeekProperties, "");

  QS::FindExitSensor findExitSensor({}, "");
  QS::EntityDependency<QS::Sensor> findExitSensorDependency{
    "", &findExitSensor, ""};
  exitSeek.setDependencies({findExitSensorDependency});

  QS::Actor actor(QS::TestUtils::getMinimalActorProperties(), "");
  actor.setPosition({0,0});

  auto exitProperties = QS::TestUtils::getMinimalExitProperties();
  exitProperties["x"] = "5.0";
  exitProperties["y"] = "5.0";
  QS::Exit exit(exitProperties, "");

  std::vector<const QS::Exit*> exits{&exit};
  QS::Sensable sensable(&actor, {}, exits, 0);
  findExitSensor.sense(sensable);

  // Test actor on the exit.
  actor.setPosition({5.0, 5.0});
  Eigen::Vector2f expectedForce = {0,0};
  Eigen::Vector2f actualForce = exitSeek.evaluate(&actor);
  EXPECT_EQ(expectedForce, actualForce)
    << "Actual: " << actualForce.format(QS::EigenHelper::prettyPrint);

  // Test actor away from the exit.
  actor.setPosition({0.0, 0.0});
  expectedForce = {12.727921485900879, 12.727921485900879};
  actualForce = exitSeek.evaluate(&actor);
  EXPECT_EQ(expectedForce, actualForce)
    << "Actual: " << actualForce.format(QS::EigenHelper::prettyPrint);

  // Test actor away from the exit.
  actor.setPosition({10.0, 10.0});
  expectedForce = {-12.727921485900879, -12.727921485900879};
  actualForce = exitSeek.evaluate(&actor);
  EXPECT_EQ(expectedForce, actualForce)
    << "Actual: " << actualForce.format(QS::EigenHelper::prettyPrint);
}
