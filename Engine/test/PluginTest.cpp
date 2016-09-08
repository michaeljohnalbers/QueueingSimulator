/**
 * @file PluginTest.cpp
 * @brief Unit test of the Plugin class.
 *
 * @author Michael Albers
 */

#include "Plugin.h"
#include "gtest/gtest.h"

// TODO: add more tests when I figure out how to build test plugins

GTEST_TEST(PluginTest, testConstruction)
{
  // Test loading plugin with missing library
  std::shared_ptr<QS::PluginDefinition> pluginDef(
    new QS::PluginDefinition("/tmp/no/such/dir"));
  pluginDef->setLibrary("DoesntExist.so");
  pluginDef->setName("BadPlugin");
  try
  {
    QS::Plugin plugin(pluginDef);
    FAIL() << "Unexpectedly successfully loaded plugin.";
  }
  catch (const std::runtime_error &exception)
  {
    std::string library = pluginDef->getDirectory() + "/" +
      pluginDef->getLibrary();
    std::string expectedError{"Failed to open library '"};
    expectedError += library + "' for plugin " + pluginDef->getName() + ": " +
      library + ": cannot open shared object file: No such file or directory.";
    
    EXPECT_EQ(expectedError, exception.what());
  }
  catch (...)
  {
    FAIL() << "Unexpected exception type thrown.";
  }
}
