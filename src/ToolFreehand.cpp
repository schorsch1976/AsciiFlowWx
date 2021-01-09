#include "ToolFreehand.h"

ToolFreehand::ToolFreehand(AsciiArtData &data) : m_data(data) {}

void ToolFreehand::OnMousePressed(wxPoint point)
{
	m_spots.clear();
	m_spots.push_back(point);
	Draw();
}
void ToolFreehand::OnMouseMove(wxRect /* area */, wxPoint point)
{
	m_spots.push_back(point);
	Draw();
}
void ToolFreehand::OnMouseReleased(wxPoint point)
{
	m_spots.push_back(point);
	Draw();
	m_spots.clear();
}

void ToolFreehand::OnKeyPressed(wxKeyEvent /* c */) {}

wxVector<wxRect> ToolFreehand::MarkedAreas() { return wxVector<wxRect>(); }

void ToolFreehand::Draw()
{
	for (int i = 0; i < static_cast<int>(m_spots.size()); ++i)
	{
		m_data.Set(m_spots[i], 'X');
	}
}
