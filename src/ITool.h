#pragma once

#include <wx/gdicmn.h>
#include <wx/wx.h>

enum class Tool
{
	Move,
	Rectangle,
	Resize,
	Class,
	Arrow,
	Line,
	Text,
	Freehand,
	Erase
};

// Strategy
class ITool
{
public:
	virtual ~ITool();

	virtual void OnMousePressed(wxPoint point) = 0;
	virtual void OnMouseMove(wxRect area, wxPoint point) = 0;
	virtual void OnMouseReleased(wxPoint point) = 0;

	virtual void OnKeyPressed(wxKeyEvent c) = 0;

	virtual wxVector<wxRect> MarkedAreas() = 0;
};
