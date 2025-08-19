#include "MainFrame.h"
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/artprov.h>

// Event table
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(wxID_EXIT, MainFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
    EVT_MENU(ID_Open, MainFrame::OnOpen)
    EVT_MENU(ID_Save, MainFrame::OnSave)
    EVT_SLIDER(ID_ScrollSlider, MainFrame::OnScrollChanged)
    EVT_TREE_SEL_CHANGED(ID_TreeCtrl, MainFrame::OnTreeSelChanged)
wxEND_EVENT_TABLE()

MainFrame::MainFrame()
    : wxFrame(nullptr, wxID_ANY, "Readify - Document Reader", 
              wxDefaultPosition, wxSize(1000, 700))
{
    // Create menu bar
    m_menuBar = new wxMenuBar();
    
    // File menu
    wxMenu* fileMenu = new wxMenu();
    fileMenu->Append(ID_Open, "&Open\tCtrl+O", "Open a document");
    fileMenu->Append(ID_Save, "&Save\tCtrl+S", "Save document");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, "E&xit\tAlt+X", "Quit this program");
    m_menuBar->Append(fileMenu, "&File");
    
    // Edit menu
    wxMenu* editMenu = new wxMenu();
    editMenu->Append(wxID_COPY, "&Copy\tCtrl+C", "Copy selection");
    editMenu->Append(wxID_PASTE, "&Paste\tCtrl+V", "Paste from clipboard");
    editMenu->Append(wxID_SELECTALL, "Select &All\tCtrl+A", "Select all content");
    m_menuBar->Append(editMenu, "&Edit");
    
    // View menu
    wxMenu* viewMenu = new wxMenu();
    viewMenu->Append(wxID_ZOOM_IN, "Zoom &In\tCtrl++", "Zoom in");
    viewMenu->Append(wxID_ZOOM_OUT, "Zoom &Out\tCtrl+-", "Zoom out");
    viewMenu->Append(wxID_ZOOM_100, "Zoom &100%\tCtrl+0", "Reset zoom");
    m_menuBar->Append(viewMenu, "&View");
    
    // Help menu
    wxMenu* helpMenu = new wxMenu();
    helpMenu->Append(wxID_ABOUT, "&About\tF1", "Show about dialog");
    m_menuBar->Append(helpMenu, "&Help");
    
    SetMenuBar(m_menuBar);
    
    // Create toolbar
    m_toolBar = CreateToolBar(wxTB_HORIZONTAL | wxTB_TEXT);
    m_toolBar->AddTool(ID_Open, "Open", 
                       wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR),
                       "Open document");
    m_toolBar->AddTool(ID_Save, "Save", 
                       wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_TOOLBAR),
                       "Save document");
    m_toolBar->AddSeparator();
    m_toolBar->AddTool(wxID_COPY, "Copy", 
                       wxArtProvider::GetBitmap(wxART_COPY, wxART_TOOLBAR),
                       "Copy");
    m_toolBar->AddTool(wxID_PASTE, "Paste", 
                       wxArtProvider::GetBitmap(wxART_PASTE, wxART_TOOLBAR),
                       "Paste");
    m_toolBar->Realize();
    
    // Create main panel
    wxPanel* mainPanel = new wxPanel(this, wxID_ANY);
    
    // Create splitter window for navigation and content
    m_splitter = new wxSplitterWindow(mainPanel, wxID_ANY, 
                                      wxDefaultPosition, wxDefaultSize,
                                      wxSP_3D | wxSP_LIVE_UPDATE);
    
    // Create navigation tree
    m_treeCtrl = new wxTreeCtrl(m_splitter, ID_TreeCtrl,
                                wxDefaultPosition, wxDefaultSize,
                                wxTR_DEFAULT_STYLE | wxTR_SINGLE);
    
    // Add some sample tree items
    wxTreeItemId rootId = m_treeCtrl->AddRoot("Document");
    wxTreeItemId chap1 = m_treeCtrl->AppendItem(rootId, "Chapter 1");
    m_treeCtrl->AppendItem(chap1, "Section 1.1");
    m_treeCtrl->AppendItem(chap1, "Section 1.2");
    wxTreeItemId chap2 = m_treeCtrl->AppendItem(rootId, "Chapter 2");
    m_treeCtrl->AppendItem(chap2, "Section 2.1");
    m_treeCtrl->ExpandAll();
    
    // Create web view for document content
    try {
        m_webView = wxWebView::New(m_splitter, wxID_ANY);
        m_webView->SetPage("<html><body><h1>Welcome to Readify</h1>"
                          "<p>This is a document reader application built with wxWidgets.</p>"
                          "<p>Use the File menu to open documents.</p>"
                          "<p>Navigate through the document using the tree on the left.</p>"
                          "<p>Use the scrollbar at the bottom to jump to specific parts of the document.</p>"
                          "</body></html>", "");
    } catch (...) {
        // Fallback to text control if webview is not available
        wxTextCtrl* textCtrl = new wxTextCtrl(m_splitter, wxID_ANY,
                                             "Welcome to Readify\n\n"
                                             "This is a document reader application built with wxWidgets.\n"
                                             "Use the File menu to open documents.\n"
                                             "Navigate through the document using the tree on the left.\n"
                                             "Use the scrollbar at the bottom to jump to specific parts of the document.",
                                             wxDefaultPosition, wxDefaultSize,
                                             wxTE_MULTILINE | wxTE_READONLY | wxTE_WORDWRAP);
        m_webView = nullptr;
        m_splitter->SplitVertically(m_treeCtrl, textCtrl, 250);
    }
    
    if (m_webView) {
        m_splitter->SplitVertically(m_treeCtrl, m_webView, 250);
    }
    
    // Create status bar with slider
    m_statusBar = CreateStatusBar(1);
    
    // Create a panel for the status bar content
    wxPanel* statusPanel = new wxPanel(this, wxID_ANY);
    
    // Create horizontal sizer for status bar content
    wxBoxSizer* statusSizer = new wxBoxSizer(wxHORIZONTAL);
    
    statusSizer->Add(new wxStaticText(statusPanel, wxID_ANY, "Position:"), 
                     0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    
    // Create slider for document navigation (0-100%)
    m_scrollSlider = new wxSlider(statusPanel, ID_ScrollSlider, 0, 0, 100,
                                  wxDefaultPosition, wxSize(200, -1),
                                  wxSL_HORIZONTAL | wxSL_LABELS);
    statusSizer->Add(m_scrollSlider, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    
    statusSizer->Add(new wxStaticText(statusPanel, wxID_ANY, "%"), 
                     0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    
    statusPanel->SetSizer(statusSizer);
    
    // Create main sizer
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(m_splitter, 1, wxEXPAND);
    mainPanel->SetSizer(mainSizer);
    
    // Create frame sizer
    wxBoxSizer* frameSizer = new wxBoxSizer(wxVERTICAL);
    frameSizer->Add(mainPanel, 1, wxEXPAND);
    frameSizer->Add(statusPanel, 0, wxEXPAND);
    SetSizer(frameSizer);
    
    // Set minimum pane size
    m_splitter->SetMinimumPaneSize(150);
    
    // Center the frame
    Center();
    
    // Set status bar text
    SetStatusText("Ready - Use File > Open to load a document");
}

void MainFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("Readify - Document Reader\n\n"
                 "A wxWidgets-based application for reading various document types.\n"
                 "Features:\n"
                 "- Navigation tree for document structure\n"
                 "- Document viewer with web rendering\n"
                 "- Percentage-based document navigation\n"
                 "- Modern toolbar and menu interface",
                 "About Readify", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnOpen(wxCommandEvent& event)
{
    wxFileDialog openFileDialog(this, "Open Document", "", "",
                               "All files (*.*)|*.*|"
                               "HTML files (*.html;*.htm)|*.html;*.htm|"
                               "Text files (*.txt)|*.txt",
                               wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;
    
    wxString filename = openFileDialog.GetPath();
    
    if (m_webView) {
        m_webView->LoadURL("file://" + filename);
    }
    
    SetStatusText("Opened: " + filename);
}

void MainFrame::OnSave(wxCommandEvent& event)
{
    wxMessageBox("Save functionality would be implemented here.",
                 "Save Document", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnScrollChanged(wxCommandEvent& event)
{
    int position = m_scrollSlider->GetValue();
    SetStatusText(wxString::Format("Position: %d%% - Use File > Open to load a document", position));
    
    // Here you would implement actual document scrolling based on percentage
    // For now, we just update the status
}

void MainFrame::OnTreeSelChanged(wxTreeEvent& event)
{
    wxTreeItemId item = event.GetItem();
    if (item.IsOk()) {
        wxString text = m_treeCtrl->GetItemText(item);
        SetStatusText("Selected: " + text);
        
        // Here you would implement navigation to the selected section
        // For now, we just update the status
    }
}