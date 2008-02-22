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

#ifndef _EIGENSYSTEM_H
#define _EIGENSYSTEM_H

#include "wx/string.h"
#include <vector>

class EigenSystem
{
protected:
	std::vector< double >                 mMeanLeaf;
	std::vector< double >                 mPredictedLeaf;
	std::vector< double >                 mEigenValues;
	std::vector< std::vector<double> >    mEigenVectors;
	double                                mTotalVariance;

	std::vector<double> SplitLineByTabs(wxString line);

public:

	EigenSystem();

	~EigenSystem(void);

	bool LoadEigenFile(wxString eigenFile);

	wxInt32 GetNumberOfCoordinates(void);

	double GetPCSD(wxInt32 PC);

	double GetPCContribution(wxInt32 PC);

	std::vector< double > GetMeanLeaf(void) {return mMeanLeaf;}
	std::vector< double > GetPredictedLeaf(void) {return mPredictedLeaf;}

	void PredictLeaf(wxUint32 PC1, wxInt32 PC1Value, wxUint32 PC2, wxInt32 PC2Value, wxUint32 PC3, wxInt32 PC3Value, wxUint32 PC4, wxInt32 PC4Value);

};


#endif /* _EIGENSYSTEM_H */
