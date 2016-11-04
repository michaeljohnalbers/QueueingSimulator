/**
 * @file TestPluginConfigData.h
 * @brief Contains XML data, in strings for testing Plugin config reading.
 *
 * @author Michael Albers
 */

std::string pluginConfigNominal =
  R"SENTINEL(<?xml version='1.0' encoding='UTF-8'?>
<Plugin name="TestPlugin" library="test-lib.so">
  <Actors creator="actorCreator" destructor="actorDestructor">
    <Actor name="Actor">
      <BehaviorSet name="Set1" />
    </Actor>
  </Actors>
  <BehaviorSets creator="behaviorSetCreator" destructor="behaviorSetDestructor">
    <BehaviorSet name="Set1">
      <Behavior name="Behavior" />
    </BehaviorSet>
  </BehaviorSets>
  <Behaviors creator="behaviorCreator" destructor="behaviorDestructor">
    <Behavior name="Behavior" />
  </Behaviors>
  <Sensors creator="sensorCreator" destructor="sensorDestructor">
    <Sensor name="Sensor"/>
  </Sensors>
  <Exits creator="exitCreator" destructor="exitDestructor">
    <Exit name="Exit"/>
  </Exits>
</Plugin>
)SENTINEL";

std::string pluginConfigMalFormed =
  R"SENTINEL(<?xml version='1.0' encoding='UTF-8'?>
<Pluginfldjlfdkj name="BasicPlugin" library="libqs-basic-plugin.so">
</Plugin>
)SENTINEL";
