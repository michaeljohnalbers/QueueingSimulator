/**
 * @file BasicWalk.cpp
 * @brief Definition of BasicWalk
 *
 * @author Michael Albers
 */

#include "BasicWalk.h"
#include "Sensable.h"
#include "Walk.h"

QS::BasicWalk::BasicWalk(const Properties &theProperties) :
  BehaviorSet(theProperties)
{
}

Eigen::Vector2f QS::BasicWalk::evaluate(const Actor *theActor,
                                        const Sensable &theSensable)
{
  auto vector = dynamic_cast<Walk*>(getDependencies()[0].myEntity)->evaluate(
    theActor, theSensable.getInterval());
  return vector;
}
