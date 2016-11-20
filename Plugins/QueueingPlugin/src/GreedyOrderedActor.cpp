/**
 * @file GreedyOrderedActor.cpp
 * @brief Definition of GreedyOrderedActor
 *
 * @author Michael Albers
 */

#include "GreedyOrderedActor.h"
#include "GreedyOrdering.h"
#include "PluginHelper.h"
#include "SemiRationalOrdering.h"
#include "Sensable.h"

QS::GreedyOrderedActor::GreedyOrderedActor(const Properties &theProperties,
                                           const std::string &theTag) :
  OrderedActor(theProperties, theTag),
  myPreviousPosition(getPosition())
{
  myToleratedMovement_m = PluginHelper::getProperty(
    theProperties, "tolerated movement", false, PluginHelper::toFloat, 0.005f);
  if (myToleratedMovement_m < 0.0)
  {
    throw std::invalid_argument(
      "Invalid tolerated movement, " + std::to_string(myToleratedMovement_m) +
      ", must be >= 0 in GreedyOrderedActor constructor.");
  }

  myPatienceTime_s = PluginHelper::getProperty(
    theProperties, "patience time", false, PluginHelper::toFloat, 20.0f);
  if (myToleratedMovement_m < 0.0)
  {
    throw std::invalid_argument(
      "Invalid patience time, " + std::to_string(myPatienceTime_s) +
      ", must be >= 0 in GreedyOrderedActor constructor.");
  }
}

Eigen::Vector2f QS::GreedyOrderedActor::evaluate(const Sensable &theSensable)
{
  Eigen::Vector2f positionDifference = getPosition() - myPreviousPosition;
  // Very first update will satisfy this, but that's OK. It's just one.
  if (positionDifference.norm() < myToleratedMovement_m)
  {
    myTimeWithoutMovement += theSensable.getIntervalInSeconds();
  }

  myPreviousPosition = getPosition();

  return Actor::evaluate(theSensable);
}

QS::BehaviorSet* QS::GreedyOrderedActor::selectBehaviorSet(
  const Sensable &theSensable)
{
  if (nullptr == myGreedyBehaviorSet)
  {
    auto dependencies = getDependencies();
    myGreedyBehaviorSet = dynamic_cast<QS::GreedyOrdering*>(
      getDependencies()[0].myEntity);
    mySemiRationalBehaviorSet = dynamic_cast<QS::SemiRationalOrdering*>(
      getDependencies()[1].myEntity);
  }

  BehaviorSet *behaviorSet = myGreedyBehaviorSet;
  if (myTimeWithoutMovement >= myPatienceTime_s)
  {
    behaviorSet = mySemiRationalBehaviorSet;
  }

  return behaviorSet;
}
