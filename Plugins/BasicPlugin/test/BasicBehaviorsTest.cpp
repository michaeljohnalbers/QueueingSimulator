/**
 * @file BasicBehaviorsTest.cpp
 * @brief Unit tests for BasicBehaviors class
 *
 * @author Michael Albers
 */

#include "gtest/gtest.h"
#include "Actor.h"
#include "BasicBehaviors.h"
#include "EigenHelper.h"
#include "TestUtils.h"

GTEST_TEST(BasicBehaviorsTest, arrival)
{
  QS::Actor actor(QS::TestUtils::getMinimalActorProperties(), "");
  actor.setPosition({0,0});

  Eigen::Vector2f position(5.0, 5.0);

  // Test invalid slowing radius.
  EXPECT_THROW(QS::BasicBehaviors::arrival(&actor, position, -1.0),
               std::invalid_argument);

  // Test that zero is OK
  EXPECT_NO_THROW(QS::BasicBehaviors::arrival(&actor, position, 0.0));

  // Test actor on the position.
  actor.setPosition({5.0, 5.0});
  Eigen::Vector2f expectedForce = {0,0};
  Eigen::Vector2f actualForce = QS::BasicBehaviors::arrival(
    &actor, position, 1.0);
  EXPECT_EQ(expectedForce, actualForce)
    << "Actual: " << actualForce.format(QS::EigenHelper::prettyPrint);

  // Test actor away from the position, outside the radius.
  actor.setPosition({0.0, 0.0});
  expectedForce = {12.727921485900879, 12.727921485900879};
  actualForce = QS::BasicBehaviors::arrival(&actor, position, 1.0);
  EXPECT_EQ(expectedForce, actualForce)
    << "Actual: " << actualForce.format(QS::EigenHelper::prettyPrint);

  // Test actor away from the position, outside the radius.
  actor.setPosition({10.0, 10.0});
  expectedForce = {-12.727921485900879, -12.727921485900879};
  actualForce = QS::BasicBehaviors::arrival(&actor, position, 1.0);
  EXPECT_EQ(expectedForce, actualForce)
    << "Actual: " << actualForce.format(QS::EigenHelper::prettyPrint);

  // Test actor away from the position, inside the radius.
  actor.setPosition({5.0, 4.0});
  expectedForce = {0, 6};
  actualForce = QS::BasicBehaviors::arrival(&actor, position, 3.0);
  EXPECT_EQ(expectedForce, actualForce)
    << "Actual: " << actualForce.format(QS::EigenHelper::prettyPrint);

  // Test actor away from the position, inside the radius.
  actor.setPosition({5.0, 6.0});
  expectedForce = {0.0, -6.0};
  actualForce = QS::BasicBehaviors::arrival(&actor, position, 3.0);
  EXPECT_EQ(expectedForce, actualForce)
    << "Actual: " << actualForce.format(QS::EigenHelper::prettyPrint);
}

GTEST_TEST(BasicBehaviorsTest, evade)
{
  QS::Actor actor(QS::TestUtils::getMinimalActorProperties(), "");
  actor.setPosition({0,0});

  QS::Actor actor2(QS::TestUtils::getMinimalActorProperties(), "");
  actor2.setPosition({0.0, 3.0});
  actor2.setVelocity({0.0, 2.0});

  // Since evade is flee based on a predicted position of where the 2nd
  // Actor will be the tests are fairly simple.
  Eigen::Vector2f predictedPosition(0.0, 3.0 + (2.0/3.0));
  Eigen::Vector2f expectedForce =
    QS::BasicBehaviors::flee(&actor, predictedPosition);
  Eigen::Vector2f actualForce = QS::BasicBehaviors::evade(&actor, &actor2);
  EXPECT_EQ(expectedForce, actualForce)
    << "Expected: " << expectedForce.format(QS::EigenHelper::prettyPrint)
    << ", Actual: " << actualForce.format(QS::EigenHelper::prettyPrint);
}

GTEST_TEST(BasicBehaviorsTest, flee)
{
  QS::Actor actor(QS::TestUtils::getMinimalActorProperties(), "");
  actor.setPosition({0,0});

  Eigen::Vector2f position(5.0, 5.0);

  // Test actor on the position.
  actor.setPosition({5.0, 5.0});
  Eigen::Vector2f expectedForce = {0,0};
  Eigen::Vector2f actualForce = QS::BasicBehaviors::flee(&actor, position);
  EXPECT_EQ(expectedForce, actualForce)
    << "Actual: " << actualForce.format(QS::EigenHelper::prettyPrint);

  // Test actor away from the position.
  actor.setPosition({0.0, 0.0});
  expectedForce = {-12.727921485900879, -12.727921485900879};
  actualForce = QS::BasicBehaviors::flee(&actor, position);
  EXPECT_EQ(expectedForce, actualForce)
    << "Actual: " << actualForce.format(QS::EigenHelper::prettyPrint);

  // Test actor away from the position.
  actor.setPosition({10.0, 10.0});
  expectedForce = {12.727921485900879, 12.727921485900879};
  actualForce = QS::BasicBehaviors::flee(&actor, position);
  EXPECT_EQ(expectedForce, actualForce)
    << "Actual: " << actualForce.format(QS::EigenHelper::prettyPrint);
}

