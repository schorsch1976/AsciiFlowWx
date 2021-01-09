#pragma once

#include "AsciiArtData.h"
#include "ITool.h"

///////////////////////////////////////////////////////////////////////////////
// Tool Class
///////////////////////////////////////////////////////////////////////////////
class ToolClass : public ITool
{
public:
	explicit ToolClass(AsciiArtData &data);
	virtual void OnMousePressed(wxPoint point) override;
	virtual void OnMouseMove(wxRect area, wxPoint point) override;
	virtual void OnMouseReleased(wxPoint point) override;

	virtual void OnKeyPressed(wxKeyEvent c) override;

	virtual wxVector<wxRect> MarkedAreas() override;

private:
	AsciiArtData &m_data;
};
