#pragma once

/**
 * @file Simulation.h
 * @brief Encapsulates everything needed for a simulation run.
 *
 * @author Michael Albers
 */

#include <string>
#include "World.h"

namespace QS
{
  class Simulation
  {
    public:

    enum class Mode
    {
      RealTime,
      Batch
    };

    /**
     * Default constructor.
     */
    Simulation() = delete;

    /**
     * Constructor for real-time simulations.
     *
     * @param theBaseDir
     *          QS base directory (installation directory)
     * @param theSimulationConfigFile
     *          simulation configuration file
     */
    Simulation(const std::string &theBaseDir,
               const std::string &theSimulationConfigFile);

    /**
     * Constructor for batch simulations.
     *
     * @param theBaseDir
     *          QS base directory (installation directory)
     * @param theSimulationConfigFile
     *          simulation configuration file
     * @param theOutputFile
     *          output file for batch mode
     */
    Simulation(const std::string &theBaseDir,
               const std::string &theSimulationConfigFile,
               const std::string &theOutputFile);

    /**
     * Copy constructor.
     */
    Simulation(const Simulation&) = delete;

    /**
     * Move constructor.
     */
    Simulation(Simulation&&) = delete;

    /**
     * Destructor.
     */
    ~Simulation() = default;

    /**
     * Copy assignment operator
     */
    Simulation& operator=(const Simulation&) = delete;

    /**
     * Move assignment operator
     */
    Simulation& operator=(Simulation&&) = delete;

    protected:

    private:

    void readSimulation();

    /** Base directory */
    const std::string myBaseDir;

    /** Mode of the simulation. */
    const Mode myMode;

    /** Output file for batch mode (empty when in real-time mode) */
    const std::string myOutputFile;

    /** Simulation configuration file. */
    const std::string mySimulationConfigFile;

    /** Simulation world. */
    World myWorld;
  };
}
