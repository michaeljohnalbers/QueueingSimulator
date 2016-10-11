/**
 * @file BatchVisualization.cpp
 * @brief Definition of BatchVisualization
 *
 * @author Michael Albers
 */

#include <cstring>
#include <stdexcept>
#include "BatchVisualization.h"

QS::BatchVisualization::BatchVisualization(
  World &theWorld,
  const std::string &theOutputFileName) :
  Visualization(theWorld),
  myOutputFile(theOutputFileName, std::ios_base::out | std::ios_base::binary),
  myOutputFileName(theOutputFileName)
{
  if (! myOutputFile.is_open())
  {
    auto thisErrno = errno;
    std::string error{"Failed to open batch mode output file \""};
    error += theOutputFileName + "\": " + std::strerror(thisErrno);
    throw std::runtime_error(error);
  }
}

float QS::BatchVisualization::getUpdateInterval() noexcept
{
  // 24 frames per second
  return 1.0/24.0;
}

std::tuple<int, int> QS::BatchVisualization::getWindowDimensions() noexcept
{
  myWindowHeight = 480; // Attempt 480p-ish resolution
  myWindowWidth = myWindowHeight * getAspectRatio();

  myBufferSize = NUMBERS_PER_PIXEL * myWindowWidth * myWindowHeight;
  myInvertedPixelBuffer.reset(new char[myBufferSize]);
  myPixelBuffer.reset(new char[myBufferSize]);

  return std::make_tuple(myWindowWidth, myWindowHeight);
}

void QS::BatchVisualization::preBufferSwap() noexcept
{
  // https://matthewarcus.wordpress.com/2013/03/10/movies-from-opengl/
  glFlush();

  auto *invertedBufferPointer = myInvertedPixelBuffer.get();
  glReadPixels(0, 0, myWindowWidth, myWindowHeight, GL_RGB, GL_UNSIGNED_BYTE,
               invertedBufferPointer);

  // glReadPixels treats (0,0) as the lower left corner, however, glfw treats
  // (0,0) as the upper left. So this code rotates the image 180 deg on the X
  // axis (in other words, inverts the Y values or makes the top row the bottom
  // and vice versa).
  auto *bufferPointer = myPixelBuffer.get();
  auto itemsPerRow = NUMBERS_PER_PIXEL * myWindowWidth;
  for (auto row = 0, invertedRow = myWindowHeight-1;
       row < myWindowHeight;
       ++row, --invertedRow)
  {
    std::memcpy(&bufferPointer[row * itemsPerRow],
                &invertedBufferPointer[invertedRow * itemsPerRow],
                itemsPerRow);
  }

  // TODO: use ffmpeg directly
  // http://stackoverflow.com/questions/36687188/create-video-using-ffmpeg
  // https://www.ffmpeg.org/doxygen/0.6/output-example_8c-source.html

  myOutputFile.write(bufferPointer, myBufferSize);
  if (!myOutputFile)
  {
    std::string error{"Failed to write batch mode frame to output file \""};
    error += myOutputFileName + "\".";
    throw std::runtime_error(error);
  }
}

void QS::BatchVisualization::setWindowHints() noexcept
{
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
}
