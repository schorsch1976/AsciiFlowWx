#include "wxAsciiArt.h"

#include <wx/wx.h>

#include "ToolArrow.h"
#include "ToolClass.h"
#include "ToolErase.h"
#include "ToolFreehand.h"
#include "ToolLine.h"
#include "ToolMove.h"
#include "ToolRectangle.h"
#include "ToolResize.h"
#include "ToolText.h"

#define WHITE 255, 255, 255
#define LIGHT_GRAY 180, 180, 180
#define BLACK 0, 0, 0

wxIMPLEMENT_DYNAMIC_CLASS(wxAsciiArt, wxScrolledWindow);

// clang-format off
wxBEGIN_EVENT_TABLE(wxAsciiArt, wxScrolledWindow)

	EVT_PAINT(wxAsciiArt::OnPaint)
	EVT_SIZE(wxAsciiArt::OnResize)

	EVT_KEY_DOWN(wxAsciiArt::OnKeyPressed)
	EVT_CHAR(wxAsciiArt::OnChar)

	EVT_CHAR(wxAsciiArt::OnKeyPressed)
	EVT_LEFT_DOWN(wxAsciiArt::OnMouseBtnPressed)
	EVT_MOTION(wxAsciiArt::OnMouseMoved)
	EVT_LEFT_UP(wxAsciiArt::OnMouseBtnReleased)

wxEND_EVENT_TABLE()
	// clang-format on

	wxAsciiArt::wxAsciiArt()
	: wxScrolledWindow(nullptr, 1000, wxDefaultPosition, wxDefaultSize,
					   wxEXPAND)
{
	Init();
}

wxAsciiArt::wxAsciiArt(wxWindow *parent)
	: wxScrolledWindow(parent, 1000, wxDefaultPosition, wxDefaultSize, wxEXPAND)
{
	Init();
	Create(parent);
}
bool wxAsciiArt::Create(wxWindow *parent)
{
	SetParent(parent);
	return true;
}

// accessors...
void wxAsciiArt::Init()
{
	m_data.Register([this](wxPoint area) {
		InvalidateBestSize();
		EnsureVisibility(area);
	});

	SetMinSize(wxSize{48 * CELLSIZE, 32 * CELLSIZE});
	// SetInitialSize(DoGetBestSize());
	SetMaxSize(wxSize{65536 * CELLSIZE, 65536 * CELLSIZE});

	SetWindowStyleFlag(wxWANTS_CHARS | wxHSCROLL | wxVSCROLL);
	EnableScrolling(true, true);
	ShowScrollbars(wxScrollbarVisibility::wxSHOW_SB_ALWAYS,
				   wxScrollbarVisibility::wxSHOW_SB_ALWAYS);

	SetFocus();

	ActivateToolHelper(Tool::Text);

	m_font = wxFont(FONTSIZE, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL,
					wxFontWeight::wxFONTWEIGHT_NORMAL);
	m_btn_pressed = false;

	// prevent crashes if no callback set
	m_redo_avail_cb = [](bool) {};
	m_undo_avail_cb = [](bool) {};
}
wxSize wxAsciiArt::DoGetBestSize() const
{
	// one extra cell for increasing the area
	return wxSize{m_data.Width() * CELLSIZE + CELLSIZE,
				  m_data.Height() * CELLSIZE + CELLSIZE};
}
void wxAsciiArt::OnPaint(wxPaintEvent &evt)
{
	wxPaintDC painter(this);

	// calculate the area that we need to redraw
	// this is different from the QT version
	wxPoint ViewStart = GetViewStart();
	wxSize ClientSize = GetClientSize();
	ClientSize.x /= CELLSIZE;
	ClientSize.y /= CELLSIZE;

	painter.Clear();

	painter.SetFont(m_font);

	wxPoint TopLeft{ViewStart};
	wxPoint DownRight = TopLeft + wxPoint{ClientSize.x, ClientSize.y};
	if (DownRight.x > m_data.Width())
	{
		DownRight.x = (m_data.Width());
	}
	if (DownRight.y > m_data.Height())
	{
		DownRight.y = (m_data.Height());
	}

	// background
	{
		wxBrush brush;
		brush.SetColour(WHITE);
		brush.SetStyle(wxBrushStyle::wxBRUSHSTYLE_SOLID);
		painter.SetBrush(brush);

		wxPoint screenpos1 = TextToScreen(TopLeft);
		wxPoint screenpos2 = TextToScreen(DownRight);

		wxRect screenpos(screenpos1.x, screenpos1.y,
						 screenpos2.x - screenpos1.x,
						 screenpos2.y - screenpos1.y);
		painter.DrawRectangle(screenpos);
	}

	// background grid
	{
		wxPen pen;
		pen.SetColour(LIGHT_GRAY);
		pen.SetWidth(1);
		painter.SetPen(pen);

		for (int x = TopLeft.x; x < DownRight.x; ++x)
		{
			wxPoint screenpos1 = TextToScreen(wxPoint(x, TopLeft.y));
			wxPoint screenpos2 = TextToScreen(wxPoint(x, DownRight.y));
			painter.DrawLine(screenpos1, screenpos2);
		}

		for (int y = TopLeft.y; y < DownRight.y; ++y)
		{
			wxPoint screenpos1 = TextToScreen(wxPoint(TopLeft.x, y));
			wxPoint screenpos2 = TextToScreen(wxPoint(DownRight.x, y));
			painter.DrawLine(screenpos1, screenpos2);
		}
	}

	// selection
	{
		wxBrush hatch;
		hatch.SetStyle(wxBrushStyle::wxBRUSHSTYLE_BDIAGONAL_HATCH);
		hatch.SetColour(LIGHT_GRAY);
		painter.SetBrush(hatch);

		/*wxPen pen;
		pen.SetColour(LIGHT_GRAY);
		pen.SetWidth(1);
		painter.SetPen(pen);
		*/
		wxVector<wxRect> selected;
		if (mp_current_tool)
		{
			selected = mp_current_tool->MarkedAreas();
		}
		for (wxRect &rect : selected)
		{
			wxPoint screenpos1 = TextToScreen(wxPoint(rect.x, rect.y));
			wxPoint screenpos2 = TextToScreen(
				wxPoint(rect.x + rect.width, rect.y + rect.height));

			wxRect screenpos(screenpos1.x, screenpos1.y,
							 screenpos2.x - screenpos1.x,
							 screenpos2.y - screenpos1.y);
			painter.DrawRectangle(screenpos);
		}
	}

	// Text
	{
		wxBrush brush;
		brush.SetColour(BLACK);
		painter.SetBrush(brush);

		wxPen pen;
		pen.SetColour(BLACK);
		pen.SetWidth(1);
		painter.SetPen(pen);

		for (int y = TopLeft.y; y < DownRight.y; ++y)
		{
			for (int x = TopLeft.x; x < DownRight.x; ++x)
			{
				wxPoint screenpos = TextToScreen(wxPoint(x, y));

				// text point is at the lower line
				wxChar c = m_data.At(wxPoint(x, y));
				painter.DrawText(c, screenpos.x + 2, screenpos.y - 1);
			}
		}
	}
}

