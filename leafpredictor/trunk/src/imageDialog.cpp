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
#pragma implementation "imageDialog.h"
#endif

#include "wx/wxprec.h"

#ifdef  __BORLANDC__
#pragma hdrstop
#endif

#include "leafpredictor.h"
#include "imageDialog.h"

#include "wx/sizer.h"
//#include "wx/intl.h"
#include "wx/version.h"
#include "wx/statline.h"
#include "wx/hyperlink.h"
#include "wx/stattext.h"
#include "wx/font.h"
#include "wx/checkbox.h"
#include "wx/textctrl.h"
#include "wx/choice.h"
#include "wx/button.h"
#include "wx/valtext.h"
#include "wx/valgen.h"

BEGIN_EVENT_TABLE(ImageDialog, wxDialog)
		EVT_UPDATE_UI( ID_XPX, ImageDialog::OnXPxUpdate)
		EVT_UPDATE_UI( ID_YPX, ImageDialog::OnYPxUpdate)
		EVT_UPDATE_UI( ID_THICKNESS, ImageDialog::OnThicknessUpdate)
		EVT_UPDATE_UI( ID_ANTIALIASE, ImageDialog::OnAntialiaseUpdate)
END_EVENT_TABLE()

ImageDialog::ImageDialog()
{
	Init();
}


ImageDialog::ImageDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
	Init();

	Create(parent, id, caption, pos, size, style);
}


void ImageDialog::Init()
{
	mSavePublishableImage = false;
	mXSize = _T("1600");
	mYSize = _T("1200");
	mLineThickness = _T("3");
	mAntialiase = false;
}


bool ImageDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
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



void ImageDialog::CreateControls()
{
	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(topSizer);

	wxBoxSizer* defaultLabSizer = new wxBoxSizer(wxVERTICAL);
	topSizer->Add(defaultLabSizer, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* boxSizer = new wxBoxSizer(wxVERTICAL);
	topSizer->Add(boxSizer, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxStaticText* defaultLabel = new wxStaticText( this, wxID_STATIC, _("Leave default values if you want to save directly from the plot window"), wxDefaultPosition, wxDefaultSize, 0);
	defaultLabSizer->Add(defaultLabel, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxCheckBox* exportCheck = new wxCheckBox( this, ID_PUBLISH, _("Produce publishable quality image"), wxDefaultPosition, wxDefaultSize, 0 );
	boxSizer->Add(exportCheck, 0, wxALIGN_LEFT|wxALL, 5);

	wxBoxSizer* imagePXSizer = new wxBoxSizer(wxHORIZONTAL);
	topSizer->Add(imagePXSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxStaticText* pxWidthLabel = new wxStaticText( this, wxID_STATIC, _("Image width (px)"), wxDefaultPosition, wxDefaultSize, 0 );
	wxTextCtrl* pxWidth = new wxTextCtrl( this, ID_XPX, _("1600"), wxDefaultPosition, wxDefaultSize, 0);
	imagePXSizer->Add(pxWidthLabel, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	imagePXSizer->Add(pxWidth, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxStaticText* pxHeightLabel = new wxStaticText( this, wxID_STATIC, _("Image height (px)"), wxDefaultPosition, wxDefaultSize, 0 );
	wxTextCtrl* pxHeight = new wxTextCtrl( this, ID_YPX, _("1200"), wxDefaultPosition, wxDefaultSize, 0);
	imagePXSizer->Add(pxHeightLabel, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	imagePXSizer->Add(pxHeight, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* imageDPISizer = new wxBoxSizer(wxHORIZONTAL);
	topSizer->Add(imageDPISizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxStaticText* thicknessLabel = new wxStaticText( this, wxID_STATIC, _("Leaf outline thickness (px)"), wxDefaultPosition, wxDefaultSize, 0 );
	wxTextCtrl* thickness = new wxTextCtrl( this, ID_THICKNESS, _("5"), wxDefaultPosition, wxDefaultSize, 0);

	wxCheckBox* antialiaseCheck = new wxCheckBox( this, ID_ANTIALIASE, _("Antialiase image (slow)"), wxDefaultPosition, wxDefaultSize, 0);

	imageDPISizer->Add(thicknessLabel, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	imageDPISizer->Add(thickness, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	imageDPISizer->Add(antialiaseCheck, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxStaticLine* line = new wxStaticLine( this, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	topSizer->Add(line, 0, wxEXPAND|wxALL, 5);

	wxBoxSizer* okBox = new wxBoxSizer(wxHORIZONTAL);
	topSizer->Add(okBox, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxButton* ok = new wxButton ( this, wxID_OK, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0);
	okBox->Add( ok, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	pxWidth->Enable(false);
	pxHeight->Enable(false);
	thickness->Enable(false);
	antialiaseCheck->Enable(false);

}


void ImageDialog::SetDialogValidators()
{
	FindWindow(ID_XPX)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, & mXSize));
	FindWindow(ID_YPX)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, & mYSize));
	FindWindow(ID_THICKNESS)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, & mLineThickness));
	FindWindow(ID_PUBLISH)->SetValidator(wxGenericValidator(& mSavePublishableImage));
	FindWindow(ID_ANTIALIASE)->SetValidator(wxGenericValidator(& mAntialiase));
}


void ImageDialog::OnThicknessUpdate( wxUpdateUIEvent& event )
{
	wxCheckBox* publishable = (wxCheckBox*) FindWindow(ID_PUBLISH);
	if (publishable->GetValue() == true)
		event.Enable(true);
	else
		event.Enable(false);
}

void ImageDialog::OnXPxUpdate( wxUpdateUIEvent& event )
{
	wxCheckBox* publishable = (wxCheckBox*) FindWindow(ID_PUBLISH);
	if (publishable->GetValue() == true)
		event.Enable(true);
	else
		event.Enable(false);
}

void ImageDialog::OnYPxUpdate( wxUpdateUIEvent& event )
{
	wxCheckBox* publishable = (wxCheckBox*) FindWindow(ID_PUBLISH);
	if (publishable->GetValue() == true)
		event.Enable(true);
	else
		event.Enable(false);
}

void ImageDialog::OnAntialiaseUpdate( wxUpdateUIEvent& event )
{
	wxCheckBox* publishable = (wxCheckBox*) FindWindow(ID_PUBLISH);
	if (publishable->GetValue() == true)
		event.Enable(true);
	else
		event.Enable(false);
}
