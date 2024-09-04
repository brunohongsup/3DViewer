#pragma once
using namespace Ravid::Framework;
using namespace Ravid;


class CUtilImageSplit
{
public:
	CUtilImageSplit();
	~CUtilImageSplit();

	void SetSplitSize(int nImageSizeX, int nImageSizeY, int nSplitSizeX, int nSplitSizeY);

	bool SaveImage(CRavidImage* pImage, CString strPath, CString strFileName, CString strDataFormat, CRavidPoint<double> rpCuttingCenter);

	void OpenImage(CRavidImage* pImage, CString strPath, CString strFileName, CString strDataFormat, CRavidPoint<double> rpCuttingCenter);


private:
	typedef	struct sImageData
	{
		CRavidImage* riImage = nullptr;
		CString strPath = _T("D:\\Image\\");
		CString strFileName = _T("SplitImage");
		CString strDataFormat = _T("bmp");

		CRavidPoint<double> rpCuttingCenter;

	}SImageData;


	int m_nImageSizeX = 0;
	int m_nImageSizeY = 0;
	int m_nSplitSizeX = 0;
	int m_nSplitSizeY = 0;

	std::queue<SImageData> m_qeueuImage;


	ThreadContext* m_pSaveImageThread;
	volatile bool m_bSaveImageThread = false;
	CCriticalSection m_csQueue;

	void StartSaveImageThread();
	void StopSaveImageThread();
};
