// CRavidLogViewer 폼 뷰입니다.

#pragma once

#include "RavidFormViewBase.h"

#include <queue>
#include <map>

namespace Ravid
{
	namespace Framework
	{
		class CRavidSheet;

		class AFX_EXT_CLASS CRavidLogViewer : public CRavidFormViewBase
		{
			RavidUseDynamicCreation();

			DECLARE_DYNCREATE(CRavidLogViewer)

			enum ETotalTableColumn
			{
				ETotalTableColumn_Time = 0,
				ETotalTableColumn_Classification,
				ETotalTableColumn_Importance,
				ETotalTableColumn_Detail,
				ETotalTableColumn_Count,
			};

			enum EOtherTableColumn
			{
				EOtherTableColumn_Time = 0,
				EOtherTableColumn_Importance,
				EOtherTableColumn_Detail,
				EOtherTableColumn_Count,
			};

			enum ELogViewerCheckBoxID
			{
				ELogViewerCheckBoxID_Normal = 1,
				ELogViewerCheckBoxID_Max = ELogViewerCheckBoxID_Normal + 9,
			};

		protected:
			CRavidLogViewer();           // 동적 만들기에 사용되는 protected 생성자입니다.
			CRavidLogViewer(const CRavidLogViewer& rlv) { }
			virtual ~CRavidLogViewer();

		public:
#ifdef AFX_DESIGN_TIME
			enum { IDD = IDD_RAVID_LOG_VIEWER };
#endif
#ifdef _DEBUG
			virtual void AssertValid() const;
#ifndef _WIN32_WCE
			virtual void Dump(CDumpContext& dc) const;
#endif
#endif

		protected:
			virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

			DECLARE_MESSAGE_MAP()
		public:
			virtual void OnInitialUpdate();
			afx_msg void OnSize(UINT nType, int cx, int cy);
			afx_msg void OnDestroy();
			afx_msg void OnTimer(UINT_PTR nIDEvent);
			afx_msg void OnCheckBoxClicked(UINT nID);

			bool Initialize(_In_ std::map<long, CString>* pLogTypes);

			void AddLogMessage(_In_ SRavidLogData* pLogData);
			bool RecalcTable();

			void DeleteAllMessage();

			long GetLimitLogDataCount();
			void SetLimitLogDataCount(long nCount = 10000);

			bool SetTabName(_In_ long nTypeID, _In_ CString strName);

			void Invalidate(bool bErase = true);

			CString GetImportanceTypeName(_In_ long nImportanceType);

			void EnableWrite(_In_ bool bEnable);

		protected:
			bool WriteLogMessage();

			bool AddTab(_In_ long nTypeID, _In_ CString strType);

			bool RecalcTotalSheet();
			bool RecalcOtherSheet(_In_ CRavidSheet* pSheet);

			CRavidSheet* GetSheet(_In_ long nTypeID);

			CRect m_crPreFrame = CRect(0, 0, 0, 0);

			std::queue<SRavidLogData*> m_queLogData;

			bool m_bIsTableAutoSizeCalcMode = true;

			long m_nDefaultTotalTableColumnSize[ETotalTableColumn_Count] = { 140, 100, 100, 500 };
			long m_nDefaultOtherTableColumnSize[EOtherTableColumn_Count] = { 140, 100, 500 };

			long m_nLogViewerMaxRows = 10000;

			CMFCTabCtrl m_ctrlTab;

			CRavidSheet* m_pTotalSheet = nullptr;

			std::map<long, CRavidSheet*> m_mapSheet;

			bool m_bEnableWrite = true;


			bool OutputLogMessage();
			bool WriteLogImportanceFiltering();

			bool IncOtherLogImportanceCount(_In_ long nType, _In_ long nImportanceType);
			bool DecOtherLogImportanceCount(_In_ long nType, _In_ long nImportanceType);
			long GetOtherLogImportanceCount(_In_ long nType, _In_ long nImportanceType);

			bool IncTotalLogImportanceCount(_In_ long nImportanceType);
			bool DecTotalLogImportanceCount(_In_ long nImportanceType);
			long GetTotalLogImportanceCount(_In_ long nImportanceType);

		private:
			bool SetImportanceTypeName(_In_ long nImportanceType, _In_ CString strImportanceName);
			bool CreateCheckBox(_In_ long nImportanceType, _In_ CString strTitle);
			bool AssignmentCheckBoxRect(_In_ CString strCheckBoxTitle, _Out_ CRect& rect);
			bool IsImportanceCheck(_In_ long nImportanceType);

			std::map<long, long> m_mapImportanceCheck;						// CheckBoxID / ImportanceType
			std::vector<CButton*> m_vctCheckBoxs;
			long m_nCheckBoxCount = 0;


			std::map<long, std::map<long, long>> m_mapLogCount;				// Type / ImportanceType / Count
			std::map<long, long> m_mapTotalLogCount;

			std::map<long, std::deque<SRavidLogData*>> m_mapLogDatas;		// Type / LogData
			std::deque<SRavidLogData*> m_dqueTotalLog;

			std::map<long, SRavidLogImportanceInfoData> m_mapImportanceTypes;	// ImportanceType / struct
		};
	}
}
