#ifndef _SimulatorTypedef_h_
#define _SimulatorTypedef_h_

/**
 * @file SimulatorTypedef.h
 * @brief Houses typedefs common in the simulator.
 *
 * @author Michael Albers
 */

/**
 * General QueueingSimulator (QS) namespace.
 */
namespace QS
{
  /**
   * Overall run configurations.
   */
  typedef enum
  {
    /** Run to produce code benchmark data. */
    Benchmark,
    /** Run an actual simulation. */
    Simulation,
  } RunConfiguration;
};

#endif
