/**
 * @file SimulationEntityConfigurations.cpp
 * @brief Configurations of SimulationEntityConfigurations class
 *
 * @author Michael Albers
 */

#include "SimulationEntityConfiguration.h"

QS::SimulationEntityConfiguration::SimulationEntityConfiguration(
  const std::string &theType,
  const std::string &theTag,
  const std::string &theSource) :

  mySource(theSource),
  myTag(theTag),
  myType(theType)
{
}

void QS::SimulationEntityConfiguration::addDependencyConfiguration(
  const SimulationEntityConfiguration &theEntityConfiguration) noexcept
{
  myDependencyConfigurations.push_back(theEntityConfiguration);
}

std::vector<QS::SimulationEntityConfiguration>
QS::SimulationEntityConfiguration::getDependencyConfigurations() const noexcept
{
  return myDependencyConfigurations;
}

void QS::SimulationEntityConfiguration::addProperty(
  const std::string &theProperty,
  const std::string &theValue) noexcept
{
  myProperties[theProperty] = theValue;
}

QS::PluginEntity::Properties QS::SimulationEntityConfiguration::getProperties()
  const noexcept
{
  return myProperties;
}

std::string QS::SimulationEntityConfiguration::getSource() const noexcept
{
  return mySource;
}

std::string QS::SimulationEntityConfiguration::getTag() const noexcept
{
  return myTag;
}

std::string QS::SimulationEntityConfiguration::getType() const noexcept
{
  return myType;
}
