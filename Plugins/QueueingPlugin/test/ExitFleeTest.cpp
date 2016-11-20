/**
 * @file ExitFleeTest.cpp
 * @brief Unit tests for ExitFlee class
 *
 * @author Michael Albers
 */

#include "gtest/gtest.h"
#include "Actor.h"
#include "EigenHelper.h"
#include "Exit.h"
#include "ExitFlee.h"
#include "FindExitSensor.h"
#include "Sensable.h"
#include "TestUtils.h"

GTEST_TEST(ExitFleeTest, class)
{
  QS::PluginEntity::Properties exitFleeProperties;

  EXPECT_NO_THROW(QS::ExitFlee(exitFleeProperties, ""));
  QS::ExitFlee exitFlee(exitFleeProperties, "");

  QS::FindExitSensor findExitSensor({}, "");
  QS::EntityDependency<QS::Sensor> findExitSensorDependency{
    "", &findExitSensor, ""};
  exitFlee.setDependencies({findExitSensorDependency});

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
  Eigen::Vector2f actualForce = exitFlee.evaluate(&actor);
  EXPECT_EQ(expectedForce, actualForce)
    << "Actual: " << actualForce.format(QS::EigenHelper::prettyPrint);

  // Test actor away from the exit.
  actor.setPosition({0.0, 0.0});
  expectedForce = {-12.727921485900879, -12.727921485900879};
  actualForce = exitFlee.evaluate(&actor);
  EXPECT_EQ(expectedForce, actualForce)
    << "Actual: " << actualForce.format(QS::EigenHelper::prettyPrint);

  // Test actor away from the exit.
  actor.setPosition({10.0, 10.0});
  expectedForce = {12.727921485900879, 12.727921485900879};
  actualForce = exitFlee.evaluate(&actor);
  EXPECT_EQ(expectedForce, actualForce)
    << "Actual: " << actualForce.format(QS::EigenHelper::prettyPrint);
}
