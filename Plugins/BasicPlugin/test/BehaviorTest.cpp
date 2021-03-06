/**
 * @file Behavior.cpp
 * @brief Unit tests for Behavior class
 *
 * @author Michael Albers
 */

#include <stdexcept>

#include "Behavior.h"
#include "gtest/gtest.h"

namespace BehaviorTest
{
  class TestBehavior : public QS::Behavior
  {
    public:
    TestBehavior() :
      Behavior(QS::PluginEntity::Properties{}, "")
    {
    }

    virtual Eigen::Vector2f evaluate(
      const QS::Actor *theActor) override
    {
      return {0.0, 0.0};
    }
  };
}

GTEST_TEST(BehaviorTest, testConstruction)
{
  // Test nominal construction
  EXPECT_NO_THROW(BehaviorTest::TestBehavior());
}
