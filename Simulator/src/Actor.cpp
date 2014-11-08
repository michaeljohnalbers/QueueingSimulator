/**
 * @file Actor.cpp
 * @brief Actor class.
 *
 * @author Michael Albers
 */

#include <Actor.h>

const Actor::Position Actor::InvalidPosition(-1,-1);

Actor::Actor(int32_t theRank) :
  myCurrentPosition(InvalidPosition),
  myRank(theRank),
  myEndTime(0)
{
}

 Actor::Actor(int32_t theRank, std::time_t &theStartTime) :
  myCurrentPosition(InvalidPosition),
  myRank(theRank),
  myEndTime(0)
{
}

Actor::Actor(const Actor &theActor) :
  myCurrentPosition(theActor.myCurrentPosition),
  myPreviousPositions(theActor.myPreviousPositions),
  myRank(theActor.myRank),
  myEndTime(theActor.myEndTime)
{
}

Actor::~Actor()
{
}

Actor::Position Actor::getCurrentPosition() const
{
  return myCurrentPosition;
}

std::time_t Actor::getEndTime() const
{
  return myEndTime;
}

const std::vector<Actor::Position>& Actor::getPreviousPositions() const
{
  return myPreviousPositions;
}

int32_t Actor::getRank() const
{
  return myRank;
}

void Actor::setCurrentPosition(const Position &theNewPosition)
{
  if (myCurrentPosition != InvalidPosition)
  {
    myPreviousPositions.push_back(myCurrentPosition);
  }
  myCurrentPosition = theNewPosition;
}

void Actor::setEndTime()
{
  myEndTime = std::time(nullptr);
}
