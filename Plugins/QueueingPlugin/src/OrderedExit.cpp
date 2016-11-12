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

  // According to:
  // http://stackoverflow.com/questions/579887/how-expensive-is-rtti
  // typeid comparsion & static_cast is faster than dynamic cast
  if (typeid(*theActor) == typeid(QS::OrderedActor))
  {
    const QS::OrderedActor *actor =
      static_cast<const QS::OrderedActor*>(theActor);

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
