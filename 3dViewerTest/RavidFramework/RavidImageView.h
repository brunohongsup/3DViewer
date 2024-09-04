#pragma once

#include "RavidObjectEx.h"

#include "../RavidCore/RavidPoint.h"
#include "../RavidCore/RavidRect.h"
#include "MultiSelectionObject.h"

#include <d2d1_1.h>
#include <queue>

namespace Ravid
{
	namespace Miscellaneous
	{
		class CPerformanceCounter;
	}

	namespace Framework
	{
		class CRavidDoc;
		class CRavidImageViewLayer;
		class CViewObject;
		class CViewObjectElement;

		class AFX_EXT_CLASS CRavidImageView : public CView, public CRavidObjectEx
		{
			RavidUseDynamicCreation();

		protected: // serialization에서만 만들어집니다.
			CRavidImageView();
			CRavidImageView(const CRavidImageView& riv);
			DECLARE_DYNCREATE(CRavidImageView)

		// 특성입니다.
		public:
			CRavidDoc* GetDocument() const;

		// 작업입니다.
		public:

		// 재정의입니다.
		public:
			virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
			virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
		protected:
			virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
			virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
			virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

		// 구현입니다.
		public:
			virtual ~CRavidImageView();
#ifdef _DEBUG
			virtual void AssertValid() const;
			virtual void Dump(CDumpContext& dc) const;
#endif

		public:

			bool DoesImageExist();
			bool IsOpen();

			virtual bool Open(_In_opt_ LPCTSTR lpszFileName = nullptr);
			virtual bool Save(_In_opt_ LPCTSTR lpszFileName = nullptr);
			virtual bool Close();

			virtual bool SetImageInfo(_In_ Algorithms::CRavidImage* pImgInfo, _In_opt_ bool bZoomFit = true);
			virtual Algorithms::CRavidImage* GetImageInfo();


			POINT ImageCoordToScreenCoord(_In_ double fX, _In_ double fY);
			POINT ImageCoordToScreenCoord(_In_ CRavidPoint<double> fpt);

			POINT ImageCoordToScreenCoord(_In_ float fX, _In_ float fY);
			POINT ImageCoordToScreenCoord(_In_ CRavidPoint<float> fpt);

			CRavidPoint<double> ScreenCoordToImageCoord(_In_ long nX, _In_ long nY);
			CRavidPoint<double> ScreenCoordToImageCoord(_In_ CRavidPoint<int> pt);

			CRavidRect<int> ImageCoordToScreenCoord(_In_ double fLeft, _In_ double fTop, _In_ double fRight, _In_ double fBottom);
			CRavidRect<int> ImageCoordToScreenCoord(_In_ CRavidRect<double> frrRect);

			CRavidRect<double> ScreenCoordToImageCoord(_In_ long nLeft, _In_ long nTop, _In_ long nRight, _In_ long nBottom);
			CRavidRect<double> ScreenCoordToImageCoord(_In_ CRavidRect<int> rect);

			virtual void Zoom(_In_ POINT point, _In_opt_ double fScale = 1.0f);
			virtual void ZoomIn(_In_ POINT point, _In_opt_ double fRatio = 1.25f);
			virtual void ZoomOut(_In_ POINT point, _In_opt_ double fRatio = .8f);
			virtual void ZoomFit();

			virtual bool IsPanned();

			virtual bool SetScale(_In_ double dblScale);
			virtual double GetScale();

			virtual bool SetPanningOffset(_In_ double x, _In_ double y);
			virtual bool SetPanningOffset(_In_ CRavidPoint<double>& rpOffset);
			virtual bool SetPanningOffset(_In_ CRavidPoint<double>* pRpOffset);
			virtual CRavidPoint<double> GetPanningOffset();

			virtual bool SetTitle(_In_ CString strTitle);

			virtual void ShowPixelValue(_In_ bool bShow);
			virtual bool IsPixelValueShowing();

			virtual void ShowPixelSpectrum(_In_ bool bShow);
			virtual bool IsPixelSpectrumShowing();

			virtual void ShowPixelGrid(_In_ bool bShow);
			virtual bool IsPixelGridShowing();

			virtual void ShowStatus(_In_ bool bShow);
			virtual bool IsStatusShowing();

