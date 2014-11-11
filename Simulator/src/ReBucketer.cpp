/**
 * @file ReBucketer.cpp
 * @brief Implementation of ReBucketer class
 *
 * @author Michael Albers
 */

#include <Bucket.h>
#include <ReBucketer.h>

//***********************
// ReBucketer::ReBucketer
//***********************
ReBucketer::ReBucketer()
{
}

//***********************
// ReBucketer::ReBucketer
//***********************
ReBucketer::ReBucketer(Individual *theIndividual,
                       Bucket *theBucket)
{
  set(theIndividual, theBucket);
}

//***********************
// ReBucketer::ReBucketer
//***********************
ReBucketer::ReBucketer(const ReBucketer &theReBucketer)
{
  *this = theReBucketer;
}

//************************
// ReBucketer::~ReBucketer
//************************
ReBucketer::~ReBucketer()
{
}

//**********************
// ReBucketer::operator=
//**********************
ReBucketer& ReBucketer::operator=(const ReBucketer &theReBucketer)
{
  myBucket = theReBucketer.myBucket;
  myIndividual = theReBucketer.myIndividual;
  return *this;
}

//**************************
// ReBucketer::getIndividual
//**************************
Individual* ReBucketer::getIndividual() const
{
  return myIndividual;
}

//*********************
// ReBucketer::rebucket
//*********************
void ReBucketer::rebucket()
{
  myBucket->receiveIndividual(myIndividual);
}

//****************
// ReBucketer::set
//****************
void ReBucketer::set(Individual *theIndividual,
                     Bucket *theBucket)
{
  myBucket = theBucket;
  myIndividual = theIndividual;
}
