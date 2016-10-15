/**
 * @file ControlGUI.cpp
 * @brief Defines ControlGUI class
 *
 * @author Michael Albers
 */

// For GNU basename (see basename(3))
#define _GNU_SORUCE
#include <cstring>
#undef _GNU_SORUCE

#include <iomanip>
#include <iostream>
#include <fstream>

#include "BatchSimulationPackage.h"
#include "ControlGUI.h"
#include "QSConfig.h"
#include "Simulation.h"
#include "RealTimeSimulationPackage.h"

QS::ControlGUI::ControlGUI(const std::string &theBaseDir) :
  myBaseDir{theBaseDir}
{
  set_title("Queueing Simulator");
  set_border_width(5);

  add(myMainBox);
  myMainBox.set_orientation(Gtk::ORIENTATION_VERTICAL);
  myMainBox.set_spacing(2);

  buildMenu(myMainBox);
  buildSimulationFrame(myMainBox);
  buildModeFrame(myMainBox);
  buildSimulationControlFrame(myMainBox);
  buildSimulationStatusFrame(myMainBox);

  buildBatch(myMainBox);
  buildRealTime(myMainBox);

  buildResultsDialog();

  setMenuSensitivities(true, false, false);
  setControlButtonSensitivities(false, false, false);
  setSensitivities(myRealTimeGrid, false);
  setSensitivities(myBatchBox, false);

  // Set initial camera values
  updateCamera();

  show_all_children();
}

QS::ControlGUI::~ControlGUI()
{
}

void QS::ControlGUI::batchFileActive()
{
  Gtk::FileChooserDialog fileChooser("Select an output file",
                                     Gtk::FILE_CHOOSER_ACTION_SAVE);
  fileChooser.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
  fileChooser.add_button("Select", Gtk::RESPONSE_OK);

  auto filterAny = Gtk::FileFilter::create();
  filterAny->set_name("All Files");
  filterAny->add_pattern("*");
  fileChooser.add_filter(filterAny);

  auto result = fileChooser.run();

  switch (result)
  {
    case Gtk::RESPONSE_OK:
      myBatchFileEntry.set_text(fileChooser.get_filename());
      break;

    case Gtk::RESPONSE_CANCEL:
    default:
      // Do nothing
      break;
  }
}

void QS::ControlGUI::buildBatch(Gtk::Container &theContainer)
{
  // Don't add the frame or Gtk warning will be written to stderr. Since
  // real-time is the default mode, this will be added when the user changes to
  // batch mode.
  //theContainer.add(myBatchFrame);

  myBatchFrame.set_label("Output File");

  myBatchFileEntry.set_activates_default(false);
  myBatchFileEntry.set_hexpand(true);
  myBatchFileEntry.set_placeholder_text("Batch Output File");

  myBatchFileButton.set_label("Select File");
  myBatchFileButton.signal_clicked().connect(
    sigc::mem_fun(*this, &ControlGUI::batchFileActive));

  myBatchFrame.add(myBatchBox);
  myBatchBox.add(myBatchFileEntry);
  myBatchBox.add(myBatchFileButton);
}

