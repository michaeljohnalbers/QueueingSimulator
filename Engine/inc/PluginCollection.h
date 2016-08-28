#pragma once

/**
 * @file PluginCollection.h
 * @brief Finds and stores all plugins.
 *
 * @author Michael Albers
 */

#include <map>
#include <memory>
#include <string>

namespace QS
{
  class Plugin;

  /**
   * The PluginCollection class provides a repository for all Plugins for the
   * simulator. It discovers and reads them in on construction then provides
   * accessors for retrieval.
   */
  class PluginCollection
  {
    public:

    /**
     * Default constructor.
     */
    PluginCollection() = delete;

    /**
     * Constructor.
     *
     * @param thePluginBaseDirectory
     *          directory where all plugin sub-directories can be found.
     */
    PluginCollection(const std::string &thePluginBaseDirectory);

    /**
     * Copy constructor.
     */
    PluginCollection(const PluginCollection&) = delete;

    /**
     * Move constructor.
     */
    PluginCollection(PluginCollection&&) = delete;

    /**
     * Destructor.
     */
    ~PluginCollection() = default;

    /**
     * Returns the plugin with the given name. If none is found, an exception
     * is thrown.
     *
     * @param theName
     *          name of the plugin to search for
     * @return plugin
     * @throws std::invalid_argument if there is no plugin with the given name
     */
    std::shared_ptr<Plugin> getPlugin(const std::string &theName) const;

    /**
     * Copy assignment operator.
     */
    PluginCollection& operator=(const PluginCollection&) = delete;

    /**
     * Move assignment operator.
     */
    PluginCollection& operator=(PluginCollection&&) = delete;

    protected:

    /**
     * Checks the given subdirectory for a .xml config file.
     *
     * @param thePluginDirectory
     *          directory which should contain plugin files
     * @return config file, or empty if none found
     * @throws std::runtime_error on error reading directory
     */
    std::string checkPluginDirectory(const std::string &thePluginDirectory);

    /**
     * Reads the plugin config file from the given directory.
     *
     * @param thePluginDirectory
     *          directory containing plugin files (including theConfigFile)
     * @param theConfigFile
     *          plugin config file
     */
    void readPlugin(const std::string &thePluginDirectory,
                    const std::string &theConfigFile);

    private:

    /** All plugins, keyed by name. */
    std::map<std::string, std::shared_ptr<Plugin>> myPlugins;
  };
}
