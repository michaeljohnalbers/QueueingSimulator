/**
 * @file NearestNTest.cpp
 * @brief Unit tests for NearestN class
 *
 * @author Michael Albers
 */

#include <random>
#include <stdexcept>
#include <vector>
#include "Eigen/Core"
#include "gtest/gtest.h"
#include "Actor.h"
#include "EigenHelper.h"
#include "NearestN.h"
#include "Sensable.h"
#include "TestUtils.h"

GTEST_TEST(NearestNTest, class)
{
  QS::PluginEntity::Properties properties{
    {"N", "3"},
    {"radius", "5.5"}
  };

  // Good construction
  ASSERT_NO_THROW(QS::NearestN(properties, ""));

  auto badProperties{properties};

  // Missing "N"
  badProperties.erase("N");
  EXPECT_THROW(QS::NearestN(badProperties, ""), std::invalid_argument);
  // Missing "radius"
  badProperties = properties;
  badProperties.erase("radius");
  EXPECT_THROW(QS::NearestN(badProperties, ""), std::invalid_argument);
  // Invalid "radius"
  badProperties = properties;
  badProperties["radius"] = -1.0;
  EXPECT_THROW(QS::NearestN(badProperties, ""), std::invalid_argument);

  QS::NearestN nearestN(properties, "");
  EXPECT_TRUE(nearestN.getActors().empty());
  EXPECT_EQ(3u, nearestN.getN());
  EXPECT_EQ(5.5, nearestN.getRadius());
}

GTEST_TEST(NearestNTest, sense)
{

  auto actorProperties{QS::TestUtils::getMinimalActorProperties()};

  QS::Actor currentActor(actorProperties, "");
  currentActor.setPosition({0.0, 0.0});

  std::vector<QS::Actor> actors;
  for (int ii = 0; ii < 10; ++ii)
  {
    actors.push_back(QS::Actor(actorProperties, ""));
    actors[ii].setPosition({ii+1.0, ii+1.0});
  }

  std::mt19937 rng(19586);
  std::shuffle(actors.begin(), actors.end(), rng);

  std::vector<const QS::Actor*> actorPtrs;
  for (auto &actor : actors)
  {
    actorPtrs.push_back(std::addressof(actor));
  }
  // In real situation, current Actor is also in the list.
  actorPtrs.push_back(&currentActor);

  QS::Sensable sensable(&currentActor, actorPtrs, {}, 0.0);

  // Test with empty actors
  {
    QS::Sensable sensable(&currentActor, {}, {}, 0.0);
    QS::PluginEntity::Properties properties{
      {"N", "3"},
      {"radius", "5.5"}
    };
    QS::NearestN nearestN(properties, "");

    nearestN.sense(sensable);
    auto actors = nearestN.getActors();
    EXPECT_TRUE(actors.empty());
  }

  auto checkActors = [](const std::vector<const QS::Actor*> theActors,
                        uint32_t theExpectedSize, const std::string &theHelper)
  {
    EXPECT_EQ(theExpectedSize, theActors.size()) << theHelper;

    // Make sure they are sorted in ascending distance.
    float ii = 1.0;
    for (auto actor : theActors)
    {
      Eigen::Vector2f expectedPosition{ii, ii};
      EXPECT_EQ(expectedPosition, actor->getPosition())
        << theHelper << " -- "
        << "expectedPosition: "
        << expectedPosition.format(QS::EigenHelper::prettyPrint)
        << ", actual: "
        << actor->getPosition().format(QS::EigenHelper::prettyPrint);
      ++ii;
    }
  };

  // Test with N > # actors, all within radius
  {
    QS::Sensable sensable(&currentActor, actorPtrs, {}, 0.0);
    QS::PluginEntity::Properties properties{
      {"N", std::to_string(actorPtrs.size() + 3)},
      {"radius", "10000.0"}
    };
    QS::NearestN nearestN(properties, "");

    nearestN.sense(sensable);
    auto actors = nearestN.getActors();
    checkActors(actors, actorPtrs.size()-1,
                "Line: " + std::to_string(__LINE__));
  }

  // Test with N < # actors, all within radius
  {
    QS::Sensable sensable(&currentActor, actorPtrs, {}, 0.0);
    QS::PluginEntity::Properties properties{
      {"N", "4"},
      {"radius", "10000.0"}
    };
    QS::NearestN nearestN(properties, "");

    nearestN.sense(sensable);
    auto actors = nearestN.getActors();
    checkActors(actors, 4, "Line: " + std::to_string(__LINE__));
  }

  // Test with N > # actors, some within radius
  {
    QS::Sensable sensable(&currentActor, actorPtrs, {}, 0.0);
    QS::PluginEntity::Properties properties{
      {"N", std::to_string(actorPtrs.size() + 5)},
      {"radius", "10.0"}
    };
    QS::NearestN nearestN(properties, "");

    nearestN.sense(sensable);
    auto actors = nearestN.getActors();
    checkActors(actors, 7, "Line: " + std::to_string(__LINE__));
  }

  // Test with N < # actors, <N within radius
  {
    QS::Sensable sensable(&currentActor, actorPtrs, {}, 0.0);
    QS::PluginEntity::Properties properties{
      {"N", "5"},
      {"radius", "4.0"}
    };
    QS::NearestN nearestN(properties, "");

    nearestN.sense(sensable);
    auto actors = nearestN.getActors();
    checkActors(actors, 2, "Line: " + std::to_string(__LINE__));
  }
}
