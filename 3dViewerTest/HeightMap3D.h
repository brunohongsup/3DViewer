#pragma once
class CHeightMap3D
{
public:
	CHeightMap3D();
	~CHeightMap3D();

public:
	void Clear();
	void Reset();

	void SetMap(CRavidImage* pSrcMap);

	void Convert();

	float GetHeight(int x, int y);

	bool GetHeight(int x, int y, float& resHeight);

	CRavidImage* GetSourceMap();
	CRavidImage* GetHeightMap();
	CRavidImage* GetColorSudoMap();
	
	int GetCount();

	float m_fInvalidValue;

	int m_nUsedSetting;
	float m_fLow, m_fMid, m_fUp;
	DWORD m_dwLowColor, m_dwMidColor, m_dwUpColor;

private:
	void Convert_0();
	void Convert_1();
	void Convert_2();
	void Convert_3();
	void Convert_4();
	void Convert_5();

	CRavidImage* m_pCopyMap;
	CRavidImage* m_pHeightMap;
	CRavidImage* m_pHeightColorMap;
};

