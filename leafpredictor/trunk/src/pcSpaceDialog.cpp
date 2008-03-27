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
#pragma implementation "pcSpaceDialog.h"
#endif

#include "wx/wxprec.h"

#ifdef  __BORLANDC__
#pragma hdrstop
#endif

#include "leafpredictor.h"
#include "pcSpaceDialog.h"

#include "tools.h"

#include "wx/sizer.h"
#include "wx/statline.h"
#include "wx/stattext.h"
#include "wx/checkbox.h"
#include "wx/textctrl.h"
#include "wx/choice.h"
#include "wx/button.h"
#include "wx/valtext.h"
#include "wx/valgen.h"
#include "wx/dirdlg.h"

BEGIN_EVENT_TABLE(PCSpaceDialog, wxDialog)
		EVT_CHECKBOX(ID_ENABLE_PC2,    PCSpaceDialog::OnCheck)
		EVT_CHECKBOX(ID_ENABLE_PC3,    PCSpaceDialog::OnCheck)
		EVT_CHECKBOX(ID_ENABLE_PC4,    PCSpaceDialog::OnCheck)
		EVT_BUTTON  (BTN_LOCATION,     PCSpaceDialog::OnChoose)
END_EVENT_TABLE()

PCSpaceDialog::PCSpaceDialog()
{
	Init();
}


PCSpaceDialog::PCSpaceDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
	Init();

	Create(parent, id, caption, pos, size, style);

	mPC2Enabled = false;
	mPC3Enabled = false;
	mPC4Enabled = false;
	mPC1 = 0;
	mPC2 = 0;
	mPC3 = 0;
	mPC4 = 0;
	mPC1Start = wxT("0");
	mPC2Start = wxT("0");
	mPC3Start = wxT("0");
	mPC4Start = wxT("0");
	mPC1End = wxT("0");
	mPC2End = wxT("0");
	mPC3End = wxT("0");
	mPC4End = wxT("0");
	mPC1Int = wxT("0");
	mPC2Int = wxT("0");
	mPC3Int = wxT("0");
	mPC4Int = wxT("0");
}


void PCSpaceDialog::Init()
{
}


bool PCSpaceDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
	SetExtraStyle(wxWS_EX_BLOCK_EVENTS);

	if (!wxDialog::Create( parent, id, caption, pos, size, style))
		return false;

	CreateControls();
	SetDialogValidators();

	GetSizer()->Fit(this);

	GetSizer()->SetSizeHints(this);

	Centre();

	return true;
}



