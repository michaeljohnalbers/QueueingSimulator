/**
 * @file Exit.cpp
 * @brief Implementation of the Exit class.
 *
 * @author Michael Albers
 */

#include <cmath>

#include <EigenHelper.h>
#include <Exit.h>
#include <Individual.h>

//*****************
// Static Variables
//*****************
Eigen::Vector2f Exit::myExit(0.0, 0.0);
int32_t Exit::myRankToExit = 1;

//******************
// Exit::canExit
//******************
bool Exit::canExit(const Individual &theIndividual)
{
  bool canExit = false;
  if (theIndividual.getRank() == myRankToExit)
  {
    const Eigen::Vector2f &position = theIndividual.getPosition();
    float distance = EigenHelper::distance(position, myExit);

    if (distance <= EXIT_RADIUS)
    {
      canExit = true;
      ++myRankToExit;
    }
  }
  return canExit;
}

//******************
// Exit::getPosition
//******************
const Eigen::Vector2f& Exit::getPosition()
{
  return myExit;
}

//********************
// Exit::getRankToExit
//********************
int32_t Exit::getRankToExit()
{
  return myRankToExit;
}

//******************************
// Exit::setPosition
//******************************
void Exit::setPosition(const Eigen::Vector2f &theExit)
{
  myExit = theExit;
}
