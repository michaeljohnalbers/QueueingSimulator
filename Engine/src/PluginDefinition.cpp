/**
 * @file PluginDefinition.cpp
 * @brief Definition of PluginDefinition
 *
 * @author Michael Albers
 */

#include <algorithm>
#include <stdexcept>
#include "PluginDefinition.h"

QS::PluginDefinition::PluginDefinition(const std::string &theName) :
  myName(theName)
{
}

void QS::PluginDefinition::addActorDefinition(
  const ActorDefinition &theActorDefinition)
{
  addItem(myActors, theActorDefinition, "actor");
}

template <class T>
void QS::PluginDefinition::addItem(std::vector<T> &theVector, const T &theItem,
                                   const std::string &theDescription)
{
  if (std::find(theVector.begin(), theVector.end(), theItem) != theVector.end())
  {
    throw std::invalid_argument("Cannot add duplicate " + theDescription +
                                " definition for '" + theItem.getName() + "'.");
  }
  theVector.push_back(theItem);
}

void QS::PluginDefinition::addBehaviorDefinition(
  const BehaviorDefinition &theBehaviorDefinition)
{
  addItem(myBehaviors, theBehaviorDefinition, "behavior");
}

void QS::PluginDefinition::addBehaviorSetDefinition(
  const BehaviorSetDefinition &theBehaviorSetDefinition)
{
  addItem(myBehaviorSets, theBehaviorSetDefinition, "behavior set");
}

void QS::PluginDefinition::addSensorDefinition(
  const SensorDefinition &theSensorDefinition)
{
  addItem(mySensors, theSensorDefinition, "sensor");
}

QS::PluginDefinition::CreatorDestructorPair
QS::PluginDefinition::getActorCreatorDestructor() const noexcept
{
  return myActorCreatorDestructor;
}

std::vector<QS::ActorDefinition> QS::PluginDefinition::getActorDefinitions()
  const noexcept
{
  return myActors;
}

QS::PluginDefinition::CreatorDestructorPair
QS::PluginDefinition::getBehaviorCreatorDestructor() const noexcept
{
  return myBehaviorCreatorDestructor;
}

std::vector<QS::BehaviorDefinition>
QS::PluginDefinition::getBehaviorDefinitions() const noexcept
{
  return myBehaviors;
}

QS::PluginDefinition::CreatorDestructorPair
QS::PluginDefinition::getBehaviorSetCreatorDestructor() const noexcept
{
  return myBehaviorSetCreatorDestructor;
}

std::vector<QS::BehaviorSetDefinition>
QS::PluginDefinition::getBehaviorSetDefinitions() const noexcept
{
  return myBehaviorSets;
}

std::string QS::PluginDefinition::getLibrary() const noexcept
{
  return myLibrary;
}

std::string QS::PluginDefinition::getName() const noexcept
{
  return myName;
}

QS::PluginDefinition::CreatorDestructorPair
QS::PluginDefinition::getSensorCreatorDestructor() const noexcept
{
  return mySensorCreatorDestructor;
}

std::vector<QS::SensorDefinition> QS::PluginDefinition::getSensorDefinitions()
  const noexcept
{
  return mySensors;
}

void QS::PluginDefinition::setActorCreatorDestructor(
  const std::string &theCreator,
  const std::string &theDestructor) noexcept
{
  myActorCreatorDestructor.first = theCreator;
  myActorCreatorDestructor.second = theDestructor;
}

void QS::PluginDefinition::setBehaviorCreatorDestructor(
  const std::string &theCreator,
  const std::string &theDestructor) noexcept
{
  myBehaviorCreatorDestructor.first = theCreator;
  myBehaviorCreatorDestructor.second = theDestructor;
}

void QS::PluginDefinition::setBehaviorSetCreatorDestructor(
  const std::string &theCreator,
  const std::string &theDestructor) noexcept
{
  myBehaviorSetCreatorDestructor.first = theCreator;
  myBehaviorSetCreatorDestructor.second = theDestructor;
}

void QS::PluginDefinition::setLibrary(const std::string &theLibrary) noexcept
{
  myLibrary = theLibrary;
}

void QS::PluginDefinition::setSensorCreatorDestructor(
  const std::string &theCreator,
  const std::string &theDestructor) noexcept
{
  mySensorCreatorDestructor.first = theCreator;
  mySensorCreatorDestructor.second = theDestructor;
}
