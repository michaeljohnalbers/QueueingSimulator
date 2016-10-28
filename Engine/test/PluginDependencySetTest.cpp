/**
 * @file PluginDependencySetTest.cpp
 * @brief Unit test of PluginDependencySet class
 *
 * @author Michael Albers
 */

#include "PluginDependencySet.h"
#include "gtest/gtest.h"

GTEST_TEST(PluginDependencySet, testOperators)
{
  QS::PluginDependencySet pair1{"BehaviorSet1", "Source1", "Tag1"};
  auto pairCopy{pair1};
  QS::PluginDependencySet pair2{"BehaviorSet2", "Source2", "Tag2"};

  EXPECT_TRUE(pair1 < pair2);
  EXPECT_FALSE(pair2 < pair1);
  EXPECT_TRUE(pair1 == pairCopy);
  EXPECT_TRUE(pairCopy == pair1);
  EXPECT_FALSE(pair1 == pair2);
  EXPECT_FALSE(pair1 == pair2);

  QS::PluginDependencySet pair3{"BehaviorSet2", "Source2", ""};
  EXPECT_FALSE(pair2 == pair3);
  EXPECT_FALSE(pair3 == pair2);
  EXPECT_FALSE(pair2 < pair3);
  EXPECT_TRUE(pair3 < pair2);
}
