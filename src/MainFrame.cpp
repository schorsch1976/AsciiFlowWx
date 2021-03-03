#include "MainFrame.h"

#include "AboutDialog.h"
#include "ExportDialog.h"
#include "ImportDialog.h"
#include "PreferencesDialog.h"

#include <wx/gdicmn.h>
#include <wx/msgdlg.h>

#include "ico/About.png.xpm"
#include "ico/Arrow.png.xpm"
#include "ico/Class.png.xpm"
#include "ico/Erase.png.xpm"
#include "ico/Export.png.xpm"
#include "ico/Freehand.png.xpm"
#include "ico/Import.png.xpm"
#include "ico/Line.png.xpm"
#include "ico/Move.png.xpm"
#include "ico/New.png.xpm"
#include "ico/Prefs.png.xpm"
#include "ico/Rectangle.png.xpm"
#include "ico/Redo.png.xpm"
#include "ico/Resize.png.xpm"
#include "ico/Text.png.xpm"
#include "ico/Undo.png.xpm"

MainFrame::MainFrame(const wxString &title, const wxPoint &pos,
					 const wxSize &size)
	: wxFrame(NULL, wxID_ANY, title, pos, size)
{
	wxBoxSizer *mainsizer = new wxBoxSizer(wxVERTICAL);

	//////////////////////////////////////////////////////////////////////////////
	// Top Button line
	//////////////////////////////////////////////////////////////////////////////
	wxBoxSizer *topsizer = new wxBoxSizer(wxHORIZONTAL);
	m_btns[Btn::New] = new wxButton(this, ID_ButtonNew, "New");
	m_btns[Btn::Import] = new wxButton(this, ID_ButtonImport, "Import");
	m_btns[Btn::Export] = new wxButton(this, ID_ButtonExport, "Export");
	m_btns[Btn::Undo] = new wxButton(this, ID_ButtonUndo, "Undo");
	m_btns[Btn::Redo] = new wxButton(this, ID_ButtonRedo, "Redo");
	m_btns[Btn::Prefs] =
		new wxButton(this, ID_ButtonPreferences, "Preferences");
	m_btns[Btn::About] = new wxButton(this, ID_ButtonAbout, "About");

	topsizer->Add(m_btns[Btn::New], 0, wxEXPAND | wxALIGN_LEFT, 5);
	topsizer->Add(m_btns[Btn::Import], 0, wxEXPAND | wxALIGN_LEFT, 5);
	topsizer->Add(m_btns[Btn::Export], 0, wxEXPAND | wxALIGN_LEFT, 5);

	topsizer->Add(m_btns[Btn::Undo], 0, wxALIGN_LEFT, 5);
	topsizer->Add(m_btns[Btn::Redo], 0, wxALIGN_LEFT, 5);

	topsizer->AddStretchSpacer(1);

	topsizer->Add(m_btns[Btn::Prefs], 0, wxALIGN_RIGHT, 5);
	topsizer->Add(m_btns[Btn::About], 0, wxALIGN_RIGHT, 5);

	mainsizer->Add(topsizer, 0, wxEXPAND | wxALL, 5);

	//////////////////////////////////////////////////////////////////////////////
	// Mid Line (including drawing area)
	//////////////////////////////////////////////////////////////////////////////
	wxBoxSizer *midsizer = new wxBoxSizer(wxHORIZONTAL);
	mainsizer->Add(midsizer, 1, wxEXPAND | wxALL, 5);

	//////////////////////////////////////////////////////////////////////////////
	// Mid Left Button line
	//////////////////////////////////////////////////////////////////////////////
	m_tool2btn[Tool::Arrow] =
		new wxToggleButton(this, ID_ButtonToolArrow, "Arrow");
	m_tool2btn[Tool::Class] =
		new wxToggleButton(this, ID_ButtonToolClass, "Class");
	m_tool2btn[Tool::Erase] =
		new wxToggleButton(this, ID_ButtonToolErase, "Erase");
	m_tool2btn[Tool::Freehand] =
		new wxToggleButton(this, ID_ButtonToolFreehand, "Freehand");
	m_tool2btn[Tool::Line] =
		new wxToggleButton(this, ID_ButtonToolLine, "Line");
	m_tool2btn[Tool::Move] =
		new wxToggleButton(this, ID_ButtonToolMove, "Move");
	m_tool2btn[Tool::Rectangle] =
		new wxToggleButton(this, ID_ButtonToolRectangle, "Rectangle");
	m_tool2btn[Tool::Resize] =
		new wxToggleButton(this, ID_ButtonToolResize, "Resize");
	m_tool2btn[Tool::Text] =
		new wxToggleButton(this, ID_ButtonToolText, "Text");

	m_btns[Btn::Move] = m_tool2btn[Tool::Move];
	m_btns[Btn::Rectangle] = m_tool2btn[Tool::Rectangle];
	m_btns[Btn::Resize] = m_tool2btn[Tool::Resize];
	m_btns[Btn::Class] = m_tool2btn[Tool::Class];
	m_btns[Btn::Arrow] = m_tool2btn[Tool::Arrow];
	m_btns[Btn::Line] = m_tool2btn[Tool::Line];
	m_btns[Btn::Text] = m_tool2btn[Tool::Text];
	m_btns[Btn::Freehand] = m_tool2btn[Tool::Freehand];
	m_btns[Btn::Erase] = m_tool2btn[Tool::Erase];

	wxBoxSizer *leftsizer = new wxBoxSizer(wxVERTICAL);
	leftsizer->Add(m_tool2btn[Tool::Move]);
	leftsizer->Add(m_tool2btn[Tool::Rectangle]);
	leftsizer->Add(m_tool2btn[Tool::Resize]);
	leftsizer->Add(m_tool2btn[Tool::Class]);
	leftsizer->Add(m_tool2btn[Tool::Arrow]);
	leftsizer->Add(m_tool2btn[Tool::Line]);
	leftsizer->Add(m_tool2btn[Tool::Text]);
	leftsizer->Add(m_tool2btn[Tool::Erase]);
	leftsizer->Add(m_tool2btn[Tool::Freehand]);

	midsizer->Add(leftsizer, 0, wxEXPAND | wxALL, 5);

	//////////////////////////////////////////////////////////////////////////////
	// Mid: Drawing area
	//////////////////////////////////////////////////////////////////////////////
	mp_asciiart = new wxAsciiArt(this);
	wxASSERT(mp_asciiart);
	midsizer->Add(mp_asciiart, 1, wxEXPAND | wxALL, 5);

	SetSizerAndFit(mainsizer); // use the sizer for layout and size window
							   // accordingly and prevent it from being resized
							   // to smaller size

	Maximize();

	//////////////////////////////////////////////////////////////////////////////
	// Connect the callbacks (undo/redo avail)
	//////////////////////////////////////////////////////////////////////////////
	mp_asciiart->OnUndoAvailable(
		[this](bool avail) { m_btns[Btn::Undo]->Enable(avail); });
	mp_asciiart->OnRedoAvailable(
		[this](bool avail) { m_btns[Btn::Redo]->Enable(avail); });
	m_btns[Btn::Undo]->Enable(false);
	m_btns[Btn::Redo]->Enable(false);
	ActivateToolHelper(Tool::Text);

	LoadBitmaps();
	ApplyPrefs();
}

