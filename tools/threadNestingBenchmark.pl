#!/usr/bin/perl

# \file threadNestingBenchmark.pl
# \brief Script to run benchmark tests on Queueing Simulator.
# \author Michael Albers

use strict;
use warnings;

use File::Basename;
use File::Path qw/make_path/;

my $runsPerConfig = 3;
my @numberIndividuals = qw/1 10 100 250 500 1000 2500 5000 10000/;

my $simulatorExecutable = "$ENV{QS_BASE_DIR}/Simulator/bin/QueueingSimulator";
my $basename = basename($simulatorExecutable);
if (! -x $simulatorExecutable)
{
  die "$0: Queueing Simulator exe, $simulatorExecutable, ".
    "does not have execute permissions (maybe need to set \$QS_BASE_DIR).\n";
}

foreach my $individuals (@numberIndividuals)
{
  my @threadDepths = qw/1 2 3/;

  foreach my $threadDepth (@threadDepths)
  {
    # Most of this path matches serialThreadedBenchmark.pl.
    my $outputDirectory =
      "$ENV{HOME}/QueueingSimulator/benchmarkData/threadNesting/";
    if (! -d $outputDirectory)
    {
      make_path("$outputDirectory") or
        die "$0: Failed to create output directory $outputDirectory: $!";
    }

    my $configFileBase = "${basename}_depth${threadDepth}_population$individuals";
    my $configFileName = "$outputDirectory/${configFileBase}.cnfg";

    open CONFIG, ">$configFileName" or
      die "$0: Failed to create config file $configFileName: $!";

    print CONFIG "$individuals\n";
    print CONFIG "500 500\n";		     # World dimensions
    print CONFIG "auto\n";		     # Number of buckets
    print CONFIG "5.5\n";		     # Neighbor search radius
    print CONFIG "15\n";		     # Number of neighbors
    print CONFIG "9202893\n";		     # Random number generator seed
    print CONFIG "benchmark $threadDepth\n"; # Run benchmark using given
                                             # thread nesting limit
    close CONFIG;

    for (my $run = 1; $run <= $runsPerConfig; ++$run)
    {
      my $outputFileName = "${configFileBase}_$run";
      if (-x "/usr/bin/bpsh")   # Running on Hydra
      {
        my $jobScript = "$outputDirectory/${outputFileName}.sh";
        open JOB_SCRIPT, ">$jobScript" or
          die "$0: Failed to create job script $jobScript: $!";
        print JOB_SCRIPT "#!/bin/bash\n";
        print JOB_SCRIPT "#PBS -N $outputFileName\n";
        print JOB_SCRIPT "#PBS -l walltime=6:00:00\n";
        print JOB_SCRIPT "#PBS -j oe\n";
        print JOB_SCRIPT "#PBS -l nodes=1\n";
        print JOB_SCRIPT "#PBS -o $outputFileName\n";
        print JOB_SCRIPT "#PBS -w $outputDirectory\n";
        print JOB_SCRIPT "bpsh " . (6 + $run) .
          " $simulatorExecutable $configFileName\n";
        close JOB_SCRIPT;
      }
      else
      {
        my $outputFile = "$outputDirectory/$outputFileName";
        print "$0: Run $run of depth $threadDepth for " .
          "$individuals individuals\n";
        print "  Output in $outputFile\n";
        system("$simulatorExecutable $configFileName > $outputFile 2>&1");
      }
    }
  }
}
exit 0;
