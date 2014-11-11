#ifndef _ReBucketer_h_
#define _ReBucketer_h_

/**
 * @file ReBucketer.h
 * @brief Stores a Bucket and the Individual to put into that Bucket.
 *
 * @author Michael Albers
 */

class Bucket;
class Individual;

/**
 * Stores an associate between an Individual and a Bucket. This class can
 * then place the Individual into the Bucket.
 */
class ReBucketer
{
  // ************************************************************
  // Public
  // ************************************************************
  public:

  /**
   * Default constructor.
   */
  ReBucketer();

  /**
   * Constructor.
   * @param theIndividual Individual to rebucket.
   * @param theBucket Bucket into which to place the individual.
   */
  ReBucketer(Individual *theIndividual,
             Bucket *theBucket);

  /**
   * Copy constructor.
   * @param theReBucketer Object to copy.
   */
  ReBucketer(const ReBucketer &theReBucketer);

  /**
   * Destructor.
   */
  ~ReBucketer();

  /**
   * Assignment operator.
   * @param theReBucketer Object to copy.
   * @return this object
   */
  ReBucketer& operator=(const ReBucketer &theReBucketer);

  /**
   * Returns the individual in this Rebucketer.
   * @return Individual to be rebucketed.
   */
  Individual* getIndividual() const;

  /**
   * Places the stored Individual into the stored Bucket.
   */
  void rebucket();

  /**
   * Sets the Individual and Bucket.
   * @param theIndividual Individual to rebucket.
   * @param theBucket Bucket into which to place the individual.
   */
  void set(Individual *theIndividual,
           Bucket *theBucket);

  // ************************************************************
  // Protected
  // ************************************************************
  protected:

  // ************************************************************
  // Private
  // ************************************************************
  private:

  /** Bucket into which to place the Individual. */
  Bucket *myBucket = nullptr;

  /** Individual to rebucket. */
  Individual *myIndividual = nullptr;
};

#endif
