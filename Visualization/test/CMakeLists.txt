# Author: Michael Albers
# Description: Builds the Visualization gtest

file(GLOB sources *cpp)
link_directories(${GTEST_LIBRARY_DIR})
add_executable(VisualizationTest ${sources})

target_include_directories(VisualizationTest PRIVATE ${GLFW_INCLUDE_DIR})
target_include_directories(VisualizationTest PRIVATE ${OPENGL_INCLUDE_DIR})
target_include_directories(VisualizationTest PRIVATE ../inc ${GTEST_INCLUDE_DIR})

target_link_libraries(VisualizationTest qs-visualization ${GTEST_LIBRARY}
  ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW_LIBRARIES} ${XERCESC_LIBRARY})
