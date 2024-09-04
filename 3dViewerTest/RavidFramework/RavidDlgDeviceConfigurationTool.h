#pragma once


// CRavidDlgDeviceConfigurationTool 대화 상자입니다.

#include "RavidDialogBase.h"
#include "RavidTreeCtrl.h"

#include <memory>

namespace Ravid
{
	namespace Device
	{
		class CDeviceBase;
	};

	namespace Framework
	{
		class AFX_EXT_CLASS CRavidDlgDeviceConfigurationTool : public CRavidDialogBase
		{
			RavidPreventCopySelf(CRavidDlgDeviceConfigurationTool);

		public:
			DECLARE_DYNAMIC(CRavidDlgDeviceConfigurationTool)
			enum EDeviceType
			{
				EDeviceType_Camera = 0,
				EDeviceType_FrameGrabber,
				EDeviceType_Dio,
				EDeviceType_Motion,
				EDeviceType_Trigger,
				EDeviceType_Comm,
				EDeviceType_Count,
				EDeviceType_None,
			};

		public:
			CRavidDlgDeviceConfigurationTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
			virtual ~CRavidDlgDeviceConfigurationTool();

			virtual bool CreateMe(CWnd* pParent = nullptr);

		// 대화 상자 데이터입니다.
		// #ifdef AFX_DESIGN_TIME
		// 	enum { IDD = IDD_RAVID_DLG_DEVICE_CONFIGURATION_TOOL };
		// #endif

		protected:
			virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

			DECLARE_MESSAGE_MAP()
		public:
			virtual BOOL PreTranslateMessage(MSG* pMsg);
			virtual BOOL OnInitDialog();
			afx_msg void OnDestroy();

			afx_msg void OnPaint();
			afx_msg void OnBnClickedButtonRavidDlgDeviceAdd();
			afx_msg void OnBnClickedButtonRavidDlgDeviceDelete();

			afx_msg void OnBnClickedButtonRavidDlgDeviceUp();
			afx_msg void OnBnClickedButtonRavidDlgDeviceDown();

			afx_msg void OnBnClickedButtonRavidDlgDeviceSave();

			afx_msg void OnTvnSelchangedTreeRavidDlgDeviceRegister(NMHDR *pNMHDR, LRESULT *pResult);
			afx_msg void OnNMDblclkTreeRavidDlgDeviceRegister(NMHDR *pNMHDR, LRESULT *pResult);

			afx_msg void OnClose();

		public:
			virtual bool UpdateAvailableDeviceTree();
			virtual bool UpdateRegisterDeviceTree();

		protected:

			virtual HTREEITEM AddRegisteredDeviceTree(_In_ Device::SRegisterDeviceInfo sInfo, _In_opt_ HTREEITEM hAfterItem = TVI_LAST);

			virtual std::vector<HTREEITEM> GetSelectedRegisteredDeviceTreeRoot();

			virtual HTREEITEM GetRegisteredDeviceTreeRoot(_In_ HTREEITEM hItem);
			virtual HTREEITEM GetRegisteredDeviceTreeItem(_In_ HTREEITEM hItem);

			virtual EDeviceType GetDeviceType(_In_ Ravid::Device::CDeviceBase* pDevice);

			virtual long GetLastDeviceObjectID(_In_ CString strClass);

			virtual bool UpdateObjectID();

			virtual bool GetRegisteredDeviceTreeUpCondition();
			virtual bool GetRegisteredDeviceTreeDownCondition();

			virtual bool UpdateButtonState();

			virtual bool Save();

		protected:
			static LPCTSTR m_lpszDevice[CRavidDlgDeviceConfigurationTool::EDeviceType_Count];

			CRavidTreeCtrl m_treeAvailableDeviceList;
			CRavidTreeCtrl m_treeRegisterDeviceList;

			std::vector<std::pair<HTREEITEM, Device::SRegisterDeviceInfo>> m_vctRegisterDevice;

			std::vector<CString> m_vctDeviceHighNode[CRavidDlgDeviceConfigurationTool::EDeviceType_Count];

			bool m_bIsChanged = false;

		protected:
			virtual ptrdiff_t OnMessage(Ravid::Framework::CMessageBase* pMessage);
		};
	}
}
