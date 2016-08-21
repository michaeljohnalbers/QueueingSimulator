/**
 * @file PluginDefinitionTest.cpp
 * @brief Unit test of PluginDefinition class
 *
 * @author Michael Albers
 */

#include "PluginDefinition.h"
#include "gtest/gtest.h"

GTEST_TEST(PluginDefinitionTest, testConstruction)
{
  EXPECT_NO_THROW(QS::PluginDefinition plugin(""));
}

GTEST_TEST(PluginDefinition, testCopyMove)
{
  QS::PluginDefinition plugin("StandardPlugin");
  plugin.setLibrary("TheLibrary.so");
  QS::ActorDefinition actor("ActorDef", {"B", "C", "D"});
  plugin.addActorDefinition(actor);

  // Test copy constructor
  QS::PluginDefinition pluginCopy(plugin);
  EXPECT_EQ(plugin.getName(), pluginCopy.getName());
  EXPECT_EQ(plugin.getLibrary(), pluginCopy.getLibrary());
  EXPECT_EQ(plugin.getActorDefinitions(),
            pluginCopy.getActorDefinitions());

  // Test move constructor
  QS::PluginDefinition pluginMove(std::move(pluginCopy));
  EXPECT_EQ(plugin.getName(), pluginMove.getName());
  EXPECT_EQ(plugin.getLibrary(), pluginMove.getLibrary());
  EXPECT_EQ(plugin.getActorDefinitions(),
            pluginMove.getActorDefinitions());

  // Test copy assignment operator
  QS::PluginDefinition pluginAssignCopy{""};
  pluginAssignCopy = plugin;
  EXPECT_EQ(plugin.getName(), pluginAssignCopy.getName());
  EXPECT_EQ(plugin.getLibrary(), pluginAssignCopy.getLibrary());
  EXPECT_EQ(plugin.getActorDefinitions(),
            pluginAssignCopy.getActorDefinitions());

  // Test move assignment operator
  QS::PluginDefinition pluginAssignMove{""};
  pluginAssignMove = plugin;
  EXPECT_EQ(plugin.getName(), pluginAssignMove.getName());
  EXPECT_EQ(plugin.getLibrary(), pluginAssignMove.getLibrary());
  EXPECT_EQ(plugin.getActorDefinitions(),
            pluginAssignMove.getActorDefinitions());
}

GTEST_TEST(PluginDefinition, testActors)
{
  QS::PluginDefinition plugin("StandardPlugin");

  // No actor stuff "built-in"
  EXPECT_TRUE(plugin.getActorDefinitions().empty());
  EXPECT_EQ(0u, plugin.getActorCreatorDestructor().first.size());
  EXPECT_EQ(0u, plugin.getActorCreatorDestructor().second.size());

  // Test basic addition
  std::vector<QS::ActorDefinition> actors{
    {"Actor1", {"BehaviorSet1"}},
    {"Actor2", {"BehaviorSet2"}},
    {"Actor3", {"BehaviorSet3", "OtherBehavior"}}
  };

  for (auto actorDef: actors)
  {
    EXPECT_NO_THROW(plugin.addActorDefinition(actorDef));
  }

  EXPECT_EQ(actors, plugin.getActorDefinitions());

  // Test adding duplicate
  try
  {
    plugin.addActorDefinition(actors[0]);
    FAIL();
  }
  catch (const std::invalid_argument &e)
  {
    EXPECT_EQ(std::string("Cannot add duplicate actor definition for '" +
                          actors[0].getName() + "'."),
              e.what());
  }
  catch (...)
  {
    FAIL();
  }

  // Test the constructor/destructor
  plugin.setActorCreatorDestructor("ActorConstructor", "ActorDestructor");
  EXPECT_EQ("ActorConstructor", plugin.getActorCreatorDestructor().first);
  EXPECT_EQ("ActorDestructor", plugin.getActorCreatorDestructor().second);
}

GTEST_TEST(PluginDefinition, testBehaviors)
{
  QS::PluginDefinition plugin("StandardPlugin");

  // No behavior stuff "built-in"
  EXPECT_TRUE(plugin.getBehaviorDefinitions().empty());
  EXPECT_EQ(0u, plugin.getBehaviorCreatorDestructor().first.size());
  EXPECT_EQ(0u, plugin.getBehaviorCreatorDestructor().second.size());

  // Test basic addition
  std::vector<QS::BehaviorDefinition> behaviors{
    {"Behavior1", {"Sensor"}},
    {"Behavior2", {"Sensor2"}},
    {"Behavior3", {"Sensor3", "OtherSensor"}}
  };

  for (auto behaviorDef: behaviors)
  {
    EXPECT_NO_THROW(plugin.addBehaviorDefinition(behaviorDef));
  }

  EXPECT_EQ(behaviors, plugin.getBehaviorDefinitions());

  // Test adding duplicate
  try
  {
    plugin.addBehaviorDefinition(behaviors[0]);
    FAIL();
  }
  catch (const std::invalid_argument &e)
  {
    EXPECT_EQ(std::string("Cannot add duplicate behavior definition for '" +
                          behaviors[0].getName() + "'."),
              e.what());
  }
  catch (...)
  {
    FAIL();
  }

  // Test the constructor/destructor
  plugin.setBehaviorCreatorDestructor("BehaviorConstructor",
                                      "BehaviorDestructor");
  EXPECT_EQ("BehaviorConstructor", plugin.getBehaviorCreatorDestructor().first);
  EXPECT_EQ("BehaviorDestructor", plugin.getBehaviorCreatorDestructor().second);
}

