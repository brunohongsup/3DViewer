#include "stdafx.h"
#include "UtilMerge.h"

CUtilMerge::CUtilMerge()
{

}

CUtilMerge::~CUtilMerge()
{
	
}
std::vector<std::vector<CRavidPoint<int>>> CUtilMerge::MergePoints(std::vector<CRavidPoint<int>>* pSrc, double dblDist)
{
	std::vector<std::vector<CRavidPoint<int>>> vctResult;

	do
	{
	
	}
	while(false);

	return vctResult;
}
std::vector<std::vector<CRavidPoint<double>>> CUtilMerge::MergePoints(std::vector<CRavidPoint<double>>* pSrc, double dblDist)
{
	std::vector<std::vector<CRavidPoint<double>>> vctResult;

	do
	{
		int nSize = pSrc->size();
		if(!nSize)
			break;

		std::sort(pSrc->begin(), pSrc->end(), [&](CRavidPoint<double> r1, CRavidPoint<double> r2) -> bool
		{
			return (r1.x < r2.x);
		});

		CRavidPoint<double>* pData = &pSrc->at(0);
		std::vector<int> vctIndex;

		for(int i = 0; i < nSize; ++i)
			vctIndex.push_back(i);
		int* pIndex = &vctIndex.at(0);

		double dblNewDist = dblDist * dblDist;
		double dblX = 0;
		double dblY = 0;
		for(int i = 0; i < nSize; ++i)
		{
			for(int k = i + 1; k < nSize; ++k)
			{
				dblX = (pData + k)->x - (pData + i)->x;
				dblX *= dblX;
				dblY = (pData + k)->y - (pData + i)->y;
				dblY *= dblY;

				if (dblY <= dblNewDist - dblX)
					Union_find(pIndex, i, k);


				if((pData + k)->x - (pData + i)->x > dblDist)
					break;
			}
		}
	
		std::vector<CRavidPoint<double>>* pSort = nullptr;
		pSort = new std::vector<CRavidPoint<double>>[nSize];

		int nNo = 0;
		for(int i = 0; i < nSize; ++i)
		{
			nNo = FindParent(pIndex, i);
			pSort[nNo].push_back(pData[i]);
		}

		for(int i = 0; i < nSize; ++i)
		{
			if(pSort[i].size())
				vctResult.push_back(pSort[i]);
		}

		delete[] pSort;

	}
	while(false);

	return vctResult;
}
std::vector<std::vector<CRavidRect<int>>> CUtilMerge::MergeRoi(std::vector<CRavidRect<int>>* pSrc)
{
	std::vector<std::vector<CRavidRect<int>>> vctResult;

	do
	{
		int nSize = pSrc->size();
		if(!nSize)
			break;

		std::sort(pSrc->begin(), pSrc->end(), [&](CRavidRect<int> r1, CRavidRect<int> r2) -> bool
		{
			return (r1.left < r2.left);
		});

		CRavidRect<int>* pData = &pSrc->at(0);
		std::vector<int> vctIndex;

		for(int i = 0; i < nSize; ++i)
			vctIndex.push_back(i);
		int* pIndex = &vctIndex.at(0);

		for(int i = 0; i < nSize; ++i)
		{
			for(int k = i + 1; k < nSize; ++k)
			{
				if((pData + i)->DoesIntersect((pData + k)))
					Union_find(pIndex, i, k);

				if((pData+i)->right < (pData+k)->left)
					break;
			}
		}
	
		std::vector<CRavidRect<int>>* pSort = nullptr;
		pSort = new std::vector<CRavidRect<int>>[nSize];

		int nNo = 0;
		for(int i = 0; i < nSize; ++i)
		{
			nNo = FindParent(pIndex, i);
			pSort[nNo].push_back(pData[i]);
		}

		for(int i = 0; i < nSize; ++i)
		{
			if(pSort[i].size())
				vctResult.push_back(pSort[i]);
		}

		delete[] pSort;

	}
	while(false);

	return vctResult;
}
std::vector<std::vector<CRavidRect<double>>> CUtilMerge::MergeRoi(std::vector<CRavidRect<double>>* pSrc)
{
	std::vector<std::vector<CRavidRect<double>>> vctResult;

	do
	{
		int nSize = pSrc->size();
		if(!nSize)
			break;

		std::sort(pSrc->begin(), pSrc->end(), [&](CRavidRect<double> r1, CRavidRect<double> r2) -> bool
		{
			return (r1.left < r2.left);
		});

		CRavidRect<double>* pData = &pSrc->at(0);
		std::vector<int> vctIndex;

		for(int i = 0; i < nSize; ++i)
			vctIndex.push_back(i);
		int* pIndex = &vctIndex.at(0);

		for(int i = 0; i < nSize; ++i)
		{
			for(int k = i + 1; k < nSize; ++k)
			{
				if((pData + i)->DoesIntersect((pData + k)))
					Union_find(pIndex, i, k);

				if((pData + i)->right < (pData + k)->left)
					break;
			}
		}

		std::vector<CRavidRect<double>>* pSort = nullptr;
		pSort = new std::vector<CRavidRect<double>>[nSize];

		int nNo = 0;
		for(int i = 0; i < nSize; ++i)
		{
			nNo = FindParent(pIndex, i);
			pSort[nNo].push_back(pData[i]);
		}

		for(int i = 0; i < nSize; ++i)
		{
			if(pSort[i].size())
				vctResult.push_back(pSort[i]);
		}

		delete[] pSort;

	}
	while(false);

	return vctResult;
}

