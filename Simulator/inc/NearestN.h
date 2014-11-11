#ifndef _NearestN_h_
#define _NearestN_h_

/**
 * @file NearestN.h
 * @brief Finds and stores the nearest N Individuals from a given point.
 *
 * @author Michael Albers
 */

#include <memory>
#include <list>

#include <Eigen/Core>

class Bucket;
class Individual;

/**
 * From the position of a given Individual this class will find the closest
 * N neighboring Individuals.
 */
class NearestN
{
  // ************************************************************
  // Public
  // ************************************************************
  public:

  /**
   * Destructor.
   */
  ~NearestN();

  /**
   * Copy constructor.
   * @param Object to copy.
   */
  NearestN(const NearestN&) = delete;

  /**
   * Assignment operator.
   * @param Object to copy.
   * @return This object.
   */
  NearestN& operator=(const NearestN&) = delete;

  /**
   * Finds the nearest N neighbors of the given Individual and returns an
   * instance of this class storing those neighbors.
   * @param theIndividual Indidivual for which to find neighbors.
   * @param theBucket Bucket containing theIndivdual.
   * @return Pointer to object containing the neighbors.
   */
  static std::shared_ptr<NearestN> findNeighbors(
    const Individual* theIndividual,
    const Bucket *theBucket);

  /**
   * Returns all of the found neighbors.
   * @return Nearst N neighbors.
   */
  const std::list<const Individual*>& getNeighbors() const;

  /**
   * Sets the maximum number of neighbors to find.
   * @param theN Maximu number of neighbors to find.
   */
  static void setN(int32_t theN);

  /**
   * Sets the search radius.
   * @param theSearchRadius Search radius, in meters.
   */
  static void setSearchRadius(float theSearchRadius);

  // ************************************************************
  // Protected
  // ************************************************************
  protected:

  /**
   * Finds all individuals from the given Bucket that fall within
   * the search radius.
   * @param theBucket Bucket to search
   * @param theIndivdual Individual at center of search circle
   * @param theLowestRank Currently seen lowest (numerically) ranked Individual
   */
  void searchBucket(const Bucket &theBucket,
                    const Individual *theIndividual,
                    int32_t &theLowestRank);

  /**
   * Default constructor.
   */
  NearestN();

  // ************************************************************
  // Private
  // ************************************************************
  private:

  /** (Up to) N neighbors of an Indivdual. */
  std::list<const Individual*> myNeighbors;

  /** Lowest (numerically) ranked individual. */
  const Individual *myLowestRankedIndividual = nullptr;

  /** Maximum number of neighbors to find. */
  static int32_t myN;

  /** Search radius, in meters. */
  static float mySearchRadius;
};

#endif
