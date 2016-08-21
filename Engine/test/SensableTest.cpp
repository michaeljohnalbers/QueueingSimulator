/**
 * @file SensableTest.cpp
 * @brief Unit test of Sensable class
 *
 * @author Michael Albers
 */

#include <memory>
#include <vector>
#include "Actor.h"
#include "Sensable.h"
#include "gtest/gtest.h"

GTEST_TEST(SensableTest, testConstruction)
{
  EXPECT_NO_THROW(QS::Sensable sensable({nullptr}));
}

GTEST_TEST(SensableTest, testCopyMove)
{
  std::shared_ptr<QS::Actor> actor{
    new QS::Actor({{"mass", "1.0"}, {"radius", "1.0"}})
      };
  std::vector<const QS::Actor*> actors{actor.get()};
  QS::Sensable sensable({actor.get()});

  std::vector<const QS::Actor*> emptyActors;


  // Test copy constructor
  QS::Sensable sensableCopy(sensable);
  EXPECT_EQ(sensable.getActors(), sensableCopy.getActors());

  // Test move constructor
  QS::Sensable sensableMove(std::move(sensableCopy));
  EXPECT_EQ(sensable.getActors(), sensableMove.getActors());

  // Test copy assignment operator
  QS::Sensable sensableAssignCopy{emptyActors};
  sensableAssignCopy = sensable;
  EXPECT_EQ(sensable.getActors(), sensableAssignCopy.getActors());

  // Test move assignment operator
  QS::Sensable sensableAssignMove{emptyActors};
  sensableAssignMove = sensable;
  EXPECT_EQ(sensable.getActors(), sensableAssignMove.getActors());
}

GTEST_TEST(SensableTest, testActors)
{
  std::shared_ptr<QS::Actor> actor{
    new QS::Actor({{"mass", "1.0"}, {"radius", "1.0"}})
      };
  std::vector<const QS::Actor*> actors{actor.get()};
  QS::Sensable sensable({actor.get()});
  EXPECT_EQ(actors, sensable.getActors());
}
