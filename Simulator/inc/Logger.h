#ifndef _Logger_h_
#define _Logger_h_

/**
 * @file Logger.h
 * @brief Logger class for the simulation
 *
 * @author Michael Albers
 */

#include <mutex>
#include <queue>
#include <thread>

/**
 */
class Logger
{
  // ************************************************************
  // Public
  // ************************************************************
  public:

  /**
   * Copy constructor.
   * @param Object to copy.
   */
  Logger(const Logger&) = delete;

  /**
   * Destructor.
   */
  ~Logger();

  /**
   * Assignment operator.
   * @param Object to copy.
   * @return This object.
   */
  Logger& operator=(const Logger&) = delete;

  /**
   * Logs a message.
   * @param theMessage Message to log.
   */
  static void log(const std::string &theMessage);

  /**
   * Tells the thread to stop running.
   */
  static void stop();

  // ************************************************************
  // Protected
  // ************************************************************
  protected:

  /**
   * Thread function.
   */
  static void run();

  /**
   * Default constructor.
   */
  Logger();

  // ************************************************************
  // Private
  // ************************************************************
  private:

  class Message
  {
    public:

    /**
     * Default constructor.
     */
    Message();

    /**
     * Copy constructor.
     * @param theMessage Message to copy.
     */
    Message(const Message &theMessage);

    /**
     * Destructor.
     */
    ~Message();

    /**
     * Assignment operator.
     * @param theMessage Object to copy.
     * @return This object.
     */
    Message& operator=(const Message &theMessage);

    std::string theMessage;
    std::chrono::time_point<std::chrono::system_clock> myTime;
  };

  static Logger myLogger;
  static std::thread myThread;
  static bool myRunning;

  static std::queue<Message> myMessages;
  static std::recursive_mutex myQueueMutex;
};

#endif
