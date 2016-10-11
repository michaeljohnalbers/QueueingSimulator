#pragma once

/**
 * @file SimulationPackage.h
 * @brief Packages everything needed to run a simulation.
 *
 * @author Michael Albers
 */

#include <memory>

namespace QS
{
  class Simulation;
  class Visualization;
  class World;

  class SimulationPackage
  {
    public:

    /**
     * Default constructor.
     */
    SimulationPackage() = delete;

    /**
     * Constructor.
     *
     * @param theSimulationConfigFile
     *          config file that defines the simulation
     * @param theBaseDir
     *          base directory for finding plugins, simulations, etc.
     */
    SimulationPackage(const std::string &theSimulationConfigFile,
                      const std::string &theBaseDir) noexcept;

    /**
     * Copy constructor.
     */
    SimulationPackage(const SimulationPackage&) = delete;

    /**
     * Move constructor.
     */
    SimulationPackage(SimulationPackage&&) = delete;

    /**
     * Destructor.
     */
    virtual ~SimulationPackage();

    /**
     * Returns the simulation object. Object will be null until
     * startSimulation is called.
     *
     * @return the simulation object.
     */
    std::shared_ptr<Simulation> getSimulation() noexcept;

    /**
     * Returns the visualization object. Object will be null until
     * startSimulation is called.
     *
     * @return the visualization object.
     */
    std::shared_ptr<Visualization> getVisualization() noexcept;

    /**
     * Copy assignment operator
     */
    SimulationPackage& operator=(const SimulationPackage&) = delete;

    /**
     * Move assignment operator
     */
    SimulationPackage& operator=(SimulationPackage&&) = delete;

    /**
     * Does everything to initialize and start the simulation running.
     *
     * @throws std::exception
     *           on any error
     */
    void startSimulation();

    /**
     * Create the appropriate visualization object.
     *
     * @param theWorld
     *          simulation world
     * @return visualization object, this class will take ownership of the
     *         pointer
     */
    virtual Visualization* createVisualization(World &theWorld) = 0;

    private:

    /** QS base directory */
    const std::string myBaseDir;

    /** Simulation configuration file. */
    const std::string mySimulationConfigFile;

    /** Actual simulation. */
    std::shared_ptr<Simulation> mySimulation;

    /** Visualization of the simulation. */
    std::shared_ptr<Visualization> myVisualization;
  };
}
