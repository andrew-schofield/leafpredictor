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
#pragma implementation "staticBoldedText.h"
#endif

#include "wx/wxprec.h"

#ifdef  __BORLANDC__
#pragma hdrstop
#endif

/**
 * \file staticBoldedText.cpp
 * Creates a bold text object.
 * \author Francois Ingelrest
 * \author Andrew Schofield
 **/

#include "leafpredictor.h"
#include "staticBoldedText.h"

#include "wx/font.h"


StaticBoldedText::StaticBoldedText(wxWindow* parent, wxWindowID id, const wxString& label, const wxPoint& pos, const wxSize& size, long style, const wxString& name) : wxStaticText(parent, id, label, pos, size, style, name)
{
	wxFont font;

	font = GetFont();
	font.SetWeight(wxFONTWEIGHT_BOLD);
	SetFont(font);
}
