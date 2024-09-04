#include "stdafx.h"
#include "HeightMap3D.h"

CHeightMap3D::CHeightMap3D() :
	m_pCopyMap(nullptr),
	m_pHeightMap(nullptr),
	m_pHeightColorMap(nullptr),
	m_nUsedSetting(0),
	m_fInvalidValue(-1000),
	m_fLow(0.0f),
	m_fMid(0.0f),
	m_fUp(0.0f),
	m_dwLowColor(RGB(0, 0, 255)),
	m_dwMidColor(RGB(0, 255, 0)),
	m_dwUpColor(RGB(255, 0, 0))
{
}

CHeightMap3D::~CHeightMap3D()
{
	Clear();
}

void CHeightMap3D::Clear()
{
	if (m_pCopyMap)
	{
		delete m_pCopyMap;
		m_pCopyMap = nullptr;
	}

	if (m_pHeightMap)
	{
		delete m_pHeightMap;
		m_pHeightMap = nullptr;
	}

	if (m_pHeightColorMap)
	{
		delete m_pHeightColorMap;
		m_pHeightColorMap = nullptr;
	}
}

void CHeightMap3D::Reset()
{
	if (m_pHeightMap)
	{
		delete m_pHeightMap;
		m_pHeightMap = nullptr;
	}

	if (m_pHeightColorMap)
	{
		delete m_pHeightColorMap;
		m_pHeightColorMap = nullptr;
	}
}

void CHeightMap3D::SetMap(CRavidImage * pSrcMap)
{
	if (!this->m_pCopyMap)
		m_pCopyMap = new CRavidImage;

	m_pCopyMap->Copy(pSrcMap);
}

void CHeightMap3D::Convert()
{
	Reset();

	do 
	{
		CRavidImage* pSrcMap = this->m_pCopyMap;
		if (!pSrcMap)
			break;

		if (!this->m_pHeightMap)
			m_pHeightMap = new CRavidImage;

		m_pHeightMap->Copy(pSrcMap);

		const int width = (int)pSrcMap->GetSizeX();
		const int height = (int)pSrcMap->GetSizeY();
		const int widthStep = pSrcMap->GetWidthStep();
		auto format = pSrcMap->GetValueFormat();
		CMultipleVariable mv;
		mv.AddValue(0);
		mv.AddValue(0);
		mv.AddValue(0);

		if (!m_pHeightColorMap)
			m_pHeightColorMap = new CRavidImage(pSrcMap->GetSizeX(), pSrcMap->GetSizeY(), mv, CRavidImage::MakeValueFormat(3, 8, CRavidImage::EValueType_Unsigned));

		switch (m_nUsedSetting)
		{
		case 0:	Convert_0();
			break;
		case 1:	Convert_1();
			break;
		case 2:	Convert_2();
			break;
		case 3:	Convert_3();
			break;
		case 4:	Convert_4();
			break;
		case 5:	Convert_5();
			break;
		default:
			break;
		}

	} 
	while(false);


}

float CHeightMap3D::GetHeight(int x, int y)
{
	float height = (float)sqrt(-1);

	CRavidImage* pMap = this->m_pHeightMap;
	do 
	{
		if (!pMap)
			break;

		if (x < 0 || y < 0)
			break;

		if (x >= pMap->GetSizeX() || y >= pMap->GetSizeY())
			break;

		BYTE** ppBufferY = pMap->GetYOffsetTable();
		size_t* pPositionX = pMap->GetXOffsetTable();

		height = *(float*)(ppBufferY[y] + pPositionX[x]);
	} 
	while(false);
	
	return height;
}

bool CHeightMap3D::GetHeight(int x, int y, float & resHeight)
{
	bool bReturn = false;

	CRavidImage* pMap = this->m_pHeightMap;

	do 
	{
		if (!pMap)
			break;

		if (x < 0 || y < 0)
			break;

		if (x >= pMap->GetSizeX() || y >= pMap->GetSizeY())
			break;

		BYTE** ppBufferY = pMap->GetYOffsetTable();
		size_t* pPositionX = pMap->GetXOffsetTable();

		resHeight = *(float*)(ppBufferY[y] + pPositionX[x]);

		bReturn = true;
	}
	while(false);

	return bReturn;
}

