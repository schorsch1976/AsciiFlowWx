#include "ToolText.h"

ToolText::ToolText(AsciiArtData &data) : m_data(data), m_cursor{0, 0} {}

void ToolText::OnMousePressed(wxPoint point) { m_cursor = point; }
void ToolText::OnMouseMove(wxRect /* area */, wxPoint /*point*/) {}
void ToolText::OnMouseReleased(wxPoint /* point */) {}

void ToolText::OnKeyPressed(wxKeyEvent event)
{
	wxChar uc = event.GetUnicodeKey();
	if (uc != WXK_NONE)
	{
		if (wxIsprint(uc))
		{
			m_data.Set(m_cursor.x, m_cursor.y, uc);
			m_cursor.x += 1;
		}
		else if (uc == WXK_BACK)
		{
			m_cursor.x -= 1;
			m_data.Set(m_cursor.x, m_cursor.y, WXK_SPACE);
		}
	}
	else
	{
		// special key
		switch (event.GetKeyCode())
		{
			case WXK_LEFT:
				m_cursor.x -= 1;
				break;
			case WXK_RIGHT:
				m_cursor.x += 1;
				break;
			case WXK_UP:
				m_cursor.y -= 1;
				break;
			case WXK_DOWN:
				m_cursor.y += 1;
				break;
		}
	}

	// sanity check
	if (m_cursor.x < 0)
	{
		m_cursor.x = 0;
	}
	if (m_cursor.y < 0)
	{
		m_cursor.y = 0;
	}

	if (m_cursor.x >= m_data.Width())
	{
		m_data.Set(m_cursor.x, m_cursor.y, WXK_SPACE);
	}
	if (m_cursor.y > m_data.Height())
	{
		m_data.Set(m_cursor.x, m_cursor.y, WXK_SPACE);
	}
}

wxVector<wxRect> ToolText::MarkedAreas()
{
	wxVector<wxRect> ret;
	ret.push_back(wxRect(m_cursor.x, m_cursor.y, 1, 1));
	return ret;
}
