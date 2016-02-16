#ifndef _Bucket_h_
#define _Bucket_h_

/**
 * @file Bucket.h
 * @brief Class which encapsulates a portion of the entire simulation world
 * and the Individuals contained therein.
 *
  * @author Michael Albers
 */

#include <array>
#include <cstdint>
#include <vector>

#include <Eigen/Core>

#include <ReBucketer.h>

/**
 * The total simulation world is broken down into Buckets for the purpose of
 * spatial hashing (see "Big Fast Crowds on the PS3" by Craig Reynolds). Each
 * Bucket contains a subset of all the Individuals in the simulation.
 *
 * Each Bucket is in charge of updating each Individual within the Bucket for
 * each frame.
 *
 * This class is not designed to be used by different threads simultaneously.
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
  Bucket() = delete;

  /**
   * Constructor.
   * @param theOrigin Origin point of the Bucket.
   * @param theHeight Bucket height.
   * @param theLength Bucket length.
   * @param theMaximumNumberIndividuals Maximum number of Individuals in this
   *        Bucket.
   */
  Bucket(const Eigen::Vector2f &theOrigin,
         int32_t theHeight,
         int32_t theLength,
         int32_t theMaximumNumberIndividuals);

  /**
   * Copy constructor.
   * @param theObjectToCopy Object to copy.
   */
  Bucket(const Bucket &theObjectToCopy) = delete;

  /**
   * Destructor.
   */
  ~Bucket();

  /**
   * Assignment operator
   * @param theRHS Object to copy.
   * @return This object.
   */
  Bucket& operator=(const Bucket &theRHS) = delete;

  /**
   * Informs this Bucket that the given Bucket is adjacent to it. Not
   * thread-safe.
   * @param theBucket Adjacent Bucket.
   * @param theRowOffset Row offset from this Bucket (-1 to 1)
   * @param theColumnOffset Column offset from this Bucket (-1 to 1)
   */
  void addAdjacentBucket(Bucket *theBucket,
                         int32_t theRowOffset,
                         int32_t theColumnOffset);

  /**
   * Determines if this Bucket contains the given point.
   * @param thePosition Point to check.
   * @return True if the given point is within the Bucket, false otherwise.
   */
  bool containsPoint(const Eigen::Vector2f &thePosition) const;

  /**
   * Once per frame update function. This will update all the Individuals
   * in this Bucket.
   * @param theFrameTime The duration of the frame, in seconds.
   */
  void frameUpdate(float theFrameTime);

  /**
   * Returns a list of all Buckets adjacent to this one.
   * @return All adjacent Buckets, in no particular order.
   */
  const std::vector<Bucket*>& getAdjacentBuckets() const;

  /**
   * Returns a list of the individuals in this Bucket.
   * @return List of the individuals in this Bucket.
   */
  const std::vector<Individual*>& getIndividuals() const;

  /**
   * Performs clean up/adjustment actions after each frame. This should be
   * called by the owner of this object after calling the frame update
   * function.
   *
   * Note: This function is not thread-safe.
   */
  void postFrameUpdateAdjustments();

  /**
   * Means to place an Individual into this Bucket.
   * @param theIndividual Individual to add to this Bucket.
   */
  void receiveIndividual(Individual *theIndividual);

  // ************************************************************
  // Protected
  // ************************************************************
  protected:

  /**
   * Removes any Individuals from this Bucket which are no longer in this
   * Bucket.
   */
  void adjustIndividualsList();

  /**
   * Takes all individual which have moved out of this Bucket and gives them
   * to their new Bucket. Not thread-safe.
   */
  void rebucketIndividuals();

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
  std::array<std::array<Bucket*, 3>, 3> myAdjacentBuckets;

  /** Different way of storing adjacent Buckets. */
  std::vector<Bucket*> myAdjacentBucketList;

  /** Height of the Bucket. */
  int32_t myHeight = 0;

  /** All Individual in this Bucket. */
  std::vector<Individual*> myIndividuals;

  /**
   * List of indeces into myIndividuals. Each entry will be removed after a
   * frame update.
   */
  std::vector<int32_t> myIndividualsToRemove;

  /** Length of the Bucket. */
  int32_t myLength = 0;

  /** Origin point of the Bucket. */
  Eigen::Vector2f myOrigin;

  /**
   * All Individuals from this Bucket which have moved into a new Bucket after
   * a frame update.
   */
  std::vector<ReBucketer> myRebucketedIndividuals;
};

#endif