void MainFrame::OnClose(wxCloseEvent &event)
{
	wxASSERT(mp_asciiart);
	wxString exported = mp_asciiart->ExportData();
	if (exported.size())
	{
		if (wxYES !=
			wxMessageBox("Data area contains data", "Are you sure?",
						 wxYES_NO | wxCENTRE | wxICON_QUESTION | wxNO_DEFAULT))
		{
			event.Veto();
			return;
		}
	}

	Destroy();
}

void MainFrame::LoadBitmaps()
{
	m_bitmap[Btn::About] = wxBITMAP(About);
	m_bitmap[Btn::Arrow] = wxBITMAP(Arrow);
	m_bitmap[Btn::Class] = wxBITMAP(Class);
	m_bitmap[Btn::Erase] = wxBITMAP(Erase);
	m_bitmap[Btn::Export] = wxBITMAP(Export);
	m_bitmap[Btn::Freehand] = wxBITMAP(Freehand);
	m_bitmap[Btn::Import] = wxBITMAP(Import);
	m_bitmap[Btn::Line] = wxBITMAP(Line);
	m_bitmap[Btn::Move] = wxBITMAP(Move);
	m_bitmap[Btn::New] = wxBITMAP(New);
	m_bitmap[Btn::Prefs] = wxBITMAP(Prefs);
	m_bitmap[Btn::Rectangle] = wxBITMAP(Rectangle);
	m_bitmap[Btn::Redo] = wxBITMAP(Redo);
	m_bitmap[Btn::Resize] = wxBITMAP(Resize);
	m_bitmap[Btn::Text] = wxBITMAP(Text);
	m_bitmap[Btn::Undo] = wxBITMAP(Undo);
}

