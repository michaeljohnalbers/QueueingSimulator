/**
 * @file ShaderTest.cpp
 * @brief Unit test for the Shader class
 *
 * @author Michael Albers
 */

#include <string>
#include "Shader.h"
#include "VisualizationInitialization.h"
#include "gtest/gtest.h"

#define CLASS_NAME ShaderTest
#include "ShaderSetup.h"
std::string CLASS_NAME::myLatestError;
#undef CLASS_NAME

static std::string goodShaderSource =
  R"delim(
#version 430 core
void main(){}
    )delim";

static std::string badShaderSource =
  R"delim(
#version 430 core
void main(){}
Syntax Error
    )delim";

TEST_F(ShaderTest, testShader)
{
  GLuint shaderId;
  try
  {
    // Test compilation of valid shader
    QS::Shader goodShader(goodShaderSource, GL_VERTEX_SHADER);
    EXPECT_EQ(GL_TRUE, glIsShader(goodShader.getShader()));
    // Test converstion operator
    shaderId = goodShader;
    EXPECT_EQ(GL_TRUE, glIsShader(shaderId));
    EXPECT_EQ(shaderId, goodShader.getShader());
  }
  catch(const std::exception &exception)
  {
    FAIL() << exception.what();
  }
  catch (...)
  {
    FAIL();
  }

  // Test that object destruction deletes the shader.
  EXPECT_EQ(GL_FALSE, glIsShader(shaderId));

  // Test that errors are caught and the right exception type is thrown.
  try
  {
    QS::Shader badShader(badShaderSource, GL_VERTEX_SHADER);
    FAIL() << "Unexpectedly didn't throw exception.";
  }
  catch (const std::logic_error &exception)
  {
    EXPECT_STREQ("Failed to compile shader. Errors: (0) : "
                 "error C0000: syntax error, unexpected $end, "
                 "expecting \"::\" at token \"<EOF>\"\n",
                 exception.what());
  }
  catch (...)
  {
    FAIL();
  }

  // Test other types of shader.
  EXPECT_NO_THROW(QS::Shader(goodShaderSource, GL_FRAGMENT_SHADER));
  EXPECT_NO_THROW(QS::Shader(goodShaderSource, GL_TESS_CONTROL_SHADER));
  EXPECT_NO_THROW(QS::Shader(goodShaderSource, GL_TESS_EVALUATION_SHADER));
  EXPECT_NO_THROW(QS::Shader(goodShaderSource, GL_GEOMETRY_SHADER));
}
