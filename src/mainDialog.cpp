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
#include "pcSpaceDialog.h"
#include "leafDialog.h"
#include "projectDialog.h"
#include "main.h"
#include "tinyxml.h"
#include "tools.h"

#include "wx/filedlg.h"
#include "wx/dcbuffer.h"
#include "wx/sizer.h"
#include "wx/choice.h"
#include "wx/panel.h"
#include "wx/msgdlg.h"

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
	SLI_MSA,
	MID_IMPORTES,
	MID_IMPORTLEAF,
	MID_RELATIVE,
	MID_RESETPCS,
	MID_MEAN_SCREEN,
	MID_PRED_SCREEN,
	MID_INVERTLEAF,
	MID_4UPLEAF,
	MID_SHOWPCINFO,
	LEAF_MEAN,
	LEAF_PRED1,
	LEAF_PRED2,
	LEAF_PRED3,
	MID_EXPORTLEAF,
	MID_LEAFVIEWER,
	MID_PROJECTMANAGER,
	MID_GRIDGEN,
	PC_SPACE_GEN,
	LEAF_VIEWER,
	MID_MEAN_OVERLAY,
	MID_MANUAL,
	MID_INDEPENDENT,
	MID_SHOWLANDMARKS
};


// Indexes of the miscellaneous parts of the status bar
enum _STATUSBAR_PART
{
	STATUS_UNUSED,
	STATUS_LEAF,
	STATUS_PCMESSAGE
};


// Events processed by this class
BEGIN_EVENT_TABLE(MainDialog, wxFrame)
	// --- Menus
	EVT_MENU               (wxID_EXIT,                           MainDialog::OnMenuQuit)
	EVT_MENU               (wxID_ABOUT,                          MainDialog::OnMenuAbout)
	EVT_MENU               (wxID_OPEN,                           MainDialog::OnMenuOpen)
	EVT_MENU               (wxID_NEW,                            MainDialog::OnMenuNew)
	EVT_MENU               (MID_IMPORTES,                        MainDialog::OnMenuImportES)
	EVT_MENU               (MID_IMPORTLEAF,                      MainDialog::OnMenuImportLeaf)
	EVT_MENU               (wxID_SAVE,                           MainDialog::OnMenuSave)
	EVT_MENU               (wxID_SAVEAS,                         MainDialog::OnMenuSaveAs)
	EVT_MENU               (MID_RELATIVE,                        MainDialog::OnMenuRelative)
	EVT_MENU               (MID_INDEPENDENT,                     MainDialog::OnMenuIndependent)
	EVT_MENU               (MID_MANUAL,                          MainDialog::OnMenuManual)
	EVT_MENU               (MID_RESETPCS,                        MainDialog::OnMenuResetPCs)
	EVT_MENU               (MID_MEAN_SCREEN,                     MainDialog::OnMenuMeanScreen)
	EVT_MENU               (MID_PRED_SCREEN,                     MainDialog::OnMenuPredScreen)
	EVT_MENU               (MID_INVERTLEAF,                      MainDialog::OnMenuInvertLeaf)
	EVT_MENU               (MID_4UPLEAF,                         MainDialog::OnMenu4UpLeaf)
	EVT_MENU               (MID_SHOWPCINFO,                      MainDialog::OnMenuShowPCInfo)
	EVT_MENU               (MID_EXPORTLEAF,                      MainDialog::OnMenuExportLeaf)
	EVT_MENU               (MID_LEAFVIEWER,                      MainDialog::OnMenuShowLeafViewer)
	EVT_MENU               (MID_PROJECTMANAGER,                  MainDialog::OnMenuShowProjectManager)
	EVT_MENU               (MID_GRIDGEN,                         MainDialog::OnMenuGenerateGrid)
	EVT_MENU               (MID_MEAN_OVERLAY,                    MainDialog::OnMenuMeanOverlay)
	EVT_MENU               (MID_SHOWLANDMARKS,                   MainDialog::OnMenuShowLandmarks)

	// --- Frame
	EVT_CLOSE              (MainDialog::OnClose)

	EVT_CHOICE             (CHC_PC1,                             MainDialog::OnChoices)
	EVT_CHOICE             (CHC_PC2,                             MainDialog::OnChoices)
	EVT_CHOICE             (CHC_PC3,                             MainDialog::OnChoices)
	EVT_CHOICE             (CHC_PC4,                             MainDialog::OnChoices)

	EVT_COMMAND_SCROLL     (SLI_PC1,                             MainDialog::OnScroll)
	EVT_COMMAND_SCROLL     (SLI_PC2,                             MainDialog::OnScroll)
	EVT_COMMAND_SCROLL     (SLI_PC3,                             MainDialog::OnScroll)
	EVT_COMMAND_SCROLL     (SLI_PC4,                             MainDialog::OnScroll)
	EVT_COMMAND_SCROLL     (SLI_MSA,                             MainDialog::OnMSAScroll)

	EVT_LEFT_DOWN          (MainDialog::OnClick)
END_EVENT_TABLE()


// The single instance of MainDialog accross the application
MainDialog* MainDialog::mInstance = NULL;

wxMutex MainDialog::mMutexUpdateCheck;


