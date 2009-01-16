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

#ifndef _PROJECTDIALOG_H
#define _PROJECTDIALOG_H

#ifdef __GNUG__
#pragma interface "projectDialog.cpp"
#endif

#include "eigenSystem.h"

#include "wx/frame.h"
#include "wx/treectrl.h"
#include "wx/menu.h"


class ProjectDialog : public wxFrame
{
protected:
	static ProjectDialog *mInstance;

	ProjectDialog(void);

	~ProjectDialog(void);

	wxTreeCtrl*      mProjectTree;
	wxTreeItemId     mProjectRoot;
	wxTreeItemId     mEigenSystemTree;
	wxTreeItemId     mPredictionTree;
	wxTreeItemId     mExternalLeafTree;


public:
	// Singleton pattern
	static ProjectDialog* GetInstance(void);
	static void CreateInstance(void);

	static void DestroyInstance(void);

	int Show(void);

	void OnClose(wxCloseEvent& event);

	void OnItemRClick(wxTreeEvent& event);

	void ShowMenu(wxTreeItemId id, const wxPoint& pt);

	void OnAddESMenu(wxCommandEvent& event);
	void OnAddPLMenu(wxCommandEvent& event);
	void OnAddILMenu(wxCommandEvent& event);
	void OnDelItemMenu(wxCommandEvent& event);
	void OnUseItemMenu(wxCommandEvent& event);

	DECLARE_EVENT_TABLE()
};

class EigenSystemData : public wxTreeItemData
{
	public:
		EigenSystemData(const EigenSystem eigenSystem) : mEigenSystem(eigenSystem) { }

		EigenSystem GetEigenSystem(void) {return mEigenSystem;}

	private:
		EigenSystem mEigenSystem;
};

class ImportedLeafData : public wxTreeItemData
{
	public:
		ImportedLeafData(const std::vector<double> importedLeaf) : mLeaf(importedLeaf) { }

		std::vector<double> GetLeaf(void) {return mLeaf;}

	private:
		std::vector<double>  mLeaf;
};


#endif /* _PROJECTDIALOG_H */
