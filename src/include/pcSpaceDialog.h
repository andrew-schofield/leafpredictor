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

#ifndef _PCSPACEDIALOG_H
#define _PCSPACEDIALOG_H

#ifdef __GNUG__
#pragma interface "pcSpaceDialog.cpp"
#endif

#include "wx/dialog.h"

enum {
	ID_ENABLE_PC1 = 10000,
	ID_ENABLE_PC2 = 10001,
	ID_ENABLE_PC3 = 10002,
	ID_ENABLE_PC4 = 10003,
	ID_PC1 = 10004,
	ID_PC2 = 10005,
	ID_PC3 = 10006,
	ID_PC4 = 10007,
	ID_PC1_START = 10008,
	ID_PC2_START = 10009,
	ID_PC3_START = 10010,
	ID_PC4_START = 10011,
	ID_PC1_END = 10012,
	ID_PC2_END = 10013,
	ID_PC3_END = 10014,
	ID_PC4_END = 10015,
	ID_PC1_INT = 10016,
	ID_PC2_INT = 10017,
	ID_PC3_INT = 10018,
	ID_PC4_INT = 10019,
	ID_LOCATION = 10020,
	BTN_LOCATION = 10021
};


class PCSpaceDialog : public wxDialog
{
public:
	PCSpaceDialog();
	PCSpaceDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& caption = _("Generate Principal Component Space"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION|wxSYSTEM_MENU );

	bool Create( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& caption = _("Generate Principal Component Space"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION|wxSYSTEM_MENU );

	wxInt32 GetPC1(void){return mPC1;}
	wxInt32 GetPC2(void){return mPC2;}
	wxInt32 GetPC3(void){return mPC3;}
	wxInt32 GetPC4(void){return mPC4;}

	wxString GetPC1Start(void){return mPC1Start;}
	wxString GetPC2Start(void){return mPC2Start;}
	wxString GetPC3Start(void){return mPC3Start;}
	wxString GetPC4Start(void){return mPC4Start;}

	wxString GetPC1End(void){return mPC1End;}
	wxString GetPC2End(void){return mPC2End;}
	wxString GetPC3End(void){return mPC3End;}
	wxString GetPC4End(void){return mPC4End;}

	wxString GetPC1Int(void){return mPC1Int;}
	wxString GetPC2Int(void){return mPC2Int;}
	wxString GetPC3Int(void){return mPC3Int;}
	wxString GetPC4Int(void){return mPC4Int;}

	wxString GetLocation(void){return mLocation;}

	bool IsPC2Enabled(void){return mPC2Enabled;}
	bool IsPC3Enabled(void){return mPC3Enabled;}
	bool IsPC4Enabled(void){return mPC4Enabled;}

private:
	wxInt32    mPC1;
	wxInt32    mPC2;
	wxInt32    mPC3;
	wxInt32    mPC4;
	wxString     mPC1Start;
	wxString     mPC2Start;
	wxString     mPC3Start;
	wxString     mPC4Start;
	wxString     mPC1End;
	wxString     mPC2End;
	wxString     mPC3End;
	wxString     mPC4End;
	wxString     mPC1Int;
	wxString     mPC2Int;
	wxString     mPC3Int;
	wxString     mPC4Int;
	wxString     mLocation;
	bool        mPC1Enabled;
	bool        mPC2Enabled;
	bool        mPC3Enabled;
	bool        mPC4Enabled;

	void OnCheck(wxCommandEvent& event);

	void OnChoose(wxCommandEvent& event);

	void CreateControls();

	void SetDialogValidators();

	void Init();

	DECLARE_EVENT_TABLE()

};


#endif /* _PCSPACEDIALOG_H */