MainDialog::MainDialog(void) : wxFrame(NULL, wxID_ANY, wxT(LEAFPREDICTOR_APPNAME))
{
	CreateMenuBar();
	CreateStatusBar(3);
	CreateLayout();
	mLinkedScale = true;
	mManualScale = 0;
	mInvertLeaf = false;
	m4UpMode = false;
	mShowPCInfo = false;
	mSelectedCanvas = wxT("Predicted Leaf 1");
	SetStatusText(wxString::Format(_("Selected Leaf: %i"),1), STATUS_LEAF);
	mShowLandmarks = true;
	SetIcon(wxICON(LeafPredictor));
	mCurrentFilename = wxT("");
	SetUnsavedState(false);
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
	//SetSize(800,600);
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
	menu->Append(wxID_NEW, _("&New\tCTRL+N"), _("Create a new LeafPredictor project"));
	menu->Append(wxID_OPEN, _("&Open\tCTRL+O"), _("Open a saved LeafPredictor project"));
	menu->AppendSeparator();
	mSaveProjectMenu = new wxMenuItem(menu, wxID_SAVE, _("&Save\tCTRL+S"), _("Save a LeafPredictor project"));
	menu->Append(mSaveProjectMenu);
	mSaveProjectMenu->Enable(false);
	mSaveAsProjectMenu = new wxMenuItem(menu, wxID_SAVEAS, _("Save &As\tSHIFT+CTRL+S"), _("Save a LeafPredictor project as a new file"));
	menu->Append(mSaveAsProjectMenu);
	mSaveAsProjectMenu->Enable(false);
	menu->AppendSeparator();
	menu->Append(MID_IMPORTES, _("&Import EigenSystem\tCTRL+I"), _("Import a LeafAnalyser eigensystem for prediction"));
	mImportLeafMenu = new wxMenuItem(menu, MID_IMPORTLEAF, _("Import &Leaf"), _("Import a Leaf file from LeafAnalyser"));
	menu->Append(mImportLeafMenu);
	mImportLeafMenu->Enable(false);
	mExportLeafMenu = new wxMenuItem(menu, MID_EXPORTLEAF, _("&Export Leaf"), _("Export the selected predicted leaf into LeafAnalyser format"));
	menu->Append(mExportLeafMenu);
	mExportLeafMenu->Enable(false);
	menu->AppendSeparator();
	menu->Append(wxID_EXIT, _("&Quit\tCtrl+Q"), wxString::Format(_("Quit %s"), wxT(LEAFPREDICTOR_APPNAME)));
	menuBar->Append(menu, _("&File"));

	menu = new wxMenu();
	mIndependentMenu = new wxMenuItem(menu,MID_INDEPENDENT, _("&Independent Scaling"), _("Scale leaves independent of each other"),wxITEM_RADIO);
	menu->Append(mIndependentMenu);
	menu->Check(MID_INDEPENDENT, false);
	mIndependentMenu->Enable(false);
	mRelativeMenu = new wxMenuItem(menu,MID_RELATIVE, _("&Relative Scaling"), _("Scale leaves relative to each other"),wxITEM_RADIO);
	menu->Append(mRelativeMenu);
	menu->Check(MID_RELATIVE, false);
	mRelativeMenu->Enable(false);
	mManualMenu = new wxMenuItem(menu,MID_MANUAL, _("&Manual Scaling"), _("Scale leaves by a given value"),wxITEM_RADIO);
	menu->Append(mManualMenu);
	menu->Check(MID_MANUAL, true);
	mManualMenu->Enable(false);
	menu->AppendSeparator();
	mInvertLeafMenu = new wxMenuItem(menu,MID_INVERTLEAF, _("&Invert Leaf\tCTRL+N"), _("Fix leaves that import upside-down"),wxITEM_CHECK);
	menu->Append(mInvertLeafMenu);
	menu->Check(MID_INVERTLEAF, false);
	mInvertLeafMenu->Enable(false);
	m4UpLeafMenu = new wxMenuItem(menu, MID_4UPLEAF, _("4&Up View\tCTRL+4"), _("Enable 4-Up view"),wxITEM_CHECK);
	menu->Append(m4UpLeafMenu);
	menu->Check(MID_4UPLEAF, false);
	m4UpLeafMenu->Enable(false);
	mShowPCInfoMenu = new wxMenuItem(menu, MID_SHOWPCINFO, _("S&how PC Scores"), _("Display PC Scores on the plot area"),wxITEM_CHECK);
	menu->Append(mShowPCInfoMenu);
	menu->Check(MID_SHOWPCINFO, false);
	mShowPCInfoMenu->Enable(false);
	menu->AppendSeparator();
	mMeanOverlayMenu = new wxMenuItem(menu, MID_MEAN_OVERLAY, _("&Overlay Mean Leaf"), _("Overlay mean leaf onto predicted leaves"),wxITEM_CHECK);
	menu->Append(mMeanOverlayMenu);
	mMeanOverlayMenu->Enable(false);
	mMeanOverlayMenu->Check(false);
	mShowLandmarksMenu = new wxMenuItem(menu, MID_SHOWLANDMARKS, _("&Show Landmarks"), _("Overlay landmarks onto leaves"),wxITEM_CHECK);
	menu->Append(mShowLandmarksMenu);
	mShowLandmarksMenu->Enable(false);
	mShowLandmarksMenu->Check(true);
	menu->AppendSeparator();
	mResetMenu = new wxMenuItem(menu,MID_RESETPCS, _("&Reset\tCTRL+W"), _("Reset all PC values back to 0"));
	menu->Append(mResetMenu);
	mResetMenu->Enable(false);
	menu->AppendSeparator();
	menu->Append(MID_MEAN_SCREEN, _("Save &Mean Leaf\tCTRL+M"), _("Save the mean leaf to an image file"));
	menu->Append(MID_PRED_SCREEN, _("Save &Predicted Leaf\tCTRL+P"), _("Save the predicted leaf to an image file"));
	menuBar->Append(menu, _("&Plots"));

	menu = new wxMenu();
	menu->Append(MID_LEAFVIEWER, _("&Open Leaf Viewer\tCTRL+L"), _("View LeafAnalyser formatted leaves"));
	//menu->Append(MID_PROJECTMANAGER, _("Open &Project Manager\tCTRL+P"), _("View files in the current LeafPredictor project"));
	mGridGenMenu = new wxMenuItem(menu, MID_GRIDGEN, _("&Generate PC Space\tCTRL+G"), _("Generate a set of Leaves that fill PC space in 1-4 dimensions"));
	menu->Append(mGridGenMenu);
	mGridGenMenu->Enable(false);
	menuBar->Append(menu, _("&Tools"));

	// The 'Help' menu
	menu = new wxMenu();
	#ifndef __WXMAC__
	//menu->Append(wxID_HELP, _("&Help Contents\tF1"), _("See help contents"));
	#else
	//menu->Append(wxID_HELP, _("&Help Contents\tCtrl+?"), _("See help contents"));
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
	wxBoxSizer *mainSizer;
	wxBoxSizer *canvasSizer;
	wxBoxSizer *pcSizer;
	wxGridSizer *plotSizer;
	wxInt32     i;
	wxPanel    *mTopLevelPanel;


	// We need to use a panel as a top level component in our frame
	// Without that, the frame looks ugly under Windows (dark grey background)
	mTopLevelPanel = new wxPanel(this, wxID_ANY);

	for(i=0;i<96;++i)
	{
		mPC1[i] = wxString::Format(_T("PC %i"), i+1);
		mPC2[i] = wxString::Format(_T("PC %i"), i+1);
		mPC3[i] = wxString::Format(_T("PC %i"), i+1);
		mPC4[i] = wxString::Format(_T("PC %i"), i+1);
	}

	mChoice1 = new wxChoice(mTopLevelPanel, CHC_PC1, wxDefaultPosition, wxDefaultSize, 96, mPC1);
	mChoice2 = new wxChoice(mTopLevelPanel, CHC_PC2, wxDefaultPosition, wxDefaultSize, 96, mPC2);
	mChoice3 = new wxChoice(mTopLevelPanel, CHC_PC3, wxDefaultPosition, wxDefaultSize, 96, mPC3);
	mChoice4 = new wxChoice(mTopLevelPanel, CHC_PC4, wxDefaultPosition, wxDefaultSize, 96, mPC4);

	mPC1Amount = new wxSlider(mTopLevelPanel, SLI_PC1, 0, -50, 50, wxDefaultPosition, wxDefaultSize);
	mPC2Amount = new wxSlider(mTopLevelPanel, SLI_PC2, 0, -50, 50, wxDefaultPosition, wxDefaultSize);
	mPC3Amount = new wxSlider(mTopLevelPanel, SLI_PC3, 0, -50, 50, wxDefaultPosition, wxDefaultSize);
	mPC4Amount = new wxSlider(mTopLevelPanel, SLI_PC4, 0, -50, 50, wxDefaultPosition, wxDefaultSize);

	mManualScaleAmount = new wxSlider(mTopLevelPanel, SLI_MSA, 1, 1, 200, wxDefaultPosition, wxDefaultSize, wxSL_VERTICAL | wxSL_INVERSE| wxSL_LABELS);

	mPC1Value = new wxStaticText(mTopLevelPanel, wxID_ANY, wxT(" 0.0"), wxDefaultPosition, wxDefaultSize);
	mPC2Value = new wxStaticText(mTopLevelPanel, wxID_ANY, wxT(" 0.0"), wxDefaultPosition, wxDefaultSize);
	mPC3Value = new wxStaticText(mTopLevelPanel, wxID_ANY, wxT(" 0.0"), wxDefaultPosition, wxDefaultSize);
	mPC4Value = new wxStaticText(mTopLevelPanel, wxID_ANY, wxT(" 0.0"), wxDefaultPosition, wxDefaultSize);

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
	mLowerPlotSizer = new wxGridSizer(2,1);

	mMeanLeafCanvas = new LeafCanvas(mTopLevelPanel, LEAF_MEAN, wxDefaultPosition, wxDefaultSize);
	mPredictedLeafCanvas1 = new LeafCanvas(mTopLevelPanel, LEAF_PRED1, wxDefaultPosition, wxDefaultSize);
	mPredictedLeafCanvas2 = new LeafCanvas(mTopLevelPanel, LEAF_PRED2, wxDefaultPosition, wxDefaultSize);
	mPredictedLeafCanvas3 = new LeafCanvas(mTopLevelPanel, LEAF_PRED3, wxDefaultPosition, wxDefaultSize);
	mMeanLeafCanvas->SetLabel(_("Mean Leaf"));
	mPredictedLeafCanvas1->SetLabel(_("Predicted Leaf 1"));
	mPredictedLeafCanvas2->SetLabel(_("Predicted Leaf 2"));
	mPredictedLeafCanvas3->SetLabel(_("Predicted Leaf 3"));

	plotSizer->Add(mMeanLeafCanvas, 0, wxEXPAND);
	plotSizer->Add(mPredictedLeafCanvas1, 0, wxEXPAND);
	mLowerPlotSizer->Add(mPredictedLeafCanvas2, 0, wxEXPAND);
	mLowerPlotSizer->Add(mPredictedLeafCanvas3, 0, wxEXPAND);

	canvasSizer = new wxBoxSizer(wxVERTICAL);
	canvasSizer->AddSpacer(5);
	canvasSizer->Add(plotSizer, 1, wxEXPAND);
	canvasSizer->Add(mLowerPlotSizer, 1, wxEXPAND);

	mainSizer = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(mManualScaleAmount, 0, wxEXPAND);
	mainSizer->Add(canvasSizer, 1, wxEXPAND);

	mTopLevelSizer = new wxBoxSizer(wxVERTICAL);

	mTopLevelSizer->Add(pcSizer, 0, wxEXPAND);
	mTopLevelSizer->Add(mainSizer, 1, wxEXPAND);
	mTopLevelPanel->SetSizer(mTopLevelSizer);
	mTopLevelSizer->Fit(this);

	mChoice1->Enable(false);
	mChoice2->Enable(false);
	mChoice3->Enable(false);
	mChoice4->Enable(false);
	mPC1Amount->Enable(false);
	mPC2Amount->Enable(false);
	mPC3Amount->Enable(false);
	mPC4Amount->Enable(false);
	mChoice1->SetSelection(0);
	mChoice2->SetSelection(0);
	mChoice3->SetSelection(0);
	mChoice4->SetSelection(0);

	mPredictedLeafCanvas2->Show(false);
	mPredictedLeafCanvas3->Show(false);
	mManualScaleAmount->Show(true);
	mManualScaleAmount->Enable(false);

	mMeanLeafCanvas->Select(true);
	mPredictedLeafCanvas1->Select(true);
}

/************************************  EVENTS  ************************************/


void MainDialog::OnMenuQuit(wxCommandEvent& event)
{
	Close(true);
}


void MainDialog::OnMenuNew(wxCommandEvent& event)
{
	mLinkedScale = true;
	mManualScale = 0;
	mInvertLeaf = false;
	m4UpMode = false;
	mShowPCInfo = false;
	mSelectedCanvas = wxT("Predicted Leaf 1");
	SetStatusText(wxString::Format(_("Selected Leaf: %i"),1), STATUS_LEAF);
	mShowLandmarks = true;
	SetIcon(wxICON(LeafPredictor));
	mCurrentFilename = wxT("");
	SetUnsavedState(false);
	//this doesn't work for some reason???
	mEigenSystem.Empty();
	mMeanLeafCanvas->Clear();
	mPredictedLeafCanvas1->Clear();
	mPredictedLeafCanvas2->Clear();
	mPredictedLeafCanvas3->Clear();
	
	mChoice1->Enable(false);
	mChoice2->Enable(false);
	mChoice3->Enable(false);
	mChoice4->Enable(false);
	mPC1Amount->Enable(false);
	mPC2Amount->Enable(false);
	mPC3Amount->Enable(false);
	mPC4Amount->Enable(false);
	mRelativeMenu->Enable(false);
	mIndependentMenu->Enable(false);
	mManualMenu->Enable(false);
	mResetMenu->Enable(false);
	mImportLeafMenu->Enable(false);
	mInvertLeafMenu->Enable(false);
	mInvertLeafMenu->Check(false);
	m4UpLeafMenu->Enable(false);
	mShowPCInfoMenu->Enable(false);
	mExportLeafMenu->Enable(false);
	mGridGenMenu->Enable(false);
	mMeanOverlayMenu->Enable(false);
	mMeanOverlayMenu->Check(false);
	mShowLandmarksMenu->Enable(false);
	mManualScaleAmount->Enable(false);
	mManualScaleAmount->SetValue((wxInt32)(1 * 50));
	mSaveAsProjectMenu->Enable(false);
	mPredictedLeafCanvas2->Show(false);
	mPredictedLeafCanvas3->Show(false);
	mManualScaleAmount->Show(true);
	mManualScaleAmount->Enable(false);
	mMeanLeafCanvas->Select(true);
	mPredictedLeafCanvas1->Select(true);
	mMeanLeafCanvas->ExtDraw();
	mPredictedLeafCanvas1->ExtDraw();
	mPredictedLeafCanvas2->ExtDraw();
	mPredictedLeafCanvas3->ExtDraw();
	
	mPC1Amount->SetValue(0);
	mPC1Value->SetLabel(wxT("0.0"));
	mChoice1->SetSelection(0);
	mPC2Amount->SetValue(0);
	mPC2Value->SetLabel(wxT("0.0"));
	mChoice2->SetSelection(0);
	mPC3Amount->SetValue(0);
	mPC3Value->SetLabel(wxT("0.0"));
	mChoice3->SetSelection(0);
	mPC4Amount->SetValue(0);
	mPC4Value->SetLabel(wxT("0.0"));
	mChoice4->SetSelection(0);
	mTopLevelSizer->Layout();
}


