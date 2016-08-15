# Author: Michael Albers
# Description: Builds the main function for Queueing Simulator

add_executable(QueueingSimulator QSMain.cpp)
target_link_libraries(QueueingSimulator qs-engine)
target_link_libraries(QueueingSimulator qs-control)
target_link_libraries(QueueingSimulator ${GTKMM_LIBRARIES})
target_include_directories(QueueingSimulator PRIVATE "../../Control/inc")
