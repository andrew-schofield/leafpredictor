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
#include "eigenSystem.h"

#include "tools.h"
#include "mainDialog.h"

#include "wx/textfile.h"
#include "wx/txtstrm.h"
#include "wx/msgdlg.h"

#include <vector>
#include <math.h>

EigenSystem::EigenSystem()
{
	mMeanLeaf.empty();
	mPredictedLeaf.empty();
	mEigenValues.empty();
	mEigenVectors.empty();
	mTotalVariance = 0;
}


EigenSystem::~EigenSystem(void)
{
}


bool EigenSystem::LoadEigenFile(wxString eigenFile)
{
	wxTextFile in;
	wxInt32    lineCount;
	wxUint8     i;

	if(in.Open(eigenFile) == false)
		return false;

	lineCount = in.GetLineCount();
	if (lineCount != 103)
		return false;

	/*
	 * Structure of eigenfile:
	 * 0: Mean Leaf
	 * 1: Tab separated values for mean leaf x,y coordinates
	 * 2: blank line
	 * 3: Eigenvalues
	 * 4: Tab separated values for coordinate eigenvalues
	 * 5: blank line
	 * 6: Eigenvectors
	 * 7-102: 96 lines of tab separated values for coordinate eigenvectors for each of the 96 PCs generated by leafanalyser
	 *
	 * The number of x,y coordinates and therefore the eigenvalues/vectors is not defined.
	 */

	mMeanLeaf = SplitLineByTabs(in.GetLine(1));
	mPredictedLeaf = mMeanLeaf;
	mEigenValues = SplitLineByTabs(in.GetLine(4));
	for(i=0;i<mMeanLeaf.size();++i)
	{
		mTotalVariance += mEigenValues.at(i);
	}

	for(i=7;i<=102;++i)
	{
		mEigenVectors.push_back(SplitLineByTabs(in.GetLine(i)));
	}

	return true;
}

std::vector<double> EigenSystem::SplitLineByTabs(wxString line)
{
	std::vector<double>    v;
	wxInt32                endingPos;
	wxString               choppedString;
	wxString               tmpString;
	double                 tmpDouble;

	choppedString = line;
	endingPos = choppedString.Find(wxT("\t"));
	while(endingPos != wxNOT_FOUND)
	{
		tmpString = choppedString.Mid(0, endingPos);
		choppedString = choppedString.Mid(endingPos + 1, choppedString.length()- 1);
		endingPos = choppedString.Find(wxT("\t"));
		tmpString.ToDouble(&tmpDouble);
		v.push_back(tmpDouble);
	}
	return v;
}


wxInt32 EigenSystem::GetNumberOfCoordinates(void)
{
	return mMeanLeaf.size();
}


double EigenSystem::GetPCSD(wxInt32 PC)
{
	return sqrt(mEigenValues.at(PC));
}


double EigenSystem::GetPCContribution(wxInt32 PC)
{
	return 100 * (mEigenValues.at(PC -1) / mTotalVariance);
}

void EigenSystem::PredictLeaf(wxUint32 PC1, wxInt32 PC1Value, wxUint32 PC2, wxInt32 PC2Value, wxUint32 PC3, wxInt32 PC3Value)
{
	double SD1, SD2, SD3, val1, val2, val3;

	SD1 = GetPCSD(PC1);
	SD2 = GetPCSD(PC2);
	SD3 = GetPCSD(PC3);

	val1 = PC1Value;
	val2 = PC2Value;
	val3 = PC3Value;

	wxUint32 i;
	for(i=0;i<mMeanLeaf.size();++i)
	{
		mPredictedLeaf[i] = mMeanLeaf[i] + (mEigenVectors[PC1][i] * (SD1 * (val1 / 10))) + (mEigenVectors[PC2][i] * (SD2 * (val2 / 10))) + (mEigenVectors[PC3][i] * (SD3 * (val3 / 10)));
	}

	MainDialog::GetInstance()->SetPCMessage(wxString::Format(wxT("PC %i: %.1f SDs; PC %i: %.1f SDs; PC %i: %.1f SDs"), PC1+1, val1/10, PC2+1, val2/10, PC3+1, val3/10));
}
