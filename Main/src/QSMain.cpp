/**
 * @file QSMain.h
 * @brief Contains 'main'
 *
 * @author Michael Albers
 */

#include <cstdlib>
#include <iostream>
#include "xercesc/util/PlatformUtils.hpp"
#include "xercesc/util/XMLString.hpp"
#include "ControlGUI.h"

XERCES_CPP_NAMESPACE_USE

int main(int argc, char **argv)
{
  int status = 1;
  try
  {
    XMLPlatformUtils::Initialize();

    auto baseDirEnvVar = std::getenv("QS_BASE_DIR");
    if (NULL == baseDirEnvVar)
    {
      throw std::runtime_error("QS_BASE_DIR environment variable is not set.");
    }

    status = QS::ControlGUI::run(argc, argv, baseDirEnvVar);

    XMLPlatformUtils::Terminate();
    status = 0;
  }
  catch (const std::runtime_error &exception)
  {
    std::cerr << argv[0] << ": Fatal error: " << exception.what() << std::endl;
  }
  catch (const std::logic_error &exception)
  {
    std::cerr << argv[0] << ": Fatal error: " << exception.what() << std::endl;
  }
  catch (const XMLException &exception)
  {
    char *transcodedError = XMLString::transcode(exception.getMessage());
    std::cerr << argv[0] << ": Fatal error initializing xerces: "
              << transcodedError << std::endl;
    XMLString::release(&transcodedError);
  }
  
  return status;
}
