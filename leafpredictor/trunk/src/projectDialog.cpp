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
#pragma implementation "projectDialog.h"
#endif

#include "wx/wxprec.h"

#ifdef  __BORLANDC__
#pragma hdrstop
#endif

#include "leafpredictor.h"
#include "projectDialog.h"
#include "mainDialog.h"
#include "leafPrediction.h"

#include "staticBoldedText.h"
#include "tools.h"

#include "wx/sizer.h"
#include "wx/textdlg.h"
#include "wx/panel.h"

enum _CONTROL_ID
{
	PROJECT_TREE,
	MID_ADD_ES,
	MID_ADD_PL,
	MID_ADD_IL,
	MID_DEL_ITEM,
	MID_USE_ITEM
};

BEGIN_EVENT_TABLE(ProjectDialog, wxFrame)

		EVT_CLOSE                (ProjectDialog::OnClose)
		EVT_TREE_ITEM_MENU(PROJECT_TREE, ProjectDialog::OnItemRClick)
		EVT_MENU(MID_ADD_ES, ProjectDialog::OnAddESMenu)
		EVT_MENU(MID_ADD_PL, ProjectDialog::OnAddPLMenu)
		EVT_MENU(MID_ADD_IL, ProjectDialog::OnAddILMenu)
		EVT_MENU(MID_DEL_ITEM, ProjectDialog::OnDelItemMenu)
		EVT_MENU(MID_USE_ITEM, ProjectDialog::OnUseItemMenu)

END_EVENT_TABLE()

ProjectDialog* ProjectDialog::mInstance = NULL;


ProjectDialog::ProjectDialog(void) : wxFrame(NULL, wxID_ANY, _("Project Manager"))
{
	wxBoxSizer      *topLevelSizer;
	wxBoxSizer      *infoSizer;
	wxPanel         *topLevelPanel;

	topLevelPanel = new wxPanel(this, wxID_ANY);
	infoSizer = new wxBoxSizer(wxVERTICAL);

	mProjectTree = new wxTreeCtrl(topLevelPanel, PROJECT_TREE, wxDefaultPosition, wxSize(240,300));
	infoSizer->Add(mProjectTree, 1, wxEXPAND | wxALL, 5);
	infoSizer->AddSpacer(5);

	topLevelSizer = new wxBoxSizer(wxVERTICAL);

	topLevelSizer->Add(infoSizer, 1, wxEXPAND | wxALL, 5);
	topLevelPanel->SetSizer(topLevelSizer);
	topLevelSizer->Fit(this);

	mProjectRoot = mProjectTree->AddRoot(_("LeafPredictor Project"));
	mEigenSystemTree = mProjectTree->AppendItem(mProjectRoot, _("EigenSystems"));
	mPredictionTree = mProjectTree->AppendItem(mProjectRoot, _("Leaf Predictions"));
	mExternalLeafTree = mProjectTree->AppendItem(mProjectRoot, _("Imported Leaves"));
}


ProjectDialog::~ProjectDialog(void)
{
}


void ProjectDialog::CreateInstance(void)
{
	wxASSERT(mInstance == NULL);

	mInstance = new ProjectDialog();
}


ProjectDialog* ProjectDialog::GetInstance(void)
{
	if(mInstance == NULL)
	{
		mInstance = new ProjectDialog();
	}

	return mInstance;
}


void ProjectDialog::DestroyInstance(void)
{
	if(mInstance != NULL)
	{
		mInstance->Destroy();
		mInstance = NULL;
	}
}


int ProjectDialog::Show(void)
{
	return wxFrame::Show();
}


void ProjectDialog::OnClose(wxCloseEvent& event)
{
	Hide();
}

void ProjectDialog::OnItemRClick(wxTreeEvent& event)
{
	wxTreeItemId itemId = event.GetItem();
	wxPoint clientpt = event.GetPoint();

	ShowMenu(itemId, clientpt);
	event.Skip();
}


void ProjectDialog::ShowMenu(wxTreeItemId id, const wxPoint& pt)
{
	wxString title;
	if ( id.IsOk() )
	{
		title << wxT("Menu for ") << mProjectTree->GetItemText(id);
	}
	else
	{
		title = wxT("Menu for no particular item");
	}
	wxMenu menu(title);
	if(mProjectTree->GetItemText(id) == _("EigenSystems"))
		menu.Append(MID_ADD_ES, _("&Add EigenSystem"));
	else if(mProjectTree->GetItemText(id) == _("Leaf Predictions"))
		menu.Append(MID_ADD_PL, _("&Add Predicted Leaf"));
	else if(mProjectTree->GetItemText(id) == _("Imported Leaves"))
		menu.Append(MID_ADD_IL, _("&Add Imported Leaf"));
	else
	{
		menu.Append(MID_USE_ITEM, wxT("&Use item"));
		menu.Append(MID_DEL_ITEM, wxT("&Delete item"));
	}

	PopupMenu(&menu, pt);
}


