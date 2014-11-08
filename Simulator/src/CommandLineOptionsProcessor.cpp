/**
 * @file CommandLineOptionsProcessor.cpp
 * @brief Command line option processor.
 *
 * @author Michael Albers
 */

#include <CommandLineOptionsProcessor.h>
#include <getopt.h>
#include <iostream>

CommandLineOptionsProcessor::CommandLineOptionsProcessor() :
  myInputOption(UNKNOWN)
{
}

CommandLineOptionsProcessor::~CommandLineOptionsProcessor()
{
}

const std::string& CommandLineOptionsProcessor::getInputFile()
  const throw(std::logic_error)
{
  if (UNKNOWN == myInputOption)
  {
    throw std::logic_error("No input option given on command line.");
  }
  else if (OPTIONS == myInputOption)
  {
    throw std::logic_error("Input option is not input file.");
  }
  return myInputFile;
}

CommandLineOptionsProcessor::InputOption
CommandLineOptionsProcessor::getInputOption() const
{
  return myInputOption;
}

const std::string& CommandLineOptionsProcessor::getNumberActors()
  const throw(std::logic_error)
{
  if (UNKNOWN == myInputOption)
  {
    throw std::logic_error("No input option given on command line.");
  }
  else if (INPUT_FILE == myInputOption)
  {
    throw std::logic_error("Input option is not command line options.");
  }
  return myNumberActors;
}

const std::string& CommandLineOptionsProcessor::getSeed()
  const throw(std::logic_error)
{
  if (UNKNOWN == myInputOption)
  {
    throw std::logic_error("No input option given on command line.");
  }
  else if (INPUT_FILE == myInputOption)
  {
    throw std::logic_error("Input option is not command line options.");
  }
  return mySeed;
}

const std::string& CommandLineOptionsProcessor::getWorldSize()
  const throw(std::logic_error)
{
  if (UNKNOWN == myInputOption)
  {
    throw std::logic_error("No input option given on command line.");
  }
  else if (INPUT_FILE == myInputOption)
  {
    throw std::logic_error("Input option is not command line options.");
  }
  return myWorldSize;
}

int32_t CommandLineOptionsProcessor::processOptions(
  int argc, char **argv, int32_t &theFirstArg) throw (std::logic_error)
{
  int32_t optionsResult = 0;

  bool keepProcessingOptions = true;

  while (keepProcessingOptions)
  {
    enum
    {
      ACTORS_OPTION = 1,
      HELP_OPTION,
      INPUT_FILE_OPTION,
      WORLD_SIZE_OPTION,
      SEED_OPTION
    };

    static struct option longOptions[] =
      {
	{"actors", required_argument, 0, ACTORS_OPTION},
	{"help", no_argument, 0, HELP_OPTION},
	{"input-file", required_argument, 0, INPUT_FILE_OPTION},
	{"seed", required_argument, 0, SEED_OPTION},
	{"world-size", required_argument, 0, WORLD_SIZE_OPTION},
	{0,0,0,0}
      };

    int optionIndex = 0;
    extern char *optarg;
    int c = getopt_long(argc, argv, "", longOptions, &optionIndex);

    if (-1 == c)
    {
      break;
    }

    switch (c)
    {
      case ACTORS_OPTION:
	if (INPUT_FILE == myInputOption)
	{
	  throw std::logic_error(
	    "Cannot mix input file and options for input method.");
	}
	myInputOption = OPTIONS;
	myNumberActors = optarg;
	break;

      case HELP_OPTION:
	optionsResult = 1;
	keepProcessingOptions = false;
	break;

      case INPUT_FILE_OPTION:
	if (OPTIONS == myInputOption)
	{
	  throw std::logic_error(
	    "Cannot mix input file and options for input method.");
	}
	myInputOption = INPUT_FILE;
	myInputFile = optarg;
	break;

      case SEED_OPTION:
	if (INPUT_FILE == myInputOption)
	{
	  throw std::logic_error(
	    "Cannot mix input file and options for input method.");
	}
	myInputOption = OPTIONS;
	mySeed = optarg;
	break;

      case WORLD_SIZE_OPTION:
	if (INPUT_FILE == myInputOption)
	{
	  throw std::logic_error(
	    "Cannot mix input file and options for input method.");
	}
	myInputOption = OPTIONS;
	myWorldSize = optarg;
	break;

      case ':':
      case '?':
      default:
	throw std::logic_error("");
	break;
    }
  }

  if (UNKNOWN == myInputOption)
  {
    throw std::logic_error("No input options were specified. "
			   "Must use either --input-file or parameters "
			   "(number actors/world size/seed/etc).");
  }
  else if (INPUT_FILE == myInputOption)
  {
    if ("" == myInputFile)
    {
      throw std::logic_error("No input file was specified.");
    }
  }
  else
  {
    if ("" == myNumberActors || "" == myWorldSize || "" == mySeed)
    {
      throw std::logic_error("One of --world-size, --actors or "
			     "--seed was not specified.");
    }
  }

  theFirstArg = optind;
  return optionsResult;
}
