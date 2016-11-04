/**
 * @file WorldTest.cpp
 * @brief Unit test of World class
 *
 * @author Michael Albers
 */

#define _USE_MATH_DEFINES // For M_PI
#include <cmath>
#include <memory>
#include "gtest/gtest.h"
#include "Actor.h"
#include "Metrics.h"
#include "World.h"

static QS::Metrics glbMetrics;

GTEST_TEST(WorldTest, construction)
{
  ASSERT_NO_THROW(QS::World world(glbMetrics));
}

GTEST_TEST(WorldTest, addActor)
{
  QS::PluginEntity::Properties properties{
    {"radius", "0.1"},
    {"mass", "0.2"},
    {"x", "0.0"},
    {"y", "0.0"}};

  QS::World world(glbMetrics);
  world.setDimensions(10, 10);

  std::vector<std::shared_ptr<QS::Actor>> actors;

  // Test just inside each corner of the world.
  {
    std::shared_ptr<QS::Actor> actor( new QS::Actor(properties, ""));
    actors.push_back(actor);
    actor->setPosition({0.1, 0.1});
    EXPECT_NO_THROW(world.addActor(actor.get()));
  }
  {
    std::shared_ptr<QS::Actor> actor( new QS::Actor(properties, ""));
    actors.push_back(actor);
    actor->setPosition({9.9, 0.1});
    EXPECT_NO_THROW(world.addActor(actor.get()));
  }
  {
    std::shared_ptr<QS::Actor> actor( new QS::Actor(properties, ""));
    actors.push_back(actor);
    actor->setPosition({9.9, 9.9});
    EXPECT_NO_THROW(world.addActor(actor.get()));
  }
  {
    std::shared_ptr<QS::Actor> actor(new QS::Actor(properties, ""));
    actors.push_back(actor);
    actor->setPosition({0.1, 9.9});
    EXPECT_NO_THROW(world.addActor(actor.get()));
  }

  // Test just outside the world
  {
    QS::Actor actor(properties, "");
    actor.setPosition({0.0, 0.1}); // Bad X (low), good y
    EXPECT_THROW(world.addActor(&actor), std::logic_error);
    actor.setPosition({10.0, 0.1}); // Bad X (high), good y
    EXPECT_THROW(world.addActor(&actor), std::logic_error);
    actor.setPosition({0.1, 0.0}); // Good X, bad y (low)
    EXPECT_THROW(world.addActor(&actor), std::logic_error);
    actor.setPosition({0.1, 10.0}); // Good X, bad y (high)
    EXPECT_THROW(world.addActor(&actor), std::logic_error);
  }

  // Test adding duplicate actor
  EXPECT_THROW(world.addActor(actors.back().get()), std::logic_error);

  // Test actor overlap
  {
    std::shared_ptr<QS::Actor> actor1(new QS::Actor(properties, ""));
    actors.push_back(actor1);
    actor1->setPosition({5.0, 5.0});

    auto newProperties{properties};
    newProperties["radius"] = "1.0";
    std::shared_ptr<QS::Actor> actor2(new QS::Actor(newProperties, ""));
    actors.push_back(actor2);

    EXPECT_NO_THROW(world.addActor(actor1.get()));

    // Actors in exact same spot.
    actor2->setPosition(actor1->getPosition());
    EXPECT_THROW(world.addActor(actor2.get()), std::logic_error);

    // Move actor2 just a little
    actor2->setPosition({actor1->getPosition().x() + 1.0,
          actor1->getPosition().y()});
    EXPECT_THROW(world.addActor(actor2.get()), std::logic_error);

    // Check when their circumferences just touch
    actor2->setPosition({actor1->getPosition().x(),
          actor1->getPosition().y() + actor1->getRadius()});
    EXPECT_THROW(world.addActor(actor2.get()), std::logic_error);
  }

  // Test that getActors and getActorsInWorld are the same.
  {
    auto actors = world.getActors();
    auto actorsInWorld = world.getActorsInWorld();

    ASSERT_EQ(actors.size(), actorsInWorld.size());

    for (auto ii = 0u; ii < actors.size(); ++ii)
    {
      EXPECT_EQ(actors[ii], actorsInWorld[ii]) << "ii == " << ii;
    }
  }
}

GTEST_TEST(WorldTest, convertPointToWorld)
{
  QS::PluginEntity::Properties properties{
    {"radius", "0.1"},
    {"mass", "0.2"},
    {"x", "0.0"},
    {"y", "0.0"}};

  QS::Actor actor{properties, ""};

  Eigen::Vector2f localPoint(1,0);
  Eigen::Vector2f expectedPoint(1,0);
  Eigen::Vector2f actualPoint;

  actualPoint = QS::World::convertPointToWorld(&actor, localPoint);
  EXPECT_EQ(expectedPoint, actualPoint);

  actor.setOrientation(M_PI);
  expectedPoint << -1, 0;
  actualPoint = QS::World::convertPointToWorld(&actor, localPoint);
  float tolerance = 0.0001;
  EXPECT_NEAR(expectedPoint.x(), actualPoint.x(), tolerance) << actualPoint;
  EXPECT_NEAR(expectedPoint.y(), actualPoint.y(), tolerance) << actualPoint;
}

GTEST_TEST(WorldTest, randomNumbers)
{
  QS::World world(glbMetrics);
  world.setSeed(45);
  std::uniform_real_distribution<float> distribution(3.0, 4.5);
  float rng = world.getRandomNumber(distribution);

  // Not much to test since getRandomNumber is just a call to the RNG engine.
  // All the real work is done by the C++ library.
  EXPECT_FLOAT_EQ(4.4835172, rng);
}
