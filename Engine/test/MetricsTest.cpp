/**
 * @file MetricsTest.cpp
 * @brief Unit test of Metrics class
 *
 * @author Michael Albers
 */

#include <stdexcept>
#include "gtest/gtest.h"
#include "Actor.h"
#include "Metrics.h"

// Using 3000 as I believe that the duration is in microseconds or
// nanoseconds. But it's something very small. Tried using 1 in an EXPECT_NEAR
// and it failed due to a difference of 2304. 3000 gives some fudge room. This
// is not enough when using valgrind.
static constexpr QS::Metrics::Clock::rep TIME_TOLERANCE = 3000;

GTEST_TEST(MetricsTest, actorMetrics)
{
  QS::PluginEntity::Properties properties{
    {"radius", "0.1"},
    {"mass", "0.2"},
    {"x", "0.0"},
    {"y", "0.0"}};

  std::vector<QS::Actor*> actors;

  for (int ii = 0; ii < 10; ++ii)
  {
    properties["x"] = std::to_string(ii);
    properties["y"] = std::to_string(ii);
    actors.push_back(new QS::Actor(properties, ""));
  }

  QS::Metrics metrics;
  metrics.initializeActorMetrics(actors);

  for (auto actor : actors)
  {
    EXPECT_NO_THROW(metrics.getActorMetrics(actor));
  }
  EXPECT_THROW(metrics.getActorMetrics(nullptr), std::out_of_range);
  QS::Actor *actor = new QS::Actor(properties, "");
  EXPECT_THROW(metrics.getActorMetrics(actor), std::out_of_range);
  delete actor;

  for (auto actor : actors)
  {
    delete actor;
  }
}

GTEST_TEST(MetricsTest, startTime)
{
  QS::Metrics::TimePoint now = QS::Metrics::Clock::now();
  QS::Metrics metrics;
  EXPECT_NEAR(now.time_since_epoch().count(),
              metrics.getStartTime().time_since_epoch().count(),
              TIME_TOLERANCE);
}

GTEST_TEST(MetricsTest, elapsedTimeAndUpdate)
{
  QS::Metrics metrics;
  EXPECT_EQ(0.0, metrics.getElapsedTimeInSeconds());
  float timeToAdd = 3.567;
  metrics.addToElapsedTime(timeToAdd);
  EXPECT_EQ(timeToAdd, metrics.getElapsedTimeInSeconds());
  EXPECT_THROW(metrics.addToElapsedTime(-1.0), std::invalid_argument);

  // Test update metrics
  metrics.addToElapsedTime(0.55);
  metrics.addToElapsedTime(3.55);
  metrics.addToElapsedTime(0.07);

  metrics.finalizeSimulationMetrics();
  auto updateMetrics = metrics.getUpdateMetrics();
  EXPECT_FLOAT_EQ(3.567, updateMetrics.myMax);
  EXPECT_FLOAT_EQ(0.07, updateMetrics.myMin);
  EXPECT_FLOAT_EQ(1.93425, updateMetrics.myAvg);
}

GTEST_TEST(MetricsTest, stopTime)
{
  QS::Metrics metrics;
  EXPECT_EQ(0, metrics.getStopTime().time_since_epoch().count());
  QS::Metrics::TimePoint now = QS::Metrics::Clock::now();
  metrics.setStopTime();
  EXPECT_NEAR(now.time_since_epoch().count(),
              metrics.getStartTime().time_since_epoch().count(),
              TIME_TOLERANCE);
}

GTEST_TEST(MetricsTest, output)
{
  QS::Metrics metrics;
  std::ostringstream metricsOutput;
  metricsOutput << metrics;
  EXPECT_FALSE(metricsOutput.str().empty());
}
