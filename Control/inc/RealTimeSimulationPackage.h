#pragma once

/**
 * @file RealTimeSimulationPackage.h
 * @brief Packages everything needed to run a simulation in real-time mode.
 *
 * @author Michael Albers
 */

#include "SimulationPackage.h"

namespace QS
{
  class RealTimeSimulationPackage : public SimulationPackage
  {
    public:

    /**
     * Default constructor
     */
    RealTimeSimulationPackage() = delete;

    /**
     * Constructor.
     *
     * @param theSimulationConfigFile
     *          config file that defines the simulation
     * @param theBaseDir
     *          base directory for finding plugins, simulations, etc.
     */
    RealTimeSimulationPackage(const std::string &theSimulationConfigFile,
                              const std::string &theBaseDir) noexcept;

    /**
     * Copy constructor.
     */
    RealTimeSimulationPackage(const RealTimeSimulationPackage&) = delete;

    /**
     * Move constructor.
     */
    RealTimeSimulationPackage(RealTimeSimulationPackage&&) = delete;

    /**
     * Destructor.
     */
    virtual ~RealTimeSimulationPackage() = default;

    /**
     * Copy assignment operator
     */
    RealTimeSimulationPackage& operator=(const RealTimeSimulationPackage&) =
      delete;

    /**
     * Move assignment operator
     */
    RealTimeSimulationPackage& operator=(RealTimeSimulationPackage&&) = delete;

    protected:

    /**
     * Creates a visualization object for real-time mode.
     *
     * @param theWorld
     *          simulation world
     * @return real-time mode visualization
     */
    virtual Visualization* createVisualization(World &theWorld) override;

    private:

  };
}
