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

#ifndef _LEAFCANVAS_H
#define _LEAFCANVAS_H

#include "wx/dcbuffer.h"
#include "wx/scrolwin.h"

#include <vector>

class LeafCanvas: public wxScrolledWindow
{
protected:
	std::vector<double>  mLeaf;
	double               mXRange, mYRange, mXMid, mYMid, mScale;
	wxString             mLabel;

	void CalculateScale(void);

	void DrawLeaf(wxDC& dc);

	void OnPaint(wxPaintEvent& event);

	void OnResize(wxSizeEvent& event);

	void PaintBackground(wxDC& dc);


public:
	LeafCanvas(wxWindow* parent, wxWindowID, const wxPoint& pos, const wxSize& size);
	virtual ~LeafCanvas();

	void SetLeaf(std::vector<double> coords);

	void SetScale(double scale) {mScale = scale;}
	double GetScale(void) {return mScale;}
	void SetLabel(wxString label) {mLabel = label;}

	//void ExtDraw(void);

private:
	DECLARE_EVENT_TABLE()
};

#endif /* _LEAFCANVAS_H */
