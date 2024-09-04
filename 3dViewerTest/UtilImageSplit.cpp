#include "stdafx.h"
#include "UtilImageSplit.h"
#include "UtilCommFuncFile.h"


CUtilImageSplit::CUtilImageSplit()
{
	StartSaveImageThread();
}

CUtilImageSplit::~CUtilImageSplit()
{
	StopSaveImageThread();
}




void CUtilImageSplit::SetSplitSize(int nImageSizeX, int nImageSizeY, int nSplitSizeX, int nSplitSizeY)
{
	m_nImageSizeX = nImageSizeX;
	m_nImageSizeY = nImageSizeY;
	m_nSplitSizeX = nSplitSizeX;
	m_nSplitSizeY = nSplitSizeY;
}


bool CUtilImageSplit::SaveImage(CRavidImage* pImage, CString strPath, CString strFileName, CString strDataFormat, CRavidPoint<double> rpCuttingCenter)
{	

	SImageData sData;
	sData.riImage = new CRavidImage();
	sData.riImage->Copy(pImage);
	sData.strDataFormat = strDataFormat;
	sData.strPath = strPath;
	sData.strFileName = strFileName;
	sData.rpCuttingCenter = rpCuttingCenter;


	m_csQueue.Lock();

	CUtilCommFuncFile cff;
	cff.MakeDirectory(strPath);

	m_qeueuImage.push(sData);

	m_csQueue.Unlock();

	return true;
}

void CUtilImageSplit::StartSaveImageThread()
{
	if(m_bSaveImageThread)
		return;
	m_bSaveImageThread = true;

	CreateRavidThreadContext(pContext)
	{
		CLogManager::Write(0, _T("[Image Thread] Start Save Image Thread"));
		CUtilCommFuncFile cff;

		while(m_bSaveImageThread)
		{
			Sleep(10);

			m_csQueue.Lock();
			if(!m_qeueuImage.size())
			{	
				m_csQueue.Unlock();
				continue;
			}

			SImageData sData = m_qeueuImage.front();
			m_qeueuImage.pop();
			m_csQueue.Unlock();

			CLogManager::Write(0, _T("[Image Thread] Start Save Image"));


			if(!sData.riImage || !sData.riImage->GetBuffer())
			{
				CLogManager::Write(0, _T("[Image Thread] Start Save NG Image Buffer Fail"));
				continue;
			}


			int nCountX = m_nImageSizeX / m_nSplitSizeX;
			int nCountY = m_nImageSizeY / m_nSplitSizeY;

			int nSizeX = m_nSplitSizeX;// + (4 - (m_nSplitSizeX % 4));
			int nSizeY = m_nSplitSizeY;// + (4 - (m_nSplitSizeY % 4));

			CRavidRect<double> rrROI;
			rrROI.top = 0;
			rrROI.bottom = nSizeY;

			if(sData.rpCuttingCenter.x == 0 && sData.rpCuttingCenter.y == 0)
			{
				rrROI.left = 0;
				rrROI.right = nSizeX;
			}
			else
			{
				rrROI.left = sData.rpCuttingCenter.x - (nSizeX / 2);
				rrROI.right = sData.rpCuttingCenter.x + (nSizeX / 2);

				int nOffset = 0;
				if(rrROI.left < 0)
				{
					nOffset = fabs(rrROI.left);
					rrROI.left = 0;
					rrROI.right += nOffset;
				}
				else if(4096 < rrROI.right)
				{
					nOffset = rrROI.right - 4096;
					rrROI.left += nOffset;
					rrROI.right = 4096;
				}
			}
			CUtilCommFuncFile cff;
			cff.MakeDirectory(sData.strPath);

			CRavidImage riTech;
			if(riTech.CreateImageBuffer(nSizeX, nSizeY, 255, CRavidImage::EValueFormat_1C_U8, 0, 8) != EAlgorithmResult_OK)
			{
				CLogManager::Write(0, _T("[Image Thread] Create Image Buffer Fail"));
				return;
			}

			int nImageCount = 0;
			for(int i = 0; i < nCountX; i++)
			{
				for(int j = 0; j < nCountY; j++)
				{
					if(sData.riImage->Paste(riTech, &rrROI) != EAlgorithmResult_OK)
					{
						CLogManager::Write(0, _T("[Image Thread] Teaching for Image Paste-2 Fail"));
						return;
					}

					CString strSaveData;

					if(nCountX == 1 && nCountY == 1)
						strSaveData.Format(_T("%s%s.%s"), sData.strPath, sData.strFileName, sData.strDataFormat);
					else
						strSaveData.Format(_T("%s%s_%d.%s"), sData.strPath, sData.strFileName, nImageCount, sData.strDataFormat);

					//CString strSaveDataTest;
					//strSaveDataTest.Format(_T("%s%s.bmp"), sData.strPath, sData.strFileName);
					//sData.riImage->Save(strSaveDataTest);
					EAlgorithmResult eSaveResult = riTech.CompressionSave(strSaveData, 85);


					if(eSaveResult != EAlgorithmResult_OK)
						CLogManager::Write(0, _T("[Image Thread] Save Image Fail %d"), eSaveResult);
					

					rrROI.top = rrROI.bottom + 1;
					rrROI.bottom = rrROI.top + nSizeY;

					nImageCount++;
				}
			}

			if(sData.riImage != nullptr)
			{
				delete sData.riImage;
				sData.riImage = nullptr;
			}

		}
	};
	CThreadHelper::Run(pContext);

	m_pSaveImageThread = pContext;
}


