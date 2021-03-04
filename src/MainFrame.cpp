#include "MainFrame.h"

#include "AboutDialog.h"
#include "ExportDialog.h"
#include "ImportDialog.h"
#include "PreferencesDialog.h"

#include <wx/gdicmn.h>
#include <wx/msgdlg.h>

namespace XPM
{
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
} // namespace XPM

MainFrame::MainFrame(const wxString &title, const wxPoint &pos,
					 const wxSize &size)
	: wxFrame(NULL, wxID_ANY, title, pos, size), mp_top_toolbar(nullptr),
	  mp_left_toolbar(nullptr)
{
	CreateTopToolbar();
	CreateLeftToolbar();

	//////////////////////////////////////////////////////////////////////////////
	// Create the layout
	//////////////////////////////////////////////////////////////////////////////
	wxBoxSizer *mainsizer = new wxBoxSizer(wxVERTICAL);
	mainsizer->Add(mp_top_toolbar, 0, wxEXPAND | wxALL);

	//////////////////////////////////////////////////////////////////////////////
	// Mid Line (including drawing area)
	//////////////////////////////////////////////////////////////////////////////
	wxBoxSizer *midsizer = new wxBoxSizer(wxHORIZONTAL);
	mainsizer->Add(midsizer, 0, wxEXPAND | wxALL);
	midsizer->Add(mp_left_toolbar, 0, wxEXPAND | wxALL);

	// also adds the tool buttons
	LoadBitmaps();
	ApplyPrefs();

	//////////////////////////////////////////////////////////////////////////////
	// Mid: Drawing area
	//////////////////////////////////////////////////////////////////////////////
	mp_asciiart = new wxAsciiArt(this);
	wxASSERT(mp_asciiart);
	midsizer->Add(mp_asciiart, 1, wxEXPAND | wxALL);

	SetSizerAndFit(mainsizer); // use the sizer for layout and size window
							   // accordingly and prevent it from being resized
							   // to smaller size

	Maximize();

	ActivateToolHelper(Tool::Text);

	//////////////////////////////////////////////////////////////////////////////
	// Connect the callbacks (undo/redo avail)
	//////////////////////////////////////////////////////////////////////////////
	wxASSERT(mp_asciiart);
	mp_asciiart->OnUndoAvailable([this](bool avail) {
		mp_top_toolbar->EnableTool(ID_ButtonUndo, avail);
	});
	mp_asciiart->OnRedoAvailable([this](bool avail) {
		mp_top_toolbar->EnableTool(ID_ButtonRedo, avail);
	});
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
	// we always use XPMs on all platforms
	m_raw_bitmap[Btn::About] = wxBitmap(XPM::About_xpm, wxBITMAP_TYPE_XPM);
	m_raw_bitmap[Btn::Arrow] = wxBitmap(XPM::Arrow_xpm, wxBITMAP_TYPE_XPM);
	m_raw_bitmap[Btn::Class] = wxBitmap(XPM::Class_xpm, wxBITMAP_TYPE_XPM);
	m_raw_bitmap[Btn::Erase] = wxBitmap(XPM::Erase_xpm, wxBITMAP_TYPE_XPM);
	m_raw_bitmap[Btn::Export] = wxBitmap(XPM::Export_xpm, wxBITMAP_TYPE_XPM);
	m_raw_bitmap[Btn::Freehand] =
		wxBitmap(XPM::Freehand_xpm, wxBITMAP_TYPE_XPM);
	m_raw_bitmap[Btn::Import] = wxBitmap(XPM::Import_xpm, wxBITMAP_TYPE_XPM);
	m_raw_bitmap[Btn::Line] = wxBitmap(XPM::Line_xpm, wxBITMAP_TYPE_XPM);
	m_raw_bitmap[Btn::Move] = wxBitmap(XPM::Move_xpm, wxBITMAP_TYPE_XPM);
	m_raw_bitmap[Btn::New] = wxBitmap(XPM::New_xpm, wxBITMAP_TYPE_XPM);
	m_raw_bitmap[Btn::Prefs] = wxBitmap(XPM::Prefs_xpm, wxBITMAP_TYPE_XPM);
	m_raw_bitmap[Btn::Rectangle] =
		wxBitmap(XPM::Rectangle_xpm, wxBITMAP_TYPE_XPM);
	m_raw_bitmap[Btn::Redo] = wxBitmap(XPM::Redo_xpm, wxBITMAP_TYPE_XPM);
	m_raw_bitmap[Btn::Resize] = wxBitmap(XPM::Resize_xpm, wxBITMAP_TYPE_XPM);
	m_raw_bitmap[Btn::Text] = wxBitmap(XPM::Text_xpm, wxBITMAP_TYPE_XPM);
	m_raw_bitmap[Btn::Undo] = wxBitmap(XPM::Undo_xpm, wxBITMAP_TYPE_XPM);
}
void MainFrame::ResizeBitmaps(int size)
{
	wxSize target_size(size, size);
	for (auto &c : m_raw_bitmap)
	{
		Btn btn = c.first;

		// calculate the new one
		const wxBitmap &original_bmp = c.second;
		wxImage original_image = original_bmp.ConvertToImage();

		wxImage scaled_image =
			original_image.Scale(target_size.GetWidth(),
								 target_size.GetHeight(), wxIMAGE_QUALITY_HIGH);

		m_resized_bitmap[btn] = wxBitmap(scaled_image);
	}
}

