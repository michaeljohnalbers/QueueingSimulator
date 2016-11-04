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
#include "EigenHelper.h"

static QS::PluginEntity::Properties glbGoodProperties{
  {"radius", "0.1"},
  {"mass", "0.2"},
  {"x", "0.5"},
  {"y", "0.6"}};

class ActorTestFixture : public ::testing::Test
{
  public:

  virtual ~ActorTestFixture() = default;

  virtual void SetUp() override
  {
    ASSERT_NO_THROW(myActor = new QS::Actor(glbGoodProperties, ""));
  }

  virtual void TearDown() override
  {
    delete myActor;
  }

  protected:
  QS::Actor *myActor;
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
    QS::Actor actor(glbGoodProperties, "");

    // Test initial values are set
    EXPECT_FLOAT_EQ(0.1, actor.getRadius());
    EXPECT_FLOAT_EQ(0.2, actor.getMass());
    Eigen::Vector2f position(0.5, 0.6);
    EXPECT_EQ(position, actor.getPosition());
    Eigen::Vector3f color(1,1,1);
    EXPECT_EQ(color, actor.getColor());
    EXPECT_FLOAT_EQ(0, actor.getOrientation());
    EXPECT_FLOAT_EQ(-1.0f, actor.getMaximumRotationSpeed());
    EXPECT_FLOAT_EQ(-1.0f, actor.getMaximumSpeed());
  }
  catch (...)
  {
    FAIL() << "Unexpected error thrown on good construction.";
  }

  // Test construction just missing mass arugment
  {
    try
    {
      QS::PluginEntity::Properties properties;
      properties.insert({"radius", "1.0"});
      properties.insert({"x", "0.5"});
      properties.insert({"y", "0.5"});
      QS::Actor actor(properties, "");
      FAIL();
    }
    catch (const std::invalid_argument &e)
    {
      EXPECT_EQ(std::string("Missing 'mass' property."), e.what());
    }
    catch(...)
    {
      FAIL();
    }
  }

  // Test construction just missing radius arugment
  {
    try
    {
      std::map<std::string, std::string> properties;
      properties.insert({"mass", "1.0"});
      properties.insert({"x", "0.5"});
      properties.insert({"y", "0.5"});
      QS::Actor actor(properties, "ActorInstance1");
      FAIL();
    }
    catch (const std::invalid_argument &e)
    {
      EXPECT_EQ(std::string("Missing 'radius' property."), e.what());
    }
    catch(...)
    {
      FAIL();
    }
  }

  // Test construction just missing x arugment
  {
    try
    {
      std::map<std::string, std::string> properties;
      properties.insert({"mass", "1.0"});
      properties.insert({"radius", "0.1"});
      properties.insert({"y", "0.5"});
      QS::Actor actor(properties, "");
      FAIL();
    }
    catch (const std::invalid_argument &e)
    {
      EXPECT_EQ(std::string("Missing 'x' property."), e.what());
    }
    catch(...)
    {
      FAIL();
    }
  }

  // Test construction just missing y arugment
  {
    try
    {
      std::map<std::string, std::string> properties;
      properties.insert({"mass", "1.0"});
      properties.insert({"radius", "0.1"});
      properties.insert({"x", "0.5"});
      QS::Actor actor(properties, "");
      FAIL();
    }
    catch (const std::invalid_argument &e)
    {
      EXPECT_EQ(std::string("Missing 'y' property."), e.what());
    }
    catch(...)
    {
      FAIL();
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
                      "must be > 0 in Actor constructor.")
    };

    for(auto invalidTuple: badValues)
    {
      try
      {
        std::map<std::string, std::string> badProperties{glbGoodProperties};
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

TEST_F(ActorTestFixture, adjustVectorToMaximums)
{
  QS::PluginEntity::Properties properties{glbGoodProperties};
  properties.insert({"max speed", "2.0"});
  // 1/4 rotation
  properties.insert({"max rotation", std::to_string(M_PI / 2.0)});

  QS::Actor actor(properties, "");

  // Null case. Vector that doesn't move shouldn't be modified.
  {
    Eigen::Vector2f inputVector(0, 0);
    float interval = 0.005;
    auto adjustedVector = actor.adjustVectorToMaximums(inputVector, interval);
    EXPECT_EQ(inputVector, adjustedVector);
  }

  // Test case with no maximums defined.
  {
    QS::PluginEntity::Properties propertiesNoMax{glbGoodProperties};
    QS::Actor actorNoMax(propertiesNoMax, "");
    float interval = 0.001;
    // Actor's default orientation is (1,0).
    Eigen::Vector2f inputVector(-50000000.0, 0);
    auto adjustedVector = actorNoMax.adjustVectorToMaximums(
      inputVector, interval);
    EXPECT_EQ(inputVector, adjustedVector);
  }

  // Basic capping of maximum speed.
  {
    float interval = 1.0;
    Eigen::Vector2f inputVector(2.1,0);
    Eigen::Vector2f expectedVector(2.0, 0);
    auto adjustedVector = actor.adjustVectorToMaximums(inputVector, interval);
    EXPECT_EQ(expectedVector, adjustedVector)
      << "Expected: " << expectedVector.format(QS::prettyPrint)
      << ", Actual:" << adjustedVector.format(QS::prettyPrint);
  }

  // Test valid counter-clockwise rotation
  {
    float interval = 1.0;
    Eigen::Vector2f inputVector(0, 1.0);
    auto adjustedVector = actor.adjustVectorToMaximums(inputVector, interval);
    EXPECT_EQ(inputVector, adjustedVector);
  }

  // Test too-far counter-clockwise rotation is adjusted
  {
    float interval = 1.0;
    // 92 degrees
    Eigen::Vector2f inputVector(-0.0349, 0.9994);
    Eigen::Vector2f expectedVector(0.0, 1.0);
    auto adjustedVector = actor.adjustVectorToMaximums(inputVector, interval);
    EXPECT_TRUE(expectedVector.isApprox(adjustedVector, QS::FLOAT_TOLERANCE))
      << "Expected: " << expectedVector.format(QS::prettyPrint)
      << ", Actual:" << adjustedVector.format(QS::prettyPrint);
  }

  // Test valid clockwise rotation
  {
    float interval = 1000.0;
    Eigen::Vector2f inputVector(0, -1.0);
    auto adjustedVector = actor.adjustVectorToMaximums(inputVector, interval);
    EXPECT_EQ(inputVector, adjustedVector);
  }

  // Test too-far clockwise rotation is adjusted
  {
    float interval = 1.0;
    // 245 degrees
    Eigen::Vector2f inputVector(-0.4226, -0.9063);
    Eigen::Vector2f expectedVector(0.0, -1.0);
    auto adjustedVector = actor.adjustVectorToMaximums(inputVector, interval);
    EXPECT_TRUE(expectedVector.isApprox(adjustedVector, QS::FLOAT_TOLERANCE))
      << "Expected: " << expectedVector.format(QS::prettyPrint)
      << ", Actual:" << adjustedVector.format(QS::prettyPrint);
  }

  // Capping of maximum speed & rotation of vector with non-zero X and Y.
  {
    float interval = 1.0;
    Eigen::Vector2f inputVector(-2.1, 3.3);
    Eigen::Vector2f expectedVector(0, 2.0);
    auto adjustedVector = actor.adjustVectorToMaximums(inputVector, interval);
    EXPECT_TRUE(expectedVector.isApprox(adjustedVector, QS::FLOAT_TOLERANCE))
      << "Expected: " << expectedVector.format(QS::prettyPrint)
      << ", Actual:" << adjustedVector.format(QS::prettyPrint);
  }

  // Tests using 0.0 maximum speed (makes rotation irrelevant)
  {
    QS::PluginEntity::Properties zeroMaxSpeedProperties{glbGoodProperties};
    zeroMaxSpeedProperties.insert({"max speed", "0.0"});
    zeroMaxSpeedProperties.insert({"max rotation", std::to_string(M_PI / 2.0)});

    QS::Actor fastActor(zeroMaxSpeedProperties, "");

    float interval = 1.0;
    Eigen::Vector2f inputVector(-2.0, 1.0);
    Eigen::Vector2f expectedVector(0, 0);
    auto adjustedVector = fastActor.adjustVectorToMaximums(inputVector,
                                                           interval);
    EXPECT_EQ(expectedVector, adjustedVector)
      << "Expected: " << expectedVector.format(QS::prettyPrint)
      << ", Actual:" << adjustedVector.format(QS::prettyPrint);
  }

  // Tests using 0.0 maximum rotation
  {
    QS::PluginEntity::Properties zeroMaxRotationProperties{glbGoodProperties};
    zeroMaxRotationProperties.insert({"max speed", "2.0"});
    zeroMaxRotationProperties.insert({"max rotation", "0.0"});

    QS::Actor fastActor(zeroMaxRotationProperties, "");

    float interval = 1.0;

    // No rotation, but does allow speed adjustment
    Eigen::Vector2f inputVector(3.0, 0.0);
    Eigen::Vector2f expectedVector(2.0, 0.0);
    auto adjustedVector = fastActor.adjustVectorToMaximums(inputVector,
                                                           interval);
    EXPECT_TRUE(expectedVector.isApprox(adjustedVector, QS::FLOAT_TOLERANCE))
      << "Expected: " << expectedVector.format(QS::prettyPrint)
      << ", Actual:" << adjustedVector.format(QS::prettyPrint);

    // Attempted rotation is squashed and movement is directly ahead.
    inputVector << 0.0, 2.0;
    expectedVector << 2.0, 0.0;
    adjustedVector = fastActor.adjustVectorToMaximums(inputVector, interval);
    EXPECT_TRUE(expectedVector.isApprox(adjustedVector, QS::FLOAT_TOLERANCE))
      << "Expected: " << expectedVector.format(QS::prettyPrint)
      << ", Actual:" << adjustedVector.format(QS::prettyPrint);
  }
}

TEST_F(ActorTestFixture, testGetMass)
{
  EXPECT_FLOAT_EQ(0.2, myActor->getMass());
}

TEST_F(ActorTestFixture, getGetMaximumRotationSpeed)
{
  QS::PluginEntity::Properties properties{glbGoodProperties};
  properties.insert({"max rotation", "3.5"});

  QS::Actor actor(properties, "");
  EXPECT_FLOAT_EQ(3.5, actor.getMaximumRotationSpeed());
}

TEST_F(ActorTestFixture, getGetMaximumSpeed)
{
  QS::PluginEntity::Properties properties{glbGoodProperties};
  properties.insert({"max speed", "1.0"});

  QS::Actor actor(properties, "");
  EXPECT_FLOAT_EQ(1.0, actor.getMaximumSpeed());
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
  EXPECT_FLOAT_EQ(0.1, myActor->getRadius());
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
    EXPECT_TRUE(theExpectedPoint.isApprox(convertedPoint, QS::FLOAT_TOLERANCE))
    << "   World Point: (" << theWorldPoint.format(QS::prettyPrint) << ")"
    << std::endl
    << " Expected Point: (" << theExpectedPoint.format(QS::prettyPrint) << ")"
    << std::endl
    << "Converted Point: (" << convertedPoint.format(QS::prettyPrint) << ")"
    << std::endl;
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
