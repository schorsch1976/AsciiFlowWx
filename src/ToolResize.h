#pragma once

#include "AsciiArtData.h"
#include "ITool.h"

///////////////////////////////////////////////////////////////////////////////
// Tool Resize
///////////////////////////////////////////////////////////////////////////////
class ToolResize : public ITool
{
public:
	explicit ToolResize(AsciiArtData &data);
	virtual void OnMousePressed(wxPoint point) override;
	virtual void OnMouseMove(wxRect area, wxPoint point) override;
	virtual void OnMouseReleased(wxPoint point) override;

	virtual void OnKeyPressed(wxKeyEvent c) override;

	virtual wxVector<wxRect> MarkedAreas() override;

private:
	bool Find(int dx, int dy, wxPoint start, wxPoint &res);
	void Selected(wxVector<wxPoint> &target, wxPoint p1, wxPoint p2, wxPoint p3,
				  wxPoint p4);

	void DrawRectangle(wxVector<wxPoint> &edge_points, wxPoint p1, wxPoint p2);

	AsciiArtData &m_data;
	wxPoint m_start;

	wxVector<wxPoint> m_move_1;
	wxVector<wxPoint> m_move_2;
	wxVector<wxPoint> m_move_3;
	wxVector<wxPoint> m_move_4;
};
