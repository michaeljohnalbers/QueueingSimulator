/**
 * @file PluginReader.cpp
 * @brief Definition of PluginReader class
 *
 * @author Michael Albers
 */
#include <stdexcept>
#include "ActorDefinition.h"
#include "BehaviorSetDefinition.h"
#include "PluginDefinition.h"
#include "PluginReader.h"
#include "XMLUtilities.h"
#include "xercesc/sax2/SAX2XMLReader.hpp"
#include "xercesc/sax2/XMLReaderFactory.hpp"
#include "xercesc/util/XMLString.hpp"
#include "xercesc/sax2/Attributes.hpp"

QS::PluginReader::PluginReader(const std::string &thePluginDirectory,
                               const std::string &theConfigFile) :
  myConfigFile(theConfigFile),
  myPluginDirectory(thePluginDirectory)
{
}

std::shared_ptr<QS::PluginDefinition> QS::PluginReader::read()
{
  try
  {
    SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();
    parser->loadGrammar("../plugins/PluginConfig.xsd",
                        Grammar::SchemaGrammarType, true);
    parser->setFeature(XMLUni::fgXercesUseCachedGrammarInParse, true);
    parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
    parser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);
    parser->setFeature(XMLUni::fgXercesSchemaFullChecking, true);

    parser->setContentHandler(this);
    parser->setErrorHandler(this);

    std::string xmlFile = myPluginDirectory + "/" + myConfigFile;
    parser->parse(xmlFile.c_str());
  }
  catch (const XMLException &exception)
  {
    auto message = XMLUtilities::cStr(exception.getMessage());
    throw std::logic_error{"Error reading plugin configuration file '" +
        myConfigFile + "': " + message.get() +
        " at " + XMLUtilities::getLocation(exception)};
  }
  catch (const SAXParseException &exception)
  {
    auto message = XMLUtilities::cStr(exception.getMessage());
    throw std::logic_error{"Error parsing plugin configuration file '" +
        myConfigFile + "': " + message.get() +
        " at " + XMLUtilities::getLocation(exception)};
  }
  catch (...)
  {
    throw std::logic_error{
      "Unexpected exception while reading plugin configuration file '" +
        myConfigFile + "'"};
  }
  return myPluginDefinition;
}

void QS::PluginReader::characters(const XMLCh *const chars,
                                  const XMLSize_t length)
{
  myStringBuffer += XMLUtilities::cStr(chars).get();
}

void QS::PluginReader::startDocument()
{
  myPluginDefinition.reset(new PluginDefinition(myPluginDirectory));
}

void QS::PluginReader::startElement(const XMLCh *const uri,
                                    const XMLCh *const localname,
                                    const XMLCh *const qname,
                                    const Attributes &attrs)
{
  auto elementNameRaw = XMLUtilities::cStr(localname);
  std::string elementName{elementNameRaw.get()};
  myStringBuffer = "";

  if ("Plugin" == elementName)
  {
    auto pluginName = XMLUtilities::getAttribute(attrs, "name");
    auto pluginLibrary = XMLUtilities::getAttribute(attrs, "library");
    myPluginDefinition->setName(pluginName);
    myPluginDefinition->setLibrary(pluginLibrary);
  }
  else if ("Actors" == elementName)
  {
    auto actorCreator = XMLUtilities::getAttribute(attrs, "creator");
    auto actorDestructor = XMLUtilities::getAttribute(attrs, "destructor");
    myPluginDefinition->setActorCreatorDestructor(
      actorCreator, actorDestructor);
  }
  else if ("Actors" == elementName)
  {
    auto actorType = XMLUtilities::getAttribute(attrs, "type");
  }
  else if ("BehaviorSets" == elementName)
  {
    auto behaviorSetCreator = XMLUtilities::getAttribute(attrs, "creator");
    auto behaviorSetDestructor = XMLUtilities::getAttribute(attrs,
                                                            "destructor");
    myPluginDefinition->setBehaviorSetCreatorDestructor(
      behaviorSetCreator, behaviorSetDestructor);
  }
  else if ("BehaviorSet" == elementName)
  {
    auto behaviorSetName = XMLUtilities::getAttribute(attrs, "name");
  }
  else if ("Behaviors" == elementName)
  {
    auto behaviorCreator = XMLUtilities::getAttribute(attrs, "creator");
    auto behaviorDestructor = XMLUtilities::getAttribute(attrs, "destructor");
    myPluginDefinition->setBehaviorCreatorDestructor(
      behaviorCreator, behaviorDestructor);
  }
  else if ("Behavior" == elementName)
  {
    auto behaviorName = XMLUtilities::getAttribute(attrs, "name");
  }
  else if ("Sensors" == elementName)
  {
    auto sensorCreator = XMLUtilities::getAttribute(attrs, "creator");
    auto sensorDestructor = XMLUtilities::getAttribute(attrs, "destructor");
    myPluginDefinition->setSensorCreatorDestructor(
      sensorCreator, sensorDestructor);
  }
  else if ("Sensor" == elementName)
  {
  }
}

void QS::PluginReader::error(const SAXParseException& e)
{
  throw e;
}

void QS::PluginReader::fatalError(const SAXParseException& e)
{
  throw e;
}

void QS::PluginReader::warning(const SAXParseException& e)
{
  throw e;
}
