/**
 * @file Bucket.cpp
 * @brief Implementation of Bucket class
 *
 * @author Michael Albers
 */

#include <algorithm>

#include <Bucket.h>
#include <Individual.h>
#include <Logger.h>
#include <NearestN.h>

//***************
// Bucket::Bucket
//***************
Bucket::Bucket(const Eigen::Vector2f &theOrigin,
               int32_t theHeight,
               int32_t theLength) :
  myHeight(theHeight),
  myLength(theLength),
  myOrigin(theOrigin)
{
  for (uint32_t row = 0; row < myAdjacentBuckets.size(); ++row)
  {
    for (uint32_t col = 0; col < myAdjacentBuckets[row].size(); ++col)
    {
      myAdjacentBuckets[row][col] = nullptr;
    }
  }
}

//****************
// Bucket::~Bucket
//****************
Bucket::~Bucket()
{
}

//**************************
// Bucket::addAdjacentBucket
//**************************
void Bucket::addAdjacentBucket(Bucket *theBucket,
                               int32_t theRowOffset,
                               int32_t theColumnOffset)
{
  myAdjacentBuckets[theRowOffset+1][theColumnOffset+1] = theBucket;
  if (nullptr != theBucket)
  {
    myAdjacentBucketList.push_back(theBucket);
  }
}

//**********************
// Bucket::containsPoint
//**********************
bool Bucket::containsPoint(const Eigen::Vector2f &thePosition) const
{
  bool inBucket = ((thePosition[0] >= myOrigin[0] &&
                    thePosition[0] < myOrigin[0] + myLength) &&
                   (thePosition[1] >= myOrigin[1] &&
                    thePosition[1] < myOrigin[1] + myHeight));
  return inBucket;
}

//********************
// Bucket::frameUpdate
//********************
void Bucket::frameUpdate(float theFrameTime)
{
  // TODO: parallelize
  auto iter = myIndividuals.begin();
  while (iter != myIndividuals.end())
  {
    Individual *individual = *iter;

    auto neighbors = NearestN::findNeighbors(individual, this);
    individual->frameUpdate(neighbors, theFrameTime);
    const Eigen::Vector2f &newPosition = individual->getPosition();

    if (individual->getExited())
    {
      Logger::log("Individual with rank " +
                  std::to_string(individual->getRank()) + " has exited.");
      iter = myIndividuals.erase(iter);
      continue;
    }

    if (! containsPoint(newPosition))
    {
      auto numberAdjacentBuckets = myAdjacentBucketList.size();
#     pragma omp parallel for
      for (uint32_t ii = 0; ii < numberAdjacentBuckets; ++ii)
      {
        Bucket *adjacentBucket = myAdjacentBucketList[ii];
        if (adjacentBucket != nullptr &&
            adjacentBucket->containsPoint(newPosition))
        {
          // No critical section needed here. The new point can only fall
          // into one adjacent bucket.
          ReBucketer rebucketer(individual, adjacentBucket);
          myRebucketedIndividuals.push_back(rebucketer);
        }
      }
    }

    ++iter;
  }
}

//***************************
// Bucket::getAdjacentBuckets
//***************************
const std::vector<Bucket*>& Bucket::getAdjacentBuckets() const
{
  return myAdjacentBucketList;
}

//***********************
// Bucket::getIndividuals
//***********************
const std::vector<Individual*>& Bucket::getIndividuals() const
{
  return myIndividuals;
}

//****************************
// Bucket::rebucketIndividuals
//****************************
void Bucket::rebucketIndividuals()
{
  for (auto &rebucketer : myRebucketedIndividuals)
  {
    auto iter = std::find(myIndividuals.begin(),
                          myIndividuals.end(),
                          rebucketer.getIndividual());
    myIndividuals.erase(iter);
    rebucketer.rebucket();
  }
  myRebucketedIndividuals.clear();
}

//**************************
// Bucket::receiveIndividual
//**************************
void Bucket::receiveIndividual(Individual *theIndividual)
{
  myIndividuals.push_back(theIndividual);
}
