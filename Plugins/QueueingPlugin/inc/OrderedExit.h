#pragma once

/**
 * @file OrderedExit.h
 * @brief Specialized Exit that only allows Actors to exit in a specific order
 * based off a numeric rank.
 *
 * @author Michael Albers
 */

#include "Exit.h"

namespace QS
{
  class Actor;

  /**
   * An exit that only allows Actors to exit if they exit in an order based off
   * a rank number. This rank is the same as a TFF queue number.
   */
  class OrderedExit: public Exit
  {
    public:

    /**
     * Default constructor.
     */
    OrderedExit() = delete;

    /**
     * Constructor. See Exit.h for required &amp; optional properties.
     *
     * @throws std::invalid_argument
     *           See Exit.h.
     */
    OrderedExit(const Properties &theProperties, const std::string &theTag);

    /**
     * Copy constructor.
     */
    OrderedExit(const OrderedExit&) = default;

    /**
     * Move constructor.
     */
    OrderedExit(OrderedExit &&) = default;

    /**
     * Destructor
     */
    virtual ~OrderedExit() = default;

    /**
     * Checks if the Actor can exit through this one. Only the Actor with a
     * rank that matches the rank internal to this object may exit.
     *
     * @param theActor
     *          Actor to check
     * @return true if the Actor can exit, false otherwise
     */
    virtual bool canActorExit(const Actor *theActor) noexcept override;

    /**
     * Copy assignment operator.
     */
    OrderedExit& operator=(const OrderedExit&) = default;

    /**
     * Move assignment operator.
     */
    OrderedExit& operator=(OrderedExit &&) = default;

    protected:

    private:

    /** Rank of Actor that is allowed to exit. */
    uint32_t myRank = 0;
  };
}
