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
  Simulation(theBaseDir, theSimulationConfigFile, "", Mode::RealTime)
{
  readSimulation();
}

QS::Simulation::Simulation(const std::string &theBaseDir,
                           const std::string &theSimulationConfigFile,
                           const std::string &theOutputFile) :
  Simulation(theBaseDir, theSimulationConfigFile, theOutputFile, Mode::Batch)
{
  readSimulation();
}

QS::Simulation::Simulation(const std::string &theBaseDir,
                           const std::string &theSimulationConfigFile,
                           const std::string &theOutputFile,
                           Mode theMode) :
  myBaseDir(theBaseDir),
  myMode(theMode),
  myOutputFile(theOutputFile),
  mySimulationConfigFile(theSimulationConfigFile),
  myWorld(myMetrics)
{
  std::string pluginsDir{myBaseDir + "/plugins"};
  myPlugins.reset(new PluginCollection{pluginsDir});

  myEntityManager.reset(new EntityManager{myPlugins});
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