void PCSpaceDialog::CreateControls()
{
	wxString PCs[96];

	for(wxInt32 i=0;i<96;++i)
	{
		PCs[i] = wxString::Format(_T("PC %i"), i+1);
	}

	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(topSizer);

	wxBoxSizer* defaultLabSizer = new wxBoxSizer(wxVERTICAL);
	topSizer->Add(defaultLabSizer, 0, wxALIGN_CENTER_VERTICAL|wxALL, LEAFPREDICTOR_GUI_BORDER);

	wxGridSizer* pcConfigSizer = new wxGridSizer(2);
	topSizer->Add(pcConfigSizer, 0, wxEXPAND|wxALL, LEAFPREDICTOR_GUI_BORDER);

	wxStaticBoxSizer* pc1Sizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString::Format(_("Principal Component %i"), 1));
	pcConfigSizer->Add(pc1Sizer, 1, wxEXPAND|wxALL, LEAFPREDICTOR_GUI_BORDER);
	wxStaticBoxSizer* pc2Sizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString::Format(_("Principal Component %i"), 2));
	pcConfigSizer->Add(pc2Sizer, 1, wxEXPAND|wxALL, LEAFPREDICTOR_GUI_BORDER);
	wxStaticBoxSizer* pc3Sizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString::Format(_("Principal Component %i"), 3));
	pcConfigSizer->Add(pc3Sizer, 1, wxEXPAND|wxALL, LEAFPREDICTOR_GUI_BORDER);
	wxStaticBoxSizer* pc4Sizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString::Format(_("Principal Component %i"), 4));
	pcConfigSizer->Add(pc4Sizer, 1, wxEXPAND|wxALL, LEAFPREDICTOR_GUI_BORDER);

	wxStaticText* defaultLabel = new wxStaticText( this, wxID_STATIC, _("Configure the PC space you want to create"), wxDefaultPosition, wxDefaultSize, 0);
	defaultLabSizer->Add(defaultLabel, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, LEAFPREDICTOR_GUI_BORDER);

	wxCheckBox* enablePC1Check = new wxCheckBox( this, ID_ENABLE_PC1, _("Enable"), wxDefaultPosition, wxDefaultSize, 0 );
	pc1Sizer->Add(enablePC1Check, 0, wxALIGN_LEFT);
	enablePC1Check->SetValue(true);
	enablePC1Check->Enable(false);
	wxGridSizer* pc1SettingsSizer = new wxGridSizer(2);
	pc1Sizer->Add(pc1SettingsSizer, 0, wxLEFT|wxALL, LEAFPREDICTOR_GUI_BORDER);
	wxStaticText* pc1Label = new wxStaticText( this, wxID_STATIC, _("Select PC:"), wxDefaultPosition, wxDefaultSize, 0);
	pc1SettingsSizer->Add(pc1Label, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL);
	wxChoice* pc1 = new wxChoice( this, ID_PC1, wxDefaultPosition, wxDefaultSize, 96, PCs);
	pc1SettingsSizer->Add(pc1, 0, wxALIGN_LEFT);
	wxStaticText* pc1StartLabel = new wxStaticText( this, wxID_STATIC, _("Starting SD:"), wxDefaultPosition, wxDefaultSize, 0);
	pc1SettingsSizer->Add(pc1StartLabel, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL);
	wxTextCtrl* pc1StartValue = new wxTextCtrl(this, ID_PC1_START, wxT("0"), wxDefaultPosition, wxDefaultSize);
	pc1SettingsSizer->Add(pc1StartValue, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL);
	wxStaticText* pc1EndLabel = new wxStaticText( this, wxID_STATIC, _("Ending SD:"), wxDefaultPosition, wxDefaultSize, 0);
	pc1SettingsSizer->Add(pc1EndLabel, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL);
	wxTextCtrl* pc1EndValue = new wxTextCtrl(this, ID_PC1_END, wxT("0"), wxDefaultPosition, wxDefaultSize);
	pc1SettingsSizer->Add(pc1EndValue, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL);
	wxStaticText* pc1IntLabel = new wxStaticText( this, wxID_STATIC, _("SD interval:"), wxDefaultPosition, wxDefaultSize, 0);
	pc1SettingsSizer->Add(pc1IntLabel, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL);
	wxTextCtrl* pc1IntValue = new wxTextCtrl(this, ID_PC1_INT, wxT("0"), wxDefaultPosition, wxDefaultSize);
	pc1SettingsSizer->Add(pc1IntValue, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL);

	wxCheckBox* enablePC2Check = new wxCheckBox( this, ID_ENABLE_PC2, _("Enable"), wxDefaultPosition, wxDefaultSize, 0 );
	pc2Sizer->Add(enablePC2Check, 0, wxALIGN_LEFT);
	wxGridSizer* pc2SettingsSizer = new wxGridSizer(2);
	pc2Sizer->Add(pc2SettingsSizer, 0, wxLEFT|wxALL, LEAFPREDICTOR_GUI_BORDER);
	wxStaticText* pc2Label = new wxStaticText( this, wxID_STATIC, _("Select PC:"), wxDefaultPosition, wxDefaultSize, 0);
	pc2SettingsSizer->Add(pc2Label, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL);
	wxChoice* pc2 = new wxChoice( this, ID_PC2, wxDefaultPosition, wxDefaultSize, 96, PCs);
	pc2SettingsSizer->Add(pc2, 0, wxALIGN_LEFT);
	wxStaticText* pc2StartLabel = new wxStaticText( this, wxID_STATIC, _("Starting SD:"), wxDefaultPosition, wxDefaultSize, 0);
	pc2SettingsSizer->Add(pc2StartLabel, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL);
	wxTextCtrl* pc2StartValue = new wxTextCtrl(this, ID_PC2_START, wxT("0"), wxDefaultPosition, wxDefaultSize);
	pc2SettingsSizer->Add(pc2StartValue, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL);
	wxStaticText* pc2EndLabel = new wxStaticText( this, wxID_STATIC, _("Ending SD:"), wxDefaultPosition, wxDefaultSize, 0);
	pc2SettingsSizer->Add(pc2EndLabel, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL);
	wxTextCtrl* pc2EndValue = new wxTextCtrl(this, ID_PC2_END, wxT("0"), wxDefaultPosition, wxDefaultSize);
	pc2SettingsSizer->Add(pc2EndValue, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL);
	wxStaticText* pc2IntLabel = new wxStaticText( this, wxID_STATIC, _("SD interval:"), wxDefaultPosition, wxDefaultSize, 0);
	pc2SettingsSizer->Add(pc2IntLabel, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL);
	wxTextCtrl* pc2IntValue = new wxTextCtrl(this, ID_PC2_INT, wxT("0"), wxDefaultPosition, wxDefaultSize);
	pc2SettingsSizer->Add(pc2IntValue, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL);
	pc2->Enable(false);
	pc2StartValue->Enable(false);
	pc2EndValue->Enable(false);
	pc2IntValue->Enable(false);

	wxCheckBox* enablePC3Check = new wxCheckBox( this, ID_ENABLE_PC3, _("Enable"), wxDefaultPosition, wxDefaultSize, 0 );
	pc3Sizer->Add(enablePC3Check, 0, wxALIGN_LEFT);
	wxGridSizer* pc3SettingsSizer = new wxGridSizer(2);
	pc3Sizer->Add(pc3SettingsSizer, 0, wxLEFT|wxALL, LEAFPREDICTOR_GUI_BORDER);
	wxStaticText* pc3Label = new wxStaticText( this, wxID_STATIC, _("Select PC:"), wxDefaultPosition, wxDefaultSize, 0);
	pc3SettingsSizer->Add(pc3Label, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL);
	wxChoice* pc3 = new wxChoice( this, ID_PC3, wxDefaultPosition, wxDefaultSize, 96, PCs);
	pc3SettingsSizer->Add(pc3, 0, wxALIGN_LEFT);
	wxStaticText* pc3StartLabel = new wxStaticText( this, wxID_STATIC, _("Starting SD:"), wxDefaultPosition, wxDefaultSize, 0);
	pc3SettingsSizer->Add(pc3StartLabel, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL);
	wxTextCtrl* pc3StartValue = new wxTextCtrl(this, ID_PC3_START, wxT("0"), wxDefaultPosition, wxDefaultSize);
	pc3SettingsSizer->Add(pc3StartValue, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL);
	wxStaticText* pc3EndLabel = new wxStaticText( this, wxID_STATIC, _("Ending SD:"), wxDefaultPosition, wxDefaultSize, 0);
	pc3SettingsSizer->Add(pc3EndLabel, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL);
	wxTextCtrl* pc3EndValue = new wxTextCtrl(this, ID_PC3_END, wxT("0"), wxDefaultPosition, wxDefaultSize);
	pc3SettingsSizer->Add(pc3EndValue, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL);
	wxStaticText* pc3IntLabel = new wxStaticText( this, wxID_STATIC, _("SD interval:"), wxDefaultPosition, wxDefaultSize, 0);
	pc3SettingsSizer->Add(pc3IntLabel, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL);
	wxTextCtrl* pc3IntValue = new wxTextCtrl(this, ID_PC3_INT, wxT("0"), wxDefaultPosition, wxDefaultSize);
	pc3SettingsSizer->Add(pc3IntValue, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL);
	pc3->Enable(false);
	pc3StartValue->Enable(false);
	pc3EndValue->Enable(false);
	pc3IntValue->Enable(false);

	wxCheckBox* enablePC4Check = new wxCheckBox( this, ID_ENABLE_PC4, _("Enable"), wxDefaultPosition, wxDefaultSize, 0 );
	pc4Sizer->Add(enablePC4Check, 0, wxALIGN_LEFT);
	wxGridSizer* pc4SettingsSizer = new wxGridSizer(2);
	pc4Sizer->Add(pc4SettingsSizer, 0, wxLEFT|wxALL, LEAFPREDICTOR_GUI_BORDER);
	wxStaticText* pc4Label = new wxStaticText( this, wxID_STATIC, _("Select PC:"), wxDefaultPosition, wxDefaultSize, 0);
	pc4SettingsSizer->Add(pc4Label, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL);
	wxChoice* pc4 = new wxChoice( this, ID_PC4, wxDefaultPosition, wxDefaultSize, 96, PCs);
	pc4SettingsSizer->Add(pc4, 0, wxALIGN_LEFT);
	wxStaticText* pc4StartLabel = new wxStaticText( this, wxID_STATIC, _("Starting SD:"), wxDefaultPosition, wxDefaultSize, 0);
	pc4SettingsSizer->Add(pc4StartLabel, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL);
	wxTextCtrl* pc4StartValue = new wxTextCtrl(this, ID_PC4_START, wxT("0"), wxDefaultPosition, wxDefaultSize);
	pc4SettingsSizer->Add(pc4StartValue, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL);
	wxStaticText* pc4EndLabel = new wxStaticText( this, wxID_STATIC, _("Ending SD:"), wxDefaultPosition, wxDefaultSize, 0);
	pc4SettingsSizer->Add(pc4EndLabel, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL);
	wxTextCtrl* pc4EndValue = new wxTextCtrl(this, ID_PC4_END, wxT("0"), wxDefaultPosition, wxDefaultSize);
	pc4SettingsSizer->Add(pc4EndValue, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL);
	wxStaticText* pc4IntLabel = new wxStaticText( this, wxID_STATIC, _("SD interval:"), wxDefaultPosition, wxDefaultSize, 0);
	pc4SettingsSizer->Add(pc4IntLabel, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL);
	wxTextCtrl* pc4IntValue = new wxTextCtrl(this, ID_PC4_INT, wxT("0"), wxDefaultPosition, wxDefaultSize);
	pc4SettingsSizer->Add(pc4IntValue, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL);
	pc4->Enable(false);
	pc4StartValue->Enable(false);
	pc4EndValue->Enable(false);
	pc4IntValue->Enable(false);

	wxBoxSizer* locationSizer = new wxBoxSizer(wxHORIZONTAL);
	topSizer->Add(locationSizer, 0, wxEXPAND|wxALIGN_CENTER_VERTICAL|wxALL, LEAFPREDICTOR_GUI_BORDER);

	wxStaticText* locationLabel = new wxStaticText( this, wxID_STATIC, _("Output folder:"), wxDefaultPosition, wxDefaultSize, 0);
	locationSizer->Add(locationLabel, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, LEAFPREDICTOR_GUI_BORDER);

	wxTextCtrl* locationBox = new wxTextCtrl( this, ID_LOCATION, _T(""), wxDefaultPosition);
	locationSizer->Add(locationBox, 1, wxEXPAND);

	wxButton* locationChooser = new wxButton(this, BTN_LOCATION, wxT("..."), wxDefaultPosition, wxSize(26, 26));
	locationSizer->Add(locationChooser, 0, wxALIGN_RIGHT);


	wxStaticLine* line = new wxStaticLine( this, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	topSizer->Add(line, 0, wxEXPAND|wxALL, LEAFPREDICTOR_GUI_BORDER);

	wxBoxSizer* okBox = new wxBoxSizer(wxHORIZONTAL);
	topSizer->Add(okBox, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, LEAFPREDICTOR_GUI_BORDER);

	wxButton* ok = new wxButton ( this, wxID_OK, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0);
	okBox->Add( ok, 0, wxALIGN_CENTER_VERTICAL);

	wxButton* cancel = new wxButton ( this, wxID_CANCEL, wxT("&Cancel"), wxDefaultPosition, wxDefaultSize, 0);
	okBox->Add( cancel, 0, wxALIGN_CENTER_VERTICAL);

}