			virtual bool SetAccuracyX(_In_ double dblAccuracyX);
			virtual double GetAccuracyX();

			virtual bool SetAccuracyY(_In_ double dblAccuracyY);
			virtual double GetAccuracyY();

			virtual bool SetAccuracyUnit(_In_opt_ ERavidAccuracyUnit eUnit = ERavidAccuracyUnit_mm);
			virtual ERavidAccuracyUnit GetAccuracyUnit();

			CRavidImageViewLayer* GetLayer(_In_ int nNumber);

			virtual bool CaptureScreen(_Out_ Algorithms::CRavidImage* pImgInfo);
			virtual bool CaptureImage(_Out_ Algorithms::CRavidImage* pImgInfo);

			// Only framework use

			virtual bool OnImageOpen();
			virtual bool OnImageSave();
			virtual bool OnImageClose();
			virtual bool OnImageCopy();
			virtual bool OnImageCopyFromTheIPT();
			virtual bool OnImageCopyFromTheProject();
			virtual bool OnImageCopyToTheIPT(_In_ Algorithms::CRavidImage* pImage);
			virtual bool OnImageCopyToTheProject(_In_ Algorithms::CRavidImage* pImage);
			virtual bool OnImageTransferIPT();
			virtual bool OnImageTransferProject();
			virtual bool OnInspect(_In_ int nIndex);
			virtual bool OnTeaching(_In_ int nIndex);
			virtual bool OnEditTeaching(_In_ int nIndex);
			virtual bool OnMeasure(_In_ ERavidMeasurementType eType);
			virtual bool OnToggleLayer(_In_ int nIndex);
			virtual bool OnAccuracySetting();
			virtual bool OnMaximize();
			virtual bool OnRestore();

			virtual int GetStatusHeight();

			virtual bool LoadExtraData();

			virtual bool SetPrevImageInfo(_In_ Algorithms::CRavidImage* pImgInfo);
			virtual Algorithms::CRavidImage* GetPrevImageInfo();

			virtual void SetCursorMode(_In_ ERavidImageViewCursorMode eMode);
			virtual ERavidImageViewCursorMode GetCursorMode();

			virtual bool AddViewObject(_In_ CViewObject* pRSO, _In_opt_ bool bAutoShape = true, _Out_opt_ CViewObject** ppCreateRSO = nullptr);

			virtual bool DeleteViewObject(_In_ CViewObject* pRSO);

			virtual bool DeleteViewObjectElement(_In_ CViewObject* pRSO, _In_ CViewObjectElement* pObjectElement);

			virtual bool ClearViewObject(_In_opt_ bool bIgnoreImageToolObject = false);

			virtual bool SelectViewObjectElement(_In_ CViewObjectElement* pRSOE);

			virtual bool CorrelatePointOfView(_In_ CRavidImageView* pImageView);
			virtual bool ReleaseCorrelatedPointOfView(_In_ CRavidImageView* pImageView);

			virtual void SetImageClientSize(_In_ int nWidth, _In_ int nHeight);
			virtual CRavidPoint<int> GetImageClientSize();

			virtual bool CopyGeometryTotheProject(_In_ CViewObjectElement* pViewObjectElement);

			virtual bool IPTTransferGeometryTotheProject(_In_ CViewObjectElement* pViewObjectElement);
			virtual bool ProjectTransferGeometryTotheProject(_In_ CViewObjectElement* pViewObjectElement);

			virtual bool ProjectCopyGeometryFromtheProject();
			virtual bool IPTCopyGeometryFromtheProject();

			virtual bool ProjectBringGeometryFromtheProject();
			virtual bool IPTBringGeometryFromtheProject();

			virtual int GetProjectImageGeometry();
			virtual int GetProjectImageGeometryToIPT();

			virtual bool DeleteAt(_In_ CViewObject* pViewObject, _In_ CRavidGeometry* pGeometry, _In_ int nIndex);

			virtual bool ChangeViewImage(_In_ Algorithms::CRavidImage* pImage, _In_ CString strTitle);

			virtual bool ProjectTransferImageToTheIPT();
			virtual bool ProjectTransferImageToTheProject();
			virtual bool ProjectBringImageFromTheProject();
			virtual bool IPTBringImageFromTheProject();