GTEST_TEST(BasicBehaviorsTest, pursuit)
{
  QS::Actor actor(QS::TestUtils::getMinimalActorProperties(), "");
  actor.setPosition({0,0});

  QS::Actor actor2(QS::TestUtils::getMinimalActorProperties(), "");
  actor2.setPosition({0.0, 3.0});
  actor2.setVelocity({0.0, 2.0});

  // Since pursuit is seek based on a predicted position of where the 2nd
  // Actor will be the tests are fairly simple.
  Eigen::Vector2f predictedPosition(0.0, 3.0 + (2.0/3.0));
  Eigen::Vector2f expectedForce =
    QS::BasicBehaviors::seek(&actor, predictedPosition);
  Eigen::Vector2f actualForce = QS::BasicBehaviors::pursuit(&actor, &actor2);
  EXPECT_EQ(expectedForce, actualForce)
    << "Expected: " << expectedForce.format(QS::EigenHelper::prettyPrint)
    << ", Actual: " << actualForce.format(QS::EigenHelper::prettyPrint);
}

GTEST_TEST(BasicBehaviorsTest, seek)
{
  QS::Actor actor(QS::TestUtils::getMinimalActorProperties(), "");
  actor.setPosition({0,0});

  Eigen::Vector2f position(5.0, 5.0);

  // Test actor on the position.
  actor.setPosition({5.0, 5.0});
  Eigen::Vector2f expectedForce = {0,0};
  Eigen::Vector2f actualForce = QS::BasicBehaviors::seek(&actor, position);
  EXPECT_EQ(expectedForce, actualForce)
    << "Actual: " << actualForce.format(QS::EigenHelper::prettyPrint);

  // Test actor away from the position.
  actor.setPosition({0.0, 0.0});
  expectedForce = {12.727921485900879, 12.727921485900879};
  actualForce = QS::BasicBehaviors::seek(&actor, position);
  EXPECT_EQ(expectedForce, actualForce)
    << "Actual: " << actualForce.format(QS::EigenHelper::prettyPrint);

  // Test actor away from the position.
  actor.setPosition({10.0, 10.0});
  expectedForce = {-12.727921485900879, -12.727921485900879};
  actualForce = QS::BasicBehaviors::seek(&actor, position);
  EXPECT_EQ(expectedForce, actualForce)
    << "Actual: " << actualForce.format(QS::EigenHelper::prettyPrint);
}

GTEST_TEST(BasicBehaviorsTest, separation)
{
  QS::Actor actor(QS::TestUtils::getMinimalActorProperties(), "");
  actor.setPosition({5,5});

  QS::Actor actor1(QS::TestUtils::getMinimalActorProperties(), "");
  actor1.setPosition({3,5});
  QS::Actor actor2(QS::TestUtils::getMinimalActorProperties(), "");
  actor2.setPosition({7,5});
  QS::Actor actor3(QS::TestUtils::getMinimalActorProperties(), "");
  actor3.setPosition({5,3});
  QS::Actor actor4(QS::TestUtils::getMinimalActorProperties(), "");
  actor4.setPosition({5,7});

  const std::vector<const QS::Actor*> actors{
    &actor1, &actor2, &actor3, &actor4};

  // Test invalid separation distance.
  EXPECT_THROW(QS::BasicBehaviors::separation(&actor, {}, -1.0),
               std::invalid_argument);

  // Test that zero is allowed
  EXPECT_NO_THROW(QS::BasicBehaviors::separation(&actor, {}, 0.0));

  // Test no other Actors, no movement needed.
  Eigen::Vector2f expectedForce = {0,0};
  Eigen::Vector2f actualForce = QS::BasicBehaviors::separation(
    &actor, {}, 1.0);
  EXPECT_EQ(expectedForce, actualForce)
    << "Actual: " << actualForce.format(QS::EigenHelper::prettyPrint);

  // Test only actor in the list of actors, no movement needed.
  expectedForce = {0,0};
  actualForce = QS::BasicBehaviors::separation(&actor, {&actor}, 1.0);
  EXPECT_EQ(expectedForce, actualForce)
    << "Actual: " << actualForce.format(QS::EigenHelper::prettyPrint);

  // Test Actor in exactly the middle, no movement needed.
  expectedForce = {0,0};
  actualForce = QS::BasicBehaviors::separation(&actor, actors, 1.0);
  EXPECT_EQ(expectedForce, actualForce)
    << "Actual: " << actualForce.format(QS::EigenHelper::prettyPrint);

  // Test Actor near another Actor.
  actor.setPosition({5, 6});
  expectedForce = {0, -6.0};
  actualForce = QS::BasicBehaviors::separation(&actor, actors, 3.0);
  EXPECT_EQ(expectedForce, actualForce)
    << "Actual: " << actualForce.format(QS::EigenHelper::prettyPrint);
}
