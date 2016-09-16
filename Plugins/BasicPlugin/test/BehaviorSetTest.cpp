/**
 * @file BehaviorSetTest.cpp
 * @brief Unit test of BehaviorSet class
 *
 * @author Michael Albers
 */

#include "Behavior.h"
#include "BehaviorSet.h"
#include "gtest/gtest.h"

GTEST_TEST(BehaviorSetTest, testConstruction)
{
  QS::PluginEntity::Properties properties = {
    {"Key1", "Value1"},
    {"Dummy", "Dummy, too"}};

  // Test nominal construction
  EXPECT_NO_THROW(QS::BehaviorSet{properties});

}

GTEST_TEST(BehaviorSetTest, testEvalutate)
{
  // TODO: fill in when base 'evaluate' finished
}