void MainFrame::ApplyPrefs()
{
	PreferencesDialog prefs;
	int icon_size = prefs.GetIconSize();
	wxLogDebug("IconSize: %d", icon_size);
	ButtonStyle style = prefs.GetButtonStyle();

	wxASSERT(mp_top_toolbar && mp_left_toolbar);
	mp_top_toolbar->ClearTools();
	mp_left_toolbar->ClearTools();

	ResizeBitmaps(icon_size);
	wxSize target_size(icon_size, icon_size);
	mp_top_toolbar->SetToolBitmapSize(target_size);
	mp_left_toolbar->SetToolBitmapSize(target_size);

	AddTopTools();
	AddLeftTools();

	long top_style = mp_top_toolbar->GetWindowStyle();
	long left_style = mp_left_toolbar->GetWindowStyle();

	switch (style)
	{
		case ButtonStyle::IconOnly:
			top_style &= ~wxTB_TEXT;
			top_style &= ~wxTB_NOICONS;
			top_style &= ~wxTB_HORZ_LAYOUT;

			left_style &= ~wxTB_TEXT;
			left_style &= ~wxTB_NOICONS;
			left_style &= ~wxTB_HORZ_LAYOUT;
			break;

		case ButtonStyle::TextOnly:
			top_style |= wxTB_TEXT;
			top_style |= wxTB_NOICONS;
			top_style &= ~wxTB_HORZ_LAYOUT;

			left_style |= wxTB_TEXT;
			left_style |= wxTB_NOICONS;
			left_style &= ~wxTB_HORZ_LAYOUT;
			break;

		case ButtonStyle::TextBesidesIcon:
			top_style |= wxTB_TEXT;
			top_style &= ~wxTB_NOICONS;
			top_style |= wxTB_HORZ_LAYOUT;

			left_style |= wxTB_TEXT;
			left_style &= ~wxTB_NOICONS;
			left_style |= wxTB_HORZ_LAYOUT;
			break;

		default:
		case ButtonStyle::TextUnderIcon:
			top_style |= wxTB_TEXT;
			top_style &= ~wxTB_NOICONS;
			top_style &= ~wxTB_HORZ_LAYOUT;

			left_style |= wxTB_TEXT;
			left_style &= ~wxTB_NOICONS;
			left_style &= ~wxTB_HORZ_LAYOUT;
			break;
	}
	mp_top_toolbar->SetWindowStyle(top_style);
	mp_left_toolbar->SetWindowStyle(left_style);

	mp_top_toolbar->Realize();
	mp_left_toolbar->Realize();

#if 0
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
		const wxBitmap &original_bmp = m_bitmap[btn];
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
#endif
	// force a new Layout
	Refresh();
	Layout();
}

void MainFrame::CreateTopToolbar()
{
	long style = wxTB_FLAT | wxTB_DOCKABLE | wxTB_TEXT;
	style &= ~(wxTB_HORIZONTAL | wxTB_VERTICAL | wxTB_BOTTOM | wxTB_RIGHT |
			   wxTB_HORZ_LAYOUT);
	style |= wxTB_TOP;

	if (mp_top_toolbar)
	{
		delete mp_top_toolbar;
	}
	mp_top_toolbar =
		new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, style);
	wxASSERT(mp_top_toolbar);
}

void MainFrame::CreateLeftToolbar()
{
	long style = wxTB_FLAT | wxTB_DOCKABLE | wxTB_TEXT;
	style &= ~(wxTB_HORIZONTAL | wxTB_VERTICAL | wxTB_BOTTOM | wxTB_RIGHT |
			   wxTB_HORZ_LAYOUT);
	style |= wxTB_LEFT;

	if (mp_left_toolbar)
	{
		delete mp_left_toolbar;
	}
	mp_left_toolbar =
		new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, style);
	wxASSERT(mp_left_toolbar);
}

