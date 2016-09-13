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
  myMode(Mode::RealTime),
  mySimulationConfigFile(theSimulationConfigFile)
{
  readSimulation();
}

QS::Simulation::Simulation(const std::string &theBaseDir,
                           const std::string &theSimulationConfigFile,
                           const std::string &theOutputFile) :
  myBaseDir(theBaseDir),
  myMode(Mode::Batch),
  myOutputFile(theOutputFile),
  mySimulationConfigFile(theSimulationConfigFile)
{
  readSimulation();
}

void QS::Simulation::readSimulation()
{
  std::string pluginsDir{myBaseDir + "/plugins"};
  std::unique_ptr<PluginCollection> plugins{
    new PluginCollection{pluginsDir}};

  std::string simulationsDir{myBaseDir + "/simulations"};
  std::unique_ptr<SimulationReader> simulationReader{
    new SimulationReader{mySimulationConfigFile, simulationsDir, myWorld}};

  simulationReader->read();
}
