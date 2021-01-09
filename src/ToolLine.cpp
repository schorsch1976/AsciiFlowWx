#include "ToolLine.h"

ToolLine::ToolLine(AsciiArtData &data) : m_data(data) {}

void ToolLine::OnMousePressed(wxPoint point) { m_start = point; }
void ToolLine::OnMouseMove(wxRect /* area */, wxPoint point)
{
	int start_x = m_start.x;
	int start_y = m_start.y;

	int end_x = point.x;
	int end_y = point.y;

	if (start_x == end_x && start_y == end_y)
	{
		// on start
		m_data.Set(start_x, start_y, '+');
	}
	else if (start_x > end_x && start_y == end_y)
	{
		// left
		for (int x = start_x; x >= end_x; --x)
		{
			m_data.Set(x, start_y, '-');
		}
		m_data.Set(start_x, start_y, '+');
		m_data.Set(end_x, start_y, '+');
	}
	else if (start_x < end_x && start_y == end_y)
	{
		// right
		for (int x = start_x; x <= end_x; ++x)
		{
			m_data.Set(x, start_y, '-');
		}
		m_data.Set(start_x, start_y, '+');
		m_data.Set(end_x, start_y, '+');
	}
	else if (start_x == end_x && start_y < end_y)
	{
		// down
		for (int y = start_y; y <= end_y; ++y)
		{
			m_data.Set(start_x, y, '|');
		}
		m_data.Set(start_x, start_y, '+');
		m_data.Set(start_x, end_y, '+');
	}
	else if (start_x == end_x && start_y > end_y)
	{
		// up
		for (int y = start_y; y >= end_y; --y)
		{
			m_data.Set(start_x, y, '|');
		}
		m_data.Set(start_x, start_y, '+');
		m_data.Set(start_x, end_y, '+');
	}
	else
	{
		// general case
		// first left/right, then up/down
		int dir_x = start_x < end_x ? 1 : -1;
		int dir_y = start_y < end_y ? 1 : -1;

		for (int x = start_x; x != end_x; x += dir_x)
		{
			m_data.Set(x, start_y, '-');
		}
		for (int y = start_y; y != end_y; y += dir_y)
		{
			m_data.Set(end_x, y, '|');
		}

		if (dir_y > 0)
		{
			m_data.Set(end_x, end_y, '+');
		}
		else if (dir_y < 0)
		{
			m_data.Set(end_x, end_y, '+');
		}

		m_data.Set(start_x, start_y, '+');
		m_data.Set(end_x, start_y, '+');
	}
}
void ToolLine::OnMouseReleased(wxPoint /* point */) {}

void ToolLine::OnKeyPressed(wxKeyEvent /* c */) {}

wxVector<wxRect> ToolLine::MarkedAreas() { return wxVector<wxRect>(); }
