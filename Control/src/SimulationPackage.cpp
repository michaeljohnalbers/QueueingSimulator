/**
 * @file SimulationPackage.cpp
 * @brief Defines SimulationPackage class
 *
 * @author Michael Albers
 */

#include "SimulationPackage.h"
#include "Simulation.h"
#include "Visualization.h"

QS::SimulationPackage::SimulationPackage(
  const std::string &theSimulationConfigFile,
  const std::string &theBaseDir) noexcept:

  myBaseDir(theBaseDir),
  mySimulationConfigFile(theSimulationConfigFile)
{
}

QS::SimulationPackage::~SimulationPackage()
{
  if (myVisualization)
  {
    myVisualization->stopThread();
  }
}

std::shared_ptr<QS::Simulation> QS::SimulationPackage::getSimulation() noexcept
{
  return mySimulation;
}

std::shared_ptr<QS::Visualization> QS::SimulationPackage::getVisualization()
  noexcept
{
  return myVisualization;
}

void QS::SimulationPackage::startSimulation()
{
  mySimulation.reset(new Simulation(myBaseDir, mySimulationConfigFile));

  World &world = mySimulation->getWorld();
  myVisualization.reset(createVisualization(world));
  myVisualization->startThread();
}
