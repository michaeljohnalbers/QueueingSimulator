/**
 * @file SeparationTest.cpp
 * @brief Unit tests for Separation class
 *
 * @author Michael Albers
 */

#include "gtest/gtest.h"
#include "Actor.h"
#include "BasicBehaviors.h"
#include "EigenHelper.h"
#include "NearestN.h"
#include "Separation.h"
#include "Sensable.h"
#include "TestUtils.h"

GTEST_TEST(SeparationTest, class)
{
  QS::PluginEntity::Properties separationProperties;

  EXPECT_NO_THROW(QS::Separation(separationProperties, ""));
  QS::Separation separation(separationProperties, "");

  QS::PluginEntity::Properties nearestNProperties{
    {"N", "4"},
    {"radius", "10"}
  };

  QS::NearestN nearestN(nearestNProperties, "");
  QS::EntityDependency<QS::Sensor> nearestNSensorDependency{
    "", &nearestN, ""};
  separation.setDependencies({nearestNSensorDependency});

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

  std::vector<const QS::Exit*> exits{};
  QS::Sensable sensable(&actor, actors, exits, 0);
  nearestN.sense(sensable);

  // Separation is really just a wrapper around BasicBehaviors::separation,
  // which is tested independently. All that needs to be done here is to
  // make sure they are equivalent.
  Eigen::Vector2f actual = separation.evaluate(&actor);
  Eigen::Vector2f expected = QS::BasicBehaviors::separation(
    &actor, nearestN.getActors(), actor.getRadius() * 2);
  EXPECT_EQ(expected, actual)
    << "Expected: " << expected.format(QS::EigenHelper::prettyPrint)
    << "Actual: " << actual.format(QS::EigenHelper::prettyPrint);

}
