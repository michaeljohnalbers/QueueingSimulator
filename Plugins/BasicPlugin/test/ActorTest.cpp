/**
 * @file ActorTest.cpp
 * @brief Unit tests for Actor class
 *
 * @author Michael Albers
 */

#define _USE_MATH_DEFINES // For M_PI
#include <cmath>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include "gtest/gtest.h"
#include "Actor.h"
#include "BehaviorSet.h"
#include "EigenHelper.h"
#include "Sensable.h"
#include "TestUtils.h"

class ActorTestFixture : public ::testing::Test
{
  public:

  virtual ~ActorTestFixture() = default;

  virtual void SetUp() override
  {
    ASSERT_NO_THROW(
      myActor = new QS::Actor(QS::TestUtils::getMinimalActorProperties(), ""));
  }

  virtual void TearDown() override
  {
    delete myActor;
  }

  protected:
  QS::Actor *myActor;

  class DummyBehaviorSet : public QS::BehaviorSet
  {
    public:
    DummyBehaviorSet() : BehaviorSet({}, ""){}
    virtual Eigen::Vector2f evaluate(const QS::Actor *theActor,
                                     const QS::Sensable &theSensable) override
    {
      return Eigen::Vector2f(14.0, 54.0);
    }
  };
};

GTEST_TEST(ActorTest, testConsts)
{
  Eigen::Vector2f expected{1,0};
  ASSERT_EQ(expected, QS::Actor::DEFAULT_ORIENTATION);
}

GTEST_TEST(ActorTest, testConstruction)
{
  // Test nominal construction
  try
  {
    QS::Actor actor(QS::TestUtils::getMinimalActorProperties(), "");

    // Test initial values are set
    EXPECT_FLOAT_EQ(1.0, actor.getRadius());
    EXPECT_FLOAT_EQ(2.0, actor.getMass());
    Eigen::Vector2f position(3.0, 4.0);
    EXPECT_EQ(position, actor.getPosition());
    Eigen::Vector3f color(1,1,1);
    EXPECT_EQ(color, actor.getColor());
    EXPECT_FLOAT_EQ(0, actor.getOrientation());
    EXPECT_FLOAT_EQ(8.5f, actor.getMaximumForce());
    EXPECT_FLOAT_EQ(9.0f, actor.getMaximumSpeed());
  }
  catch (...)
  {
    FAIL() << "Unexpected error thrown on good construction.";
  }

  // Test construction missing each of the required properties.
  std::vector<std::string> propertyNames{"mass", "radius", "x", "y",
      "max speed", "max force"};
  for (auto propertyName : propertyNames)
  {
    try
    {
      QS::PluginEntity::Properties properties{
        QS::TestUtils::getMinimalActorProperties()};
      properties.erase(propertyName);
      QS::Actor actor(properties, "");
      FAIL() << "On " << propertyName;
    }
    catch (const std::invalid_argument &e)
    {
      std::string expectedError{"Missing '"};
      expectedError += propertyName + "' property.";
      EXPECT_EQ(expectedError, e.what()) << "On " << propertyName;
    }
    catch(...)
    {
      FAIL() << "On " << propertyName;
    }
  }

  // Test various invalid property values.
  {
    std::vector<std::tuple<std::string, std::string, std::string>> badValues{
      std::make_tuple("mass", "0.0", "Invalid mass 0.000000, "
                      "must be > 0 in Actor constructor."),
      std::make_tuple("mass", "-1.0", "Invalid mass -1.000000, "
                      "must be > 0 in Actor constructor."),
      std::make_tuple("radius", "0.0", "Invalid radius 0.000000, "
                      "must be > 0 in Actor constructor."),
      std::make_tuple("radius", "-1.0", "Invalid radius -1.000000, "
                      "must be > 0 in Actor constructor."),
      std::make_tuple("max speed", "-1.0", "Invalid maximum speed -1.000000, "
                      "must be >= 0 in Actor constructor."),
      std::make_tuple("max force", "-1.0", "Invalid maximum force -1.000000, "
                      "must be >= 0 in Actor constructor.")
    };

    for(auto invalidTuple: badValues)
    {
      try
      {
        QS::PluginEntity::Properties badProperties{
          QS::TestUtils::getMinimalActorProperties()};
        badProperties[std::get<0>(invalidTuple)] = std::get<1>(invalidTuple);
        QS::Actor actor(badProperties, "");
        FAIL() << "Unexpectedly passed with bad property: "
               << std::get<0>(invalidTuple) << ", with value: "
               << std::get<1>(invalidTuple);
      }
      catch (const std::invalid_argument &e)
      {
        EXPECT_EQ(std::get<2>(invalidTuple), e.what());
      }
      catch(...)
      {
        FAIL();
      }
    }
  }
}

TEST_F(ActorTestFixture, testEvaluate)
{
  DummyBehaviorSet behaviorSet;
  QS::EntityDependency<QS::BehaviorSet> dependency{"", &behaviorSet, ""};
  myActor->setDependencies({dependency});
  QS::Sensable sensable(myActor, {}, {}, 0.0);
  EXPECT_EQ(Eigen::Vector2f(14.0, 54.0), myActor->evaluate(sensable));
}

