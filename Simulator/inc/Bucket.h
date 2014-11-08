#ifndef _Bucket_h_
#define _Bucket_h_

/**
 * @file Bucket.h
 * @brief Class which encapsulates a portion of the entire simulation world
 * and the Individuals contained therein.
 *
 * The total simulation world is broken down into Buckets for the purpose of
 * spatial hashing (see "Big Fast Crowds on the PS3" by Craig Reynolds). Each
 * Bucket contains a subset of all the Individuals in the simulation.
 *
 * @author Michael Albers
 */

#include <array>
#include <cstdint>
#include <vector>

#include <ReBucketer.h>

/**
 * TODO: fill in
 */
class Bucket
{
  // ************************************************************
  // Public
  // ************************************************************
  public:

  /**
   * Default constructor.
   */
  Bucket();

  /**
   * Copy constructor.
   * @param Object to copy.
   */
  Bucket(const Bucket&) = delete;

  /**
   * Destructor.
   */
  ~Bucket();

  /**
   * Assignment operator
   * @param Object to copy.
   * @return This object.
   */
  Bucket& operator=(const Bucket&) = delete;

  /**
   * Informs this Bucket that the given Bucket is adjacent to it.
   * @param theBucket Adjacent Bucket.
   */
  void addAdjacentBucket(Bucket *theBucket);

  /**
   * Once per frame update function. This will update all the Individuals
   * in this Bucket.
   * @param theFrameTime The duration of the frame, in seconds.
   */
  void frameUpdate(float theFrameTime);

  /**
   * Means to place an Individual into this Bucket.
   * @param theIndividual Individual to add to this Bucket.
   */
  void receiveIndividual(Individual *theIndividual);

  // ************************************************************
  // Protected
  // ************************************************************
  protected:

  // ************************************************************
  // Private
  // ************************************************************
  private:

  /**
   * All adjacent Buckets. Stored in no particular order. In a 2-D world,
   * there will always be at most 8 adjacent Buckets:
   * X X X
   * X B X
   * X X X
   */
  std::array<Bucket*, 8> myAdjacentBuckets {
    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr} };

  /** All Individual in this Bucket. */
  Individual* myIndividuals[];

  /**
   * All Individuals from this Bucket which have moved into a new Bucket after
   * a frame update.
   */
  std::vector<ReBucketer> myRebucketedIndividuals;
};

#endif
