#include "stdafx.h"
#include "UtilLog.h"
#include <algorithm>
#include "Def.h"
#include "UtilCommFuncFile.h"
#include "UtilTime.h"

//////////////////////////////////////////////////////////////////////////
// 싱글톤 변수 초기화
CUtilLog*				CUtilLog::instance = nullptr;
ThreadContext*			CUtilLog::m_pThreadDeleteFile = nullptr;
volatile bool			CUtilLog::m_bIsThreadDeleteFile = false;
volatile ULONG			CUtilLog::m_ulExpirationDate = 180;
volatile bool			CUtilLog::m_bRecursive = true;
volatile bool			CUtilLog::m_bRemoveFileAttribute = false;
volatile bool			CUtilLog::m_bLimitDriveVolume = false;
volatile float			CUtilLog::m_fDriveVolumeLimitPercentage = 80.;
std::vector<CString>	CUtilLog::m_vctPathList;
CCriticalSection		CUtilLog::m_csDeleteLock;
std::queue<sSaveImage>	CUtilLog::m_queImageData;
CCriticalSection		CUtilLog::m_csImageLock;
std::queue<sSaveLog>	CUtilLog::m_queCsvData;
ThreadContext*			CUtilLog::m_pThreadSaveImage	= nullptr;
volatile bool			CUtilLog::m_bThreadSaveImage	= false;
ThreadContext*			CUtilLog::m_pThreadSaveLog		= nullptr;
volatile bool			CUtilLog::m_bThreadSaveLog		= false;
CCriticalSection		CUtilLog::m_csDataSaveLock;
CString					CUtilLog::m_strParamerLogFolder = _T("D:\\Vision Log\\Model");
//////////////////////////////////////////////////////////////////////////

CUtilLog::CUtilLog(void)
{
	Initialize();
}


CUtilLog::~CUtilLog(void)
{
	Terminate();
}

CUtilLog * CUtilLog::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new CUtilLog();
		//atexit(DestroyInstance);
	}
	return instance;
}

void CUtilLog::DestroyInstance()
{
	if (instance != nullptr)
		delete instance;
}


bool CUtilLog::Initialize()
{
	bool bReturn = true;
	setlocale(LC_ALL, "korean");
	do 
	{
#ifndef _DEBUG 
		bReturn &= StartDeleteThread();
#endif
		bReturn &= StartSaveImageThread();
		bReturn &= StartSaveLogThread();
	} while (false);
	return bReturn;
}

bool CUtilLog::Terminate()
{
	bool bReturn = true;
	do
	{
		bReturn &= StopDeleteThread();
		bReturn &= StopSaveImageThread();
		bReturn &= StopSaveLogThread();
	}
	while(false);
	return bReturn;
}

bool CUtilLog::AddDeletingFolder(LPCTSTR lpszPath)
{
	bool bReturn = false;

	if(!lpszPath || !_tcslen(lpszPath))
		return bReturn;

	m_csDeleteLock.Lock();
	m_vctPathList.push_back(lpszPath);
	m_csDeleteLock.Unlock();

	bReturn = true;

	return bReturn;
}

double CUtilLog::GetUsedDriverPercentage(LPCTSTR lpszDriver)
{
	double dblReturn = 0.;

	do 
	{
		if(!lpszDriver)
			break;

		ULARGE_INTEGER  ulnFreeBytesAvailableToCaller, ulnTotalNumberOfBytes, ulnTotalNumberOfFreeBytes;

		double dblTotalSize, dblFreeSize, dblCurrentPercentage;

		if(!GetDiskFreeSpaceEx(lpszDriver, &ulnFreeBytesAvailableToCaller, &ulnTotalNumberOfBytes, &ulnTotalNumberOfFreeBytes))
			break;

		dblTotalSize = (double)ulnTotalNumberOfBytes.QuadPart / 1073741824.;
		dblFreeSize = (double)ulnFreeBytesAvailableToCaller.QuadPart / 1073741824.;
		dblCurrentPercentage = ((dblTotalSize - dblFreeSize) / dblTotalSize) * 100.;

		dblReturn = dblCurrentPercentage;
	}
		while(false);

	return dblReturn;
}

