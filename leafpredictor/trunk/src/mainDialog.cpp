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
#include "mainDialog.h"

#include "aboutDialog.h"
#include "eigenSystem.h"

#include "wx/menu.h"
#include "wx/filedlg.h"
#include "wx/dcbuffer.h"
#include "wx/sizer.h"
#include "wx/choice.h"
#include "wx/panel.h"

// Identifiers for the controls
enum _CONTROL_ID
{

	CHC_PC1 = wxID_HIGHEST,
    CHC_PC2,
	CHC_PC3,
	SLI_PC1,
	SLI_PC2,
	SLI_PC3
};


// Indexes of the miscellaneous parts of the status bar
enum _STATUSBAR_PART
{
	STATUS_UNUSED,
	STATUS_PCMESSAGE
};


// Custom events
DEFINE_EVENT_TYPE(EVT_NEW_MESSAGE_LOGGED)


// Events processed by this class
BEGIN_EVENT_TABLE(MainDialog, wxFrame)
	// --- Menus
	EVT_MENU       (wxID_EXIT,                               MainDialog::OnMenuQuit)
	EVT_MENU       (wxID_ABOUT,                              MainDialog::OnMenuAbout)
	EVT_MENU       (wxID_OPEN,                               MainDialog::OnMenuOpen)

	// --- Frame
	EVT_CLOSE      (MainDialog::OnClose)

	EVT_COMMAND    (wxID_ANY, EVT_NEW_MESSAGE_LOGGED,        MainDialog::OnNewMessageLogged)

	EVT_CHOICE     (CHC_PC1,                                 MainDialog::OnChoices)
	EVT_CHOICE     (CHC_PC2,                                 MainDialog::OnChoices)
	EVT_CHOICE     (CHC_PC3,                                 MainDialog::OnChoices)

	EVT_COMMAND_SCROLL     (SLI_PC1,        MainDialog::OnScroll)
	EVT_COMMAND_SCROLL     (SLI_PC2,        MainDialog::OnScroll)
	EVT_COMMAND_SCROLL     (SLI_PC3,        MainDialog::OnScroll)
END_EVENT_TABLE()


// The single instance of MainDialog accross the application
MainDialog* MainDialog::mInstance = NULL;

wxMutex MainDialog::mMutexUpdateCheck;


MainDialog::MainDialog(void) : wxFrame(NULL, wxID_ANY, wxT(LEAFPREDICTOR_APPNAME))
{

	// Setting the icon for the main dialog will allows child frames and dialog to inherit from it
/*
#ifdef __WXGTK__
	SetIcon(wxIcon(PathManager::GetImgPath() + wxT(FMC_FILE_IMG_DIALOG)));
#elif _FAHMON_WIN32_
	SetIcon(wxICON(dialog_icon));
#endif
*/

	CreateMenuBar();
	CreateStatusBar(2);
	CreateLayout();
}


MainDialog::~MainDialog(void)
{
}


void MainDialog::CreateInstance(void)
{
	wxASSERT(mInstance == NULL);

	mInstance = new MainDialog();
}


void MainDialog::DestroyInstance(void)
{
	wxASSERT(mInstance != NULL);

	delete mInstance;
	mInstance = NULL;
}


bool MainDialog::HasBeenInstanciated(void)
{
	return mInstance != NULL;
}


MainDialog* MainDialog::GetInstance(void)
{
	wxASSERT(mInstance != NULL);

	return mInstance;
}


bool MainDialog::Show(bool show)
{
	bool result = wxFrame::Show(show);
	Maximize();

	return result;
}

/************************************  FRAME CREATION  ************************************/


inline void MainDialog::CreateMenuBar(void)
{
	wxMenu    *menu;
	wxMenuBar *menuBar;

	// This method must be called once!
	wxASSERT(GetMenuBar() == NULL);

	// Create the menubar
	menuBar = new wxMenuBar();

	// The 'Main' menu
	menu = new wxMenu();
	menu->Append(wxID_OPEN, _("&Open...\tCTRL+O"), _("Open an eigensystem for prediction"));
	menu->AppendSeparator();
	menu->Append(wxID_EXIT, _("&Quit\tCtrl+Q"), wxString::Format(_("Quit %s"), wxT(LEAFPREDICTOR_APPNAME)));
	menuBar->Append(menu, _("&File"));

	// The 'Help' menu
	menu = new wxMenu();
#ifndef __WXMAC__
	menu->Append(wxID_HELP, _("&Help Contents\tF1"), _("See help contents"));
#else
	menu->Append(wxID_HELP, _("&Help Contents\tCtrl+?"), _("See help contents"));
#endif
	menu->Append(wxID_ABOUT, _("&About"), wxString::Format(_("About %s"), wxT(LEAFPREDICTOR_APPNAME)));
#ifdef __WXMAC__
	{
	    wxApp::s_macAboutMenuItemId = wxID_ABOUT;
	}
#endif
	menuBar->Append(menu, _("&Help"));

	SetMenuBar(menuBar);
}


