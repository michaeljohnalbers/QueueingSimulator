#ifndef _NearestN_h_
#define _NearestN_h_

/**
 * @file NearestN.h
 * @brief Finds and stores the nearest N Individuals from a given point.
 *
 * @author Michael Albers
 */

#include <memory>

class Bucket;
class Individual;

/**
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
   * @param theIndivdual Indidivual for which to find neighbors.
   * @param theBucket Bucket containing theIndivdual.
   * @return Pointer to object containing the neighbors.
   */
  static std::shared_ptr<NearestN> findNeighbors(
    const Individual* theIndivdual,
    const Bucket *theBucket);

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
   * Default constructor.
   */
  NearestN();

  // ************************************************************
  // Private
  // ************************************************************
  private:

  /** Search radius, in meters. */
  static float mySearchRadius;

  /** Maximum number of neighbors to find. */
  static int32_t myN;
};

#endif

