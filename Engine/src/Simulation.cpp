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
  mySimulationConfigFile(theSimulationConfigFile)
{
  std::string pluginsDir{myBaseDir + "/plugins"};
  myPlugins.reset(new PluginCollection{pluginsDir});

  myEntityManager.reset(new EntityManager{myPlugins});
}

void QS::Simulation::readSimulation()
{
  std::string simulationsDir{myBaseDir + "/simulations"};
  std::unique_ptr<SimulationReader> simulationReader{
    new SimulationReader(mySimulationConfigFile, simulationsDir,
                         myEntityManager, myWorld)};

  simulationReader->read();
}