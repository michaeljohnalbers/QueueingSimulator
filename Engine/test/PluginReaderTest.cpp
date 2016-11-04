/**
 * @file PluginReaderTest.cpp
 * @brief Unit test of the PluginReader class.
 *
 * @author Michael Albers
 */

#include <fstream>
#include <unistd.h>
#include "PluginDefinition.h"
#include "PluginReader.h"
#include "TestPluginConfigData.h"
#include "gtest/gtest.h"

GTEST_TEST(PluginReaderTest, testConstruction)
{
  EXPECT_NO_THROW(QS::PluginReader("", "", ""));
}

GTEST_TEST(PluginReaderTest, testRead)
{
  std::string pluginDirectory{"/tmp"};
  std::string configFile{"TestConfig.xml"};
  std::string schemaDirectory{QS_PLUGIN_SCHEMA_DIR};

  auto writeConfig = [=](const std::string &theConfigData)
  {
    std::string fileName = pluginDirectory + "/" + configFile;
    unlink(fileName.c_str());
    std::ofstream file(fileName);
    ASSERT_TRUE(file.is_open());
    file << theConfigData;
  };

  // Test reading in a well formatted file.
  {
    writeConfig(pluginConfigNominal);
    QS::PluginReader reader(pluginDirectory, configFile, schemaDirectory);
    std::shared_ptr<QS::PluginDefinition> pluginDef;
    try
    {
      pluginDef = reader.read();
    }
    catch (const std::exception &e)
    {
      FAIL() << "Unexpectedly threw an exception during parsing: " << e.what();
    }
    catch (...)
    {
      FAIL() << "Unexpectedly threw an unknown exception during parsing.";
    }

    // Spot check some things in the definition.
    EXPECT_EQ("test-lib.so", pluginDef->getLibrary());
    EXPECT_EQ(pluginDirectory, pluginDef->getDirectory());
    EXPECT_EQ("TestPlugin", pluginDef->getName());

    EXPECT_EQ("actorCreator", pluginDef->getActorCreatorDestructor().first);
    EXPECT_EQ("actorDestructor", pluginDef->getActorCreatorDestructor().second);
    EXPECT_EQ(1u, pluginDef->getActorDefinitions().size());

    EXPECT_EQ("behaviorSetCreator",
              pluginDef->getBehaviorSetCreatorDestructor().first);
    EXPECT_EQ("behaviorSetDestructor",
              pluginDef->getBehaviorSetCreatorDestructor().second);
    EXPECT_EQ(1u, pluginDef->getBehaviorSetDefinitions().size());

    EXPECT_EQ("behaviorCreator",
              pluginDef->getBehaviorCreatorDestructor().first);
    EXPECT_EQ("behaviorDestructor",
              pluginDef->getBehaviorCreatorDestructor().second);
    EXPECT_EQ(1u, pluginDef->getBehaviorDefinitions().size());

    EXPECT_EQ("sensorCreator", pluginDef->getSensorCreatorDestructor().first);
    EXPECT_EQ("sensorDestructor",
              pluginDef->getSensorCreatorDestructor().second);
    EXPECT_EQ(1u, pluginDef->getSensorDefinitions().size());

    EXPECT_EQ("exitCreator", pluginDef->getExitCreatorDestructor().first);
    EXPECT_EQ("exitDestructor",
              pluginDef->getExitCreatorDestructor().second);
    EXPECT_EQ(1u, pluginDef->getExitDefinitions().size());
  }

  // Test reading in a malformed file. Just testing that an exception of
  // the right type is thrown.
  {
    writeConfig(pluginConfigMalFormed);
    QS::PluginReader reader(pluginDirectory, configFile, schemaDirectory);
    EXPECT_THROW(reader.read(), std::logic_error);
  }
}
