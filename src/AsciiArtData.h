#pragma once

#include <wx/event.h>
#include <wx/wx.h>

#include <functional>

#include "ITool.h"

using DataAreaListenerCB = std::function<void(wxPoint)>;

class AsciiArtData
{
public:
	// rule of 5
	AsciiArtData();
	virtual ~AsciiArtData();

	AsciiArtData(const AsciiArtData &rhs);
	AsciiArtData &operator=(const AsciiArtData &rhs);

	AsciiArtData(AsciiArtData &&rhs);
	AsciiArtData &operator=(AsciiArtData &&rhs);

	// register the listener
	void Register(DataAreaListenerCB listener);

	// access the data
	wxString ExportData() const;
	void ImportData(wxString data);

	void Set(wxPoint p, wxChar c);
	void Set(int x, int y, wxChar c);
	wxChar At(wxPoint p);
	wxChar At(int x, int y);

	void Clear();

	int Width() const;
	int Height() const;

private:
	void ResizeData(wxPoint point);
	void CheckData();

	void SendDataAreaChanged(wxPoint area);

	wxVector<DataAreaListenerCB> m_listeners;

	int m_width;
	int m_height;
	wxVector<wxString> m_data;
};
