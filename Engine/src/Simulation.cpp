/**
 * @file Simulation.cpp
 * @brief Definition of Simulation class
 *
 * @author Michael Albers
 */

#include <memory>
#include "PluginCollection.h"
#include "Simulation.h"
#include "SimulationReader.h"

QS::Simulation::Simulation(const std::string &theBaseDir,
                           const std::string &theSimulationConfigFile) :
  myBaseDir(theBaseDir),
  mySimulationConfigFile(theSimulationConfigFile),
  myWorld(myMetrics)
{
  std::string pluginsDir{myBaseDir + "/plugins"};
  myPlugins.reset(new PluginCollection{pluginsDir});

  myEntityManager.reset(new EntityManager{myPlugins});

  readSimulation();
}

const QS::Metrics& QS::Simulation::getMetrics() const noexcept
{
  return myMetrics;
}

QS::Metrics& QS::Simulation::getMetrics() noexcept
{
  return myMetrics;
}

const QS::World& QS::Simulation::getWorld() const noexcept
{
  return myWorld;
}

QS::World& QS::Simulation::getWorld() noexcept
{
  return myWorld;
}

void QS::Simulation::readSimulation()
{
  std::string simulationsDir{myBaseDir + "/simulations"};
  std::unique_ptr<SimulationReader> simulationReader{
    new SimulationReader(mySimulationConfigFile, simulationsDir,
                         myEntityManager, myWorld)};

  simulationReader->read();

  myWorld.initializeActorMetrics();
}
