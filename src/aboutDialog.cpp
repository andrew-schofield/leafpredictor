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
#pragma implementation "aboutDialog.h"
#endif

#include "wx/wxprec.h"

#ifdef  __BORLANDC__
#pragma hdrstop
#endif

#include "leafpredictor.h"
#include "aboutDialog.h"

#include "staticBoldedText.h"

#include "wx/sizer.h"
#include "wx/version.h"
#include "wx/statline.h"
#include "wx/hyperlink.h"
#include "wx/stattext.h"

AboutDialog* AboutDialog::mInstance = NULL;


AboutDialog::AboutDialog(wxWindow* parent) : wxDialog(parent, wxID_ANY, wxString::Format(_("About %s"), wxT(LEAFPREDICTOR_APPNAME)))
{
	wxBoxSizer      *topLevelSizer;
	wxBoxSizer      *infoSizer;
	wxFlexGridSizer *authorHomepageSizer;

	authorHomepageSizer = new wxFlexGridSizer(2);

	authorHomepageSizer->Add(new StaticBoldedText(this, wxID_ANY, _("Author: ")), 0, wxALIGN_RIGHT);
	authorHomepageSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Andrew Schofield")), 0, wxALIGN_LEFT);
	authorHomepageSizer->Add(new StaticBoldedText(this, wxID_ANY, _("Homepage: ")), 0, wxALIGN_RIGHT);
	authorHomepageSizer->Add(new wxHyperlinkCtrl(this, wxID_ANY, wxT(LEAFPREDICTOR_URL_HOMEPAGE), wxT(LEAFPREDICTOR_URL_HOMEPAGE)));
	authorHomepageSizer->Add(new StaticBoldedText(this, wxID_ANY, _("Licence: ")), 0, wxALIGN_RIGHT);
	authorHomepageSizer->Add(new wxHyperlinkCtrl(this, wxID_ANY, wxT("GNU GPL v2"), wxT(LEAFPREDICTOR_URL_LICENCE)));
	authorHomepageSizer->AddSpacer(5);
	authorHomepageSizer->AddSpacer(5);
	authorHomepageSizer->Add(new StaticBoldedText(this, wxID_ANY, _("Library: ")), 0, wxALIGN_RIGHT);
	authorHomepageSizer->Add(new wxHyperlinkCtrl(this, wxID_ANY, wxVERSION_STRING, wxT(LEAFPREDICTOR_URL_WXWIDGETS)));
	authorHomepageSizer->AddSpacer(5);


	infoSizer = new wxBoxSizer(wxVERTICAL);

	infoSizer->Add(new StaticBoldedText(this, wxID_ANY, wxT(LEAFPREDICTOR_APPNAME)), 0, wxALIGN_CENTER);
	infoSizer->AddSpacer(5);
	infoSizer->Add(new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxSize(65, -1)), 0, wxALIGN_CENTER);
	infoSizer->AddSpacer(5);
	infoSizer->Add(authorHomepageSizer);


	topLevelSizer = new wxBoxSizer(wxVERTICAL);

	topLevelSizer->Add(infoSizer, 1, wxEXPAND | wxALL, 5);
	SetSizer(topLevelSizer);
	topLevelSizer->Fit(this);
}


AboutDialog::~AboutDialog(void)
{
}


AboutDialog* AboutDialog::GetInstance(wxWindow* parent)
{
	if(mInstance == NULL)
	{
		mInstance = new AboutDialog(parent);
	}

	return mInstance;
}


void AboutDialog::DestroyInstance(void)
{
	if(mInstance != NULL)
	{
		mInstance->Destroy();
		mInstance = NULL;
	}
}


int AboutDialog::ShowModal(void)
{
	Center();

	return wxDialog::ShowModal();
}
