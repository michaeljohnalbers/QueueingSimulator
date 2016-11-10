/**
 * @file CreatorDestructorTest.cpp
 * @brief Unit test of basic plugin creator/destructor functions.
 *
 * @author Michael Albers
 */

#include <string>
#include "gtest/gtest.h"
#include "OrderedActor.h"
// #include "Behavior.h"
// #include "BehaviorSet.h"
#include "OrderedExit.h"
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

  // QS::BehaviorSet* behaviorSetCreator(
  //   const std::string&,
  //   const QS::PluginEntity::Properties&,
  //   const std::string &theTag);
  // void behaviorSetDestructor(QS::BehaviorSet*);

  // QS::Behavior* behaviorCreator(
  //   const std::string&,
  //   const QS::PluginEntity::Properties&,
  //   const std::string &theTag);
  // void behaviorDestructor(QS::Behavior*);

  // QS::Sensor* sensorCreator(
  //   const std::string&,
  //   const QS::PluginEntity::Properties&,
  //   const std::string &theTag);
  // void sensorDestructor(QS::Sensor*);

  QS::Exit* exitCreator(
    const std::string&,
    const QS::PluginEntity::Properties&,
    const std::string &theTag);
  void exitDestructor(QS::Exit*);
}

GTEST_TEST(CreatorDestructor, testActor)
{
  QS::PluginEntity::Properties properties{
    QS::TestUtils::getMinimalActorProperties()};
  properties["rank"] = "1";

  QS::Actor *actor = nullptr;
  ASSERT_NO_THROW(actor = actorCreator("OrderedActor", properties, ""));
  ASSERT_NE(nullptr, actor);
  EXPECT_EQ(typeid(*actor), typeid(QS::OrderedActor));

  EXPECT_NO_THROW(actorDestructor(actor));

  // Test catching an exception on Actor construction
  EXPECT_THROW(actorCreator("OrderedActor", {}, ""), std::invalid_argument);

  // Test an invalid actor type
  EXPECT_THROW(actorCreator("Actor", properties, ""), std::invalid_argument);
}

// GTEST_TEST(CreatorDestructor, testBehaviorSet)
// {
//   QS::PluginEntity::Properties properties{{"BehaviorSet", "Property"}};
//   QS::BehaviorSet* behaviorSet = behaviorSetCreator("", properties, "");
//   EXPECT_NE(nullptr, behaviorSet);

//   EXPECT_NO_THROW(behaviorSetDestructor(behaviorSet));
// }

// GTEST_TEST(CreatorDestructor, testBehavior)
// {
//   QS::PluginEntity::Properties properties{{"Behavior", "Property"}};
//   QS::Behavior *behavior = behaviorCreator("", properties, "");
//   EXPECT_NE(nullptr, behavior);

//   EXPECT_NO_THROW(behaviorDestructor(behavior));
// }

// GTEST_TEST(CreatorDestructor, testSensor)
// {
//   QS::PluginEntity::Properties properties{{"Sensor", "Property"}};
//   QS::Sensor *sensor = sensorCreator("", properties, "");
//   EXPECT_NE(nullptr, sensor);

//   EXPECT_NO_THROW(sensorDestructor(sensor));
// }

GTEST_TEST(CreatorDestructor, testExit)
{
  QS::Exit *exit = exitCreator(
    "", QS::TestUtils::getMinimalExitProperties(), "");
  ASSERT_NE(nullptr, exit);
  EXPECT_EQ(typeid(*exit), typeid(QS::OrderedExit));

  EXPECT_NO_THROW(exitDestructor(exit));

  // Test catching an exception on Exit construction
  EXPECT_THROW(exitCreator("", {}, ""), std::invalid_argument);
}