void QS::ControlGUI::buildMenu(Gtk::Container &theContainer)
{
  theContainer.add(myMenuBox);

  myFileMenuItem.set_label("_File");
  myFileMenuItem.set_use_underline(true);

  myFileOpenMenuItem.set_label("_Open");
  myFileOpenMenuItem.set_use_underline(true);
  myFileOpenMenuItem.signal_activate().connect(
    sigc::mem_fun(*this, &ControlGUI::fileOpenHandler));
  //  myFileOpenMenuItem.set_icon("document-open");

  myFileSaveResultsMenuItem.set_label("_Save Results");
  myFileSaveResultsMenuItem.set_use_underline(true);
  myFileSaveResultsMenuItem.signal_activate().connect(
    sigc::mem_fun(*this, &ControlGUI::fileSaveResultsHandler));
  //  myFileSaveResultsMenuItem.set_icon("document-save");

  myFileExitMenuItem.set_label("Exit");
  myFileExitMenuItem.set_use_underline(true);
  myFileExitMenuItem.signal_activate().connect(
    sigc::mem_fun(*this, &ControlGUI::fileExitHandler));
  //  myFileExitMenuItem.set_icon("application-exit");

  myFileMenuItem.set_submenu(myFileSubMenu);
  myFileSubMenu.append(myFileOpenMenuItem);
  myFileSubMenu.append(myFileSaveResultsMenuItem);
  myFileSubMenu.append(myFileExitMenuItem);

  myViewMenuItem.set_label("_View");
  myViewMenuItem.set_use_underline(true);

  myViewResultsMenuItem.set_label("_Results");
  myViewResultsMenuItem.set_use_underline(true);
  myViewResultsMenuItem.signal_activate().connect(
    sigc::mem_fun(*this, &ControlGUI::viewResultsHandler));

  myViewMessagesMenuItem.set_label("_Messages");
  myViewMessagesMenuItem.set_use_underline(true);
  myViewMessagesMenuItem.signal_activate().connect(
    sigc::mem_fun(*this, &ControlGUI::viewMessagesHandler));

  myViewMenuItem.set_submenu(myViewSubMenu);
  myViewSubMenu.append(myViewResultsMenuItem);
  myViewSubMenu.append(myViewMessagesMenuItem);

  myHelpMenuItem.set_label("_Help");
  myHelpMenuItem.set_use_underline(true);

  myHelpAboutMenuItem.set_label("_About");
  myHelpAboutMenuItem.set_use_underline(true);
  myHelpAboutMenuItem.signal_activate().connect(
    sigc::mem_fun(*this, &ControlGUI::helpAboutHandler));

  myHelpMenuItem.set_submenu(myHelpSubMenu);
  myHelpSubMenu.append(myHelpAboutMenuItem);

  myMenuBox.add(myMenuBar);
  myMenuBar.append(myFileMenuItem);
  myMenuBar.append(myViewMenuItem);
  myMenuBar.append(myHelpMenuItem);
}

void QS::ControlGUI::buildModeFrame(Gtk::Container &theContainer)
{
  theContainer.add(myModeFrame);
  myModeFrame.set_label("Mode");

  myRealTimeButton.set_label("Real-Time");
  myRealTimeButton.set_active(true);
  auto group = myRealTimeButton.get_group();

  myBatchButton.set_label("Batch");
  myBatchButton.set_group(group);

  // Only one signal handler is needed since there are only two radio buttons.
  // I couldn't find a generic signal handler to account for the group changing
  // so if a third radio button is added this will have to dramatically change.
  myBatchButton.signal_toggled().connect(
    sigc::mem_fun(*this, &ControlGUI::modeRadioButtonToggled));

  myModeBox.set_orientation(Gtk::ORIENTATION_VERTICAL);
  myModeFrame.add(myModeBox);

  myModeBox.add(myRealTimeButton);
  myModeBox.add(myBatchButton);
}

