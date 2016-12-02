/**
 * @file SpatialHashTest.cpp
 * @brief Unit test of SpatialHash class
 *
 * @author Michael Albers
 */

#include "gtest/gtest.h"
#include "Actor.h"
#include "SpatialHash.h"
#include "TestUtils.h"

GTEST_TEST(SpatialHashTest, construction)
{
  {
    float width = 50;
    float length = 50;
    float averageSize = 1.5;
    QS::SpatialHash hash(width, length, averageSize);

    EXPECT_EQ(8u, hash.getCellSize());
    EXPECT_EQ(49u, hash.getNumberCells());
  }

  // Ensure that there is never less than 1 cell.
  {
    float width = 5.0;
    float length = 3.0;
    float averageSize = 3.0;
    QS::SpatialHash hash(width, length, averageSize);

    EXPECT_EQ(15u, hash.getCellSize());
    EXPECT_EQ(1u, hash.getNumberCells());
  }

  // Test invalid average size.
  EXPECT_THROW(QS::SpatialHash(3.3, 3.3, 0.0), std::invalid_argument);
  EXPECT_THROW(QS::SpatialHash(3.3, 3.3, -0.5), std::invalid_argument);
}

GTEST_TEST(SpatialHashTest, hashing)
{
  float width = 50.0;
  float length = 50.0;
  float averageSize = 1.0;

  auto actorProperties = QS::TestUtils::getMinimalActorProperties();
  actorProperties["radius"] = "1.0";
  QS::Actor actor1(actorProperties, "");
  QS::Actor actor2(actorProperties, "");
  QS::Actor actor3(actorProperties, "");
  QS::Actor actor4(actorProperties, "");

  std::vector<QS::Actor*> actors{&actor1, &actor2, &actor3, &actor4};

  // Test that Actors in individual cells are hashed correctly.
  {
    QS::SpatialHash hash(width, length, averageSize);

    actor1.setPosition({2.5, 2.5});
    hash.hashActor(&actor1);
    actor2.setPosition({7.5, 2.5});
    hash.hashActor(&actor2);
    actor3.setPosition({2.5, 7.5});
    hash.hashActor(&actor3);
    actor4.setPosition({7.5, 7.5});
    hash.hashActor(&actor4);

    for (auto actor : actors)
    {
      std::set<const QS::Actor*> cellmates =
        hash.getActors(actor->getPosition(), actor->getRadius());
      ASSERT_EQ(1u, cellmates.size());
      EXPECT_EQ(actor, *cellmates.begin());
    }
  }

  // Tests multiple Actors hashed to the same cell.
  {
    QS::SpatialHash hash(width, length, averageSize);

    // Intentionally putting actor in a position to where it's circumference
    // sits just under a cell boundary.
    actor1.setPosition({3.99999, 0.0});
    hash.hashActor(&actor1);
    actor2.setPosition({1.0, 1.0});
    hash.hashActor(&actor2);
    actor3.setPosition({2.0, 2.0});
    hash.hashActor(&actor3);

    // Put last Actor in next cell to the right (the cell that actor1 is
    // adjacent to)
    actor4.setPosition({7.5, 2.5});
    hash.hashActor(&actor4);
    
    std::set<const QS::Actor*> cellmates;

    cellmates = hash.getActors(actor1.getPosition(), actor1.getRadius());
    EXPECT_EQ(3u, cellmates.size());
    EXPECT_NE(cellmates.end(), cellmates.find(&actor1));
    EXPECT_NE(cellmates.end(), cellmates.find(&actor2));
    EXPECT_NE(cellmates.end(), cellmates.find(&actor3));

    cellmates = hash.getActors(actor2.getPosition(), actor2.getRadius());
    EXPECT_EQ(3u, cellmates.size());
    EXPECT_NE(cellmates.end(), cellmates.find(&actor1));
    EXPECT_NE(cellmates.end(), cellmates.find(&actor2));
    EXPECT_NE(cellmates.end(), cellmates.find(&actor3));

    cellmates = hash.getActors(actor3.getPosition(), actor3.getRadius());
    EXPECT_EQ(3u, cellmates.size());
    EXPECT_NE(cellmates.end(), cellmates.find(&actor1));
    EXPECT_NE(cellmates.end(), cellmates.find(&actor2));
    EXPECT_NE(cellmates.end(), cellmates.find(&actor3));

    cellmates = hash.getActors(actor4.getPosition(), actor4.getRadius());
    EXPECT_EQ(1u, cellmates.size());
    EXPECT_NE(cellmates.end(), cellmates.find(&actor4));
  }

  // Tests multiple Actors hashed to the same cell.
  {
    QS::SpatialHash hash(width, length, averageSize);

    // Intentionally putting actor in a position to where it's circumference
    // sits just under a cell boundary.
    actor1.setPosition({3.99999, 0.0});
    hash.hashActor(&actor1);
    actor2.setPosition({1.0, 1.0});
    hash.hashActor(&actor2);
    actor3.setPosition({2.0, 2.0});
    hash.hashActor(&actor3);

    // Put last Actor in next cell to the right (the cell that actor1 is
    // adjacent to)
    actor4.setPosition({7.5, 2.5});
    hash.hashActor(&actor4);
    
    std::set<const QS::Actor*> cellmates;

    cellmates = hash.getActors(actor1.getPosition(), actor1.getRadius());
    EXPECT_EQ(3u, cellmates.size());
    EXPECT_NE(cellmates.end(), cellmates.find(&actor1));
    EXPECT_NE(cellmates.end(), cellmates.find(&actor2));
    EXPECT_NE(cellmates.end(), cellmates.find(&actor3));

    cellmates = hash.getActors(actor2.getPosition(), actor2.getRadius());
    EXPECT_EQ(3u, cellmates.size());
    EXPECT_NE(cellmates.end(), cellmates.find(&actor1));
    EXPECT_NE(cellmates.end(), cellmates.find(&actor2));
    EXPECT_NE(cellmates.end(), cellmates.find(&actor3));

    cellmates = hash.getActors(actor3.getPosition(), actor3.getRadius());
    EXPECT_EQ(3u, cellmates.size());
    EXPECT_NE(cellmates.end(), cellmates.find(&actor1));
    EXPECT_NE(cellmates.end(), cellmates.find(&actor2));
    EXPECT_NE(cellmates.end(), cellmates.find(&actor3));

    cellmates = hash.getActors(actor4.getPosition(), actor4.getRadius());
    EXPECT_EQ(1u, cellmates.size());
    EXPECT_NE(cellmates.end(), cellmates.find(&actor4));
  }

  // Tests Actor sitting right on 4-corner cell boundary
  {
    QS::SpatialHash hash(width, length, averageSize);

    // Intentionally putting actor in a position to where it's circumference
    // sits just under a cell boundary.
    actor1.setPosition({5.0, 5.0});
    hash.hashActor(&actor1);
    actor2.setPosition({2.5, 2.5});
    hash.hashActor(&actor2);
    actor3.setPosition({7.5, 2.5});
    hash.hashActor(&actor3);
    actor4.setPosition({2.5, 7.5});
    hash.hashActor(&actor4);
    
    std::set<const QS::Actor*> cellmates;

    cellmates = hash.getActors(actor1.getPosition(), actor1.getRadius());
    EXPECT_EQ(4u, cellmates.size());
    EXPECT_NE(cellmates.end(), cellmates.find(&actor1));
    EXPECT_NE(cellmates.end(), cellmates.find(&actor2));
    EXPECT_NE(cellmates.end(), cellmates.find(&actor3));
    EXPECT_NE(cellmates.end(), cellmates.find(&actor4));

    cellmates = hash.getActors(actor2.getPosition(), actor2.getRadius());
    EXPECT_EQ(2u, cellmates.size());
    EXPECT_NE(cellmates.end(), cellmates.find(&actor1));
    EXPECT_NE(cellmates.end(), cellmates.find(&actor2));

    cellmates = hash.getActors(actor3.getPosition(), actor3.getRadius());
    EXPECT_EQ(2u, cellmates.size());
    EXPECT_NE(cellmates.end(), cellmates.find(&actor1));
    EXPECT_NE(cellmates.end(), cellmates.find(&actor3));

    cellmates = hash.getActors(actor4.getPosition(), actor4.getRadius());
    EXPECT_EQ(2u, cellmates.size());
    EXPECT_NE(cellmates.end(), cellmates.find(&actor1));
    EXPECT_NE(cellmates.end(), cellmates.find(&actor4));
  }
}

