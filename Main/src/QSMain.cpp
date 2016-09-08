/**
 * @file QSMain.h
 * @brief Contains 'main'
 *
 * @author Michael Albers
 */

#include <iostream>
#include "xercesc/util/PlatformUtils.hpp"
#include "xercesc/util/XMLString.hpp"
#include "ControlGUI.h"

XERCES_CPP_NAMESPACE_USE

int main(int argc, char **argv)
{
  // Just do this once for the entire program.
  int status = 0;
  try
  {
    XMLPlatformUtils::Initialize();

    status = QS::ControlGUI::run(argc, argv);
    XMLPlatformUtils::Terminate();
  }
  catch (const XMLException &exception)
  {
    char *transcodedError = XMLString::transcode(exception.getMessage());
    std::cerr << argv[0] << ": Fatal error initializing xerces: "
              << transcodedError << std::endl;
    XMLString::release(&transcodedError);
    status = 1;
  }
  
  return status;
}