CString CUtilLog::GetDriverName(LPCTSTR lpszFile)
{
	CString strReturn;
	strReturn.Empty();

	do 
	{
		if(!lpszFile)
			break;

		CString strFile = lpszFile;

		int nPos = strFile.Find(_T("\\"));

		if(nPos < 0)
			break;

		if(nPos + 1 >= strFile.GetLength())
			break;

		strReturn = strFile.Left(nPos + 1);
	}
		while(false);

	return strReturn;
}

bool CUtilLog::StartDeleteThread()
{
	bool bReturn = false;
	do 
	{
		if(m_bIsThreadDeleteFile)
			break;

		m_bIsThreadDeleteFile = true;
		
		CreateRavidThreadContext(pContext)
		{

			std::set<CString> setCleanDriver;

			while(m_bIsThreadDeleteFile)
			{
				Sleep(1);
				std::vector<CString> vctPath;
				m_csDeleteLock.Lock();
				vctPath = *GetPathList();
				m_csDeleteLock.Unlock();

				if(IsLimitDriveVolume())
				{
					for(auto iter = vctPath.begin(); iter != vctPath.end(); ++iter)
					{
						CString strDriver = GetDriverName(*iter);

						if(strDriver.IsEmpty())
							continue;

						if(GetUsedDriverPercentage(strDriver) >= GetDriveVolumeLimitPercentage())
							setCleanDriver.insert(strDriver);
					}
				}


				std::function<bool(CString)> rmdRecursive = [&](CString strPath) -> bool
				{
					CString strFindFile, strTemp, strBuffer;
					CFileFind ff;
					CTime currentTime;
					CTime tm;
					CTimeSpan tmSpan;
					bool bFind;

					strFindFile.Format(_T("%s\\*.*"), strPath);
					bFind = ff.FindFile(strFindFile);

					currentTime = CTime::GetCurrentTime();

					while(bFind)
					{
						bFind = ff.FindNextFile();

						if(ff.IsDots()) continue;
						if(ff.IsDirectory())
						{
							if(!IsRecursive())
								continue;

							CString strNextLevel;
							strNextLevel = strFindFile;
							strNextLevel.Replace(_T("\\*.*"), _T("\\"));
							strNextLevel += ff.GetFileName();
							rmdRecursive(strNextLevel);
						}

						CString strFilePath = ff.GetFilePath();
						ff.GetCreationTime(tm);

						tmSpan = currentTime - tm;

						CString strDriver = GetDriverName(strFilePath);

						auto iter = setCleanDriver.find(strDriver);

						bool bIsClean = iter != setCleanDriver.end();


						if((tmSpan.GetDays() >= GetExpirationDate()) || bIsClean)
						{
							if(IsRemoveFileAttribute())
								SetFileAttributes(ff.GetFilePath(), FILE_ATTRIBUTE_NORMAL);

							::DeleteFile(strFilePath);
							::RemoveDirectory(strFilePath);

							if(bIsClean)
							{
								if(GetUsedDriverPercentage(strDriver) < GetDriveVolumeLimitPercentage())
									setCleanDriver.erase(iter);
							}
						}

						Sleep(1);
					}

					return 0;
				};

				std::for_each(vctPath.begin(), vctPath.end(), rmdRecursive);

				Sleep(1);
			};
			int k = 0;

		};

		CThreadHelper::Run(pContext);
		m_pThreadDeleteFile = pContext;
		bReturn = true;
	} while (false);

	return bReturn;
}


bool CUtilLog::StopDeleteThread()
{
	bool bReturn = false;
	do 
	{
		if(m_bIsThreadDeleteFile)
		{
			m_bIsThreadDeleteFile = false;

			if(m_pThreadDeleteFile)
			{
				CThreadHelper::Wait(m_pThreadDeleteFile, 1500);
			}
			
		}
		bReturn = true;
	} while (false);

	return bReturn;
}



