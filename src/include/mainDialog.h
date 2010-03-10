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

#ifndef _MAINDIALOG_H
#define _MAINDIALOG_H

#ifdef __GNUG__
#pragma interface "mainDialog.cpp"
#endif

#include "eigenSystem.h"
#include "leafCanvas.h"

#include "wx/frame.h"
#include "wx/choice.h"
#include "wx/slider.h"
#include "wx/stattext.h"
#include "wx/menu.h"
#include "wx/sizer.h"


/**
* This is the main dialog box.
* This class can only be instantiated once.
**/
class MainDialog : public wxFrame
{
protected:
	static MainDialog *mInstance; /**< The single instance of the Main Dialog */
	static wxMutex     mMutexUpdateCheck; /**< Locks access to the update method */

	EigenSystem        mEigenSystem;
	wxString           mPC1[96];
	wxString           mPC2[96];
	wxString           mPC3[96];
	wxString           mPC4[96];
	wxString           mSelectedCanvas;
	wxChoice           *mChoice1;
	wxChoice           *mChoice2;
	wxChoice           *mChoice3;
	wxChoice           *mChoice4;
	wxSlider           *mPC1Amount;
	wxSlider           *mPC2Amount;
	wxSlider           *mPC3Amount;
	wxSlider           *mPC4Amount;
	wxSlider           *mManualScaleAmount;
	wxStaticText       *mPC1Value;
	wxStaticText       *mPC2Value;
	wxStaticText       *mPC3Value;
	wxStaticText       *mPC4Value;
	LeafCanvas*        mMeanLeafCanvas;
	LeafCanvas*        mPredictedLeafCanvas1;
	LeafCanvas*        mPredictedLeafCanvas2;
	LeafCanvas*        mPredictedLeafCanvas3;
	bool               mLinkedScale;
	bool               mInvertLeaf;
	bool               m4UpMode;
	bool               mShowLandmarks;
	wxMenuItem        *mRelativeMenu;
	wxMenuItem        *mIndependentMenu;
	wxMenuItem        *mManualMenu;
	wxMenuItem        *mResetMenu;
	wxMenuItem        *mImportLeafMenu;
	wxMenuItem        *mInvertLeafMenu;
	wxMenuItem        *m4UpLeafMenu;
	wxMenuItem        *mExportLeafMenu;
	wxMenuItem        *mGridGenMenu;
	wxMenuItem        *mMeanOverlayMenu;
	wxMenuItem        *mShowLandmarksMenu;
	wxBoxSizer        *mTopLevelSizer;
	wxGridSizer       *mLowerPlotSizer;
	wxPanel           *mTopLevelPanel;
	double             mManualScale;

	/**
	 * Constructor.
	 * Constructs the main dialog window.
	 **/
	MainDialog(void);

	/**
	 * Destructor.
	 **/
	~MainDialog(void);

	/**
	 * Create the menubar.
	 **/
	void CreateMenuBar(void);

	/**
	 * Create the needed widgets and place them in the frame.
	 **/
	void CreateLayout(void);

	// Events
	/**
	 * Event: Manage the 'About' menu item.
	 * Display the about box.
	 * @param event The event itself. This is sent automatically.
	 **/
	void OnMenuAbout(wxCommandEvent& event);

	void OnMenuOpen(wxCommandEvent& event);

	void OnMenuSave(wxCommandEvent& event);

	void OnMenuImportES(wxCommandEvent& event);

	void OnMenuImportLeaf(wxCommandEvent& event);

	void OnMenuRelative(wxCommandEvent& event);

	void OnMenuIndependent(wxCommandEvent& event);

	void OnMenuManual(wxCommandEvent& event);

	void OnMenuResetPCs(wxCommandEvent& event);

	void OnMenuMeanScreen(wxCommandEvent& event);

	void OnMenuPredScreen(wxCommandEvent& event);

	void OnMenuInvertLeaf(wxCommandEvent& event);

	void OnMenu4UpLeaf(wxCommandEvent& event);

	void OnMenuExportLeaf(wxCommandEvent& event);

	void OnMenuShowLeafViewer(wxCommandEvent& event);

	void OnMenuShowProjectManager(wxCommandEvent& event);

	void OnMenuGenerateGrid(wxCommandEvent& event);

	void OnMenuMeanOverlay(wxCommandEvent& event);

	void OnMenuShowLandmarks(wxCommandEvent& event);

	void OnChoices(wxCommandEvent& event);

	void OnScroll(wxScrollEvent& event);

	void OnMSAScroll(wxScrollEvent& event);

	void OnClick(wxMouseEvent& event);

	/**
	 * Event: Triggered when the application is closed.
	 * The application is going to stop, perform some last actions and/or decide if we are really going to quit.
	 * @param event The event itself. This is sent automatically.
	 **/
	void OnClose(wxCloseEvent& event);

	/**
	 * Event: Manage the 'Quit' menu item.
	 * Called when the user wants to quit the application.
	 **/
	void OnMenuQuit(wxCommandEvent& event);

public:
	// Singleton pattern
	/**
	 * Create the instance of mainDialog.
	 **/
	static void CreateInstance(void);

	/**
	 * Destroy the single instance of the mainDialog.
	 * Only if it exists.
	 **/
	static void DestroyInstance(void);

	/**
	 * Check whether this dialog has been already instanciated.
	 * @return State of instanciation.
	 **/
	static bool HasBeenInstanciated(void);

	/**
	 * Retrieve the instance of mainDialog.
	 * If an instance doesn't already exist, create it.
	 * @return An instance of class mainDialog.
	 **/
	static MainDialog* GetInstance(void);


	/**
	 * Redefine the wxFrame::Show() method in order to perform some initializations before displaying the frame.
	 * @param show Should the frame actually be shown.
	 **/
	bool Show(bool show = true);

	void SetPCMessage(wxString msg);

	void UpdateLeaves(bool all = false);

	bool GetInvertLeaf(void) {return mInvertLeaf;}

	void ExternalCanvasSelect(const wxString& canvas);

	EigenSystem GetEigenSystem(void) {return mEigenSystem;}

	EigenSystem* GetEigenSystemPointer(void) {return &mEigenSystem;}

	double GetManualScale(void) {return mManualScale;}

	void SetEigenSystem(EigenSystem ES);

	bool GetShowLandmarks(void) {return mShowLandmarks;}

	LeafCanvas* GetSelectedCanvas(void);

private:
	DECLARE_EVENT_TABLE()
};

#endif /* _MAINDIALOG_H */
