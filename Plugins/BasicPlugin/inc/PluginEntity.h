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
     * @param theTag
     *          optional user-defined tag for differentiating Actors of the
     *          same type
     */
    PluginEntity(const Properties &theProperties, const std::string &theTag);

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
     * Returns the Entity's tag. The tag is an optional user-defined string that
     * can be used to differentiate different entities of the same type.
     *
     * @return tag
     */
    std::string getTag() const noexcept;

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

    /**
     * Optional user-defined string for differentiating multiple Actors of the
     * same type.
     */
    std::string myTag;

    private:
  };
}
