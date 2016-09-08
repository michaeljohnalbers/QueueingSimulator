/**
 * @file EngineTest.cpp
 * @brief Main file for tests for Engine
 *
 * @author Michael Albers
 */

#include "gtest/gtest.h"
#include "xercesc/util/PlatformUtils.hpp"

XERCES_CPP_NAMESPACE_USE

class XercesInit
{
  public:
  XercesInit() {XMLPlatformUtils::Initialize();}
  ~XercesInit() {XMLPlatformUtils::Terminate();}
};

int main(int argc, char **argv)
{
  XercesInit xi;
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
