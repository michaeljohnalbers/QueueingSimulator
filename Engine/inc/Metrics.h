#pragma once

/**
 * @file Metrics.h
 * @brief Contains metrics for simulation run.
 *
 * @author Michael Albers
 */

#include <chrono>
#include <cfloat>
#include <limits>
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

    /**
     * Helper class for basic statistics.
     */
    template<class T >
    class MinMaxAvg
    {
      public:
      T myMin = std::numeric_limits<T>::max();
      T myMax = std::numeric_limits<T>::min();
      T myAvg = 0.0;
      T myCount = 0.0;

      void update(T theValue)
      {
        myMin = std::min(theValue, myMin);
        myMax = std::max(theValue, myMax);
        myAvg += theValue;
        myCount += 1.0;
      }
    };

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
     * Finalizes all simulation metrics.
     */
    void finalizeSimulationMetrics() noexcept;

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
     * Returns the metrics for update interval times.
     *
     * @return update interval metrics.
     */
    MinMaxAvg<float> getUpdateMetrics() const noexcept;

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

    /** Statistics for Actor gross distance*/
    MinMaxAvg<float> myActorGrossStats;

    /** Statistics for Actor net distance */
    MinMaxAvg<float> myActorNetStats;

    /** Per-Actor metrics */
    std::map<const Actor*, std::shared_ptr<ActorMetrics>> myActorMetrics;

    /** Amount of time the simulation has run. */
    float myElapsedTime = 0.0;

    /** Start time of the simulation. */
    TimePoint myStartTime;

    /** Stop time of the simulation. */
    TimePoint myStopTime;

    /** Metrics for update intervals. */
    MinMaxAvg<float> myUpdateMetrics;

    private:

  };
}

