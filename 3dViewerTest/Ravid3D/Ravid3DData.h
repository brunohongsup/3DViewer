#pragma once

#include "Definitions.h"

namespace Ravid
{
	namespace Algorithms
	{
		class CRavidImage;
	}
}

class AFX_EXT_CLASS CRavid3DData
{
public:
	enum EModelType
	{
		eModelType_Polygon = 0,
		eModelType_Point,
		eModelType_Wireframe,
		eModelType_Volume,
		eModelType_Count,
	};

	enum EVolumDataSectionMode
	{
		eVolumDataSectionMode_None = 0,
		eVolumDataSectionMode_X = 1,
		eVolumDataSectionMode_Y = 2,
		eVolumDataSectionMode_Z = 4,
		eVolumDataSectionMode_Camera = 8
	};

public:
	CRavid3DData();
	CRavid3DData(_In_ CRavid3DData* pData);
	virtual ~CRavid3DData();

	virtual void Clear();

	virtual bool Open(_In_ LPCTSTR lpszFileName, _In_ int nWidth, _In_ int nHeight, _In_ int nDepth, _In_ int nBpp, _In_ EModelType eType); // .dat, .cav, .raw(Volume)

	virtual bool AddData(_In_ Ravid::Algorithms::CRavidImage* pData, _In_opt_ Ravid::Algorithms::CRavidImage* pColor = nullptr, _In_opt_ EModelType eType = eModelType_Polygon, _In_opt_ float fOffsetX = 0., _In_opt_ float fOffsetY = 0., _In_opt_ float fOffsetZ = 0.);

	virtual bool AddData(_In_ std::vector<Ravid::Algorithms::CRavidImage*>* pData); // Volume

	virtual bool Copy(_In_ CRavid3DData* pData);

	virtual void ClearRainbowTable();
	virtual void CreateRainbowTable(_In_ float fMinRange, _In_ float fMaxRange, _In_ BYTE r, _In_ BYTE g, _In_ BYTE b, _In_ BYTE a);

	virtual bool GetTextureValue(_In_ float fVertex, _Out_ BYTE* r, _Out_ BYTE* g, _Out_ BYTE* b, _Out_ BYTE* a);

	virtual void SetVolumDataSectionMode(_In_ EVolumDataSectionMode eSection);

protected:
	virtual void InitializeVBO();
	virtual void DestroyVBO();

	virtual void InitializeVAO();
	virtual void DestroyVAO();

	virtual void ClearTexture();  // 1D Texture
	virtual bool CreateTexture();

	virtual int GetTextureIndex(_In_ float fVertex);

	virtual bool IsUpdatedRendering();

protected:
	friend class CRavid3D;

protected:
	unsigned char* m_pBuffer;
	int64 m_nBufferCount;
	int3 m_nBufferSize;

	unsigned char* m_pColor;
	int64 m_nColorCount;
	int m_nColorChannels;

	float m_fOffsetX;
	float m_fOffsetY;
	float m_fOffsetZ;

	float m_fNormalScale;

	EModelType m_eType;

	std::vector<std::pair<float, float>> m_vctTextureRange;
	std::vector<byte4> m_vctRainbowTable;

	uint m_uVertexBufferObjectID;
	uint m_uElementBufferObjectID;

	uint m_uVAOObjectID;

	byte4* m_pTexture;
	uint m_uTextureID;

	bool m_bEnableColor;
	bool m_bEnableTexture;

	uint m_u3DTextureID;  // Volume

	EVolumDataSectionMode m_eSection;
	double4	m_dblRegion;
	double4	m_dblDelta;
};

