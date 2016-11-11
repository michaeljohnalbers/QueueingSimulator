/**
 * @file SensableTest.cpp
 * @brief Unit test of Sensable class
 *
 * @author Michael Albers
 */

#include <memory>
#include <vector>
#include "gtest/gtest.h"
#include "Actor.h"
#include "Exit.h"
#include "Sensable.h"
#include "TestUtils.h"

static float interval{.055};

GTEST_TEST(SensableTest, testConstruction)
{
  EXPECT_NO_THROW(QS::Sensable sensable({nullptr}, {}, interval));
}

GTEST_TEST(SensableTest, testCopyMove)
{
  std::shared_ptr<QS::Actor> actor{
    new QS::Actor(QS::TestUtils::getMinimalActorProperties(), "")};
  std::vector<const QS::Actor*> actors{actor.get()};

  std::shared_ptr<QS::Exit> exit{
    new QS::Exit(QS::TestUtils::getMinimalExitProperties(), "")};
  std::vector<const QS::Exit*> exits{exit.get()};

  QS::Sensable sensable(actors, exits, interval);

  std::vector<const QS::Actor*> emptyActors;
  std::vector<const QS::Exit*> emptyExits;


  // Test copy constructor
  QS::Sensable sensableCopy(sensable);
  EXPECT_EQ(sensable.getActors(), sensableCopy.getActors());
  EXPECT_EQ(sensable.getExits(), sensableCopy.getExits());

  // Test move constructor
  QS::Sensable sensableMove(std::move(sensableCopy));
  EXPECT_EQ(sensable.getActors(), sensableMove.getActors());
  EXPECT_EQ(sensable.getExits(), sensableMove.getExits());
}

GTEST_TEST(SensableTest, testActors)
{
  std::shared_ptr<QS::Actor> actor{
    new QS::Actor(QS::TestUtils::getMinimalActorProperties(), "")};
  std::vector<const QS::Actor*> actors{actor.get()};
  QS::Sensable sensable(actors, {}, interval);
  EXPECT_EQ(actors, sensable.getActors());
}

GTEST_TEST(SensableTest, testExits)
{
  std::shared_ptr<QS::Exit> exit{
    new QS::Exit(QS::TestUtils::getMinimalExitProperties(), "")};
  std::vector<const QS::Exit*> exits{exit.get()};
  QS::Sensable sensable({}, exits, interval);
  EXPECT_EQ(exits, sensable.getExits());
}

GTEST_TEST(SensableTest, getIntervalInSeconds)
{
  QS::Sensable sensable({nullptr}, {}, interval);
  EXPECT_EQ(interval, sensable.getIntervalInSeconds());
}
