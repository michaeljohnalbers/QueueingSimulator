/**
 * @file InputGenerator.cpp
 * @brief Tool used to generate input files for the Queueing Simulator.
 * @author Michael Albers
 */

#include <ActorGenerator.h>
#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <string>

/**
 * Main function
 * @param argc Argument count
 * @param argv Argument list
 * @return Program status
 */
int main(int argc, char **argv)
{
  if (argc != 5)
  {
    std::cerr << "Usage: " << argv[0]
	      << " [Num actors] [world size (XxY)] [seed] [file name]"
	      << std::endl;
    std::exit(1);
  }

  int returnStatus = 0;

  int32_t numActors = std::atoi(argv[1]);
  std::string worldSize(argv[2]);
  int32_t seed = std::atoi(argv[3]);
  std::string fileName(argv[4]);

  auto xPos = worldSize.find("x");
  int32_t numberRows = std::atoi(worldSize.substr(0, xPos).c_str());
  int32_t numberColumns = std::atoi(worldSize.substr(xPos+1).c_str());;

  ActorGenerator generator(numActors,
                           numberRows,
                           numberColumns,
                           seed);
  try
  {
    generator.generateActors();
    generator.writeInputFile(fileName);
  }
  catch (const std::logic_error &exception)
  {
    std::cerr << "Error with creating input file: " << exception.what()
              << std::endl;
    returnStatus = 2;
  }

  return returnStatus;
}
