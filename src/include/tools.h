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

#ifndef _TOOLS_H
#define _TOOLS_H

#ifdef __GNUG__
#pragma interface "tools.cpp"
#endif

#include "leafpredictor.h"

#include "wx/msgdlg.h"

/**
 * A class of useful tools.
 * A set of static methods.
 **/
class Tools
{
protected:

	/**
	 * Wrapper around wxMessageBox.
	 * This function eliminates the need to specify the dialog title.
	 * @param msg Text to display in the messagebox.
	 * @param style Dialog style.
	 * @return A wxMessageDialog.
	 **/
	static int MsgBox(const wxString& msg, long style) {return wxMessageBox(msg, wxT(LEAFPREDICTOR_APPNAME), style);}


public:
	// Miscellaneous message boxes
	/**
	 * Create an error dialog box.
	 * This is a wrapper around MsgBox.
	 * @param error The error message to display.
	 **/
	static void ErrorMsgBox(const wxString& error) {MsgBox(error, wxICON_ERROR | wxOK);}

	/**
	 * Create an question dialog box.
	 * This is a wrapper around MsgBox.
	 * @param question The question to ask.
	 **/
	static bool QuestionMsgBox(const wxString& question) {return MsgBox(question, wxICON_QUESTION | wxYES_NO) == wxID_YES;}

	/**
	 * Create an information dialog box.
	 * This is a wrapper around MsgBox.
	 * @param information The information to display.
	 **/
	static void InfoMsgBox(const wxString& information) {MsgBox(information, wxICON_INFORMATION | wxOK);}
};


#endif /* _TOOLS_H */
