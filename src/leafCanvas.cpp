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

#include "leafpredictor.h"
#include "leafCanvas.h"

#include "tools.h"

BEGIN_EVENT_TABLE(LeafCanvas, wxScrolledWindow)
		EVT_PAINT(LeafCanvas::OnPaint)
		EVT_SIZE (LeafCanvas::OnResize)
END_EVENT_TABLE()

LeafCanvas::LeafCanvas(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize &size) : wxScrolledWindow(parent, wxID_ANY, pos, size, wxSUNKEN_BORDER)
{
	SetBackgroundColour(*wxWHITE);
}


LeafCanvas::~LeafCanvas()
{
}


void LeafCanvas::SetLeaf(std::vector<double> coords)
{
	mLeaf = coords;


	double   minX = 0, minY = 0, maxX = 0, maxY = 0;
	wxUint32   i;

	for(i=0;i<mLeaf.size()/2;++i)
	{
		double x = mLeaf.at(i*2);
		double y = mLeaf.at((i*2)+1);

		if(x<minX)
			minX = x;
		if(x>maxX)
			maxX = x;
		if(y<minY)
			minY = y;
		if(y>maxY)
			maxY = y;
	}
	mXRange = maxX - minX;
	mYRange = maxY - minY;

	mXMid = minX + (mXRange / 2);
	mYMid = minY + (mYRange / 2);

	//Tools::InfoMsgBox(wxString::Format(wxT("%.2f, %.2f"), mXMid, mYMid));

	Refresh();
}


void LeafCanvas::OnPaint(wxPaintEvent& event)
{

	wxBufferedPaintDC dc(this);

	DoPrepareDC(dc);

	PaintBackground(dc);

	DrawLeaf(dc);

}


void LeafCanvas::PaintBackground(wxDC& dc)
{

	//Tools::InfoMsgBox(wxString::Format(wxT("Painting background")));
	wxColour backgroundColour = *wxWHITE;


	dc.SetBrush(wxBrush(backgroundColour));
	//dc.SetPen(wxPen(backgroundColour, 1));

	//wxRect windowRect(wxPoint(0,0), GetClientSize());

	//CalcUnscrolledPosition(windowRect.x, windowRect.y, & windowRect.x, & windowRect.y);

	//dc.DrawRectangle(windowRect);
	dc.Clear();
}


void LeafCanvas::DrawLeaf(wxDC& dc)
{
	wxColour leafColour = *wxGREEN;
	wxUint32 i;
	double   xScale, yScale, scale;


	xScale = GetClientSize().x / mXRange;
	yScale = GetClientSize().y / mYRange;

	if(xScale<yScale)
		scale = xScale;
	else
		scale = yScale;

	//dc.SetUserScale(2,2);

	dc.SetBrush(wxBrush(leafColour));
	dc.SetPen(wxPen(*wxLIGHT_GREY, 1));
	//dc.DrawLine(0,GetClientSize().y/2,GetClientSize().x,GetClientSize().y/2);
	//dc.DrawLine(GetClientSize().x/2,0,GetClientSize().x/2,GetClientSize().y);

	dc.SetPen(wxPen(leafColour, 1));

	wxPoint leaf[mLeaf.size()/2];
	//scaledLeaf = ScaleLeaf();
	for(i=0;i<mLeaf.size()/2;++i)
	{
		int x = (int)(mLeaf.at(i*2) * scale + (GetClientSize().x/2 - mXMid * scale));
		int y = (int)(mLeaf.at((i*2)+1) * scale + (GetClientSize().y/2 - mYMid * scale));
		x = dc.LogicalToDeviceX(x);
		y = dc.LogicalToDeviceY(y);
		leaf[i] = wxPoint(x,y);
		dc.DrawCircle(x,y,2);
	}

	dc.DrawSpline(WXSIZEOF(leaf),leaf);
}



void LeafCanvas::OnResize(wxSizeEvent& event)
{
	Refresh();
}
