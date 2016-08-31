/**
 * @file TestPluginConfigData.h
 * @brief Contains XML data, in strings for testing Plugin config reading.
 *
 * @author Michael Albers
 */

std::string pluginConfigNominal = "\
<?xml version='1.0' encoding='UTF-8'?>\n\
<Plugin name=\"TestPlugin\" library=\"test-lib.so\">\n\
  <Actors creator=\"actorCreator\" destructor=\"actorDestructor\">\n\
    <Actor name=\"Actor\">\n\
      <BehaviorSet name=\"Set1\" />\n\
    </Actor>\n\
  </Actors>\n\
  <BehaviorSets creator=\"behaviorSetCreator\" destructor=\"behaviorSetDestructor\">\n\
    <BehaviorSet name=\"Set1\">\n\
      <Behavior name=\"Behavior\" />\n\
    </BehaviorSet>\n\
  </BehaviorSets>\n\
  <Behaviors creator=\"behaviorCreator\" destructor=\"behaviorDestructor\">\n\
    <Behavior name=\"Behavior\" />\n\
  </Behaviors>\n\
  <Sensors creator=\"sensorCreator\" destructor=\"sensorDestructor\">\n\
    <Sensor name=\"Sensor\"/>\n\
  </Sensors>\n\
</Plugin>\n\
";

std::string pluginConfigMalFormed = "\
<?xml version='1.0' encoding='UTF-8'?>\n\
<Pluginfldjlfdkj name=\"BasicPlugin\" library=\"libqs-basic-plugin.so\">\n\
</Plugin>\n\
";
