#pragma once

/**
 * @file PluginHelper.h
 * @brief Contains helper functions to make writing plugin classes easier.
 *
 * @author Michael Albers
 */

#include <cstdint>
#include <functional>
#include <stdexcept>
#include <string>
#include "PluginEntity.h"

namespace QS
{
  /**
   * Contains helpful functions for writing plugin classes.
   */
  class PluginHelper
  {
    public:

    /**
     * Returns the property with the given name.
     *
     * @param theProperties
     *          properties from which to retrieve the specified property
     * @param thePropertyName
     *          property name
     * @param theRequired
     *          true if the property is required
     * @param theConversionFunction
     *          function to convert the string to the type T
     * @param theDefault
     *          default value if the property isn't given or the conversion
     *          fails on an invalid optional property
     * @return property value of defined type
     * @throws std::invalid_argument if theRequired is true and the property
     *         doesn't exist.
     * @throws &lt;unknown&gt; whatever the converter function might throw
     */
    template<class T>
    static T getProperty(
      const PluginEntity::Properties &theProperties,
      const std::string &thePropertyName,
      bool theRequired,
      std::function<T(const std::string&)> theConversionFunction,
      T theDefault = T())
    {
      T value = theDefault;
      auto propertyIter = theProperties.find(thePropertyName);
      if (theProperties.end() == propertyIter)
      {
        if (theRequired)
        {
          throw std::invalid_argument(
            "Missing '" + thePropertyName + "' property.");
        }
      }
      else
      {
        value = theConversionFunction(propertyIter->second);
      }
      return value;
    }

    /**
     * Converter function for use in getProperty. Converts the given value
     * to a float.
     *
     * @throws std::invalid_argument
     *           if value cannot be converted to a float
     */
    static const std::function<float(const std::string&)> toFloat;

    /**
     * Converter function for use in getProperty. Converts the given value
     * to an unsigned int.
     *
     * @throws std::invalid_argument
     *           if value cannot be converted to an unsigned int
     */
    static const std::function<uint64_t(const std::string&)> toUint;

    protected:

    private:
  };
}