GTEST_TEST(SpatialHashTest, removeActor)
{
  float width = 50.0;
  float length = 50.0;
  float averageSize = 1.0;

  auto actorProperties = QS::TestUtils::getMinimalActorProperties();
  actorProperties["radius"] = "1.0";
  QS::Actor actor1(actorProperties, "");
  QS::Actor actor2(actorProperties, "");
  QS::Actor actor3(actorProperties, "");
  QS::Actor actor4(actorProperties, "");

  QS::SpatialHash hash(width, length, averageSize);

  actor1.setPosition({2.5, 2.6});
  hash.hashActor(&actor1);
  actor2.setPosition({2.5, 2.7});
  hash.hashActor(&actor2);
  actor3.setPosition({2.5, 2.8});
  hash.hashActor(&actor3);
  actor4.setPosition({2.5, 2.8});
  hash.hashActor(&actor4);

  hash.removeActor(&actor3);

  auto cellmates = hash.getActors(actor4.getPosition(), actor4.getRadius());
  EXPECT_EQ(3u, cellmates.size());
  EXPECT_NE(cellmates.end(), cellmates.find(&actor1));
  EXPECT_NE(cellmates.end(), cellmates.find(&actor2));
  EXPECT_NE(cellmates.end(), cellmates.find(&actor4));
}
