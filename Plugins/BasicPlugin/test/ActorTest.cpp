/**
 * @file Actor.cpp
 * @brief Unit tests for Actor class
 *
 * @author Michael Albers
 */

#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include "Actor.h"
#include "gtest/gtest.h"

GTEST_TEST(ActorTest, testConstruction)
{
  // Test nominal construction.
  {
    std::map<std::string, std::string> properties;
    properties.insert({"mass", "0.1"});
    properties.insert({"radius", "0.1"});
    EXPECT_NO_THROW(QS::Actor actor(properties));
  }

  // Test construction just missing mass arugment
  {
    try
    {
      std::map<std::string, std::string> properties;
      properties.insert({"radius", "1.0"});
      QS::Actor actor(properties);
      FAIL();
    }
    catch (const std::invalid_argument &e)
    {
      EXPECT_EQ(std::string("Missing 'mass' property in Actor constructor."),
                e.what());
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
      QS::Actor actor(properties);
      FAIL();
    }
    catch (const std::invalid_argument &e)
    {
      EXPECT_EQ(std::string("Missing 'radius' property in Actor constructor."),
                e.what());
    }
    catch(...)
    {
      FAIL();
    }
  }

  // Test various invalid property values.
  {
    std::map<std::string, std::string> properties;
    properties.insert({"mass", "1.0"});
    properties.insert({"radius", "1.0"});
    std::vector<std::tuple<std::string, std::string, std::string>> badValues{
      std::make_tuple("mass", "0.0", "Invalid mass 0.000000, "
                      "must be >= 0 in Actor constructor."),
      std::make_tuple("mass", "-1.0", "Invalid mass -1.000000, "
                      "must be >= 0 in Actor constructor."),
      std::make_tuple("radius", "0.0", "Invalid radius 0.000000, "
                      "must be >= 0 in Actor constructor."),
      std::make_tuple("radius", "-1.0", "Invalid radius -1.000000, "
                      "must be >= 0 in Actor constructor."),
    };

    for(auto invalidTuple: badValues)
    {
      try
      {
        std::map<std::string, std::string> badProperties{properties};
        badProperties[std::get<0>(invalidTuple)] = std::get<1>(invalidTuple);
        QS::Actor actor(badProperties);
        FAIL();
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

GTEST_TEST(ActorTest, testGetMass)
{
  std::map<std::string, std::string> properties;
  properties.insert({"mass", "0.1"});
  properties.insert({"radius", "0.1"});
  QS::Actor actor(properties);
  EXPECT_FLOAT_EQ(0.1, actor.getMass());
}

GTEST_TEST(ActorTest, testGetSetPosition)
{
  std::map<std::string, std::string> properties;
  properties.insert({"mass", "0.1"});
  properties.insert({"radius", "0.1"});
  QS::Actor actor(properties);

  Eigen::Vector2f position(1.0, 20.0);
  EXPECT_NO_THROW(actor.setPosition(position));
  EXPECT_EQ(position, actor.getPosition());
}

GTEST_TEST(ActorTest, testGetRadius)
{
  std::map<std::string, std::string> properties;
  properties.insert({"mass", "0.1"});
  properties.insert({"radius", "0.2"});
  QS::Actor actor(properties);
  EXPECT_FLOAT_EQ(0.2, actor.getRadius());
}

GTEST_TEST(ActorTest, testGetSetVelocity)
{
  std::map<std::string, std::string> properties;
  properties.insert({"mass", "0.1"});
  properties.insert({"radius", "0.1"});
  QS::Actor actor(properties);

  Eigen::Vector2f velocity(3.2, 21.99);
  EXPECT_NO_THROW(actor.setVelocity(velocity));
  EXPECT_EQ(velocity, actor.getVelocity());
}