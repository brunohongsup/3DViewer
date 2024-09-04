#pragma once

#include "RavidDialogBase.h"
#include <vector>

class CRavidTreeView;

namespace Ravid
{
	namespace Framework
	{
		class CRavidBehavior;
	}

	namespace Device
	{
		/**
		* 디바이스 계열의 기저 클래스입니다.
		*/
		class AFX_EXT_CLASS CDeviceBase : public Framework::CRavidDialogBase
		{
		public: 
			DECLARE_DYNAMIC(CDeviceBase)

			enum EDeviceBaseMessage
			{
				EDeviceBaseMessage_UpdateControls = 0x3250,
				EDeviceBaseMessage_Terminate,
				EDeviceBaseMessage_UpdateMessage,
				EDeviceBaseMessage_UpdateStatus,
			};

		public:
			CDeviceBase();
			virtual ~CDeviceBase();

			virtual ptrdiff_t OnMessage(_In_ Ravid::Framework::CMessageBase* pMessage);

			virtual void DoDataExchange(CDataExchange* pDX);
			virtual BOOL OnInitDialog();
			virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
			virtual BOOL PreTranslateMessage(MSG* pMsg);
			afx_msg void OnDestroy();
			afx_msg void OnSize(UINT nType, int cx, int cy);
			afx_msg void OnTimer(UINT_PTR nIDEvent);
			afx_msg void OnHitExpand(NMHDR* pNotifyStruct, LRESULT* plResult);
			afx_msg void OnTreeItemChanged(NMHDR* pNotifyStruct, LRESULT* plResult);
			afx_msg void OnCbnSelchangeComboDeviceList();
			afx_msg LRESULT OnUpdateControls(WPARAM wParam, LPARAM lParam);
			afx_msg LRESULT OnTerminate(WPARAM wParam, LPARAM lParam);
			afx_msg void OnClose();

		public:
			
			/**
			* 디바이스를 사용하기 위한 초기화 작업을 실행합니다.
			* 순수 가상함수로 꼭 정의해야 합니다.
			*
			* return value : 실패하면 EDeviceInitializeResult_OK 이외의 값을 가집니다.
			*/
			virtual EDeviceInitializeResult Initialize() = 0;
			
			/**
			* 디바이스의 리소스를 반환하기 위한 해제 작업을 실행합니다.
			* 순수 가상함수로 꼭 정의해야 합니다.
			*
			* return value : 실패하면 EDeviceTerminateResult_OK 이외의 값을 가집니다.
			*/
			virtual EDeviceTerminateResult Terminate() = 0;

		public:
			
			/**
			* 데이터 베이스에서 설정된 파라메터 값을 읽어옵니다.
			*
			* return value : 실패하면 false 값을 가집니다.
			*/
			virtual bool LoadSettings();

		protected:
			
			/**
			* 화면에 출력하기 위한 UI 컨트롤을 추가합니다.
			* 사용자는 서브클래스에 사용하고자 하는 UI 를 추가합니다.
			*
			* return value : 실패하면 false 값을 가집니다.
			*/
			virtual bool AddControls();

		public:
			virtual bool SaveSettings(_In_ int nSaveParameter);
			virtual bool SaveSettings(_In_opt_ CString strSavingParam = _T(""));

			virtual CString GetParamValue(_In_ int nParameter);
			virtual bool SetParamValue(_In_ int nParameter, _In_ CString strValue);

 			virtual CString GetParamValue(_In_ CString strParameter);
 			virtual bool SetParamValue(_In_ CString strParameter, _In_ CString strValue);

			virtual CString GetDeviceID();
			virtual EDeviceBaseIDResult SetDeviceID(_In_ CString strDeviceID);

			virtual CString GetSubUnitID();
			virtual EDeviceBaseIDResult SetSubUnitID(_In_ CString strSubUnitID);

			virtual bool IsInitialized();

			virtual bool Create(_In_opt_ CWnd* pParent = nullptr);

			virtual bool AddUndoData();
			virtual bool BackupUndoData(_In_ Framework::CRavidBehavior* pBehavior);

			virtual bool ClearUndoData();
			virtual bool ClearRedoData();

			virtual bool RangeUndoData();
			virtual bool RangeRedoData();

			virtual bool Undo();
			virtual bool Redo();

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue);

			virtual bool DoesModuleExist() = 0;
		public:
			DECLARE_MESSAGE_MAP()

		public:
			afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

		protected:

			virtual bool SetMessage(_In_ CString strMessage);
			virtual bool SetStatus(_In_ CString strStatus);

			virtual bool UpdateParameterList();
			virtual bool UpdateControls();

			virtual bool UpdateExpandSheet(_In_ int nIndex);

			virtual bool RecalcSheet();
			virtual bool RecalcSheetRow();

			virtual bool AddParameterFieldConfigurations(_In_ int nPrimaryKey, _In_ LPCTSTR lpszName, _In_ LPCTSTR lpszValue, _In_opt_ EParameterFieldType eType = EParameterFieldType_Edit, _In_opt_ LPCTSTR lpszComboList = nullptr, _In_opt_ LPCTSTR lpszToolTip = nullptr, _In_opt_ int nDepth = 0, _In_opt_ bool bShow = true);

			bool StringTokenize(_In_ CString str, _In_ CString strToken, _Out_ std::vector<CString>* pVct);

			CString ConvertStringArrayToComboElement(_In_ LPCTSTR* ppStringArray, _In_ size_t stCount);

			CString IntegerToString(_In_opt_ int nInteger = 0);

			int GetStringToFeatureIndex(_In_ CString strFeature, _In_ LPCTSTR* ppStringArray, _In_ size_t stCount);

			virtual bool UpdateParameterData(_In_ CString strParam, _In_ CString strValue);

			virtual bool ExtractIntroduction(_Inout_ CString& strSource, _In_ CString chDelimiter, _Out_ CString& strResult);

			virtual bool UpdateParameterListTest();

		protected:
			volatile bool m_bIsSheetEditMode;

			volatile bool m_bIsInitialized;

			volatile bool m_bInitializeSetting;

			ERavidSkinType m_ePreSkinType;

			int m_nDefaultSheetRowHeight;

			CString m_strStatus;
			CString m_strMessage;

			CRavidTreeView* m_pTreeParam;

			Framework::CRavidBehavior* m_pBackupUndoData;

			std::vector<long> m_vctRavidTreeViewID;

			std::vector<SDeviceParameterFieldConfigurations> m_vctParameterFieldConfigurations;

			std::vector<CWnd*> m_vctCtrl;

 			friend class CDeviceManager;

			std::deque<Framework::CRavidBehavior*> m_deqUndoBuffer;
			std::deque<Framework::CRavidBehavior*> m_deqRedoBuffer;
			
			virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
	}
}

