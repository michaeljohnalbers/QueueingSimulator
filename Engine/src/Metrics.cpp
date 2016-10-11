/**
 * @file Metrics.cpp
 * @brief Definition of Metrics
 *
 * @author Michael Albers
 */

#include <iomanip>
#include <sstream>
#include <stdexcept>
#include "ActorMetrics.h"
#include "Metrics.h"

QS::Metrics::Metrics()
{
  myStartTime = Clock::now();
}

void QS::Metrics::addToElapsedTime(float theNumberSeconds)
{
  if (theNumberSeconds < 0.0)
  {
    throw std::invalid_argument("Cannot add negative seconds to elapsed time.");
  }
  myElapsedTime += theNumberSeconds;
}

std::string QS::Metrics::asISO8601(const TimePoint &theTime) noexcept
{
  std::time_t time = QS::Metrics::Clock::to_time_t(theTime);
  std::tm tm = *std::localtime(&time);
  std::ostringstream timeStr;
  timeStr << std::put_time(&tm, "%FT%T");
  return timeStr.str();
}

void QS::Metrics::finalizeActorMetrics(const std::vector<Actor*> theActors)
  noexcept
{
  for (const auto actor : theActors)
  {
    auto &actorMetrics = getActorMetrics(actor);
    float net = actorMetrics.calculateNetDistance(actor);
    myActorNetStats.update(net);
    float gross = actorMetrics.getGrossDistanceMeters();
    myActorGrossStats.update(gross);
  }

  myActorGrossStats.myAvg /= static_cast<float>(theActors.size());
  myActorNetStats.myAvg /= static_cast<float>(theActors.size());
}

const QS::ActorMetrics& QS::Metrics::getActorMetrics(
  const Actor *theActor) const
{
  return *myActorMetrics.at(theActor);
}

QS::ActorMetrics& QS::Metrics::getActorMetrics(const Actor *theActor)
{
  return const_cast<ActorMetrics&>(
    static_cast<const Metrics*>(this)->getActorMetrics(theActor));
}

float QS::Metrics::getElapsedTimeInSeconds() const noexcept
{
  return myElapsedTime;
}

QS::Metrics::TimePoint QS::Metrics::getStartTime() const noexcept
{
  return myStartTime;
}

QS::Metrics::TimePoint QS::Metrics::getStopTime() const noexcept
{
  return myStopTime;
}

void QS::Metrics::initializeActorMetrics(
  const std::vector<QS::Actor*> theActors) noexcept
{
  for (Actor *actor : theActors)
  {
    myActorMetrics[actor].reset(new ActorMetrics(actor));
  }
}

void QS::Metrics::setStopTime()
{
  myStopTime = Clock::now();
}

void QS::Metrics::MinMaxAvg::update(float theValue)
{
  myMin = std::min(theValue, myMin);
  myMax = std::max(theValue, myMax);
  myAvg += theValue;
}

namespace QS
{
std::ostream& operator<<(std::ostream &os, const Metrics &theMetrics)
{
  os << "Metrics" << std::endl
     << "=======" << std::endl << std::endl
     << "Simulation Metrics" << std::endl
     << "------------------" << std::endl
     << "  Start time: " << Metrics::asISO8601(theMetrics.getStartTime())
     << std::endl
     << "   Stop time: " << Metrics::asISO8601(theMetrics.getStopTime())
     << std::endl
     << "Elapsed Time: " << std::fixed
     << theMetrics.getElapsedTimeInSeconds() << " seconds" << std::endl;

  os << std::endl
     << "Actor Metrics" << std::endl
     << "-------------" << std::endl
     << "High Gross Distance: " << std::fixed
     << theMetrics.myActorGrossStats.myMax << " meters" << std::endl
     << "Average Gross Distance: " << std::fixed
     << theMetrics.myActorGrossStats.myAvg << " meters" << std::endl
     << "Low Gross Distance: " << std::fixed
     << theMetrics.myActorGrossStats.myMin << " meters" << std::endl
     << "High Net Distance: " << std::fixed
     << theMetrics.myActorNetStats.myMax << " meters" << std::endl
     << "Average Net Distance: " << std::fixed
     << theMetrics.myActorNetStats.myAvg << " meters" << std::endl
     << "Low Net Distance: " << std::fixed
     << theMetrics.myActorNetStats.myMin << " meters" << std::endl
     << std::endl;
  
  for (auto actorMetrics : theMetrics.myActorMetrics)
  {
    os << "--Actor" << std::endl
       << *(actorMetrics.second) << std::endl;
  }
  return os;
}
}
