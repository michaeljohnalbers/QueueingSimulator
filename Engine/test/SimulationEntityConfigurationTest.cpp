/**
 * @file SimulationEntityConfigurationTest.cpp
 * @brief Unit test of SimulationEntityConfiguration class
 *
 * @author Michael Albers
 */

#include "SimulationEntityConfiguration.h"
#include "gtest/gtest.h"

GTEST_TEST(SimulationEntityConfigurationTest, basicAttributes)
{
  // Test that this class doesn't care if values are empty (let the
  // reader/schema care about that)
  ASSERT_NO_THROW(QS::SimulationEntityConfiguration config("", "", ""));

  auto type = "MyType";
  auto tag = "SomeTag";
  auto source = "MySource";
  ASSERT_NO_THROW(QS::SimulationEntityConfiguration(type, tag, source));

  QS::SimulationEntityConfiguration config(type, tag, source);

  // Now test the getters
  EXPECT_EQ(type, config.getType());
  EXPECT_EQ(tag, config.getTag());
  EXPECT_EQ(source, config.getSource());
}

GTEST_TEST(SimulationEntityConfigurationTest, properties)
{
  QS::SimulationEntityConfiguration config("", "", "");
  EXPECT_EQ(0u, config.getProperties().size());

  QS::PluginEntity::Properties properties{
    {"Prop1", "Value1"},
    {"Prop2", "Value2"},
    {"Prop3", "Value3"}
  };

  for (auto property : properties)
  {
    EXPECT_NO_THROW(config.addProperty(property.first, property.second));
  }

  auto configProperties = config.getProperties();
  EXPECT_EQ(properties, configProperties);

  // Test that duplicates overwrite
  properties["Prop2"] = "OtherValue";
  EXPECT_NO_THROW(config.addProperty("Prop2", properties["Prop2"]));
  configProperties = config.getProperties();
  EXPECT_EQ(properties, configProperties);
}

GTEST_TEST(SimulationEntityConfigurationTest, dependencyConfigurations)
{
  QS::SimulationEntityConfiguration config("", "", "");
  EXPECT_EQ(0u, config.getDependencyConfigurations().size());

  QS::SimulationEntityConfiguration dep1("Dep1", "", "");
  QS::SimulationEntityConfiguration dep2("", "", "");
  QS::SimulationEntityConfiguration dep3("", "", "");

  config.addDependencyConfiguration(dep1);
  config.addDependencyConfiguration(dep2);
  config.addDependencyConfiguration(dep3);

  auto dependencyConfigs = config.getDependencyConfigurations();
  EXPECT_EQ(3u, dependencyConfigs.size());

  // Test adding a duplicate
  config.addDependencyConfiguration(dep1);
  dependencyConfigs = config.getDependencyConfigurations();
  EXPECT_EQ(dependencyConfigs.back().getType(),
            dependencyConfigs.front().getType());
}
