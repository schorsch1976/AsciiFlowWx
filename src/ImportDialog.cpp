#include "ImportDialog.h"

#include <wx/filedlg.h>
#include <wx/stattext.h>
#include <wx/wfstream.h>

ImportDialog::ImportDialog(std::function<void(wxString)> imported_cb)
	: wxDialog(NULL, -1, "Import...", wxDefaultPosition, wxSize(250, 230)),
	  m_imported_cb(imported_cb)
{
	wxBoxSizer *mainsizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *hbox_just = new wxBoxSizer(wxHORIZONTAL);
	hbox_just->AddStretchSpacer(1);
	hbox_just->Add(
		new wxStaticText(this, 0, "Just copy from your clipboard..."));
	hbox_just->AddStretchSpacer(1);
	mainsizer->Add(hbox_just, 0, wxEXPAND | wxCENTRE | wxALL, 5);

	mp_text_ctrl = new wxTextCtrl(this, 0, wxEmptyString, wxDefaultPosition,
								  wxSize(400, 300),
								  wxTE_MULTILINE | wxHSCROLL | wxVSCROLL);
	mainsizer->Add(mp_text_ctrl, 1, wxEXPAND | wxALL, 5);

	wxButton *ok_btn = new wxButton(this, 0, "Ok");
	mainsizer->Add(ok_btn, 0, wxEXPAND | wxCENTRE | wxALL, 5);
	ok_btn->Bind(wxEVT_BUTTON, [this](wxCommandEvent &) { OnOkButton(); });

	wxBoxSizer *hbox_or = new wxBoxSizer(wxHORIZONTAL);
	hbox_or->AddStretchSpacer(1);
	hbox_or->Add(new wxStaticText(this, 0, "or"));
	hbox_or->AddStretchSpacer(1);
	mainsizer->Add(hbox_or, 0, wxEXPAND | wxCENTRE | wxALL, 5);

	wxButton *load_btn = new wxButton(this, 0, "Load from file ...");
	mainsizer->Add(load_btn, 0, wxEXPAND | wxCENTRE | wxALL, 5);
	SetSizerAndFit(mainsizer);

	load_btn->Bind(wxEVT_BUTTON, [this](wxCommandEvent &) { OnFileImport(); });
}

void ImportDialog::OnFileImport()
{
	wxFileDialog loadDlg(this, ("Load TXT1 file"), "", "",
						 "TXT files (*.txt)|*.txt", wxFD_OPEN);

	if (wxID_CANCEL == loadDlg.ShowModal())
	{
		return;
	}

	// save the current contents in the file;
	// this can be done with e.g. wxWidgets output streams:
	wxFileInputStream input_stream(loadDlg.GetPath());
	if (!input_stream.IsOk())
	{
		wxLogError("Cannot load file '%s'.", loadDlg.GetPath());
		return;
	}
	wxString data;
	input_stream.GetFile()->ReadAll(&data, wxConvAuto());
	mp_text_ctrl->SetValue(data);
}

void ImportDialog::OnOkButton()
{
	wxString data = mp_text_ctrl->GetValue();
	m_imported_cb(data);
	Close(true);
}