void MainDialog::OnMenuAbout(wxCommandEvent& event)
{
	AboutDialog::GetInstance(this)->ShowModal();
}

void MainDialog::OnMenuImportES(wxCommandEvent& event)
{
	wxString selectedFile;

	wxFileDialog *OpenDialog = new wxFileDialog(this, _("Select a LeafAnalyser eigensystem to import"), wxT(""), wxT(""),wxT("Text Files (*.txt)|*.txt"), wxOPEN|wxFD_FILE_MUST_EXIST, wxDefaultPosition);
	if (OpenDialog->ShowModal() == wxID_OK) // if the user click "Open" instead of "cancel"
	{
		selectedFile = OpenDialog->GetPath();
		//mEigenSystem = new EigenSystem();
		if(mEigenSystem.LoadEigenFile(selectedFile))
		{
			mMeanLeafCanvas->SetLeaf(mEigenSystem.GetMeanLeaf());
			mMeanLeafCanvas->ExtDraw();
			mPredictedLeafCanvas1->SetLeaf(mEigenSystem.GetPredictedLeaf());
			mPredictedLeafCanvas1->ExtDraw();
			mPredictedLeafCanvas2->SetLeaf(mEigenSystem.GetPredictedLeaf());
			mPredictedLeafCanvas2->ExtDraw();
			mPredictedLeafCanvas3->SetLeaf(mEigenSystem.GetPredictedLeaf());
			mPredictedLeafCanvas3->ExtDraw();

			mChoice1->Enable(true);
			mChoice2->Enable(true);
			mChoice3->Enable(true);
			mChoice4->Enable(true);
			mPC1Amount->Enable(true);
			mPC2Amount->Enable(true);
			mPC3Amount->Enable(true);
			mPC4Amount->Enable(true);
			mRelativeMenu->Enable(true);
			mIndependentMenu->Enable(true);
			mManualMenu->Enable(true);
			mResetMenu->Enable(true);
			mImportLeafMenu->Enable(true);
			mInvertLeafMenu->Enable(true);
			mInvertLeafMenu->Check(false);
			m4UpLeafMenu->Enable(true);
			mShowPCInfoMenu->Enable(true);
			mExportLeafMenu->Enable(true);
			mGridGenMenu->Enable(true);
			mMeanOverlayMenu->Enable(true);
			mMeanOverlayMenu->Check(false);
			mShowLandmarksMenu->Enable(true);
			mManualScaleAmount->Enable(true);
			mManualScaleAmount->SetValue((wxInt32)(mMeanLeafCanvas->GetScale() * 50));
			mManualScale = mMeanLeafCanvas->GetScale();
			mSaveAsProjectMenu->Enable(true);
			//UpdateLeaves();


			if(mMeanLeafCanvas->NeedsInversion())
			{
				wxMessageDialog dialog(this, _("The position of the centroid in this eigensystem indicates that the leaf may be inverted. Fix this now?"), wxT(LEAFPREDICTOR_APPNAME), wxYES_NO | wxICON_QUESTION, wxDefaultPosition);
				if(dialog.ShowModal() == wxID_YES)
				{
					mInvertLeaf = !mInvertLeaf;
					mEigenSystem.InvertLeaf();
					UpdateLeaves(true);
					mInvertLeafMenu->Check(true);
				}
			}
			SetUnsavedState(true);
		}
		else
		{
			Tools::ErrorMsgBox(wxString::Format(_("Could not import EigenSystem from %s"), selectedFile.c_str()));
		}
	}
}


void MainDialog::OnClose(wxCloseEvent& event)
{
	if(mUnsavedState)
	{
		int response = wxMessageBox(wxT("Save changes before closing?"), wxT(LEAFPREDICTOR_APPNAME), wxYES_NO|wxCANCEL|wxICON_QUESTION );
		if(response == wxYES)
			if( !Save())
			{
				event.Veto();
				return;
			}
		if(response == wxCANCEL)
		{
			event.Veto();
			return;
		}
	}
	AboutDialog::DestroyInstance();
	ProjectDialog::DestroyInstance();
	Destroy();
}


void MainDialog::OnChoices(wxCommandEvent& event)
{
	switch(event.GetId())
	{

		case CHC_PC1:
			mPC1Value->SetLabel(wxT(" 0.0"));
			mPC1Amount->SetValue(0);
			if(mSelectedCanvas == wxT("Predicted Leaf 1"))
			{
				mPredictedLeafCanvas1->SetPC1Value(0);
				mPredictedLeafCanvas1->SetPC1(mChoice1->GetSelection());
			}
			else if(mSelectedCanvas == wxT("Predicted Leaf 2"))
			{
				mPredictedLeafCanvas2->SetPC1Value(0);
				mPredictedLeafCanvas2->SetPC1(mChoice1->GetSelection());
			}
			else
			{
				mPredictedLeafCanvas3->SetPC1Value(0);
				mPredictedLeafCanvas3->SetPC1(mChoice1->GetSelection());
			}
			UpdateLeaves();
			break;
		case CHC_PC2:
			mPC2Value->SetLabel(wxT(" 0.0"));
			mPC2Amount->SetValue(0);
			if(mSelectedCanvas == wxT("Predicted Leaf 1"))
			{
				mPredictedLeafCanvas1->SetPC2Value(0);
				mPredictedLeafCanvas1->SetPC2(mChoice2->GetSelection());
			}
			else if(mSelectedCanvas == wxT("Predicted Leaf 2"))
			{
				mPredictedLeafCanvas2->SetPC2Value(0);
				mPredictedLeafCanvas2->SetPC2(mChoice2->GetSelection());
			}
			else
			{
				mPredictedLeafCanvas3->SetPC2Value(0);
				mPredictedLeafCanvas3->SetPC2(mChoice2->GetSelection());
			}
			UpdateLeaves();
			break;
		case CHC_PC3:
			mPC3Value->SetLabel(wxT(" 0.0"));
			mPC3Amount->SetValue(0);
			if(mSelectedCanvas == wxT("Predicted Leaf 1"))
			{
				mPredictedLeafCanvas1->SetPC3Value(0);
				mPredictedLeafCanvas1->SetPC3(mChoice3->GetSelection());
			}
			else if(mSelectedCanvas == wxT("Predicted Leaf 2"))
			{
				mPredictedLeafCanvas2->SetPC3Value(0);
				mPredictedLeafCanvas2->SetPC3(mChoice3->GetSelection());
			}
			else
			{
				mPredictedLeafCanvas3->SetPC3Value(0);
				mPredictedLeafCanvas3->SetPC3(mChoice3->GetSelection());
			}
			UpdateLeaves();
			break;
		case CHC_PC4:
			mPC4Value->SetLabel(wxT(" 0.0"));
			mPC4Amount->SetValue(0);
			if(mSelectedCanvas == wxT("Predicted Leaf 1"))
			{
				mPredictedLeafCanvas1->SetPC4Value(0);
				mPredictedLeafCanvas1->SetPC4(mChoice4->GetSelection());
			}
			else if(mSelectedCanvas == wxT("Predicted Leaf 2"))
			{
				mPredictedLeafCanvas2->SetPC4Value(0);
				mPredictedLeafCanvas2->SetPC4(mChoice4->GetSelection());
			}
			else
			{
				mPredictedLeafCanvas3->SetPC4Value(0);
				mPredictedLeafCanvas3->SetPC4(mChoice4->GetSelection());
			}
			UpdateLeaves();
			break;

		default:
			wxASSERT(false);
			break;
	}
	SetUnsavedState(true);
}


void MainDialog::OnScroll(wxScrollEvent& event)
{
	float tmpFloat;

	switch(event.GetId())
	{

		case SLI_PC1:
			tmpFloat = mPC1Amount->GetValue();
			mPC1Value->SetLabel(wxString::Format(wxT("%.1f"),tmpFloat/10));
			if(mSelectedCanvas == wxT("Predicted Leaf 1"))
				mPredictedLeafCanvas1->SetPC1Value(mPC1Amount->GetValue());
			else if(mSelectedCanvas == wxT("Predicted Leaf 2"))
				mPredictedLeafCanvas2->SetPC1Value(mPC1Amount->GetValue());
			else
				mPredictedLeafCanvas3->SetPC1Value(mPC1Amount->GetValue());
			UpdateLeaves();
			break;
		case SLI_PC2:
			tmpFloat = mPC2Amount->GetValue();
			mPC2Value->SetLabel(wxString::Format(wxT("%.1f"),tmpFloat/10));
			if(mSelectedCanvas == wxT("Predicted Leaf 1"))
				mPredictedLeafCanvas1->SetPC2Value(mPC2Amount->GetValue());
			else if(mSelectedCanvas == wxT("Predicted Leaf 2"))
				mPredictedLeafCanvas2->SetPC2Value(mPC2Amount->GetValue());
			else
				mPredictedLeafCanvas3->SetPC2Value(mPC2Amount->GetValue());
			UpdateLeaves();
			break;
		case SLI_PC3:
			tmpFloat = mPC3Amount->GetValue();
			mPC3Value->SetLabel(wxString::Format(wxT("%.1f"),tmpFloat/10));
			if(mSelectedCanvas == wxT("Predicted Leaf 1"))
				mPredictedLeafCanvas1->SetPC3Value(mPC3Amount->GetValue());
			else if(mSelectedCanvas == wxT("Predicted Leaf 2"))
				mPredictedLeafCanvas2->SetPC3Value(mPC3Amount->GetValue());
			else
				mPredictedLeafCanvas3->SetPC3Value(mPC3Amount->GetValue());
			UpdateLeaves();
			break;
		case SLI_PC4:
			tmpFloat = mPC4Amount->GetValue();
			mPC4Value->SetLabel(wxString::Format(wxT("%.1f"),tmpFloat/10));
			if(mSelectedCanvas == wxT("Predicted Leaf 1"))
				mPredictedLeafCanvas1->SetPC4Value(mPC4Amount->GetValue());
			else if(mSelectedCanvas == wxT("Predicted Leaf 2"))
				mPredictedLeafCanvas2->SetPC4Value(mPC4Amount->GetValue());
			else
				mPredictedLeafCanvas3->SetPC4Value(mPC4Amount->GetValue());
			UpdateLeaves();
			break;

		default:
			wxASSERT(false);
			break;
	}
	SetUnsavedState(true);
}