CRavidImage * CHeightMap3D::GetSourceMap()
{
	return m_pCopyMap;
}

CRavidImage * CHeightMap3D::GetHeightMap()
{
	return m_pHeightMap;
}

CRavidImage * CHeightMap3D::GetColorSudoMap()
{
	return m_pHeightColorMap;
}

int CHeightMap3D::GetCount()
{
	return 5;
}

void CHeightMap3D::Convert_0()
{
	do
	{
		const int width = (int)m_pHeightMap->GetSizeX();
		const int height = (int)m_pHeightMap->GetSizeY();

		BYTE** ppSrc = m_pHeightMap->GetYOffsetTable();
		size_t* pSrc = m_pHeightMap->GetXOffsetTable();

		BYTE** ppDst = m_pHeightColorMap->GetYOffsetTable();
		size_t* pDst = m_pHeightColorMap->GetXOffsetTable();

		double dblMin = INT_MAX;
		double dblMax = INT_MIN;

		for (auto y = 0; y < height; ++y)
		{
			for (auto x = 0; x < width; ++x)
			{
				float* pBuffer = (float*)(ppSrc[y] + pSrc[x]);
				if (*pBuffer <= m_fInvalidValue)
					continue;

				dblMin = __min(dblMin, *pBuffer);
				dblMax = __max(dblMax, *pBuffer);
			}
		}

		double average = dblMax + dblMin;
		average *= 0.5;
		double distance = dblMax - average;

		for (auto y = 0; y < height; ++y)
		{
			for (auto x = 0; x < width; ++x)
			{
				float* pBuffer = (float*)(ppSrc[y] + pSrc[x]);

				if (*pBuffer <= m_fInvalidValue)
					continue;

				if (*pBuffer < average)
				{
					double rate = *pBuffer - dblMin;
					rate /= distance;

					*(ppDst[y] + pDst[x] + 0) = (BYTE)__max(0, __min(255, (255) * (1.0 - rate)));
					*(ppDst[y] + pDst[x] + 1) = (BYTE)__max(0, __min(255, (255) * (rate)));
					*(ppDst[y] + pDst[x] + 2) = 0;

					continue;
				}
				if (*pBuffer >= average)
				{
					double rate = *pBuffer - average;
					rate /= distance;

					*(ppDst[y] + pDst[x] + 0) = 0;
					*(ppDst[y] + pDst[x] + 1) = (BYTE)__max(0, __min(255, (255) * (1.0 - rate)));
					*(ppDst[y] + pDst[x] + 2) = (BYTE)__max(0, __min(255, (255) * (rate)));

					continue;
				}
			}
		}
	}
	while (false);
}

