/**
 * @file Plugin.cpp
 * @brief Definition of Plugin
 *
 * @author Michael Albers
 */

#include <dlfcn.h>
#include <stdexcept>
#include "Plugin.h"

QS::Plugin::Plugin(std::shared_ptr<PluginDefinition> theDefinition) :
  myDefinition(theDefinition)
{
  openLibrary();

  myActorCreatorDestructor = loadCreatorDestructor<Actor>(
    myDefinition->getActorCreatorDestructor(), "Actor");

  myBehaviorSetCreatorDestructor = loadCreatorDestructor<BehaviorSet>(
    myDefinition->getBehaviorSetCreatorDestructor(), "Behavior Set");
    
  myBehaviorCreatorDestructor = loadCreatorDestructor<Behavior>(
    myDefinition->getBehaviorCreatorDestructor(), "Behavior");

  mySensorCreatorDestructor = loadCreatorDestructor<Sensor>(
    myDefinition->getSensorCreatorDestructor(), "Sensor");
}

QS::Plugin::~Plugin()
{
  // Don't care about return status.
  dlclose(myLibraryHandle);
}

template<class T>
T* QS::Plugin::create(const std::string &theType,
                      const std::map<std::string, std::string> &theProperties,
                      const std::string &theTag,
                      CreatorDestructor<T> theCreatorDestructor,
                      const std::string &theName)
{
  if (! theCreatorDestructor.first)
  {
    throw std::logic_error(
      "Unexpectedly attempting to create " + theName + " from plugin " +
      getName() + " with no creator function defined.");
  }

  return theCreatorDestructor.first(theType, theProperties, theTag);
}

QS::Actor* QS::Plugin::createActor(
  const std::string &theType,
  const std::map<std::string, std::string> &theProperties,
  const std::string &theTag)
{
  return create<Actor>(theType, theProperties, theTag, myActorCreatorDestructor,
                       "Actor");
}

QS::Behavior* QS::Plugin::createBehavior(
  const std::string &theType,
  const std::map<std::string, std::string> &theProperties,
  const std::string &theTag)
{
  return create<Behavior>(theType, theProperties, theTag,
                          myBehaviorCreatorDestructor, "Behavior");
}

QS::BehaviorSet* QS::Plugin::createBehaviorSet(
  const std::string &theType,
  const std::map<std::string, std::string> &theProperties,
  const std::string &theTag)
{
  return create<BehaviorSet>(theType, theProperties, theTag,
                             myBehaviorSetCreatorDestructor, "BehaviorSet");
}

QS::Sensor* QS::Plugin::createSensor(
  const std::string &theType,
  const std::map<std::string, std::string> &theProperties,
  const std::string &theTag)
{
  return create<Sensor>(theType, theProperties, theTag,
                        mySensorCreatorDestructor, "Sensor");
}

template<class T>
void QS::Plugin::destroy(T *theObject,
                         CreatorDestructor<T> theCreatorDestructor,
                         const std::string &theName) const
{
  if (! theCreatorDestructor.second)
  {
    throw std::logic_error(
      "Unexpectedly attempting to destroy " + theName + " from plugin " +
      getName() + " with no destructor function defined.");
  }

  theCreatorDestructor.second(theObject);
}

void QS::Plugin::destroyActor(QS::Actor *theActor) const
{
  destroy<Actor>(theActor, myActorCreatorDestructor, "Actor");
}

void QS::Plugin::destroyBehavior(QS::Behavior *theBehavior) const
{
  destroy<Behavior>(theBehavior, myBehaviorCreatorDestructor, "Behavior");
}

void QS::Plugin::destroyBehaviorSet(QS::BehaviorSet *theBehaviorSet) const
{
  destroy<BehaviorSet>(theBehaviorSet, myBehaviorSetCreatorDestructor,
                       "BehaviorSet");
}

void QS::Plugin::destroySensor(QS::Sensor *theSensor) const
{
  destroy<Sensor>(theSensor, mySensorCreatorDestructor, "Sensor");
}

std::shared_ptr<QS::PluginDefinition> QS::Plugin::getDefinition() const noexcept
{
  return myDefinition;
}

std::string QS::Plugin::getName() const noexcept
{
  return myDefinition->getName();
}

template<class T>
QS::Plugin::CreatorDestructor<T> QS::Plugin::loadCreatorDestructor(
  PluginDefinition::CreatorDestructorPair theCreatorDestructor,
  const std::string &theType)
{
  std::string creatorName, destructorName;
  std::tie(creatorName, destructorName) = theCreatorDestructor;

  CreatorDestructor<T> creatorDestructor;

  if (! creatorName.empty())
  {
    auto creatorFunction = reinterpret_cast<CreatorFunction<T>>(
      ::dlsym(myLibraryHandle, creatorName.c_str()));

    if (NULL == creatorFunction)
    {
      throw std::runtime_error("Failed to load " + theType +
                               " creator function '" +
                               creatorName + "' from library " +
                               myDefinition->getLibrary() + ": " +
                               ::dlerror() + ".");
    }

    creatorDestructor.first = creatorFunction;

    auto destructorFunction = reinterpret_cast<DestructorFunction<T>>(
      ::dlsym(myLibraryHandle, destructorName.c_str()));

    if (NULL == destructorFunction)
    {
      throw std::runtime_error("Failed to load " + theType +
                               " destructor function '" +
                               destructorName + "' from library " +
                               myDefinition->getLibrary() + ": " +
                               ::dlerror() + ".");
    }

    creatorDestructor.second = destructorFunction;
  }
  return creatorDestructor;
}

void QS::Plugin::openLibrary()
{
  std::string libraryPath =
    myDefinition->getDirectory() + "/" + myDefinition->getLibrary();

  myLibraryHandle = ::dlopen(libraryPath.c_str(),
                             RTLD_LAZY | RTLD_GLOBAL);
  if (NULL == myLibraryHandle)
  {
    throw std::runtime_error("Failed to open library '" + libraryPath +
                             "' for plugin " + myDefinition->getName() +
                             ": " + ::dlerror() + ".");
  }
}