bool CUtilLog::StartSaveImageThread()
{
	bool bReturn = false;
	do
	{
		if(m_bThreadSaveImage)
			break;
		
		m_bThreadSaveImage = true;

		CreateRavidThreadContext(pContext)
		{
			Sleep(1);
			CUtilCommFuncFile ucff;

			while (m_bThreadSaveImage)
			{
				sSaveImage sData;
				sData.pImgInfo = nullptr;

				m_csImageLock.Lock();
				if (m_queImageData.size())
				{
					sData = m_queImageData.front();
					m_queImageData.pop();
				}
				m_csImageLock.Unlock();

				if(sData.pImgInfo)
				{
					ucff.MakeDirectory(sData.strPath);

					if(sData.bCapture)
					{
						CDrawImage cdI(sData.pImgInfo);
						cdI.DrawingLayer(sData.pLayer);
					
// 							if(cdI.SaveDrawImage(sData.strPath))
// 							{
// 							}
							
					}
					else
					{
						if(sData.pImgInfo->Save(sData.strPath) == EAlgorithmResult_OK)
						{

						}
						else
						{

						}
					}
					if(sData.pLayer)
						delete sData.pLayer;
					delete sData.pImgInfo;
				}
			
				
			}
		};
		CThreadHelper::Run(pContext);

		m_pThreadSaveImage = pContext;

		bReturn = true;
	}
	while(false);

	return bReturn;
}


