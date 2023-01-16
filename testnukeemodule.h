#ifndef TESTNUKEEMODULE_H
#define TESTNUKEEMODULE_H
#include <stdio.h>
#include <string>
#include <iostream>

#include <interface/NUKEEInteface.h>
#include <imgui/imgui.h>
#include <interface/EditorInstance.h>

// NOTE, that singletons are created for different modules. Use AppInstance-><singleton> to get access to engines singletons!
// To check it, you can try next code:
//
// cout << "KeyBoard Singleton for this module: " << KeyBoard::getSingleton() << endl;
// cout << "KeyBoard Singleton for main module: " << instance->keyboard << endl;
//
// OR
//
// ASSERT(KeyBoard::getSingleton() == instance->keyboard)
//

using namespace std;

#define MODULE_TITLE    "Test NukeEngine module"
#define MODULE_AUTHOR   "ExQDev Team"
#define MODULE_DESC     "Module that shows ability of NukeEngine to load custom modules. Just experimental."
#define MODULE_VERSION  "1.0.0.0"
#define MODULE_SITE     "https://exqdev.org"

#define MSG             "==> MESSAGE FROM TEST NUKE MODULE! <=="
#define MSG_ON_KB       "Message invoked from KBoard, key: [M]"
#define MSG_ON_CLOSE    "!! ON CLOSE [TEST MODULE] !!"

#define WINDOW_TITLE    "Test Nuke Window"
#define WINDOW_BODY_TXT "This is a test window for NukeEngine sample plugin. You can make your own windows, but be careful with designing and other functions!\n Remember, that these functions are done in loop, so, be sure you are using background threads for heavy functions!\n\nThats all! Enjoy!:)"
#define WINDOW_KEY      "testnukemodule-testwin"

#define MENU_PATH       "Plugins/TestPlugin"
#define MENU_MSG_PATH   "Message"
#define MENU_WIN_PATH   "Toggle test window"


// ============== Callbacks ================

void Message(){
    cout << MSG << endl;
}

void MsgPrint(unsigned char c, int x, int y){
    if(c == 'm')
        cout << MSG_ON_KB << endl;
}

void OnClose(){
    cout << MSG_ON_CLOSE << endl;
}

// =========================================

bool testWindow = false;
// Draw our window
void TestWindow(){
    // Restrict drawing if it is off
    if(!testWindow)
        return;

    // Use ImGui API
    ImGui::Begin(WINDOW_TITLE, &testWindow, 0);
    ImGui::TextWrapped(WINDOW_BODY_TXT);
    ImGui::End();
}

// Toggle our window
void toggleTestWindow(){
    testWindow = !testWindow;
}

struct TestNUKEEModule : public NUKEModule
{
    // Set up your data in constructor, before plugin wiil be loaded. It is needed.
    TestNUKEEModule()
        {
            strcpy(title, MODULE_TITLE);
            strcpy(author, MODULE_AUTHOR);
            strcpy(description, MODULE_DESC);
            strcpy(version, MODULE_VERSION);
            strcpy(site, MODULE_SITE);
        }

        // On run method, we are getting App instance, which is can be a game, or an editor. Check please instance, before using. App extensions can be runned in Editor, but not backwards.
        void Run(AppInstance* instance)
        {
            // Set up the local instance, for background processes, e.g.
            this->instance = instance;

            // Set up [stopped] variable, to know if we are running. Use this for cycles in background
            stopped = false;

            // Address of main renderer. Note, that if you want use App renderer or another singleton, you need to get it from passed instance, do not use GetSingleton directly - there will be escepcially module's singleton, local.
            cout << "Module Render: " << instance->render << endl;
            if(NukeBGFX* render = dynamic_cast<NukeBGFX*>(instance->render)){
                // Setting up a callback
                render->setOnClose(OnClose);
            }

            // Check if passed instance is editor, then...
            if(instance->isEditor()){

                cout << "==>> Instance is editor" << endl;

                // Lets get editor singleton
                EditorInstance* instan = ((EditorInstance*)instance);
                cout << "===>> Inctance casted" << endl;

                // Append editor menu strip to interact with user
                instan->menuStrip->AddItem(MENU_PATH, MENU_MSG_PATH, &Message);
                instan->menuStrip->AddItem(MENU_PATH, MENU_WIN_PATH, &toggleTestWindow);
                cout << "====>> MenuStrip appended" << endl;

                // Push our window to interact with user more efficiently
                instan->PushWindow(WINDOW_KEY, &TestWindow);

                cout << "=====>> Window pushed" << endl;
            }

            // Add our callback to keyboard handler, when we will press 'M', it will print our message
            *instance->keyboard += MsgPrint;

            // All done
            cout << "TestNuke plugin loaded" << endl;
        }

        // Thats for plugin manager, if it is true, plugin manager can call function, that toggles settings window (like "Test Nuke Window", described above, but which has our settings)
        bool HasSettings()
        {
            // Example. If false, button won't be available
            return true;
        }

        // This function should toggle settings window, but it just prints a message... (You can fix it, using TestWindow function as template for settings window function, and toggleTestWindow function as template for this function)
        void Settings()
        {
            printf("Test Nuke Settings\n");
        }

        // This function will be called when user decided to unload this plugin or is quitting the application. Stop here all your processes and save progress if you don't want to loose it.
        void Shutdown()
        {
            // We will remove all registered menus and windows due to unload. After unload they will break editor, so be careful: always unreginser your menus and windows.
            auto editor = ((EditorInstance*)instance);
            auto menu = editor->menuStrip;
            menu->RemoveItem(string(MENU_PATH) + MENU_MSG_PATH);    // Use FULL path to delete node
            menu->RemoveItem(string(MENU_PATH) + MENU_WIN_PATH);
            editor->PopWindow(WINDOW_KEY);            // Use window KEY to unregister window
            stopped = true;                                         // Set up stopped to false
            cout << "TestNuke plugin unloaded" << endl;             // All done
        }
};


NUKEENGINE_API TestNUKEEModule plugin;                                         // EVERY plugin MUST HAVE this two lines with YOUR plugin module class
TestNUKEEModule plugin;

#endif // TESTNUKEEMODULE_H
