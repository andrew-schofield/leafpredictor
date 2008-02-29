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
#pragma implementation "leafCanvas.h"
#endif

#include "wx/wxprec.h"

#ifdef  __BORLANDC__
#pragma hdrstop
#endif

#include "leafpredictor.h"
#include "leafCanvas.h"

#include "tools.h"
#include "imageDialog.h"

#include "wx/image.h"

// Identifiers for the controls
enum _CONTROL_ID
{
	EXPORT_IMAGE = wxID_HIGHEST
};

BEGIN_EVENT_TABLE(LeafCanvas, wxScrolledWindow)
		EVT_PAINT(LeafCanvas::OnPaint)
		EVT_SIZE (LeafCanvas::OnResize)
END_EVENT_TABLE()

LeafCanvas::LeafCanvas(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize &size) : wxScrolledWindow(parent, wxID_ANY, pos, size, wxSUNKEN_BORDER)
{
	SetBackgroundColour(*wxWHITE);
	mLabel = wxT("");
	mLeafExists = false;
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

	CalculateScale();
	mLeafExists = true;

	//Refresh();
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

	wxColour backgroundColour = *wxWHITE;

	dc.SetBrush(wxBrush(backgroundColour));

	dc.Clear();
}


void LeafCanvas::DrawLeaf(wxDC& dc)
{
	wxColour leafColour = *wxGREEN;
	wxUint32 i;


	dc.DrawText(mLabel,0,0);

	dc.SetPen(wxPen(*wxLIGHT_GREY, 1));
	//axes
	dc.DrawLine(0,GetClientSize().y/2,GetClientSize().x,GetClientSize().y/2);
	dc.DrawLine(GetClientSize().x/2,0,GetClientSize().x/2,GetClientSize().y);

	dc.SetPen(wxPen(*wxRED, 1));

	if (mLeafExists)
	{
		wxPoint leaf[(mLeaf.size()/2)+1];
		for(i=0;i<(mLeaf.size()/2);++i)
		{
			int x = (int)(mLeaf.at(i*2) * mScale + (GetClientSize().x/2 - mXMid * mScale));
			int y = (int)(mLeaf.at((i*2)+1) * mScale + (GetClientSize().y/2 - mYMid * mScale));
			x = dc.LogicalToDeviceX(x);
			y = dc.LogicalToDeviceY(y);
			leaf[i] = wxPoint(x,y);
			dc.DrawCircle(x,y,2);
			if (i == 0)
				leaf[(mLeaf.size()/2)] = wxPoint(x,y);
		}

		dc.SetBrush(wxBrush(leafColour));
		dc.SetPen(wxPen(leafColour, 1));
		dc.DrawSpline(WXSIZEOF(leaf),leaf);
	}
}


void LeafCanvas::OnResize(wxSizeEvent& event)
{
	CalculateScale();
	Refresh();
}


void LeafCanvas::CalculateScale(void)
{
	double   xScale, yScale;

	xScale = (GetClientSize().x -20) / mXRange;
	yScale = (GetClientSize().y -20) / mYRange;

	if(xScale<yScale)
		mScale = xScale;
	else
		mScale = yScale;
}


void LeafCanvas::ExtDraw(void)
{
	Refresh();
}


wxBitmap LeafCanvas::GetScreenShot(void)
{
	bool publish = false;
	wxInt32 width = 0;
	wxInt32 height = 0;
	wxInt32 thickness = 0;
	bool antialiase = false;

	ImageDialog dialog(NULL, EXPORT_IMAGE);
	if (dialog.ShowModal() == wxID_OK)
	{
		double tmpDouble;
		publish = dialog.GetPublishableImage();
		dialog.GetXSize().ToDouble(&tmpDouble);
		width = (int)tmpDouble;
		dialog.GetYSize().ToDouble(&tmpDouble);
		height = (int)tmpDouble;
		dialog.GetThickness().ToDouble(&tmpDouble);
		thickness = (int)tmpDouble;
		antialiase = dialog.GetAntialiase();
	}
	if(!publish)
	{
		width = GetClientSize().x;
		height = GetClientSize().y;
		thickness = 1;
	}
	if(antialiase)
	{
		width *= 2;
		height *= 2;
		thickness *= 2;
	}
	wxBitmap bitmap(width, height);
	wxBufferedDC dc(NULL, bitmap);

	PaintBackground(dc);
	DrawHQLeaf(dc, width, height, thickness);

	if(antialiase)
	{
		wxImage original = bitmap.ConvertToImage();
		wxImage anti(bitmap.GetWidth()/2, bitmap.GetHeight()/2);

		for (int y = 1; y < (bitmap.GetHeight()/2)-1; y++)
			for (int x = 1; x < (bitmap.GetWidth()/2)-1; x++)
		{
			int red = original.GetRed( x*2, y*2 ) +
					original.GetRed( x*2-1, y*2 ) +
					original.GetRed( x*2, y*2+1 ) +
					original.GetRed( x*2+1, y*2+1 );
			red = red/4;

			int green = original.GetGreen( x*2, y*2 ) +
					original.GetGreen( x*2-1, y*2 ) +
					original.GetGreen( x*2, y*2+1 ) +
					original.GetGreen( x*2+1, y*2+1 );
			green = green/4;

			int blue = original.GetBlue( x*2, y*2 ) +
					original.GetBlue( x*2-1, y*2 ) +
					original.GetBlue( x*2, y*2+1 ) +
					original.GetBlue( x*2+1, y*2+1 );
			blue = blue/4;
			anti.SetRGB( x, y, (unsigned char)red, (unsigned char)green, (unsigned char)blue );
		}

		bitmap = anti;
	}

	return bitmap;
}


void LeafCanvas::DrawHQLeaf(wxDC& dc, wxInt32 xOut, wxInt32 yOut, wxInt32 thickness)
{
	wxColour leafColour = *wxGREEN;
	wxUint32 i;
	double xScale, yScale, scale;

	xScale = xOut/(double)GetClientSize().x;
	yScale = yOut/(double)GetClientSize().y;
	scale = std::min(xScale,yScale);

	//dc.DrawText(mLabel,0,0);

	dc.SetPen(wxPen(*wxLIGHT_GREY, thickness));
	//axes
	dc.DrawLine(0,yOut/2,xOut,yOut/2);
	dc.DrawLine(xOut/2,0,xOut/2,yOut);

	dc.SetPen(wxPen(*wxRED, thickness));

	if (mLeafExists)
	{
		wxPoint leaf[(mLeaf.size()/2)+1];
		for(i=0;i<(mLeaf.size()/2);++i)
		{
			int x = (int)(mLeaf.at(i*2) * (mScale * scale) + (xOut/2 - mXMid * (mScale * scale)));
			int y = (int)(mLeaf.at((i*2)+1) * (mScale * scale) + (yOut/2 - mYMid * (mScale * scale)));
			x = dc.LogicalToDeviceX(x);
			y = dc.LogicalToDeviceY(y);
			leaf[i] = wxPoint(x,y);
			dc.DrawCircle(x,y,thickness*2);
			if (i == 0)
				leaf[(mLeaf.size()/2)] = wxPoint(x,y);
		}

		dc.SetBrush(wxBrush(leafColour));
		dc.SetPen(wxPen(leafColour, thickness));
		dc.DrawSpline(WXSIZEOF(leaf),leaf);
	}
}
