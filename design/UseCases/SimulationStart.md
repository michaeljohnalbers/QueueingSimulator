# Simulation Start #
This use case describes the steps need to start a simulation running. This assumes the [Startup](Startup.md) use case has already been run to completion.

## Main Success Scenario ##
1. From the GUI the user selects 'File->Open' to select a simulation configuration file.
2. File system navigation dialog is opened.
3. User finds desired configuration file.
4. User selects 'Ok' (or 'Open', depending on implementation). Dialog closes.
5. File is read in.
6. Simulation name field is populated on GUI.
7. Simulator enables the 'Play' button, Disables the File->Open menu selection.
8. Use selects real-time or batch mode. GUI shows/hides available controls, feedback mechanisms (i.e., % complete, elapsed time, camera control buttons) based on mode selection.
9. User selects the 'Play' button. 'Pause' and 'Stop' buttons enabled, 'Play' button disabled.
10. Simulation starts.

## Extensions ##
4.<br>
  1. Use selected 'Cancel'
	 1. Navigation dialog closed, use case aborted.

5.<br>
  1. File cannot be read.
	 1. Error dialog displayed, use case aborted.
  2. File has invalid format, invalid data, other read/parse error.
	 1. Error dialog displayed, use case aborted.
