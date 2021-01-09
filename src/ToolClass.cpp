#include "ToolClass.h"

ToolClass::ToolClass(AsciiArtData &data) : m_data(data) {}

void ToolClass::OnMousePressed(wxPoint /* point */) {}
void ToolClass::OnMouseMove(wxRect area, wxPoint /* point */)
{
	for (int y = area.y; y < area.y + area.height - 1; ++y)
	{
		if (y == area.y || y == area.y + area.height - 2)
		{
			for (int x = area.x; x < area.x + area.width - 1; ++x)
			{
				m_data.Set(x, y, '-');
			}
			m_data.Set(area.x, y, '+');
			m_data.Set(area.x + area.width - 1, y, '+');
		}
		else
		{
			m_data.Set(area.x, y, '|');
			m_data.Set(area.x + area.width - 1, y, '|');
		}
	}

	if (area.height > 2)
	{
		for (int x = area.x; x < area.x + area.width - 1; ++x)
		{
			m_data.Set(x, area.y + 2, '-');
		}
		m_data.Set(area.x, area.y + 2, '+');
		m_data.Set(area.x + area.width - 1, area.y + 2, '+');
	}
}
void ToolClass::OnMouseReleased(wxPoint /* point */) {}

void ToolClass::OnKeyPressed(wxKeyEvent /* c */) {}

wxVector<wxRect> ToolClass::MarkedAreas() { return wxVector<wxRect>(); }