void MainDialog::SetPCMessage(wxString msg)
{
	SetStatusText(msg, STATUS_PCMESSAGE);
}


void MainDialog::OnMenuOpen(wxCommandEvent& event)
{
	wxString selectedFile;
	double tmpDouble;
	std::vector< std::vector< double > > tmpVector;

	wxFileDialog *OpenDialog = new wxFileDialog(this, _("Open Project..."), wxT(""), wxT(""),wxT("LeafPredictor Project XML (*.lpx)|*.lpx"), wxOPEN|wxFD_FILE_MUST_EXIST, wxDefaultPosition);
	if (OpenDialog->ShowModal() == wxID_OK) // if the user click "Save" instead of "cancel"
	{
		selectedFile = OpenDialog->GetPath();
		mCurrentFilename = selectedFile;
		TiXmlDocument doc((const char*)selectedFile.mb_str());
		TiXmlElement* elem;
		bool loadOkay = doc.LoadFile();
		if (loadOkay)
		{
			TiXmlHandle docHandle( &doc );
			TiXmlElement* leafpredictor = docHandle.FirstChild( "LeafPredictor" ).ToElement();
			if ( leafpredictor )
			{
				mChoice1->Enable(true);
				mChoice2->Enable(true);
				mChoice3->Enable(true);
				mChoice4->Enable(true);
				mPC1Amount->Enable(true);
				mPC2Amount->Enable(true);
				mPC3Amount->Enable(true);
				mPC4Amount->Enable(true);
				mRelativeMenu->Enable(true);
				mIndependentMenu->Enable(true);
				mManualMenu->Enable(true);
				mResetMenu->Enable(true);
				mImportLeafMenu->Enable(true);
				mInvertLeafMenu->Enable(true);
				m4UpLeafMenu->Enable(true);
				mShowPCInfoMenu->Enable(true);
				mExportLeafMenu->Enable(true);
				mGridGenMenu->Enable(true);
				mMeanOverlayMenu->Enable(true);
				mShowLandmarksMenu->Enable(true);
				mManualScaleAmount->Enable(true);
				mSaveAsProjectMenu->Enable(true);
			
				elem = docHandle.FirstChild( "LeafPredictor" ).FirstChild( "Eigensystem").FirstChild( "MeanLeaf" ).Element();
				if ( elem )
				{
					mEigenSystem.SetLeaf(Tools::SplitLineByDelim(wxString::FromUTF8(elem->GetText())+wxT(",x"), wxT(",")));
				}
				elem = docHandle.FirstChild( "LeafPredictor" ).FirstChild( "Eigensystem").FirstChild( "Eigenvalues" ).Element();
				if ( elem )
				{
					mEigenSystem.SetEigenValues(Tools::SplitLineByDelim(wxString::FromUTF8(elem->GetText())+wxT(",x"), wxT(",")));
				}
				elem = docHandle.FirstChild( "LeafPredictor" ).FirstChild( "Eigensystem" ).FirstChild( "Eigenvectors" ).FirstChild( "Eigenvector" ).ToElement();
				while( elem )
				{
					tmpVector.push_back(Tools::SplitLineByDelim(wxString::FromUTF8(elem->GetText())+wxT(",x"), wxT(",")));
					elem=elem->NextSiblingElement();
				}
				mEigenSystem.SetEigenVectors(tmpVector);

				//load settings after, as some depend on there actually being an eigensystem loaded
				elem = docHandle.FirstChild( "LeafPredictor" ).FirstChild( "Settings" ).FirstChild( "scale" ).Element();
				if ( elem )
				{
					wxString::FromUTF8(elem->GetText()).ToDouble(&tmpDouble);
					mManualScale = (double)tmpDouble / 50;
					mManualScaleAmount->SetValue((int)tmpDouble);
				}
				elem = docHandle.FirstChild( "LeafPredictor" ).FirstChild( "Settings" ).FirstChild( "scaletype" ).Element();
				if ( elem )
				{
					if(wxString::FromUTF8(elem->GetText()).Cmp(wxT("relative")) == 0)
					{
						mManualScale = 0;
						mLinkedScale = true;
						mManualScaleAmount->Show(false);
						mTopLevelSizer->Layout();
						mRelativeMenu->Check(true);
					}
					else if(wxString::FromUTF8(elem->GetText()).Cmp(wxT("independent")) == 0)
					{
						mManualScale = 0;
						mLinkedScale = false;
						mManualScaleAmount->Show(false);
						mTopLevelSizer->Layout();
						mIndependentMenu->Check(true);
					}
					else
					{
						mManualMenu->Check(true);
						mManualScaleAmount->Show(true);
						mTopLevelSizer->Layout();
					}
				}
				elem = docHandle.FirstChild( "LeafPredictor" ).FirstChild( "Settings" ).FirstChild( "inverted" ).Element();
				if ( elem )
				{
					if(wxString::FromUTF8(elem->GetText()).Cmp(wxT("true")) == 0)
					{
						mInvertLeaf = true;
						mInvertLeafMenu->Check(true);
					}
					else
					{
						mInvertLeaf = false;
						mInvertLeafMenu->Check(false);
					}
				}
				elem = docHandle.FirstChild( "LeafPredictor" ).FirstChild( "Settings" ).FirstChild( "fourupmode" ).Element();
				if ( elem )
				{
					bool foup;
					foup = (wxString::FromUTF8(elem->GetText()).Cmp(wxT("true")) == 0) ? true : false;
					m4UpMode = foup;
					mPredictedLeafCanvas2->Show(foup);
					mPredictedLeafCanvas3->Show(foup);
					mTopLevelSizer->Layout();
					if(!m4UpMode)
					{
						mSelectedCanvas = wxT("Predicted Leaf 1");
						SetStatusText(wxString::Format(_("Selected Leaf: %i"),1), STATUS_LEAF);
					}
				}
				elem = docHandle.FirstChild( "LeafPredictor" ).FirstChild( "Settings" ).FirstChild( "showpcinfo" ).Element();
				if ( elem )
				{
					if(wxString::FromUTF8(elem->GetText()).Cmp(wxT("true")) == 0)
					{
						mShowPCInfo = true;
						mShowPCInfoMenu->Check(true);
					}
					else
					{
						mShowPCInfo = false;
						mShowPCInfoMenu->Check(false);
					}
				}
				elem = docHandle.FirstChild( "LeafPredictor" ).FirstChild( "Settings" ).FirstChild( "showmeanoverlay" ).Element();
				if ( elem )
				{
					if(wxString::FromUTF8(elem->GetText()).Cmp(wxT("true")) == 0)
						mMeanOverlayMenu->Check(true);
					else
						mMeanOverlayMenu->Check(false);
					if(mMeanOverlayMenu->IsChecked())
					{
						mPredictedLeafCanvas1->SetOverlay(mEigenSystem.GetMeanLeaf());
						mPredictedLeafCanvas2->SetOverlay(mEigenSystem.GetMeanLeaf());
						mPredictedLeafCanvas3->SetOverlay(mEigenSystem.GetMeanLeaf());
					}
					else
					{
						mPredictedLeafCanvas1->RemoveOverlay();
						mPredictedLeafCanvas2->RemoveOverlay();
						mPredictedLeafCanvas3->RemoveOverlay();
					}
				}
				elem = docHandle.FirstChild( "LeafPredictor" ).FirstChild( "Settings" ).FirstChild( "showlandmarks" ).Element();
				if ( elem )
				{
					if(wxString::FromUTF8(elem->GetText()).Cmp(wxT("true")) == 0)
					{
						mShowLandmarks = true;
						mShowLandmarksMenu->Check(true);
					}
					else
					{
						mShowLandmarks = false;
						mShowLandmarksMenu->Check(false);
					}
				}
				// load up prediction view settings
				wxInt32 val;
				elem = docHandle.FirstChild( "LeafPredictor" ).FirstChild( "PredictionView" ).FirstChild( "Prediction1" ).FirstChild( "PC" ).ToElement();
				elem->QueryIntAttribute("Component", &val);
				mPredictedLeafCanvas1->SetPC1(val);
				elem->QueryIntAttribute("Value", &val);
				mPredictedLeafCanvas1->SetPC1Value(val);
				elem=elem->NextSiblingElement();
				elem->QueryIntAttribute("Component", &val);
				mPredictedLeafCanvas1->SetPC2(val);
				elem->QueryIntAttribute("Value", &val);
				mPredictedLeafCanvas1->SetPC2Value(val);
				elem=elem->NextSiblingElement();
				elem->QueryIntAttribute("Component", &val);
				mPredictedLeafCanvas1->SetPC3(val);
				elem->QueryIntAttribute("Value", &val);
				mPredictedLeafCanvas1->SetPC3Value(val);
				elem=elem->NextSiblingElement();
				elem->QueryIntAttribute("Component", &val);
				mPredictedLeafCanvas1->SetPC4(val);
				elem->QueryIntAttribute("Value", &val);
				mPredictedLeafCanvas1->SetPC4Value(val);
				
				elem = docHandle.FirstChild( "LeafPredictor" ).FirstChild( "PredictionView" ).FirstChild( "Prediction2" ).FirstChild( "PC" ).ToElement();
				elem->QueryIntAttribute("Component", &val);
				mPredictedLeafCanvas2->SetPC1(val);
				elem->QueryIntAttribute("Value", &val);
				mPredictedLeafCanvas2->SetPC1Value(val);
				elem=elem->NextSiblingElement();
				elem->QueryIntAttribute("Component", &val);
				mPredictedLeafCanvas2->SetPC2(val);
				elem->QueryIntAttribute("Value", &val);
				mPredictedLeafCanvas2->SetPC2Value(val);
				elem=elem->NextSiblingElement();
				elem->QueryIntAttribute("Component", &val);
				mPredictedLeafCanvas2->SetPC3(val);
				elem->QueryIntAttribute("Value", &val);
				mPredictedLeafCanvas2->SetPC3Value(val);
				elem=elem->NextSiblingElement();
				elem->QueryIntAttribute("Component", &val);
				mPredictedLeafCanvas2->SetPC4(val);
				elem->QueryIntAttribute("Value", &val);
				mPredictedLeafCanvas2->SetPC4Value(val);
				
				elem = docHandle.FirstChild( "LeafPredictor" ).FirstChild( "PredictionView" ).FirstChild( "Prediction3" ).FirstChild( "PC" ).ToElement();
				elem->QueryIntAttribute("Component", &val);
				mPredictedLeafCanvas3->SetPC1(val);
				elem->QueryIntAttribute("Value", &val);
				mPredictedLeafCanvas3->SetPC1Value(val);
				elem=elem->NextSiblingElement();
				elem->QueryIntAttribute("Component", &val);
				mPredictedLeafCanvas3->SetPC2(val);
				elem->QueryIntAttribute("Value", &val);
				mPredictedLeafCanvas3->SetPC2Value(val);
				elem=elem->NextSiblingElement();
				elem->QueryIntAttribute("Component", &val);
				mPredictedLeafCanvas3->SetPC3(val);
				elem->QueryIntAttribute("Value", &val);
				mPredictedLeafCanvas3->SetPC3Value(val);
				elem=elem->NextSiblingElement();
				elem->QueryIntAttribute("Component", &val);
				mPredictedLeafCanvas3->SetPC4(val);
				elem->QueryIntAttribute("Value", &val);
				mPredictedLeafCanvas3->SetPC4Value(val);

				//UpdateLeaves();
				ExternalCanvasSelect(wxT("Predicted Leaf 3"));
				ExternalCanvasSelect(wxT("Predicted Leaf 2"));
				ExternalCanvasSelect(wxT("Predicted Leaf 1"));
				SetUnsavedState(false);
			} else {
				//invalid project file
			}
		}
		else
		{
			Tools::ErrorMsgBox(wxString::Format(_("Could not load project from %s"), selectedFile.c_str()));
		}

	}
}


