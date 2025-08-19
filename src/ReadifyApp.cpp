#include "ReadifyApp.h"
#include "MainFrame.h"

// Declare the application instance
wxIMPLEMENT_APP(ReadifyApp);

bool ReadifyApp::OnInit()
{
    // Create the main frame window
    MainFrame* frame = new MainFrame();
    frame->Show(true);
    
    return true;
}