void wxAsciiArt::OnResize(wxSizeEvent &event)
{
	wxSize s = event.GetSize();
	wxLogDebug("Resized (%d|%d)", s.GetWidth(), s.GetHeight());

	SetVirtualSize(DoGetBestSize());
	SetScrollRate(CELLSIZE, CELLSIZE);

	InvalidateBestSize();
	Layout();
}

void wxAsciiArt::OnKeyPressed(wxKeyEvent &event)
{
	if (event.GetUnicodeKey() != WXK_NONE)
	{
		event.Skip();
		return;
	}

	wxLogDebug("Key Pressed");

	wxASSERT(mp_current_tool);
	mp_current_tool->OnKeyPressed(event);

	Refresh();
}

void wxAsciiArt::OnChar(wxKeyEvent &event)
{
	if (event.GetUnicodeKey() == WXK_NONE)
	{
		event.Skip();
		return;
	}

	wxLogDebug("Char Pressed");

	m_redo.clear();
	m_undo.push_back(m_data.ExportData());

	wxASSERT(mp_current_tool);
	mp_current_tool->OnKeyPressed(event);

	Refresh();
}

void wxAsciiArt::OnMouseBtnPressed(wxMouseEvent &event)
{
	m_btn_pressed = true;

	wxASSERT(mp_current_tool);
	SetFocus();

	wxPoint HitText = ScreenToText(wxPoint(event.GetX(), event.GetY()));
	wxLogDebug("Mouse Button Pressed (%d|%d)", HitText.x, HitText.y);

	m_start = HitText;

	m_redo.clear();
	m_undo.push_back(m_data.ExportData());

	wxASSERT(mp_current_tool);
	m_data_backup = m_data;
	mp_current_tool->OnMousePressed(
		ScreenToText(wxPoint(event.GetX(), event.GetY())));

	Refresh();

	// emit
	m_redo_avail_cb(m_redo.size() > 0);
	m_undo_avail_cb(m_undo.size() > 0);
}
void wxAsciiArt::OnMouseMoved(wxMouseEvent &event)
{
	if (!m_btn_pressed)
	{
		return;
	}

	wxASSERT(mp_current_tool);

	wxPoint HitText = ScreenToText(wxPoint(event.GetX(), event.GetY()));

	int width = std::abs(HitText.x - m_start.x + 1);
	int height = std::abs(HitText.y - m_start.y + 1);
	int x = std::min(HitText.x, m_start.x);
	int y = std::min(HitText.y, m_start.y);

	wxRect area(x, y, width, height);

	m_data = m_data_backup;
	mp_current_tool->OnMouseMove(area, HitText);

	Refresh();
}
void wxAsciiArt::OnMouseBtnReleased(wxMouseEvent &event)
{
	m_btn_pressed = false;

	wxASSERT(mp_current_tool);
	SetFocus();

	wxPoint HitText = ScreenToText(wxPoint(event.GetX(), event.GetY()));
	wxLogDebug("Mouse Button Released (%d|%d)", HitText.x, HitText.y);

	mp_current_tool->OnMouseReleased(HitText);

	Refresh();
}

