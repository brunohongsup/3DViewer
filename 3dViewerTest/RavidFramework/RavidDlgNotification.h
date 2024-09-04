#pragma once

#include "RavidDialogBase.h"

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CRavidDlgNotification : public CRavidDialogBase
		{
			RavidPreventCopySelf(CRavidDlgNotification);

		public:
			DECLARE_DYNAMIC(CRavidDlgNotification);

		public:
			CRavidDlgNotification(_In_ CString strMessage, _In_opt_ ERavidNotificationType eNotification = ERavidNotificationType_Information, _In_opt_ ERavidModalType eModalType = ERavidModalType_Modal); //표준 생성자입니다.
			virtual ~CRavidDlgNotification();

			// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
			enum { IDD = IDD_RAVID_DLG_NOTIFICATION };
#endif

			virtual bool Create();

		protected:
			virtual void DoDataExchange(CDataExchange* pDX);

			DECLARE_MESSAGE_MAP();
		public:
			virtual BOOL OnInitDialog();
			virtual void SetBackgroundColor();

			afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
			afx_msg void OnPaint();
			afx_msg void OnDestroy();
			afx_msg void OnBnClickedOk();
			
			virtual BOOL PreTranslateMessage(MSG* pMsg);

		protected:
			virtual bool InitializeNotificationDialog();

		public:
			afx_msg void OnBnClickedButtonAllok();
			CRect MonitorSize();

		protected:
			CString m_strNotification = _T("");

			ERavidNotificationType m_eRavidNotificationType = ERavidNotificationType_Information;
			ERavidModalType m_eRavidModalType = ERavidModalType_Modal;

			CFont m_fontMessage;
			CRect m_crTextRange;

			COLORREF m_clrBackground = 0;
			COLORREF m_clrFrameColor = 0;
			COLORREF m_clrMouseHover = 0;
			CStatic m_ctrlImage;

			bool m_bIsOKButtonHover = false;
			bool m_bIsAllOkButtonHover = false;

		};
	}
}