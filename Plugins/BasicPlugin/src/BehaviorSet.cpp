/**
 * @file BehaviorSet.cpp
 * @brief Definition of BehaviorSet
 *
 * @author Michael Albers
 */

#include <stdexcept>
#include "Behavior.h"
#include "BehaviorSet.h"
#include "Sensable.h"
#include "Sensor.h"

QS::BehaviorSet::BehaviorSet(const Properties &theProperties,
                             const std::string &theTag) :
  PluginEntity(theProperties, theTag)
{
}

Eigen::Vector2f QS::BehaviorSet::evaluate(const Actor *theActor,
                                          const Sensable &theSensable)
{
  Eigen::Vector2f average(0.0, 0.0);
  auto dependencies = getDependencies();
  float count = 0;
  for (auto dependency : dependencies)
  {
    auto steeringForce = dependency.myEntity->evaluate(theActor);
    average += steeringForce;
    count++;
  }

  average /= count;
  return average;
}

void QS::BehaviorSet::populateSensors(const Sensable &theSensable) noexcept
{
  std::vector<EntityDependency<Behavior>> behaviors = getDependencies();
  for (EntityDependency<Behavior> behavior : behaviors)
  {
    std::vector<EntityDependency<Sensor>> sensorDependencies =
      behavior.myEntity->getDependencies();
    for (EntityDependency<Sensor> sensorDependency : sensorDependencies)
    {
      Sensor *sensor = sensorDependency.myEntity;
      sensor->sense(theSensable);
    }
  }
}