bool MainDialog::Save(bool forcenew)
{
	if(mCurrentFilename.Cmp(wxT(""))==0 || forcenew)
	{
		wxFileDialog *SaveDialog = new wxFileDialog(this, _("Save as..."), wxT(""), wxT(""),wxT("LeafPredictor Project XML (*.lpx)|*.lpx"), wxSAVE|wxFD_OVERWRITE_PROMPT, wxDefaultPosition);
		if (SaveDialog->ShowModal() == wxID_OK) // if the user click "Save" instead of "cancel"
		{
			mCurrentFilename = SaveDialog->GetPath();
		}
		else
			return false;
	}
	TiXmlDocument doc;
	TiXmlElement* msg;
	TiXmlElement* prediction;
	TiXmlComment * comment;
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "UTF-8", "yes");
	doc.LinkEndChild( decl );
	
	comment = new TiXmlComment();
	comment->SetValue(" LeafPredictor Project File ");
	doc.LinkEndChild( comment );

	TiXmlElement * root = new TiXmlElement( "LeafPredictor" );
	doc.LinkEndChild( root );
	wxString Version = wxT(LEAFPREDICTOR_VERSION);
	root->SetAttribute( "Version", Version.mb_str( wxConvUTF8 ) );
	
	comment = new TiXmlComment();
	comment->SetValue(" Project Settings ");
	root->LinkEndChild( comment );
	
	TiXmlElement * settings = new TiXmlElement( "Settings" );
	root->LinkEndChild( settings );
	
	msg = new TiXmlElement( "scale" );
	msg->LinkEndChild( new TiXmlText( wxString::Format(wxT("%i"),mManualScaleAmount->GetValue()).mb_str( wxConvUTF8 ) ) );
	settings->LinkEndChild( msg );
	
	msg = new TiXmlElement( "scaletype" );
	wxString scale;
	if (mManualScale != 0)
		scale = wxT("manual");
	else
	{
		if(mLinkedScale)
			scale = wxT("relative");
		else
			scale = wxT("independent");
	}
	msg->LinkEndChild( new TiXmlText( scale.mb_str( wxConvUTF8 ) ) );
	settings->LinkEndChild( msg );
	
	msg = new TiXmlElement( "inverted" );
	wxString inverted;
	if (mInvertLeaf)
		inverted = wxT("true");
	else
		inverted = wxT("false");
	msg->LinkEndChild( new TiXmlText( inverted.mb_str( wxConvUTF8 ) ) );
	settings->LinkEndChild( msg );
	
	msg = new TiXmlElement( "fourupmode" );
	wxString fourupmode;
	if (m4UpMode)
		fourupmode = wxT("true");
	else
		fourupmode = wxT("false");
	msg->LinkEndChild( new TiXmlText( fourupmode.mb_str( wxConvUTF8 ) ) );
	settings->LinkEndChild( msg );
	
	msg = new TiXmlElement( "showpcinfo" );
	wxString showpcinfo;
	if (mShowPCInfo)
		showpcinfo = wxT("true");
	else
		showpcinfo = wxT("false");
	msg->LinkEndChild( new TiXmlText( showpcinfo.mb_str( wxConvUTF8 ) ) );
	settings->LinkEndChild( msg );
	
	msg = new TiXmlElement( "showmeanoverlay" );
	wxString showmeanoverlay;
	if (mMeanOverlayMenu->IsChecked())
		showmeanoverlay = wxT("true");
	else
		showmeanoverlay = wxT("false");
	msg->LinkEndChild( new TiXmlText( showmeanoverlay.mb_str( wxConvUTF8 ) ) );
	settings->LinkEndChild( msg );
	
	msg = new TiXmlElement( "showlandmarks" );
	wxString showlandmarks;
	if (mShowLandmarks)
		showlandmarks = wxT("true");
	else
		showlandmarks = wxT("false");
	msg->LinkEndChild( new TiXmlText( showlandmarks.mb_str( wxConvUTF8 ) ) );
	settings->LinkEndChild( msg );
	
	comment = new TiXmlComment();
	comment->SetValue(" Statistical System ");
	root->LinkEndChild( comment );
	
	TiXmlElement * eigensystem = new TiXmlElement( "Eigensystem" );
	root->LinkEndChild( eigensystem );
	
	msg = new TiXmlElement( "CoordCount" );
	msg->LinkEndChild( new TiXmlText( wxString::Format(wxT("%i"),mEigenSystem.GetNumberOfCoordinates()).mb_str( wxConvUTF8 ) ) );
	eigensystem->LinkEndChild( msg );
	
	msg = new TiXmlElement( "EValueCount" );
	msg->LinkEndChild( new TiXmlText( wxString::Format(wxT("%i"),mEigenSystem.GetEigenValueCount()).mb_str( wxConvUTF8 ) ) );
	eigensystem->LinkEndChild( msg );
	
	msg = new TiXmlElement( "EVectorCount" );
	msg->LinkEndChild( new TiXmlText( wxString::Format(wxT("%i"),mEigenSystem.GetEigenVectorsCount()).mb_str( wxConvUTF8 ) ) );
	eigensystem->LinkEndChild( msg );
	
	msg = new TiXmlElement( "MeanLeaf" );
	msg->LinkEndChild( new TiXmlText( mEigenSystem.OutputMeanLeaf().mb_str( wxConvUTF8 ) ) );
	eigensystem->LinkEndChild( msg );
	
	msg = new TiXmlElement( "Eigenvalues" );
	msg->LinkEndChild( new TiXmlText( mEigenSystem.OutputEigenValues().mb_str( wxConvUTF8 ) ));
	eigensystem->LinkEndChild( msg );
			
	TiXmlElement * eigenvalues = new TiXmlElement( "Eigenvectors" );
	eigensystem->LinkEndChild( eigenvalues );
	
	for(wxUint32 i=0;i<mEigenSystem.GetEigenVectorsCount();i++)
	{
		msg = new TiXmlElement( "Eigenvector" );
		msg->LinkEndChild( new TiXmlText( mEigenSystem.OutputEigenVectors(i).mb_str( wxConvUTF8 ) ) );
		eigenvalues->LinkEndChild( msg );
	}
	
	comment = new TiXmlComment();
	comment->SetValue(" Prediction View ");
	root->LinkEndChild( comment );
	
	TiXmlElement * predictionview = new TiXmlElement( "PredictionView" );
	root->LinkEndChild( predictionview );
	
	prediction = new TiXmlElement( "Prediction1" );
	predictionview->LinkEndChild( prediction );
	
	msg = new TiXmlElement( "PC" );
	msg->SetAttribute( "Component", wxString::Format(wxT("%i"),mPredictedLeafCanvas1->GetPC1()).mb_str( wxConvUTF8 ));
	msg->SetAttribute( "Value", wxString::Format(wxT("%i"),mPredictedLeafCanvas1->GetPC1Value()).mb_str( wxConvUTF8 ));
	prediction->LinkEndChild( msg );
	
	msg = new TiXmlElement( "PC" );
	msg->SetAttribute( "Component", wxString::Format(wxT("%i"),mPredictedLeafCanvas1->GetPC2()).mb_str( wxConvUTF8 ));
	msg->SetAttribute( "Value", wxString::Format(wxT("%i"),mPredictedLeafCanvas1->GetPC2Value()).mb_str( wxConvUTF8 ));
	prediction->LinkEndChild( msg );
	
	msg = new TiXmlElement( "PC" );
	msg->SetAttribute( "Component", wxString::Format(wxT("%i"),mPredictedLeafCanvas1->GetPC3()).mb_str( wxConvUTF8 ));
	msg->SetAttribute( "Value", wxString::Format(wxT("%i"),mPredictedLeafCanvas1->GetPC3Value()).mb_str( wxConvUTF8 ));
	prediction->LinkEndChild( msg );
	
	msg = new TiXmlElement( "PC" );
	msg->SetAttribute( "Component", wxString::Format(wxT("%i"),mPredictedLeafCanvas1->GetPC4()).mb_str( wxConvUTF8 ));
	msg->SetAttribute( "Value", wxString::Format(wxT("%i"),mPredictedLeafCanvas1->GetPC4Value()).mb_str( wxConvUTF8 ));
	prediction->LinkEndChild( msg );

	prediction = new TiXmlElement( "Prediction2" );
	predictionview->LinkEndChild( prediction );
	
	msg = new TiXmlElement( "PC" );
	msg->SetAttribute( "Component", wxString::Format(wxT("%i"),mPredictedLeafCanvas2->GetPC1()).mb_str( wxConvUTF8 ));
	msg->SetAttribute( "Value", wxString::Format(wxT("%i"),mPredictedLeafCanvas2->GetPC1Value()).mb_str( wxConvUTF8 ));
	prediction->LinkEndChild( msg );
	
	msg = new TiXmlElement( "PC" );
	msg->SetAttribute( "Component", wxString::Format(wxT("%i"),mPredictedLeafCanvas2->GetPC2()).mb_str( wxConvUTF8 ));
	msg->SetAttribute( "Value", wxString::Format(wxT("%i"),mPredictedLeafCanvas2->GetPC2Value()).mb_str( wxConvUTF8 ));
	prediction->LinkEndChild( msg );
	
	msg = new TiXmlElement( "PC" );
	msg->SetAttribute( "Component", wxString::Format(wxT("%i"),mPredictedLeafCanvas2->GetPC3()).mb_str( wxConvUTF8 ));
	msg->SetAttribute( "Value", wxString::Format(wxT("%i"),mPredictedLeafCanvas2->GetPC3Value()).mb_str( wxConvUTF8 ));
	prediction->LinkEndChild( msg );
	
	msg = new TiXmlElement( "PC" );
	msg->SetAttribute( "Component", wxString::Format(wxT("%i"),mPredictedLeafCanvas2->GetPC4()).mb_str( wxConvUTF8 ));
	msg->SetAttribute( "Value", wxString::Format(wxT("%i"),mPredictedLeafCanvas2->GetPC4Value()).mb_str( wxConvUTF8 ));
	prediction->LinkEndChild( msg );

	prediction = new TiXmlElement( "Prediction3" );
	predictionview->LinkEndChild( prediction );
	
	msg = new TiXmlElement( "PC" );
	msg->SetAttribute( "Component", wxString::Format(wxT("%i"),mPredictedLeafCanvas3->GetPC1()).mb_str( wxConvUTF8 ));
	msg->SetAttribute( "Value", wxString::Format(wxT("%i"),mPredictedLeafCanvas3->GetPC1Value()).mb_str( wxConvUTF8 ));
	prediction->LinkEndChild( msg );
	
	msg = new TiXmlElement( "PC" );
	msg->SetAttribute( "Component", wxString::Format(wxT("%i"),mPredictedLeafCanvas3->GetPC2()).mb_str( wxConvUTF8 ));
	msg->SetAttribute( "Value", wxString::Format(wxT("%i"),mPredictedLeafCanvas3->GetPC2Value()).mb_str( wxConvUTF8 ));
	prediction->LinkEndChild( msg );
	
	msg = new TiXmlElement( "PC" );
	msg->SetAttribute( "Component", wxString::Format(wxT("%i"),mPredictedLeafCanvas3->GetPC3()).mb_str( wxConvUTF8 ));
	msg->SetAttribute( "Value", wxString::Format(wxT("%i"),mPredictedLeafCanvas3->GetPC3Value()).mb_str( wxConvUTF8 ));
	prediction->LinkEndChild( msg );
	
	msg = new TiXmlElement( "PC" );
	msg->SetAttribute( "Component", wxString::Format(wxT("%i"),mPredictedLeafCanvas3->GetPC4()).mb_str( wxConvUTF8 ));
	msg->SetAttribute( "Value", wxString::Format(wxT("%i"),mPredictedLeafCanvas3->GetPC4Value()).mb_str( wxConvUTF8 ));
	prediction->LinkEndChild( msg );
	
	doc.SaveFile( (const char*)mCurrentFilename.mb_str());
	SetUnsavedState(false);
	return true;
}


