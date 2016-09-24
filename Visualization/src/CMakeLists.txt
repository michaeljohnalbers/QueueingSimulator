# Author: Michael Albers
# Description: Builds the OpenGL visualizatoin library.

file(GLOB sources *.cpp)

# This bit of code takes the shaders in the 'shaders' directory and wraps them
# in a raw string literal so that 'std::string = #include "SomeShader.vert";
# brings in the shader source at compile time.
file(GLOB shaders ../shaders/*vert ../shaders/*frag)
foreach(shader ${shaders})
  file(READ ${shader} shaderSource)
  get_filename_component(shaderBasename ${shader} NAME)
  configure_file(../inc/ShaderTemplate.in ../inc/${shaderBasename} @ONLY)
endforeach()

add_library(qs-visualization SHARED ${sources})

target_include_directories(qs-visualization PRIVATE ../inc)
target_include_directories(qs-visualization PRIVATE
                           ../../Plugins/BasicPlugin/inc)
# For processed shaders
target_include_directories(qs-visualization PRIVATE
                           ${CMAKE_CURRENT_BINARY_DIR}/../inc)
target_include_directories(qs-visualization PRIVATE ${GLFW_INCLUDE_DIR})
target_include_directories(qs-visualization PRIVATE ${OPENGL_INCLUDE_DIR})
target_include_directories(qs-visualization PRIVATE ${GLM_INCLUDE_DIR})
target_link_libraries(qs-visualization ${OPENGL_LIBRARIES})
target_link_libraries(qs-visualization ${GLEW_LIBRARIES})
target_link_libraries(qs-visualization ${GLFW_LIBRARIES})
target_link_libraries(qs-visualization qs-basic-plugin)

install(TARGETS qs-visualization
        LIBRARY DESTINATION ${QS_INSTALL_LIB_DIR})
