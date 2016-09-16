#pragma once

/**
 * @file EntityDependency.h
 * @brief Simple class to encapsulate an entity (Behavior, BehaviorSet, Sensor),
   its type name and a user-defined tag.
 *
 * @author Michael Albers
 */

#include <string>

namespace QS
{
  /**
   * This class encapsulates a single dependency for a plugin entity. The
   * plugin entities each use a set of other entities (i.e., Actors use
   * BehaviorSets). This class defines a single on of these uses (dependencies).
   */
  template<class Entity>
  class EntityDependency
  {
    public:

    /** Name of the entity's data type (i.e., CustomActor) */
    std::string myTypeName;

    /** None ownership pointer to the entity. */
    Entity *myEntity;

    /**
     * Optional user-defined tag. This is used to differentitate multiple
     * entities of the same type. For instance a BehaviorSet might contain two
     * instances of BehaviorX, this tag is used to differentitate them.
     */
    std::string myTag;
  };
}
