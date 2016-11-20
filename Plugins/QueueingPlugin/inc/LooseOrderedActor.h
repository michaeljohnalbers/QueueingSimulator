#pragma once

/**
 * @file LooseOrderedActor.h
 * @brief Actor which also has an ordering attribute.
 *
 * @author Michael Albers
 */

#include "OrderedActor.h"

namespace QS
{
  /**
   * Defines an Actor that orders loosely. That is not strictly behind the next
   * highest ranked Actor, but the closest ranked one in a vicinity.
   *
   * Mostly the purpose of this class is to allow for a different Behavior Set
   * than other OrderedActors.
   */
  class LooseOrderedActor: public OrderedActor
  {
    public:

    /**
     * Default constructor.
     */
    LooseOrderedActor() = delete;

    /**
     * Constructor. Initializes the the LooseOrderedActor from the properties
     * map.
     *
     * @param theProperties
     *          Map of property name (key) to property value (value).
     *
     *          See OrderedActor.h
     * @param theTag
     *          optional user-defined tag for differentiating LooseOrderedActors
     * @throws std::invalid_argument
     *           if map is missing any required property listed above, or if
     *           any property has an invalid value
     */
    LooseOrderedActor(const Properties &theProperties,
                      const std::string &theTag);

    /**
     * Destructor.
     */
    virtual ~LooseOrderedActor() = default;

    protected:

    private:
  };
}