void MainFrame::ApplyPrefs()
{
	PreferencesDialog prefs;
	int icon_size = prefs.GetIconSize();
	wxLogDebug("IconSize: %d", icon_size);
	ButtonStyle style = prefs.GetButtonStyle();

	wxSize target_size(icon_size, icon_size);

	if (m_labels.empty())
	{
		for (auto &c : m_btns)
		{
			m_labels[c.first] = c.second->GetLabel();
		}
	}

	for (auto &c : m_btns)
	{
		// shorter
		Btn btn = c.first;
		wxAnyButton *p_button = c.second;

		// calculate the new one
		wxBitmap &original_bmp = m_bitmap[btn];
		wxImage original_image = original_bmp.ConvertToImage();

		wxImage scaled_image =
			original_image.Scale(target_size.GetWidth(),
								 target_size.GetHeight(), wxIMAGE_QUALITY_HIGH);
		wxBitmap scaled_bmp(scaled_image);

		p_button->SetBitmap(wxNullBitmap);
		switch (style)
		{
			case ButtonStyle::IconOnly:
				p_button->SetBitmap(scaled_bmp);
				p_button->SetBitmapPosition(wxDirection::wxUP);
				p_button->SetLabel("");
				break;

			case ButtonStyle::TextOnly:
				p_button->SetBitmap(wxNullBitmap);
				break;

			case ButtonStyle::TextBesidesIcon:
				p_button->SetBitmap(scaled_bmp);
				p_button->SetBitmapPosition(wxDirection::wxLEFT);
				p_button->SetLabel(m_labels[btn]);
				break;

			default:
			case ButtonStyle::TextUnderIcon:
				p_button->SetBitmap(scaled_bmp);
				p_button->SetBitmapPosition(wxDirection::wxUP);
				p_button->SetLabel(m_labels[btn]);
				break;
		}
		p_button->Refresh();
		p_button->Update();
	}

	// force a new Layout
	Layout();
}

////////////////////////////////////////////////////////////////////////////////
// File stuff
////////////////////////////////////////////////////////////////////////////////
void MainFrame::OnFileNew(wxCommandEvent &event)
{
	wxASSERT(mp_asciiart);
	wxString exported = mp_asciiart->ExportData();
	if (exported.size())
	{
		if (wxYES ==
			wxMessageBox("Data area contains data", "Are you sure?",
						 wxYES_NO | wxCENTRE | wxICON_QUESTION | wxNO_DEFAULT))
		{
			mp_asciiart->Clear();
		}
	}
}
void MainFrame::OnImport(wxCommandEvent &event)
{
	auto cb = [this](wxString data) {
		if (data.size())
		{
			mp_asciiart->ImportData(data);
		}
	};

	ImportDialog dlg(cb);
	dlg.ShowModal();
}
void MainFrame::OnExport(wxCommandEvent &event)
{
	ExportDialog dlg(mp_asciiart->ExportData());
	dlg.ShowModal();
}

