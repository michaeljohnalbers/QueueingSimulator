#ifndef _CommandLineOptionsProcessor_h_
#define _CommandLineOptionsProcessor_h_

/**
 * @file CommandLineOptionsProcessor.h
 * @brief Command line option processor.
 *
 * Provides a class used to parse command line options.
 * 
 * @author Michael Albers
 */

#include <cstdint>
#include <stdexcept>
#include <string>

/**
 * Class used to retrieve and validate command line options (arguments
 *  are ignored).
 *
 * This class defines the allowable options for the Queueing Simulator. It is
 * used to parse the command line arguments and later to determine which options
 * were given.
 */
class CommandLineOptionsProcessor
{
  // ************************************************************
  // Public
  // ************************************************************
  public:

  /**
   * Enum defining how the input to the simulator is given.
   */
  typedef enum
  {
    UNKNOWN,     /** No method yet provided. */
    INPUT_FILE,  /** Input comes from a pre-made file. */
    OPTIONS      /** Command line options are used. */
  } InputOption;

  /**
   * Default constructor.
   */
  CommandLineOptionsProcessor();

  /**
   * Destructor.
   */
  virtual ~CommandLineOptionsProcessor();

  /**
   * Returns the input file being used as input.
   * @return Input file name.
   * @throws std::logic_error - If the user is not using the input file input
   *                            method.
   */
  const std::string& getInputFile() const throw(std::logic_error);

  /**
   * Returns which type of input options the user decided to use.
   * @return Input options type.
   */
  InputOption getInputOption() const;

  /**
   * Returns the number of actors in the simulation as specified by the user.
   * @return String representation of the number of actors.
   * @throws std::logic_error - If the user is not using the options input
   *                            method.
   */
  const std::string& getNumberActors() const throw(std::logic_error);

  /**
   * Returns the seed used for random number generation.
   * @return String representation of the seed.
   * @throws std::logic_error - If the user is not using the options input
   *                            method.
   */
  const std::string& getSeed() const throw(std::logic_error);

  /**
   * Returns the world size in the format "XxY", where 'X' is the size of the
   * x-dimension and 'Y' is the size of the y-dimension.
   * @return World size.
   * @throws std::logic_error - If the user is not using the options input
   *                            method.
   */
  const std::string& getWorldSize() const throw(std::logic_error);

  /**
   * Function used to parse the command line options. This will ignore all
   * non-option arguments. That is only arguments with the GNU "--<option"
   * format will be used.
   *
   * @param argc - Argument count
   * @param argv - Full list of command line arguments. This will be permuted
   *               during processing of options.
   * @param theFirstArg - OUT parameter. Index into argv of the first
   *                      non-option argument.
   * @return Status of options parse. 0 indicates success, 1 indicates parsing
   *         quit due to help option.
   * @throw std::logic_error On parse error.
   */
  int32_t processOptions(int argc, char **argv, int32_t &theFirstArg)
    throw (std::logic_error);

  // ************************************************************
  // Protected
  // ************************************************************
  protected:

  /** Value of the input file option. */
  std::string myInputFile;
  /** Which input option the user will be using. */
  InputOption myInputOption;
  /** Value of the number of actors option. */
  std::string myNumberActors;
  /** Random number generation seed. */
  std::string mySeed;
  /** Value of the world size option. */
  std::string myWorldSize;

  // ************************************************************
  // Private
  // ************************************************************
  private:
  /**
   * Copy constructor.
   * @param Object to copy.
   */
  CommandLineOptionsProcessor(const CommandLineOptionsProcessor&);

  /**
   * Assignment operator.
   * @param Object to copy.
   * @return This object.
   */
  CommandLineOptionsProcessor& operator=(const CommandLineOptionsProcessor&);
};

#endif
