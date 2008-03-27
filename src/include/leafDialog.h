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

#ifndef _LEAFDIALOG_H
#define _LEAFDIALOG_H

#ifdef __GNUG__
#pragma interface "leafDialog.cpp"
#endif

#include "leafCanvas.h"

#include "wx/frame.h"

enum {
	LEAF = 10001,
	NEW_LEAF_VIEWER
};


class LeafDialog : public wxFrame
{
public:
	LeafDialog();
	LeafDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& caption = _("Leaf Viewer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxMINIMIZE_BOX );


	bool Create( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& caption = _("Leaf Viewer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxMINIMIZE_BOX );


private:

	LeafCanvas*    mLeafCanvas;

	void OnClose(wxCloseEvent& event);
	void OnQuit(wxCommandEvent& event);
	void OnOpenMenu(wxCommandEvent& event);
	void OnNewMenu(wxCommandEvent& event);
	void CreateControls();
	void Init();

	void SetDialogValidators();

	void CreateMenuBar(void);

	DECLARE_EVENT_TABLE()

};


#endif /* _LEAFDIALOG_H */
