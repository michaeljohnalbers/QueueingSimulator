#pragma once

/**
 * @file BatchVisualization.h
 * @brief Visualization of the simulation in real-time
 *
 * @author Michael Albers
 */

#include <fstream>
#include <string>
#include "Visualization.h"

namespace QS
{
  /**
   * Batch visualization runs the simulation at a fixed frame rate and writes
   * the drawn data to an output movie file instead of displaying it to the
   * user.
   *
   * The format of the output movie file is raw frame buffer data (i.e., pixels
   * and their colors). This file should be post-processed by a multimedia
   * conversion program such as 'avconv' or 'ffmpeg'.
   */
  // TODO: change it so this class write the converted file instead of having
  // the user do it.
  class BatchVisualization : public Visualization
  {
    public:

    /**
     * Default constructor.
     */
    BatchVisualization() = delete;

    /**
     * Constructor.
     *
     * @param theWorld
     *          simulation world
     * @param theOutputFileName
     *          full path and file name of movie file
     */
    BatchVisualization(World &theWorld,
                       const std::string &theOutputFileName);

    /**
     * Copy constructor
     */
    BatchVisualization(const BatchVisualization&) = delete;

    /**
     * Move constructor
     */
    BatchVisualization(BatchVisualization&&) = delete;

    /**
     * Destructor.
     */
    virtual ~BatchVisualization() = default;

    /**
     * Copy assignment operator
     */
    BatchVisualization& operator=(const BatchVisualization&) = delete;

    /**
     * Move assignment operator
     */
    BatchVisualization& operator=(BatchVisualization&&) = delete;

    protected:

    /**
     * Returns a fixed update interval.
     *
     * @return update interval
     */
    virtual float getUpdateInterval() noexcept override;

    /**
     * Gets the window size for the simulation.
     *
     * @return width/height pair
     */
    virtual std::tuple<int, int> getWindowDimensions() noexcept override;

    /**
     * Writes frame buffer data to the output file.
     */
    virtual void preBufferSwap() noexcept override;

    /**
     * Makes window invisible.
     */
    void setWindowHints() noexcept override;

    private:

    /** Number of numbers per pixel (i.e, RGB)*/
    static constexpr auto NUMBERS_PER_PIXEL = 3u;

    /** Size of pixel data buffer */
    int myBufferSize;

    /** Buffer to which pixel data will be placed. */
    std::unique_ptr<char[]> myInvertedPixelBuffer;

    /** Output file handle. */
    std::ofstream myOutputFile;

    /** Full path and file name of movie file */
    const std::string myOutputFileName;

    /** Buffer to which pixel data will be placed. */
    std::unique_ptr<char[]> myPixelBuffer;

    /** Window height */
    int myWindowHeight;

    /** Window width */
    int myWindowWidth;
  };
}