void MainFrame::OnUndo(wxCommandEvent &event)
{
	wxASSERT(mp_asciiart);
	mp_asciiart->Undo();
}
void MainFrame::OnRedo(wxCommandEvent &event)
{
	wxASSERT(mp_asciiart);
	mp_asciiart->Redo();
}

void MainFrame::OnPreferences(wxCommandEvent &event)
{
	{
		PreferencesDialog dlg;
		dlg.ShowModal();
	}

	ApplyPrefs();
}
void MainFrame::OnAbout(wxCommandEvent &event)
{
	AboutDialog dlg;
	dlg.ShowModal();
}

////////////////////////////////////////////////////////////////////////////////
// Tools
////////////////////////////////////////////////////////////////////////////////
void MainFrame::OnToolMove(wxCommandEvent &event)
{
	ActivateToolHelper(Tool::Move);
}
void MainFrame::OnToolRectangle(wxCommandEvent &event)
{
	ActivateToolHelper(Tool::Rectangle);
}
void MainFrame::OnToolResize(wxCommandEvent &event)
{
	ActivateToolHelper(Tool::Resize);
}
void MainFrame::OnToolClass(wxCommandEvent &event)
{
	ActivateToolHelper(Tool::Class);
}
void MainFrame::OnToolArrow(wxCommandEvent &event)
{
	ActivateToolHelper(Tool::Arrow);
}
void MainFrame::OnToolLine(wxCommandEvent &event)
{
	ActivateToolHelper(Tool::Line);
}
void MainFrame::OnToolText(wxCommandEvent &event)
{
	ActivateToolHelper(Tool::Text);
}
void MainFrame::OnToolErase(wxCommandEvent &event)
{
	ActivateToolHelper(Tool::Erase);
}
void MainFrame::OnToolFreehand(wxCommandEvent &event)
{
	ActivateToolHelper(Tool::Freehand);
}

void MainFrame::ActivateToolHelper(Tool tool)
{
	wxASSERT(mp_asciiart);
	mp_asciiart->ActivateToolHelper(tool);

	for (auto &c : m_tool2btn)
	{
		c.second->SetValue(false);
	}

	wxASSERT(m_tool2btn[tool]);
	m_tool2btn[tool]->SetValue(true);
}

// clang-format off
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_CLOSE(MainFrame::OnClose)
	
	EVT_BUTTON(ID_ButtonNew, 			MainFrame::OnFileNew)
	EVT_BUTTON(ID_ButtonImport, 		MainFrame::OnImport)
	EVT_BUTTON(ID_ButtonExport, 		MainFrame::OnExport)

	EVT_BUTTON(ID_ButtonUndo,			MainFrame::OnUndo)
	EVT_BUTTON(ID_ButtonRedo,			MainFrame::OnRedo)

	EVT_BUTTON(ID_ButtonPreferences,	MainFrame::OnPreferences)
	EVT_BUTTON(ID_ButtonAbout,			MainFrame::OnAbout)

	EVT_TOGGLEBUTTON(ID_ButtonToolMove,		MainFrame::OnToolMove)
	EVT_TOGGLEBUTTON(ID_ButtonToolRectangle,MainFrame::OnToolRectangle)
	EVT_TOGGLEBUTTON(ID_ButtonToolResize, 	MainFrame::OnToolResize)
	EVT_TOGGLEBUTTON(ID_ButtonToolClass,	MainFrame::OnToolClass)
	EVT_TOGGLEBUTTON(ID_ButtonToolArrow,	MainFrame::OnToolArrow)
	EVT_TOGGLEBUTTON(ID_ButtonToolLine,		MainFrame::OnToolLine)
	EVT_TOGGLEBUTTON(ID_ButtonToolText,		MainFrame::OnToolText)
	EVT_TOGGLEBUTTON(ID_ButtonToolErase,	MainFrame::OnToolErase)
	EVT_TOGGLEBUTTON(ID_ButtonToolFreehand,	MainFrame::OnToolFreehand)
wxEND_EVENT_TABLE()
	// clang-format on
