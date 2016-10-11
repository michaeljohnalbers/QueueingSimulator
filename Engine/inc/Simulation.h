#pragma once

/**
 * @file Simulation.h
 * @brief Encapsulates everything needed for a simulation run.
 *
 * @author Michael Albers
 */

#include <string>
#include "EntityManager.h"
#include "Metrics.h"
#include "World.h"

namespace QS
{
  class PluginCollection;

  /**
   * Holds everything for a simulation run, the world, plugins, plugin
   * entities, the works.
   */
  class Simulation
  {
    public:

    /**
     * Default constructor.
     */
    Simulation() = delete;

    /**
     * Constructor.
     *
     * @param theBaseDir
     *          QS base directory (installation directory)
     * @param theSimulationConfigFile
     *          simulation configuration file
     */
    Simulation(const std::string &theBaseDir,
               const std::string &theSimulationConfigFile);

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
     * Returns the metrics for this simulation.
     *
     * @return the metrics for this simulation.
     */
    const Metrics& getMetrics() const noexcept;

    /**
     * @see const version
     */
    Metrics& getMetrics()noexcept;

    /**
     * Returns the simulation world.
     *
     * @return simulation world
     */
    const World& getWorld() const noexcept;

    /**
     * Returns the simulation world.
     *
     * @return simulation world
     */
    World& getWorld() noexcept;

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

    /**
     * Read the simulation config file.
     *
     * @throws std::logic_error
     *           on any error reading the file
     */
    void readSimulation();

    /** Base directory */
    const std::string myBaseDir;

    /** Creator/manager of plugin entities. */
    std::shared_ptr<EntityManager> myEntityManager;

    /** Metrics for the simulation. */
    Metrics myMetrics;

    /** All loaded plugins. */
    std::shared_ptr<PluginCollection> myPlugins;

    /** Simulation configuration file. */
    const std::string mySimulationConfigFile;

    /** Simulation world. */
    World myWorld;
  };
}
