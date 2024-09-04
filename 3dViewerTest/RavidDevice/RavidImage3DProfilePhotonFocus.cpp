#include "stdafx.h"

#include "RavidImage3DProfilePhotonFocus.h"

#include "../RavidFramework/LogManager.h"
#include "../RavidFramework/DeviceManager.h"
#include "../RavidFramework/LibraryManager.h"
#include "../RavidFramework/SequenceManager.h"
#include "../RavidFramework/AuthorityManager.h"
#include "../RavidFramework/FrameworkManager.h"
#include "../RavidFramework/EventHandlerManager.h"
#include "../RavidFramework/MultiLanguageManager.h"

using namespace Ravid;
using namespace Ravid::Device;
using namespace Ravid::Database;
using namespace Ravid::Framework;
using namespace Ravid::Algorithms;

CRavidImage3DProfilePhotonFocus::CRavidImage3DProfilePhotonFocus()
{
}

CRavidImage3DProfilePhotonFocus::~CRavidImage3DProfilePhotonFocus()
{
	Terminate();
}

void CRavidImage3DProfilePhotonFocus::Initialize(size_t st3DSizeX, size_t st3DSizeY)
{
//	__super::Initialize(st3DSizeX, st3DSizeY);

	if(m_pcPeak)
	{
		delete[] m_pcPeak;
		m_pcPeak = nullptr;
	}

	if(m_pcWidth)
	{
		delete[] m_pcWidth;
		m_pcWidth = nullptr;
	}

	if(m_pcQuality)
	{
		delete[] m_pcQuality;
		m_pcQuality = nullptr;
	}

	if(!m_pcPeak)
		m_pcPeak = new BYTE[st3DSizeX * st3DSizeY];

	if(!m_pcWidth)
		m_pcWidth = new BYTE[st3DSizeX * st3DSizeY];

	if(!m_pcQuality)
		m_pcQuality = new BYTE[st3DSizeX * st3DSizeY];
}

void CRavidImage3DProfilePhotonFocus::Terminate()
{
	if(m_pcPeak)
	{
		delete[] m_pcPeak;
		m_pcPeak = nullptr;
	}

	if(m_pcWidth)
	{
		delete[] m_pcWidth;
		m_pcWidth = nullptr;
	}

	if(m_pcQuality)
	{
		delete[] m_pcQuality;
		m_pcQuality = nullptr;
	}

//	__super::Terminate();
}

void CRavidImage3DProfilePhotonFocus::UpdateProfile()
{
	do 
	{
// 		if(!GetBuffer())
// 			break;
// 
// 		if(!GetSizeX())
// 			break;
// 
// 		ptrdiff_t nWidth = GetSizeX();
// 		ptrdiff_t nHeight = GetSizeY();
// 
// 		BYTE* pSrcInfo = GetBuffer();
// 		
// 		pSrcInfo += Get3DOffset();
// 
// 		for(size_t x = 0; x < nWidth; ++x)
// 		{
// 			BYTE* pPixel = pSrcInfo + x;
// 
// 			if(!(*pPixel))
// 				m_pfPeak[x] = 0.;
// 			else
// 				m_pfPeak[x] = ((float)(*pPixel << 8) + *(pPixel + nWidth)) / 64.;
// 			
// 			m_pcPeak[x] = *pPixel;
// 			m_pcWidth[x] = *(pPixel + (2 * nWidth));
// 			m_pcQuality[x] = *(pPixel + (3 * nWidth)) >> 4;
// 		}
	} 
	while(false);
}

BYTE CRavidImage3DProfilePhotonFocus::GetPeakInByte(int nIndex)
{
	BYTE cReturn = 0;

// 	do 
// 	{
// 		if(nIndex >= m_st3DSizeX)
// 			break;
// 
// 		cReturn = *(m_pcPeak + nIndex);
// 	} 
// 	while(false);

	return cReturn;
}

BYTE CRavidImage3DProfilePhotonFocus::GetWidth(int nIndex)
{
	BYTE cReturn = 0;

// 	do
// 	{
// 		if(nIndex >= m_st3DSizeX)
// 			break;
// 		
// 		cReturn = *(m_pcWidth + nIndex);
// 	}
// 	while(false);

	return cReturn;
}

BYTE CRavidImage3DProfilePhotonFocus::GetQuality(int nIndex)
{
	BYTE cReturn = 0;

// 	do
// 	{
// 		if(nIndex >= m_st3DSizeX)
// 			break;
// 
// 		cReturn = *(m_pcQuality + nIndex);
// 	}
// 	while(false);

	return cReturn;
}

BYTE* CRavidImage3DProfilePhotonFocus::GetPeak()
{
	return m_pcPeak;
}

BYTE* CRavidImage3DProfilePhotonFocus::GetWidth()
{
	return m_pcWidth;
}

BYTE* CRavidImage3DProfilePhotonFocus::GetQuality()
{
	return m_pcQuality;
}

void CRavidImage3DProfilePhotonFocus::Set3DOffset(ptrdiff_t pdtOffset)
{
	m_pdt3DOffset = pdtOffset;
}

ptrdiff_t CRavidImage3DProfilePhotonFocus::Get3DOffset()
{
	return m_pdt3DOffset;
}
