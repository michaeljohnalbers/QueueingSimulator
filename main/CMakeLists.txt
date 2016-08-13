# Author: Michael Albers
# Description: Builds the main function for Queueing Simulator

add_executable(QueueingSimulator QSMain.cpp)
target_link_libraries(QueueingSimulator qs-engine)
