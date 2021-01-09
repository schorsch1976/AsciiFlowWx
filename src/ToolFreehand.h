#pragma once

#include "AsciiArtData.h"
#include "ITool.h"

///////////////////////////////////////////////////////////////////////////////
// Tool Freehand
///////////////////////////////////////////////////////////////////////////////
class ToolFreehand : public ITool
{
public:
	explicit ToolFreehand(AsciiArtData &data);
	virtual void OnMousePressed(wxPoint point) override;
	virtual void OnMouseMove(wxRect area, wxPoint point) override;
	virtual void OnMouseReleased(wxPoint point) override;

	virtual void OnKeyPressed(wxKeyEvent c) override;

	virtual wxVector<wxRect> MarkedAreas() override;

private:
	void Draw();
	AsciiArtData &m_data;
	wxVector<wxPoint> m_spots;
};
