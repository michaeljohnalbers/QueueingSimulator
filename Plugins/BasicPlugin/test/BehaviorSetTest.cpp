/**
 * @file BehaviorSetTest.cpp
 * @brief Unit test of BehaviorSet class
 *
 * @author Michael Albers
 */

#include "gtest/gtest.h"
#include "Behavior.h"
#include "BehaviorSet.h"
#include "EigenHelper.h"
#include "EntityDependency.h"
#include "Sensable.h"

namespace BehaviorSetTest
{
  class TestBehavior : public QS::Behavior
  {
    public:

    TestBehavior(const Eigen::Vector2f &theMotionVector) :
      Behavior({}, "TestBehavior"),
      myMotionVector(theMotionVector)
    {
    }

    virtual ~TestBehavior() = default;

    virtual Eigen::Vector2f evaluate(const QS::Actor *theActor,
                                     float theIntervalInSeconds) override
    {
      return myMotionVector;
    }

    protected:
    private:

    Eigen::Vector2f myMotionVector;
  };
}

GTEST_TEST(BehaviorSetTest, testConstruction)
{
  QS::PluginEntity::Properties properties = {
    {"Key1", "Value1"},
    {"Dummy", "Dummy, too"}};

  // Test nominal construction
  EXPECT_NO_THROW(QS::BehaviorSet(properties, ""));

}

GTEST_TEST(BehaviorSetTest, testEvalutate)
{
  QS::BehaviorSet behaviorSet({}, "");

  Eigen::Vector2f motionVector1(1.0, 0.0);
  BehaviorSetTest::TestBehavior behavior1(motionVector1);
  QS::EntityDependency<QS::Behavior> dep1{"1", &behavior1, "TAgg1"};

  Eigen::Vector2f motionVector2(0.35, 1.22);
  BehaviorSetTest::TestBehavior behavior2(motionVector2);
  QS::EntityDependency<QS::Behavior> dep2{"2", &behavior2, "Tag2"};

  Eigen::Vector2f motionVector3(2.1, -2.876);
  BehaviorSetTest::TestBehavior behavior3(motionVector3);
  QS::EntityDependency<QS::Behavior> dep3{"3", &behavior3, "3gaT"};

  std::vector<QS::EntityDependency<QS::Behavior>> dependencies{
    dep1, dep2, dep3};
  behaviorSet.setDependencies(dependencies);

  Eigen::Vector2f expectedMotionVector(
    (motionVector1.x() + motionVector2.x() + motionVector3.x()) / 3,
    (motionVector1.y() + motionVector2.y() + motionVector3.y()) / 3);

  QS::Sensable sensable({}, 1.0);
  auto actualMotionVector = behaviorSet.evaluate(nullptr, sensable);
  EXPECT_EQ(expectedMotionVector, actualMotionVector)
    << "Expected: " << expectedMotionVector.format(QS::prettyPrint)
    << ", Actual: " << actualMotionVector.format(QS::prettyPrint);
}
