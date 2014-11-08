/**
 * @file ActorGenerator.cpp
 * @brief Generates actors to populate the world.
 */

#include <ActorGenerator.h>
#include <fstream>
#include <random>
#include <set>
#include <sstream>

ActorGenerator::ActorGenerator(int32_t theNumberActors,
			       int32_t theNumberRows,
			       int32_t theNumberColumns,
			       int32_t theSeed) :
  myGenerationType(PARAMETERS),
  myNumberActors(theNumberActors),
  myNumberColumns(theNumberColumns),
  myNumberRows(theNumberRows),
  mySeed(theSeed)
{
}

ActorGenerator::ActorGenerator(const std::string &theInputFile) :
  myGenerationType(FILE),
  myInputFile(theInputFile)
{
}

const std::vector<Actor>& ActorGenerator::getActors() const
{
  return myActors;
}

int32_t ActorGenerator::getNumberColumns() const
{
  return myNumberColumns;
}

int32_t ActorGenerator::getNumberRows() const
{
  return myNumberRows;
}

void ActorGenerator::generateActors() throw (std::logic_error)
{
  if (FILE == myGenerationType)
  {
    generateActorsFromFile();
  }
  else
  {
    generateActorsFromParameters();
  }
}

void ActorGenerator::generateActorsFromFile() throw (std::logic_error)
{
  std::ifstream inputFile;
  inputFile.exceptions(std::ios::badbit | std::ios::failbit |
		       std::ios::eofbit);
  try
  {
    inputFile.open(myInputFile);
  }
  catch (std::ios_base::failure &exception)
  {
    std::ostringstream error;
    error << "Failed to open file \"" << myInputFile << "\". "
	  << exception.what();
    throw std::logic_error(error.str());
  }

  try
  {
    inputFile >> myNumberActors >> myNumberRows >> myNumberColumns;
    verifyActorsAndWorldSize();
    std::set<Actor::Position> usedPositions;
    for (int32_t ii = 0; ii < myNumberActors; ++ii)
    {
      int32_t rank, x, y;
      inputFile >> rank >> x >> y;
      Actor newActor(rank);
      Actor::Position position(x,y);
      if (usedPositions.find(position) == usedPositions.end())
      {
	newActor.setCurrentPosition(position);
	myActors.push_back(newActor);
	usedPositions.insert(position);
      }
      else
      {
	std::ostringstream error;
	error << "Actor with rank " << rank
	      << " is invalid, its x/y position is not unique.";
	throw std::logic_error(error.str());
      }
    }  
  }
  catch (std::ios_base::failure &exception)
  {
    std::ostringstream error;
    error << "Failed to read from file \"" << myInputFile << "\". "
	  << exception.what();
    throw std::logic_error(error.str());
  }
}

void ActorGenerator::generateActorsFromParameters() throw (std::logic_error)
{
  verifyActorsAndWorldSize();

  std::default_random_engine engine(mySeed);
  std::uniform_int_distribution<int32_t> randomNumGeneratorRow(
    0, myNumberRows-1);
  std::uniform_int_distribution<int32_t> randomNumGeneratorCol(
    0, myNumberColumns-1);

  std::set<Actor::Position> usedPositions;

  for (int32_t ii = 0; ii < myNumberActors; ++ii)
  {
    bool goToNextActor = false;
    while (! goToNextActor)
    {
      int32_t x = randomNumGeneratorRow(engine);
      int32_t y = randomNumGeneratorCol(engine);
      Actor::Position actorPosition(x,y);

      if (usedPositions.find(actorPosition) == usedPositions.end())
      {
	goToNextActor = true;
	usedPositions.insert(actorPosition);
	int32_t rank = ii+1;
	Actor newActor(rank);
	newActor.setCurrentPosition(actorPosition);
	myActors.push_back(newActor);
      }
    }
  }
}

void ActorGenerator::verifyActorsAndWorldSize() throw (std::logic_error)
{
  if (myNumberRows < 4)
  {
    throw std::logic_error("Invalid number of rows. Must be at least 4.");
  }

  if (myNumberColumns < 4)
  {
    throw std::logic_error("Invalid number of columns. Must be at least 4.");
  }

  int32_t worldSize = myNumberRows * myNumberColumns;
  // 15 is an arbitrary value to give the actors empty spots to move to.
  if (worldSize <= (myNumberActors + 15))
  {
    throw std::logic_error("The world is not large enough to hold all "
			   "the actors which suitable movement room.");
  }
}

void ActorGenerator::writeInputFile(const std::string &theFileName) const
  throw (std::logic_error)
{
  std::ofstream outputFile;
  outputFile.exceptions(std::ios::badbit | std::ios::failbit |
			std::ios::eofbit);

  try
  {
    outputFile.open(theFileName, std::ios::out);
  }
  catch (std::ios_base::failure &exception)
  {
    std::ostringstream error;
    error << "Failed to open file \"" << theFileName << "\". "
	  << exception.what();
    throw std::logic_error(error.str());
  }

  try
  {
    outputFile << myNumberActors << " "
	       << myNumberRows << " " << myNumberColumns
	       << std::endl;
    for (Actor a : myActors)
    {
      Actor::Position position(a.getCurrentPosition());
      outputFile << a.getRank() << " " << std::get<Actor::ROW>(position) << " "
		 << std::get<Actor::COL>(position) << std::endl;
    }

    outputFile.close();
  }
  catch (std::ios_base::failure &exception)
  {
    std::ostringstream error;
    error << "Failed to write to file \"" << theFileName << "\". "
	  << exception.what();
    throw std::logic_error(error.str());
  }
}
