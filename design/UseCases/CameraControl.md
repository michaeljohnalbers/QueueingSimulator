# Camera Control #
This use case describes the steps taken by the user to control the camera while a simulation is in progress. This assumes the [Simulation Start](SimulationStart.md) use case has already been run to completion. This can be run in parallel with [Simulation Control](SimulationControl.md) use case.

## Main Success Scenario ##
1. User clicks one of the arrow buttons.
2. Simulator repositions camera a fixed distance, in the direction of the button pushed, from the current camera position. Simulator updates displayed camera X & Y coordinates.
3. User clicks one of the zoom buttons.
4. Simulator repositions the camera a fixed distance, in the direction of the button pushed, from the current camera position (i.e., zoom in/out). Simulator updates zoom percentage (100% is the default, 0% is level with simulation "surface").

## Extensions ##
2.<br>
  1. Camera at limit of current direction of movement.
     1. Simulator does not move camera any further. No update to camera coordinates is made.

4.<br>
  1. Camera is at 0% zoom.
     1. Camera is not moved any closer to "surface". Zoom percentage is not updated.

