/**
 * @file Logger.cpp
 * @brief Implementation of the Logger class.
 *
 * @author Michael Albers
 */

#include <chrono>
#include <cstring>
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
  static const uint32_t fractionSecondsWidth = 6;

  auto messageTime = theMessage.myTime;
  std::time_t time = std::chrono::system_clock::to_time_t(messageTime);
  std::tm *tmTime = std::localtime(&time);
  char timeBuffer[24];
  std::strftime(timeBuffer, sizeof timeBuffer, "%FT%T", tmTime);
  std::size_t timeStampLen = std::strlen(timeBuffer);

  std::chrono::microseconds ms =
    std::chrono::duration_cast<std::chrono::microseconds>(
      messageTime.time_since_epoch());

  std::size_t fractionalSeconds = ms.count() % 1000000;

  // 4 is for the "." and " - " in the logged message.
  timeStampLen += fractionSecondsWidth + 4;

  std::string message = theMessage.myMessage;
  for (std::string::size_type ii = 0; ii < message.size(); ++ii)
  {
    if (message[ii] == '\n')
    {
      message.insert(ii+1, timeStampLen, ' ');
    }
  }

  std::cout << timeBuffer << "."
            << std::setfill('0') << std::setw(fractionSecondsWidth)
            << fractionalSeconds << " - " << message << std::endl;
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
