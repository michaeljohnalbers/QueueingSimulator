#!/usr/bin/perl

# \file serialThreadedBenchmark.pl
# \brief Script to run benchmark tests on Queueing Simulator.
# \author Michael Albers

use strict;
use warnings;

use File::Basename;
use File::Path qw/make_path/;

my $runsPerConfig = 3;
my @numberIndividuals = qw/1 10 100 250 500 1000 2000/;

my $simulatorExecutable = "../Simulator/bin/QueueingSimulator";
my $simulatorExecutableSerial = "${simulatorExecutable}_serial";

my @executables = ($simulatorExecutable, $simulatorExecutableSerial);

foreach (@executables)
{
  if (! -x $_)
  {
    die "$0: Queueing Simulator exe, $_, does not have execute permissions.\n";
  }
}

foreach my $exe (@executables)
{
  my $basename = basename($exe);

  # Most of this path matches threadNestingBenchmark.pl.
  my $outputDirectory =
    "$ENV{HOME}/QueueingSimulator/benchmarkData/serialvsThreaded/$basename";
  if (! -d $outputDirectory)
  {
    make_path("$outputDirectory") or
      die "$0: Failed to create output directory $outputDirectory: $!";
  }

  foreach my $individuals (@numberIndividuals)
  {
    my $configFileBase = "${basename}_$individuals";
    my $configFileName = "$outputDirectory/${configFileBase}.cnfg";

    open CONFIG, ">$configFileName" or
      die "$0: Failed to create config file $configFileName: $!";

    print CONFIG "$individuals\n";
    print CONFIG "500 500\n"; # World dimensions
    print CONFIG "auto\n"; # Number of buckets
    print CONFIG "5.5\n"; # Neighbor search radius
    print CONFIG "15\n"; # Number of neighbors
    print CONFIG "9202893\n"; # Random number generator seed
    print CONFIG "benchmark 0\n"; # Run benchmark using OpenMP's
				# default thread nesting limit
    close CONFIG;

    for (my $run = 1; $run <= $runsPerConfig; ++$run)
    {
      my $outputFile = "$outputDirectory/${configFileBase}_$run";
      print "$0: Run $run of $basename for $individuals individuals\n";
      print "  Output in $outputFile\n";
      system("$exe $configFileName > $outputFile 2>&1");
    }
  }
}

exit 0;
