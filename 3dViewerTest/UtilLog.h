#pragma once

#include "DrawImage.h"

#include "gdiplus.h"
#pragma comment(lib, "gdiplus")
using namespace Gdiplus;


typedef struct sSaveImage
{
	CRavidImage* pImgInfo = nullptr;
	CString strPath;

	bool bCapture = false;
	CRavidImageViewLayer* pLayer = nullptr;

	void Clear()
	{
		CRavidImage* pImgInfo = nullptr;
		CString strPath = _T("");

	};
}SSaveImage;

typedef struct sSaveLog
{
	CString strItem;
	CString strLog;
	CString strPath;

}SSaveLog;

//////////////////////////////////////////////////////////////////////////

class CUtilLog
{
private:
	// 복사 생성자 삭제
	CUtilLog(CUtilLog const&) = delete;
	// 이동 생성자 삭제
	CUtilLog(CUtilLog&&) = delete;
	// 복사 연산자 삭제
	CUtilLog& operator=(CUtilLog const&) = delete;
	// 이동 생성자 삭제
	CUtilLog& operator=(CUtilLog&&) = delete;
//////////////////////////////////////////////////////////////////////////
	// 기본 생성자
	CUtilLog(void);

	// 생성자 변수
	static CUtilLog *instance;

public:
	virtual ~CUtilLog(void);

	// 생성자 함수
	static CUtilLog* GetInstance();
	// 생성자 소멸 함수
	static void DestroyInstance();
public:

	static bool Initialize();
	static bool Terminate();

	static void SetRecursive(bool bRecursive) { m_bRecursive = bRecursive; }

	static bool AddDeletingFolder(LPCTSTR lpszPath);
	static void SetExpirationDate(ULONG ulDate) { m_ulExpirationDate = ulDate; }

	static void SetRemoveFileAttribute(bool bRemoveFileAttribute) { m_bRemoveFileAttribute = bRemoveFileAttribute; }

	static double GetUsedDriverPercentage(LPCTSTR lpszDriver);
	static CString GetDriverName(LPCTSTR lpszFile);
	static void SetLimitDriveVolume(bool bSet) { m_bLimitDriveVolume = bSet; }
	static bool IsLimitDriveVolume() { return m_bLimitDriveVolume; }
	static void SetDriveVolumeLimitPercentage(float fPercentage) { m_fDriveVolumeLimitPercentage = fPercentage; }

	static ULONG GetExpirationDate() { return m_ulExpirationDate; }
	static bool	IsRecursive() { return m_bRecursive; }
	static bool	IsRemoveFileAttribute() { return m_bRemoveFileAttribute; }
	static float GetDriveVolumeLimitPercentage() { return m_fDriveVolumeLimitPercentage; }
	static std::vector<CString>* GetPathList() { return &m_vctPathList; }
///////////////////////////////////////////////////////////////////////////
// Delete File
protected:

	static ThreadContext* m_pThreadDeleteFile			/*= nullptr*/;
	static volatile bool m_bIsThreadDeleteFile			/*= false*/;
	static volatile ULONG m_ulExpirationDate			/*= 180*/;
	static volatile bool m_bRecursive					/*= true*/;
	static volatile bool m_bRemoveFileAttribute			/*= false*/;
	static volatile bool m_bLimitDriveVolume			/*= false*/;
	static volatile float m_fDriveVolumeLimitPercentage /*= 80.*/;
	static std::vector<CString> m_vctPathList;
	static CCriticalSection m_csDeleteLock;

public:
	static bool StartDeleteThread();
	static bool StopDeleteThread();
//////////////////////////////////////////////////////////////////////////
// Image Thread
protected:
	static ThreadContext* m_pThreadSaveImage /*= nullptr*/;
	static volatile bool m_bThreadSaveImage /*= false*/;

	/*static std::queue<sSaveImage>* GetImage() { return &m_queImageData; }*/
	static std::queue<sSaveImage> m_queImageData;
	static CCriticalSection m_csImageLock;

	static bool StartSaveImageThread();
	static bool StopSaveImageThread();

public:
	static bool SaveImage(CRavidImage* pImgInfo, CString strPath);
	static bool SaveDrawImage(CRavidImage* pImgInfo, CRavidImageViewLayer* pLayer, CString strPath);
//////////////////////////////////////////////////////////////////////////
// CSV Thread

private:
	static std::queue<sSaveLog> m_queCsvData;

protected:
	static ThreadContext* m_pThreadSaveLog /*= nullptr*/;
	static volatile bool m_bThreadSaveLog /*= false*/;

	static bool StartSaveLogThread();
	static bool StopSaveLogThread();

	static CCriticalSection m_csDataSaveLock;
	static bool WriteCsvFile(const sSaveLog& sData);

public:
	static bool SaveCsvData(const CString& strItem, const CString& strData, const CString& strPath);
//////////////////////////////////////////////////////////////////////////

// 모델
protected:
	static CString m_strParamerLogFolder;
public:
	//모델 파라미터 csv 저장
	static void SetParameterLogPath(CString strPath) { m_strParamerLogFolder = strPath; }
	static CString GetParameterLogPath() { return m_strParamerLogFolder; }

	static bool SaveOriginalParamData(CModelInfo* pModel);
	static bool SaveChangeParamData(CModelInfo* pModel);

};
