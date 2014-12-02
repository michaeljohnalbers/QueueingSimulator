QueueingSimulator
=================

Contents
1. Overview
2. Building
3. Running
4. Benchmarking

1. Overview
-----------
Queueing Simulator is a simulator written as my research project for CSCI 5551,
Parallel & Distributed Systems, at the University of Colorado - Denver. The
simulator uses the Boids model of crowd simulation originally developed by
Craig Reynolds in 1987 to simulate a crowd of people forming a queue. It also
serves as a mechanism to benchmark/measure the effectiveness of OpenMP thread
nesting.

The basic idea of the simulator is that there are some number of individuals in
the simulated world. Each has a rank. This rank is the individual's place in
line.  The individuals start out scattered around the world and then work their
way toward the world exit, a fixed spot. They can only exit in order of their
rank, with rank one being the first individual which can exit. The simulation
progresses until the individual with rank N (where N is the number of
individuals) has exited the world. Right now the simulation capability is not
fully implemented and tested. There is a rough simulation capability. The full
capability will probably not be added until there is a GUI overlayed. Testing
the correctness of the simulation is very difficult to do without a GUI.

Also, please see the license in LICENSE.txt for terms of use, permissions
granted, etc.

2. Building
-----------
Queueing Simulator was developed and tested with the following:
C++11
g++ 4.8.2 (gcc.gnu.org/)
Linux Kernel 3.13.0-24
gmake 3.81 (www.gnu.org/software/make/)

It also requires that the Eigen Linear Algebra library is installed. This can
be found at http://eigen.tuxfamily.org/. Version 3.2.2 is the version currently
used by Queueing Simulator.

Two environment variables need to be set before building:
1. QS_BASE_DIR
This is directory where Queueing Simulator was installed (plus
"QueueingSimulator"). For instance, if Queueing Simulator was installed in your
home directory, you would set QS_BASE_DIR to "~/QueueingSimulator."

2. QS_EIGEN_BASE_DIR
This is the directory where the Eigen library can be found.  When un-taring the
Eigen tar file it creates a directory named something like
"eigen-eigen-1306d75b4a21". QS_EIGEN_BASE_DIR should be set to the full path to
this directory. For instance, if the tar file was un-tared in your home
directory, you would set QS_EIGEN_BASE_DIR to "~/eigen-eigen-1306d75b4a21".

Once these environment variables are set, change directories to $QS_BASE_DIR and
run 'gmake'. You can also run 'gmake help' to see a list of all targets.

The Queueing Simulator executable will be found in the
$QS_BASE_DIR/Simulator/bin directory when the build is finished. It is called
QueueingSimulator. A fully serial version is also created. See the
"Benchmarking' section below for further details.

Queueing Simulator also supports the use of Doxygen (www.doxygen.org) for
documentation generation. Doxygen itself must be installed along with LaTeX
(www.latex-project.org) and Graphviz (www.graphviz.org). The Makefiles assume
that all of the needed executables are on your PATH which they likely will be
once the packages are installed.

To create the documentation, from $QS_BASE_DIR, run 'gmake doc'. This will
create HTML documentation which can be found in $QS_BASE_DIR/doc/html (start
with index.html). A PDF called QueueingSimulator.pdf will also be created in
$QS_BASE_DIR/doc. To clean the documentation, run 'gmake doc_clean' from the
same directory as the original gmake.

3. Running
----------
To run Queueing Simulator simply change directories to the Simulator/bin
directory and run the QueueingSimulator executable. No environment variables
are needed.  It is suggested that you first run Queueing Simulator with the
"--help" command line option. This will list all of the command line arguments
required.

Currently, Queueing Simulator requires the name of a configuration file as its
sole argument. This configuration file contains values used to define the
simulation, such as the number of individuals in the simulation and the world
size. The '--help' option gives details for the format and contents of the
configuration file. An example configuration file can be found in the
QueueingSimulator/data directory.

QueueingSimulator writes all of its data to standard out. It does not create
any files. It is recommended that when running QueueingSimulator you redirect
its output to a file or pipe it to 'tee'.

QueueingSimulator will print out its configuration data before starting the
actual simulation. During the course of the simulation it will only print out
details of when each individual in the queue exits the world. Once all
individuals have exited the world the simulation will be over. At that time
QueueingSimulator will print out some statistics for each individual.

4. Benchmarking
---------------
For the benchmarking capability the simulation runs in a different
fashion. Each individual simply moves directly toward the exit. There is no
collision detection or ordering done. Since the individual is moving in a
straight line at a known velocity the expected time it should take to cover th
distance from its starting point to the exit is a trivial calculation. The
expected time can then be compared to the actual time it took.  This can be
used to determine how well the simulator handled the given number of
individuals as well as to determine the impact of the nested OpenMP threading
the simulator uses. To put the simulator into the benchmark mode the
configuration file for Queueing Simulator needs to be altered (see the
"Running" section above for details).

A fully serial version of Queueing Simulator is also created at compile time.
This executable is called QueueingSimulator_serial. This executable is also
located in the $QS_BASE_DIR/Simulator/bin directory. The serial version of
Queueing Simulator is exactly identical to the normal Queueing Simulator with
the lone exception that there is no parallelism at all.

In the "tools" directory there are some scripts which can be used to run
multiple benchmark tests on Queueing Simulator. These can be run without any
setup after the project has been built. They will create a directory in the
user's home directory with the captured data from each run of Queueing
Simulator.
