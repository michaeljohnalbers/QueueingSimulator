/**
 * @file PluginHelper.cpp
 * @brief Definition of PluginHelper
 *
 * @author Michael Albers
 */

#include "PluginHelper.h"

const std::function<float(const std::string&)> QS::PluginHelper::toFloat =
  [](const std::string &value)
{
  return std::stof(value);
};