void PCSpaceDialog::SetDialogValidators()
{
	FindWindow(ID_ENABLE_PC1)->SetValidator(wxGenericValidator(& mPC1Enabled));
	FindWindow(ID_PC1)->SetValidator(wxGenericValidator(& mPC1));
	FindWindow(ID_PC1_START)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, & mPC1Start));
	FindWindow(ID_PC1_END)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, & mPC1End));
	FindWindow(ID_PC1_INT)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, & mPC1Int));

	FindWindow(ID_ENABLE_PC2)->SetValidator(wxGenericValidator(& mPC2Enabled));
	FindWindow(ID_PC2)->SetValidator(wxGenericValidator(& mPC2));
	FindWindow(ID_PC2_START)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, & mPC2Start));
	FindWindow(ID_PC2_END)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, & mPC2End));
	FindWindow(ID_PC2_INT)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, & mPC2Int));

	FindWindow(ID_ENABLE_PC3)->SetValidator(wxGenericValidator(& mPC3Enabled));
	FindWindow(ID_PC3)->SetValidator(wxGenericValidator(& mPC3));
	FindWindow(ID_PC3_START)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, & mPC3Start));
	FindWindow(ID_PC3_END)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, & mPC3End));
	FindWindow(ID_PC3_INT)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, & mPC3Int));

	FindWindow(ID_ENABLE_PC4)->SetValidator(wxGenericValidator(& mPC4Enabled));
	FindWindow(ID_PC4)->SetValidator(wxGenericValidator(& mPC4));
	FindWindow(ID_PC4_START)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, & mPC4Start));
	FindWindow(ID_PC4_END)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, & mPC4End));
	FindWindow(ID_PC4_INT)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, & mPC4Int));

	FindWindow(ID_LOCATION)->SetValidator(wxGenericValidator(& mLocation));
}


