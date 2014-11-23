/**
 * @file Individual.cpp
 * @brief Implementation of the Individual class.
 *
 * @author Michael Albers
 */

#include <iomanip>
#include <sstream>

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
                       int32_t theRank,
                       QS::RunConfiguration &theRunConfiguration) :
  myBodyRadius(theMass * 0.005), // Rough approximation
  myMass(theMass),
  myMaximumAcceleration(theMaximumAcceleration),
  myMaximumForce(225),
  myMaximumSpeed(theMaximumSpeed),
  myOrientation(theOrientation),
  myOriginalPosition(thePosition),
  myPosition(thePosition),
  myRank(theRank),
  myRunConfiguration(theRunConfiguration)
{
  int32_t idealDistance =
    EigenHelper::distance(myPosition, Exit::getPosition()) - Exit::getRadius();
  myIdealStraightLineTime = idealDistance / myMaximumSpeed;
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
  Eigen::Vector2f steeringForce;

  if (QS::Benchmark == myRunConfiguration)
  {
    steeringForce = Seek::calculateForce(*this, &Exit::getPosition());
  }
  else
  {
    // TODO: change to RankedLeaderFollow when it's ready
    steeringForce = Seek::calculateForce(*this, &Exit::getPosition());
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
  // if (10000 == myRank) // For output filtering
  // {
  //   Logger::log("------");
  //   Logger::log("      Rank: " + std::to_string(myRank));
  //   Logger::log("  Position: " + EigenHelper::print(myPosition));
  //   Logger::log("  Velocity: " + EigenHelper::print(timeCorrectedVelocity));
  //   Logger::log("Frame time: " + std::to_string(theFrameTime));
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

//****************************************
// Individual::getIdealStraightLineTime
//****************************************
float Individual::getIdealStraightLineTime() const
{
  return myIdealStraightLineTime;
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
