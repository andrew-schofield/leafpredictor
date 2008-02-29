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
#pragma implementation "mainDialog.h"
#endif

#include "wx/wxprec.h"

#ifdef  __BORLANDC__
#pragma hdrstop
#endif

#include "leafpredictor.h"
#include "mainDialog.h"

#include "aboutDialog.h"
#include "eigenSystem.h"
#include "tools.h"

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
	CHC_PC4,
	SLI_PC1,
	SLI_PC2,
	SLI_PC3,
	SLI_PC4,
	MID_IMPORTES,
	MID_IMPORTLEAF,
	MID_RELATIVE,
	MID_RESETPCS,
	MID_MEAN_SCREEN,
	MID_PRED_SCREEN
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
	EVT_MENU       (MID_IMPORTES,                            MainDialog::OnMenuImportES)
	EVT_MENU       (MID_IMPORTLEAF,                          MainDialog::OnMenuImportLeaf)
	EVT_MENU       (wxID_SAVE,                               MainDialog::OnMenuSave)
	EVT_MENU       (MID_RELATIVE,                            MainDialog::OnMenuRelative)
	EVT_MENU       (MID_RESETPCS,                            MainDialog::OnMenuResetPCs)
	EVT_MENU       (MID_MEAN_SCREEN,                         MainDialog::OnMenuMeanScreen)
	EVT_MENU       (MID_PRED_SCREEN,                         MainDialog::OnMenuPredScreen)

	// --- Frame
	EVT_CLOSE      (MainDialog::OnClose)

	EVT_COMMAND    (wxID_ANY, EVT_NEW_MESSAGE_LOGGED,        MainDialog::OnNewMessageLogged)

	EVT_CHOICE     (CHC_PC1,                                 MainDialog::OnChoices)
	EVT_CHOICE     (CHC_PC2,                                 MainDialog::OnChoices)
	EVT_CHOICE     (CHC_PC3,                                 MainDialog::OnChoices)
	EVT_CHOICE     (CHC_PC4,                                 MainDialog::OnChoices)

	EVT_COMMAND_SCROLL     (SLI_PC1,        MainDialog::OnScroll)
	EVT_COMMAND_SCROLL     (SLI_PC2,        MainDialog::OnScroll)
	EVT_COMMAND_SCROLL     (SLI_PC3,        MainDialog::OnScroll)
	EVT_COMMAND_SCROLL     (SLI_PC4,        MainDialog::OnScroll)
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
	mLinkedScale = true;
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
	menu->Append(MID_IMPORTES, _("&Import EigenSystem\tCTRL+I"), _("Import a LeafAnalyser eigensystem for prediction"));
	mImportLeafMenu = new wxMenuItem(menu, MID_IMPORTLEAF, _("Import &Leaf"), _("Import a Leaf file from LeafAnalyser"));
	menu->Append(mImportLeafMenu);
	mImportLeafMenu->Enable(false);
	menu->AppendSeparator();
	menu->Append(wxID_OPEN, _("&Open\tCTRL+O"), _("Open a saved LeafPredictor project"));
	menu->Append(wxID_SAVE, _("&Save\tCTRL+S"), _("Save a LeafPredictor project"));
	menu->AppendSeparator();
	menu->Append(wxID_EXIT, _("&Quit\tCtrl+Q"), wxString::Format(_("Quit %s"), wxT(LEAFPREDICTOR_APPNAME)));
	menuBar->Append(menu, _("&File"));

	menu = new wxMenu();
	mRelativeMenu = new wxMenuItem(menu,MID_RELATIVE, _("&Relative scaling\tCTRL+R"), _("Scale leaves relative to each other"),true);
	menu->Append(mRelativeMenu);
	menu->Check(MID_RELATIVE, true);
	mRelativeMenu->Enable(false);
	menu->AppendSeparator();
	mResetMenu = new wxMenuItem(menu,MID_RESETPCS, _("&Reset\tCTRL+W"), _("Reset all PC values back to 0"));
	menu->Append(mResetMenu);
	mResetMenu->Enable(false);
	menu->AppendSeparator();
	menu->Append(MID_MEAN_SCREEN, _("Save &Mean Leaf\tCTRL+M"), _("Save the mean leaf to an image file"));
	menu->Append(MID_PRED_SCREEN, _("Save &Predicted Leaf\tCTRL+P"), _("Save the predicted leaf to an image file"));
	menuBar->Append(menu, _("&Plots"));

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
		mPC4[i] = wxString::Format(_T("PC %i"), i+1);
	}

	mChoice1 = new wxChoice(this, CHC_PC1, wxDefaultPosition, wxDefaultSize, 96, mPC1);
	mChoice2 = new wxChoice(this, CHC_PC2, wxDefaultPosition, wxDefaultSize, 96, mPC2);
	mChoice3 = new wxChoice(this, CHC_PC3, wxDefaultPosition, wxDefaultSize, 96, mPC3);
	mChoice4 = new wxChoice(this, CHC_PC4, wxDefaultPosition, wxDefaultSize, 96, mPC4);

	mPC1Amount = new wxSlider(this, SLI_PC1, 0, -50, 50, wxDefaultPosition, wxDefaultSize);
	mPC2Amount = new wxSlider(this, SLI_PC2, 0, -50, 50, wxDefaultPosition, wxDefaultSize);
	mPC3Amount = new wxSlider(this, SLI_PC3, 0, -50, 50, wxDefaultPosition, wxDefaultSize);
	mPC4Amount = new wxSlider(this, SLI_PC4, 0, -50, 50, wxDefaultPosition, wxDefaultSize);

	mPC1Value = new wxStaticText(this, wxID_ANY, wxT(" 0.0"), wxDefaultPosition, wxDefaultSize);
	mPC2Value = new wxStaticText(this, wxID_ANY, wxT(" 0.0"), wxDefaultPosition, wxDefaultSize);
	mPC3Value = new wxStaticText(this, wxID_ANY, wxT(" 0.0"), wxDefaultPosition, wxDefaultSize);
	mPC4Value = new wxStaticText(this, wxID_ANY, wxT(" 0.0"), wxDefaultPosition, wxDefaultSize);

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
	pcSizer->Add(mChoice4, 0, wxALIGN_LEFT);
	pcSizer->Add(mPC4Amount, 1,  wxEXPAND);
	pcSizer->Add(mPC4Value, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

	plotSizer = new wxGridSizer(2,1);

	mMeanLeafCanvas = new LeafCanvas(this, -1, wxDefaultPosition, wxDefaultSize);
	mPredictedLeafCanvas = new LeafCanvas(this, -1, wxDefaultPosition, wxDefaultSize);
	mMeanLeafCanvas->SetLabel(_("Mean Leaf"));
	mPredictedLeafCanvas->SetLabel(_("Predicted Leaf"));

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
	mChoice4->Enable(false);
	mPC1Amount->Enable(false);
	mPC2Amount->Enable(false);
	mPC3Amount->Enable(false);
	mPC4Amount->Enable(false);

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

void MainDialog::OnMenuImportES(wxCommandEvent& event)
{
	wxString selectedFile;

	wxFileDialog *OpenDialog = new wxFileDialog(this, _("Select a LeafAnalyser eigensystem to import"), wxT(""), wxT(""),wxT("Text Files (*.txt)|*.txt"), wxOPEN, wxDefaultPosition);
	if (OpenDialog->ShowModal() == wxID_OK) // if the user click "Open" instead of "cancel"
	{
		selectedFile = OpenDialog->GetPath();
		mEigenSystem = new EigenSystem();
		mEigenSystem->LoadEigenFile(selectedFile);
		mMeanLeafCanvas->SetLeaf(mEigenSystem->GetMeanLeaf());
		mMeanLeafCanvas->ExtDraw();
		mPredictedLeafCanvas->SetLeaf(mEigenSystem->GetPredictedLeaf());
		mPredictedLeafCanvas->ExtDraw();

		mChoice1->Enable(true);
		mChoice2->Enable(true);
		mChoice3->Enable(true);
		mChoice4->Enable(true);
		mPC1Amount->Enable(true);
		mPC2Amount->Enable(true);
		mPC3Amount->Enable(true);
		mPC4Amount->Enable(true);
		mRelativeMenu->Enable(true);
		mResetMenu->Enable(true);
		mImportLeafMenu->Enable(true);
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
			UpdateLeaves();
			break;
		case CHC_PC2:
			mPC2Value->SetLabel(wxT(" 0.0"));
			mPC2Amount->SetValue(0);
			UpdateLeaves();
			break;
		case CHC_PC3:
			mPC3Value->SetLabel(wxT(" 0.0"));
			mPC3Amount->SetValue(0);
			UpdateLeaves();
			break;
		case CHC_PC4:
			mPC4Value->SetLabel(wxT(" 0.0"));
			mPC4Amount->SetValue(0);
			UpdateLeaves();
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
			UpdateLeaves();
			break;
		case SLI_PC2:
			tmpFloat = mPC2Amount->GetValue();
			mPC2Value->SetLabel(wxString::Format(wxT("%.1f"),tmpFloat/10));
			UpdateLeaves();
			break;
		case SLI_PC3:
			tmpFloat = mPC3Amount->GetValue();
			mPC3Value->SetLabel(wxString::Format(wxT("%.1f"),tmpFloat/10));
			UpdateLeaves();
			break;
		case SLI_PC4:
			tmpFloat = mPC4Amount->GetValue();
			mPC4Value->SetLabel(wxString::Format(wxT("%.1f"),tmpFloat/10));
			UpdateLeaves();
			break;

		default:
			wxASSERT(false);
			break;
	}
}


void MainDialog::SetPCMessage(wxString msg)
{
	SetStatusText(msg, STATUS_PCMESSAGE);
}


void MainDialog::OnMenuOpen(wxCommandEvent& event)
{
}


void MainDialog::OnMenuSave(wxCommandEvent& event)
{
}


void MainDialog::UpdateLeaves(void)
{
	mEigenSystem->PredictLeaf(mChoice1->GetSelection(), mPC1Amount->GetValue(), mChoice2->GetSelection(), mPC2Amount->GetValue(), mChoice3->GetSelection(), mPC3Amount->GetValue(), mChoice4->GetSelection(), mPC4Amount->GetValue());
	mMeanLeafCanvas->SetLeaf(mEigenSystem->GetMeanLeaf());
	mPredictedLeafCanvas->SetLeaf(mEigenSystem->GetPredictedLeaf());
	if(mLinkedScale)
	{
		double meanScale, predScale;

		predScale = mPredictedLeafCanvas->GetScale();
		meanScale = mMeanLeafCanvas->GetScale();
		mMeanLeafCanvas->SetScale(std::min(predScale,meanScale));
		mPredictedLeafCanvas->SetScale(std::min(predScale,meanScale));
	}
	mMeanLeafCanvas->ExtDraw();
	mPredictedLeafCanvas->ExtDraw();
}


void MainDialog::OnMenuRelative(wxCommandEvent& event)
{
	mLinkedScale = !mLinkedScale;
	UpdateLeaves();
}


void MainDialog::OnMenuResetPCs(wxCommandEvent& event)
{
	mPC1Amount->SetValue(0);
	mPC1Value->SetLabel(wxT(" 0.0"));
	mPC2Amount->SetValue(0);
	mPC2Value->SetLabel(wxT(" 0.0"));
	mPC3Amount->SetValue(0);
	mPC3Value->SetLabel(wxT(" 0.0"));
	mPC4Amount->SetValue(0);
	mPC4Value->SetLabel(wxT(" 0.0"));
	UpdateLeaves();
}


void MainDialog::OnMenuMeanScreen(wxCommandEvent& event)
{
	wxString selectedFile;
	wxBitmap screenShot(mMeanLeafCanvas->GetScreenShot());

	wxFileDialog *SaveDialog = new wxFileDialog(this, _("Choose location to save mean leaf image"), wxT(""), wxT(""),wxT("PNG Images (*.png)|*.png"), wxSAVE, wxDefaultPosition);
	if (SaveDialog->ShowModal() == wxID_OK) // if the user click "Open" instead of "cancel"
	{
		selectedFile = SaveDialog->GetPath();
		screenShot.SaveFile(selectedFile, wxBITMAP_TYPE_PNG);
	}
}


void MainDialog::OnMenuPredScreen(wxCommandEvent& event)
{
	wxString selectedFile;
	wxBitmap screenShot(mPredictedLeafCanvas->GetScreenShot());

	wxFileDialog *SaveDialog = new wxFileDialog(this, _("Choose location to save predicted leaf image"), wxT(""), wxT(""),wxT("PNG Images (*.png)|*.png"), wxSAVE, wxDefaultPosition);
	if (SaveDialog->ShowModal() == wxID_OK) // if the user click "Open" instead of "cancel"
	{
		selectedFile = SaveDialog->GetPath();
		screenShot.SaveFile(selectedFile, wxBITMAP_TYPE_PNG);
	}
}


void MainDialog::OnMenuImportLeaf(wxCommandEvent& event)
{
	wxString selectedFile;

	wxFileDialog *OpenDialog = new wxFileDialog(this, _("Select a Leaf file to import"), wxT(""), wxT(""),wxT("Text Files (*.txt)|*.txt"), wxOPEN, wxDefaultPosition);
	if (OpenDialog->ShowModal() == wxID_OK) // if the user click "Open" instead of "cancel"
	{
		selectedFile = OpenDialog->GetPath();
		//mEigenSystem = new EigenSystem();
		mEigenSystem->LoadLeafFile(selectedFile);
		mMeanLeafCanvas->SetLeaf(mEigenSystem->GetMeanLeaf());
		//mMeanLeafCanvas->ExtDraw();
		UpdateLeaves();
	}
}
