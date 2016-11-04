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
  class ExitDefinition;
  class SensorDefinition;
  class PluginDefinition;

  /**
   * This class reads a plugin configuration file and returns a populated
   * plugin definition object.
   */
  class PluginReader : public DefaultHandler
  {
    public:

    /** Name of the plugin schema. */
    static constexpr auto SCHEMA_FILE = "PluginConfig.xsd";

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
     * @param thePluginSchemaDirectory
     *          directory in which to find the plugin schema
     */
    PluginReader(const std::string &thePluginDirectory,
                 const std::string &theConfigFile,
                 const std::string &thePluginSchemaDirectory);

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
    virtual void endElement(const XMLCh *const uri,
                            const XMLCh *const localname,
                            const XMLCh *const qname) override;
    // Error callbacks
    void error(const SAXParseException& e) override;
    void fatalError(const SAXParseException& e) override;
    void warning(const SAXParseException& e) override;

    protected:

    private:

    /**
     * Returns the 'source' attribute, and if it doesn't exist, returns the
     * current plugin name. This is used for reading the source of a particular
     * type.
     */
    std::string getPluginSource(const Attributes &theAttributes) const noexcept;

    /**
     * Returns the tag attribute, or empty string if the attribute doesn't
     * exist. This is just a convenience functions to avoid a lot of
     * try/catches.
     *
     * @return tag attribute or empty string if no attribute
     */
    std::string getTag(const Attributes &theAttributes) const noexcept;
    
    /** Current actor definition */
    std::shared_ptr<ActorDefinition> myActorDefinition;

    /** Current behavior set definition */
    std::shared_ptr<BehaviorSetDefinition> myBehaviorSetDefinition;

    /** Current behavior definition */
    std::shared_ptr<BehaviorDefinition> myBehaviorDefinition;

    /** Plugin config file */
    const std::string myConfigFile;

    /** Current Exit definition */
    std::shared_ptr<ExitDefinition> myExitDefinition;

    /** Plugin definition to populate while reading. */
    std::shared_ptr<PluginDefinition> myPluginDefinition;

    /** Plugin directory */
    const std::string myPluginDirectory;

    /** Directory in which the schema is located. */
    const std::string myPluginSchemaDirectory;
    
    /** Current sensor definition */
    std::shared_ptr<SensorDefinition> mySensorDefinition;

    /** Buffer for handling 'characters' callback. */
    std::string myStringBuffer;
  };
}
