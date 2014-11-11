/**
 * @file Logger.cpp
 * @brief Implementation of the Logger class.
 *
 * @author Michael Albers
 */

#include <chrono>
#include <iomanip>
#include <iostream>

#include <Logger.h>

//*****************
// Static Variables
//*****************
std::condition_variable Logger::myNotifier;
std::queue<Logger::Message> Logger::myQueue;
std::mutex Logger::myMutex;
bool Logger::myRunning = true;
std::thread Logger::myThread {Logger::run};

//************
// Logger::log
//************
void Logger::log(const std::string &theMessage)
{
  std::unique_lock<std::mutex> lock(myMutex);
  myQueue.push(Message(theMessage));
  myNotifier.notify_all();
}

//************
// Logger::log
//************
void Logger::log(const Message &theMessage)
{
  auto messageTime = theMessage.myTime;
  std::time_t time = std::chrono::system_clock::to_time_t(messageTime);
  std::tm *tmTime = std::localtime(&time);
  char timeBuffer[24];
  std::strftime(timeBuffer, sizeof timeBuffer, "%FT%T", tmTime);

  std::chrono::microseconds ms =
    std::chrono::duration_cast<std::chrono::microseconds>(
      messageTime.time_since_epoch());

  std::size_t fractionalSeconds = ms.count() % 1000000;

  std::cout << timeBuffer << "."
            << std::setfill('0') << std::setw(6) << fractionalSeconds << " - "
            << theMessage.myMessage << std::endl;
}

//***************
// Logger::logAll
//***************
void Logger::logAll()
{
  while (! myQueue.empty())
  {
    log(myQueue.front());
    myQueue.pop();
  }
}

//************
// Logger::run
//************
void Logger::run()
{
  while (myRunning)
  {
    std::unique_lock<std::mutex> lock(myMutex);
    myNotifier.wait_until(lock, std::chrono::system_clock::now() +
                          std::chrono::milliseconds(100));
    logAll();
  }
}

//******************
// Logger::terminate
//******************
void Logger::terminate()
{
  myRunning = false;
  myThread.join();
  logAll();
}

//**************************
// Logger:::Message::Message
//**************************
Logger::Message::Message() : Message {""}
{
}

//*************************
// Logger::Message::Message
//*************************
Logger::Message::Message(const std::string &theMessage) :
  myMessage(theMessage),
  myTime(std::chrono::system_clock::now())
{
}

//*************************
// Logger::Message::Message
//*************************
Logger::Message::Message(const Logger::Message &theMessage) :
  myMessage(theMessage.myMessage),
  myTime(theMessage.myTime)
{
}

//**************************
// Logger::Message::~Message
//**************************
Logger::Message::~Message()
{
}

//***************************
// Logger::Message::operator=
//***************************
Logger::Message& Logger::Message::operator=(const Message &theMessage)
{
  myTime = theMessage.myTime;
  myMessage = theMessage.myMessage;
  return *this;
}