void wxAsciiArt::ActivateToolHelper(Tool tool)
{
	m_current_tool = tool;
	switch (m_current_tool)
	{
		case Tool::Move:
			mp_current_tool.reset(new ToolMove(m_data));
			break;
		case Tool::Rectangle:
			mp_current_tool.reset(new ToolRectangle(m_data));
			break;
		case Tool::Resize:
			mp_current_tool.reset(new ToolResize(m_data));
			break;
		case Tool::Class:
			mp_current_tool.reset(new ToolClass(m_data));
			break;
		case Tool::Arrow:
			mp_current_tool.reset(new ToolArrow(m_data));
			break;
		case Tool::Line:
			mp_current_tool.reset(new ToolLine(m_data));
			break;
		case Tool::Text:
			mp_current_tool.reset(new ToolText(m_data));
			break;
		case Tool::Freehand:
			mp_current_tool.reset(new ToolFreehand(m_data));
			break;
		case Tool::Erase:
			mp_current_tool.reset(new ToolErase(m_data));
			break;
	}

	Refresh();
}

void wxAsciiArt::Clear()
{
	m_undo.clear();
	m_redo.clear();

	m_data.Clear();
	m_data_backup.Clear();

	// emit
	m_redo_avail_cb(m_redo.size() > 0);
	m_undo_avail_cb(m_undo.size() > 0);
}

wxString wxAsciiArt::ExportData() const { return m_data.ExportData(); }

void wxAsciiArt::ImportData(wxString import) { m_data.ImportData(import); }

Tool wxAsciiArt::CurrentTool() const { return m_current_tool; }

void wxAsciiArt::Undo()
{
	if (m_undo.size() > 0)
	{
		wxString tmp = *m_undo.rbegin();
		m_undo.pop_back();

		m_redo.push_back(m_data.ExportData());
		m_data.ImportData(tmp);
		Refresh();
	}

	// emit
	m_redo_avail_cb(m_redo.size() > 0);
	m_undo_avail_cb(m_undo.size() > 0);
}
void wxAsciiArt::Redo()
{
	if (m_redo.size() > 0)
	{
		wxString tmp = *m_redo.rbegin();
		m_redo.pop_back();

		m_undo.push_back(m_data.ExportData());
		m_data.ImportData(tmp);
		Refresh();
	}

	// emit
	m_redo_avail_cb(m_redo.size() > 0);
	m_undo_avail_cb(m_undo.size() > 0);
}

void wxAsciiArt::OnUndoAvailable(std::function<void(bool)> cb)
{
	m_undo_avail_cb = cb;
}
void wxAsciiArt::OnRedoAvailable(std::function<void(bool)> cb)
{
	m_redo_avail_cb = cb;
}

wxPoint wxAsciiArt::ScreenToText(wxPoint point)
{
	wxPoint ViewStart = GetViewStart();

	int x = (point.x + ViewStart.x * CELLSIZE) / CELLSIZE;
	int y = (point.y + ViewStart.y * CELLSIZE) / CELLSIZE;
	if (x < 0)
	{
		x = 0;
	}
	if (y < 0)
	{
		y = 0;
	}
	return wxPoint(x, y);
}
wxPoint wxAsciiArt::TextToScreen(wxPoint point)
{
	wxPoint ViewStart = GetViewStart();
	return wxPoint((point.x - ViewStart.x) * CELLSIZE,
				   (point.y - ViewStart.y) * CELLSIZE);
}

void wxAsciiArt::EnsureVisibility(wxPoint p)
{
	wxPoint ViewStart = GetViewStart();

	wxSize ClientSize = GetClientSize();
	ClientSize.x /= CELLSIZE;
	ClientSize.y /= CELLSIZE;

	wxPoint ViewEnd = ViewStart + ClientSize;

	wxRect rect(ViewStart, ViewEnd);
	if (rect.Contains(p))
	{
		// visible
		return;
	}

	wxPoint ScrollTarget = ViewStart;
	if (p.x < ViewStart.x)
	{
		ScrollTarget.x -= (ViewStart.x - p.x);
	}
	if (p.x >= ViewEnd.x)
	{
		ScrollTarget.x += (p.x - ViewEnd.x);
	}

	if (p.y < ViewStart.y)
	{
		ScrollTarget.y -= (ViewStart.y - p.y);
	}
	if (p.y >= ViewEnd.y)
	{
		ScrollTarget.y += (p.y - ViewEnd.y);
	}

	SetVirtualSize(DoGetBestSize());
	Scroll(ScrollTarget);
}
