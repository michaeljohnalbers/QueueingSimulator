/**
 * @file ControlGUI.cpp
 * @brief Defines ControlGUI class
 *
 * @author Michael Albers
 */

#include <iostream>

#include "ControlGUI.h"
#include "QSConfig.h"

QS::ControlGUI::ControlGUI()
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

  setMenuSensitivities(true, false, false);
  setControlButtonSensitivities(false, false, false);
  setSensitivities(myRealTimeGrid, false);
  setSensitivities(myBatchBox, false);

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

  myViewSummaryMenuItem.set_label("_Summary");
  myViewSummaryMenuItem.set_use_underline(true);
  myViewSummaryMenuItem.signal_activate().connect(
    sigc::mem_fun(*this, &ControlGUI::viewSummaryHandler));

  myViewMessagesMenuItem.set_label("_Messages");
  myViewMessagesMenuItem.set_use_underline(true);
  myViewMessagesMenuItem.signal_activate().connect(
    sigc::mem_fun(*this, &ControlGUI::viewMessagesHandler));

  myViewMenuItem.set_submenu(myViewSubMenu);
  myViewSubMenu.append(myViewSummaryMenuItem);
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
  myRealTimeFrame.set_label("Camera Control");

  myRealTimeUpButton.property_always_show_image() = true;
  myRealTimeUpButton.set_image_from_icon_name("go-up");
  myRealTimeUpButton.set_label("Up");

  myRealTimeLeftButton.property_always_show_image() = true;
  myRealTimeLeftButton.set_image_from_icon_name("go-previous");
  myRealTimeLeftButton.set_label("Left");

  myRealTimeDownButton.property_always_show_image() = true;
  myRealTimeDownButton.set_image_from_icon_name("go-down");
  myRealTimeDownButton.set_label("Down");

  myRealTimeRightButton.property_always_show_image() = true;
  myRealTimeRightButton.set_image_from_icon_name("go-next");
  myRealTimeRightButton.set_label("Right");

  myRealTimePositionLabel.set_text("(0,0)");

  myRealTimeZoomInButton.property_always_show_image() = true;
  myRealTimeZoomInButton.set_image_from_icon_name("zoom-in");
  myRealTimeZoomInButton.set_label("Zoom In");

  myRealTimeZoomOutButton.property_always_show_image() = true;
  myRealTimeZoomOutButton.set_image_from_icon_name("zoom-out");
  myRealTimeZoomOutButton.set_label("Zoom Out");

  myRealTimeZoomLabel.set_text("100 %");

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

void QS::ControlGUI::fileOpenHandler()
{
  Gtk::FileChooserDialog fileChooser("Select a Simulation file",
                                     Gtk::FILE_CHOOSER_ACTION_OPEN);
  fileChooser.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
  fileChooser.add_button("Open", Gtk::RESPONSE_OK);

  auto filterXml = Gtk::FileFilter::create();
  filterXml->set_name("Simulation Files");
  filterXml->add_mime_type("text/xml");
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
      // TODO: need to read simulation file, and if valid, populate fields.
      std::cout << "Selected " << fileChooser.get_filename() << std::endl;
      setControlButtonSensitivities(true, false, false);
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
  std::cout << "File->Save Results" << std::endl;
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
  show_all_children();
  // Set to such a size that the window/box/whatever is forced to expand. This
  // has the effect of shrinking the window on real-time->batch transition.
  resize(1, 1);
}

void QS::ControlGUI::pauseButtonHandler()
{
  setControlButtonSensitivities(true, false, true);
}

void QS::ControlGUI::playButtonHandler()
{
  setControlButtonSensitivities(false, true, true);
  setMenuSensitivities(false, false, false);
}

int QS::ControlGUI::run(int argc, char **argv)
{
  auto app = Gtk::Application::create(argc, argv, "gs.Main");

  ControlGUI gui;

  // Shows the window and returns when it is closed.
  return app->run(gui);
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

void QS::ControlGUI::setMenuSensitivities(bool theOpen,
                                          bool theSaveResults,
                                          bool theSummary)
{
  myFileOpenMenuItem.set_sensitive(theOpen);
  myFileSaveResultsMenuItem.set_sensitive(theSaveResults);
  myViewSummaryMenuItem.set_sensitive(theSummary);
}

void QS::ControlGUI::stopButtonHandler()
{
  setControlButtonSensitivities(true, false, false);
  setMenuSensitivities(true, true, true);
}

void QS::ControlGUI::viewSummaryHandler()
{
  std::cout << "View->Summary" << std::endl;
}

void QS::ControlGUI::viewMessagesHandler()
{
  std::cout << "View->Messages" << std::endl;
}
