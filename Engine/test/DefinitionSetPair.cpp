/**
 * @file DefinitionPairTest.cpp
 * @brief Unit test of DefinitionPair class
 *
 * @author Michael Albers
 */

#include "DefinitionPair.h"
#include "gtest/gtest.h"

GTEST_TEST(DefinitionPair, testOperators)
{
  QS::DefinitionPair pair1{"BehaviorSet1", "Source1"};
  auto pairCopy{pair1};
  QS::DefinitionPair pair2{"BehaviorSet2", "Source2"};

  EXPECT_TRUE(pair1 < pair2);
  EXPECT_FALSE(pair2 < pair1);
  EXPECT_TRUE(pair1 == pairCopy);
  EXPECT_TRUE(pairCopy == pair1);
  EXPECT_FALSE(pair1 == pair2);
  EXPECT_FALSE(pair1 == pair2);
}