void MainDialog::OnMenuSave(wxCommandEvent& event)
{
	Save();
}


void MainDialog::OnMenuSaveAs(wxCommandEvent& event)
{
	Save(true);
}


void MainDialog::UpdateLeaves(bool all)
{
	mEigenSystem.PredictLeaf(1, 0, 1, 0, 1, 0, 1, 0);
	mMeanLeafCanvas->SetLeaf(mEigenSystem.GetMeanLeaf());

	if(all || mPredictedLeafCanvas2->IsShown())
	{
		mEigenSystem.PredictLeaf(mPredictedLeafCanvas1->GetPC1(), mPredictedLeafCanvas1->GetPC1Value(), mPredictedLeafCanvas1->GetPC2(), mPredictedLeafCanvas1->GetPC2Value(), mPredictedLeafCanvas1->GetPC3(), mPredictedLeafCanvas1->GetPC3Value(), mPredictedLeafCanvas1->GetPC4(), mPredictedLeafCanvas1->GetPC4Value());
		mPredictedLeafCanvas1->SetLeaf(mEigenSystem.GetPredictedLeaf());

		mEigenSystem.PredictLeaf(mPredictedLeafCanvas2->GetPC1(), mPredictedLeafCanvas2->GetPC1Value(), mPredictedLeafCanvas2->GetPC2(), mPredictedLeafCanvas2->GetPC2Value(), mPredictedLeafCanvas2->GetPC3(), mPredictedLeafCanvas2->GetPC3Value(), mPredictedLeafCanvas2->GetPC4(), mPredictedLeafCanvas2->GetPC4Value());
		mPredictedLeafCanvas2->SetLeaf(mEigenSystem.GetPredictedLeaf());

		mEigenSystem.PredictLeaf(mPredictedLeafCanvas3->GetPC1(), mPredictedLeafCanvas3->GetPC1Value(), mPredictedLeafCanvas3->GetPC2(), mPredictedLeafCanvas3->GetPC2Value(), mPredictedLeafCanvas3->GetPC3(), mPredictedLeafCanvas3->GetPC3Value(), mPredictedLeafCanvas3->GetPC4(), mPredictedLeafCanvas3->GetPC4Value());
		mPredictedLeafCanvas3->SetLeaf(mEigenSystem.GetPredictedLeaf());
	} else {

		if(mSelectedCanvas == wxT("Predicted Leaf 1"))
		{
			mEigenSystem.PredictLeaf(mPredictedLeafCanvas1->GetPC1(), mPredictedLeafCanvas1->GetPC1Value(), mPredictedLeafCanvas1->GetPC2(), mPredictedLeafCanvas1->GetPC2Value(), mPredictedLeafCanvas1->GetPC3(), mPredictedLeafCanvas1->GetPC3Value(), mPredictedLeafCanvas1->GetPC4(), mPredictedLeafCanvas1->GetPC4Value());
			mPredictedLeafCanvas1->SetLeaf(mEigenSystem.GetPredictedLeaf());
		}
		else if(mSelectedCanvas == wxT("Predicted Leaf 2"))
		{
			mEigenSystem.PredictLeaf(mPredictedLeafCanvas2->GetPC1(), mPredictedLeafCanvas2->GetPC1Value(), mPredictedLeafCanvas2->GetPC2(), mPredictedLeafCanvas2->GetPC2Value(), mPredictedLeafCanvas2->GetPC3(), mPredictedLeafCanvas2->GetPC3Value(), mPredictedLeafCanvas2->GetPC4(), mPredictedLeafCanvas2->GetPC4Value());
			mPredictedLeafCanvas2->SetLeaf(mEigenSystem.GetPredictedLeaf());
		}
		else
		{
			mEigenSystem.PredictLeaf(mPredictedLeafCanvas3->GetPC1(), mPredictedLeafCanvas3->GetPC1Value(), mPredictedLeafCanvas3->GetPC2(), mPredictedLeafCanvas3->GetPC2Value(), mPredictedLeafCanvas3->GetPC3(), mPredictedLeafCanvas3->GetPC3Value(), mPredictedLeafCanvas3->GetPC4(), mPredictedLeafCanvas3->GetPC4Value());
			mPredictedLeafCanvas3->SetLeaf(mEigenSystem.GetPredictedLeaf());
		}
	}

	if(mLinkedScale)
	{
		double meanScale, pred1Scale, pred2Scale, pred3Scale;

		pred1Scale = mPredictedLeafCanvas1->GetScale();
		pred2Scale = mPredictedLeafCanvas2->GetScale();
		pred3Scale = mPredictedLeafCanvas3->GetScale();
		meanScale = mMeanLeafCanvas->GetScale();
		if(!mPredictedLeafCanvas2->IsShown()){
			mMeanLeafCanvas->SetScale(std::min(pred1Scale,meanScale));
			mPredictedLeafCanvas1->SetScale(std::min(pred1Scale,meanScale));
		} else {
			mMeanLeafCanvas->SetScale(std::min(std::min(pred1Scale,meanScale),std::min(pred2Scale, pred3Scale)));
			mPredictedLeafCanvas1->SetScale(std::min(std::min(pred1Scale,meanScale),std::min(pred2Scale, pred3Scale)));
			mPredictedLeafCanvas2->SetScale(std::min(std::min(pred1Scale,meanScale),std::min(pred2Scale, pred3Scale)));
			mPredictedLeafCanvas3->SetScale(std::min(std::min(pred1Scale,meanScale),std::min(pred2Scale, pred3Scale)));
		}
	}
	mMeanLeafCanvas->ExtDraw();
	mPredictedLeafCanvas1->ExtDraw();
	if(mPredictedLeafCanvas2->IsShown())
	{
		mPredictedLeafCanvas2->ExtDraw();
		mPredictedLeafCanvas3->ExtDraw();
	}
}


