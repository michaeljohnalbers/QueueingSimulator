#pragma once

/**
 * @file Metrics.h
 * @brief Contains metrics for simulation run.
 *
 * @author Michael Albers
 */

#include <chrono>
#include <cfloat>
#include <map>
#include <memory>
#include <vector>

namespace QS
{
  class Actor;
  class ActorMetrics;

  /**
   * This class is used for all Metrics gathering in a simulation. It provides
   * mechanisms to update metrics, retrieve metrics and print them.
   */
  class Metrics
  {
    public:

    using Clock = std::chrono::system_clock;

    /** Shortcut to reduce typing. */
    using TimePoint = std::chrono::time_point<Clock>;

    /**
     * Output operator.
     */
    friend std::ostream& operator<<(std::ostream &os,
                                    const Metrics &theMetrics);

    /**
     * Default constructor. Sets the start time to the current time.
     */
    Metrics();

    /**
     * Copy constructor.
     */
    Metrics(const Metrics&) = default;

    /**
     * Move constructor.
     */
    Metrics(Metrics&&) = default;

    /**
     * Destructor
     */
    virtual ~Metrics() = default;

    /**
     * Adds the given number of seconds to the elapsed time.
     *
     * @param theNumberSeconds
     *          number of seconds to add
     * @throws std::invalid_argument
     *          if theNumberSeconds is negative
     */
    void addToElapsedTime(float theNumberSeconds);

    /**
     * Converts the given time to ISO 8601 format (in local time).
     *
     * @param theTime
     *          time to conver
     * @return time in ISO 8601 format
     */
    static std::string asISO8601(const TimePoint &theTime) noexcept;

    /**
     * Finalizes all Actor metrics. Calculates net distances and computes
     * basic statistics for Actor metrics. It is expected that the provided
     * Actors list matches that provided to initializeActorMetrics, if not
     * results are undefined.
     *
     * @param theActors
     *          actors to gather metrics for
     */
    void finalizeActorMetrics(const std::vector<Actor*> theActors) noexcept;

    /**
     * Returns the metrics for the given Actor.
     *
     * @param theActor
     *          Actor to retrieve metrics for.
     * @return metrics for the Actorj
     * @throws std::out_of_range
     *           if metrics for the given Actor cannot be found (be sure this
     *           Actor was included in the list passed to
     *           initializeActorMetrics)
     */
    const ActorMetrics& getActorMetrics(const Actor *theActor) const;

    /**
     * @see Const version
     */
    ActorMetrics& getActorMetrics(const Actor *theActor);

    /**
     * Returns the number of seconds the simulation has run.
     *
     * @return the number of seconds the simulation has run.
     */
    float getElapsedTimeInSeconds() const noexcept;

    /**
     * Returns the start time of the simulation.
     *
     * @return the start time of the simulation.
     */
    TimePoint getStartTime() const noexcept;

    /**
     * Returns the stop time of the simulation.
     *
     * @return the stop time of the simulation.
     */
    TimePoint getStopTime() const noexcept;

    /**
     * Initializes all actor metrics from the given list of Actors
     *
     * @param theActors
     *          actors to gather metrics for
     */
    void initializeActorMetrics(const std::vector<Actor*> theActors)
      noexcept;

    /**
     * Copy assignment operator
     */
    Metrics& operator=(const Metrics&) = default;

    /**
     * Move assignment operator
     */
    Metrics& operator=(Metrics&&) = default;

    /**
     * Sets the stop time to the current time;
     */
    void setStopTime();

    protected:

    /**
     * Helper class for basic statistics.
     */
    class MinMaxAvg
    {
      public:
      float myMin = FLT_MAX;
      float myMax = FLT_MIN;
      float myAvg = 0.0;

      void update(float theValue);
    };

    /** Statistics for Actor gross distance*/
    MinMaxAvg myActorGrossStats;

    /** Statistics for Actor net distance */
    MinMaxAvg myActorNetStats;

    /** Per-Actor metrics */
    std::map<const Actor*, std::shared_ptr<ActorMetrics>> myActorMetrics;

    /** Amount of time the simulation has run. */
    float myElapsedTime = 0.0;

    /** Start time of the simulation. */
    TimePoint myStartTime;

    /** Stop time of the simulation. */
    TimePoint myStopTime;

    private:

  };
}

