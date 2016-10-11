/**
 * @file BatchSimulationPackage.cpp
 * @brief Defines BatchSimulationPackage class
 *
 * @author Michael Albers
 */

#include <stdexcept>
#include "BatchSimulationPackage.h"
#include "BatchVisualization.h"

QS::BatchSimulationPackage::BatchSimulationPackage(
  const std::string &theSimulationConfigFile,
  const std::string &theBaseDir,
  const std::string &theOutputFile) :
  SimulationPackage(theSimulationConfigFile, theBaseDir),
  myOutputFile(theOutputFile)
{
  if (myOutputFile.empty())
  {
    throw std::invalid_argument("Invalid batch mode output file provided. "
                                "It cannot be an empty name.");
  }
}

QS::Visualization* QS::BatchSimulationPackage::createVisualization(
  World &theWorld)
{
  return new BatchVisualization(theWorld, myOutputFile);
}
