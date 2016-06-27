# Queuing Simulator Startup #
This use case describes the steps taken on the part of the user to start the Queuing Simulator.

## Main Success Scenario ##
1. User enters program name at command line, providing any options so desired.
2. Program initializes self
3. Program displays GUI for further interaction. No more actions taken until the user proceeds.
   * The following GUI elements will be enabled (all others disabled):
      * File->Open, File->Exit
	  * View->Messages
	  * Help->About

## Extensions ##
1.<br>
   1. Invalid option(s) provided
      1. Error message is printed to the terminal, program terminates with error code.
   2. User enters 'help' option
      1. Help information printed to terminal, program terminates with nominal code.

2.<br>
   1. Initialization failure
      1. Error message(s?) is printed to terminal, program terminates with error code.
