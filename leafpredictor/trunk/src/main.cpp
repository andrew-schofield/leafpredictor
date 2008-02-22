/*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Library General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include "leafpredictor.h"
#include "main.h"
#include "mainDialog.h"

#include "locale.h"
#include "wx/image.h"

IMPLEMENT_APP(LeafPredictorApp)

BEGIN_EVENT_TABLE(LeafPredictorApp, wxApp)
	EVT_QUERY_END_SESSION (LeafPredictorApp::OnQueryEndSession)
	EVT_END_SESSION (LeafPredictorApp::OnEndSession)
	EVT_CLOSE (LeafPredictorApp::OnClose)
END_EVENT_TABLE()


bool LeafPredictorApp::OnInit(void)
{

	if ( !m_locale.Init(wxLANGUAGE_DEFAULT, wxLOCALE_CONV_ENCODING) )
	{
		printf("This language is not supported by the system.");
		return false;
	}

	wxString locale = m_locale.GetLocale();

#ifdef _FAHMON_WIN32_
	{
		wxLocale::AddCatalogLookupPathPrefix(wxT("./lang"));
	}
#elif __WXGTK__
	{
		wxLocale::AddCatalogLookupPathPrefix(wxT(DATADIR));
	}
#elif __WXMAC__
	{
		wxFileName appPath = wxFileName(wxTheApp->argv[0]).GetPath (wxPATH_GET_VOLUME);
		appPath.RemoveLastDir();

		wxString resourcesPath = appPath.GetPath();

		resourcesPath += _T("/Contents/SharedSupport/lang/");

		wxLocale::AddCatalogLookupPathPrefix(resourcesPath);
	}
#endif
	// Initialize the catalogs we'll be using
	m_locale.AddCatalog(wxT("leafpredictor"));

	// this catalog is installed in standard location on Linux systems and
	// shows that you may make use of the standard message catalogs as well
	//
	// if it's not installed on your system, it is just silently ignored
#ifdef __WXGTK__
	{
		wxLogNull noLog;
		m_locale.AddCatalog(_T("fileutils"));
	}
#endif

	// We must explicitly tell to wxGTK which locale we want to use
	// Otherwise, floating point number conversion doesn't work on non-english Linux systems that do not use point as the decimal character
#ifdef __WXGTK__
	setlocale(LC_NUMERIC, "C");
#endif

	// Miscellaneous initializations
	wxImage::AddHandler(new wxPNGHandler);      // We use only PNG images

	//Create mandatory singletons
	//MessagesManager::CreateInstance();          // MUST be created first, so that other manager can log messages immediately
	//PreferencesManager::CreateInstance();       // MUST be created second, so that other managers can retrieve some preferences when created
	//ProjectsManager::CreateInstance();
	//ClientsManager::CreateInstance();
	//BenchmarksManager::CreateInstance();
	//WebMonitor::CreateInstance();
	MainDialog::CreateInstance();               // MUST be created last, when all other managers have been created
	MainDialog::GetInstance()->Show(true);

	return true;
}


int LeafPredictorApp::OnExit(void)
{
	// This frame is automatically destroyed by wxWidget, so we don't have to do it
	// MainDialog::DestroyInstance();
	// TrayManager::DestroyInstance();
	// BenchmarksManager::DestroyInstance();
	// ClientsManager::DestroyInstance();
	// ProjectsManager::DestroyInstance();
	// WebMonitor::DestroyInstance();
	// PreferencesManager::DestroyInstance();        // MUST be destroyed last, so that other managers can save their preferences when they are destroyed
	// MessagesManager::DestroyInstance();

	return 0;
}


void LeafPredictorApp::OnEndSession(wxCloseEvent& event)
{
	//_LogMsgInfo(wxString::Format(wxT("Running OnEndSession")));
	//MainDialog::GetInstance()->Close();
}


void LeafPredictorApp::OnQueryEndSession(wxCloseEvent& event)
{
	//_LogMsgInfo(wxString::Format(wxT("Running OnQueryEndSession")));
	//MainDialog::GetInstance()->Close();
}

void LeafPredictorApp::OnClose(wxCloseEvent& event)
{
	//_LogMsgInfo(wxString::Format(wxT("Running OnClose")));
	//MainDialog::GetInstance()->Close();
}
