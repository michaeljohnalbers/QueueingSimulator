/**
 * @file PluginCollection.cpp
 * @brief Definition of PluginCollection class
 *
 * @author Michael Albers
 */

#include <cerrno>
#include <cstring>
#include <dirent.h>
#include <stdexcept>
#include "Finally.h"
#include "Plugin.h"
#include "PluginCollection.h"
#include "PluginDefinition.h"
#include "PluginReader.h"

QS::PluginCollection::PluginCollection(
  const std::string &thePluginBaseDirectory)
{
  DIR *directory = ::opendir(thePluginBaseDirectory.c_str());
  if (NULL == directory)
  {
    auto thisErrno = errno;
    std::string error{
      "Failed to open plugin configuration file base directory '"};
    error += thePluginBaseDirectory + "': " + std::strerror(thisErrno) + ".";
    throw std::runtime_error(error);
  }

  Finally directoryClean([=]() {::closedir(directory);});

  // Directory (expected) layout:
  //  thePluginBaseDirectory
  //   |-Plugin1
  //   |    |- Plugin1.xml
  //   |    |- libPlugin1.so
  //   |-OtherPlugin
  //   |    |- OtherPlugin.xml
  //   |    |- libOtherPlugin.so

  dirent *entry;
  errno = 0;
  while ((entry = ::readdir(directory)) != NULL)
  {
    std::string fileName{entry->d_name};
    if (entry->d_type == DT_DIR && "." != fileName && ".." != fileName)
    {
      std::string pluginDirectory{thePluginBaseDirectory};
      pluginDirectory += "/" + fileName;

      auto configFile = checkPluginDirectory(pluginDirectory);
      readPlugin(pluginDirectory, configFile, thePluginBaseDirectory);
    }
  }

  if (errno)
  {
    auto thisErrno = errno;
    std::string error{"Error while reading from plugin configuration "
        "file base directory '"};
    error += thePluginBaseDirectory + "': " + std::strerror(thisErrno) + ".";
    throw std::runtime_error(error);
  }
}

std::string QS::PluginCollection::checkPluginDirectory(
  const std::string &thePluginDirectory)
{
  std::string configFile;

  DIR *pluginDirectory = ::opendir(thePluginDirectory.c_str());
  if (NULL != pluginDirectory)
  {
    Finally directoryClean([=]() {::closedir(pluginDirectory);});

    auto hasSuffix = [](const std::string &str, const std::string &suffix) ->
      bool
      {
        return str.size() >= suffix.size() &&
        str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
      };

    dirent *entry;
    errno = 0;
    while ((entry = ::readdir(pluginDirectory)) != NULL)
    {
      std::string fileName{entry->d_name};
      if (hasSuffix(fileName, ".xml"))
      {
        if (configFile.empty())
        {
          configFile = fileName;
        }
        else
        {
          std::string error("Too many XML files found in plugin configuration "
                            "file directory '");
          error += thePluginDirectory + "'. There can be only one.";
          throw std::runtime_error(error);
        }
      }
    }

    if (errno)
    {
      auto thisErrno = errno;
      std::string error{"Error while reading from plugin configuration "
          "file directory '"};
      error += thePluginDirectory + "': " + std::strerror(thisErrno) + ".";
      throw std::runtime_error(error);
    }
  }
  else
  {
    auto thisErrno = errno;
    std::string error{"Failed to open plugin directory '"};
    error += thePluginDirectory + "': " + std::strerror(thisErrno) + ".";
    throw std::runtime_error(error);
  }

  if (configFile.empty())
  {
    throw std::runtime_error("No plugin configuration file found in '" +
                             thePluginDirectory + "'.");
  }

  return configFile;
}

std::shared_ptr<QS::Plugin> QS::PluginCollection::getPlugin(
  const std::string &theName) const
{
  auto thisPluginIter = myPlugins.find(theName);
  if (thisPluginIter == myPlugins.end())
  {
    throw std::invalid_argument{"No plugin with name '" + theName + "' found."};
  }
  return thisPluginIter->second;
}

void QS::PluginCollection::readPlugin(
  const std::string &thePluginDirectory,
  const std::string &theConfigFile,
  const std::string &thePluginSchemaDirectory)
{
  PluginReader pluginReader(
    thePluginDirectory, theConfigFile, thePluginSchemaDirectory);
  auto pluginDefinition = pluginReader.read();
  std::shared_ptr<Plugin> plugin(new Plugin(pluginDefinition));
  myPlugins[pluginDefinition->getName()] = plugin;
}
