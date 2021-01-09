#include "App.h"
#include "MainFrame.h"

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
	wxImage::AddHandler(new wxPNGHandler);

	MainFrame *frame =
		new MainFrame("AsciiFlowWx", wxPoint(50, 50), wxSize(800, 600));
	frame->Show(true);
	return true;
}
