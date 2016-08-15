/**
 * @file QSMain.h
 * @brief Contains 'main'
 *
 * @author Michael Albers
 */

#include <iostream>
#include "ControlGUI.h"

int main(int argc, char **argv)
{
  return QS::ControlGUI::run(argc, argv);
}
