//==============================================================================

#include "MainApplication.h"
#include "MainWindow.h"
#include "MainComponent.h"

/// MainMenuBarModel provides the application menubar's command set. On the Mac
/// the menubar will be installed at the top of the screen and on Windows/Linux
/// it will be at the top of the window.
struct MainApplication::MainMenuBarModel : public MenuBarModel {
  
  /// The MainMenuBarModel constructor.
  /// Your method should call the function setApplicationCommandManagerToWatch()
  /// and pass it a pointer to the application's commandManager instance.
  MainMenuBarModel() {
  }
  
  
  /// This method returns a juce::StringArray containing all the menubar's menu names.
  /// Our app has a single menu, called "Texts".
  StringArray getMenuBarNames() override {
    return {"Texts"};
  }
  
  /// This method returns a juce::PopupMenu filled with the menu command items
  /// for a given menuName passed to it.
  /// When this method receives the menuName "Texts" it should take the following steps:
  /// * Allocate a PopupMenu on the stack.
  /// * For each text id LATIN_TEXT, CYRILLIC_TEXT and GREEK_TEXT call
  /// juce::PopupMenu.addCommandItem() and pass it a pointer to the app's
  /// commandManager and the current text id.
  /// * Return the menu.
  PopupMenu getMenuForIndex (int, const String& menuName) override {
    PopupMenu menu;
    menu.addCommandItem(&getApp().commandManager, MainApplication::LATIN_TEXT);
    menu.addCommandItem(&getApp().commandManager, MainApplication::CYRILLIC_TEXT);
    menu.addCommandItem(&getApp().commandManager, MainApplication::GREEK_TEXT);
    
    return menu;
  }
  
  // Our app uses a command manager so this function should be empty.
  void menuItemSelected (int menuItemID, int /*topLevelMenuIndex*/) override {
    
  }
};

//==============================================================================

void MainApplication::getAllCommands (juce::Array<juce::CommandID> &commands) {
    const CommandID ids[] = {
    LATIN_TEXT, CYRILLIC_TEXT, GREEK_TEXT};
  commands.addArray(ids, numElementsInArray(ids)); //!!
}

void MainApplication::getCommandInfo (juce::CommandID commandID, juce::ApplicationCommandInfo &result) {
    if(commandID == LATIN_TEXT){
        result.setInfo("Latin", "", "", 0);
    } else if (commandID == CYRILLIC_TEXT){
        result.setInfo("Cyrillic", "", "", 0);
    } else if (commandID == GREEK_TEXT){
        result.setInfo("Greek", "","", 0);
    }

}

bool MainApplication::perform (const InvocationInfo &info) { //!!
    auto component = mainWindow->getContentComponent();
    auto mainComponent = dynamic_cast<MainComponent*>(component);
    mainComponent->setTextForId(info.commandID);
    return true;  // Return true if the command was handled
}

MainApplication& MainApplication::getApp() {
  MainApplication* const app = dynamic_cast<MainApplication*>(JUCEApplication::getInstance());
  assert(app != nullptr);
  return *app;
}

MenuBarModel* MainApplication::getMenuBarModel() {
  return mainMenuBarModel.get();
}

//==============================================================================
// JUCEApplication overrides

MainApplication::MainApplication() {
}

const String MainApplication::getApplicationName() {
    return ProjectInfo::projectName;
}

const String MainApplication::getApplicationVersion() {
    return ProjectInfo::versionString;
}

bool MainApplication::moreThanOneInstanceAllowed() {
  return false;
}

void MainApplication::initialise(const String& commandLine) {
  // Set mainMenuBarModel to a new MainMenuBarModel
    mainMenuBarModel = std::make_unique<MainMenuBarModel>();

  // Call registerAllCommandsForTarget to add all the application
  // commands. See getAllCommands(), getCommandInfo().
    commandManager.registerAllCommandsForTarget(this);

  // On mac add the app's menubar to the top of the screen.
  // (On windows and linux it is added to the main window.)
    #if JUCE_MAC
        MenuBarModel::setMacMainMenu(MainApplication::getApp().getMenuBarModel());
    #endif 
    
  // Create our MainWindow.
  mainWindow = std::make_unique<MainWindow>(getApplicationName());
}

void MainApplication::shutdown() {
    #if JUCE_WINDOWS || JUCE_LINUX
    setMenuBar(nullptr);
    #else
    MenuBarModel::setMacMainMenu(nullptr);
    #endif
}

void MainApplication::systemRequestedQuit() {
  quit();
}

void MainApplication::anotherInstanceStarted(const String& commandLine) {
}

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (MainApplication)
