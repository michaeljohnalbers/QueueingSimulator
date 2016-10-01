/**
 * @file WorldTets.cpp
 * @brief Unit test of World class
 *
 * @author Michael Albers
 */

#define _USE_MATH_DEFINES // For M_PI
#include <cmath>
#include "gtest/gtest.h"
#include "Actor.h"
#include "World.h"

GTEST_TEST(WorldTest, construction)
{
  // World uses default constructor. So just make sure it exists.
  ASSERT_NO_THROW(QS::World world);
}

GTEST_TEST(WorldTest, convertPointToWorld)
{
  QS::PluginEntity::Properties properties{
    {"radius", "0.1"},
    {"mass", "0.2"},
    {"x", "0.0"},
    {"y", "0.0"}};

  QS::Actor actor{properties};

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
