/**
 * @file Individual.cpp
 * @brief Implementation of the Individual class.
 *
 * @author Michael Albers
 */

#include <EigenHelper.h>
#include <Exit.h>
#include <Individual.h>
#include <Logger.h>
#include <Seek.h>

//***********************
// Individual::Individual
//***********************
Individual::Individual(float theMass,
                       float theMaximumSpeed,
                       float theMaximumAcceleration,
                       float theOrientation,
                       const Eigen::Vector2f &thePosition,
                       int32_t theRank) :
  myBodyRadius(theMass * 0.005), // Rough approximation
  myMass(theMass),
  myMaximumAcceleration(theMaximumAcceleration),
  myMaximumForce(225),
  myMaximumSpeed(theMaximumSpeed),
  myOrientation(theOrientation),
  myPosition(thePosition),
  myRank(theRank)
{
  // TODO: caclcuate maximum force from mass & max speed?
  // See http://www.ccohs.ca/oshanswers/ergonomics/push1.html
  // See http://msis.jsc.nasa.gov/sections/section04.htm#_4.9_STRENGTH
  // See https://forum.solidworks.com/thread/43514
}

//************************
// Individual::~Individual
//************************
Individual::~Individual()
{
}

//**********************
// Individual::collision
//**********************
bool Individual::collision(const Individual *theIndividual)
{
  float distance = EigenHelper::distance(getPosition(),
                                         theIndividual->getPosition());
  float radii = getBodyRadius() + theIndividual->getBodyRadius();
  return distance < radii;
}

//************************
// Individual::frameUpdate
//************************
// TODO: remvoe frame time & just run as fast as possible?
void Individual::frameUpdate(std::shared_ptr<NearestN> theNeighbors,
                             float theFrameTime)
{
  Seek seek;

  Eigen::Vector2f steeringForce(
    seek.calculateForce(*this, nullptr, &Exit::getPosition()));
  Eigen::Vector2f steeringForce2(steeringForce);
  // TODO: don't forget to truncate steering force, max force
  //  steeringForce /= myMass;

  myCurrentVelocity = EigenHelper::truncate(myCurrentVelocity + steeringForce,
                                            myMaximumSpeed);

  auto adjustment = myCurrentVelocity * theFrameTime;

  // TODO: this might be a little off.
  myDistanceTraveled += adjustment.norm();

  myPosition += adjustment;

  //  Logger::log(EigenHelper::print(myPosition));

  myExited = Exit::canExit(*this);
  if (myExited)
  {
    myExitTime = std::chrono::system_clock::now();
  }
}

//**************************
// Individual::getBodyRadius
//**************************
float Individual::getBodyRadius() const
{
  return myBodyRadius;
}

//*******************************
// Individual::getCurrentVelocity
//*******************************
const Eigen::Vector2f& Individual::getCurrentVelocity() const
{
  return myCurrentVelocity;
}

//********************************
// Individual::getDistanceTraveled
//********************************
float Individual::getDistanceTraveled() const
{
  return myDistanceTraveled;
}

//**********************
// Individual::getExited
//**********************
bool Individual::getExited() const
{
  return myExited;
}

//************************
// Individual::getExitTime
//************************
const std::chrono::time_point<std::chrono::system_clock>&
Individual::getExitTime() const
{
  return myExitTime;
}

//***********************************
// Individual::getMaximumAcceleration
//***********************************
float Individual::getMaximumAcceleration() const
{
  return myMaximumAcceleration;
}

//****************************
// Individual::getMaximumForce
//****************************
float Individual::getMaximumForce() const
{
  return myMaximumForce;
}

//****************************
// Individual::getMaximumSpeed
//****************************
float Individual::getMaximumSpeed() const
{
  return myMaximumSpeed;
}

//***************************
// Individual::getOrientation
//***************************
float Individual::getOrientation() const
{
  return myOrientation;
}

//************************
// Individual::getPosition
//************************
const Eigen::Vector2f& Individual::getPosition() const
{
  return myPosition;
}

//********************
// Individual::getRank
//********************
int32_t Individual::getRank() const
{
  return myRank;
}

//*********************
// Individual::getSpeed
//*********************
float Individual::getSpeed() const
{
  return mySpeed;
}

//*******************************
// Individual::isPositionInRadius
//*******************************
bool Individual::isPositionInRadius(const Eigen::Vector2f &thePosition)
{
  float distance = EigenHelper::distance(myPosition, thePosition);
  return distance <= myBodyRadius;
}
