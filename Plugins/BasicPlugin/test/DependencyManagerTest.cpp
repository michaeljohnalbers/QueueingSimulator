/**
 * @file DependencyManagerTest.cpp
 * @brief Unit tests for DependencyManager class
 *
 * @author Michael Albers
 */

#include "DependencyManager.h"
#include "gtest/gtest.h"

GTEST_TEST(DependencyManagerTest, testDefaults)
{
  // All defaulted, just make sure they exist.
  QS::DependencyManager<int> depMgr;
  auto depMgrCopy(depMgr);
  depMgrCopy = depMgr;
  auto depMgrMove(std::move(depMgr));
  depMgrMove = std::move(depMgr);
}

GTEST_TEST(DependencyManagerTest, testGetSetDependencies)
{
  // Test empty dependency list
  QS::DependencyManager<std::string> depMgr;
  try
  {
    std::vector<QS::EntityDependency<std::string>> emptyDependencies;
    depMgr.setDependencies(emptyDependencies);
    FAIL();
  }
  catch (const std::invalid_argument &e)
  {
    EXPECT_STREQ(
      "Cannot have empty set of dependencies.",
      e.what());
  }
  catch (...)
  {
    FAIL();
  }

  // Make sure empty dependency name & tag is OK.
  {
    std::string *string = reinterpret_cast<std::string*>(0x1);
    std::vector<QS::EntityDependency<std::string>> missingName{
      {"", string, ""}};
    EXPECT_NO_THROW(depMgr.setDependencies(missingName));
  }

  // Test dependency with nullptr
  try
  {
    std::vector<QS::EntityDependency<std::string>> nullDep{
      {"NullDependency", nullptr, ""}};
    depMgr.setDependencies(nullDep);
    FAIL();
  }
  catch (const std::invalid_argument &e)
  {
    EXPECT_STREQ("Cannot have null entity (for type name 'NullDependency').",
                 e.what());
  }
  catch (...)
  {
    FAIL();
  }
}
