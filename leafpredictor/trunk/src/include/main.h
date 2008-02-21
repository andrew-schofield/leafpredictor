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

#ifndef _MAIN_H
#define _MAIN_H


#include <wx/wx.h>

/**
* This is the main application class, it contains the entry point.
**/
class LeafPredictorApp : public wxApp
{
protected:
	wxLocale                 m_locale; /**< The locale of the system */

public:
	/**
	 * This is the entry point of the application.
	 **/
	virtual bool OnInit(void);

	/**
	 * This is called just before the application exits.
	 **/
	virtual int OnExit(void);

private:
DECLARE_EVENT_TABLE()
	/**
	 * Event: Catches shutdown/logoff events.
	 * This event allows LeafPredictor to save settings and exit correctly when the system is logged off or shutdown.
	 * For some reason it doesn't work on Linux.
	 * @param event The actual event. This gets sent automatically.
	 **/
	void OnEndSession( wxCloseEvent& event );

	/**
	 * Event: Catches shutdown/logoff events.
	 * This event allows LeafPredictor to save settings and exit correctly when the system is logged off or shutdown.
	 * For some reason it doesn't work on Linux.
	 * @param event The actual event. This gets sent automatically.
	 **/
	void OnQueryEndSession( wxCloseEvent& event );

	/**
	 * Event: Catches close events.
	 * This event allows LeafPredictor to save settings and exit correctly when it is closed.
	 * @param event The actual event. This gets sent automatically.
	 **/
	void OnClose( wxCloseEvent& event);

};

#endif /* _MAIN_H */
