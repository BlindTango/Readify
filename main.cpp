#include <wx/wx.h>
#include <wx/html/htmlwin.h>
#include <wx/treectrl.h>
#include <wx/splitter.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/artprov.h>
#include <wx/dir.h>

class ReadifyApp : public wxApp
{
public:
    virtual bool OnInit();
};

class ReadifyFrame : public wxFrame
{
public:
    ReadifyFrame();

private:
    void OnOpen(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnBack(wxCommandEvent& event);
    void OnForward(wxCommandEvent& event);
    void OnTreeItemSelected(wxTreeEvent& event);
    
    void LoadHtmlFile(const wxString& filepath);
    void PopulateTree();
    void SetupUI();
    
    wxHtmlWindow* m_htmlWindow;
    wxTreeCtrl* m_treeCtrl;
    wxSplitterWindow* m_splitter;
    wxMenuBar* m_menuBar;
    wxToolBar* m_toolBar;
    wxStatusBar* m_statusBar;
    
    wxString m_assetsPath;
    
    enum
    {
        ID_Open = 1000,
        ID_Back,
        ID_Forward,
        ID_Tree
    };
    
    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(ReadifyFrame, wxFrame)
    EVT_MENU(ID_Open, ReadifyFrame::OnOpen)
    EVT_MENU(wxID_EXIT, ReadifyFrame::OnExit)
    EVT_MENU(wxID_ABOUT, ReadifyFrame::OnAbout)
    EVT_TOOL(ID_Back, ReadifyFrame::OnBack)
    EVT_TOOL(ID_Forward, ReadifyFrame::OnForward)
    EVT_TREE_SEL_CHANGED(ID_Tree, ReadifyFrame::OnTreeItemSelected)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(ReadifyApp);

bool ReadifyApp::OnInit()
{
    ReadifyFrame* frame = new ReadifyFrame();
    frame->Show(true);
    return true;
}

ReadifyFrame::ReadifyFrame()
    : wxFrame(NULL, wxID_ANY, "Readify - Document Reader")
{
    SetSize(1000, 700);
    SetMinSize(wxSize(600, 400));
    
    // Determine assets path
    wxString exePath = wxStandardPaths::Get().GetExecutablePath();
    wxFileName exeFile(exePath);
    m_assetsPath = exeFile.GetPath() + wxFileName::GetPathSeparator() + "assets";
    
    SetupUI();
    PopulateTree();
    
    // Load index.html at startup if available
    wxString indexPath = m_assetsPath + wxFileName::GetPathSeparator() + "index.html";
    if (wxFileName::FileExists(indexPath))
    {
        LoadHtmlFile(indexPath);
    }
    else
    {
        m_htmlWindow->SetPage("<html><body><h1>Welcome to Readify</h1><p>Please open an HTML file to begin reading.</p></body></html>");
    }
    
    m_statusBar->SetStatusText("Ready");
}

void ReadifyFrame::SetupUI()
{
    // Create menu bar
    m_menuBar = new wxMenuBar();
    
    wxMenu* fileMenu = new wxMenu();
    fileMenu->Append(ID_Open, "&Open\tCtrl+O", "Open an HTML file");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, "E&xit\tCtrl+Q", "Exit the application");
    
    wxMenu* helpMenu = new wxMenu();
    helpMenu->Append(wxID_ABOUT, "&About\tF1", "About Readify");
    
    m_menuBar->Append(fileMenu, "&File");
    m_menuBar->Append(helpMenu, "&Help");
    SetMenuBar(m_menuBar);
    
    // Create toolbar
    m_toolBar = CreateToolBar(wxTB_HORIZONTAL | wxTB_TEXT);
    m_toolBar->AddTool(ID_Open, "Open", wxArtProvider::GetBitmap(wxART_FILE_OPEN), "Open HTML file");
    m_toolBar->AddSeparator();
    m_toolBar->AddTool(ID_Back, "Back", wxArtProvider::GetBitmap(wxART_GO_BACK), "Go back (disabled)");
    m_toolBar->AddTool(ID_Forward, "Forward", wxArtProvider::GetBitmap(wxART_GO_FORWARD), "Go forward (disabled)");
    
    // Disable back/forward buttons as they are no-ops
    m_toolBar->EnableTool(ID_Back, false);
    m_toolBar->EnableTool(ID_Forward, false);
    
    m_toolBar->Realize();
    
    // Create status bar
    m_statusBar = CreateStatusBar();
    
    // Create splitter window
    m_splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D | wxSP_LIVE_UPDATE);
    
    // Create tree control for navigation
    m_treeCtrl = new wxTreeCtrl(m_splitter, ID_Tree, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE | wxTR_SINGLE);
    
    // Create HTML window
    m_htmlWindow = new wxHtmlWindow(m_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_SCROLLBAR_AUTO);
    
    // Split the window
    m_splitter->SplitVertically(m_treeCtrl, m_htmlWindow, 200);
    m_splitter->SetMinimumPaneSize(150);
    
    // Create sizer for the frame
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(m_splitter, 1, wxEXPAND);
    SetSizer(sizer);
}

void ReadifyFrame::PopulateTree()
{
    m_treeCtrl->DeleteAllItems();
    
    wxTreeItemId root = m_treeCtrl->AddRoot("Documents");
    
    // Check if assets directory exists
    if (wxFileName::DirExists(m_assetsPath))
    {
        wxDir dir(m_assetsPath);
        if (dir.IsOpened())
        {
            wxString filename;
            bool cont = dir.GetFirst(&filename, "*.html", wxDIR_FILES);
            while (cont)
            {
                m_treeCtrl->AppendItem(root, filename);
                cont = dir.GetNext(&filename);
            }
        }
    }
    else
    {
        m_treeCtrl->AppendItem(root, "No assets found");
    }
    
    m_treeCtrl->Expand(root);
}

void ReadifyFrame::LoadHtmlFile(const wxString& filepath)
{
    if (wxFileName::FileExists(filepath))
    {
        m_htmlWindow->LoadPage(filepath);
        m_statusBar->SetStatusText("Loaded: " + wxFileName(filepath).GetFullName());
    }
    else
    {
        wxMessageBox("File not found: " + filepath, "Error", wxOK | wxICON_ERROR);
    }
}

void ReadifyFrame::OnOpen(wxCommandEvent& event)
{
    wxFileDialog dialog(this, "Open HTML file", "", "", "HTML files (*.html;*.htm)|*.html;*.htm|All files (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    
    if (dialog.ShowModal() == wxID_OK)
    {
        LoadHtmlFile(dialog.GetPath());
    }
}

void ReadifyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void ReadifyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("Readify - A simple document reader\n\nBuilt with wxWidgets and wxHtmlWindow", "About Readify", wxOK | wxICON_INFORMATION);
}

void ReadifyFrame::OnBack(wxCommandEvent& event)
{
    // No-op as specified
}

void ReadifyFrame::OnForward(wxCommandEvent& event)
{
    // No-op as specified
}

void ReadifyFrame::OnTreeItemSelected(wxTreeEvent& event)
{
    wxTreeItemId item = event.GetItem();
    if (item.IsOk() && item != m_treeCtrl->GetRootItem())
    {
        wxString filename = m_treeCtrl->GetItemText(item);
        if (filename != "No assets found")
        {
            wxString filepath = m_assetsPath + wxFileName::GetPathSeparator() + filename;
            LoadHtmlFile(filepath);
        }
    }
}