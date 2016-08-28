# QueueingSimulator

## Overview
Queueing Simulator is a crowd simulator written as my Master's project at the University of Colorado, Denver. The simulator uses the [Boids](http://www.red3d.com/cwr/boids/) model of crowd simulation originally developed by Craig Reynolds in 1987 to simulate a crowd of people forming a queue.

## Building
Queueing Simulator uses [CMake](http://www.cmake.org) as the means for building. There is a 'build' directory already in the project for out of source builds.

From the build directory, simply run:

    $ cmake ..

This will generate all of the required Makefiles and such. Once that is complete, run:

    $ make

This will download all needed projects, build them then build Queueing Simulator.

Multiple different software packages are used including [Eigen](http://eigen.tuxfamily.org), [OpenGL](https://www.opengl.org) (along with the usual supporing packages), [gtkmm](http://www.gtkmm.org) and others. The CMake setup will download some of these, others it expects to find on the system. (Basically if it was trivial to download/build it will be downloaded.)

Different types of builds are supported including a debug and release build. Debug is the default build. To change the build type, add the '-DCMAKE_BUILD_TYPE=<release type> option when invoking CMake. Refer to [this](https://cmake.org/cmake/help/v3.0/variable/CMAKE_BUILD_TYPE.html) for more information.

### Testing
Queueing Simulator has many self-tests that are built when using Debug mode. These can be run by:

    $ make test

or

    $ ctest

## Installation
Once the build is complete, again, from the build directory, run:

    $ make install

As of now this will install the package into the build directory in the 'install' directory. This will be changing in the future once I get the build/installation stuff better sorted out.

## Running
Queueing Simulator comes with a launcher script to start the program. It is found in the installed 'bin' directory. To run Queueing Simulator:

    $ cd bin
	$ ./QueueingSimulator.sh

## License
Refer to the LICENSE.txt file in the distribution.
