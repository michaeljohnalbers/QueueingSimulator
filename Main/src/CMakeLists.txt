# Author: Michael Albers
# Description: Builds the main function for Queueing Simulator

add_executable(QueueingSimulator QSMain.cpp)
target_link_libraries(QueueingSimulator qs-engine)
target_link_libraries(QueueingSimulator qs-control)
target_link_libraries(QueueingSimulator ${GTKMM_LIBRARIES})
target_link_libraries(QueueingSimulator ${GTKMM_LIBRARIES})
target_link_libraries(QueueingSimulator ${XERCESC_LIBRARY})
target_include_directories(QueueingSimulator PRIVATE "../../Control/inc")
target_include_directories(QueueingSimulator PRIVATE "../../Engine/inc")

install(TARGETS QueueingSimulator
        RUNTIME DESTINATION ${QS_INSTALL_BIN_DIR})

# Install script
configure_file(../script/QueueingSimulator.sh.in
               ../script/QueueingSimulator.sh
	       @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/../script/QueueingSimulator.sh
        DESTINATION ${QS_INSTALL_BIN_DIR}
	PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)