void MainDialog::OnMenuRelative(wxCommandEvent& event)
{
	mManualScale = 0;
	mLinkedScale = true;
	UpdateLeaves();
	mManualScaleAmount->Show(false);
	mTopLevelSizer->Layout();
	SetUnsavedState(true);
}


void MainDialog::OnMenuIndependent(wxCommandEvent& event)
{
	mManualScale = 0;
	mLinkedScale = false;
	UpdateLeaves();
	mManualScaleAmount->Show(false);
	mTopLevelSizer->Layout();
	SetUnsavedState(true);
}


void MainDialog::OnMenuManual(wxCommandEvent& event)
{
	mManualScale = (double)mManualScaleAmount->GetValue() / 50;
	UpdateLeaves();
	mManualScaleAmount->Show(true);
	mTopLevelSizer->Layout();
	SetUnsavedState(true);
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
	if(mSelectedCanvas == wxT("Predicted Leaf 1"))
	{
		mPredictedLeafCanvas1->SetPC1Value(0);
		mPredictedLeafCanvas1->SetPC2Value(0);
		mPredictedLeafCanvas1->SetPC3Value(0);
		mPredictedLeafCanvas1->SetPC4Value(0);
	}
	else if(mSelectedCanvas == wxT("Predicted Leaf 2"))
	{
		mPredictedLeafCanvas2->SetPC1Value(0);
		mPredictedLeafCanvas2->SetPC2Value(0);
		mPredictedLeafCanvas2->SetPC3Value(0);
		mPredictedLeafCanvas2->SetPC4Value(0);
	}
	else
	{
		mPredictedLeafCanvas3->SetPC1Value(0);
		mPredictedLeafCanvas3->SetPC2Value(0);
		mPredictedLeafCanvas3->SetPC3Value(0);
		mPredictedLeafCanvas3->SetPC4Value(0);
	}
	UpdateLeaves();
	SetUnsavedState(true);
}


void MainDialog::OnMenuMeanScreen(wxCommandEvent& event)
{
	wxString selectedFile;
	wxBitmap screenShot;

	wxFileDialog *SaveDialog = new wxFileDialog(this, _("Choose location to save mean leaf image"), wxT(""), wxT(""),wxT("Vector Graphics (*.svg)|*.svg|PNG Images (*.png)|*.png"), wxSAVE|wxFD_OVERWRITE_PROMPT, wxDefaultPosition);
	if (SaveDialog->ShowModal() == wxID_OK) // if the user click "Open" instead of "cancel"
	{
		selectedFile = SaveDialog->GetPath();
		if (SaveDialog->GetFilterIndex() == 1)
		{
			screenShot = (mMeanLeafCanvas->GetScreenShot());
			screenShot.SaveFile(selectedFile, wxBITMAP_TYPE_PNG);
		}
		else
		{
			mMeanLeafCanvas->CreateSVGFile(selectedFile);
		}
	}
}


void MainDialog::OnMenuPredScreen(wxCommandEvent& event)
{
	wxString selectedFile;
	wxBitmap screenShot;

	wxFileDialog *SaveDialog = new wxFileDialog(this, _("Choose location to save predicted leaf image"), wxT(""), wxT(""),wxT("Vector Graphics (*.svg)|*.svg|PNG Images (*.png)|*.png"), wxSAVE|wxFD_OVERWRITE_PROMPT, wxDefaultPosition);
	if (SaveDialog->ShowModal() == wxID_OK) // if the user click "Open" instead of "cancel"
	{
		selectedFile = SaveDialog->GetPath();
		if(SaveDialog->GetFilterIndex() == 1)
		{
			if(mSelectedCanvas == wxT("Predicted Leaf 1"))
				screenShot = (mPredictedLeafCanvas1->GetScreenShot());
			else if(mSelectedCanvas == wxT("Predicted Leaf 2"))
				screenShot = (mPredictedLeafCanvas2->GetScreenShot());
			else
				screenShot = (mPredictedLeafCanvas3->GetScreenShot());
			screenShot.SaveFile(selectedFile, wxBITMAP_TYPE_PNG);
		} else {
			if(mSelectedCanvas == wxT("Predicted Leaf 1"))
				mPredictedLeafCanvas1->CreateSVGFile(selectedFile);
			else if(mSelectedCanvas == wxT("Predicted Leaf 2"))
				mPredictedLeafCanvas2->CreateSVGFile(selectedFile);
			else
				mPredictedLeafCanvas3->CreateSVGFile(selectedFile);		
		}
	}
}


void MainDialog::OnMenuImportLeaf(wxCommandEvent& event)
{
	wxString selectedFile;

	wxFileDialog *OpenDialog = new wxFileDialog(this, _("Select a Leaf file to import"), wxT(""), wxT(""),wxT("Text Files (*.txt)|*.txt"), wxOPEN|wxFD_FILE_MUST_EXIST, wxDefaultPosition);
	if (OpenDialog->ShowModal() == wxID_OK) // if the user click "Open" instead of "cancel"
	{
		selectedFile = OpenDialog->GetPath();
		mEigenSystem.LoadLeafFile(selectedFile);
		mMeanLeafCanvas->SetLeaf(mEigenSystem.GetMeanLeaf());
		UpdateLeaves();
	}
	SetUnsavedState(true);
}

void MainDialog::OnMenuInvertLeaf(wxCommandEvent& event)
{
	mInvertLeaf = !mInvertLeaf;
	mEigenSystem.InvertLeaf();
	UpdateLeaves(true);
	SetUnsavedState(true);
}


void MainDialog::OnMenuShowLandmarks(wxCommandEvent& event)
{
	mShowLandmarks = !mShowLandmarks;
	UpdateLeaves(true);
	SetUnsavedState(true);
}


void MainDialog::OnMenu4UpLeaf(wxCommandEvent& event)
{
	m4UpMode = !m4UpMode;
	mPredictedLeafCanvas2->Show(m4UpMode);
	mPredictedLeafCanvas3->Show(m4UpMode);
	mTopLevelSizer->Layout();
	if(!m4UpMode)
	{
		mSelectedCanvas = wxT("Predicted Leaf 1");
		SetStatusText(wxString::Format(_("Selected Leaf: %i"),1), STATUS_LEAF);
	}
	UpdateLeaves(true);
	SetUnsavedState(true);
}


void MainDialog::OnMenuShowPCInfo(wxCommandEvent& event)
{
	mShowPCInfo = !mShowPCInfo;
	UpdateLeaves(true);
	SetUnsavedState(true);
}


void MainDialog::OnClick(wxMouseEvent& event)
{
}

void MainDialog::ExternalCanvasSelect(const wxString& canvas)
{
	float tmpFloat;

	if(canvas != wxT("Mean Leaf"))
		mSelectedCanvas = canvas;
	if(mSelectedCanvas == wxT("Predicted Leaf 1"))
	{
		mPC1Amount->SetValue(mPredictedLeafCanvas1->GetPC1Value());
		tmpFloat = mPC1Amount->GetValue();
		mPC1Value->SetLabel(wxString::Format(wxT("%.1f"),tmpFloat/10));
		mChoice1->SetSelection(mPredictedLeafCanvas1->GetPC1());

		mPC2Amount->SetValue(mPredictedLeafCanvas1->GetPC2Value());
		tmpFloat = mPC2Amount->GetValue();
		mPC2Value->SetLabel(wxString::Format(wxT("%.1f"),tmpFloat/10));
		mChoice2->SetSelection(mPredictedLeafCanvas1->GetPC2());

		mPC3Amount->SetValue(mPredictedLeafCanvas1->GetPC3Value());
		tmpFloat = mPC3Amount->GetValue();
		mPC3Value->SetLabel(wxString::Format(wxT("%.1f"),tmpFloat/10));
		mChoice3->SetSelection(mPredictedLeafCanvas1->GetPC3());

		mPC4Amount->SetValue(mPredictedLeafCanvas1->GetPC4Value());
		tmpFloat = mPC4Amount->GetValue();
		mPC4Value->SetLabel(wxString::Format(wxT("%.1f"),tmpFloat/10));
		mChoice4->SetSelection(mPredictedLeafCanvas1->GetPC4());

		SetStatusText(wxString::Format(_("Selected Leaf: %i"),1), STATUS_LEAF);

		mPredictedLeafCanvas1->Select(true);
		mPredictedLeafCanvas2->Select(false);
		mPredictedLeafCanvas3->Select(false);
	}
	else if(mSelectedCanvas == wxT("Predicted Leaf 2"))
	{
		mPC1Amount->SetValue(mPredictedLeafCanvas2->GetPC1Value());
		tmpFloat = mPC1Amount->GetValue();
		mPC1Value->SetLabel(wxString::Format(wxT("%.1f"),tmpFloat/10));
		mChoice1->SetSelection(mPredictedLeafCanvas2->GetPC1());

		mPC2Amount->SetValue(mPredictedLeafCanvas2->GetPC2Value());
		tmpFloat = mPC2Amount->GetValue();
		mPC2Value->SetLabel(wxString::Format(wxT("%.1f"),tmpFloat/10));
		mChoice2->SetSelection(mPredictedLeafCanvas2->GetPC2());

		mPC3Amount->SetValue(mPredictedLeafCanvas2->GetPC3Value());
		tmpFloat = mPC3Amount->GetValue();
		mPC3Value->SetLabel(wxString::Format(wxT("%.1f"),tmpFloat/10));
		mChoice3->SetSelection(mPredictedLeafCanvas2->GetPC3());

		mPC4Amount->SetValue(mPredictedLeafCanvas2->GetPC4Value());
		tmpFloat = mPC4Amount->GetValue();
		mPC4Value->SetLabel(wxString::Format(wxT("%.1f"),tmpFloat/10));
		mChoice4->SetSelection(mPredictedLeafCanvas2->GetPC4());

		SetStatusText(wxString::Format(_("Selected Leaf: %i"),2), STATUS_LEAF);

		mPredictedLeafCanvas1->Select(false);
		mPredictedLeafCanvas2->Select(true);
		mPredictedLeafCanvas3->Select(false);
	}
	else
	{
		mPC1Amount->SetValue(mPredictedLeafCanvas3->GetPC1Value());
		tmpFloat = mPC1Amount->GetValue();
		mPC1Value->SetLabel(wxString::Format(wxT("%.1f"),tmpFloat/10));
		mChoice1->SetSelection(mPredictedLeafCanvas3->GetPC1());

		mPC2Amount->SetValue(mPredictedLeafCanvas3->GetPC2Value());
		tmpFloat = mPC2Amount->GetValue();
		mPC2Value->SetLabel(wxString::Format(wxT("%.1f"),tmpFloat/10));
		mChoice2->SetSelection(mPredictedLeafCanvas3->GetPC2());

		mPC3Amount->SetValue(mPredictedLeafCanvas3->GetPC3Value());
		tmpFloat = mPC3Amount->GetValue();
		mPC3Value->SetLabel(wxString::Format(wxT("%.1f"),tmpFloat/10));
		mChoice3->SetSelection(mPredictedLeafCanvas3->GetPC3());

		mPC4Amount->SetValue(mPredictedLeafCanvas3->GetPC4Value());
		tmpFloat = mPC4Amount->GetValue();
		mPC4Value->SetLabel(wxString::Format(wxT("%.1f"),tmpFloat/10));
		mChoice4->SetSelection(mPredictedLeafCanvas3->GetPC4());

		SetStatusText(wxString::Format(_("Selected Leaf: %i"),3), STATUS_LEAF);

		mPredictedLeafCanvas1->Select(false);
		mPredictedLeafCanvas2->Select(false);
		mPredictedLeafCanvas3->Select(true);
	}
	UpdateLeaves();
}

