#pragma once

#include "RavidObjectEx.h"

namespace DirectX
{
	struct XMMATRIX;
	struct XMFLOAT3;
}

interface IDXGISwapChain;
interface ID3D11Device;
interface ID3D11DeviceContext;
interface ID3D11RenderTargetView;
interface ID3D11Texture2D;
interface ID3D11DepthStencilState;
interface ID3D11DepthStencilView;
interface ID3D11RasterizerState;
interface ID3D11DepthStencilState;
interface ID3D11BlendState;
interface ID3D11Buffer;
interface ID3D11VertexShader;
interface ID3D11PixelShader;
interface ID3D11InputLayout;
interface ID3D11SamplerState;
interface ID3D11Buffer;
interface ID3D11Buffer;

namespace Ravid
{
	namespace Algorithms
	{
		class CravidImage;
	}

	namespace Framework
	{
		class CRavidDoc;

		class AFX_EXT_CLASS CRavidOld3DView : public CView, public CRavidObjectEx
		{
			RavidUseDynamicCreation();

		protected: // serialization에서만 만들어집니다.
			CRavidOld3DView();
			CRavidOld3DView(const CRavidOld3DView& r3v);
			DECLARE_DYNCREATE(CRavidOld3DView)


			typedef struct sMap
			{
				float fX, fY, fZ;
				float fNX, fNY, fNZ;
				float fR, fG, fB;
			}
			SMap;


		public:

			bool SetMap(Algorithms::CRavidImage* pRiMap);
			bool ClearMap();


		// 특성입니다.
		public:
			CRavidDoc* GetDocument() const;

		// 재정의입니다.
		public:
			virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
			virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

		public:
			virtual ~CRavidOld3DView();

#ifdef _DEBUG
			virtual void AssertValid() const;
			virtual void Dump(CDumpContext& dc) const;
#endif
		public:
			virtual void OnInitialUpdate();

			virtual bool GetCameraPosition(_Out_ float* pX, _Out_ float* pY, _Out_ float* pZ);
			virtual void SetCameraPosition(_In_ float fX, _In_ float fY, _In_ float fZ);

			virtual bool GetCameraRotation(_Out_ float* pX, _Out_ float* pY, _Out_ float* pZ);
			virtual void SetCameraRotation(_In_ float fX, _In_ float fY, _In_ float fZ);


			virtual bool GetPositionRate(_Out_ float* pRate);
			virtual void SetPositionRate(_In_ float fRate);

			virtual bool GetRotationRate(_Out_ float* pRate);
			virtual void SetRotationRate(_In_ float fRate);

			virtual void SetTerrainVertexShaderPath(CString strPath);
			virtual void SetTerrainPixelShaderPath(CString strPath);

		private:
			virtual bool ClearD3DBuffer();

			virtual bool InitializeD3D();
			virtual bool TerminateD3D();
			
			int m_nIndexCount = 0;

			IDXGISwapChain* m_pSwapChain = nullptr;
			ID3D11Device* m_pDevice = nullptr;
			ID3D11DeviceContext* m_pDeviceContext = nullptr;
			ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
			ID3D11Texture2D* m_pDepthStencilBuffer = nullptr;
			ID3D11DepthStencilState* m_pDepthStencilState = nullptr;
			ID3D11DepthStencilView* m_pDepthStencilView = nullptr;
			ID3D11RasterizerState* m_pRasterState = nullptr;
// 
 			DirectX::XMMATRIX* m_pXmmProjectionMatrix = nullptr;
 			DirectX::XMMATRIX* m_pXmmWorldMatrix = nullptr;
 			DirectX::XMMATRIX* m_pXmmOrthoMatrix = nullptr;

			ID3D11DepthStencilState* m_pDepthDisabledStencilState = nullptr;
			ID3D11BlendState* m_pAlphaEnableBlendingState = nullptr;
			ID3D11BlendState* m_pAlphaDisableBlendingState = nullptr;

			SMap* m_pMap = nullptr;
			ID3D11Buffer* m_pVertexBuffer = nullptr;
			ID3D11Buffer* m_pIndexBuffer = nullptr;

			ID3D11VertexShader* m_pVertexShader = nullptr;
			ID3D11PixelShader* m_pPixelShader = nullptr;
			ID3D11InputLayout* m_pLayout = nullptr;
			ID3D11SamplerState* m_pSampleState = nullptr;
			ID3D11Buffer* m_pMatrixBuffer = nullptr;
			ID3D11Buffer* m_pLightBuffer = nullptr;

			DirectX::XMFLOAT3* m_pXmf3CameraPosition = nullptr;
			DirectX::XMFLOAT3* m_pXmf3CameraRotation = nullptr;

			volatile bool m_bRenderThread = false;
			CWinThread* m_pRenderThread = nullptr;
			static UINT RenderThread(LPVOID pParam);

			volatile bool m_bFocus = false;

			virtual bool Render();


			bool m_bLButtonDown = false;
			bool m_bMButtonDown = false;

			CPoint m_ptDragPoint;

			float m_fPositionRate = 0.3f;
			float m_fRotationRate = 0.3f;

			// Framework default path + User set path
			CString m_strTerrainVertexShader;
			CString m_strTerrainPixelShader;

		public:
			DECLARE_MESSAGE_MAP()
			afx_msg void OnSize(UINT nType, int cx, int cy);
			afx_msg void OnDestroy();
			afx_msg void OnSetFocus(CWnd* pOldWnd);
			afx_msg void OnKillFocus(CWnd* pNewWnd);
			afx_msg void OnMouseMove(UINT nFlags, CPoint point);
			afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
			afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
			afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
			afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
			afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
	}
}