bool CUtilLog::StopSaveImageThread()
{
	bool bReturn = false;
	do
	{
		if(m_bThreadSaveImage)
		{
			m_bThreadSaveImage = false;

			if(m_pThreadSaveImage)
			{
				CThreadHelper::Wait(m_pThreadSaveImage, 1500);
			}
		}
		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CUtilLog::SaveImage(CRavidImage * pImgInfo, CString strPath)
{
	bool bReturn = false;
	do
	{
		if(!pImgInfo)
			break;

		SSaveImage sImgData;

		sImgData.pImgInfo = new CRavidImage(pImgInfo);
		sImgData.strPath = strPath;

		m_csImageLock.Lock();
		m_queImageData.push(sImgData);
		m_csImageLock.Unlock();

		bReturn = true;
	} while (false);
	return bReturn;
}

bool CUtilLog::SaveDrawImage(CRavidImage* pImgInfo, CRavidImageViewLayer* pLayer, CString strPath)
{
	bool bReturn = false;
	do
	{
		if(!pImgInfo)
			break;

		SSaveImage sImgData;
		sImgData.pImgInfo = new CRavidImage(pImgInfo);
		sImgData.strPath = strPath;
		sImgData.bCapture = true;
		sImgData.pLayer = new CRavidImageViewLayer;
		sImgData.pLayer->Copy(pLayer);

		m_csImageLock.Lock();
		m_queImageData.push(sImgData);
		m_csImageLock.Unlock();

		bReturn = true;
	}
	while(false);
	return bReturn;
}
bool CUtilLog::StartSaveLogThread()
{
	bool bReturn = false;
	do
	{
		if(m_bThreadSaveLog)
			break;

		m_bThreadSaveLog = true;

		CreateRavidThreadContext(pContext)
		{
			while (m_bThreadSaveLog)
			{
				Sleep(1);
				m_csDataSaveLock.Lock();
				if (m_queCsvData.size())
				{
					sSaveLog sData = m_queCsvData.front();
					m_queCsvData.pop();

					if(WriteCsvFile(sData))
					{

					}
					else
					{

					}
				}
				m_csDataSaveLock.Unlock();
			}
		};
		CThreadHelper::Run(pContext);
		m_pThreadSaveLog = pContext;
		bReturn = true;
	}
	while(false);

	return bReturn;
}


bool CUtilLog::StopSaveLogThread()
{
	bool bReturn = false;
	do
	{
		if(m_bThreadSaveLog)
		{
			m_bThreadSaveLog = false;

			if(m_pThreadSaveLog)
			{
				CThreadHelper::Wait(m_pThreadSaveLog, 1500);
			}
		}
		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CUtilLog::WriteCsvFile(const sSaveLog & sData)
{
	bool bReturn = false;
	do
	{

		CUtilCommFuncFile ucff;
		CUtilTime ut;

		CString strPath;

		strPath = sData.strPath;
		ucff.MakeDirectory(strPath);

		//////////////////////////////////////////////////////////////////////////
		//csv 저장 항목
		CString strLog;
		FILE* fp = nullptr;

		if (!ucff.IsFileExist(strPath))
		{
			_tfopen_s(&fp, strPath, _T("wt"));
			
			strLog = sData.strItem;
			strLog += _T("\n");
		}
		else
		{
			_tfopen_s(&fp, strPath, _T("at"));
		}
		if(!fp)
		{
			int nExtend = strPath.Find(_T(".csv"));

			if(nExtend < 0)
				break;
			strPath = strPath.Left(nExtend) + _T("_Temp.csv");
			if(!ucff.IsFileExist(strPath))
			{
				_tfopen_s(&fp, strPath, _T("wt"));
				strLog = sData.strItem;
				strLog += _T("\n");
			}
			else
				_tfopen_s(&fp, strPath, _T("at"));
		}

		strLog += sData.strLog;
		strLog += _T("\n");

		if(!fp)
			break;

		fputws(strLog, fp);
		fclose(fp);

		bReturn = true;

	} while (false);
	return bReturn;
}

bool CUtilLog::SaveCsvData(const CString & strItem, const CString & strData, const CString & strPath)
{
	bool bReturn = false;
	do
	{
		sSaveLog sData;

		sData.strItem = strItem;
		sData.strLog = strData;
		sData.strPath = strPath;

		m_csDataSaveLock.Lock();
		m_queCsvData.push(sData);
		m_csDataSaveLock.Unlock();

		bReturn = true;
	} while (false);
	return bReturn;
}

// 파라미터 변경 내용 저장

bool CUtilLog::SaveOriginalParamData(CModelInfo* pModel)
{
	bool bReturn = false;
	do
	{
		if(!pModel)
			break;

		SType0ModelParam sParam;
		pModel->GetData(&sParam, sizeof(sParam));

		//////////////////////////////////////////////////////////////////////////
		std::vector<SRavidParameterElement>* pEle = nullptr;
		pEle = pModel->GetParameterElements();
		if(!pEle)
			break;
		if(!pEle->size())
			break;

		CUtilCommFuncFile ucff;
		CUtilTime ut;
		CString strPath;
		strPath = GetParameterLogPath();
		int nPos = strPath.ReverseFind(_T('\\'));
		if(nPos > 0)
			strPath = strPath.Left(nPos);
		else
			strPath = _T("D:\\Vision Log\\Model");

		CString strFileName;
		strFileName.Format(_T("%s\\%d_%s\\%s.csv"), strPath, pModel->GetModelNumber(), pModel->GetModelName(), ut.GetStringOfCurrentDay());
		//항목
		CString strTitle(_T(""));
		strTitle.AppendFormat(_T("Time,"));
		for(auto iter : *pEle)
			strTitle.AppendFormat(_T("%s,"), iter.strParameterName);

		CString strData(_T(""));
		strData.AppendFormat(_T("Original,"));

		for(auto iter : *pEle)
		{
			switch(iter.eDataType)
			{
			case ERavidParameterDataType_Char:
				strData.AppendFormat(_T("%d,"), *(char*)iter.pData);
				break;

			case ERavidParameterDataType_UnsingedChar:
				strData.AppendFormat(_T("%d,"), *(unsigned char*)iter.pData);
				break;

			case ERavidParameterDataType_Short:
				strData.AppendFormat(_T("%d,"), *(short*)iter.pData);
				break;

			case ERavidParameterDataType_UnsignedShort:
				strData.AppendFormat(_T("%d,"), *(unsigned short*)iter.pData);
				break;

			case ERavidParameterDataType_Long:
				strData.AppendFormat(_T("%d,"), *(long*)iter.pData);
				break;

			case ERavidParameterDataType_UnsignedLong:
				strData.AppendFormat(_T("%d,"), *(unsigned long*)iter.pData);
				break;

			case ERavidParameterDataType_LongLong:
				strData.AppendFormat(_T("%d,"), *(long long*)iter.pData);
				break;

			case ERavidParameterDataType_UnsignedLongLong:
				strData.AppendFormat(_T("%d,"), *(unsigned long long*)iter.pData);
				break;

			case ERavidParameterDataType_Float:
				strData.AppendFormat(_T("%lf,"), *(float*)iter.pData);
				break;

			case ERavidParameterDataType_Double:
				strData.AppendFormat(_T("%lf,"), *(double*)iter.pData);
				break;

			case ERavidParameterDataType_String:
				strData.AppendFormat(_T("%s,"), *(CString*)iter.pData);
				break;

			default:
				strData.AppendFormat(_T("NULL,"));
				break;
			}
		}

		SaveCsvData(strTitle, strData, strFileName);

		bReturn = true;
	}
	while(false);
	return bReturn;
}
bool CUtilLog::SaveChangeParamData(CModelInfo* pModel)
{
	bool bReturn = false;
	do 
	{
		if (!pModel)
			break;

		SType0ModelParam sParam;
		pModel->GetData(&sParam, sizeof(sParam));

		//////////////////////////////////////////////////////////////////////////
		std::vector<SRavidParameterElement>* pEle = nullptr;
		pEle = pModel->GetParameterElements();
		if (!pEle)
			break;
		if (!pEle->size())
			break;

		CUtilCommFuncFile ucff;
		CUtilTime ut;
		CString strPath;
		strPath = GetParameterLogPath();
		int nPos = strPath.ReverseFind(_T('\\'));
		if(nPos > 0)
			strPath = strPath.Left(nPos);
		else
			strPath = _T("D:\\Vision Log\\Model");

		CString strFileName;
		strFileName.Format(_T("%s\\%d_%s\\%s.csv"), strPath, pModel->GetModelNumber(), pModel->GetModelName(), ut.GetStringOfCurrentDay());
		//항목
		CString strTitle(_T(""));
		strTitle.AppendFormat(_T("Time,"));
		for(auto iter : *pEle)
			strTitle.AppendFormat(_T("%s,"), iter.strParameterName);

		CString strData(_T(""));
		strData.AppendFormat(_T("%s,"), ut.GetStringOfCurrentTime());

		for(auto iter : *pEle)
		{
			switch(iter.eDataType)
			{
			case ERavidParameterDataType_Char:
				strData.AppendFormat(_T("%d,"), *(char*)iter.pData);
				break;

			case ERavidParameterDataType_UnsingedChar:
				strData.AppendFormat(_T("%d,"), *(unsigned char*)iter.pData);
				break;

			case ERavidParameterDataType_Short:
				strData.AppendFormat(_T("%d,"), *(short*)iter.pData);
				break;

			case ERavidParameterDataType_UnsignedShort:
				strData.AppendFormat(_T("%d,"), *(unsigned short*)iter.pData);
				break;

			case ERavidParameterDataType_Long:
				strData.AppendFormat(_T("%d,"), *(long*)iter.pData);
				break;

			case ERavidParameterDataType_UnsignedLong:
				strData.AppendFormat(_T("%d,"), *(unsigned long*)iter.pData);
				break;

			case ERavidParameterDataType_LongLong:
				strData.AppendFormat(_T("%d,"), *(long long*)iter.pData);
				break;

			case ERavidParameterDataType_UnsignedLongLong:
				strData.AppendFormat(_T("%d,"), *(unsigned long long*)iter.pData);
				break;

			case ERavidParameterDataType_Float:
				strData.AppendFormat(_T("%lf,"), *(float*)iter.pData);
				break;

			case ERavidParameterDataType_Double:
				strData.AppendFormat(_T("%lf,"), *(double*)iter.pData);
				break;

			case ERavidParameterDataType_String:
				strData.AppendFormat(_T("%s,"), *(CString*)iter.pData);
				break;

			default:
				strData.AppendFormat(_T("NULL,"));
				break;
			}
		}
		
		SaveCsvData(strTitle, strData, strFileName);

		bReturn = true;
	} while (false);
	return bReturn;
}