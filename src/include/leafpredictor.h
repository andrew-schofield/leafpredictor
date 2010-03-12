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

#ifndef _LEAFPREDICTOR_H
#define _LEAFPREDICTOR_H


#include <wx/defs.h>

#ifdef __WXMSW__
#include "resource.h"
#endif

#define LEAFPREDICTOR_APPNAME 	"LeafPredictor 0.8.9"
#define LEAFPREDICTOR_VERSION   "0.8.9"
#define LEAFPREDICTOR_URL_HOMEPAGE	"http://fire-salamander.co.uk/leafpredictor/"
#define LEAFPREDICTOR_URL_WXWIDGETS	"http://www.wxwidgets.org/"
#define LEAFPREDICTOR_URL_LICENCE	"http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt"

#ifdef __WXGTK__
#define LEAFPREDICTOR_GUI_BORDER 2     // GTK already has some borders
#elif __WXMSW__
#define LEAFPREDICTOR_GUI_BORDER 5     // Windows does not have default borders, so we use ours
#elif __WXMAC__
#define LEAFPREDICTOR_GUI_BORDER 5
#endif

#define LEAFPREDICTOR_GUI_SPACING_LOW          5       // (Low) Spacing used bewteen controls
#define LEAFPREDICTOR_GUI_SPACING_HIGH         10      // (High) Spacing used bewteen controls

#endif /* _LEAFPREDICTOR_H */
