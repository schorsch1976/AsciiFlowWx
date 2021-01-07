#pragma once

#include <wx/wx.h>

enum
{
	ID_Hello = 1
};

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

private:
	void OnHello(wxCommandEvent &event);
	void OnExit(wxCommandEvent &event);
	void OnAbout(wxCommandEvent &event);

	wxDECLARE_EVENT_TABLE();
};