void CHeightMap3D::Convert_1()
{
	do
	{
		const int width = (int)m_pHeightMap->GetSizeX();
		const int height = (int)m_pHeightMap->GetSizeY();

		BYTE** ppSrc = m_pHeightMap->GetYOffsetTable();
		size_t* pSrc = m_pHeightMap->GetXOffsetTable();

		BYTE** ppDst = m_pHeightColorMap->GetYOffsetTable();
		size_t* pDst = m_pHeightColorMap->GetXOffsetTable();

		for (auto y = 0; y < height; ++y)
		{
			for (auto x = 0; x < width; ++x)
			{
				float* pBuffer = (float*)(ppSrc[y] + pSrc[x]);
				if (*pBuffer <= m_fInvalidValue)
					continue;

				if ((m_fLow > *pBuffer))
				{
					*(ppDst[y] + pDst[x] + 0) = (BYTE)GetBValue(m_dwLowColor);
					*(ppDst[y] + pDst[x] + 1) = (BYTE)GetGValue(m_dwLowColor);
					*(ppDst[y] + pDst[x] + 2) = (BYTE)GetRValue(m_dwLowColor);
					continue;
				}
				if ((m_fLow <= *pBuffer) && (*pBuffer < m_fMid))
				{
					double distance = m_fMid - m_fLow;
					double rate = *pBuffer - m_fLow;
					rate /= distance;

					*(ppDst[y] + pDst[x] + 0) = (BYTE)((GetBValue(m_dwMidColor) - GetBValue(m_dwLowColor)) * (1.0 - rate));
					*(ppDst[y] + pDst[x] + 1) = (BYTE)((GetGValue(m_dwMidColor) - GetGValue(m_dwLowColor)) * (1.0 - rate));
					*(ppDst[y] + pDst[x] + 2) = (BYTE)((GetRValue(m_dwMidColor) - GetRValue(m_dwLowColor)) * (1.0 - rate));
					continue;
				}
				if ((m_fMid <= *pBuffer) && (*pBuffer < m_fUp))
				{
					double distance = m_fUp - m_fMid;
					double rate = *pBuffer - m_fMid;
					rate /= distance;

					*(ppDst[y] + pDst[x] + 0) = (BYTE)((GetBValue(m_dwUpColor) - GetBValue(m_dwMidColor)) * (1.0 - rate));
					*(ppDst[y] + pDst[x] + 1) = (BYTE)((GetGValue(m_dwUpColor) - GetGValue(m_dwMidColor)) * (1.0 - rate));
					*(ppDst[y] + pDst[x] + 2) = (BYTE)((GetRValue(m_dwUpColor) - GetRValue(m_dwMidColor)) * (1.0 - rate));
					continue;
				}
				if ((m_fUp <= *pBuffer))
				{
					*(ppDst[y] + pDst[x] + 0) = (BYTE)GetBValue(m_dwUpColor);
					*(ppDst[y] + pDst[x] + 1) = (BYTE)GetGValue(m_dwUpColor);
					*(ppDst[y] + pDst[x] + 2) = (BYTE)GetRValue(m_dwUpColor);
					continue;
				}
			}
		}

	}
	while (false);
}

void CHeightMap3D::Convert_2()
{
	do
	{
		const int width = (int)m_pHeightMap->GetSizeX();
		const int height = (int)m_pHeightMap->GetSizeY();

		BYTE** ppSrc = m_pHeightMap->GetYOffsetTable();
		size_t* pSrc = m_pHeightMap->GetXOffsetTable();

		BYTE** ppDst = m_pHeightColorMap->GetYOffsetTable();
		size_t* pDst = m_pHeightColorMap->GetXOffsetTable();

		double dblMin = INT_MAX;
		double dblMax = INT_MIN;

		for (auto y = 0; y < height; ++y)
		{
			for (auto x = 0; x < width; ++x)
			{
				float* pBuffer = (float*)(ppSrc[y] + pSrc[x]);
				if (*pBuffer <= m_fInvalidValue)
					continue;

				dblMin = __min(dblMin, *pBuffer);
				dblMax = __max(dblMax, *pBuffer);
			}
		}

		double average = dblMax + dblMin;
		average *= 0.5;
		double distance = dblMax - average;

		for (auto y = 0; y < height; ++y)
		{
			for (auto x = 0; x < width; ++x)
			{
				float* pBuffer = (float*)(ppSrc[y] + pSrc[x]);

				if (*pBuffer <= m_fInvalidValue)
					continue;

				if ((m_fLow > *pBuffer))
				{
					*(ppDst[y] + pDst[x] + 0) = (BYTE)GetBValue(m_dwLowColor);
					*(ppDst[y] + pDst[x] + 1) = (BYTE)GetGValue(m_dwLowColor);
					*(ppDst[y] + pDst[x] + 2) = (BYTE)GetRValue(m_dwLowColor);
					continue;
				}
				if ((m_fUp <= *pBuffer))
				{
					*(ppDst[y] + pDst[x] + 0) = (BYTE)GetBValue(m_dwUpColor);
					*(ppDst[y] + pDst[x] + 1) = (BYTE)GetGValue(m_dwUpColor);
					*(ppDst[y] + pDst[x] + 2) = (BYTE)GetRValue(m_dwUpColor);
					continue;
				}

				if (*pBuffer < average)
				{
					double rate = *pBuffer - dblMin;
					rate /= distance;

					*(ppDst[y] + pDst[x] + 0) = (BYTE)__max(0, __min(255, (255) * (1.0 - rate)));
					*(ppDst[y] + pDst[x] + 1) = (BYTE)__max(0, __min(255, (255) * (rate)));
					*(ppDst[y] + pDst[x] + 2) = 0;

					continue;
				}
				if (*pBuffer >= average)
				{
					double rate = *pBuffer - average;
					rate /= distance;

					*(ppDst[y] + pDst[x] + 0) = 0;
					*(ppDst[y] + pDst[x] + 1) = (BYTE)__max(0, __min(255, (255) * (1.0 - rate)));
					*(ppDst[y] + pDst[x] + 2) = (BYTE)__max(0, __min(255, (255) * (rate)));

					continue;
				}
			}
		}
	}
	while (false);
}

