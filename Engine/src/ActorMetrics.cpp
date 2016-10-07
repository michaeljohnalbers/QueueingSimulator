/**
 * @file ActorMetrics.cpp
 * @brief Definition of ActorMetrics
 *
 * @author Michael Albers
 */

#include <cmath>
#include <stdexcept>
#include "Actor.h"
#include "ActorMetrics.h"

QS::ActorMetrics::ActorMetrics(const Actor *theActor) :
  myStartPosition(theActor->getPosition())
{
}

void QS::ActorMetrics::addGrossDistance(float theDistanceMeters)
{
  if (theDistanceMeters < 0.0)
  {
    throw std::invalid_argument("Cannot add a negative distance to gross. "
                                "Distance: " +
                                std::to_string(theDistanceMeters));
  }
  myGrossDistanceMeters += theDistanceMeters;
}

float QS::ActorMetrics::calculateNetDistance(const Actor *theActor)
{
  Eigen::Vector2f net = theActor->getPosition() - myStartPosition;
  myNetDistanceMeters = std::abs(net.norm());
  return myNetDistanceMeters;
}

float QS::ActorMetrics::getGrossDistanceMeters() const noexcept
{
  return myGrossDistanceMeters;
}

namespace QS
{
  std::ostream& operator<<(std::ostream &os,
                           const ActorMetrics &theMetrics)
  {
    os << "Gross Distance: " << std::fixed
       << theMetrics.myGrossDistanceMeters << " meters" << std::endl
       << "  Net Distance: " << std::fixed
       << theMetrics.myNetDistanceMeters << " meters" << std::endl;
    return os;
  }
}
