# Author: Michael Albers
# Description: Delegation CMake file to install simulation directory & schema

file(GLOB simulationFiles Simulations/*.xml Simulations/*.xsd)

install(FILES ${simulationFiles}
        DESTINATION ${QS_INSTALL_SIMULATIONS_BASE_DIR})
