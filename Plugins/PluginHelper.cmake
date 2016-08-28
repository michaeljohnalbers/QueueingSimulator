# Author: Michael Albers
# Description: Helper file to make creating plugins easier

# TODO: have user set target name & config name, then this will call add_library, include_directory, and install stuff below.

macro(QS_PLUGIN_INSTALL lib config)
  install(TARGETS ${lib}
    LIBRARY DESTINATION ${QS_INSTALL_PLUGIN_BASE_DIR}/BasicPlugin)
  install(FILES ../${config}
    DESTINATION ${QS_INSTALL_PLUGIN_BASE_DIR}/BasicPlugin)
endmacro()