void CHeightMap3D::Convert_3()
{
	do
	{
		const int width = (int)m_pHeightMap->GetSizeX();
		const int height = (int)m_pHeightMap->GetSizeY();

		BYTE** ppSrc = m_pHeightMap->GetYOffsetTable();
		size_t* pSrc = m_pHeightMap->GetXOffsetTable();

		BYTE** ppDst = m_pHeightColorMap->GetYOffsetTable();
		size_t* pDst = m_pHeightColorMap->GetXOffsetTable();

		double dblMin = INT_MAX;
		double dblMax = INT_MIN;

		for (auto y = 0; y < height; ++y)
		{
			for (auto x = 0; x < width; ++x)
			{
				float* pBuffer = (float*)(ppSrc[y] + pSrc[x]);
				if (*pBuffer <= m_fInvalidValue)
					continue;

				dblMin = __min(dblMin, *pBuffer);
				dblMax = __max(dblMax, *pBuffer);
			}
		}

		double average = dblMax + dblMin;
		average *= 0.5;
		double distance = dblMax - average;

		for (auto y = 0; y < height; ++y)
		{
			for (auto x = 0; x < width; ++x)
			{
				float* pBuffer = (float*)(ppSrc[y] + pSrc[x]);

				if (*pBuffer <= m_fInvalidValue)
					continue;

				if ((m_fLow > *pBuffer))
				{
					*(ppDst[y] + pDst[x] + 0) = GetBValue(m_dwLowColor);
					*(ppDst[y] + pDst[x] + 1) = GetGValue(m_dwLowColor);
					*(ppDst[y] + pDst[x] + 2) = GetRValue(m_dwLowColor);
					continue;
				}
				if ((m_fUp <= *pBuffer))
				{
					*(ppDst[y] + pDst[x] + 0) = GetBValue(m_dwUpColor);
					*(ppDst[y] + pDst[x] + 1) = GetGValue(m_dwUpColor);
					*(ppDst[y] + pDst[x] + 2) = GetRValue(m_dwUpColor);
					continue;
				}
				if ((m_fLow <= *pBuffer) && (*pBuffer < m_fUp))
				{
					double distance = m_fUp - m_fLow;
					double rate = *pBuffer - m_fLow;
					rate /= distance;

					*(ppDst[y] + pDst[x] + 0) = (BYTE)(255 * rate);
					*(ppDst[y] + pDst[x] + 1) = (BYTE)(255 * rate);
					*(ppDst[y] + pDst[x] + 2) = (BYTE)(255 * rate);
					continue;
				}
			}
		}
	}
	while (false);
}

