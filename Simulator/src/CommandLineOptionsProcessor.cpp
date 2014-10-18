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

std::string CommandLineOptionsProcessor::getInputFile()
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

std::string CommandLineOptionsProcessor::getNumberActors()
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

std::string CommandLineOptionsProcessor::getWorldSize()
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
      WORLD_SIZE_OPTION,
      INPUT_FILE_OPTION,
      HELP_OPTION
    };

    static struct option longOptions[] =
      {
	{"actors", required_argument, 0, ACTORS_OPTION},
	{"world-size", required_argument, 0, WORLD_SIZE_OPTION},
	{"input-file", required_argument, 0, INPUT_FILE_OPTION},
	{"help", no_argument, 0, HELP_OPTION},
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
	if (UNKNOWN == myInputOption)
	{
	  myInputOption = OPTIONS;
	  myNumberActors = optarg;
	}
	else if (INPUT_FILE == myInputOption)
	{
	  throw std::logic_error(
	    "Cannot mix input file and options for input method.");
	}
	break;

      case WORLD_SIZE_OPTION:
	if (UNKNOWN == myInputOption)
	{
	  myInputOption = OPTIONS;
	  myWorldSize = optarg;
	}
	else if (INPUT_FILE == myInputOption)
	{
	  throw std::logic_error(
	    "Cannot mix input file and options for input method.");
	}
	break;

      case INPUT_FILE_OPTION:
	if (UNKNOWN == myInputOption)
	{
	  myInputOption = INPUT_FILE;
	  myInputFile = optarg;
	}
	else if (OPTIONS == myInputOption)
	{
	  throw std::logic_error(
	    "Cannot mix input file and options for input method.");
	}
	break;

      case HELP_OPTION:
	optionsResult = 1;
	keepProcessingOptions = false;
	break;

      case ':':
      case '?':
      default:
	throw std::logic_error("");
	break;
    }
  }

  theFirstArg = optind;
  return optionsResult;
}
