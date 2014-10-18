/**
 * @file InputGenerator.cpp
 * @brief Tool used to generate input files for the Queueing Simulator.
 * @author Michael Albers
 */

#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <random>
#include <set>
#include <string>
#include <fstream>
#include <tuple>
#include <unistd.h>

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

  uint32_t numActors = std::atoi(argv[1]);
  int32_t seed = std::atoi(argv[2]);
  uint32_t worldSize = std::atoi(argv[2]);
  std::string fileName(argv[4]);

  std::ofstream outputFile;
  outputFile.exceptions(std::ios::badbit | std::ios::failbit |
			std::ios::eofbit);

  try
  {
    outputFile.open(fileName, std::ios::out);
  }
  catch (std::ios_base::failure &exception)
  {
    std::cerr << argv[0] << ": Failed to open output file \"" << fileName
	      << "\". " << exception.what()
	      << std::endl;
    std::exit(2);
  }

  std::cout << "Generating file with " << numActors << " actors."
	    << std::endl;

  try
  {
    outputFile << numActors << std::endl
	       << worldSize << " " << worldSize << std::endl;

    std::default_random_engine engine(seed);
    std::uniform_int_distribution<uint32_t> randomNumGenerator(0, worldSize);

    typedef std::tuple<int32_t, int32_t> Position;
    std::set<Position> usedPositions;
  
    for (uint32_t ii = 0; ii < numActors; ++ii)
    {
      bool goToNextActor = false;
      while (! goToNextActor)
      {
	uint32_t x = randomNumGenerator(engine);
	uint32_t y = randomNumGenerator(engine);
	Position actorPosition(x,y);

	if (usedPositions.find(actorPosition) == usedPositions.end())
	{
	  goToNextActor = true;
	  usedPositions.insert(actorPosition);

	  outputFile << (ii+1) << " " << x << " " << y << std::endl;
	}
      }
    }

    outputFile.close();
  }
  catch (std::ios_base::failure &exception)
  {
    std::cerr << argv[0] << ": Failed writing to output file \"" << fileName
	      << "\". " << exception.what()
	      << std::endl;
    unlink(fileName.c_str());
    std::exit(2);
  }

  return 0;
}