void QS::ControlGUI::buildRealTime(Gtk::Container &theContainer)
{
  theContainer.add(myRealTimeFrame);
  myRealTimeFrame.set_label("World View");

  myRealTimeUpButton.property_always_show_image() = true;
  myRealTimeUpButton.set_image_from_icon_name("go-up");
  myRealTimeUpButton.set_label("Up");
  myRealTimeUpButton.signal_pressed().connect(
    sigc::bind<Visualization::UserInputType>(
      sigc::mem_fun(*this, &ControlGUI::cameraMoveHandler),
      Visualization::UserInputType::CAMERA_UP));

  myRealTimeLeftButton.property_always_show_image() = true;
  myRealTimeLeftButton.set_image_from_icon_name("go-previous");
  myRealTimeLeftButton.set_label("Left");
  myRealTimeLeftButton.signal_pressed().connect(
    sigc::bind<Visualization::UserInputType>(
      sigc::mem_fun(*this, &ControlGUI::cameraMoveHandler),
      Visualization::UserInputType::CAMERA_LEFT));

  myRealTimeDownButton.property_always_show_image() = true;
  myRealTimeDownButton.set_image_from_icon_name("go-down");
  myRealTimeDownButton.set_label("Down");
  myRealTimeDownButton.signal_pressed().connect(
    sigc::bind<Visualization::UserInputType>(
      sigc::mem_fun(*this, &ControlGUI::cameraMoveHandler),
      Visualization::UserInputType::CAMERA_DOWN));

  myRealTimeRightButton.property_always_show_image() = true;
  myRealTimeRightButton.set_image_from_icon_name("go-next");
  myRealTimeRightButton.set_label("Right");
  myRealTimeRightButton.signal_pressed().connect(
    sigc::bind<Visualization::UserInputType>(
      sigc::mem_fun(*this, &ControlGUI::cameraMoveHandler),
      Visualization::UserInputType::CAMERA_RIGHT));

  myRealTimePositionLabel.set_text("(0,0)");

  myRealTimeZoomInButton.property_always_show_image() = true;
  myRealTimeZoomInButton.set_image_from_icon_name("zoom-in");
  myRealTimeZoomInButton.set_label("Zoom In");
  myRealTimeZoomInButton.signal_pressed().connect(
    sigc::bind<Visualization::UserInputType>(
      sigc::mem_fun(*this, &ControlGUI::cameraMoveHandler),
      Visualization::UserInputType::ZOOM_IN));

  myRealTimeZoomOutButton.property_always_show_image() = true;
  myRealTimeZoomOutButton.set_image_from_icon_name("zoom-out");
  myRealTimeZoomOutButton.set_label("Zoom Out");
  myRealTimeZoomOutButton.signal_pressed().connect(
    sigc::bind<Visualization::UserInputType>(
      sigc::mem_fun(*this, &ControlGUI::cameraMoveHandler),
      Visualization::UserInputType::ZOOM_OUT));

  myRealTimeZoomLabel.set_text("100.0 %");

  myRealTimeFrame.add(myRealTimeGrid);
  myRealTimeGrid.attach(myRealTimeUpButton, 1, 0, 1, 1);
  myRealTimeGrid.attach(myRealTimeLeftButton, 0, 1, 1, 1);
  myRealTimeGrid.attach(myRealTimeDownButton, 1, 2, 1, 1);
  myRealTimeGrid.attach(myRealTimeRightButton, 2, 1, 1, 1);
  myRealTimeGrid.attach(myRealTimePositionLabel, 1, 1, 1, 1);

  myRealTimeGrid.attach(myRealTimeZoomInButton, 3, 0, 1, 1);
  myRealTimeGrid.attach(myRealTimeZoomLabel, 3, 1, 1, 1);
  myRealTimeGrid.attach(myRealTimeZoomOutButton, 3, 2, 1, 1);
}

void QS::ControlGUI::buildResultsDialog()
{
  myResultsDialog.set_title("Simulation Results");
  myResultsDialog.add_button("Close", 0);
  myResultsDialog.get_content_area()->add(myResultsScrolledWindow);
  myResultsDialog.set_size_request(400, 500);

  myResultsScrolledWindow.set_vexpand(true);
  myResultsScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC,
                                     Gtk::POLICY_AUTOMATIC);
  myResultsScrolledWindow.add(myResultsTextView);

  myResultsTextView.set_editable(false);
  myResultsTextView.set_cursor_visible(false);
  myResultsTextView.set_wrap_mode(Gtk::WRAP_NONE);

  myResultsDialog.signal_response().connect(
    sigc::mem_fun(*this, &QS::ControlGUI::resultsDialogResponse));

  myResultsDialog.show_all_children();
}

void QS::ControlGUI::buildSimulationFrame(Gtk::Container &theContainer)
{
  theContainer.add(mySimulationFrame);
  mySimulationFrame.set_label("Simulation");

  mySimulationFrame.add(mySimulationEntry);
  mySimulationEntry.set_placeholder_text("<Simulation Name>");
  mySimulationEntry.set_sensitive(false);
  mySimulationEntry.property_editable() = false;
}

