#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H

#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/webview.h>
#include <wx/slider.h>
#include <wx/splitter.h>

class MainFrame : public wxFrame
{
public:
    MainFrame();

private:
    // Event handlers
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnOpen(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnScrollChanged(wxCommandEvent& event);
    void OnTreeSelChanged(wxTreeEvent& event);

    // GUI components
    wxMenuBar* m_menuBar;
    wxToolBar* m_toolBar;
    wxTreeCtrl* m_treeCtrl;
    wxWebView* m_webView;
    wxStatusBar* m_statusBar;
    wxSlider* m_scrollSlider;
    wxSplitterWindow* m_splitter;

    // Menu and toolbar IDs
    enum
    {
        ID_Open = 1000,
        ID_Save,
        ID_ScrollSlider,
        ID_TreeCtrl
    };

    wxDECLARE_EVENT_TABLE();
};

#endif // MAIN_FRAME_H