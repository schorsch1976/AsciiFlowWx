#include "ToolErase.h"

ToolErase::ToolErase(AsciiArtData &data) : m_data(data) {}

void ToolErase::OnMousePressed(wxPoint /* point */) {}
void ToolErase::OnMouseMove(wxRect area, wxPoint /* point */)
{
	m_marked = area;
}
void ToolErase::OnMouseReleased(wxPoint /* point */)
{
	for (int y = m_marked.y; y < m_marked.y + m_marked.height; ++y)
	{
		for (int x = m_marked.x; x < m_marked.x + m_marked.width; ++x)
		{
			m_data.Set(x, y, WXK_SPACE);
		}
	}
	m_marked = wxRect();
}

void ToolErase::OnKeyPressed(wxKeyEvent /* c */) {}

wxVector<wxRect> ToolErase::MarkedAreas()
{
	wxVector<wxRect> ret;
	if (m_marked.width > 0 && m_marked.height > 0)
	{
		ret.push_back(m_marked);
	}
	return ret;
}