void ProjectDialog::OnAddESMenu(wxCommandEvent& event)
{
	wxTreeItemId id = mProjectTree->GetSelection();

	wxTextEntryDialog dialog(this, _("EigenSystem Name\n"), _("Please enter a string"), _("Default value"), wxOK);

	if (dialog.ShowModal() == wxID_OK)
	{
		mProjectTree->AppendItem(id, dialog.GetValue(), -1, -1, new EigenSystemData(MainDialog::GetInstance()->GetEigenSystem()));
	}
}


void ProjectDialog::OnAddPLMenu(wxCommandEvent& event)
{
	wxTreeItemId id = mProjectTree->GetSelection();
	LeafPrediction mPrediction;

	wxTextEntryDialog dialog(this, _("Predicted Leaf Name\n"), _("Please enter a string"), _("Default value"), wxOK);

	if (dialog.ShowModal() == wxID_OK)
	{
		mPrediction.SetPC1(MainDialog::GetInstance()->GetSelectedCanvas()->GetPC1());
		mPrediction.SetPC2(MainDialog::GetInstance()->GetSelectedCanvas()->GetPC2());
		mPrediction.SetPC3(MainDialog::GetInstance()->GetSelectedCanvas()->GetPC3());
		mPrediction.SetPC4(MainDialog::GetInstance()->GetSelectedCanvas()->GetPC4());
		mPrediction.SetPC1Value(MainDialog::GetInstance()->GetSelectedCanvas()->GetPC1Value());
		mPrediction.SetPC2Value(MainDialog::GetInstance()->GetSelectedCanvas()->GetPC2Value());
		mPrediction.SetPC3Value(MainDialog::GetInstance()->GetSelectedCanvas()->GetPC3Value());
		mPrediction.SetPC4Value(MainDialog::GetInstance()->GetSelectedCanvas()->GetPC4Value());

		mProjectTree->AppendItem(id, dialog.GetValue(), -1, -1, new PredictedLeafData(mPrediction));
	}
}


void ProjectDialog::OnAddILMenu(wxCommandEvent& event)
{
	wxTreeItemId id = mProjectTree->GetSelection();

	wxTextEntryDialog dialog(this, _("Imported Leaf Name\n"), _("Please enter a string"), _("Default value"), wxOK);

	if (dialog.ShowModal() == wxID_OK)
	{
		mProjectTree->AppendItem(id, dialog.GetValue(), -1, -1, new ImportedLeafData(MainDialog::GetInstance()->GetEigenSystem().GetMeanLeaf()));
	}
}


void ProjectDialog::OnDelItemMenu(wxCommandEvent& event)
{
	wxTreeItemId id = mProjectTree->GetSelection();
	if( !((mProjectTree->GetItemText(id) == _("EigenSystems")) || (mProjectTree->GetItemText(id) == _("Imported Leaves")) || (mProjectTree->GetItemText(id) == _("Leaf Predictions"))) )
		mProjectTree->Delete(id);
}


void ProjectDialog::OnUseItemMenu(wxCommandEvent& event)
{
	wxTreeItemId id = mProjectTree->GetSelection();
	wxTreeItemId parent = mProjectTree->GetItemParent(id);
	if(mProjectTree->GetItemText(parent) == _("EigenSystems"))
	{
		EigenSystemData *item = (EigenSystemData *)mProjectTree->GetItemData(id);
		MainDialog::GetInstance()->SetEigenSystem(item->GetEigenSystem());
		MainDialog::GetInstance()->UpdateLeaves(true);
	}
	if(mProjectTree->GetItemText(parent) == _("Imported Leaves"))
	{
		ImportedLeafData *item = (ImportedLeafData *)mProjectTree->GetItemData(id);
		MainDialog::GetInstance()->GetEigenSystemPointer()->SetLeaf( item->GetLeaf());
		MainDialog::GetInstance()->UpdateLeaves(true);
	}
	if(mProjectTree->GetItemText(parent) == _("Leaf Predictions"))
	{
		PredictedLeafData *item = (PredictedLeafData *)mProjectTree->GetItemData(id);
		MainDialog::GetInstance()->GetSelectedCanvas()->SetPrediction(item->GetLeafPrediction());
		MainDialog::GetInstance()->UpdateLeaves(true);
	}
}


void ProjectDialog::SaveProject(wxFileName file)
{
}


void ProjectDialog::LoadProject(wxFileName file)
{
}