void QS::ControlGUI::buildSimulationControlFrame(Gtk::Container &theContainer)
{
  theContainer.add(mySimulationControlFrame);
  mySimulationControlFrame.set_label("Simulation Control");

  mySimulationControlPlayButton.property_always_show_image() = true;
  mySimulationControlPlayButton.set_image_from_icon_name(
    "media-playback-start");
  mySimulationControlPlayButton.set_label("Play");
  mySimulationControlPlayButton.signal_clicked().connect(
    sigc::mem_fun(*this, &ControlGUI::playButtonHandler));

  mySimulationControlPauseButton.property_always_show_image() = true;
  mySimulationControlPauseButton.set_image_from_icon_name(
    "media-playback-pause");
  mySimulationControlPauseButton.set_label("Pause");
  mySimulationControlPauseButton.signal_clicked().connect(
    sigc::mem_fun(*this, &ControlGUI::pauseButtonHandler));

  mySimulationControlStopButton.property_always_show_image() = true;
  mySimulationControlStopButton.set_image_from_icon_name(
    "media-playback-stop");
  mySimulationControlStopButton.set_label("Stop");
  mySimulationControlStopButton.signal_clicked().connect(
    sigc::mem_fun(*this, &ControlGUI::stopButtonHandler));

  mySimulationControlFrame.add(mySimulationControlButtonBox);
  mySimulationControlButtonBox.add(mySimulationControlPlayButton);
  mySimulationControlButtonBox.add(mySimulationControlPauseButton);
  mySimulationControlButtonBox.add(mySimulationControlStopButton);
}

void QS::ControlGUI::buildSimulationStatusFrame(Gtk::Container &theContainer)
{
  theContainer.add(mySimulationStatusFrame);
  mySimulationStatusFrame.set_label("Simulation Status");

  mySimulationStatusBox.set_orientation(Gtk::ORIENTATION_VERTICAL);
  mySimulationStatusBox.set_hexpand(true);
  mySimulationStatusBox.set_spacing(2);

  mySimulationStatusStartTimeLabel.set_text("Start Time");
  mySimulationStatusStartTimeLabel.set_halign(Gtk::Align::ALIGN_START);
  mySimulationStatusStartTimeEntry.set_placeholder_text("<Start Time>");
  mySimulationStatusStartTimeEntry.set_sensitive(false);
  mySimulationStatusStartTimeEntry.property_editable() = false;

  mySimulationStatusElapsedTimeLabel.set_text("Elapsed Time");
  mySimulationStatusElapsedTimeLabel.set_halign(Gtk::Align::ALIGN_START);
  mySimulationStatusElapsedTimeEntry.set_placeholder_text("<Elapsed Time>");
  mySimulationStatusElapsedTimeEntry.set_sensitive(false);;
  mySimulationStatusElapsedTimeEntry.property_editable() = false;

  mySimulationStatusStopTimeLabel.set_text("Stop Time");
  mySimulationStatusStopTimeLabel.set_halign(Gtk::Align::ALIGN_START);
  mySimulationStatusStopTimeEntry.set_placeholder_text("<Stop Time>");
  mySimulationStatusStopTimeEntry.set_sensitive(false);
  mySimulationStatusStopTimeEntry.property_editable() = false;

  mySimulationStatusProgressLabel.set_text("Progress");
  mySimulationStatusProgressLabel.set_halign(Gtk::Align::ALIGN_START);
  mySimulationStatusProgressBar.set_show_text(true);

  mySimulationStatusFrame.add(mySimulationStatusBox);

  mySimulationStatusBox.add(mySimulationStatusStartTimeLabel);
  mySimulationStatusBox.add(mySimulationStatusStartTimeEntry);

  mySimulationStatusBox.add(mySimulationStatusElapsedTimeLabel);
  mySimulationStatusBox.add(mySimulationStatusElapsedTimeEntry);

  mySimulationStatusBox.add(mySimulationStatusStopTimeLabel);
  mySimulationStatusBox.add(mySimulationStatusStopTimeEntry);

  mySimulationStatusBox.add(mySimulationStatusProgressLabel);
  mySimulationStatusBox.add(mySimulationStatusProgressBar);
}

void QS::ControlGUI::cameraMoveHandler(
  Visualization::UserInputType theInputType)
{
  mySimulation->getVisualization()->userInput(theInputType);
}

