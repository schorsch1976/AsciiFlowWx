#include "PreferencesDialog.h"

#include <wx/spinctrl.h>
#include <wx/choice.h>

PreferencesDialog::PreferencesDialog()
	: wxDialog(NULL, -1, "Preferences", wxDefaultPosition, wxSize(250, 230)),

	m_config("AsciiFlowWx")
{
	Read();

	wxGridSizer *mainsizer = new wxGridSizer(2, 2, 5, 5);

	mp_choice = new wxChoice(this, -1);
	mp_choice->Append("TextUnderIcon");
	mp_choice->Append("TextBesidesIcon");
	mp_choice->Append("IconOnly");
	mp_choice->Append("TextOnly");
	mp_choice->SetSelection(static_cast<int>(m_style));

	mp_choice->Bind(wxEVT_CHOICE, [this](wxCommandEvent& evt)
	{
		OnChangeStyle(evt);
	});

	mp_spinctl = new wxSpinCtrl(this, -1);
	mp_spinctl->SetRange(32, 128);
	mp_spinctl->SetValue(m_icon_size);

	mp_spinctl->Bind(wxEVT_SPINCTRL, [this](wxSpinEvent& evt)
	{
		OnChangeIconSize(evt);
	});

	mainsizer->Add(new wxStaticText(this, -1, "Button Style"));
	mainsizer->Add(mp_choice);
	mainsizer->Add(new wxStaticText(this, -1, "Iconsize"));
	mainsizer->Add(mp_spinctl);

	SetSizerAndFit(mainsizer);
}

ButtonStyle PreferencesDialog::GetButtonStyle()
{
	return m_style;
}

int PreferencesDialog::GetIconSize() const
{
	return m_icon_size;
}

void PreferencesDialog::OnChangeStyle(wxCommandEvent& evt)
{
}
void PreferencesDialog::OnChangeIconSize(wxSpinEvent& evt)
{
}


void PreferencesDialog::Read()
{
	long style = m_config.Read("/ButtonStyle", 0l);
	if (style < 0)
	{
		style = 0;
	}
	if (style >= static_cast<long>(ButtonStyle::TextOnly))
	{
		style = static_cast<long>(ButtonStyle::TextOnly);
	}
	m_style = static_cast<ButtonStyle>(style);

	long iconsize = m_config.Read("/IconSize", 48l);
	if (iconsize < 32)
	{
		iconsize = 32;
	}
	if (iconsize >= 128)
	{
		iconsize = 128;
	}
	m_icon_size = static_cast<int>(iconsize);

}
void PreferencesDialog::Write()
{
	m_config.Write("/ButtonStyle", static_cast<long>(m_style));
	m_config.Write("/IconSize", static_cast<long>(m_icon_size));
}
