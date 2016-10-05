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
#include "Sensable.h"

static float interval{.055};

GTEST_TEST(SensableTest, testConstruction)
{
  EXPECT_NO_THROW(QS::Sensable sensable({nullptr}, interval));
}

GTEST_TEST(SensableTest, testCopyMove)
{
  std::shared_ptr<QS::Actor> actor{
    new QS::Actor({{"mass", "1.0"}, {"radius", "1.0"},
                   {"x", "5.0"}, {"y", "5.0"}})
      };
  std::vector<const QS::Actor*> actors{actor.get()};
  QS::Sensable sensable({actor.get()}, interval);

  std::vector<const QS::Actor*> emptyActors;


  // Test copy constructor
  QS::Sensable sensableCopy(sensable);
  EXPECT_EQ(sensable.getActors(), sensableCopy.getActors());

  // Test move constructor
  QS::Sensable sensableMove(std::move(sensableCopy));
  EXPECT_EQ(sensable.getActors(), sensableMove.getActors());

  // Test copy assignment operator
  QS::Sensable sensableAssignCopy{emptyActors, interval};
  sensableAssignCopy = sensable;
  EXPECT_EQ(sensable.getActors(), sensableAssignCopy.getActors());

  // Test move assignment operator
  QS::Sensable sensableAssignMove{emptyActors, interval};
  sensableAssignMove = sensable;
  EXPECT_EQ(sensable.getActors(), sensableAssignMove.getActors());
}

GTEST_TEST(SensableTest, testActors)
{
  std::shared_ptr<QS::Actor> actor{
    new QS::Actor({{"mass", "1.0"}, {"radius", "1.0"},
                   {"x", "5.0"}, {"y", "5.0"}})
      };
  std::vector<const QS::Actor*> actors{actor.get()};
  QS::Sensable sensable({actor.get()}, interval);
  EXPECT_EQ(actors, sensable.getActors());
}

GTEST_TEST(SensableTest, getIntervalInSeconds)
{
  QS::Sensable sensable({nullptr}, interval);
  EXPECT_EQ(interval, sensable.getIntervalInSeconds());
}
