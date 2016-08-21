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
    const std::string &theActorName,
    const std::map<std::string, std::string> &theProperties);
  void actorDestructor(QS::Actor *theActor);

  QS::BehaviorSet* behaviorSetCreator(
    const std::string &theBehaviorSetName,
    const std::map<std::string, QS::Behavior*> &theBehaviors);
  void behaviorSetDestructor(QS::BehaviorSet *theBehaviorSet);

  QS::Behavior* behaviorCreator(
    const std::string &theBehaviorName,
    const std::vector<std::string> &theInputSensorTypes);
  void behaviorDestructor(QS::Behavior *theBehavior);

  QS::Sensor* sensorCreator(const std::string &theSensorName);
  void sensorDestructor(QS::Sensor *theSensor);
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
  QS::Behavior *behavior = reinterpret_cast<QS::Behavior*>(0x1);
  QS::BehaviorSet* behaviorSet = behaviorSetCreator("",
                                                    {{"Behavior", behavior}});
  EXPECT_NE(nullptr, behaviorSet);

  EXPECT_NO_THROW(behaviorSetDestructor(behaviorSet));

  // Test catching an exception on BehaviorSet construction
  EXPECT_THROW(behaviorSetCreator("", {}), std::invalid_argument);
}

GTEST_TEST(CreatorDestructor, testBehavior)
{
  // TODO: no behaviors implemented yet, when they are, use one here
  QS::Behavior *behavior = behaviorCreator("", {""});
  EXPECT_EQ(nullptr, behavior);
  EXPECT_NO_THROW(behaviorDestructor(behavior));
}

GTEST_TEST(CreatorDestructor, testSensor)
{
  // TODO: no sensors implemented yet, when they are, sue one here
  QS::Sensor *sensor = sensorCreator("");
  EXPECT_EQ(nullptr, sensor);
  EXPECT_NO_THROW(sensorDestructor(sensor));
}
