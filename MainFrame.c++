#ifndef MainFrame_H
#define MainFrame_H
#include <wx/wx.h>
class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title);
private :
    wxTimer* Typingtimer;
    wxStaticText* timelabel;
    wxTextCtrl* inputtext;
    wxStaticText* text;
    wxStaticText* accuracy;
    wxStaticText* totwords;
    wxString Typetext;
    int Remainingtime;
    void UpdateStats();
    bool timerStarted;
    void OnTypingStart(wxCommandEvent& event);
    void OnTimerTick(wxTimerEvent& event);

};
#endif