std::vector<std::vector<CRavidQuadrangle<double>>> CUtilMerge::MergeRoi(std::vector<CRavidQuadrangle<double>>* pSrc)
{
	std::vector<std::vector<CRavidQuadrangle<double>>> vctResult;

	do
	{
		int nSize = pSrc->size();
		if(!nSize)
			break;

		CRavidQuadrangle<double>* pData = &pSrc->at(0);

		std::vector<std::pair<CRavidPoint<double>, CRavidQuadrangle<double>>> vctpairData;
		CRavidPoint<double> dpt;
		for(int i = 0; i < nSize; ++i)
		{
			dpt.x = __min(pData[i].rpPoints[0].x, __min(pData[i].rpPoints[1].x, __min(pData[i].rpPoints[2].x, pData[i].rpPoints[3].x)));
			dpt.y = __max(pData[i].rpPoints[0].x, __max(pData[i].rpPoints[1].x, __max(pData[i].rpPoints[2].x, pData[i].rpPoints[3].x)));

			vctpairData.push_back(std::make_pair(dpt, pData[i]));
		}
	
		std::sort(vctpairData.begin(), vctpairData.end(), [&](std::pair<CRavidPoint<double>, CRavidQuadrangle<double>> r1, std::pair<CRavidPoint<double>, CRavidQuadrangle<double>> r2) -> bool
		{
			return (r1.first.x < r2.first.x);
		});

		std::vector<int> vctIndex;

		for(int i = 0; i < nSize; ++i)
			vctIndex.push_back(i);
		int* pIndex = &vctIndex.at(0);

		std::pair<CRavidPoint<double>, CRavidQuadrangle<double>>* pPairData = &vctpairData.at(0);

		for(int i = 0; i < nSize; ++i)
		{
			for(int k = i + 1; k < nSize; ++k)
			{
				if((pPairData + i)->second.DoesIntersect((pPairData + k)->second))
					Union_find(pIndex, i, k);

				if((pPairData + i)->first.y < (pPairData + k)->first.x)
					break;
			}
		}

		std::vector<CRavidQuadrangle<double>>* pSort = nullptr;
		pSort = new std::vector<CRavidQuadrangle<double>>[nSize];

		int nNo = 0;
		for(int i = 0; i < nSize; ++i)
		{
			nNo = FindParent(pIndex, i);
			pSort[nNo].push_back(pPairData[i].second);
		}

		for(int i = 0; i < nSize; ++i)
		{
			if(pSort[i].size())
				vctResult.push_back(pSort[i]);
		}

		delete[] pSort;

	}
	while(false);

	return vctResult;
}

