/**
 * @file SimulatorMain.cpp
 * @brief Main function for Queueing Simulator.
 * @author Michael Albers
 */

#include <cstdint>
#include <getopt.h>
#include <iostream>
#include <string>

#include <Lattice.h>
#include <Logger.h>

/**
 * Prints help information.
 */
static void help();

/** Name of the program (argv[0]). */
static std::string glbProgramName;

/**
 * Main function
 * @param argc Argument count
 * @param argv Argument list
 * @return Program status
 */
int main(int argc, char **argv)
{
  glbProgramName = argv[0];
  std::atexit(Logger::terminate);

  // TODO: signal handler

  while (true)
  {
    enum
    {
      HELP_OPTION = 1
    };

    static struct option longOptions[] =
    {
      {"help", no_argument, 0, HELP_OPTION},
      {0,0,0,0}
    };

    int optionIndex = 0;
    int c = getopt_long(argc, argv, "", longOptions, &optionIndex);

    if (-1 == c)
    {
      break;
    }

    switch (c)
    {
      case HELP_OPTION:
      case ':':
      case '?':
      default:
        help();
        return 0;
        break;
    }

  }

  if (argc - optind == 1)
  {
    Logger::log("Beginning execution of " + glbProgramName + ".");
    std::string configFileName(argv[optind]);
    Lattice simulator(configFileName);
    simulator.runSimulation();
  }
  else
  {
    help();
  }

  return 0;
};

void help()
{
  std::cout << "Usage: " << glbProgramName << " [OPTIONS]... "
	    << "[CONFIGURATION FILE]" << std::endl
	    << std::endl
	    << "OPTIONS"
	    << std::endl
	    << "--help               Print this help and exit."
	    << std::endl << std::endl
	    << "CONFIGURATION FIle" << std::endl
	    << "File containing configuration data. Format:"
    // TODO: fill in
	    << std::endl
	    << std::endl;
}
