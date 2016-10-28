/**
 * @file SimulationReader.cpp
 * @brief Definition of SimulationReader class
 *
 * @author Michael Albers
 */

#include <stdexcept>
#include "xercesc/sax2/SAX2XMLReader.hpp"
#include "xercesc/sax2/XMLReaderFactory.hpp"
#include "xercesc/util/XMLString.hpp"
#include "xercesc/sax2/Attributes.hpp"
#include "EntityManager.h"
#include "Finally.h"
#include "SimulationReader.h"
#include "XMLUtilities.h"
#include "World.h"

QS::SimulationReader::SimulationReader(
  const std::string &theConfigFile,
  const std::string &theSimulationSchemaDirectory,
  std::shared_ptr<EntityManager> theEntityManager,
  World &theWorld) :

  myConfigFile(theConfigFile),
  myEntityManager(theEntityManager),
  mySimulationSchemaDirectory(theSimulationSchemaDirectory),
  myWorld(theWorld)
{
}

void QS::SimulationReader::read()
{
  try
  {
    SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();
    Finally parserCleanup([=](){delete parser;});
    std::string schema = mySimulationSchemaDirectory + "/" + SCHEMA_FILE;
    parser->loadGrammar(schema.c_str(), Grammar::SchemaGrammarType, true);
    parser->setFeature(XMLUni::fgXercesUseCachedGrammarInParse, true);
    parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
    parser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);
    parser->setFeature(XMLUni::fgXercesSchemaFullChecking, true);

    parser->setContentHandler(this);
    parser->setErrorHandler(this);

    parser->parse(myConfigFile.c_str());
  }
  catch (const XMLException &exception)
  {
    auto message = XMLUtilities::cStr(exception.getMessage());
    throw std::logic_error{"Error reading simulation configuration file '" +
        myConfigFile + "': " + message.get() +
        " at " + XMLUtilities::getLocation(exception)};
  }
  catch (const SAXParseException &exception)
  {
    auto message = XMLUtilities::cStr(exception.getMessage());
    throw std::logic_error{"Error parsing simulation configuration file '" +
        myConfigFile + "': " + message.get() +
        " at " + XMLUtilities::getLocation(exception)};
  }
  catch (const std::exception &e)
  {
    throw std::logic_error{
      "Error while reading simulation configuration file '" +
        myConfigFile + "': " + e.what()};
  }
  catch (...)
  {
    throw std::logic_error{
      "Unexpected exception while reading simulation configuration file '" +
        myConfigFile + "'."};
  }
}

void QS::SimulationReader::endElement(const XMLCh *const uri,
                                      const XMLCh *const localname,
                                      const XMLCh *const qname)
{
  std::string elementName{XMLUtilities::cStr(localname).get()};
  if ("Actor" == elementName)
  {
    auto actorConfiguration = myEntityConfigurations.top();
    auto newActor = myEntityManager->createActor(actorConfiguration);
    myWorld.addActor(newActor);
    myEntityConfigurations.pop();
  }
  else if ("BehaviorSet" == elementName || "Behavior" == elementName ||
           "Sensor" == elementName)
  {
    auto entityDependencyConfiguration = myEntityConfigurations.top();
    myEntityConfigurations.pop();
    myEntityConfigurations.top().addDependencyConfiguration(
      entityDependencyConfiguration);
  }
}

void QS::SimulationReader::startElement(const XMLCh *const uri,
                                        const XMLCh *const localname,
                                        const XMLCh *const qname,
                                        const Attributes &attrs)
{
  std::string elementName{XMLUtilities::cStr(localname).get()};

  if ("Seed" == elementName)
  {
    auto seedString = XMLUtilities::getAttribute(attrs, "value");
    myWorld.setSeed(std::stoull(seedString));
  }
  else if ("World" == elementName)
  {
    auto lengthString = XMLUtilities::getAttribute(attrs, "length");
    auto widthString = XMLUtilities::getAttribute(attrs, "width");
    myWorld.setDimensions(std::stof(widthString), std::stof(lengthString));
  }
  else if ("Actor" == elementName || "BehaviorSet" == elementName ||
           "Behavior" == elementName || "Sensor" == elementName)
  {
    auto type = XMLUtilities::getAttribute(attrs, "type");
    auto source = XMLUtilities::getAttribute(attrs, "source");
    std::string tag;
    try
    {
      tag = XMLUtilities::getAttribute(attrs, "tag");
    } catch (...) {}

    myEntityConfigurations.emplace(type, tag, source);
  }
  else if ("Property" == elementName)
  {
    myEntityConfigurations.top().addProperty(
      XMLUtilities::getAttribute(attrs, "key"),
      XMLUtilities::getAttribute(attrs, "value"));
  }
}

void QS::SimulationReader::error(const SAXParseException& e)
{
  throw e;
}

void QS::SimulationReader::fatalError(const SAXParseException& e)
{
  throw e;
}

void QS::SimulationReader::warning(const SAXParseException& e)
{
  throw e;
}
