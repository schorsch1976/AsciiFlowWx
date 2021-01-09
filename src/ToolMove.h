#pragma once

#include "AsciiArtData.h"
#include "ITool.h"

///////////////////////////////////////////////////////////////////////////////
// Tool Move
///////////////////////////////////////////////////////////////////////////////
class ToolMove : public ITool
{
public:
	explicit ToolMove(AsciiArtData &data);
	virtual void OnMousePressed(wxPoint point) override;
	virtual void OnMouseMove(wxRect area, wxPoint point) override;
	virtual void OnMouseReleased(wxPoint point) override;

	virtual void OnKeyPressed(wxKeyEvent c) override;

	virtual wxVector<wxRect> MarkedAreas() override;

private:
	AsciiArtData &m_data;

	wxPoint m_start;
	wxRect m_area;
	wxRect m_current_area;

	struct Info
	{
		wxPoint p;
		wxChar c;
	};
	wxVector<Info> m_moved_data;

	enum class State
	{
		NotMarked,
		Marked,
		Moving
	};
	State m_state;
};
