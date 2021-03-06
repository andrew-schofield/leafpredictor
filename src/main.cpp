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

#ifdef __GNUG__
#pragma implementation "main.h"
#endif

#include "wx/wxprec.h"

#ifdef  __BORLANDC__
#pragma hdrstop
#endif

#include "leafpredictor.h"
#include "main.h"

#include "mainDialog.h"
#include "projectDialog.h"

#include "locale.h"
#include "wx/image.h"
#include "wx/filename.h"

IMPLEMENT_APP(LeafPredictorApp)

BEGIN_EVENT_TABLE(LeafPredictorApp, wxApp)
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

#ifdef __WXMSW__
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

	ProjectDialog::CreateInstance();
	MainDialog::CreateInstance();               // MUST be created last, when all other managers have been created
	MainDialog::GetInstance()->Show(true);

	return true;
}


int LeafPredictorApp::OnExit(void)
{
	ProjectDialog::DestroyInstance();

	return 0;
}

void LeafPredictorApp::OnClose(wxCloseEvent& event)
{
}
