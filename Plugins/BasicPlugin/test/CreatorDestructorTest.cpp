/**
 * @file CreatorDestructorTest.cpp
 * @brief Unit test of basic plugin creator/destructor functions.
 *
 * @author Michael Albers
 */

#include <map>
#include <string>
#include "Actor.h"
#include "Behavior.h"
#include "BehaviorSet.h"
#include "gtest/gtest.h"

// No header for the creator/destructors since they aren't used directly in the
// code
extern "C"
{
  QS::Actor* actorCreator(
    const std::string&,
    const QS::PluginEntity::Properties&);
  void actorDestructor(QS::Actor*);

  QS::BehaviorSet* behaviorSetCreator(
    const std::string&,
    const QS::PluginEntity::Properties&);
  void behaviorSetDestructor(QS::BehaviorSet*);

  QS::Behavior* behaviorCreator(
    const std::string&,
    const QS::PluginEntity::Properties&);
  void behaviorDestructor(QS::Behavior*);

  QS::Sensor* sensorCreator(
    const std::string&,
    const QS::PluginEntity::Properties&);
  void sensorDestructor(QS::Sensor*);
}

GTEST_TEST(CreatorDestructor, testActor)
{
  std::map<std::string, std::string> properties;
  properties.insert({"mass", "0.1"});
  properties.insert({"radius", "0.1"});
  QS::Actor *actor = actorCreator("", properties);
  EXPECT_NE(nullptr, actor);

  EXPECT_NO_THROW(actorDestructor(actor));

  // Test catching an exception on Actor construction
  EXPECT_THROW(actorCreator("", {}), std::invalid_argument);
}

GTEST_TEST(CreatorDestructor, testBehaviorSet)
{
  QS::PluginEntity::Properties properties{{"Behavior", "Property"}};
  QS::BehaviorSet* behaviorSet = behaviorSetCreator("", properties);
  EXPECT_NE(nullptr, behaviorSet);

  EXPECT_NO_THROW(behaviorSetDestructor(behaviorSet));
}

GTEST_TEST(CreatorDestructor, testBehavior)
{
  // TODO: no behaviors implemented yet, when they are, use one here
  QS::Behavior *behavior = behaviorCreator("", {});
  EXPECT_EQ(nullptr, behavior);
  EXPECT_NO_THROW(behaviorDestructor(behavior));
}

GTEST_TEST(CreatorDestructor, testSensor)
{
  // TODO: no sensors implemented yet, when they are, use one here
  QS::Sensor *sensor = sensorCreator("", {});
  EXPECT_EQ(nullptr, sensor);
  EXPECT_NO_THROW(sensorDestructor(sensor));
}
