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
#include "mainDialog.h"

#include "wx/image.h"
#include "wx/txtstrm.h"
#include "wx/textfile.h"
#include "wx/wfstream.h"

// Identifiers for the controls
enum _CONTROL_ID
{
	EXPORT_IMAGE = wxID_HIGHEST
};

BEGIN_EVENT_TABLE(LeafCanvas, wxScrolledWindow)
		EVT_PAINT(LeafCanvas::OnPaint)
		EVT_SIZE (LeafCanvas::OnResize)
		EVT_LEFT_DOWN(LeafCanvas::OnClick)
END_EVENT_TABLE()

LeafCanvas::LeafCanvas(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize &size) : wxScrolledWindow(parent, wxID_ANY, pos, size, wxSUNKEN_BORDER)
{
	//SetBackgroundColour(*wxWHITE);
	mLabel = wxT("");
	mLeafExists = false;
	mPC1 = 0;
	mPC2 = 0;
	mPC3 = 0;
	mPC4 = 0;
	mPC1Value = 0;
	mPC2Value = 0;
	mPC3Value = 0;
	mPC4Value = 0;
	mOverlayExists = false;
	mSelected = false;
	mXRange = 0;
	mYRange = 0;
	mXMid = 0;
	mYMid = 0;
}


LeafCanvas::~LeafCanvas()
{
}


void LeafCanvas::SetLeaf(const std::vector<double> &coords)
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

	DrawLeaf(dc, GetClientSize().x, GetClientSize().y, 1, true, MainDialog::GetInstance()->GetManualScale());

}


void LeafCanvas::PaintBackground(wxDC& dc)
{

	wxColour backgroundColour;

	if(mSelected)
	{
		backgroundColour = *wxWHITE;
	}
	else
	{
		backgroundColour = wxColour(245, 245, 245);
	}

	dc.SetBrush(wxBrush(backgroundColour));
	dc.SetPen(wxPen(backgroundColour, 1));

	wxRect windowRect(wxPoint(0, 0), GetClientSize());
	CalcUnscrolledPosition(windowRect.x, windowRect.y, & windowRect.x, & windowRect.y);
	dc.DrawRectangle(windowRect);

	SetBackgroundColour(backgroundColour);

	//dc.Clear();
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
	double dScale = MainDialog::GetInstance()->GetManualScale();

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
		dScale *= 2;
	}
	wxBitmap bitmap(width, height);
	wxBufferedDC dc(NULL, bitmap);

	PaintBackground(dc);
	DrawLeaf(dc, width, height, thickness, false, dScale);

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

		wxBitmap aabitmap(anti, -1);
		bitmap = aabitmap;
	}

	return bitmap;
}