void CHeightMap3D::Convert_4()
{
	do
	{
		const int width = (int)m_pHeightMap->GetSizeX();
		const int height = (int)m_pHeightMap->GetSizeY();

		BYTE** ppSrc = m_pHeightMap->GetYOffsetTable();
		size_t* pSrc = m_pHeightMap->GetXOffsetTable();

		BYTE** ppDst = m_pHeightColorMap->GetYOffsetTable();
		size_t* pDst = m_pHeightColorMap->GetXOffsetTable();

		double dblMin = INT_MAX;
		double dblMax = INT_MIN;

		for (auto y = 0; y < height; ++y)
		{
			for (auto x = 0; x < width; ++x)
			{
				float* pBuffer = (float*)(ppSrc[y] + pSrc[x]);
				if (*pBuffer <= m_fInvalidValue)
					continue;

				dblMin = __min(dblMin, *pBuffer);
				dblMax = __max(dblMax, *pBuffer);
			}
		}

		double average = dblMax + dblMin;
		average *= 0.5;
		double distance = dblMax - average;

		for (auto y = 0; y < height; ++y)
		{
			for (auto x = 0; x < width; ++x)
			{
				float* pBuffer = (float*)(ppSrc[y] + pSrc[x]);

				if (*pBuffer <= m_fInvalidValue)
					continue;

				if ((m_fLow <= *pBuffer) && (*pBuffer < m_fUp))
				{
					double distance = m_fUp - m_fLow;
					double rate = *pBuffer - m_fLow;
					rate /= distance;

					*(ppDst[y] + pDst[x] + 0) = (BYTE)(255 * rate);
					*(ppDst[y] + pDst[x] + 1) = (BYTE)(255 * rate);
					*(ppDst[y] + pDst[x] + 2) = (BYTE)(255 * rate);
					continue;
				}

				if (*pBuffer < average)
				{
					double rate = *pBuffer - dblMin;
					rate /= distance;

					*(ppDst[y] + pDst[x] + 0) = (BYTE)__max(0, __min(255, (255) * (1.0 - rate)));
					*(ppDst[y] + pDst[x] + 1) = (BYTE)__max(0, __min(255, (255) * (rate)));
					*(ppDst[y] + pDst[x] + 2) = 0;

					continue;
				}
				if (*pBuffer >= average)
				{
					double rate = *pBuffer - average;
					rate /= distance;

					*(ppDst[y] + pDst[x] + 0) = 0;
					*(ppDst[y] + pDst[x] + 1) = (BYTE)__max(0, __min(255, (255) * (1.0 - rate)));
					*(ppDst[y] + pDst[x] + 2) = (BYTE)__max(0, __min(255, (255) * (rate)));

					continue;
				}
			}
		}
	}
	while (false);
}

void CHeightMap3D::Convert_5()
{
	do
	{
		const int width = (int)m_pHeightMap->GetSizeX();
		const int height = (int)m_pHeightMap->GetSizeY();

		BYTE** ppSrc = m_pHeightMap->GetYOffsetTable();
		size_t* pSrc = m_pHeightMap->GetXOffsetTable();

		BYTE** ppDst = m_pHeightColorMap->GetYOffsetTable();
		size_t* pDst = m_pHeightColorMap->GetXOffsetTable();

		double dblMin = INT_MAX;
		double dblMax = INT_MIN;

		for (auto y = 0; y < height; ++y)
		{
			for (auto x = 0; x < width; ++x)
			{
				float* pBuffer = (float*)(ppSrc[y] + pSrc[x]);
				if (*pBuffer <= m_fInvalidValue)
					continue;

				dblMin = __min(dblMin, *pBuffer);
				dblMax = __max(dblMax, *pBuffer);
			}
		}

		double average = dblMax + dblMin;
		average *= 0.5;
		double distance = dblMax - dblMin;

		for (auto y = 0; y < height; ++y)
		{
			for (auto x = 0; x < width; ++x)
			{
				float* pBuffer = (float*)(ppSrc[y] + pSrc[x]);

				if (*pBuffer <= m_fInvalidValue)
					continue;

				double rate = *pBuffer - dblMin;
				rate /= distance;

				*(ppDst[y] + pDst[x] + 0) = (BYTE)__max(0, __min(255, (255) * (1.0 - rate)));
				*(ppDst[y] + pDst[x] + 1) = 0;
				*(ppDst[y] + pDst[x] + 2) = (BYTE)__max(0, __min(255, (255) * (rate)));
			}
		}
	}
	while (false);
}