GTEST_TEST(PluginDefinition, testBehaviorSets)
{
  QS::PluginDefinition plugin("StandardPlugin");

  // No behavior set stuff "built-in"
  EXPECT_TRUE(plugin.getBehaviorSetDefinitions().empty());
  EXPECT_EQ(0u, plugin.getBehaviorSetCreatorDestructor().first.size());
  EXPECT_EQ(0u, plugin.getBehaviorSetCreatorDestructor().second.size());

  // Test basic addition
  std::vector<QS::BehaviorSetDefinition> behaviorSets{
    {"BehaviorSet1", {"Behanvior1"}},
    {"BehaviorSet2", {"Behavior2"}},
    {"BehaviorSet3", {"Behavior3", "OtherBehavior"}}
  };

  for (auto behaviorsetDef: behaviorSets)
  {
    EXPECT_NO_THROW(plugin.addBehaviorSetDefinition(behaviorsetDef));
  }

  EXPECT_EQ(behaviorSets, plugin.getBehaviorSetDefinitions());

  // Test adding duplicate
  try
  {
    plugin.addBehaviorSetDefinition(behaviorSets[0]);
    FAIL();
  }
  catch (const std::invalid_argument &e)
  {
    EXPECT_EQ(std::string("Cannot add duplicate behavior set definition for '" +
                          behaviorSets[0].getName() + "'."),
              e.what());
  }
  catch (...)
  {
    FAIL();
  }

  // Test the constructor/destructor
  plugin.setBehaviorSetCreatorDestructor("BehaviorSetConstructor",
                                         "BehaviorSetDestructor");
  EXPECT_EQ("BehaviorSetConstructor",
            plugin.getBehaviorSetCreatorDestructor().first);
  EXPECT_EQ("BehaviorSetDestructor",
            plugin.getBehaviorSetCreatorDestructor().second);
}

GTEST_TEST(PluginDefinition, testLibrary)
{
  QS::PluginDefinition plugin("StandardPlugin");

  // Test no library built in
  EXPECT_EQ(0u, plugin.getLibrary().size());

  // Test getter/setter
  plugin.setLibrary("SomeLibrary");
  EXPECT_EQ("SomeLibrary", plugin.getLibrary());
}

GTEST_TEST(PluginDefinition, testSensors)
{
  QS::PluginDefinition plugin("StandardPlugin");

  // No sensor stuff "built-in"
  EXPECT_TRUE(plugin.getSensorDefinitions().empty());
  EXPECT_EQ(0u, plugin.getSensorCreatorDestructor().first.size());
  EXPECT_EQ(0u, plugin.getSensorCreatorDestructor().second.size());

  // Test basic addition
  std::vector<QS::SensorDefinition> sensors{
    {"Sensor1"},
    {"Sensor2"},
    {"Sensor3"}
  };

  for (auto sensorDef: sensors)
  {
    EXPECT_NO_THROW(plugin.addSensorDefinition(sensorDef));
  }

  EXPECT_EQ(sensors, plugin.getSensorDefinitions());

  // Test adding duplicate
  try
  {
    plugin.addSensorDefinition(sensors[0]);
    FAIL();
  }
  catch (const std::invalid_argument &e)
  {
    EXPECT_EQ(std::string("Cannot add duplicate sensor definition for '" +
                          sensors[0].getName() + "'."),
              e.what());
  }
  catch (...)
  {
    FAIL();
  }

  // Test the constructor/destructor
  plugin.setSensorCreatorDestructor("SensorConstructor", "SensorDestructor");
  EXPECT_EQ("SensorConstructor", plugin.getSensorCreatorDestructor().first);
  EXPECT_EQ("SensorDestructor", plugin.getSensorCreatorDestructor().second);
}

GTEST_TEST(PluginDefinition, testGetName)
{
  QS::PluginDefinition plugin("StandardPlugin");

  // getName
  EXPECT_EQ("StandardPlugin", plugin.getName());
}