void CUtilImageSplit::StopSaveImageThread()
{
	if(!m_bSaveImageThread)
		return;

	if(m_pSaveImageThread)
	{
		m_bSaveImageThread = false;
		if(m_pSaveImageThread)
		{
			//CThreadHelper::Wait(m_pSaveImageThread, 1500);
			WaitForSingleObject(m_pSaveImageThread, 5000);
		}
	}
}


void CUtilImageSplit::OpenImage(CRavidImage* pImage, CString strPath, CString strFileName, CString strDataFormat, CRavidPoint<double> rpCuttingCenter)
{

	CRavidImage riSplit;


	int nSizeX = m_nSplitSizeX + (4 - (m_nSplitSizeX % 4));
	int nSizeY = m_nSplitSizeY + (4 - (m_nSplitSizeY % 4));

	if(riSplit.CreateImageBuffer(nSizeX, nSizeY , 255, CRavidImage::EValueFormat_1C_U8, 0, 8) != EAlgorithmResult_OK)
	{
		CLogManager::Write(0, _T("Create Image Buffer Fail"));
		return;
	}

	int nCountX = m_nImageSizeX / m_nSplitSizeX;
	int nCountY = m_nImageSizeY / m_nSplitSizeY;

	int nCount = 0;


	//붙여넣을땐 왜 또 사이즈를 1개씩줄여야되나.. 거지같은
	int nPasteSizeX = m_nSplitSizeX - 1;
	int nPasteSizeY = m_nSplitSizeY - 1;

	CRavidRect<int> rrROI;


	rrROI.left = rpCuttingCenter.x - 512;
	rrROI.top = 0;
	rrROI.right = rpCuttingCenter.x + 512;
	rrROI.bottom = nSizeY;

	int nOffset = 0;
	if(rrROI.left < 0)
	{
		nOffset = fabs(rrROI.left);
		rrROI.left = 0;
		rrROI.right += nOffset;
	}
	else if(4096 < rrROI.right)
	{
		nOffset = rrROI.right - 4096;
		rrROI.left += nOffset;
		rrROI.right = 4096;
	}

	CRavidRect<int> rrSrcROI = rrROI;

	//for(int i = 0; i < nCountX; i++)
	{
		for(int j = 0; j < nCountY; j++)
		{

			CString strImageData;
			strImageData.Format(_T("%s\\%s_%d.%s"), strPath, strFileName, nCount, strDataFormat);
			EAlgorithmResult loadResult =  riSplit.Load(strImageData);

			//riSplit.Paste(pImage);
			EAlgorithmResult fdsa = riSplit.Paste(pImage, &rrSrcROI, &rrROI);



			rrROI.top = rrROI.bottom + 1;
			rrROI.bottom = rrROI.top + nSizeY;
			nCount++; 
		}
	}


}

