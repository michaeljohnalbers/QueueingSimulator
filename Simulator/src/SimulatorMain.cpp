/**
 * @file SimulatorMain.cpp
 * @brief Main function for Queueing Simulator.
 * @author Michael Albers
 */

//#include <CommandLineOptionsProcessor.h>
#include <cstdint>
#include <iostream>
#include <string>
//#include <World.h>

#include <Lattice.h>
#include <Bucket.h>
#include <Individual.h>
#include <NearestN.h>
#include <Logger.h>
#include <ReBucketer.h>

/**
 * Prints help information.
 */
static void help();

/** Name of the program (argv[0]).*/
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

  // std::cout << "Beginning execution of " << glbProgramName << "." << std::endl;

  // CommandLineOptionsProcessor commandLineOptionsProcessor;
  // int32_t firstArg = 0;
  // try
  // {
  //   int32_t result = commandLineOptionsProcessor.processOptions(
  //     argc, argv, firstArg);

  //   if (1 == result)
  //   {
  //     help();
  //     std::exit(0);
  //   }
  //   else if ((argc - firstArg) != 2)
  //   {
  //     throw std::logic_error("Incorrect number of arguments provided.");
  //   }
  // }
  // catch (const std::logic_error &exception)
  // {
  //   if (exception.what() != std::string())
  //   {
  //     std::cerr << glbProgramName << ": " << exception.what()
  //       	<< std::endl;
  //   }
  //   std::cerr << "Usage: " << glbProgramName
  //             << " [OPTIONS]... [CONFIGURATION] [OUTPUT FILE]"
  //             << std::endl;
  //   std::cerr << "Try '" << argv[0] << " --help' for more information."
  //             << std::endl;
  //   std::exit(1);
  // }

  // std::cout << "Queue Simulator program successfully "
  //           << "parsed command line options." << std::endl;

  // std::string configuration(argv[firstArg]);
  // std::string outputFile(argv[firstArg+1]);

  // World *world = nullptr;
  // if (commandLineOptionsProcessor.getInputOption() ==
  //     CommandLineOptionsProcessor::INPUT_FILE)
  // {
  //   std::cout << "Creating simulation world based on input file "
  //             << commandLineOptionsProcessor.getInputFile()
  //             << "." << std::endl;
  //   world = new World(commandLineOptionsProcessor.getInputFile(), outputFile);
  // }
  // else
  // {
  //   std::cout << "Creating simulation world based on command line options."
  //             << std::endl;

  //   int32_t numberActors = std::atoi(
  //     commandLineOptionsProcessor.getNumberActors().c_str());
  //   const std::string &worldSize = commandLineOptionsProcessor.getWorldSize();
  //   auto xPos = worldSize.find("x");
  //   int32_t numberRows = std::atoi(worldSize.substr(0, xPos).c_str());
  //   int32_t numberColumns = std::atoi(worldSize.substr(xPos+1).c_str());;
  //   int32_t seed = std::atoi(
  //     commandLineOptionsProcessor.getSeed().c_str());

  //   world = new World(
  //     numberActors, numberRows, numberColumns, seed, outputFile);
  // }

  // world->runSimulation();

  // delete world;

  // std::cout << "End of execution of " << glbProgramName << "." << std::endl;

  return 0;
};

void help()
{
  std::cout << "Usage: " << glbProgramName << " [OPTIONS]... "
	    << "[CONFIGURATION] [OUTPUT FILE]" << std::endl
	    << std::endl
	    << "OPTIONS"
	    << std::endl
	    << "--actors=NUMBER      Specify number of actors to use.\n"
	    << "                     Used in conjunction with --world-size\n"
	    << "                     and --seed.\n"
	    << std::endl
	    << "--help               Print this help and exit."
	    << std::endl
	    << "--input-file=FILE    Use an input file to define world and\n"
	    << "                     actors."
	    << std::endl
	    << "--seed=SEED          Seed value for random number generation.\n"
	    << "                     Used in conjunction with --actors and\n"
	    << "                     --world-size."
	    << std::endl
	    << "--world-size=SIZE    Specify world size. Format: XxY.\n"
	    << "                     Example: 500x700. Used in conjunction\n"
	    << "                     with --actors and --seed."
	    << std::endl << std::endl
	    << "CONFIGURATION" << std::endl
	    << "Specify which simulator configuration to use (required):"
	    << std::endl
	    << "  serial     Fully serial simulation." << std::endl
	    << "  fork       Parallelize using multiple processes." << std::endl
	    << "  pthread    Parallelize using pthreads." << std::endl
	    << "  OpenMP     Parallelize using OpenMP." << std::endl
	    << std::endl << std::endl
	    << "OUTPUT FILE" << std::endl
	    << "Name of outfile to which simulation run data will be written."
	    << std::endl
	    << std::endl;
}
