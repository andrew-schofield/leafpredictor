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
#pragma implementation "leafDialog.h"
#endif

#include "wx/wxprec.h"

#ifdef  __BORLANDC__
#pragma hdrstop
#endif

#include "leafpredictor.h"
#include "leafDialog.h"

#include "leafCanvas.h"
#include "eigenSystem.h"

#include "wx/sizer.h"
#include "wx/version.h"
#include "wx/statline.h"
#include "wx/hyperlink.h"
#include "wx/stattext.h"
#include "wx/font.h"
#include "wx/checkbox.h"
#include "wx/textctrl.h"
#include "wx/choice.h"
#include "wx/button.h"
#include "wx/valtext.h"
#include "wx/valgen.h"
#include "wx/menu.h"
#include "wx/filedlg.h"

BEGIN_EVENT_TABLE(LeafDialog, wxFrame)
		EVT_CLOSE(LeafDialog::OnClose)
		EVT_MENU(wxID_EXIT,    LeafDialog::OnQuit)
		EVT_MENU(wxID_OPEN,    LeafDialog::OnOpenMenu)
		EVT_MENU(wxID_NEW,     LeafDialog::OnNewMenu)
END_EVENT_TABLE()

LeafDialog::LeafDialog()
{
	Init();
}


LeafDialog::LeafDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
	Init();

	Create(parent, id, caption, pos, size, style);
}


void LeafDialog::Init()
{
}


bool LeafDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
	SetExtraStyle(wxWS_EX_BLOCK_EVENTS);

	if (!wxFrame::Create( parent, id, caption, pos, size, style))
		return false;

	CreateMenuBar();
	CreateStatusBar(1);
	CreateControls();
	SetDialogValidators();

	GetSizer()->Fit(this);

	GetSizer()->SetSizeHints(this);

	return true;
}



void LeafDialog::CreateControls()
{
	wxBoxSizer* topLevelSizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(topLevelSizer);

	mLeafCanvas = new LeafCanvas(this, LEAF, wxDefaultPosition, wxSize(640,480));
	topLevelSizer->Add(mLeafCanvas, 0, wxEXPAND);

}


void LeafDialog::SetDialogValidators()
{
}


void LeafDialog::OnClose(wxCloseEvent& event)
{
	Destroy();
}


inline void LeafDialog::CreateMenuBar(void)
{
	wxMenu    *menu;
	wxMenuBar *menuBar;

	// This method must be called once!
	wxASSERT(GetMenuBar() == NULL);

	// Create the menubar
	menuBar = new wxMenuBar();

	// The 'Main' menu
	menu = new wxMenu();
	menu->Append(wxID_NEW, _("&New\tCTRL+N"), _("Create a new Leaf Viewer window"));
	menu->Append(wxID_OPEN, _("&Open\tCTRL+O"), _("Open a LeafAnalyser format leaf"));
	//menu->Append(wxID_SAVE, _("&Save\tCTRL+S"), _("Save a LeafPredictor project"));
	menu->AppendSeparator();
	menu->Append(wxID_EXIT, _("&Quit\tCtrl+Q"), _("Quit Leaf Viewer"));
	menuBar->Append(menu, _("&File"));

	// The 'Help' menu
	menu = new wxMenu();
#ifndef __WXMAC__
	menu->Append(wxID_HELP, _("&Help Contents\tF1"), _("See help contents"));
#else
	menu->Append(wxID_HELP, _("&Help Contents\tCtrl+?"), _("See help contents"));
#endif
	menuBar->Append(menu, _("&Help"));

	SetMenuBar(menuBar);
}


void LeafDialog::OnQuit(wxCommandEvent& event)
{
	Close();
}


void LeafDialog::OnOpenMenu(wxCommandEvent& event)
{
	wxString selectedFile;
	EigenSystem* tmpES = new EigenSystem();

	wxFileDialog *OpenDialog = new wxFileDialog(this, _("Select a Leaf file to open"), wxT(""), wxT(""),wxT("Text Files (*.txt)|*.txt"), wxOPEN, wxDefaultPosition);
	if (OpenDialog->ShowModal() == wxID_OK) // if the user click "Open" instead of "cancel"
	{
		selectedFile = OpenDialog->GetPath();
		tmpES->LoadLeafFile(selectedFile);
		mLeafCanvas->SetLeaf(tmpES->GetMeanLeaf());
		tmpES->GetMeanLeaf();
		SetTitle(wxString::Format(wxT("%s - %s"),_("Leaf Viewer"),selectedFile.c_str()));
	}
}


void LeafDialog::OnNewMenu(wxCommandEvent& event)
{
	LeafDialog* dialog = new LeafDialog(NULL, NEW_LEAF_VIEWER);
	dialog->Show(true);
}
