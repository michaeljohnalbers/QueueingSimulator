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

extern int32_t glbIndividualsLeft;

//***************
// Bucket::Bucket
//***************
Bucket::Bucket(const Eigen::Vector2f &theOrigin,
               int32_t theHeight,
               int32_t theLength,
               int32_t theMaximumNumberIndividuals,
               QS::RunConfiguration theRunConfiguration) :
  myHeight(theHeight),
  myIndividuals(theMaximumNumberIndividuals, nullptr),
  myIndividualsToRemove(theMaximumNumberIndividuals, -1),
  myLength(theLength),
  myOrigin(theOrigin),
  myRebucketedIndividuals(theMaximumNumberIndividuals),
  myRunConfiguration(theRunConfiguration)
{
  // In order to avoid simultaneous memory allocation by the std::vectors
  // the initial capacity/size is set on contruction. These clear calls set
  // the size back to zero.
  myIndividuals.clear();
  myIndividualsToRemove.clear();
  myRebucketedIndividuals.clear();

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

//******************************
// Bucket::adjustIndividualsList
//******************************
void Bucket::adjustIndividualsList()
{
  auto iter = myIndividualsToRemove.begin();
  while (iter != myIndividualsToRemove.end())
  {
    int32_t index = *iter;
    //    Logger::log("nulling " + std::to_string(index) + " of " +
    //                std::to_string(myIndividuals.size()));
    myIndividuals[index] = nullptr;
    ++iter;
  }
  myIndividualsToRemove.clear();

  auto iter2 = myIndividuals.begin();
  while (iter2 != myIndividuals.end())
  {
    if (nullptr == *iter2)
    {
      iter2 = myIndividuals.erase(iter2);
    }
    else
    {
      ++iter2;
    }
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
  int32_t numberIndividuals = myIndividuals.size();
#ifndef SERIAL
# pragma omp parallel for
#endif
  for (int32_t ii = 0; ii < numberIndividuals; ++ii)
  {
    Individual *individual = myIndividuals[ii];
    bool removeIndividual = false;

    auto neighbors = NearestN::findNeighbors(individual, this);
    individual->frameUpdate(neighbors, theFrameTime);
    const Eigen::Vector2f &newPosition = individual->getPosition();

    if (individual->getExited())
    {
#ifndef SERIAL
#     pragma omp critical (glbIndividualsLeft)
#endif
      {
        --glbIndividualsLeft;
      }
      if (QS::Benchmark == myRunConfiguration)
      {
        Logger::log("Individual with rank " +
                    std::to_string(individual->getRank()) +
                    " has exited. Individuals remaining: " +
                    std::to_string(glbIndividualsLeft));
      }
      else
      {
        Logger::log("Individual with rank " +
                    std::to_string(individual->getRank()) +
                    " has exited.");
      }
      removeIndividual = true;
    }
    else if (! containsPoint(newPosition))
    {
      auto numberAdjacentBuckets = myAdjacentBucketList.size();
#ifndef SERIAL
#     pragma omp parallel for
#endif
      for (uint32_t ii = 0; ii < numberAdjacentBuckets; ++ii)
      {
        Bucket *adjacentBucket = myAdjacentBucketList[ii];
        if (adjacentBucket != nullptr &&
            adjacentBucket->containsPoint(newPosition))
        {
#ifndef SERIAL
#         pragma omp critical
#endif
          {
            ReBucketer rebucketer(individual, adjacentBucket);
            myRebucketedIndividuals.push_back(rebucketer);
            removeIndividual = true;
          }
        }
      }
    }

    if (removeIndividual)
    {
#ifndef SERIAL
#     pragma omp critical
#endif
      {
        myIndividualsToRemove.push_back(ii);
      }
    }
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

//***********************************
// Bucket::postFrameUpdateAdjustments
//***********************************
void Bucket::postFrameUpdateAdjustments()
{
  rebucketIndividuals();
  adjustIndividualsList();
}

//****************************
// Bucket::rebucketIndividuals
//****************************
void Bucket::rebucketIndividuals()
{
  for (auto &rebucketer : myRebucketedIndividuals)
  {
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
