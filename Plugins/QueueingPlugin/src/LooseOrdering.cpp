/**
 * @file LooseOrdering.cpp
 * @brief Definition of LooseOrdering
 *
 * @author Michael Albers
 */

#include "LooseOrdering.h"
#include "OrderedActor.h"
#include "OrderedExit.h"
#include "Sensable.h"

#include "Behavior.h"
#include "Sensor.h"

QS::LooseOrdering::LooseOrdering(const Properties &theProperties,
                                 const std::string &theTag) :
  BehaviorSet(theProperties, theTag)
{
}

Eigen::Vector2f QS::LooseOrdering::evaluate(const Actor *theActor,
                                            const Sensable &theSensable)
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
    // const QS::FindExitSensor *exitSensor =
    // dynamic_cast<const QS::FindExitSensor*>(getDependencies()[0].myEntity);


  Eigen::Vector2f steeringForce = BehaviorSet::evaluate(theActor, theSensable);

  return steeringForce;
}
