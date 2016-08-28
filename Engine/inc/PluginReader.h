#pragma once

/**
 * @file PluginReader.h
 * @brief Reads in a single plugin configuration file
 *
 * @author Michael Albers
 */

#include <memory>
#include <string>
#include "xercesc/sax2/DefaultHandler.hpp"

XERCES_CPP_NAMESPACE_USE

namespace QS
{
  class ActorDefinition;
  class BehaviorDefinition;
  class BehaviorSetDefinition;
  class SensorDefinition;
  class PluginDefinition;

  /**
   * This class reads a plugin configuration file and returns a populated
   * plugin definition object.
   */
  class PluginReader : public DefaultHandler
  {
    public:

    /**
     * Default constructor.
     */
    PluginReader() = delete;

    /**
     * Constructor.
     *
     * @param thePluginDirectory
     *          directory where plugin files reside
     * @param theConfigFile
     *          plugin configuration file
     */
    PluginReader(const std::string &thePluginDirectory,
                 const std::string &theConfigFile);

    /**
     * Destructor.
     */
    ~PluginReader() = default;

    /**
     * Reads the plugin configuration file returning a populated definition
     * object.
     *
     * @return definition object for the plugin configuration file just read
     * @throws std::logic_error
     *           on any error reading/parsing the file.
     */
    std::shared_ptr<PluginDefinition> read();

    /*
     * SAX Callbacks
     */
    virtual void startDocument() override;
    virtual void startElement(const XMLCh *const uri,
                              const XMLCh *const localname,
                              const XMLCh *const qname,
                              const Attributes &attrs) override;
    virtual void characters(const XMLCh *const chars, const XMLSize_t length)
      override;
    // Error callbacks
    void error(const SAXParseException& e) override;
    void fatalError(const SAXParseException& e) override;
    void warning(const SAXParseException& e) override;

    protected:

    private:

    static constexpr auto SCHEMA_FILE = "PluginConfig.xsd";

    /** Current actor definition */
    std::shared_ptr<ActorDefinition> myActorDefinition;

    /** Current behavior set definition */
    std::shared_ptr<BehaviorSetDefinition> myBehaviorSetDefinition;

    /** Current behavior definition */
    std::shared_ptr<BehaviorDefinition> myBehaviorDefinition;

    /** Plugin config file */
    const std::string myConfigFile;

    /** Plugin definition to populate while reading. */
    std::shared_ptr<PluginDefinition> myPluginDefinition;

    /** Plugin directory */
    const std::string myPluginDirectory;

    /** Current sensor definition */
    std::shared_ptr<SensorDefinition> mySensorDefinition;

    /** Buffer for handling 'characters' callback. */
    std::string myStringBuffer;
  };
}
