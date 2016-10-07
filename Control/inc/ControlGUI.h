#pragma once

/**
 * @file ControlGUI.h
 * @brief Creates all GUI controls for the simulator.
 *
 * @author Michael Albers
 */

#include <gtkmm.h>
#include <memory>
#include "SimulationPackage.h"
#include "Visualization.h"

namespace QS
{
  class Metrics;

  /**
   * This class manages the entire control GUI for Queueing Simulator. This is
   * the primary user interface. From this GUI the user can select the
   * simulation to run, start/stop/pause the simulation, set the simulation
   * mode. Other abilities are provided.
   *
   * The 'run' function is a convenience function for taking all necessary steps
   * to display and run the GUI. The user shouldn't need to instantiate this
   * class at all.
   */
  class ControlGUI : public Gtk::Window
  {
    public:

    /**
     * Default constructor.
     */
    ControlGUI() = delete;

    /**
     * Constructor
     *
     * @param theBaseDir
     *          QS base directory (installation directory)
     */
    ControlGUI(const std::string &theBaseDir);

    /**
     * Copy constructor.
     */
    ControlGUI(const ControlGUI&) = delete;

    /**
     * Move constructor.
     */
    ControlGUI(ControlGUI&&) = delete;

    /**
     * Destructor.
     */
    virtual ~ControlGUI();

    /**
     * Move assignment operator.
     */
    ControlGUI& operator=(const ControlGUI&) = delete;

    /**
     * Move assignment operator
     */
    ControlGUI& operator=(ControlGUI&&) = delete;

    /**
     * Main GUI function. When this returns the GUI has finished.
     *
     * @param argc
     *          argc from main
     * @param argv
     *          argv from main
     * @param theBaseDir
     *          QS base directory (installation directory)
     *
     * @return overall GUI status
     */
    static int run(int argc, char **argv, const std::string &theBaseDir);

    protected:

    /**
     * Callback for selecting the batch file select button.
     */
    void batchFileActive();

    /**
     * Builds the batch mode controls.
     *
     * @param theContainer
     *          container in which to place the controls.
     */
    void buildBatch(Gtk::Container &theContainer);

    /**
     * Builds the application menu
     *
     * @param theContainer
     *          container in which to place the menu.
     */
    void buildMenu(Gtk::Container &theContainer);

    /**
     * Builds the mode selection controls
     *
     * @param theContainer
     *          container in which to place the controls.
     */
    void buildModeFrame(Gtk::Container &theContainer);

    /**
     * Builds the real-time mode controls
     *
     * @param theContainer
     *          container in which to place the controls.
     */
    void buildRealTime(Gtk::Container &theContainer);

    /**
     * Builds the results dialog and child widgets.
     */
    void buildResultsDialog();

    /**
     * Builds the simulation name frame
     *
     * @param theContainer
     *          container in which to place the frame.
     */
    void buildSimulationFrame(Gtk::Container &theContainer);

    /**
     * Builds the simulation control widgets
     *
     * @param theContainer
     *          container in which to place the widgets.
     */
    void buildSimulationControlFrame(Gtk::Container &theContainer);

    /**
     * Builds the simulation status widgets
     *
     * @param theContainer
     *          container in which to place the widgets.
     */
    void buildSimulationStatusFrame(Gtk::Container &theContainer);

    /**
     * Callback for clicking any camera move button.
     *
     * @param theInputType
     *          camera direction
     */
    void cameraMoveHandler(Visualization::UserInputType theInputType);

    /**
     * Callback for File->Open
     */
    void fileOpenHandler();

    /**
     * Callback for File->Save Results
     */
    void fileSaveResultsHandler();

    /**
     * Callback for File->Exit
     */
    void fileExitHandler();

    /**
     * Callback for Help->About
     */
    void helpAboutHandler();

    /**
     * Callback for when the either Mode radio button is toggled.
     */
    void modeRadioButtonToggled();

    /**
     * Callback for clicking the Pause button.
     */
    void pauseButtonHandler();

    /**
     * Callback for clicking the Play button.
     */
    void playButtonHandler();

    /**
     * Callback for closing results dialog.
     *
     * @param theResponseId
     *          response triggering the callback
     */
    void resultsDialogResponse(int theResponseId);

    /**
     * Saves simulation results to the given file.
     *
     * @param theFileName
     *          file name to save results to
     */
    void saveResults(const std::string &theFileName);

    /**
     * Helper function to set individual sensitivies of the simulation contol
     * buttons.
     *
     * @param thePlayButton
     *          play button sensitive value
     * @param thePauseButton
     *          pause button sensitive value
     * @param theStopButton
     *          stop button sensitive value
     */
    void setControlButtonSensitivities(bool thePlayButton,
                                       bool thePauseButton,
                                       bool theStopButton);

