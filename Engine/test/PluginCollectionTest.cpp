/**
 * @file PluginCollectionTest.cpp
 * @brief Unit test of the PluginCollection class.
 *
 * @author Michael Albers
 */

#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <unistd.h>
#include "PluginCollection.h"
#include "gtest/gtest.h"

class PluginCollectionTest : public ::testing::Test
{
  public:
  void SetUp() override
  {
    char *tmpDirTemplate = ::strdup("/tmp/PluginCollectionTest_XXXXXX");
    char *tmpDirCStr = ::mkdtemp(tmpDirTemplate);
    auto thisErrno = errno;
    ASSERT_TRUE(NULL != tmpDirCStr)
      << "Unexpectedly failed to make temporary dir from '"
      << tmpDirTemplate << ": " << std::strerror(thisErrno);
    
    myPluginBaseDir = tmpDirCStr;
    std::free(tmpDirTemplate);
  }

  void TearDown() override
  {
    std::string removalCmd{"/bin/rm -rf "};
    removalCmd += myPluginBaseDir;
    ::system(removalCmd.c_str());
  }

  std::string myPluginBaseDir;
};

TEST_F(PluginCollectionTest, testConstruction)
{
  // Test invalid plugin directory.
  try
  {
    QS::PluginCollection collection("");
    FAIL() << "Unexpectedly did not throw an exception.";
  }
  catch (const std::runtime_error &exception)
  {
    std::string expectedError{
      "Failed to open plugin configuration file base directory '"};
    expectedError += "': No such file or directory.";
    EXPECT_EQ(expectedError, exception.what());
  }
  catch (...)
  {
    FAIL();
  }

  std::string pluginDir{myPluginBaseDir + "/TestPlugin"};
  auto mkdirStatus = ::mkdir(pluginDir.c_str(), 0);
  auto thisErrno = errno;
  ASSERT_EQ(0, mkdirStatus) << "On mkdir " << pluginDir << ": "
                            << std::strerror(thisErrno);

  // Test error on directory which cannot be opened.
  try
  {
    QS::PluginCollection collection(myPluginBaseDir);
  }
  catch (const std::runtime_error &exception)
  {
    std::string expectedError{
      "Failed to open plugin directory '"};
    expectedError += pluginDir + "': Permission denied.";
    EXPECT_EQ(expectedError, exception.what());
  }
  catch (...)
  {
    FAIL();
  }

  auto chmodStatus = ::chmod(pluginDir.c_str(), 0700);
  thisErrno = errno;
  ASSERT_EQ(0, chmodStatus) << "On chmod 0700 " << pluginDir << ": "
                            << std::strerror(thisErrno);
  

  // Test no XML files in plugin directory
  try
  {
    QS::PluginCollection collection(myPluginBaseDir);
  }
  catch (const std::runtime_error &exception)
  {
    std::string expectedError{
      "No plugin configuration file found in '"};
    expectedError += pluginDir + "'.";
    EXPECT_EQ(expectedError, exception.what());
  }
  catch (...)
  {
    FAIL();
  }

  std::ofstream xmlFile1(pluginDir + "/File1.xml");
  ASSERT_TRUE(xmlFile1.is_open());
  std::ofstream xmlFile2(pluginDir + "/File2.xml");
  ASSERT_TRUE(xmlFile2.is_open());

  // Test too many XMLs files in plugin directory
  try
  {
    QS::PluginCollection collection(myPluginBaseDir);
  }
  catch (const std::runtime_error &exception)
  {
    std::string expectedError{
      "Too many XML files found in plugin configuration file directory '"};
    expectedError += pluginDir + "'. There can be only one.";
    EXPECT_EQ(expectedError, exception.what());
  }
  catch (...)
  {
    FAIL();
  }

  std::string unlinkFile{pluginDir + "/File2.xml"};
  auto unlinkStatus = ::unlink(unlinkFile.c_str());
  thisErrno = errno;
  ASSERT_EQ(0, unlinkStatus) << "On unlink " << unlinkFile << ": "
                             << std::strerror(thisErrno);

  // Test invalid file (just making sure the PluginReader class is called,
  // more extensive tests for that class are in its test file).
  EXPECT_THROW(QS::PluginCollection collection(myPluginBaseDir),
               std::logic_error);

}

TEST_F(PluginCollectionTest, testGetPlugin)
{
  QS::PluginCollection collection(myPluginBaseDir);

  try
  {
    collection.getPlugin("SomePlugin");
    FAIL() << "Unexpectedly did not throw an exception.";
  }
  catch (const std::invalid_argument &exception)
  {
    std::string expectedError{"No plugin with name 'SomePlugin' found."};
    EXPECT_EQ(expectedError, exception.what());
  }
  catch (...)
  {
    FAIL();
  }

  // TODO: at some point will need to verify a plugin can be retrieved
}
