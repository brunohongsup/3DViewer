#pragma once

class CRavid3DData;
class AFX_EXT_CLASS CRavid3D
{
public:
	enum ECoordinateType
	{
		ECoordinateType_None = 0,
		ECoordinateType_ModelView,
		ECoordinateType_PerspectiveView,
		ECoordinateType_ScreenView,
	};

public:
	CRavid3D();
	virtual ~CRavid3D();

public:
	virtual bool Initialize(_In_ int nWidth, _In_ int nHeight);
	virtual bool Terminate();

	virtual bool AddMap(_In_ CRavid3DData* pData);

	virtual bool Clear();

	virtual void SetViewSize(_In_ int nWidth, _In_ int nHeight);

	virtual void Rendering();

	virtual void UpdateRendering(_In_ bool bUpdate);
	virtual bool IsUpdatedRendering();

protected:
	virtual void PointRendering(_In_ CRavid3DData* pData, _In_ int iLOD);
	virtual void WireframeRendering(_In_ CRavid3DData* pData, _In_ int iLOD);
	virtual void PolygonRendering(_In_ CRavid3DData* pData, _In_ int iLOD);
	virtual void VolumeRendering(_In_ CRavid3DData* pData, _In_ int iLOD);

	virtual void SectionRendering(_In_ CRavid3DData* pData);

	virtual void SettingCamera();
	virtual void SettingCoordinate(ECoordinateType eType);

	virtual void InitializeFBO(_In_ int nWidth, _In_ int nHeight);
	virtual void DestroyFBO();

	virtual bool EnableFBO(_In_ bool bEnable);

	virtual void RestorationFBO();

	virtual bool InitializeShader();
	virtual bool DestroyShader();

	virtual bool EnableShader(_In_ bool bEnable);

	virtual bool InitializeExtension();

	virtual bool GPUDownload(_In_ CRavid3DData* pData);

	virtual void* Lock(_In_ uint eType, _In_ intptr nSize);
	virtual bool Unlock(_In_ uint eType);

protected:
	friend class C3DViewer;

protected:
	std::vector<CRavid3DData*> m_vctData;

	int2 m_nViewSize;
	double m_dblViewAspectRatio;

	bool m_bUpdateRendering;
	bool m_bEnableRendering;

	int m_nLOD;

	float m_fScale;

	// Camera
	double m_dblFOV;

	float3 m_fTarget;
	float3 m_fPosition;

	float4 m_fCoordinates;
	float3 m_fRotate;
	float m_fMagnification;

	// FBO
	uint m_uRenderBufferID;
	uint m_uRenderTextureID;
	uint m_uRenderFrameID;

	// Shader
	uint m_uProgramID;

	uint m_uVertexShaderID;
	uint m_uFragmentShaderID;
};

