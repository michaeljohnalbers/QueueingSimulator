#pragma once

/**
 * @file SimulationReader.h
 * @brief Reads in a single simulation configuration file
 *
 * @author Michael Albers
 */

#include <map>
#include <string>
#include "xercesc/sax2/DefaultHandler.hpp"

XERCES_CPP_NAMESPACE_USE

namespace QS
{
  class World;

  /**
   */
  class SimulationReader : public DefaultHandler
  {
    public:

    /** Name of the plugin schema. */
    static constexpr auto SCHEMA_FILE = "SimulationConfig.xsd";

    /**
     * Default constructor.
     */
    SimulationReader() = delete;

    /**
     * Constructor.
     *
     * @param theConfigFile
     *          simulation configuration file
     * @param theSimulationSchemaDirectory
     *          directory in which to find the simulation schema
     * @param theWorld
     *          simulation world to populate from data from the config file
     */
    SimulationReader(const std::string &theConfigFile,
                     const std::string &theSimulationSchemaDirectory,
                     World &theWorld);

    /**
     * Copy constructor.
     */
    SimulationReader(const SimulationReader&) = delete;

    /**
     * Move constructor.
     */
    SimulationReader(SimulationReader&&) = delete;

    /**
     * Destructor.
     */
    virtual ~SimulationReader() = default;

    /**
     * Copy assignment operator
     */
    SimulationReader& operator=(const SimulationReader&) = delete;

    /**
     * Move assignment operator
     */
    SimulationReader& operator=(SimulationReader&&) = delete;

    /**
     * Reads the simulation configuration file returning a populated definition
     * object.
     *
     * @return definition object for the simulation configuration file just read
     * @throws std::logic_error
     *           on any error reading/parsing the file.
     */
    void read();

    /*
     * SAX Callbacks
     */
    virtual void startElement(const XMLCh *const uri,
                              const XMLCh *const localname,
                              const XMLCh *const qname,
                              const Attributes &attrs) override;
    virtual void endElement(const XMLCh *const uri,
                            const XMLCh *const localname,
                            const XMLCh *const qname) override;
    // Error callbacks
    void error(const SAXParseException& e) override;
    void fatalError(const SAXParseException& e) override;
    void warning(const SAXParseException& e) override;

    protected:

    private:

    /** Simulation config file */
    const std::string myConfigFile;

    /** Map of properties to fill while parsing config file. */
    std::map<std::string, std::string> myProperties;

    /** Directory in which the schema is located. */
    const std::string mySimulationSchemaDirectory;

    /** Simulation world to populate. */
    World &myWorld;
  };
}
