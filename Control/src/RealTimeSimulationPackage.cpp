/**
 * @file RealTimeSimulationPackage.cpp
 * @brief Defines RealTimeSimulationPackage class
 *
 * @author Michael Albers
 */

#include "RealTimeSimulationPackage.h"
#include "RealTimeVisualization.h"

QS::RealTimeSimulationPackage::RealTimeSimulationPackage(
  const std::string &theSimulationConfigFile,
  const std::string &theBaseDir) noexcept :
  SimulationPackage(theSimulationConfigFile, theBaseDir)
{
}

QS::Visualization* QS::RealTimeSimulationPackage::createVisualization(
  World &theWorld)
{
  return new RealTimeVisualization(theWorld);
}