bool QS::ControlGUI::checkForOverwrite(const std::string &theFileName,
                                       const std::string &theDescription)
{
  bool goAhead = false;
  auto fileStatus = ::access(theFileName.c_str(), F_OK);
  if (0 == fileStatus)
  {
    std::string promptText{theDescription};
    promptText += " file, \"" + theFileName + "\" exists. Overwrite?";
    Gtk::MessageDialog questionDialog(promptText,
                                      false, Gtk::MESSAGE_QUESTION,
                                      Gtk::BUTTONS_YES_NO);
    auto response = questionDialog.run();
    if (response == Gtk::RESPONSE_YES)
    {
      goAhead = true;
    }
  }
  else
  {
    // File does not exist
    goAhead = true;
  }

  return goAhead;
}

void QS::ControlGUI::fileOpenHandler()
{
  Gtk::FileChooserDialog fileChooser("Select a Simulation file",
                                     Gtk::FILE_CHOOSER_ACTION_OPEN);
  fileChooser.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
  fileChooser.add_button("Open", Gtk::RESPONSE_OK);

  auto filterXml = Gtk::FileFilter::create();
  filterXml->set_name("Simulation Files");
  filterXml->add_pattern("*.xml");
  fileChooser.add_filter(filterXml);

  auto filterAny = Gtk::FileFilter::create();
  filterAny->set_name("All Files");
  filterAny->add_pattern("*");
  fileChooser.add_filter(filterAny);

  auto result = fileChooser.run();

  switch (result)
  {
    case Gtk::RESPONSE_OK:
    {
      setControlButtonSensitivities(true, false, false);

      mySimulationConfigFile = fileChooser.get_filename();
      std::string basename = ::basename(mySimulationConfigFile.c_str());
      mySimulationEntry.set_text(basename);
    }
    break;

    case Gtk::RESPONSE_CANCEL:
    default:
      // Do nothing
      break;
  }
}

void QS::ControlGUI::fileSaveResultsHandler()
{
  Gtk::FileChooserDialog fileChooser("Save Results",
                                     Gtk::FILE_CHOOSER_ACTION_SAVE);
  fileChooser.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
  fileChooser.add_button("Save", Gtk::RESPONSE_OK);

  auto filterAny = Gtk::FileFilter::create();
  filterAny->set_name("All Files");
  filterAny->add_pattern("*");
  fileChooser.add_filter(filterAny);

  auto result = fileChooser.run();

  switch (result)
  {
    case Gtk::RESPONSE_OK:
      saveResults(fileChooser.get_filename());
      break;

    case Gtk::RESPONSE_CANCEL:
    default:
      // Do nothing
      break;
  }
}

void QS::ControlGUI::fileExitHandler()
{
  // Hides window, causing Application::run to return, see 'run' function.
  hide();
}

void QS::ControlGUI::helpAboutHandler()
{
  Gtk::AboutDialog aboutDialog;

  // TODO: create icon for QS

  aboutDialog.set_authors({"Michael Albers"});
  aboutDialog.add_credit_section("Advisor", {"Dr. Gita Alaghband"});

  std::string version{std::to_string(QS_VERSION_MAJOR)};
  version += "." + std::to_string(QS_VERSION_MINOR);
  aboutDialog.set_version(version);

  aboutDialog.set_comments("Microscopic crowd simulation engine created for "
                           "UC Denver Computer Science Masters Project");

  aboutDialog.set_license_type(Gtk::License::LICENSE_MIT_X11);

  aboutDialog.set_website(
    "https://github.com/michaeljohnalbers/QueueingSimulator");

  aboutDialog.set_copyright("(C) 2016 Michael Albers");

  aboutDialog.run();
}

void QS::ControlGUI::modeRadioButtonToggled()
{
  if (myRealTimeButton.get_active())
  {
    myMainBox.add(myRealTimeFrame);
    myMainBox.remove(myBatchFrame);
  }
  else
  {
    myMainBox.add(myBatchFrame);
    myMainBox.remove(myRealTimeFrame);
  }

  setSensitivities(myBatchBox, myBatchButton.get_active());

  show_all_children();
  // Set to such a size that the window/box/whatever is forced to expand. This
  // has the effect of shrinking the window on real-time->batch transition.
  resize(1, 1);
}

