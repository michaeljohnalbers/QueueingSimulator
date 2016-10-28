#pragma once

/**
 * @file SimulationEntityConfiguration.h
 * @brief Contains the configuration of a simulation entity (Actor, BehaviorSet,
 * etc.).
 *
 * @author Michael Albers
 */

#include <string>
#include <vector>
#include "PluginEntity.h"

namespace QS
{
  /**
   * This class stores all configuration for a single simulation entity (Actor,
   * BehaviorSet, etc.). This configuration includes recursive entity
   * configurations (i.e, configuration of a BehaviorSet is part of the
   * configuration of the Actor using it.).
   *
   * Configuration information given to the entity on creation. This information
   * is user-driven via the simulation configuration file. The purpose of this
   * data is to tweak (well, yes, configure) the entity to the needs ot the
   * simulation.
   *
   * The primary configuration data specific to the entity (not including
   * dependency configuration data) is simply a set of key/value properties.
   */
  class SimulationEntityConfiguration
  {
    public:

    /**
     * Default constructor
     */
    SimulationEntityConfiguration() = delete;

    /**
     * Constructor.
     *
     * @param theType
     *          entity type
     * @param theTag
     *          optional tag
     * @param theSource
     *          plugin name where entitiy originates
     */
    SimulationEntityConfiguration(const std::string &theType,
                                  const std::string &theTag,
                                  const std::string &theSource);

    /**
     * Copy constructor
     */
    SimulationEntityConfiguration(const SimulationEntityConfiguration&) =
      default;

    /**
     * Move constructor
     */
    SimulationEntityConfiguration(SimulationEntityConfiguration&&) = default;

    /**
     * Destructor
     */
    ~SimulationEntityConfiguration() = default;

    /**
     * Adds the given dependency configuration to this entity. Duplicate
     * dependency configurations DO NOT overwrite the original.
     *
     * @param theEntityConfiguration
     *          dependency configuration of this entity
     */
    void addDependencyConfiguration(
      const SimulationEntityConfiguration &theEntityConfiguration) noexcept;

    /**
     * Add the given property to the property set. If the property already
     * exists it will be overwritten.
     *
     * @param theProperty
     *          property name
     * @param theValue
     *          property value
     */
    void addProperty(const std::string &theProperty,
                     const std::string &theValue) noexcept;

    /**
     * Returns the configurations of the dependencies of this Entity (i.e., if
     * this is an Actor this will return the BehaviorSet dependencies). This
     * may be an empty set if nothing was specified in the configuration file.
     *
     * @return dependencies
     */
    std::vector<SimulationEntityConfiguration> getDependencyConfigurations()
      const noexcept;

    /**
     * Returns the properties for this entity.
     *
     * @return properties for this entity
     */
    PluginEntity::Properties getProperties() const noexcept;

    /**
     * Returns the source of the entity.
     *
     * @return entity source
     */
    std::string getSource() const noexcept;

    /**
     * Returns the entity's tag
     *
     * @return entity tag
     */
    std::string getTag() const noexcept;

    /**
     * Returns the type of the entity.
     *
     * @return entity type
     */
    std::string getType() const noexcept;

    /**
     * Copy assignment operator
     */
    SimulationEntityConfiguration& operator=(
      const SimulationEntityConfiguration&) =
      default;

    /**
     * Move assignment operator
     */
    SimulationEntityConfiguration& operator=(SimulationEntityConfiguration&&) =
      default;

    protected:

    private:

    /**
     * Configurations of some or all dependencies of this entity (i.e.,
     * Actor depends on BehaviorSets
     */
    std::vector<SimulationEntityConfiguration> myDependencyConfigurations;

    /** User-defined properties for this entity. */
    PluginEntity::Properties myProperties;

    /** Plugin name from where the entity comes. */
    std::string mySource;

    /**
     * Entity tag. Since simulations may have many entities of the same type,
     * the tag allows for a means to differentitate them.
     */
    std::string myTag;

    /** Type of the entity */
    std::string myType;
  };
}
