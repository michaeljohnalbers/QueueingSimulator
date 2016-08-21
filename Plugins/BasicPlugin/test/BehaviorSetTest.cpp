/**
 * @file BehaviorSetTest.cpp
 * @brief Unit test of BehaviorSet class
 *
 * @author Michael Albers
 */

#include "BehaviorSet.h"
#include "gtest/gtest.h"

GTEST_TEST(BehaviorSetTest, testConstruction)
{
  // Test nominal construction
  {
    QS::Behavior *behavior = reinterpret_cast<QS::Behavior*>(0x1);
    EXPECT_NO_THROW(QS::BehaviorSet({{"Behavior", behavior}}));
    // Make sure empty behavior name is OK.
    EXPECT_NO_THROW(QS::BehaviorSet({{"", behavior}}));
  }

  // Test empty behavior list
  try
  {
    QS::BehaviorSet set{{}};
    FAIL();
  }
  catch (const std::invalid_argument &e)
  {
    EXPECT_STREQ(
      "Cannot have empty set of behaviors in BehaviorSet constructor.",
      e.what());
  }
  catch (...)
  {
    FAIL();
  }

  // Test behavior list with nullptr
  try
  {
    QS::BehaviorSet set{{{"NullBehavior", nullptr}}};
    FAIL();
  }
  catch (const std::invalid_argument &e)
  {
    EXPECT_STREQ("Cannot have null Behavior (for name 'NullBehavior') in "
                 "BehaviorSet constructor.",
                 e.what());
  }
  catch (...)
  {
    FAIL();
  }
}

GTEST_TEST(BehaviorSetTest, testEvalutate)
{
  // TODO: fill in when base 'evaluate' finished
}
