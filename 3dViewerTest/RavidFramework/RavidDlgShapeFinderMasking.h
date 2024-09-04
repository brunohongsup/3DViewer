#pragma once
#include "RavidDialogBase.h"

namespace Ravid
{
	namespace Algorithms
	{
		class CShapeFinder;
	}
	namespace Framework
	{
		class CRavidDlgShapeFinderImageView;
		class AFX_EXT_CLASS CRavidDlgShapeFinderMasking : public CRavidDialogBase
		{
			DECLARE_DYNAMIC(CRavidDlgShapeFinderMasking)

		public:
			CRavidDlgShapeFinderMasking(Algorithms::CShapeFinder* pFinder, CWnd* pParent = nullptr);
			virtual ~CRavidDlgShapeFinderMasking();

		public:
			virtual void DoDataExchange(CDataExchange* pDX) override;
			virtual bool Create(CWnd* pParent = nullptr);
			virtual BOOL OnInitDialog() override;			
			virtual BOOL PreTranslateMessage(MSG* pMsg);
			
			DECLARE_MESSAGE_MAP()

			afx_msg void OnDestroy();
			afx_msg void OnClose();
			afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

			afx_msg void OnBnClickedToolClean();
			afx_msg void OnBnClickedToolCreate();
			afx_msg void OnBnClickedToolUndo();
			afx_msg void OnBnClickedToolRedo();

			afx_msg void OnBnClickedTypeRectangle();
			afx_msg void OnBnClickedTypeEllipse();

			afx_msg void OnBnClickedReset();
			afx_msg void OnBnClickedSave();
			afx_msg void OnBnClickedCancel();

			afx_msg void OnEnChangeBrushSizeW();
			afx_msg void OnEnChangeBrushSizeH();

			afx_msg LRESULT OnMsgCleanMasking(WPARAM wparam, LPARAM lparam);
			afx_msg LRESULT OnMsgUndoMasking(WPARAM wparam, LPARAM lparam);

			afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
		private:
			void UpdateFeature();
			void SetImage();		
			void Undo();
			void Redo();
		private:

			CRavidDlgShapeFinderImageView* m_pView = nullptr;
			CCreateContext m_context;

			Algorithms::CRavidImage* m_pImage = nullptr;

			CStatic m_labelTitleImage;
			CStatic m_labelTitleParameter;

			HBRUSH m_brushTitle = nullptr;
			CFont m_font;

			CButton m_btnBrushTypeRect;
			CButton m_btnBrushTypeEllipse;

			CEdit m_edtBrushSizeW;
			CEdit m_edtBrushSizeH;

			CButton m_btnToolClean;
			CButton m_btnToolCreate;
			CButton m_btnToolUndo;
			CButton m_btnToolRedo;

			CSliderCtrl m_sliderWidth;
			CSliderCtrl m_sliderHeight;

			Algorithms::CShapeFinder* m_pFinder = nullptr;
		};
	}
}