			virtual std::vector<CViewObject*>* GetViewObjects();

			virtual CViewObject* GetViewObject(_In_ int nObjectID);
			virtual CViewObject* GetViewObject(_In_ CString strTitle);

			virtual std::vector<CViewObjectElement*>* GetViewElements();

			virtual CViewObjectElement* GetSelectViewObjectElement();
			virtual bool UpdateMultiSelectionObject();

			virtual bool LoadOperation(_In_ CViewObjectElement* pViewObjectElement);
			virtual bool OperationViewObject(_In_ CViewObjectElement* pViewObjectElement, _In_ CRavidGeometryArray* pGeometryArray);

			virtual bool UpdateOperation();
			virtual bool UpdateHighlightOperation(_In_ CViewObjectElement* pViewObjectElement);

			virtual bool ImageViewUpdateToolBar();

			virtual CString GetSupportingFormatFilter(_In_ Algorithms::CRavidImage* pImage);
			virtual bool DoesSupportingFormat(_In_ Algorithms::CRavidImage* pImage);

			virtual bool ImageViewUpdateScrollBar();

			virtual void RunHScroll(_In_ UINT nSBCode, _In_ UINT nPos, _In_ CScrollBar * pScrollBar);
			virtual void RunVScroll(_In_ UINT nSBCode, _In_ UINT nPos, _In_ CScrollBar * pScrollBar);

			virtual bool EnableScrollBar(_In_ bool bEnable);

			virtual void EnableInvalidate(_In_ bool bEnable);
			virtual bool IsInvalidateEnable();

			virtual void EnablePopupMenu(_In_ bool bEnable);
			virtual bool IsPopupMenuEnable();

			virtual CRavidPoint<int> GetLastUpImagePoint();

			virtual void GetImageClientRect(_In_ RECT* pClient);

		protected:

			void __fastcall FastCeil(_Inout_ double& fValue);

			int __fastcall GetWidthBytes(_In_ int nCx);

			virtual void SetClientSize(_In_ int nWidth, _In_ int nHeight);
			virtual CRavidPoint<int> GetClientSize();

			virtual void ChangeImageCanvasSize();

			virtual bool InitializeDIB();
			virtual bool TerminateDIB();

			virtual bool DrawCanvas();
			virtual bool DrawImage();
			virtual bool DrawLayer();
			virtual bool DrawPixelValue();
			virtual bool DrawGrid();
			virtual bool DrawStatus();

			bool CommitDraw(_In_ CDC* pDC);

			void SetDragging(_In_ bool bSet);
			bool IsDragging();

			void SetPanning(_In_ bool bSet);
			bool IsPanning();

			void SetPanned(_In_ bool bSet);

			virtual double GetZoomFitScale();

			CRavidPoint<int> GetImageDimension();

			virtual bool RunPopupMenu(CPoint point);

			virtual bool IsCorrelatedPointOfView(_In_ CRavidImageView* pImageView);
			virtual CRavidImageView* GetBeginCorrelatedPointOfView();
			virtual CRavidImageView* GetEndCorrelatedPointOfView();

			virtual bool GetCorrelatedPointOfViewList(_Out_ std::vector<CRavidImageView*>* pVctCorrelatedPointOfViews);

			virtual bool ArrangeCorrelatedPointOfView();

			virtual bool Undo();
			virtual bool Redo();

		private:
			virtual bool AddThreadDisplayer(_In_ TViewThreadDisplayer* pDisp);
			virtual TViewThreadDisplayer* GetDisplayer();
			virtual bool IsDisplayerExisting();
			virtual bool RunThreadDisplayer();
			virtual void ClearDisplayer();

		// 생성된 메시지 맵 함수
		protected:
			afx_msg void OnFilePrintPreview();
			afx_msg void OnRButtonUp(UINT nFlags, CPoint point);

			DECLARE_MESSAGE_MAP()
		public:
			virtual void OnInitialUpdate();
			afx_msg void OnDestroy();
			afx_msg BOOL OnEraseBkgnd(CDC* pDC);
			afx_msg void OnSize(UINT nType, int cx, int cy);
			afx_msg void OnMouseMove(UINT nFlags, CPoint point);
			afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
			afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
			afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
			afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
			afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
			virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
			virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
			afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
			afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
			afx_msg void OnDropFiles(HDROP hDropInfo);
			afx_msg void OnTimer(UINT_PTR nIDEvent);
			afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
			afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
			afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
			afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
			afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);