TEST_F(ActorTestFixture, testGetMass)
{
  EXPECT_FLOAT_EQ(2.0, myActor->getMass());
}

TEST_F(ActorTestFixture, getGetMaximumForce)
{
  QS::PluginEntity::Properties properties{
    QS::TestUtils::getMinimalActorProperties()};

  {
    properties["max force"] = "93.5544";
    QS::Actor actor(properties, "");
    EXPECT_FLOAT_EQ(93.5544, actor.getMaximumForce());
  }

  // Test 0.0
  {
    properties["max force"] = "0.0";
    QS::Actor actor(properties, "");
    EXPECT_FLOAT_EQ(0.0, actor.getMaximumForce());
  }
}

TEST_F(ActorTestFixture, getGetMaximumSpeed)
{
  QS::PluginEntity::Properties properties{
    QS::TestUtils::getMinimalActorProperties()};

  {
    properties["max speed"] = "55.9344";
    QS::Actor actor(properties, "");
    EXPECT_FLOAT_EQ(55.9344, actor.getMaximumSpeed());
  }

  // Test 0.0
  {
    properties["max speed"] = "0.0";
    QS::Actor actor(properties, "");
    EXPECT_FLOAT_EQ(0.0, actor.getMaximumSpeed());
  }
}

TEST_F(ActorTestFixture, testGetSetOrientation)
{
  EXPECT_NO_THROW(myActor->setOrientation(3.4));
  EXPECT_FLOAT_EQ(3.4, myActor->getOrientation());

  // Test that values are normalized
  myActor->setOrientation(2 * M_PI);
  EXPECT_FLOAT_EQ(0, myActor->getOrientation());

  myActor->setOrientation(4 * M_PI);
  EXPECT_FLOAT_EQ(0, myActor->getOrientation());

  myActor->setOrientation(-M_PI);
  EXPECT_FLOAT_EQ(M_PI, myActor->getOrientation());
}

TEST_F(ActorTestFixture, testGetSetPosition)
{
  Eigen::Vector2f position(1.5, 20.5);
  EXPECT_NO_THROW(myActor->setPosition(position));
  EXPECT_EQ(position, myActor->getPosition());
}

TEST_F(ActorTestFixture, testGetRadius)
{
  EXPECT_FLOAT_EQ(1.0, myActor->getRadius());
}

TEST_F(ActorTestFixture, testGetSetVelocity)
{
  Eigen::Vector2f velocity(3.2, 21.99);
  EXPECT_NO_THROW(myActor->setVelocity(velocity));
  EXPECT_EQ(velocity, myActor->getVelocity());
}

TEST_F(ActorTestFixture, testConvertPointToLocal)
{
  // First are basic tests for simple, obvious cases. Just to see if the
  // conversion is partially working.
  myActor->setPosition({0,0});
  myActor->setOrientation(0);

  auto runTest = [=](const Eigen::Vector2f &theWorldPoint,
                     const Eigen::Vector2f &theExpectedPoint)
  {
    Eigen::Vector2f convertedPoint = myActor->convertPointToLocal(
      theWorldPoint);
    // Using FLOAT_EQ as Eigen's built in == does exact comparison. There
    // are better ways to do this than duplicating the output code, but I'm
    // being lazy here.
    EXPECT_TRUE(theExpectedPoint.isApprox(convertedPoint,
                                          QS::EigenHelper::FLOAT_TOLERANCE))
    << "   World Point: ("
    << theWorldPoint.format(QS::EigenHelper::prettyPrint) << ")" << std::endl
    << " Expected Point: ("
    << theExpectedPoint.format(QS::EigenHelper::prettyPrint) << ")" << std::endl
    << "Converted Point: ("
    << convertedPoint.format(QS::EigenHelper::prettyPrint) << ")" << std::endl;
  };

  runTest({1,0}, {1,0});
  runTest({0,1}, {0,1});
  runTest({-1,0}, {-1,0});
  runTest({0,-1}, {0,-1});

  /*
   * Now move the Actor away from the origin, but still use points at 90 degree
   * intervals. Change the orientation of the Actor, too.
   */
  myActor->setPosition({1,0});
  myActor->setOrientation(M_PI/2.0); // Left 90 degrees

  runTest(myActor->getPosition(), {0,0});
  runTest({2,0}, {0,-1});
  runTest({2,1}, {1,-1});
  runTest({0,0}, {0,1});
  runTest({1,-1}, {-1,0});

  /*
   * Heavy tests. Actor in arbitrary position, arbitrary world positions.
   */
  myActor->setPosition({34.1333,98.5});
  myActor->setOrientation(1.77432);

  runTest(myActor->getPosition(), {0,0});
  // Not testing anything further because I don't want to do the math by hand.
  // If everything worked to this point. There isn't much reason to think this
  // won't either (famous last words...).
}
