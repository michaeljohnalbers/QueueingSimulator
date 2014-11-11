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
int32_t NearestN::myN = 1;
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

  int32_t lowestRank = INT32_MAX;

  neighbors->searchBucket(*theBucket, theIndividual, lowestRank);

  // TODO: Future enhancement: only search buckets which overlap search radius
  // TODO: Future future enhancement: Have Bucket keep track of Individuals in
  //       overlap area
  for (auto bucket : theBucket->getAdjacentBuckets())
  {
    neighbors->searchBucket(*bucket, theIndividual, lowestRank);
  }

  neighbors->myNeighbors.sort(
    [& theIndividual] (const Individual *a,
                       const Individual *b)
    {
      auto origin = theIndividual->getPosition();
      float aDistance = EigenHelper::distance(a->getPosition(), origin);
      float bDistance = EigenHelper::distance(b->getPosition(), origin);
      return aDistance < bDistance;
    });

  neighbors->myNeighbors.resize(myN);

  return neighbors;
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
                            const Individual *theIndividual,
                            int32_t &theLowestRank)
{
  const Eigen::Vector2f &origin = theIndividual->getPosition();
  auto bucketIndividuals = theBucket.getIndividuals();
  auto bucketSize = bucketIndividuals.size();

# pragma omp parallel for shared(theLowestRank, bucketIndividuals)
  for (uint32_t ii = 0; ii < bucketSize; ++ii)
  {
    Individual *individual = bucketIndividuals[ii];
    if (individual != theIndividual)
    {
      const Eigen::Vector2f &position = individual->getPosition();
      float distance = EigenHelper::distance(origin, position);
      if (distance <= mySearchRadius)
      {
        auto rank = individual->getRank();

#       pragma omp critical
        {
          myNeighbors.push_back(individual);
          if (rank < theLowestRank)
          {
            myLowestRankedIndividual = individual;
            theLowestRank = rank;
          }
        }
      }
    }
  }
}

//***************
// NearestN::setN
//***************
void NearestN::setN(int32_t theN)
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
