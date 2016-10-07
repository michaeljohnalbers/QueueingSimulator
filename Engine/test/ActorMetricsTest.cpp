/**
 * @file ActorMetricsTest.cpp
 * @brief Unit test of ActorMetrics class
 *
 * @author Michael Albers
 */

#include <sstream>
#include "gtest/gtest.h"
#include "Actor.h"
#include "ActorMetrics.h"

class ActorMetricsTestFixture : public ::testing::Test
{
  public:

  void SetUp() override
  {
    QS::PluginEntity::Properties properties{
      {"radius", "0.1"},
      {"mass", "0.2"},
      {"x", "0.0"},
      {"y", "0.0"}};

    myActor = new QS::Actor(properties);
  }

  void TearDown() override
  {
    delete myActor;
  }

  QS::Actor *myActor;
};

TEST_F(ActorMetricsTestFixture, grossDistance)
{
  QS::ActorMetrics actorMetrics(myActor);
  EXPECT_EQ(0.0, actorMetrics.getGrossDistanceMeters());
  actorMetrics.addGrossDistance(2.2);
  EXPECT_FLOAT_EQ(2.2, actorMetrics.getGrossDistanceMeters());
}

TEST_F(ActorMetricsTestFixture, netDistance)
{
  QS::ActorMetrics actorMetrics(myActor);
  EXPECT_EQ(0.0, actorMetrics.calculateNetDistance(myActor));

  myActor->setPosition({1.0, 1.0});
  EXPECT_FLOAT_EQ(std::sqrt(2.0), actorMetrics.calculateNetDistance(myActor));

  myActor->setPosition({3.553, -1.0});
  EXPECT_FLOAT_EQ(3.6910443, actorMetrics.calculateNetDistance(myActor));
}

TEST_F(ActorMetricsTestFixture, output)
{
  QS::ActorMetrics actorMetrics(myActor);
  std::ostringstream os;
  os << actorMetrics;
  EXPECT_FALSE(os.str().empty());
}
