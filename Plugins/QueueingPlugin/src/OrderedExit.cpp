/**
 * @file OrderedExit.cpp
 * @brief Definition of OrderedExit
 *
 * @author Michael Albers
 */

#include <typeinfo>
#include "OrderedActor.h"
#include "OrderedExit.h"

QS::OrderedExit::OrderedExit(const Properties &theProperties,
                             const std::string &theTag) :
  Exit(theProperties, theTag)
{
}

bool QS::OrderedExit::canActorExit(const Actor *theActor) noexcept
{
  bool overlap = Exit::canActorExit(theActor);

  // Non-ordered Actors can exit if they overlap.
  bool canExit = overlap;

  const QS::OrderedActor *actor =
    dynamic_cast<const QS::OrderedActor*>(theActor);

  if (nullptr != actor)
  {
    bool orderMatches = actor->getRank() == myRank;

    canExit = orderMatches && canExit;

    if (canExit)
    {
      ++myRank;
    }
  }

  return canExit;
}

uint32_t QS::OrderedExit::getRank() const noexcept
{
  return myRank;
}