void QS::ControlGUI::pauseButtonHandler()
{
  setControlButtonSensitivities(true, false, true);
  mySimulation->getVisualization()->userInput(
    Visualization::UserInputType::PAUSE);
}

void QS::ControlGUI::playButtonHandler()
{
  setControlButtonSensitivities(false, true, true);
  setMenuSensitivities(false, false, false);
  if (myRealTimeButton.get_active())
  {
    setSensitivities(myRealTimeGrid, true);
  }
  else
  {
    setSensitivities(myBatchBox, false);
  }

  myResultsTextView.get_buffer()->set_text("");
  myResultsDialog.hide();

  if (! mySimulation)
  {
    startSimulation();
  }
  else
  {
    // Simulation must be paused.
    mySimulation->getVisualization()->userInput(
      Visualization::UserInputType::PLAY);
  }
}

int QS::ControlGUI::run(int argc, char **argv, const std::string &theBaseDir)
{
  // Don't provide an application ID otherwise only one instance of the
  // executable can be run at one time.
  auto app = Gtk::Application::create(argc, argv);

  ControlGUI gui{theBaseDir};

  // Shows the window and returns when it is closed.
  return app->run(gui);
}

void QS::ControlGUI::resultsDialogResponse(int theResponseId)
{
  myResultsDialog.hide();
}

void QS::ControlGUI::saveResults(const std::string &theFileName)
{
  auto goAhead = checkForOverwrite(theFileName, "Metrics output");

  if (goAhead)
  {
    std::ofstream fileWriter(theFileName);
    if (fileWriter.is_open())
    {
      fileWriter << myResultsTextView.get_buffer()->get_text();
      fileWriter.close();
    }
    else
    {
      auto thisErrno = errno;
      std::string error = "Failed to open \"" + theFileName +
        "\" for saving results: ";
      // No guarantee errno is set, or set correctly. But this is the best you
      // get with fstream.
      error += std::strerror(thisErrno);

      Gtk::MessageDialog errorDialog(error, false, Gtk::MESSAGE_ERROR);
      errorDialog.run();
    }
  }
}

void QS::ControlGUI::setSensitivities(Gtk::Container &theContainer,
                                      bool theSensitive)
{
  auto on_foreach = [=](Gtk::Widget &theWidget)
  {
    theWidget.set_sensitive(theSensitive);
  };

  theContainer.foreach(on_foreach);
}

void QS::ControlGUI::setControlButtonSensitivities(bool thePlayButton,
                                                   bool thePauseButton,
                                                   bool theStopButton)
{
  mySimulationControlPlayButton.set_sensitive(thePlayButton);
  mySimulationControlPauseButton.set_sensitive(thePauseButton);
  mySimulationControlStopButton.set_sensitive(theStopButton);
}

void QS::ControlGUI::setElapsedTime(const Metrics &theMetrics)
{
  float elapsedTime = theMetrics.getElapsedTimeInSeconds();
  std::ostringstream elapsedTimeStr;
  elapsedTimeStr << std::fixed << std::setprecision(2) << elapsedTime;
  mySimulationStatusElapsedTimeEntry.set_text(elapsedTimeStr.str());
}

void QS::ControlGUI::setMenuSensitivities(bool theOpen,
                                          bool theSaveResults,
                                          bool theResults)
{
  myFileOpenMenuItem.set_sensitive(theOpen);
  myFileSaveResultsMenuItem.set_sensitive(theSaveResults);
  myViewResultsMenuItem.set_sensitive(theResults);
}

