#include "AsciiArtData.h"

#include <wx/regex.h>

static const int MIN_WIDTH = 80;
static const int MIN_HEIGHT = 50;

///////////////////////////////////////////////////////////////////////////////
// The AsciiArtData
///////////////////////////////////////////////////////////////////////////////
AsciiArtData::AsciiArtData()
{
	m_width = m_height = 0;
	ResizeData(wxPoint(-1, -1));
}
AsciiArtData::~AsciiArtData() {}

AsciiArtData::AsciiArtData(const AsciiArtData &rhs) { *this = rhs; }
AsciiArtData &AsciiArtData::operator=(const AsciiArtData &rhs)
{
	m_width = rhs.m_width;
	m_height = rhs.m_height;
	m_data = rhs.m_data;

	ResizeData(wxPoint(m_width - 1, m_height - 1));
	return *this;
}

AsciiArtData::AsciiArtData(AsciiArtData &&rhs) { *this = rhs; }
AsciiArtData &AsciiArtData::operator=(AsciiArtData &&rhs)
{
	m_width = rhs.m_width;
	m_height = rhs.m_height;
	m_data = rhs.m_data;

	ResizeData(wxPoint(m_width - 1, m_height - 1));
	return *this;
}

// register the listener
void AsciiArtData::Register(DataAreaListenerCB listener)
{
	m_listeners.push_back(listener);
}
// access the data
wxString AsciiArtData::ExportData() const
{
	wxRegEx ex(" +$");
	wxVector<wxString> cleaned;
	for (int y = 0; y < static_cast<int>(m_data.size()); ++y)
	{
		// remove and space from the end of the line
		wxString line = m_data[y];
		ex.Replace(&line, "");
		cleaned.push_back(line);
	}

	int last_used_line = -1;
	for (int y = 0; y < static_cast<int>(cleaned.size()); ++y)
	{
		if (cleaned[y].size())
		{
			last_used_line = y + 1;
		}
	}
	wxString ret;
	for (int y = 0; y < last_used_line; ++y)
	{
		ret += cleaned[y] + '\n';
	}
	return ret;
}
void AsciiArtData::ImportData(wxString data)
{
	m_data.clear();
	m_width = 0;
	m_height = 0;

	// split by \n
	wxString line;
	int x = 0;
	for (wxString::size_type i = 0; i < data.size(); ++i)
	{
		wxChar c = data[i];
		if (c == '\r')
		{
			continue;
		}
		else if (c == '\n')
		{
			m_data.push_back(line);
			line.clear();
			x = 0;
		}
		else if (wxIsprint(c))
		{
			line += c;
			++x;
		}
		else
		{
			// convert to " "
			line += WXK_SPACE;
			++x;
		}
		m_width = std::max(x, m_width);
	}
	m_data.push_back(line);

	m_width = m_height = -1;

	ResizeData(wxPoint(-1, -1));
	CheckData();
}

void AsciiArtData::Clear()
{
	m_data.clear();
	m_width = 0;
	m_height = 0;
	ResizeData(wxPoint(10 - 1, 10 - 1));
}
void AsciiArtData::Set(wxPoint p, wxChar c)
{
	ResizeData(p);
	if (p.x < 0 || p.y < 0)
	{
		return;
	}
	m_data[p.y][p.x] = c;
}
void AsciiArtData::Set(int x, int y, wxChar c) { Set(wxPoint(x, y), c); }

wxChar AsciiArtData::At(wxPoint p)
{
	if (p.x < 0 || p.y < 0)
	{
		return wxChar(0);
	}
	if (p.x >= m_width || p.y >= m_height)
	{
		ResizeData(p);
	}
	return m_data[p.y][p.x];
}
wxChar AsciiArtData::At(int x, int y) { return At(wxPoint(x, y)); }

int AsciiArtData::Width() const { return m_width; }
int AsciiArtData::Height() const { return m_height; }

// private
void AsciiArtData::ResizeData(wxPoint point)
{
	if (m_width > 0 && m_height > 0 && point.x >= 0 && point.y > 0 &&
		point.x < m_width && point.y < m_height)
	{
		return;
	}

	// determine minimum needed size
	int old_width = m_width;
	int old_height = m_height;

	int min_needed_width = std::max(MIN_WIDTH, point.x + 1);
	min_needed_width = std::max(min_needed_width, static_cast<int>(m_width));

	int min_needed_height = std::max(MIN_HEIGHT, point.y + 1);
	min_needed_height = std::max(min_needed_height, old_height);
	min_needed_height =
		std::max(min_needed_height, static_cast<int>(m_data.size()));

	int current_width = 0;
	for (decltype(m_data)::size_type y = 0; y < m_data.size(); ++y)
	{
		current_width =
			std::max(current_width, static_cast<int>(m_data[y].size()));
	}
	min_needed_width = std::max(min_needed_width, current_width);

	// now resize everything to the needed size
	m_data.resize(min_needed_height);
	for (int y = 0; y < static_cast<int>(m_data.size()); ++y)
	{
		wxString &line = m_data[y];
		line.resize(min_needed_width, WXK_SPACE);
	}

	m_width = min_needed_width;
	m_height = min_needed_height;

	CheckData();

	if (m_width != old_width || m_height != old_height)
	{
		SendDataAreaChanged(wxPoint(m_width, m_height));
	}
}
void AsciiArtData::CheckData()
{
	for (wxString::size_type y = 0; y < m_data.size(); ++y)
	{
		wxString &line = m_data[y];
		for (wxString::size_type x = 0; x < line.size(); ++x)
		{
			if (!wxIsprint(line[x]))
			{
				line[x] = WXK_SPACE;
			}
		}
	}
}

void AsciiArtData::SendDataAreaChanged(wxPoint area)
{
	for (auto &c : m_listeners)
	{
		c(area);
	}
}
