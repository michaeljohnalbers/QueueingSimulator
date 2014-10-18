/**
 * @file SimulatorMain.cpp
 * @brief Main function for Queueing Simulator.
 * @author Michael Albers
 */

#include <CommandLineOptionsProcessor.h>
#include <cstdint>
#include <getopt.h>
#include <iostream>
#include <string>

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

  CommandLineOptionsProcessor commandLineOptionsProcessor;
  int32_t firstArg = 0;
  try
  {
    int32_t result = commandLineOptionsProcessor.processOptions(
      argc, argv, firstArg);

    if (1 == result)
    {
      help();
      std::exit(0);
    }
    else if ((argc - firstArg) != 2)
    {
      throw std::logic_error("Incorrect number of arguments provided.");
    }
  }
  catch (const std::logic_error &exception)
  {
    if (exception.what() != std::string())
    {
      std::cerr << glbProgramName << ": " << exception.what()
		<< std::endl;
    }
    std::cerr << "Usage: " << glbProgramName
	      << " [OPTIONS]... [CONFIGURATION] [OUTPUT FILE]"
	      << std::endl;
    std::cerr << "Try '" << argv[0] << " --help' for more information."
	      << std::endl;
    std::exit(1);
  }

  std::string configuration(argv[firstArg]);
  std::string outputFile(argv[firstArg+1]);

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
	    << "                     Used in conjunction with --world-size.\n"
	    << std::endl
	    << "--help               Print this help and exit."
	    << std::endl
	    << "--input-file=FILE    Use an input file to define world and\n"
	    << "                     actors."
	    << std::endl
	    << "--world-size=SIZE    Specify world size. Format: XxY.\n"
	    << "                     Example: 500x700. Used in conjunction\n"
	    << "                     with --actors."
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