void QS::ControlGUI::startSimulation()
{
  bool realTime = myRealTimeButton.get_active();

  auto resetSensitivites = [=]()
  {
    setControlButtonSensitivities(true, false, false);
    setMenuSensitivities(true, false, false);
    if (! realTime)
    {
      setSensitivities(myBatchBox, true);
    }
  };

  try
  {
    if (realTime)
    {
      mySimulation.reset(new RealTimeSimulationPackage(mySimulationConfigFile,
                                                       myBaseDir));
    }
    else
    {
      std::string outputFile = myBatchFileEntry.get_text();
      auto goAhead = checkForOverwrite(outputFile, "Batch mode output");
      if (!goAhead)
      {
        throw 1; // Not the best practice.
      }

      mySimulation.reset(new BatchSimulationPackage(
                           mySimulationConfigFile,
                           myBaseDir,
                           outputFile));
    }
    mySimulation->startSimulation();

    auto &metrics = mySimulation->getSimulation()->getMetrics();
    std::string startTime(Metrics::asISO8601(metrics.getStartTime()));
    mySimulationStatusStartTimeEntry.set_text(startTime);
    mySimulationStatusStopTimeEntry.set_text("");
    mySimulationStatusElapsedTimeEntry.set_text("");

    // Timeout to update simulation times
    sigc::slot<bool> slot = sigc::mem_fun(
      *this, &ControlGUI::updateSimulationData);
    // Non-round number time makes elapsed time update look better.
    myUpdateSimulationConnection = Glib::signal_timeout().connect(slot, 78);

    // Timer to update the camera position values.
    auto updateCameraSlot = sigc::mem_fun(*this, &ControlGUI::updateCamera);
    myUpdateCameraConnection = Glib::signal_timeout().connect(
      updateCameraSlot, 250);
  }
  catch (const std::exception &exception)
  {
    std::string error{"Error loading simulation: "};
    error += exception.what();
    Gtk::MessageDialog errorDialog(error, false, Gtk::MESSAGE_ERROR);
    errorDialog.run();

    // Bad simulation, get rid of it.
    mySimulation.reset();
    resetSensitivites();
  }
  catch (int)
  {
    resetSensitivites();
  }
}

void QS::ControlGUI::stopButtonHandler()
{
  setControlButtonSensitivities(true, false, false);
  setMenuSensitivities(true, true, true);
  if (myRealTimeButton.get_active())
  {
    setSensitivities(myRealTimeGrid, false);
  }
  else
  {
    setSensitivities(myBatchBox, true);
  }

  myUpdateSimulationConnection.disconnect();
  myUpdateCameraConnection.disconnect();

  mySimulation->getSimulation()->getWorld().finalizeActorMetrics();
  auto &metrics = mySimulation->getSimulation()->getMetrics();
  metrics.setStopTime();

  std::string stopTime(Metrics::asISO8601(metrics.getStopTime()));
  mySimulationStatusStopTimeEntry.set_text(stopTime);

  // Need to set elapsed time as the last timeout callback probably won't have
  // caught the last updates in the simulation.
  setElapsedTime(metrics);

  std::ostringstream metricsResults;
  metricsResults << metrics;
  myResultsTextView.get_buffer()->set_text(metricsResults.str());

  mySimulation.reset();
  // Reset camera position values.
  updateCamera();
}

bool QS::ControlGUI::updateSimulationData()
{
  if (mySimulation)
  {
    auto &metrics = mySimulation->getSimulation()->getMetrics();
    setElapsedTime(metrics);
  }
  return true;
}

bool QS::ControlGUI::updateCamera()
{
  updateCameraPosition();
  updateCameraZoom();
  return true;
}

void QS::ControlGUI::updateCameraPosition()
{
  float x = 0.0, y = 0.0;
  if (mySimulation)
  {
    std::tie(x, y) = mySimulation->getVisualization()->getCameraPosition();
  }
  std::ostringstream positionString;
  int precision = 2;

  positionString << "(" << std::setprecision(precision) << std::fixed << x
                 << "," << std::setprecision(precision) << std::fixed << y
                 << ")";
  myRealTimePositionLabel.set_text(positionString.str());
}

void QS::ControlGUI::updateCameraZoom()
{
  float zoom = 100;
  if (mySimulation)
  {
    zoom = mySimulation->getVisualization()->getCameraZoom();
  }
  std::ostringstream zoomString;
  int precision = 1;

  zoomString << std::setprecision(precision) << std::fixed << zoom << " %";
  myRealTimeZoomLabel.set_text(zoomString.str());
}

void QS::ControlGUI::viewResultsHandler()
{
  myResultsDialog.show();
}

void QS::ControlGUI::viewMessagesHandler()
{
  std::cout << "View->Messages" << std::endl;
}