bool CUtilMerge::MergeRoi(std::vector<CRavidRect<int>>* pSrc, std::vector<CRavidLine<double>>* pSrc2, std::vector<std::vector<CRavidRect<int>>>* pResultRoi, std::vector<std::vector<CRavidLine<double>>>* pResultLine)
{
	bool bReturn = false;
	do
	{
		int nSize = pSrc->size();
		if(!nSize)
			break;
		if(nSize != pSrc2->size())
			break;

// 		std::sort(pSrc->begin(), pSrc->end(), [&](CRavidRect<int> r1, CRavidRect<int> r2) -> bool
// 		{
// 			return (r1.left < r2.left);
// 		});

		CRavidRect<int>* pData = &pSrc->at(0);
		CRavidLine<double>* pData2 = &pSrc2->at(0);
		std::vector<int> vctIndex;

		for(int i = 0; i < nSize; ++i)
			vctIndex.push_back(i);
		int* pIndex = &vctIndex.at(0);

		for(int i = 0; i < nSize; ++i)
		{
			for(int k = i + 1; k < nSize; ++k)
			{
				if((pData + i)->DoesIntersect((pData + k)))
					Union_find(pIndex, i, k);

// 				if((pData + i)->right < (pData + k)->left)
// 					break;
			}
		}

		std::vector<CRavidRect<int>>* pSort = nullptr;
		pSort = new std::vector<CRavidRect<int>>[nSize];
		std::vector<CRavidLine<double>>* pSort2 = nullptr;
		pSort2 = new std::vector<CRavidLine<double>>[nSize];

		int nNo = 0;
		for(int i = 0; i < nSize; ++i)
		{
			nNo = FindParent(pIndex, i);
			pSort[nNo].push_back(pData[i]);
			pSort2[nNo].push_back(pData2[i]);
		}

		for(int i = 0; i < nSize; ++i)
		{
			if(pSort[i].size())
				pResultRoi->push_back(pSort[i]);
			if(pSort2[i].size())
				pResultLine->push_back(pSort2[i]);
		}

		delete[] pSort;
		delete[] pSort2;

		bReturn = true;
	}
	while(false);

	return bReturn;
}
double CUtilMerge::GetArea(std::vector<CRavidRect<double>>* pSrc)
{
	double dblReturn = 0.;

	do 
	{
		int nSize = pSrc->size();
		if(!nSize)
			break;
		

		CRavidRect<double>* pData = &pSrc->at(0);
		std::vector<Line> vctLine;
		std::vector<double> vctComp;
		for(int i = 0; i < nSize; ++i)
		{
			vctLine.push_back({pData[i].left, pData[i].top, pData[i].bottom, 1});
			vctLine.push_back({ pData[i].right, pData[i].top, pData[i].bottom, -1 });
			vctComp.push_back(pData[i].top);
			vctComp.push_back(pData[i].bottom);
		}

		std::sort(vctComp.begin(), vctComp.end());
		vctComp.erase(std::unique(vctComp.begin(), vctComp.end()), vctComp.end());

		std::sort(vctLine.begin(), vctLine.end(), [&](Line r1, Line r2) -> bool
		{
			return (r1.dblX < r2.dblX);
		});
	
		SEG seg(vctComp.size()-1);
		Line* pLine = &vctLine.at(0);
		double dblBeforeX = pLine[0].dblX;
		int n = nSize * 2;

		for(int i = 0; i < n; ++i)
		{
			dblReturn += (pLine[i].dblX - dblBeforeX) * seg.Query();
			dblBeforeX = pLine[i].dblX;

			seg.Update(1, 1, vctComp.size()-1, GetComp(&vctComp, pLine[i].dblY1) + 1, GetComp(&vctComp, pLine[i].dblY2), pLine[i].nCheck, &vctComp);
		}
	} while (false);

	return dblReturn;
}
int CUtilMerge::FindParent(int* pIndex, int nV)
{
	if(pIndex[nV] != nV)
		pIndex[nV] = FindParent(pIndex, pIndex[nV]);

	return pIndex[nV];
}
void CUtilMerge::Union_find(int* pIndex, int nV1, int nV2)
{
	nV1 = FindParent(pIndex, nV1);
	nV2 = FindParent(pIndex, nV2);
	if(nV1 < nV2)
		pIndex[nV2] = nV1;
	else
		pIndex[nV1] = nV2;
}
int CUtilMerge::GetComp(std::vector<double>* pVctComp, double dblY)
{
	return std::lower_bound(pVctComp->begin(), pVctComp->end(), dblY) - pVctComp->begin();
}
