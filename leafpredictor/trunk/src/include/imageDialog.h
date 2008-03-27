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

#ifndef _IMAGEDIALOG_H
#define _IMAGEDIALOG_H

#ifdef __GNUG__
#pragma interface "imageDialog.cpp"
#endif

#include "wx/dialog.h"

enum {
	ID_IMAGE_EXPORT = 10000,
	ID_XPX = 10001,
	ID_YPX = 10002,
	ID_PUBLISH = 10006,
	ID_THICKNESS = 10007,
	ID_ANTIALIASE = 10008
};


class ImageDialog : public wxDialog
{
public:
	ImageDialog();
	ImageDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& caption = _("Export image options"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION|wxSYSTEM_MENU );

	bool Create( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& caption = _("Export image options"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION|wxSYSTEM_MENU );

	void SetSavePublishableImage(const bool publishable) {mSavePublishableImage = publishable;}
	bool GetPublishableImage(void) const {return mSavePublishableImage;}

	void SetXSize(const wxString x) {mXSize = x;}
	wxString GetXSize(void) const {return mXSize;}

	void SetYSize(const wxString y) {mYSize = y;}
	wxString GetYSize(void) const {return mYSize;}

	void SetThickness(const wxString thickness) {mLineThickness = thickness;}
	wxString GetThickness(void) const {return mLineThickness;}

	void SetAntialiase(const bool antialiase) {mAntialiase = antialiase;}
	bool GetAntialiase(void) const {return mAntialiase;}

private:

	bool mSavePublishableImage;
	bool mAntialiase;
	wxString mXSize;
	wxString mYSize;
	wxString mLineThickness;

	void CreateControls();

	void SetDialogValidators();

	void OnThicknessUpdate( wxUpdateUIEvent& event );
	void OnXPxUpdate( wxUpdateUIEvent& event );
	void OnYPxUpdate( wxUpdateUIEvent& event );
	void OnAntialiaseUpdate( wxUpdateUIEvent& event );

	void Init();


	DECLARE_EVENT_TABLE()

};


#endif /* _IMAGEDIALOG_H */
