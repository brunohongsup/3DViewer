
// RavidChildFrameImageView.h : CRavidChildFrameImageView 클래스의 인터페이스
//

#pragma once

#include "RavidChildFrameBase.h"

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CRavidChildFrameImageView : public CRavidChildFrameBase
		{
			RavidUseDynamicCreation();

			DECLARE_DYNCREATE(CRavidChildFrameImageView)

			enum ERavidChildFrameImagePopupMenu
			{
				ERavidChildFrameImagePopupMenu_Distance,
				ERavidChildFrameImagePopupMenu_DistanceCircle,
				ERavidChildFrameImagePopupMenu_DistanceCircumcenter,
				ERavidChildFrameImagePopupMenu_DistanceParallel,
				ERavidChildFrameImagePopupMenu_Displacement,
				ERavidChildFrameImagePopupMenu_AngleMeasurement,
				ERavidChildFrameImagePopupMenu_AngleAzimuth,
				ERavidChildFrameImagePopupMenu_AngleGradient,
				ERavidChildFrameImagePopupMenu_Area,
				ERavidChildFrameImagePopupMenu_Count,
			};

		public:
			CRavidChildFrameImageView();
			virtual ~CRavidChildFrameImageView();

			// 생성된 메시지 맵 함수
		protected:
			DECLARE_MESSAGE_MAP()
		public:
			virtual bool InitialzeToolBar();

			virtual bool UpdateToolbarStatus();

			afx_msg void OnBnClickedImageOpen();
			afx_msg void OnBnClickedImageSave();
			afx_msg void OnBnClickedImageClose();
			afx_msg void OnBnClickedInspect();
			afx_msg void OnBnClickedTeaching();
			afx_msg void OnBnClickedEditTeaching();
			afx_msg void OnBnClickedArrow();
			afx_msg void OnBnClickedZoomIn();
			afx_msg void OnBnClickedZoomOut();
			afx_msg void OnBnClickedZoomAll();
			afx_msg void OnBnClickedMove();
			afx_msg void OnBnClickedMeasure();
			afx_msg void OnBnClickedLayerOnOff();
			afx_msg void OnBnClickedGridOn();
			afx_msg void OnBnClickedGridOff();
			afx_msg void OnBnClickedGVColorOn();
			afx_msg void OnBnClickedGVColorOff();
			afx_msg void OnBnClickedGVOn();
			afx_msg void OnBnClickedGVOff();
			afx_msg void OnBnClickedAccuracySetting();
			afx_msg void OnBnClickedStatusBarOn();
			afx_msg void OnBnClickedStatusBarOff();
			afx_msg void OnBnClickedScrollBarOn();
			afx_msg void OnBnClickedScrollBarOff();
			afx_msg void OnBnClickedInvalidateOn();
			afx_msg void OnBnClickedInvalidateOff();
			afx_msg void OnBnClickedMaximize();
			afx_msg void OnBnClickedRestore();
			afx_msg void OnBnClickedUndo();
			afx_msg void OnBnClickedRedo();

			afx_msg LRESULT OnUpdateToolbarStatus(WPARAM wParam, LPARAM lParam);
			afx_msg LRESULT OnUpdateCursorModeButton(WPARAM wParam, LPARAM lParam);

			virtual bool UpdateCursorModeButton(_In_ ERavidImageViewCursorMode eSelectedMode);

			virtual bool VisibleToolBarButton(_In_ ERavidToolBarEventImageView eEvent, _In_ bool bVisible);

		protected:
			CBitmap m_bmMenuIcons[ERavidChildFrameImagePopupMenu_Count];

		};
	}
}
