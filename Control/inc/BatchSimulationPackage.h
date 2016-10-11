#pragma once

/**
 * @file BatchSimulationPackage.h
 * @brief Packages everything needed to run a simulation in real-time mode.
 *
 * @author Michael Albers
 */

#include <string>
#include "SimulationPackage.h"

namespace QS
{
  /**
   * Everything needed to run a simulation in batch mode. In batch mode the
   * simulation visualization is written to a movie output file instead of
   * being displayed to the user in real-time.
   */
  class BatchSimulationPackage : public SimulationPackage
  {
    public:

    /**
     * Default constructor
     */
    BatchSimulationPackage() = delete;

    /**
     * Constructor.
     *
     * @param theSimulationConfigFile
     *          config file that defines the simulation
     * @param theBaseDir
     *          base directory for finding plugins, simulations, etc.
     * @param theOutputFile
     *          full path and file name of movie file
     * @throws std::invalid_argument
     *          if theOutputFile is empty
     */
    BatchSimulationPackage(const std::string &theSimulationConfigFile,
                           const std::string &theBaseDir,
                           const std::string &theOutputFile);

    /**
     * Copy constructor.
     */
    BatchSimulationPackage(const BatchSimulationPackage&) = delete;

    /**
     * Move constructor.
     */
    BatchSimulationPackage(BatchSimulationPackage&&) = delete;

    /**
     * Destructor.
     */
    virtual ~BatchSimulationPackage() = default;

    /**
     * Copy assignment operator
     */
    BatchSimulationPackage& operator=(const BatchSimulationPackage&) = delete;

    /**
     * Move assignment operator
     */
    BatchSimulationPackage& operator=(BatchSimulationPackage&&) = delete;

    protected:

    /**
     * Creates a visualization object for batch mode.
     *
     * @param theWorld
     *          simulation world
     * @return batch mode visualization
     */
    virtual Visualization* createVisualization(World &theWorld) override;

    private:

    /** Full path and name of output file. */
    const std::string myOutputFile;
  };
}
