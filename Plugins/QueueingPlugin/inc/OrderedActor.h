#pragma once

/**
 * @file OrderedActor.h
 * @brief Actor which also has an ordering attribute.
 *
 * @author Michael Albers
 */

#include <cstdint>
#include "Actor.h"

namespace QS
{
  /**
   */
  class OrderedActor: public Actor
  {
    public:

    /**
     * Default constructor.
     */
    OrderedActor() = delete;

    /**
     * Constructor. Initializes the the OrderedActor from the properties map.
     *
     * @param theProperties
     *          Map of property name (key) to property value (value).
     *
     *          Required properties:
     *            See OrderedActor for baseline required properties.          
     *            "rank", unsigned integer rank
     * @param theTag
     *          optional user-defined tag for differentiating OrderedActors
     * @throws std::invalid_argument
     *           if map is missing any required property listed above, or if
     *           any property has an invalid value
     */
    OrderedActor(const Properties &theProperties, const std::string &theTag);

    /**
     * Destructor.
     */
    virtual ~OrderedActor() = default;

    /**
     * Returns the rank.
     *
     * @return rank
     */
    uint32_t getRank() const noexcept;

    protected:

    private:

    /** Rank used for ordering. */
    uint32_t myRank;
  };
}