void PCSpaceDialog::OnCheck(wxCommandEvent& event)
{
	wxCheckBox* pc;
	switch(event.GetId())
	{
		case ID_ENABLE_PC2:
			pc = (wxCheckBox*) FindWindow(ID_ENABLE_PC2);
			FindWindow(ID_PC2)->Enable(pc->GetValue());
			FindWindow(ID_PC2_START)->Enable(pc->GetValue());
			FindWindow(ID_PC2_END)->Enable(pc->GetValue());
			FindWindow(ID_PC2_INT)->Enable(pc->GetValue());
			break;
		case ID_ENABLE_PC3:
			pc = (wxCheckBox*) FindWindow(ID_ENABLE_PC3);
			FindWindow(ID_PC3)->Enable(pc->GetValue());
			FindWindow(ID_PC3_START)->Enable(pc->GetValue());
			FindWindow(ID_PC3_END)->Enable(pc->GetValue());
			FindWindow(ID_PC3_INT)->Enable(pc->GetValue());
			break;
		case ID_ENABLE_PC4:
			pc = (wxCheckBox*) FindWindow(ID_ENABLE_PC4);
			FindWindow(ID_PC4)->Enable(pc->GetValue());
			FindWindow(ID_PC4_START)->Enable(pc->GetValue());
			FindWindow(ID_PC4_END)->Enable(pc->GetValue());
			FindWindow(ID_PC4_INT)->Enable(pc->GetValue());
			break;
		default:
			break;
	}
}


void PCSpaceDialog::OnChoose(wxCommandEvent& event)
{
	wxString selectedDir;

	wxDirDialog *DirDialog = new wxDirDialog(this);
	if(DirDialog->ShowModal() == wxID_OK)
	{
		selectedDir = DirDialog->GetPath();
		wxTextCtrl* location = (wxTextCtrl*) FindWindow(ID_LOCATION);
		location->SetValue(selectedDir);
	}
}
