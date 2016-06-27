# Simulation Control #
This use case describes the ability to control the simulation while it is running. This assumes the [Simulation Start](SimulationStart.md) use case has already been run to completion. This can be run in parallel with [Camera Control](CameraControl.md) use case.

## Main Success Scenario ##
1. User clicks the pause button.
2. Simulator pauses the simulation:
   1. Elapsed time count pauses
   2. Simulation state becomes frozen (no movement updates).
   3. Pause button disabled
   4. Play button enabled.
3. User clicks the play button.
4. Simulator continues the simulation:
   1. Elapsed time count resumes
   2. Simulation state resumes updating (movement continues).
   3. Pause button enabled.
   4. Play button disabled.
5. User clicks the stop button:
   1. Execute [SimulationTermination](SimulationTermination.md) use case.

## Extensions ##
1.<br>
  1. User clicks the stop button
     1. Jump to step 5.
4.<br>
  1. User clicks the stop button.
     1. Jump to step 5.
