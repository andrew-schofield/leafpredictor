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

#ifndef _LEAFPREDICTION_H
#define _LEAFPREDICTION_H

#ifdef __GNUG__
#pragma interface "leafPrediction.cpp"
#endif

class LeafPrediction
{
public:

	LeafPrediction();

	~LeafPrediction(void);

	void SetPC1(wxUint32 value) {mPC1 = value;}
	void SetPC2(wxUint32 value) {mPC2 = value;}
	void SetPC3(wxUint32 value) {mPC3 = value;}
	void SetPC4(wxUint32 value) {mPC4 = value;}

	void SetPC1Value(wxUint32 value) {mPC1Value = value;}
	void SetPC2Value(wxUint32 value) {mPC2Value = value;}
	void SetPC3Value(wxUint32 value) {mPC3Value = value;}
	void SetPC4Value(wxUint32 value) {mPC4Value = value;}

	wxUint32 GetPC1(void) {return mPC1;}
	wxUint32 GetPC2(void) {return mPC2;}
	wxUint32 GetPC3(void) {return mPC3;}
	wxUint32 GetPC4(void) {return mPC4;}

	wxInt32  GetPC1Value(void) {return mPC1Value;}
	wxInt32  GetPC2Value(void) {return mPC2Value;}
	wxInt32  GetPC3Value(void) {return mPC3Value;}
	wxInt32  GetPC4Value(void) {return mPC4Value;}


private:

	wxUint32 mPC1;
	wxUint32 mPC2;
	wxUint32 mPC3;
	wxUint32 mPC4;
	wxInt32  mPC1Value;
	wxInt32  mPC2Value;
	wxInt32  mPC3Value;
	wxInt32  mPC4Value;
};


#endif /* _LEAFPREDICTION_H */
