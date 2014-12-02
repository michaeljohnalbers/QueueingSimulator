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
   * @param theObjectToCopy Object to copy.
   */
  NearestN(const NearestN &theObjectToCopy) = delete;

  /**
   * Assignment operator.
   * @param theRHS Object to copy.
   * @return This object.
   */
  NearestN& operator=(const NearestN &theRHS) = delete;

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
   * Returns the Individual with the closest numerically higher rank. If no
   * such Individual exists in the group of neighbors nullptr is returned.
   * @return Individual with closest numerically higher rank or nullptr.
   */
  const Individual* getClosestRankedNeighbor() const;

  /**
   * Returns all of the found neighbors.
   * @return Nearst N neighbors.
   */
  const std::list<const Individual*>& getNeighbors() const;

  /**
   * Sets the maximum number of neighbors to find.
   * @param theN Maximu number of neighbors to find.
   */
  static void setN(uint32_t theN);

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
   * Default constructor.
   */
  NearestN();

  /**
   * Finds all individuals from the given Bucket that fall within
   * the search radius.
   * @param theBucket Bucket to search
   * @param theIndividual Individual at center of search circle
   */
  void searchBucket(const Bucket &theBucket,
                    const Individual *theIndividual);

  /**
   * Sorts the list, truncates it to N then find the Individual with the
   * closest rank (above the individual around which the search in centered.)
   * @param theIndividual Individual at center of search circle
   */ 
  void sortAndFindClosestRank(const Individual *theIndividual);

  // ************************************************************
  // Private
  // ************************************************************
  private:

  /** (Up to) N neighbors of an Indivdual. */
  std::list<const Individual*> myNeighbors;

  /** Individual with the closest numerically higher rank. */
  const Individual *myClosestRankedIndividual = nullptr;

  /** Maximum number of neighbors to find. */
  static uint32_t myN;

  /** Search radius, in meters. */
  static float mySearchRadius;
};

#endif
