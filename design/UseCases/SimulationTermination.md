# Simulation Termination #
This use case describes the steps taken when a simultion is terminated, either by naturally ending or by user-defined termination (i.e., pressing the Stop button). This assumes the [Simulation Start](SimulationStart.md) use case has already been run to completion.

## Main Success Scenario ##
1. Simulator disables the pause and stop buttons.
2. Simulator disables the camera control buttons.
3. Simulator freezes all time values and % complete.
4. Simulator enables the play button.
5. Simulator enables the File->Save Results and View->Results menu selections.
6. Simulator enables the File->Open menu selection.

## Extensions ##
None.