void LeafCanvas::DrawLeaf(wxDC& dc, wxInt32 xOut, wxInt32 yOut, wxInt32 thickness, bool label, double fixedscale)
{
	wxColour leafColour = *wxGREEN;
	wxUint32 i;
	double xScale, yScale, scale;

	xScale = xOut/(double)GetClientSize().x;
	yScale = yOut/(double)GetClientSize().y;
	if (fixedscale != 0)
		scale = fixedscale;
	else
		scale = std::min(xScale,yScale) * mScale;
	//Tools::InfoMsgBox(wxString::Format(wxT("%.2f, %.2f"), fixedscale, scale));

	if(label)
		dc.DrawText(mLabel,0,0);

	dc.SetPen(wxPen(*wxLIGHT_GREY, thickness));
	//axes
	dc.DrawLine(0,(int)(yOut/2 - mYMid * scale),xOut,(int)(yOut/2 - mYMid * scale));
	dc.DrawLine((int)(xOut/2 - mXMid * scale),0,(int)(xOut/2 - mXMid * scale),yOut);

	wxInt32 min = 0 - (int)(xOut / scale - (xOut/2 - mXMid * scale));
	wxInt32 max = (int)(xOut / scale - (xOut/2 - mXMid * scale));

	if(min != max) //traps initial state where both min and max are -2^32
	{
		for(wxInt32 j=0;j<=max;j+=100)
		{
			wxUint32 x, y;
			x = (int)(j * scale + (xOut/2 - mXMid * scale));
			x = dc.LogicalToDeviceX(x);
			y = (int)(yOut/2 - mYMid * scale);
			dc.DrawLine(x,y,x,y-5);
		}
		for(wxInt32 j=0;j>=min;j-=100)
		{
			wxUint32 x, y;
			x = (int)(j * scale + (xOut/2 - mXMid * scale));
			x = dc.LogicalToDeviceX(x);
			y = (int)(yOut/2 - mYMid * scale);
			dc.DrawLine(x,y,x,y-5);
		}
	}

	min = (int)(0 - yOut / scale - (yOut/2 - mYMid * scale));
	max = (int)(yOut / scale - (yOut/2 - mYMid * scale));

	if(min != max)
	{
		for(wxInt32 j=0;j<=max;j+=100)
		{
			wxUint32 x, y;
			y = (int)(j * scale + (yOut/2 - mYMid * scale));
			y = dc.LogicalToDeviceY(y);
			x = (int)(xOut/2 - mXMid * scale);
			dc.DrawLine(x,y,x+5,y);
		}

		for(wxInt32 j=0;j>=min;j-=100)
		{
			wxUint32 x, y;
			y = (int)(j * scale + (yOut/2 - mYMid * scale));
			y = dc.LogicalToDeviceY(y);
			x = (int)(xOut/2 - mXMid * scale);
			dc.DrawLine(x,y,x+5,y);
		}
	}

	if (mOverlayExists)
	{
		dc.SetPen(wxPen(*wxBLACK, thickness));
		wxPoint *leaf;
		leaf = new wxPoint[(mLeafOverlay.size()/2)+1];
		for(i=0;i<(mLeafOverlay.size()/2);++i)
		{
			int x = (int)(mLeafOverlay.at(i*2) * scale + (xOut/2 - mXMid * scale));
			int y = (int)(mLeafOverlay.at((i*2)+1) * scale + (yOut/2 - mYMid * scale));
			x = dc.LogicalToDeviceX(x);
			y = dc.LogicalToDeviceY(y);
			leaf[i] = wxPoint(x,y);
			if(MainDialog::GetInstance()->GetShowLandmarks())
				dc.DrawCircle(x,y,thickness*2);
			if (i == 0)
				leaf[(mLeafOverlay.size()/2)] = wxPoint(x,y);
		}

		//dc.SetBrush(wxBrush(*wxBLACK));
		dc.SetPen(wxPen(*wxBLACK, thickness));
		dc.DrawSpline((mLeafOverlay.size()/2)+1,leaf);
		delete [] leaf;
		leaf = NULL;
	}

	dc.SetPen(wxPen(*wxRED, thickness));

	if (mLeafExists)
	{
		wxPoint *leaf;
		leaf = new wxPoint[(mLeaf.size()/2)+1];
		for(i=0;i<(mLeaf.size()/2);++i)
		{
			int x = (int)(mLeaf.at(i*2) * scale + (xOut/2 - mXMid * scale));
			int y = (int)(mLeaf.at((i*2)+1) * scale + (yOut/2 - mYMid * scale));
			x = dc.LogicalToDeviceX(x);
			y = dc.LogicalToDeviceY(y);
			leaf[i] = wxPoint(x,y);
			if(MainDialog::GetInstance()->GetShowLandmarks())
				dc.DrawCircle(x,y,thickness*2);
			if (i == 0)
				leaf[(mLeaf.size()/2)] = wxPoint(x,y);
		}

		dc.SetBrush(wxBrush(leafColour));
		dc.SetPen(wxPen(leafColour, thickness));
		dc.DrawSpline((mLeaf.size()/2)+1,leaf);
		delete [] leaf;
		leaf = NULL;
	}
}


void LeafCanvas::OnClick(wxMouseEvent& event)
{
	MainDialog::GetInstance()->ExternalCanvasSelect(mLabel);
}


void LeafCanvas::ExportLeaf(const wxString &location)
{
	wxFileOutputStream   fileOS(location);
	wxTextOutputStream   textOS(fileOS);
	wxString outLine =  wxT("0");

	for(wxUint32 i=0; i<mLeaf.size();++i)
		outLine += wxString::Format(wxT(",%.2f"),mLeaf.at(i));

	if(fileOS.Ok() == false)
	{
		Tools::ErrorMsgBox(wxString::Format(_("Could not open file for writing!\nThe leaf will not be saved!")));
		return;
	}
	else
	{
		textOS.WriteString(outLine);
		fileOS.Close();
	}
}


bool LeafCanvas::NeedsInversion(void)
{
	if((GetClientSize().y/2 - mYMid * mScale) > (GetClientSize().y * 0.66))
		return true;
	return false;
}


void LeafCanvas::SetOverlay(const std::vector<double> &coords)
{
	mLeafOverlay = coords;
	mOverlayExists = true;
}


void LeafCanvas::RemoveOverlay(void)
{
	mLeafOverlay.empty();
	mOverlayExists = false;
}


void LeafCanvas::SetPrediction(LeafPrediction prediction)
{
	SetPC1(prediction.GetPC1());
	SetPC2(prediction.GetPC2());
	SetPC3(prediction.GetPC3());
	SetPC4(prediction.GetPC4());
	SetPC1Value(prediction.GetPC1Value());
	SetPC2Value(prediction.GetPC2Value());
	SetPC3Value(prediction.GetPC3Value());
	SetPC4Value(prediction.GetPC4Value());
	CalculateScale();
	Refresh();
	MainDialog::GetInstance()->ExternalCanvasSelect(mLabel);
}


void LeafCanvas::CreateSVGFile(wxString filename)
{
	wxInt32 width = GetClientSize().x;
	wxInt32 height = GetClientSize().y;
	wxInt32 thickness = 2;
	double dScale = MainDialog::GetInstance()->GetManualScale();

 	wxSVGFileDC svgDC (filename, width, height);
	PaintBackground(svgDC);
	DrawLeaf(svgDC, width, height, thickness, false, dScale);
}

