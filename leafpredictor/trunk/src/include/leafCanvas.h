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

#ifdef __GNUG__
#pragma interface "leafCanvas.cpp"
#endif

#include "leafPrediction.h"

#include "wx/dcbuffer.h"
#include "wx/scrolwin.h"
#include "wx/dcscreen.h"
#include "dcsvg.h"

#include <vector>

class LeafCanvas: public wxScrolledWindow
{
protected:
	std::vector<double>  mLeaf;
	std::vector<double>  mLeafOverlay;
	double               mXRange, mYRange, mXMid, mYMid, mScale;
	wxString             mLabel;
	bool                 mLeafExists, mOverlayExists, mSelected;
	wxInt32              mPC1, mPC2, mPC3, mPC4, mPC1Value, mPC2Value, mPC3Value, mPC4Value;

	void CalculateScale(void);

	void DrawLeaf(wxDC& dc, wxInt32 xOut, wxInt32 yOut, wxInt32 thickness, bool label = false, double fixedscale = 0);

	void OnPaint(wxPaintEvent& event);

	void OnResize(wxSizeEvent& event);

	void PaintBackground(wxDC& dc);

	void OnClick(wxMouseEvent& event);


public:
	LeafCanvas(wxWindow* parent, wxWindowID, const wxPoint& pos, const wxSize& size);
	virtual ~LeafCanvas();

	void SetLeaf(const std::vector<double> &coords);
	void SetOverlay(const std::vector<double> &coords);
	void RemoveOverlay(void);

	void SetScale(double scale) {mScale = scale;}
	double GetScale(void) {return mScale;}
	void SetLabel(wxString label) {mLabel = label;}

	void SetPrediction(LeafPrediction prediction);

	void SetPC1(wxInt32 PC) {mPC1 = PC;}
	void SetPC2(wxInt32 PC) {mPC2 = PC;}
	void SetPC3(wxInt32 PC) {mPC3 = PC;}
	void SetPC4(wxInt32 PC) {mPC4 = PC;}

	void SetPC1Value(wxInt32 value) {mPC1Value = value;}
	void SetPC2Value(wxInt32 value) {mPC2Value = value;}
	void SetPC3Value(wxInt32 value) {mPC3Value = value;}
	void SetPC4Value(wxInt32 value) {mPC4Value = value;}

	wxUint32 GetPC1(void) {return mPC1;}
	wxUint32 GetPC2(void) {return mPC2;}
	wxUint32 GetPC3(void) {return mPC3;}
	wxUint32 GetPC4(void) {return mPC4;}

	wxInt32 GetPC1Value(void) {return mPC1Value;}
	wxInt32 GetPC2Value(void) {return mPC2Value;}
	wxInt32 GetPC3Value(void) {return mPC3Value;}
	wxInt32 GetPC4Value(void) {return mPC4Value;}

	void ExtDraw(void);

	wxBitmap GetScreenShot(void);
	
	void CreateSVGFile(wxString filename);

	void ExportLeaf(const wxString &location);

	bool NeedsInversion(void);

	void Select(bool select) {mSelected = select;}
private:
	DECLARE_EVENT_TABLE()
};

#endif /* _LEAFCANVAS_H */