inline void MainDialog::CreateLayout(void)
{
	wxBoxSizer *topLevelSizer;
	wxBoxSizer *mainSizer;
	wxBoxSizer *pcSizer;
	wxGridSizer *plotSizer;
	wxInt32     i;

	for(i=0;i<96;++i)
	{
		mPC1[i] = wxString::Format(_T("PC %i"), i+1);
		mPC2[i] = wxString::Format(_T("PC %i"), i+1);
		mPC3[i] = wxString::Format(_T("PC %i"), i+1);
	}

	mChoice1 = new wxChoice(this, CHC_PC1, wxDefaultPosition, wxDefaultSize, 96, mPC1);
	mChoice2 = new wxChoice(this, CHC_PC2, wxDefaultPosition, wxDefaultSize, 96, mPC2);
	mChoice3 = new wxChoice(this, CHC_PC3, wxDefaultPosition, wxDefaultSize, 96, mPC3);

	mPC1Amount = new wxSlider(this, SLI_PC1, 0, -50, 50, wxDefaultPosition, wxDefaultSize);
	mPC2Amount = new wxSlider(this, SLI_PC2, 0, -50, 50, wxDefaultPosition, wxDefaultSize);
	mPC3Amount = new wxSlider(this, SLI_PC3, 0, -50, 50, wxDefaultPosition, wxDefaultSize);

	mPC1Value = new wxStaticText(this, wxID_ANY, wxT(" 0.0"), wxDefaultPosition, wxDefaultSize);
	mPC2Value = new wxStaticText(this, wxID_ANY, wxT(" 0.0"), wxDefaultPosition, wxDefaultSize);
	mPC3Value = new wxStaticText(this, wxID_ANY, wxT(" 0.0"), wxDefaultPosition, wxDefaultSize);

	pcSizer = new wxBoxSizer(wxHORIZONTAL);
	pcSizer->Add(mChoice1, 0, wxALIGN_LEFT);
	pcSizer->Add(mPC1Amount, 1, wxEXPAND);
	pcSizer->Add(mPC1Value, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	pcSizer->Add(mChoice2, 0, wxALIGN_LEFT);
	pcSizer->Add(mPC2Amount, 1,  wxEXPAND);
	pcSizer->Add(mPC2Value, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	pcSizer->Add(mChoice3, 0, wxALIGN_LEFT);
	pcSizer->Add(mPC3Amount, 1,  wxEXPAND);
	pcSizer->Add(mPC3Value, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

	plotSizer = new wxGridSizer(2,1);

	mMeanLeafCanvas = new LeafCanvas(this, -1, wxDefaultPosition, wxDefaultSize);
	mPredictedLeafCanvas = new LeafCanvas(this, -1, wxDefaultPosition, wxDefaultSize);

	plotSizer->Add(mMeanLeafCanvas, 0, wxEXPAND);
	plotSizer->Add(mPredictedLeafCanvas, 0, wxEXPAND);

	mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(pcSizer, 0, wxEXPAND);
	mainSizer->AddSpacer(5);
	mainSizer->Add(plotSizer, 1, wxEXPAND);

	topLevelSizer = new wxBoxSizer(wxVERTICAL);

	topLevelSizer->Add(mainSizer, 1, wxEXPAND);
	SetSizer(topLevelSizer);
	topLevelSizer->Fit(this);

	mChoice1->Enable(false);
	mChoice2->Enable(false);
	mChoice3->Enable(false);
	mPC1Amount->Enable(false);
	mPC2Amount->Enable(false);
	mPC3Amount->Enable(false);

}

/************************************  EVENTS  ************************************/


void MainDialog::OnMenuQuit(wxCommandEvent& event)
{
	Close(true);
}


void MainDialog::OnMenuAbout(wxCommandEvent& event)
{
	AboutDialog::GetInstance(this)->ShowModal();
}

void MainDialog::OnMenuOpen(wxCommandEvent& event)
{
	wxString selectedFile;

	wxFileDialog *OpenDialog = new wxFileDialog(this, _("Select an eigensystem to use for prediction"), wxT(""), wxT(""),wxT("Text Files (*.txt)|*.txt"), wxOPEN, wxDefaultPosition);
	if (OpenDialog->ShowModal() == wxID_OK) // if the user click "Open" instead of "cancel"
	{
		selectedFile = OpenDialog->GetPath();
		mEigenSystem = new EigenSystem();
		mEigenSystem->LoadEigenFile(selectedFile);
		mMeanLeafCanvas->SetLeaf(mEigenSystem->GetMeanLeaf());
		mPredictedLeafCanvas->SetLeaf(mEigenSystem->GetPredictedLeaf());

		mChoice1->Enable(true);
		mChoice2->Enable(true);
		mChoice3->Enable(true);
		mPC1Amount->Enable(true);
		mPC2Amount->Enable(true);
		mPC3Amount->Enable(true);
	}
}


void MainDialog::OnClose(wxCloseEvent& event)
{
	AboutDialog::DestroyInstance();
	Destroy();
}


void MainDialog::OnNewMessageLogged(wxCommandEvent& event)
{
	//MessagesFrame::OnNewMessage();
}


void MainDialog::OnChoices(wxCommandEvent& event)
{
	switch(event.GetId())
	{

		case CHC_PC1:
			mPC1Value->SetLabel(wxT(" 0.0"));
			mPC1Amount->SetValue(0);
			mEigenSystem->PredictLeaf(mChoice1->GetSelection(), mPC1Amount->GetValue(), mChoice2->GetSelection(), mPC2Amount->GetValue(), mChoice3->GetSelection(), mPC3Amount->GetValue());
			mPredictedLeafCanvas->SetLeaf(mEigenSystem->GetPredictedLeaf());
			break;
		case CHC_PC2:
			mPC2Value->SetLabel(wxT(" 0.0"));
			mPC2Amount->SetValue(0);
			mEigenSystem->PredictLeaf(mChoice1->GetSelection(), mPC1Amount->GetValue(), mChoice2->GetSelection(), mPC2Amount->GetValue(), mChoice3->GetSelection(), mPC3Amount->GetValue());
			mPredictedLeafCanvas->SetLeaf(mEigenSystem->GetPredictedLeaf());
			break;
		case CHC_PC3:
			mPC3Value->SetLabel(wxT(" 0.0"));
			mPC3Amount->SetValue(0);
			mEigenSystem->PredictLeaf(mChoice1->GetSelection(), mPC1Amount->GetValue(), mChoice2->GetSelection(), mPC2Amount->GetValue(), mChoice3->GetSelection(), mPC3Amount->GetValue());
			mPredictedLeafCanvas->SetLeaf(mEigenSystem->GetPredictedLeaf());
			break;

		default:
			wxASSERT(false);
			break;
	}
}


void MainDialog::OnScroll(wxScrollEvent& event)
{
	float tmpFloat;

	switch(event.GetId())
	{

		case SLI_PC1:
			tmpFloat = mPC1Amount->GetValue();
			mPC1Value->SetLabel(wxString::Format(wxT("%.1f"),tmpFloat/10));
			mEigenSystem->PredictLeaf(mChoice1->GetSelection(), mPC1Amount->GetValue(), mChoice2->GetSelection(), mPC2Amount->GetValue(), mChoice3->GetSelection(), mPC3Amount->GetValue());
			mPredictedLeafCanvas->SetLeaf(mEigenSystem->GetPredictedLeaf());
			break;
		case SLI_PC2:
			tmpFloat = mPC2Amount->GetValue();
			mPC2Value->SetLabel(wxString::Format(wxT("%.1f"),tmpFloat/10));
			mEigenSystem->PredictLeaf(mChoice1->GetSelection(), mPC1Amount->GetValue(), mChoice2->GetSelection(), mPC2Amount->GetValue(), mChoice3->GetSelection(), mPC3Amount->GetValue());
			mPredictedLeafCanvas->SetLeaf(mEigenSystem->GetPredictedLeaf());
			break;
		case SLI_PC3:
			tmpFloat = mPC3Amount->GetValue();
			mPC3Value->SetLabel(wxString::Format(wxT("%.1f"),tmpFloat/10));
			mEigenSystem->PredictLeaf(mChoice1->GetSelection(), mPC1Amount->GetValue(), mChoice2->GetSelection(), mPC2Amount->GetValue(), mChoice3->GetSelection(), mPC3Amount->GetValue());
			mPredictedLeafCanvas->SetLeaf(mEigenSystem->GetPredictedLeaf());
			break;

		default:
			wxASSERT(false);
			break;
	}
}
