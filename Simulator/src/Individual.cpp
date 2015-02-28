/**
 * @file Individual.cpp
 * @brief Implementation of the Individual class.
 *
 * @author Michael Albers
 */

#include <cmath>
#include <iomanip>
#include <sstream>

#include <EigenHelper.h>
#include <Exit.h>
#include <Individual.h>
#include <Logger.h>
#include <RankedLeaderFollow.h>
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
  myOriginalPosition(thePosition),
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
void Individual::frameUpdate(std::shared_ptr<NearestN> theNeighbors,
                             float theFrameTime)
{
  Eigen::Vector2f steeringForce(
    RankedLeaderFollow::calculateForce(*this, *theNeighbors));

  // Sometimes the steering force will have a nan component. This can happen
  // with Seek and the position of the Individual is exactly the same as the
  // target position. Each steering force should make sure it doesn't return
  // nan. However, that is error prone (it would be easy to miss a condition
  // which causes nan or forget to check altogether). So the check is being
  // made here.
  for (int32_t ii = 0; ii < 2; ++ii)
  {
    if (std::isnan(steeringForce[ii]))
    {
      steeringForce[ii] = 0.0;
    }
  }

  // The nearest I can figure, Reynolds appears to use this operation to
  // turn the "force" returned from the steering behavior(s) (which is usually
  // the result of some number of velocity vectors added together) into
  // an actual force (i.e. Newtons).
  Eigen::Vector2f force(EigenHelper::truncate(steeringForce, myMaximumForce));

  Eigen::Vector2f acceleration(force / myMass);

  Eigen::Vector2f newVelocity(myVelocity + (acceleration * theFrameTime));
  myVelocity = EigenHelper::truncate(newVelocity, myMaximumSpeed);

  auto timeCorrectedVelocity = myVelocity * theFrameTime;

  // norm() returns the vector's magnitude
  myDistanceTraveled += timeCorrectedVelocity.norm();

  // Forward Euler integration.
  myPosition += timeCorrectedVelocity;

  // Just in case the Individual wanders out of the world. This can happen
  // if the frame time gets large enough. That can happen when the simulator
  // has more individuals than it can handle.
  // TODO: need to make sure it doesn't pass max world value
  if (myPosition[0] < 0.0)
  {
    myPosition[0] = 0.0;
  }
  if (myPosition[1] < 0.0)
  {
    myPosition[1] = 0.0;
  }

  // Uncomment for debugging.
  // static int count[3] = {0,0,0};
  // if (myRank < 3 && ++count[myRank] % 10000 == 0) // For output filtering
  // {
  //   count[myRank] = 0;
  //   Logger::log("------");
  //   Logger::log("      Rank: " + std::to_string(myRank));
  //   Logger::log("  Position: " + EigenHelper::print(myPosition));
  //   Logger::log("  Velocity: " + EigenHelper::print(timeCorrectedVelocity));
  //   Logger::log(" Max Speed: " + std::to_string(myMaximumSpeed));
  //   Logger::log("  Distance: " + std::to_string(
  //                 EigenHelper::distance(myPosition, Exit::getPosition())));
  //   Logger::log("Frame time: " + std::to_string(theFrameTime));
  //   Logger::log("Steering Force: " + EigenHelper::print(steeringForce));
  //   Logger::log("------");
  // }

  myExited = Exit::canExit(*this);
  if (myExited)
  {
    myExitTime = std::chrono::system_clock::now();
  }
}

//*************************
// Individual::getBasicData
//*************************
std::string Individual::getBasicData() const
{
  std::ostringstream basicData;
  basicData << "Rank: " << myRank
            << ", Mass: " << std::setprecision(4) << myMass
            << ", Radius: " << std::setprecision(2) << myBodyRadius
            << ", Max Accel: " << std::setprecision(4) << myMaximumAcceleration
            << ", Max Force: " << std::setprecision(3) << myMaximumForce
            << ", Max Speed: " << std::setprecision(2) << myMaximumSpeed;
  return basicData.str();
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
const Eigen::Vector2f& Individual::getVelocity() const
{
  return myVelocity;
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

//********************************
// Individual::getOriginalPosition
//********************************
const Eigen::Vector2f Individual::getOriginalPosition() const
{
  return myOriginalPosition;
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
