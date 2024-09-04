#pragma once

class CUtilMerge
{
public:
	CUtilMerge();
	~CUtilMerge();

public:	
	std::vector<std::vector<CRavidPoint<int>>> MergePoints(std::vector<CRavidPoint<int>>* pSrc, double dblDist = 1.);
	std::vector<std::vector<CRavidPoint<double>>> MergePoints(std::vector<CRavidPoint<double>>* pSrc, double dblDist = 1.);
	std::vector<std::vector<CRavidRect<int>>> MergeRoi(std::vector<CRavidRect<int>>* pSrc);
	std::vector<std::vector<CRavidRect<double>>> MergeRoi(std::vector<CRavidRect<double>>* pSrc);
	std::vector<std::vector<CRavidQuadrangle<double>>> MergeRoi(std::vector<CRavidQuadrangle<double>>* pSrc);

	bool MergeRoi(std::vector<CRavidRect<int>>* pSrc, std::vector<CRavidLine<double>>* pSrc2, std::vector<std::vector<CRavidRect<int>>>* pResultRoi, std::vector<std::vector<CRavidLine<double>>>* pResultLine);

	double GetArea(std::vector<CRavidRect<double>>* pSrc);

protected:
	int FindParent(int* pIndex, int nV);
	void Union_find(int* pIndex, int nV1, int nV2);

	int GetComp(std::vector<double>* pVctComp, double dblY);
	struct Line
	{
		double dblX, dblY1, dblY2;
		int nCheck;
		bool operator < (const Line& p) { return dblX < p.dblX; }
	};

	struct SEG
	{
		int n;
		std::vector<double> sum;
		std::vector<long> cnt;

		SEG(int n) : n(n), sum(4 * n + 10), cnt(4 * n + 10) { }

		void Update(int node, int tl, int tr, int l, int r, int nCheck, std::vector<double>* pVctComp)
		{

			if(tr < l || r < tl)
				return;

			double* pComp = &pVctComp->at(0);
			if(l <= tl && tr <= r)
			{
				cnt[node] += nCheck;
				if(cnt[node] != 0)
					sum[node] = pComp[tr] - pComp[tl - 1];
				else
				{
					if(tl != tr) 
						sum[node] = sum[node * 2] + sum[node * 2 + 1];
					else 
						sum[node] = 0;
				}
				return;
			}
		
			int mid = (tl + tr) >> 1;
			Update(node * 2, tl, mid, l, r, nCheck, pVctComp);
			Update(node * 2 + 1, mid + 1, tr, l, r, nCheck, pVctComp);

			if(cnt[node] != 0) 
				sum[node] = pComp[tr] - pComp[tl - 1];
			else
			{
				if(tl != tr) sum[node] = sum[node * 2] + sum[node * 2 + 1];
				else sum[node] = 0;
			}
		}

		double Query() { return sum[1]; }
	};
private:

};

