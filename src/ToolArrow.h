#pragma once

#include "AsciiArtData.h"
#include "ITool.h"

///////////////////////////////////////////////////////////////////////////////
// Tool Arrow
///////////////////////////////////////////////////////////////////////////////
class ToolArrow : public ITool
{
public:
	explicit ToolArrow(AsciiArtData &data);
	virtual void OnMousePressed(wxPoint point) override;
	virtual void OnMouseMove(wxRect area, wxPoint point) override;
	virtual void OnMouseReleased(wxPoint point) override;

	virtual void OnKeyPressed(wxKeyEvent c) override;

	virtual wxVector<wxRect> MarkedAreas() override;

private:
	AsciiArtData &m_data;
	wxPoint m_start;
};
