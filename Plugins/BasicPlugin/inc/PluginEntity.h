#pragma once

/**
 * @file PluginEntity.h
 * @brief Parent class of plugin entities (Actors, Behaviors, etc.)
 *
 * @author Michael Albers
 */

#include <map>
#include <string>

namespace QS
{
  class PluginEntity
  {
    public:

    /** Properties type. */
    using Properties = std::map<std::string, std::string>;

    /**
     * Default constructor.
     */
    PluginEntity() = delete;

    /**
     * Constructor.
     *
     * @param theProperties
     *          properties of this entity
     */
    PluginEntity(const Properties &theProperties);

    /**
     * Copy constructor.
     */
    PluginEntity(const PluginEntity&) = default;

    /**
     * Move constructor.
     */
    PluginEntity(PluginEntity&&) = default;

    /**
     * Destructor.
     */
    virtual ~PluginEntity() = default;

    /**
     * Returns the entity's properties.
     *
     * @return the entity's properties.
     */
    Properties getProperties() const noexcept;

    /**
     * Copy assignment operator
     */
    PluginEntity& operator=(const PluginEntity&) = default;

    /**
     * Move assignment operator
     */
    PluginEntity& operator=(PluginEntity&&) = default;

    protected:

    /** Properties. */
    Properties myProperties;

    private:
  };
}