			enum
			{
				EMaxLayerCount = 10,
			};

			virtual bool RunPopupMenuCancelToDeleteElement(CViewObjectElement* pSelectedVOE);

			virtual void SetStatusFont(LONG weight, LONG height, CString facename);

		protected:
			SRavidDibInfo m_bitmapInfo;
			HBITMAP m_hDibSection = 0;

			CBitmap m_bitmapLayer;
			CBitmap* m_pPreviousBitmapLayer = nullptr;

			CDC m_dcDIB;
			CDC m_dcLayer;

			HDC m_hdcDIB;
			HGDIOBJ	m_hPreviousSelectedGdiObj;
			HANDLE m_hDrawDIB;

			BYTE* m_pDibBase = nullptr;
			BYTE* m_pOffBuffer = nullptr;

			ID2D1DCRenderTarget* m_pD2DRender = nullptr;

			Miscellaneous::CPerformanceCounter* m_pPCFrame = nullptr;

			CRavidPoint<int> m_nptImageDimension;
			long m_nPitch = 0;

			CSize m_sizeClient;

			Algorithms::CRavidImage* m_pRavidImage = nullptr;
			Algorithms::CRavidImage* m_pPreviousRavidImage = nullptr;
			Algorithms::CRavidImage* m_pImageInfo = nullptr;

			CRavidPoint<int> m_nptClientSize;
			CRavidPoint<int> m_nptImageClientSize;

			CRavidPoint<int> m_nptBase;
			CRavidPoint<int> m_nptCurrent;
			CRavidPoint<int> m_rpDragStart;
			CRavidPoint<int> m_rpLastUp;

			double m_fScale = 1.f;
			CRavidPoint<double> m_fptOffset;

			bool m_bIsDragging = false;
			bool m_bIsPanning = false;
			bool m_bIsPanned = false;
			bool m_bIsOpen = false;

			COLORREF m_crGVSpectrum[256] = { 0, };
			BYTE m_arrFloatingPointGVTable[1001];

			std::vector<CRavidImageViewLayer*> m_vctDrawingLayers;

			CBitmap m_bmMenuIcons[ERavidImageViewPopupMenu_Count];

			std::vector<CViewObject*> m_vctViewObjects;
			std::vector<CViewObjectElement*> m_vctShapeViewElements;
			CViewObjectElement* m_pSelectedViewObjectElement = nullptr; //
			CMultiSelectionObject* m_pMultiSelectionObject = nullptr;

			CRavidImageViewLayer* m_pTeachingObjectLayer = nullptr;

			bool m_bIsLbuttonDown = false;
			bool m_bIsMbuttonDown = false;

			bool m_bIsPopupCreated = false;

			ERavidImageViewCursorMode m_eCursorMode = ERavidImageViewCursorMode_Arrow;

			bool m_bShowPixelValue = true;
			bool m_bShowPixelSpectrum = true;
			bool m_bShowPixelGrid = false;
			bool m_bShowStatus = true;

			int m_nStatusHeight = -1;

			double m_dblAccuracyX = 1.;
			double m_dblAccuracyY = 1.;

			ERavidAccuracyUnit m_eAccuracyUnit = ERavidAccuracyUnit_mm;

			CRavidImageView* m_pPrevCorrelatedPointOfView = nullptr;
			CRavidImageView* m_pNextCorrelatedPointOfView = nullptr;

			CRavidImageView* m_pPrevPerspectiveView = nullptr;
			CRavidImageView* m_pNextPerspectiveView = nullptr;

			std::queue<TViewThreadDisplayer*> m_queDisplayer;

			bool m_bEnableInvalidate = true;
			bool m_bEnablePopupMenu = true;

			SRavidFontInfo m_sViewFont;

			CString m_strTitle = _T("");
		};
	}
}

#ifndef _DEBUG  // RavidImageView.cpp의 디버그 버전
inline Ravid::Framework::CRavidDoc* Ravid::Framework::CRavidImageView::GetDocument() const
{
	return reinterpret_cast<Ravid::Framework::CRavidDoc*>(m_pDocument);
}
#endif

