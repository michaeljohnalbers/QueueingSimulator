/**
 * @file NearestN.cpp
 * @brief Implementation of NearestN class.
 *
 * @author Michael Albers
 */

#include <omp.h>

#include <Bucket.h>
#include <EigenHelper.h>
#include <Individual.h>
#include <NearestN.h>

//*****************
// Static Variables
//*****************
uint32_t NearestN::myN = 1;
float NearestN::mySearchRadius = 1.0;

//*******************
// NearestN::NearestN
//*******************
NearestN::NearestN()
{
}

//********************
// NearestN::~NearestN
//********************
NearestN::~NearestN()
{
}

//************************
// NearestN::findNeighbors
//************************
std::shared_ptr<NearestN> NearestN::findNeighbors(
  const Individual* theIndividual,
  const Bucket *theBucket)
{
  std::shared_ptr<NearestN> neighbors(new NearestN());

  neighbors->searchBucket(*theBucket, theIndividual);

  // TODO: Future enhancement: only search buckets which overlap search radius
  // TODO: Future future enhancement: Have Bucket keep track of Individuals in
  //       overlap area
  for (auto bucket : theBucket->getAdjacentBuckets())
  {
    neighbors->searchBucket(*bucket, theIndividual);
  }

  neighbors->sortAndFindClosestRank(theIndividual);

  return neighbors;
}

//***********************************
// NearestN::getClosestRankedNeighbor
//***********************************
const Individual* NearestN::getClosestRankedNeighbor() const
{
  return myClosestRankedIndividual;
}

//***********************
// NearestN::getNeighbors
//***********************
const std::list<const Individual*>& NearestN::getNeighbors() const
{
  return myNeighbors;
}

//***********************
// NearestN::searchBucket
//***********************
void NearestN::searchBucket(const Bucket &theBucket,
                            const Individual *theIndividual)
{
  const Eigen::Vector2f &origin = theIndividual->getPosition();
  auto bucketIndividuals = theBucket.getIndividuals();
  auto numberIndividuals = bucketIndividuals.size();

  // This will start off small and increase as the simulation moves along and
  // more Individuals move into the Bucket containing the exit.
#ifndef SERIAL
# pragma omp parallel for shared(bucketIndividuals)
#endif
  for (uint32_t ii = 0; ii < numberIndividuals; ++ii)
  {
    Individual *individual = bucketIndividuals[ii];
    if (individual != theIndividual)
    {
      const Eigen::Vector2f &position = individual->getPosition();
      float distance = EigenHelper::distance(origin, position);
      if (distance <= mySearchRadius)
      {
#ifndef SERIAL
#       pragma omp critical
#endif
        {
          myNeighbors.push_back(individual);
        }
      }
    }
  }
}

//***************
// NearestN::setN
//***************
void NearestN::setN(uint32_t theN)
{
  myN = theN;
}

//**************************
// NearestN::setSearchRadius
//**************************
void NearestN::setSearchRadius(float theSearchRadius)
{
  mySearchRadius = theSearchRadius;
}

//*********************************
// NearestN::sortAndFindClosestRank
//*********************************
void NearestN::sortAndFindClosestRank(const Individual *theIndividual)
{
  myNeighbors.sort(
    [& theIndividual] (const Individual *a,
                       const Individual *b)
    {
      auto origin = theIndividual->getPosition();
      float aDistance = EigenHelper::distance(a->getPosition(), origin);
      float bDistance = EigenHelper::distance(b->getPosition(), origin);
      return aDistance < bDistance;
    });

  if (myNeighbors.size() > myN)
  {
    myNeighbors.resize(myN);
  }

  /* This will find the Individual with the closest numerically, not proximity,
   * higher (remember higher rank means lower number) rank. This is Individual
   * which is closest to this Individual, but still ahead in the queue.
   */
  int32_t lowestRankDifference = INT32_MAX;
  const auto individualRank = theIndividual->getRank();

  for (auto individual : myNeighbors)
  {
    auto rank = individual->getRank();
    int32_t rankDifference = individualRank - rank;
    if (rank < individualRank && rankDifference < lowestRankDifference)
    {
      lowestRankDifference = rankDifference;
      myClosestRankedIndividual = individual;
    }
  }
}
