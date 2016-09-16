#pragma once

/**
 * @file DependencyManager.h
 * @brief Maintains entity dependencies.
 *
 * @author Michael Albers
 */

#include <stdexcept>
#include <vector>

#include "EntityDependency.h"

namespace QS
{
  /**
   * This class holds a list of EntityDependency objects.
   */
  template<class T>
  class DependencyManager
  {
    public:
    /**
     * Default constructor.
     */
    DependencyManager() = default;

    /**
     * Copy constructor.
     */
    DependencyManager(const DependencyManager&) = default;

    /**
     * Move constructor.
     */
    DependencyManager(DependencyManager&&) = default;

    /**
     * Destructor.
     */
    virtual ~DependencyManager() = default;

    /**
     * Returns all dependencies
     *
     * @return all dependencies
     */
    virtual std::vector<EntityDependency<T>> getDependencies() const noexcept
    {
      return myDependencies;
    }

    /**
     * Copy assignment operator
     */
    DependencyManager& operator=(const DependencyManager&) = default;

    /**
     * Move assignment operator
     */
    DependencyManager& operator=(DependencyManager&&) = default;

    /**
     * Sets the dependencies.
     *
     * @param theDependencies
     *          new list of dependencies
     * @throws std::invalid_argument if any dependency is invalid.
     */
    void setDependencies(
      const std::vector<EntityDependency<T>> &theDependencies)
    {
      if (theDependencies.size() == 0)
      {
        throw std::invalid_argument("Cannot have empty set of dependencies.");
      }

      for (auto dependency : theDependencies)
      {
        // For the time being, having an empty name or tag is OK. The name is
        // really just a key for the author and if they don't want it, I
        // guess that's their business.

        if (nullptr == dependency.myEntity)
        {
          throw std::invalid_argument(
            "Cannot have null entity (for type name '" + dependency.myTypeName +
            "').");
        }
      }

      myDependencies = theDependencies;
    }

    protected:

    /** Dependencies to track. */
    std::vector<EntityDependency<T>> myDependencies;

    private:
  };
}
