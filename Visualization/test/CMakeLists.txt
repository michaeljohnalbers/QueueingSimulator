# Author: Michael Albers
# Description: Builds the Visualization gtest

file(GLOB sources *cpp)
link_directories(${GTEST_LIBRARY_DIR})
add_executable(VisualizationTest ${sources})

target_include_directories(VisualizationTest PRIVATE ${GLFW_INCLUDE_DIR})
target_include_directories(VisualizationTest PRIVATE ${OPENGL_INCLUDE_DIR})
target_include_directories(VisualizationTest PUBLIC ../inc ${GTEST_INCLUDE_DIR})

target_link_libraries(VisualizationTest ${GTEST_LIBRARY} qs-visualization)
target_link_libraries(VisualizationTest ${OPENGL_LIBRARIES})
target_link_libraries(VisualizationTest ${GLEW_LIBRARIES})
target_link_libraries(VisualizationTest ${GLFW_LIBRARIES})
