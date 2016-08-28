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
#include "PluginCollection.h"
#include "PluginReader.h"

#include <iostream> // TODO: remove

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
    if (entry->d_type == DT_DIR)
    {
      std::string pluginDirectory{thePluginBaseDirectory};
      pluginDirectory += "/" + fileName;

      auto configFile = checkPluginDirectory(pluginDirectory);
      if (false == configFile.empty())
      {
        readPlugin(pluginDirectory, configFile);
      }
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
    auto hasSuffix = [](const std::string &str, const std::string &suffix) ->
      bool
      {
        return str.size() >= suffix.size() &&
        str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
      };

    bool foundProblem = false;

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
          // Too many XML files, ignore this invalid plugin directory.
          foundProblem = true;
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
    else if (foundProblem)
    {
      // Ignore this invalid plugin directory.
      configFile = "";
    }
  }
  else
  {
    // Fail silently on this seemingly bad directory.
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

void QS::PluginCollection::readPlugin(const std::string &thePluginDirectory,
                                      const std::string &theConfigFile)
{
  try
  {
    PluginReader pluginReader(thePluginDirectory, theConfigFile);
    auto pluginDefinition = pluginReader.read();
  }
  catch (const std::logic_error &exception)
  {
    std::cout << exception.what() << std::endl;
  }
  catch (...)
  {
    std::cout << "Other exception" << std::endl;
    throw;
  }
}
