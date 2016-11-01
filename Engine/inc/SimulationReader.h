#pragma once

/**
 * @file SimulationReader.h
 * @brief Reads in a single simulation configuration file
 *
 * @author Michael Albers
 */

#include <map>
#include <memory>
#include <stack>
#include <string>
#include "xercesc/sax2/DefaultHandler.hpp"
#include "PluginEntity.h"
#include "PropertyGenerator.h"
#include "SimulationEntityConfiguration.h"

XERCES_CPP_NAMESPACE_USE

namespace QS
{
  class EntityManager;
  class World;

  /**
   * Instances of this class read a simulation configuration file and populate
   * the provided word with simulation entities defined by the file.
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
     * @param theEntityManager
     *          creator of plugin entities
     * @param theWorld
     *          simulation world to populate from data from the config file
     */
    SimulationReader(const std::string &theConfigFile,
                     const std::string &theSimulationSchemaDirectory,
                     std::shared_ptr<EntityManager> theEntityManager,
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
     * Reads the simulation configuration file.
     *
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

    /** Name of the plugin where the Actor can be found. */
    std::string myActorSource;

    /** Type name of Actor to create */
    std::string myActorTypeName;

    /** Simulation config file */
    const std::string myConfigFile;

    /**
     * Entity configurations, as a new dependency is encountered, it will be
     * added to the stack, and removed when finished.
     */
    std::stack<SimulationEntityConfiguration> myEntityConfigurations;

    /** Creator of plugin entities. */
    std::shared_ptr<EntityManager> myEntityManager;

    /**
     * Generates properties based on small language which can be embedded in
     * the configuration file.
     */
    PropertyGenerator myPropertyGenerator;

    /** Directory in which the schema is located. */
    const std::string mySimulationSchemaDirectory;

    /** Simulation world to populate. */
    World &myWorld;
  };
}
