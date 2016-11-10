/**
 * @file CreatorDestructorTest.cpp
 * @brief Unit test of basic plugin creator/destructor functions.
 *
 * @author Michael Albers
 */

#include <map>
#include <string>
#include "gtest/gtest.h"
#include "Actor.h"
#include "Behavior.h"
#include "BehaviorSet.h"
#include "Exit.h"
#include "Sensor.h"
#include "TestUtils.h"

// No header for the creator/destructors since they aren't used directly in the
// code
extern "C"
{
  QS::Actor* actorCreator(
    const std::string&,
    const QS::PluginEntity::Properties&,
    const std::string &theTag);
  void actorDestructor(QS::Actor*);

  QS::BehaviorSet* behaviorSetCreator(
    const std::string&,
    const QS::PluginEntity::Properties&,
    const std::string &theTag);
  void behaviorSetDestructor(QS::BehaviorSet*);

  QS::Behavior* behaviorCreator(
    const std::string&,
    const QS::PluginEntity::Properties&,
    const std::string &theTag);
  void behaviorDestructor(QS::Behavior*);

  QS::Sensor* sensorCreator(
    const std::string&,
    const QS::PluginEntity::Properties&,
    const std::string &theTag);
  void sensorDestructor(QS::Sensor*);

  QS::Exit* exitCreator(
    const std::string&,
    const QS::PluginEntity::Properties&,
    const std::string &theTag);
  void exitDestructor(QS::Exit*);
}

GTEST_TEST(CreatorDestructor, testActor)
{
  QS::Actor *actor = actorCreator(
    "", QS::TestUtils::getMinimalActorProperties(), "");
  EXPECT_NE(nullptr, actor);

  EXPECT_NO_THROW(actorDestructor(actor));

  // Test catching an exception on Actor construction
  EXPECT_THROW(actorCreator("", {}, ""), std::invalid_argument);
}

GTEST_TEST(CreatorDestructor, testBehaviorSet)
{
  QS::PluginEntity::Properties properties{{"BehaviorSet", "Property"}};
  QS::BehaviorSet* behaviorSet = behaviorSetCreator("", properties, "");
  EXPECT_NE(nullptr, behaviorSet);

  EXPECT_NO_THROW(behaviorSetDestructor(behaviorSet));
}

GTEST_TEST(CreatorDestructor, testBehavior)
{
  QS::PluginEntity::Properties properties{{"Behavior", "Property"}};
  QS::Behavior *behavior = behaviorCreator("", properties, "");
  EXPECT_NE(nullptr, behavior);

  EXPECT_NO_THROW(behaviorDestructor(behavior));
}

GTEST_TEST(CreatorDestructor, testSensor)
{
  QS::PluginEntity::Properties properties{{"Sensor", "Property"}};
  QS::Sensor *sensor = sensorCreator("", properties, "");
  EXPECT_NE(nullptr, sensor);

  EXPECT_NO_THROW(sensorDestructor(sensor));
}

GTEST_TEST(CreatorDestructor, testExit)
{
  QS::Exit *exit = exitCreator(
    "", QS::TestUtils::getMinimalExitProperties(), "");
  EXPECT_NE(nullptr, exit);

  EXPECT_NO_THROW(exitDestructor(exit));
}
