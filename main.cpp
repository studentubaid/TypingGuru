#include <wx/wx.h>
#include "MainFrame.h"
#include "MyApp.h"
#include <string>
#include <algorithm>

using namespace std;

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MainFrame* frame = new MainFrame("Typing Master Pro");
    frame->SetClientSize(800, 600);
    frame->Center();
    frame->Show(true);
    return true;
}

MainFrame::MainFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition)
{
    wxPanel* panel = new wxPanel(this, wxID_ANY);

    text = new wxStaticText(panel, wxID_ANY,
        "Type this text:\n\nThe quick brown fox jumps over the lazy dog and ran very fast.");

    inputtext = new wxTextCtrl(panel, wxID_ANY, "",
        wxDefaultPosition, wxDefaultSize,
        wxTE_MULTILINE | wxBORDER_THEME);

    totwords = new wxStaticText(panel, wxID_ANY, "WPM: 0");
    accuracy = new wxStaticText(panel, wxID_ANY, "Accuracy: 0%");
    timelabel = new wxStaticText(panel, wxID_ANY, "01:00");

    wxFont font(20, wxFONTFAMILY_DEFAULT,
        wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

    text->SetFont(font);
    inputtext->SetFont(font);
    totwords->SetFont(font);
    accuracy->SetFont(font);
    timelabel->SetFont(font);

    wxBoxSizer* timebox = new wxBoxSizer(wxHORIZONTAL);
    timebox->AddStretchSpacer(1);
    timebox->Add(timelabel, 0, wxALL | wxALIGN_CENTER, 20);

    wxBoxSizer* contentbox = new wxBoxSizer(wxVERTICAL);
    contentbox->Add(text, 0, wxALL | wxALIGN_LEFT, 15);
    contentbox->Add(inputtext, 1, wxALL | wxEXPAND, 25);

    wxBoxSizer* statsbox = new wxBoxSizer(wxHORIZONTAL);
    statsbox->Add(totwords, 0, wxALL | wxALIGN_LEFT, 15);
    statsbox->AddStretchSpacer(1);
    statsbox->Add(accuracy, 0, wxALL | wxALIGN_CENTER, 15);

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(timebox, 0, wxEXPAND);
    mainSizer->Add(contentbox, 1, wxEXPAND);
    mainSizer->Add(statsbox, 0, wxEXPAND);

    panel->SetSizer(mainSizer);

    Remainingtime = 60;
    timerStarted = false;

    Typingtimer = new wxTimer(this);

    Bind(wxEVT_TIMER, &MainFrame::OnTimerTick, this);
    inputtext->Bind(wxEVT_TEXT, &MainFrame::OnTypingStart, this);
}

void MainFrame::OnTypingStart(wxCommandEvent& event)
{
    if (!timerStarted)
    {
        timerStarted = true;
        Typingtimer->Start(1000);
    }

    UpdateStats();
}

void MainFrame::OnTimerTick(wxTimerEvent& event)
{
    if (Remainingtime > 0)
    {
        Remainingtime--;

        timelabel->SetLabel(wxString::Format("%02d:%02d",
            Remainingtime / 60,
            Remainingtime % 60));
    }
    else
    {
        Typingtimer->Stop();
        timelabel->SetLabel("Time Up");
        inputtext->Enable(false);
    }

    UpdateStats();
}

void MainFrame::UpdateStats()
{
    string givtext =
        "The quick brown fox jumps over the lazy dog and ran very fast.";

    string typedText = string(inputtext->GetValue().mb_str());

    // -------- CORRECT ACCURACY LOGIC --------
    int correctChars = 0;

    int compareLength = min(typedText.length(), givtext.length());

    for (int i = 0; i < compareLength; i++)
    {
        if (typedText[i] == givtext[i])
            correctChars++;
    }

    double accuracyPercent = 0.0;

    if (typedText.length() > 0)
    {
        accuracyPercent =
            (correctChars * 100.0) / typedText.length();
    }

    accuracy->SetLabel(
        wxString::Format("Accuracy: %.2f%%", accuracyPercent));

    // -------- WPM --------
    int characters = typedText.length();

    double elapsedMinutes =
        (60 - Remainingtime) / 60.0;

    double wpm = 0.0;

    if (elapsedMinutes > 0)
    {
        wpm = (characters / 5.0) / elapsedMinutes;
    }

    totwords->SetLabel(
        wxString::Format("WPM: %.2f", wpm));
}