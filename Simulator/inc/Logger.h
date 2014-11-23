#ifndef _Logger_h_
#define _Logger_h_

/**
 * @file Logger.h
 * @brief Logger class for the simulation
 *
 * @author Michael Albers
 */

#include <condition_variable>
#include <mutex>
#include <ostream>
#include <queue>
#include <thread>

/**
 * Provides logging capabilities for the simulator.
 */
class Logger
{
  // ************************************************************
  // Public
  // ************************************************************
  public:

  /**
   * Default constructor.
   */
  Logger() = delete;

  /**
   * Copy constructor.
   * @param theObjectToCopy Object to copy.
   */
  Logger(const Logger &theObjectToCopy) = delete;

  /**
   * Destructor.
   */
  ~Logger();

  /**
   * Assignment operator.
   * @param theRHS Object to copy.
   * @return This object.
   */
  Logger& operator=(const Logger &theRHS) = delete;

  /**
   * Logs a message.
   * @param theMessage Message to log.
   */
  static void log(const std::string &theMessage);

  /**
   * Tells the thread to stop running.
   */
  static void terminate();

  // ************************************************************
  // Protected
  // ************************************************************
  protected:

  // ************************************************************
  // Private
  // ************************************************************
  private:

  /**
   * Stores a message to be logged and the time of the message.
   */
  class Message
  {
    public:

    /**
     * Default constructor.
     */
    Message();

    /**
     * Constructor
     * @param theMessage Message to log.
     */
    Message(const std::string &theMessage);

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

    /** Message to log. */
    std::string myMessage;

    /** Origin time of the message. */
    std::chrono::time_point<std::chrono::system_clock> myTime;
  };

  /**
   * Writes out the log message.
   * @param theMessage Message to log.
   */
  static void log(const Message &theMessage);

  /**
   * Empties the message queue logging all messages.
   */
  static void logAll();

  /**
   * Thread function.
   */
  static void run();

  /** Message queue. All messages to be logged are placed on this queue. */
  static std::queue<Message> myQueue;

  /** Means to block logging thread until there is a message to log. */
  static std::condition_variable myNotifier;

  /** Mutex for thread-safing the message queue. */
  static std::mutex myMutex;

  /** Thread control variable. */
  static bool myRunning;

  /** Thread reading from the messages queue. */
  static std::thread myThread;
};

#endif
