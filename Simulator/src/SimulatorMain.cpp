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
  std::cout << "Usage: " << glbProgramName << " [OPTIONS...] "
            << "[CONFIGURATION FILE]"
            << std::endl << std::endl
            << "OPTIONS"
            << std::endl
            << "--help    Print this help and exit."
            << std::endl << std::endl
            << "CONFIGURATION FILE" << std::endl
            << "File containing configuration data. Format:" << std::endl
            << "<number individuals, int>" << std::endl
            << "<world length, int, meters> <world height, int, meters>"
            << std::endl
            << "<number of buckets, see below>" << std::endl
            << "<Neighbor search radius, float, meters>" << std::endl
            << "<Number of neighbors to find, int>" << std::endl
            << "<Random number seed, int>" << std::endl
            << "<Run Configuration, see below>" << std::endl
            << std::endl
            << "Number of Buckets:" << std::endl
            << "Can be one of the following: " << std::endl
            << " 1) An integer multiple of 2 or 3" << std::endl
            << " 2) \"auto\", matches buckets to HW concurrency" << std::endl
            << " 3) \"yx\", where y is an integer, " << std::endl
            << "    creates buckets equal to y * HW concurrency, " << std::endl
            << "    example: \"3x\"" << std::endl
            << std::endl
            << "Run Configuration:" << std::endl
            << "Can be one of:" << std::endl
            << " 1) \"simulation\" - Runs an actual simulation." << std::endl
            << " 2) \"benchmark\" - Runs in a mode to gauge how" << std::endl
            << "    well the local machine can handle the given" << std::endl
            << "    number of individuals."
            << std::endl
            << std::endl;
}