void MainDialog::OnMenuExportLeaf(wxCommandEvent& event)
{
	wxString selectedFile;

	wxFileDialog *SaveDialog = new wxFileDialog(this, _("Choose location to save predicted leaf"), wxT(""), wxT(""),wxT("LeafAnalyser Leaves (*.txt)|*.txt"), wxSAVE|wxFD_OVERWRITE_PROMPT, wxDefaultPosition);
	if (SaveDialog->ShowModal() == wxID_OK) // if the user click "Open" instead of "cancel"
	{
		selectedFile = SaveDialog->GetPath();
		if(mSelectedCanvas == wxT("Predicted Leaf 1"))
			mPredictedLeafCanvas1->ExportLeaf(selectedFile);
		else if(mSelectedCanvas == wxT("Predicted Leaf 2"))
			mPredictedLeafCanvas2->ExportLeaf(selectedFile);
		else
			mPredictedLeafCanvas3->ExportLeaf(selectedFile);

	}
}


void MainDialog::OnMenuShowLeafViewer(wxCommandEvent&)
{
	LeafDialog* dialog = new LeafDialog(NULL, LEAF_VIEWER);
	dialog->Show(true);
}


void MainDialog::OnMenuShowProjectManager(wxCommandEvent&)
{
	ProjectDialog::GetInstance()->Show();
}


void MainDialog::OnMenuGenerateGrid(wxCommandEvent&)
{
	wxInt32 PC1 = 0;
	wxInt32 PC2 = 0;
	wxInt32 PC3 = 0;
	wxInt32 PC4 = 0;
	wxInt32 PC1Start = 0;
	wxInt32 PC2Start = 0;
	wxInt32 PC3Start = 0;
	wxInt32 PC4Start = 0;
	wxInt32 PC1End = 0;
	wxInt32 PC2End = 0;
	wxInt32 PC3End = 0;
	wxInt32 PC4End = 0;
	wxInt32 PC1Int = 1;
	wxInt32 PC2Int = 1;
	wxInt32 PC3Int = 1;
	wxInt32 PC4Int = 1;
	wxString Location = wxT("./");

	PCSpaceDialog dialog(NULL, PC_SPACE_GEN);
	if(dialog.ShowModal() == wxID_OK)
	{
		double tmpDouble;
		PC1 = dialog.GetPC1();
		dialog.GetPC1Start().ToDouble(&tmpDouble);
		PC1Start = (int)(tmpDouble * 10);
		dialog.GetPC1End().ToDouble(&tmpDouble);
		PC1End = (int)(tmpDouble * 10);
		dialog.GetPC1Int().ToDouble(&tmpDouble);
		PC1Int = (int)(tmpDouble * 10);
		if(dialog.IsPC2Enabled())
		{
			PC2 = dialog.GetPC2();
			dialog.GetPC2Start().ToDouble(&tmpDouble);
			PC2Start = (int)(tmpDouble * 10);
			dialog.GetPC2End().ToDouble(&tmpDouble);
			PC2End = (int)(tmpDouble * 10);
			dialog.GetPC2Int().ToDouble(&tmpDouble);
			PC2Int = (int)(tmpDouble * 10);
		}
		if(dialog.IsPC3Enabled())
		{
			PC3 = dialog.GetPC3();
			dialog.GetPC3Start().ToDouble(&tmpDouble);
			PC3Start = (int)(tmpDouble * 10);
			dialog.GetPC3End().ToDouble(&tmpDouble);
			PC3End = (int)(tmpDouble * 10);
			dialog.GetPC3Int().ToDouble(&tmpDouble);
			PC3Int = (int)(tmpDouble * 10);
		}
		if(dialog.IsPC4Enabled())
		{
			PC4 = dialog.GetPC4();
			dialog.GetPC4Start().ToDouble(&tmpDouble);
			PC4Start = (int)(tmpDouble * 10);
			dialog.GetPC4End().ToDouble(&tmpDouble);
			PC4End = (int)(tmpDouble * 10);
			dialog.GetPC4Int().ToDouble(&tmpDouble);
			PC4Int = (int)(tmpDouble * 10);
		}
		Location = dialog.GetLocation();
		for(wxInt32 i=PC1Start; i<=PC1End;i+=PC1Int)
		{
			for(wxInt32 j=PC2Start; j<=PC2End; j+=PC2Int)
			{
				for(wxInt32 k=PC3Start; k<=PC3End; k+=PC3Int)
				{
					for(wxInt32 l=PC4Start; l<=PC4End; l+=PC4Int)
					{
						mEigenSystem.PredictLeaf(PC1, i, PC2, j, PC3, k, PC4, l);
						mEigenSystem.ExportLeaf(wxString::Format(wxT("%s/[%i]-[%i]-[%i]-[%i].txt"), Location.c_str(), i, j, k, l));

					}
				}
			}
		}
	}
}


void MainDialog::OnMenuMeanOverlay(wxCommandEvent& event)
{
	if(mMeanOverlayMenu->IsChecked())
	{
		mPredictedLeafCanvas1->SetOverlay(mEigenSystem.GetMeanLeaf());
		mPredictedLeafCanvas2->SetOverlay(mEigenSystem.GetMeanLeaf());
		mPredictedLeafCanvas3->SetOverlay(mEigenSystem.GetMeanLeaf());
	}
	else
	{
		mPredictedLeafCanvas1->RemoveOverlay();
		mPredictedLeafCanvas2->RemoveOverlay();
		mPredictedLeafCanvas3->RemoveOverlay();
	}
	UpdateLeaves();
	SetUnsavedState(true);
}


void MainDialog::OnMSAScroll(wxScrollEvent& event)
{
	mManualScale = (double)mManualScaleAmount->GetValue() / 50;
	UpdateLeaves();
	SetUnsavedState(true);
}


void MainDialog::SetEigenSystem(EigenSystem ES)
{
	mEigenSystem.Empty();
	mEigenSystem = ES;
	mInvertLeaf = GetEigenSystemPointer()->GetInversionFactor();
	mInvertLeafMenu->Check(mInvertLeaf);
	mMeanLeafCanvas->SetLeaf(mEigenSystem.GetMeanLeaf());
	mMeanLeafCanvas->ExtDraw();
	mPredictedLeafCanvas1->SetLeaf(mEigenSystem.GetPredictedLeaf());
	mPredictedLeafCanvas1->ExtDraw();
	mPredictedLeafCanvas2->SetLeaf(mEigenSystem.GetPredictedLeaf());
	mPredictedLeafCanvas2->ExtDraw();
	mPredictedLeafCanvas3->SetLeaf(mEigenSystem.GetPredictedLeaf());
	mPredictedLeafCanvas3->ExtDraw();
	SetUnsavedState(true);
}


LeafCanvas* MainDialog::GetSelectedCanvas(void)
{
	if(mSelectedCanvas == wxT("Predicted Leaf 1"))
		return mPredictedLeafCanvas1;
	else if(mSelectedCanvas == wxT("Predicted Leaf 2"))
		return mPredictedLeafCanvas2;
	else if(mSelectedCanvas == wxT("Predicted Leaf 3"))
		return mPredictedLeafCanvas3;
	else
		return NULL;
}


void MainDialog::SetUnsavedState(bool state)
{
	if(state && (mCurrentFilename.Cmp(wxT("")) != 0))
	{
		mSaveProjectMenu->Enable(true);
	}
	else
	{
		mSaveProjectMenu->Enable(false);
	}
		mUnsavedState = state;
}

