/**
 * @file TestUtils.cpp
 * @brief Definition of TestUtils
 *
 * @author Michael Albers
 */

#include "TestUtils.h"

QS::PluginEntity::Properties QS::TestUtils::getFullActorProperties() noexcept
{
  PluginEntity::Properties properties{getMinimalActorProperties()};
  // Using unique numbers to aid in test robust-ness.
  properties["color"] = "0.5 0.6 0.7";
  properties["orientation"] = "0.8";
  properties["max force"] = "0.85";
  properties["max speed"] = "9.0";

  return properties;
}

QS::PluginEntity::Properties QS::TestUtils::getMinimalActorProperties() noexcept
{
  PluginEntity::Properties properties{
  // Using unique numbers to aid in test robust-ness.
    {"radius", "1.0"},
    {"mass", "2.0"},
    {"x", "3.0"},
    {"y", "4.0"}};

  return properties;
}

QS::PluginEntity::Properties QS::TestUtils::getFullExitProperties() noexcept
{
  PluginEntity::Properties properties{getMinimalExitProperties()};
  // Using unique numbers to aid in test robust-ness.
  properties["color"] = "0.4 0.5 0.6";

  return properties;
}

QS::PluginEntity::Properties QS::TestUtils::getMinimalExitProperties() noexcept
{
  return getTargetProperties();
}

QS::PluginEntity::Properties QS::TestUtils::getTargetProperties() noexcept
{
  PluginEntity::Properties properties{
  // Using unique numbers to aid in test robust-ness.
    {"radius", "1.0"},
    {"x", "2.0"},
    {"y", "3.0"}};

  return properties;
}
