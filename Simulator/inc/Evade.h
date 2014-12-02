#ifndef _Evade_h_
#define _Evade_h_

/**
 * @file Evade.h
 * @brief Defines the Evade steering behavior.
 *
 * @author Michael Albers
 */

#include <Eigen/Core>

class Individual;

/**
 * This class defines the 'Evade' steering behavior. This behavior is
 * a predictive version of 'Flee'. It makes an educated guess as to
 * where an Individual is going to be and moves the away from that
 * spot.
 * Based off of description at:
 * http://gamedevelopment.tutsplus.com/tutorials/understanding-steering-behaviors-pursuit-and-evade--gamedev-2946
 */
class Evade
{
  // ************************************************************
  // Public
  // ************************************************************
  public:

  /**
   * Default constructor.
   */
  Evade() = delete;

  /**
   * Destructor.
   */
  ~Evade() = delete;

  /**
   * Drives an Individual away from another Individual.
   * @param theIndividual Individual being steered.
   * @param theIndividualEvaded Individual being evaded.
   * @return Force vector
   */
  static Eigen::Vector2f calculateForce(const Individual &theIndividual,
                                        const Individual &theIndividualEvaded);

  // ************************************************************
  // Protected
  // ************************************************************
  protected:

  // ************************************************************
  // Private
  // ************************************************************
  private:

};

#endif
