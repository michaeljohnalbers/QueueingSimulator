/**
 * @file ShaderProgramTest.cpp
 * @brief Unit test for the ShaderProgram class
 *
 * @author Michael Albers
 */

#include <string>
#include "Shader.h"
#include "ShaderProgram.h"
#include "VisualizationInitialization.h"
#include "gtest/gtest.h"


#define CLASS_NAME ShaderProgramTest
#include "ShaderSetup.h"
#undef CLASS_NAME

static std::string vertexShaderSource =
  R"delim(
#version 430 core
void main(){}
    )delim";

static std::string fragmentShaderSource = vertexShaderSource;

TEST_F(ShaderProgramTest, testShaderProgram)
{
  QS::Shader vertexShader(vertexShaderSource, GL_VERTEX_SHADER);
  QS::Shader fragmentShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

  GLuint program;
  {
    // Test construction
    QS::ShaderProgram shaderProgram;

    // Test program ID is valid
    ASSERT_EQ(GL_TRUE, glIsProgram(shaderProgram.getProgram()));

    // Test conversion function
    program = shaderProgram;
    EXPECT_EQ(GL_TRUE, glIsProgram(program));

    // Test adding/linking shaders
    shaderProgram.attachShader(vertexShader);
    shaderProgram.attachShader(fragmentShader);
    EXPECT_NO_THROW(shaderProgram.link());

    // Test link failure.
    QS::Shader tempShader(vertexShaderSource, GL_VERTEX_SHADER);
    shaderProgram.attachShader(tempShader);
    try
    {
      shaderProgram.link();
      FAIL() << "Unexpectedly didn't throw exception.";
    }
    catch (const std::logic_error &exception)
    {
      EXPECT_STREQ("Failed to link shader program. Errors: "
                   "Vertex info\n-----------\n0(3) : "
                   "error C1013: function \"main\" "
                   "is already defined at 0(3)\n",
                   exception.what());
    }
    catch (...)
    {
      FAIL();
    }

    // Test detach/relink
    shaderProgram.detachShader(vertexShader);
    EXPECT_NO_THROW(shaderProgram.link());

    // Not much to test for use, but do it anyway.
    shaderProgram.use();
    glUseProgram(0); // Clean up use
  }

  EXPECT_EQ(GL_FALSE, glIsProgram(program));
}
