#pragma once

/**
 * @file TestUtils.h
 * @brief Helper functions for writing gtests.
 *
 * @author Michael Albers
 */

#include "PluginEntity.h"

namespace QS
{
  class TestUtils
  {
    public:

    /**
     * Returns a full set of properties to instantiate an Actor object. These
     * properties will include all required an optional properties.
     *
     * @return properties for needed to create an Actor instance
     */
    static PluginEntity::Properties getFullActorProperties() noexcept;

    /**
     * Returns the minimal set of properties needed to instantiate an Actor
     * object.
     *
     * @return properties for needed to create an Actor instance
     */
    static PluginEntity::Properties getMinimalActorProperties() noexcept;

    /**
     * Returns a full set of properties to instantiate an Exit object. These
     * properties will include all required an optional properties.
     *
     * @return properties for needed to create an Exit instance
     */
    static PluginEntity::Properties getFullExitProperties() noexcept;

    /**
     * Returns the minimal set of properties needed to instantiate an Exit
     * object.
     *
     * @return properties for needed to create an Exit instance
     */
    static PluginEntity::Properties getMinimalExitProperties() noexcept;

    /**
     * Returns the set of properties needed to instantiate an Target object.
     *
     * @return properties for needed to create an Target instance
     */
    static PluginEntity::Properties getTargetProperties() noexcept;
  };
}
