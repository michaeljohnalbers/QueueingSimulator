/**
 * @file PropertyGeneratorTest.cpp
 * @brief Unit test of PropertyGenerator class
 *
 * @author Michael Albers
 */

#define _USE_MATH_DEFINES // For M_PI
#include <cmath>
#include <limits>
#include "gtest/gtest.h"
#include "Metrics.h"
#include "PropertyGenerator.h"
#include "SimulationEntityConfiguration.h"
#include "World.h"

static QS::Metrics glbMetrics;
static QS::World glbWorld(glbMetrics);

GTEST_TEST(PropertyGeneratorTest, construction)
{
  ASSERT_NO_THROW(QS::PropertyGenerator propertyGenerator(glbWorld));
}

GTEST_TEST(PropertyGeneratorTest, generation)
{
  glbWorld.setSeed(1);
  float width = 2345.1;
  float length = 4321.9;
  glbWorld.setDimensions(width, length);

  QS::PropertyGenerator propertyGenerator(glbWorld);

  // type/tag/source don't matter for this
  QS::SimulationEntityConfiguration entityConfig("", "", "");
  float goodPropertyValue = 34.54;
  entityConfig.addProperty("goodProp", std::to_string(goodPropertyValue));
  entityConfig.addProperty("nonFloat", "Hey Joe.");
  entityConfig.addProperty("invalidFloatRange",
                           std::to_string(std::numeric_limits<double>::max()));

  std::string input, newProperty, expectedProperty;

  //
  // Test pass-through
  //
  input = "4"; // Integer
  newProperty = propertyGenerator.generateProperty(input, entityConfig);
  EXPECT_EQ(input, newProperty);

  input = "0.123"; // Float
  newProperty = propertyGenerator.generateProperty(input, entityConfig);
  EXPECT_EQ(input, newProperty);

  input = "Nonsense"; // String
  newProperty = propertyGenerator.generateProperty(input, entityConfig);
  EXPECT_EQ(input, newProperty);


  //
  // Test processing (valid syntax)
  //
  {
    std::vector<std::tuple<std::string, std::string>> tests{
      std::make_tuple("4", "4.000000"),
      std::make_tuple("44", "44.000000"),
      std::make_tuple("45.012", "45.012001"), // Float slop
      std::make_tuple("4 + 4", "8.000000"),
      std::make_tuple("3 - 4", "-1.000000"),
      std::make_tuple("55 * 32", "1760.000000"),
      std::make_tuple("12 / 4", "3.000000"),
      std::make_tuple("(33 + 99)", "132.000000"),
      std::make_tuple("33 - 99 - 12", "-78.000000"),
      std::make_tuple("144 / 12 - 12", "0.000000"),
      std::make_tuple("156 - 12 / 12", "155.000000"),
      std::make_tuple("(156 - 12) / 12", "12.000000"),
      std::make_tuple("144 / 12 / 3", "4.000000"),
      std::make_tuple("(144 / 12) * (6 / 3)", "24.000000"),
      std::make_tuple("PI", std::to_string(M_PI)),
      std::make_tuple("pi", std::to_string(M_PI)), // Case insensitivity
      std::make_tuple("2 * PI", std::to_string(2 * M_PI)),
      std::make_tuple("WoRlD.width", std::to_string(width)),
      std::make_tuple("WoRlD.WidtH", std::to_string(width)),
      std::make_tuple("world.length", std::to_string(length)),
      std::make_tuple("world.lEngTh", std::to_string(length)),
      std::make_tuple("world.lEngTh", std::to_string(length)),
      std::make_tuple("this.goodProp", std::to_string(goodPropertyValue)),
      std::make_tuple("rAnD(1.0, 2.0)", "1.417022"),
      };

    for (auto test : tests)
    {
      input = std::get<0>(test);
      try
      {
        newProperty = propertyGenerator.generateProperty(
          ":" + input, entityConfig);
        EXPECT_EQ(std::get<1>(test), newProperty) << "Input: " << input;
      }
      catch (const std::exception &e)
      {
        ADD_FAILURE() << "Input: " << input << ", exception: " << e.what();
      }
      catch (...)
      {
        ADD_FAILURE() << "Input: " << input << ", unexpected exception.";
      }
    }
  }

  //
  // Test processing (invalid syntax)
  //
  {
    std::vector<std::string> tests{
      "",
      "3 + ",
      "3 + )",
      "3 + 5)",
      "3 * ",
      "3 * )",
      "3 * 5)",
      "(3 + 5",
      "3 + + 5",
      "3 - - 5",
      "3 * * 5",
      "3 / / 5",

      "somethingElse", // Invalid identifier
      "world", // No dot
      "world.", // No attribute
      "world.height", // Unknown attribute
      "world.x", // Unknown attribute
      "world.y", // Unknown attribute
      "worldXYZ.length", // world substring
      "this",
      "this.",
      "this.noSuchProperty",
      "this.nonFloat", // Not of floating point type
      "this.invalidFloatRange", // Out of float range
      "thisBB.goodProp", // this substring
      "rand",
      "rand(",
      "rand(,",
      "rand(,)",
      "rand(2.1,2.0)", // low > high
    };

    for (auto test : tests)
    {
      EXPECT_THROW(newProperty = propertyGenerator.generateProperty(
                     ":" + test, entityConfig), std::runtime_error)
        << "Input: " << test;
#if 0
      // Leaving this code here as it is very useful for debugging failures.
      try
      {
        newProperty = propertyGenerator.generateProperty(
          ":" + test, entityConfig);
        FAIL() << "Input: " << test;
      }
      catch (const std::runtime_error &e)
      {
        std::cout << e.what() << std::endl;
      }
      catch (const std::exception &e)
      {
        ADD_FAILURE() << "Exception type: " << typeid(e).name()
                      << ". " << e.what()
                      << std::endl;
      }
      catch (...)
      {
        ADD_FAILURE() << "Unknown exception type";
      }
#endif
    }
  }
}