    /**
     * Sets the sensitivies for each menu item that can change
     *
     * @param theOpen
     *          File->Open sensitive
     * @param theSaveResults
     *          File->Save Results sensitive
     * @param theResults
     *          View->Results sensitive
     */
    void setMenuSensitivities(bool theOpen,
                              bool theSaveResults,
                              bool theResults);

    /**
     * Sets the sensitive value for each child in the given container
     *
     * @param theSensitive
     *          sensitive value
     */
    void setSensitivities(Gtk::Container &theContainer, bool theSensitive);

    /**
     * Callback for clicking the Stop button.
     */
    void stopButtonHandler();

    /**
     * Callback for timeout signal.
     *
     * @return true to keep timeout for another duration, false to terminate
     */
    bool timeoutCallback();

    /**
     * Updates the camera's position coordinates
     */
    void updateCameraPosition();

    /**
     * Updates the camera's zoom percentage.
     */
    void updateCameraZoom();

    /**
     * Callback for View->Results
     */
    void viewResultsHandler();

    /**
     * Callback for View->Messages
     */
    void viewMessagesHandler();

    private:

    /**
     * Callback for timer expiration.
     */
    bool timerFunction();

    /**
     * Updates the elapsed time widget with the elapsed time from the provided
     * Metrics object.
     *
     * @param theMetrics
     *          container with elapsed time
     */
    void setElapsedTime(const Metrics &theMetrics);

    /** Base directory */
    const std::string myBaseDir;

    Gtk::Box myMainBox;

    Gtk::Box myMenuBox;
    Gtk::MenuBar myMenuBar;
    Gtk::MenuItem myFileMenuItem;
    Gtk::Menu myFileSubMenu;
    Gtk::MenuItem myFileOpenMenuItem;
    Gtk::MenuItem myFileSaveResultsMenuItem;
    Gtk::MenuItem myFileExitMenuItem;
    Gtk::MenuItem myViewMenuItem;
    Gtk::Menu myViewSubMenu;
    Gtk::MenuItem myViewResultsMenuItem;
    Gtk::MenuItem myViewMessagesMenuItem;
    Gtk::MenuItem myHelpMenuItem;
    Gtk::Menu myHelpSubMenu;
    Gtk::MenuItem myHelpAboutMenuItem;

    Gtk::Frame mySimulationFrame;
    Gtk::Entry mySimulationEntry;

    Gtk::Box myModeBox;
    Gtk::Frame myModeFrame;
    Gtk::RadioButton myBatchButton;
    Gtk::RadioButton myRealTimeButton;

    Gtk::Frame mySimulationControlFrame;
    Gtk::ButtonBox mySimulationControlButtonBox;
    Gtk::Button mySimulationControlPauseButton;
    Gtk::Button mySimulationControlPlayButton;
    Gtk::Button mySimulationControlStopButton;

    Gtk::Frame mySimulationStatusFrame;
    Gtk::Box mySimulationStatusBox;
    Gtk::Label mySimulationStatusStartTimeLabel;
    Gtk::Entry mySimulationStatusStartTimeEntry;
    Gtk::Label mySimulationStatusElapsedTimeLabel;
    Gtk::Entry mySimulationStatusElapsedTimeEntry;
    Gtk::Label mySimulationStatusStopTimeLabel;
    Gtk::Entry mySimulationStatusStopTimeEntry;
    Gtk::Label mySimulationStatusProgressLabel;
    Gtk::ProgressBar mySimulationStatusProgressBar;

    Gtk::Frame myBatchFrame;
    Gtk::Box myBatchBox;
    Gtk::FileChooserDialog *myBatchFileChooserDialog;
    Gtk::Entry myBatchFileEntry;
    Gtk::Button myBatchFileButton;

    Gtk::Frame myRealTimeFrame;
    Gtk::Grid myRealTimeGrid;
    Gtk::Button myRealTimeUpButton;
    Gtk::Button myRealTimeLeftButton;
    Gtk::Button myRealTimeDownButton;
    Gtk::Button myRealTimeRightButton;
    Gtk::Label myRealTimePositionLabel;

    Gtk::Button myRealTimeZoomInButton;
    Gtk::Button myRealTimeZoomOutButton;
    Gtk::Label myRealTimeZoomLabel;

    Gtk::TextView myResultsTextView;
    Gtk::ScrolledWindow myResultsScrolledWindow;
    Gtk::Dialog myResultsDialog;

    sigc::connection myTimeoutConnection;

    /** Simulation data */
    std::unique_ptr<SimulationPackage> mySimulation;

    /** Simulation configuration file. */
    std::string mySimulationConfigFile;
  };
}