void MainFrame::AddTopTools()
{
	wxASSERT(mp_top_toolbar);

	mp_top_toolbar->AddTool(ID_ButtonNew, "New", m_resized_bitmap[Btn::New],
							wxNullBitmap, wxITEM_NORMAL, "Create new file");

	mp_top_toolbar->AddTool(ID_ButtonImport, "Import",
							m_resized_bitmap[Btn::Import], wxNullBitmap,
							wxITEM_NORMAL, "Import file or clipboard");

	mp_top_toolbar->AddTool(ID_ButtonExport, "Export",
							m_resized_bitmap[Btn::Export], wxNullBitmap,
							wxITEM_NORMAL, "Export to file or clipboard");

	mp_top_toolbar->AddTool(ID_ButtonUndo, "Undo", m_resized_bitmap[Btn::Undo],
							wxNullBitmap, wxITEM_NORMAL, "Revert a change");

	mp_top_toolbar->AddTool(ID_ButtonRedo, "Redo", m_resized_bitmap[Btn::Redo],
							wxNullBitmap, wxITEM_NORMAL, "Redo a change");

	mp_top_toolbar->AddTool(ID_ButtonPreferences, "Preferences",
							m_resized_bitmap[Btn::Prefs], wxNullBitmap,
							wxITEM_NORMAL, "Open Preference dialog");

	mp_top_toolbar->AddTool(ID_ButtonAbout, "About",
							m_resized_bitmap[Btn::About], wxNullBitmap,
							wxITEM_NORMAL, "Open the About dialog");

	mp_top_toolbar->Realize();

	wxASSERT(mp_top_toolbar);
	mp_top_toolbar->EnableTool(ID_ButtonUndo, false);
	mp_top_toolbar->EnableTool(ID_ButtonRedo, false);
}

void MainFrame::AddLeftTools()
{
	wxASSERT(mp_left_toolbar);
	mp_left_toolbar->AddRadioTool(ID_ButtonToolMove, "Arrow",
								  m_resized_bitmap[Btn::Arrow]);
	mp_left_toolbar->AddRadioTool(ID_ButtonToolRectangle, "Rectangle",
								  m_resized_bitmap[Btn::Rectangle]);
	mp_left_toolbar->AddRadioTool(ID_ButtonToolResize, "Resize",
								  m_resized_bitmap[Btn::Resize]);
	mp_left_toolbar->AddRadioTool(ID_ButtonToolClass, "Class",
								  m_resized_bitmap[Btn::Class]);
	mp_left_toolbar->AddRadioTool(ID_ButtonToolArrow, "Arrow",
								  m_resized_bitmap[Btn::Arrow]);
	mp_left_toolbar->AddRadioTool(ID_ButtonToolLine, "Line",
								  m_resized_bitmap[Btn::Line]);
	mp_left_toolbar->AddRadioTool(ID_ButtonToolText, "Text",
								  m_resized_bitmap[Btn::Text]);
	mp_left_toolbar->AddRadioTool(ID_ButtonToolErase, "Erase",
								  m_resized_bitmap[Btn::Erase]);
	mp_left_toolbar->AddRadioTool(ID_ButtonToolFreehand, "Freehand",
								  m_resized_bitmap[Btn::Freehand]);

	mp_left_toolbar->Realize();
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
#if 0
	for (auto &c : m_tool2btn)
	{
		c.second->SetValue(false);
	}

	wxASSERT(m_tool2btn[tool]);
	m_tool2btn[tool]->SetValue(true);
#endif
}

// clang-format off
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_CLOSE(MainFrame::OnClose)
	
	EVT_TOOL(ID_ButtonNew, 			MainFrame::OnFileNew)
	EVT_TOOL(ID_ButtonImport, 		MainFrame::OnImport)
	EVT_TOOL(ID_ButtonExport, 		MainFrame::OnExport)

	EVT_TOOL(ID_ButtonUndo,			MainFrame::OnUndo)
	EVT_TOOL(ID_ButtonRedo,			MainFrame::OnRedo)

	EVT_TOOL(ID_ButtonPreferences,	MainFrame::OnPreferences)
	EVT_TOOL(ID_ButtonAbout,		MainFrame::OnAbout)

	EVT_TOOL(ID_ButtonToolMove,		MainFrame::OnToolMove)
	EVT_TOOL(ID_ButtonToolRectangle,MainFrame::OnToolRectangle)
	EVT_TOOL(ID_ButtonToolResize, 	MainFrame::OnToolResize)
	EVT_TOOL(ID_ButtonToolClass,	MainFrame::OnToolClass)
	EVT_TOOL(ID_ButtonToolArrow,	MainFrame::OnToolArrow)
	EVT_TOOL(ID_ButtonToolLine,		MainFrame::OnToolLine)
	EVT_TOOL(ID_ButtonToolText,		MainFrame::OnToolText)
	EVT_TOOL(ID_ButtonToolErase,	MainFrame::OnToolErase)
	EVT_TOOL(ID_ButtonToolFreehand,	MainFrame::OnToolFreehand)

wxEND_EVENT_TABLE()
	// clang-format on
