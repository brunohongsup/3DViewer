#pragma once

#include "RavidSheetCellRange.h"
#include "RavidSheetCell.h"
#include <afxtempl.h>
#include <vector>


#ifdef _WIN32_WCE
#   define GRIDCONTROL_NO_TITLETIPS
#   define GRIDCONTROL_NO_DRAGDROP 
#   define GRIDCONTROL_NO_CLIPBOARD
#   define GRIDCONTROL_NO_PRINTING 
#   ifdef WCE_NO_PRINTING			
#       define _WIN32_WCE_NO_PRINTING
#   endif
#   ifdef WCE_NO_CURSOR
#       define _WIN32_WCE_NO_CURSOR
#   endif
#endif  // _WIN32_WCE

#define GRIDCTRL_CLASSNAME    _T("MFCRavidSheet")
#define IDC_INPLACE_CONTROL   8                  



#ifndef GRIDCONTROL_NO_TITLETIPS
	#include "RavidSheetTitleTip.h"
#endif

#ifndef GRIDCONTROL_NO_DRAGDROP
#   include "RavidSheetDropTarget.h"
#   undef GRIDCONTROL_NO_CLIPBOARD
#endif

#ifndef GRIDCONTROL_NO_CLIPBOARD
#   include <afxole.h>
#endif



#define IsSHIFTpressed() ( (GetKeyState(VK_SHIFT) & (1 << (sizeof(SHORT)*8-1))) != 0   )
#define IsCTRLpressed()  ( (GetKeyState(VK_CONTROL) & (1 << (sizeof(SHORT)*8-1))) != 0 )

#define DDX_GridControl(pDX, nIDC, rControl)  DDX_Control(pDX, nIDC, rControl)     


namespace Ravid
{
	namespace Framework
	{
		typedef struct tagNM_GRIDVIEW
		{
			NMHDR hdr;
			int   iRow;
			int   iColumn;
		}
		NM_GRIDVIEW;

		typedef struct tagGV_DISPINFO
		{
			NMHDR   hdr;
			SRavidSheetCellInfo item;
		}
		GV_DISPINFO;

		typedef struct tagGV_CACHEHINT
		{
			NMHDR      hdr;
			CRavidSheetCellRange range;
		}
		GV_CACHEHINT;

		typedef CTypedPtrArray<CObArray, CRavidSheetCellBase*> GRID_ROW;

		typedef BOOL(CALLBACK* GRIDCALLBACK)(GV_DISPINFO *, LPARAM);


#define GVL_NONE                0L
#define GVL_HORZ                1L
#define GVL_VERT                2L
#define GVL_BOTH                3L

#define GVS_DEFAULT             0
#define GVS_HEADER              1
#define GVS_DATA                2
#define GVS_BOTH                3

#define GVNI_FOCUSED            0x0001
#define GVNI_SELECTED           0x0002
#define GVNI_DROPHILITED        0x0004
#define GVNI_READONLY           0x0008
#define GVNI_FIXED              0x0010
#define GVNI_MODIFIED           0x0020

#define GVNI_ABOVE              LVNI_ABOVE
#define GVNI_BELOW              LVNI_BELOW
#define GVNI_TOLEFT             LVNI_TOLEFT
#define GVNI_TORIGHT            LVNI_TORIGHT
#define GVNI_ALL                (LVNI_BELOW|LVNI_TORIGHT|LVNI_TOLEFT)
#define GVNI_AREA               (LVNI_BELOW|LVNI_TORIGHT)

#define GVHT_DATA               0x0000
#define GVHT_TOPLEFT            0x0001
#define GVHT_COLHDR             0x0002
#define GVHT_ROWHDR             0x0004
#define GVHT_COLSIZER           0x0008
#define GVHT_ROWSIZER           0x0010
#define GVHT_LEFT               0x0020
#define GVHT_RIGHT              0x0040
#define GVHT_ABOVE              0x0080
#define GVHT_BELOW              0x0100

#define GVN_BEGINDRAG           LVN_BEGINDRAG
#define GVN_BEGINLABELEDIT      LVN_BEGINLABELEDIT
#define GVN_BEGINRDRAG          LVN_BEGINRDRAG
#define GVN_COLUMNCLICK         LVN_COLUMNCLICK
#define GVN_DELETEITEM          LVN_DELETEITEM
#define GVN_ENDLABELEDIT        LVN_ENDLABELEDIT
#define GVN_SELCHANGING         LVN_ITEMCHANGING
#define GVN_SELCHANGED          LVN_ITEMCHANGED
#define GVN_GETDISPINFO         LVN_GETDISPINFO 
#define GVN_ODCACHEHINT         LVN_ODCACHEHINT 

#define GVN_CANCELEDIT			(LVN_FIRST - 1024)
#define GVN_HEADERSORTING		(LVN_FIRST - 1023)
#define GVN_CHANGEDCOLUMNWIDTH		(LVN_FIRST - 1022)
#define GVN_CUTSELECTEDTEXT			(LVN_FIRST - 1021)
#define GVN_EDITCUT				(LVN_FIRST - 1020)
#define GVN_EDITCOPY			(LVN_FIRST - 1019)
#define GVN_EDITPASTE			(LVN_FIRST - 1018)
#define GVN_SETCHEKED			(LVN_FIRST - 1017)

		class CRavidSheet;

		typedef bool(*PVIRTUALCOMPARE)(int, int);

		class AFX_EXT_CLASS CRavidSheet : public CWnd
		{
			DECLARE_DYNCREATE(CRavidSheet)
			friend class CRavidSheetCell;
			friend class CRavidSheetCellBase;

		public:
			CRavidSheet();

			CRavidSheet(int nRows, int nCols, int nFixedRows, int nFixedCols);

			BOOL Create(RECT& rect, CWnd* parent, UINT nID,
						DWORD dwStyle = WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE);

		public:
			int  GetRowCount() { return m_nRows; }
			int  GetColumnCount() { return m_nCols; }
			int  GetFixedRowCount() { return m_nFixedRows; }
			int  GetFixedColumnCount() { return m_nFixedCols; }
			BOOL SetRowCount(int nRows = 10);
			BOOL SetColumnCount(int nCols = 10);
			BOOL SetFixedRowCount(int nFixedRows = 1);
			BOOL SetFixedColumnCount(int nFixedCols = 1);

			int  GetRowHeight(int nRow);
			BOOL SetRowHeight(int row, int height);
			int  GetColumnWidth(int nCol);
			BOOL SetColumnWidth(int col, int width);

			BOOL GetCellOrigin(int nRow, int nCol, LPPOINT p);
			BOOL GetCellOrigin(CRavidSheetCellID& cell, LPPOINT p);
			BOOL GetCellRect(int nRow, int nCol, LPRECT pRect);
			BOOL GetCellRect(CRavidSheetCellID& cell, LPRECT pRect);

			BOOL GetTextRect(CRavidSheetCellID& cell, LPRECT pRect);
			BOOL GetTextRect(int nRow, int nCol, LPRECT pRect);

			CRavidSheetCellID GetCellFromPt(CPoint point, BOOL bAllowFixedCellCheck = TRUE);

			int  GetFixedRowHeight();
			int  GetFixedColumnWidth();
			long GetVirtualWidth();
			long GetVirtualHeight();

			CSize GetTextExtent(int nRow, int nCol, LPCTSTR str);
			inline CSize GetCellTextExtent(int nRow, int nCol) { return GetTextExtent(nRow, nCol, GetItemText(nRow, nCol)); }

			void     SetGridBkColor(COLORREF clr) { m_crGridBkColour = clr; }
			COLORREF GetGridBkColor() { return m_crGridBkColour; }
			void	 SetGridEdgeColor(COLORREF clr) { m_crGridEdge = clr; }
			COLORREF GetGridEdgeColor() { return m_crGridEdge; }
			void     SetGridLineColor(COLORREF clr) { m_crGridLineColour = clr; }
			COLORREF GetGridLineColor() { return m_crGridLineColour; }

			void	 SetTitleTipBackClr(COLORREF clr = CLR_DEFAULT) { m_crTTipBackClr = clr; }
			COLORREF GetTitleTipBackClr() { return m_crTTipBackClr; }
			void	 SetTitleTipTextClr(COLORREF clr = CLR_DEFAULT) { m_crTTipTextClr = clr; }
			COLORREF GetTitleTipTextClr() { return m_crTTipTextClr; }

			void     SetTextColor(COLORREF clr) { m_cellDefault.SetTextClr(clr); }
			COLORREF GetTextColor() { return m_cellDefault.GetTextClr(); }
			void     SetTextBkColor(COLORREF clr) { m_cellDefault.SetBackClr(clr); }
			COLORREF GetTextBkColor() { return m_cellDefault.GetBackClr(); }
			void     SetFixedTextColor(COLORREF clr)
			{
				m_cellFixedRowDef.SetTextClr(clr);
				m_cellFixedColDef.SetTextClr(clr);
				m_cellFixedRowColDef.SetTextClr(clr);
			}
			COLORREF GetFixedTextColor() { return m_cellFixedRowDef.GetTextClr(); }
			void     SetFixedBkColor(COLORREF clr)
			{
				m_cellFixedRowDef.SetBackClr(clr);
				m_cellFixedColDef.SetBackClr(clr);
				m_cellFixedRowColDef.SetBackClr(clr);
			}
			COLORREF GetFixedBkColor() { return m_cellFixedRowDef.GetBackClr(); }
			void     SetGridColor(COLORREF clr) { SetGridLineColor(clr); }
			COLORREF GetGridColor() { return GetGridLineColor(); }
			void     SetBkColor(COLORREF clr) { SetGridBkColor(clr); }
			COLORREF GetBkColor() { return GetGridBkColor(); }

			void     SetDefCellMargin(int nMargin)
			{
				m_cellDefault.SetMargin(nMargin);
				m_cellFixedRowDef.SetMargin(nMargin);
				m_cellFixedColDef.SetMargin(nMargin);
				m_cellFixedRowColDef.SetMargin(nMargin);
			}
			int      GetDefCellMargin() { return m_cellDefault.GetMargin(); }

			int      GetDefCellHeight() { return m_cellDefault.GetHeight(); }
			void     SetDefCellHeight(int nHeight)
			{
				m_cellDefault.SetHeight(nHeight);
				m_cellFixedRowDef.SetHeight(nHeight);
				m_cellFixedColDef.SetHeight(nHeight);
				m_cellFixedRowColDef.SetHeight(nHeight);
			}
			int      GetDefCellWidth() { return m_cellDefault.GetWidth(); }
			void     SetDefCellWidth(int nWidth)
			{
				m_cellDefault.SetWidth(nWidth);
				m_cellFixedRowDef.SetWidth(nWidth);
				m_cellFixedColDef.SetWidth(nWidth);
				m_cellFixedRowColDef.SetWidth(nWidth);
			}


			int GetSelectedCount() { return (int)m_SelectedCellMap.GetCount(); }

			CRavidSheetCellID SetFocusCell(CRavidSheetCellID cell);
			CRavidSheetCellID SetFocusCell(int nRow, int nCol);
			CRavidSheetCellID GetFocusCell() { return m_idCurrentCell; }


			void SetVirtualMode(BOOL bVirtual);
			BOOL GetVirtualMode() { return m_bVirtualMode; }
			void SetCallbackFunc(GRIDCALLBACK pCallback,
								 LPARAM lParam)
			{
				m_pfnCallback = pCallback; m_lParam = lParam;
			}
			GRIDCALLBACK GetCallbackFunc() { return m_pfnCallback; }


			void SetImageList(CImageList* pList) { m_pImageList = pList; }
			CImageList* GetImageList() { return m_pImageList; }

			void SetGridLines(int nWhichLines = GVL_BOTH);
			int  GetGridLines() { return m_nGridLines; }
			void SetEditable(BOOL bEditable = TRUE) { m_bEditable = bEditable; }
			BOOL IsEditable() { return m_bEditable; }
			void SetListMode(BOOL bEnableListMode = TRUE);
			BOOL GetListMode() { return m_bListMode; }
			void SetSingleRowSelection(BOOL bSing = TRUE) { m_bSingleRowSelection = bSing; }
			BOOL GetSingleRowSelection() { return m_bSingleRowSelection & m_bListMode; }
			void SetSingleColSelection(BOOL bSing = TRUE) { m_bSingleColSelection = bSing; }
			BOOL GetSingleColSelection() { return m_bSingleColSelection; }
			void EnableSelection(BOOL bEnable = TRUE) { ResetSelectedRange(); m_bEnableSelection = bEnable; ResetSelectedRange(); }
			BOOL IsSelectable() { return m_bEnableSelection; }
			void EnableMultiSelectionCell(bool bEnable) { m_bEnableMultiSelection = bEnable; }
			bool IsMultiSelectableCell() { return m_bEnableMultiSelection; }
			void SetFixedColumnSelection(BOOL bSelect) { m_bFixedColumnSelection = bSelect; }
			BOOL GetFixedColumnSelection() { return m_bFixedColumnSelection; }
			void SetFixedRowSelection(BOOL bSelect) { m_bFixedRowSelection = bSelect; }
			BOOL GetFixedRowSelection() { return m_bFixedRowSelection; }
			void EnableDragAndDrop(BOOL bAllow = TRUE) { m_bAllowDragAndDrop = bAllow; }
			BOOL GetDragAndDrop() { return m_bAllowDragAndDrop; }
			void SetRowResize(BOOL bResize = TRUE) { m_bAllowRowResize = bResize; }
			BOOL GetRowResize() { return m_bAllowRowResize; }
			void SetColumnResize(BOOL bResize = TRUE);
			BOOL GetColumnResize() { return m_bAllowColumnResize; }

			void SetColumnResize(int nCol, bool bResize);


			void SetHeaderSort(BOOL bSortOnClick = TRUE) { m_bSortOnClick = bSortOnClick; }
			BOOL GetHeaderSort() { return m_bSortOnClick; }
			void SetHandleTabKey(BOOL bHandleTab = TRUE) { m_bHandleTabKey = bHandleTab; }
			BOOL GetHandleTabKey() { return m_bHandleTabKey; }
			void SetDoubleBuffering(BOOL bBuffer = TRUE) { m_bDoubleBuffer = bBuffer; }
			BOOL GetDoubleBuffering() { return m_bDoubleBuffer; }
			void EnableTitleTips(BOOL bEnable = TRUE) { m_bTitleTips = bEnable; }
			BOOL GetTitleTips() { return m_bTitleTips; }
			void SetSortColumn(int nCol);
			int  GetSortColumn() { return m_nSortColumn; }
			void SetSortAscending(BOOL bAscending) { m_bAscending = bAscending; }
			BOOL GetSortAscending() { return m_bAscending; }
			void SetTrackFocusCell(BOOL bTrack) { m_bTrackFocusCell = bTrack; }
			BOOL GetTrackFocusCell() { return m_bTrackFocusCell; }
			void SetFrameFocusCell(BOOL bFrame) { m_bFrameFocus = bFrame; }
			BOOL GetFrameFocusCell() { return m_bFrameFocus; }
			void SetAutoSizeStyle(int nStyle = GVS_BOTH) { m_nAutoSizeColumnStyle = nStyle; }
			int  GetAutoSizeStyle() { return m_nAutoSizeColumnStyle; }

			void EnableHiddenColUnhide(BOOL bEnable = TRUE) { m_bHiddenColUnhide = bEnable; }
			BOOL GetHiddenColUnhide() { return m_bHiddenColUnhide; }
			void EnableHiddenRowUnhide(BOOL bEnable = TRUE) { m_bHiddenRowUnhide = bEnable; }
			BOOL GetHiddenRowUnhide() { return m_bHiddenRowUnhide; }

			void EnableColumnHide(BOOL bEnable = TRUE) { m_bAllowColHide = bEnable; }
			BOOL GetColumnHide() { return m_bAllowColHide; }
			void EnableRowHide(BOOL bEnable = TRUE) { m_bAllowRowHide = bEnable; }
			BOOL GetRowHide() { return m_bAllowRowHide; }

		public:
			CRavidSheetCellBase* GetDefaultCell(BOOL bFixedRow, BOOL bFixedCol);

		public:
			CRavidSheetCellBase* GetCell(int nRow, int nCol);

			void SetModified(BOOL bModified = TRUE, int nRow = -1, int nCol = -1);
			BOOL GetModified(int nRow = -1, int nCol = -1);
			BOOL IsCellFixed(int nRow, int nCol);

			BOOL   SetItem(SRavidSheetCellInfo* pItem);
			BOOL   GetItem(SRavidSheetCellInfo* pItem);
			BOOL   SetItemText(int nRow, int nCol, LPCTSTR str);
			CString GetItemText(int nRow, int nCol);

#if !defined(_WIN32_WCE) || (_WIN32_WCE >= 210)
			BOOL   SetItemTextFmt(int nRow, int nCol, LPCTSTR szFmt, ...);
			BOOL   SetItemTextFmtID(int nRow, int nCol, UINT nID, ...);
#endif

			BOOL   SetItemData(int nRow, int nCol, LPARAM lParam);
			LPARAM GetItemData(int nRow, int nCol);
			BOOL   SetItemImage(int nRow, int nCol, int iImage);
			int    GetItemImage(int nRow, int nCol);
			BOOL   SetItemState(int nRow, int nCol, UINT state);
			UINT   GetItemState(int nRow, int nCol);
			BOOL   SetItemFormat(int nRow, int nCol, UINT nFormat);
			UINT   GetItemFormat(int nRow, int nCol);
			BOOL   SetItemBkColour(int nRow, int nCol, COLORREF cr = CLR_DEFAULT);
			COLORREF GetItemBkColour(int nRow, int nCol);
			BOOL   SetItemFgColour(int nRow, int nCol, COLORREF cr = CLR_DEFAULT);
			COLORREF GetItemFgColour(int nRow, int nCol);
			BOOL SetItemFont(int nRow, int nCol, LOGFONT* lf);
			LOGFONT* GetItemFont(int nRow, int nCol);

			BOOL IsItemEditing(int nRow, int nCol);

			BOOL SetCellType(int nRow, int nCol, CRuntimeClass* pRuntimeClass);
			BOOL SetDefaultCellType(CRuntimeClass* pRuntimeClass);

		public:
			int  InsertColumn(LPCTSTR strHeading, UINT nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE,
							  int nColumn = -1);
			int  InsertRow(LPCTSTR strHeading, int nRow = -1);
			BOOL DeleteColumn(int nColumn);
			BOOL DeleteRow(int nRow);
			BOOL DeleteNonFixedRows();
			BOOL DeleteAllItems();

			void ClearCells(CRavidSheetCellRange Selection);

			BOOL AutoSizeRow(int nRow, BOOL bResetScroll = TRUE);
			BOOL AutoSizeColumn(int nCol, UINT nAutoSizeStyle = GVS_DEFAULT, BOOL bResetScroll = TRUE);
			void AutoSizeRows();
			void AutoSizeColumns(UINT nAutoSizeStyle = GVS_DEFAULT);
			void AutoSize(UINT nAutoSizeStyle = GVS_DEFAULT);
			void ExpandColumnsToFit(BOOL bExpandFixed = TRUE);
			void ExpandLastColumn();
			void ExpandRowsToFit(BOOL bExpandFixed = TRUE);
			void ExpandToFit(BOOL bExpandFixed = TRUE);

			void Refresh();
			void AutoFill();

			void EnsureVisible(CRavidSheetCellID& cell) { EnsureVisible((size_t)cell.m_nRow, (size_t)cell.m_nCol); }
			void EnsureVisible(size_t stRow, size_t stCol);
			BOOL IsCellVisible(int nRow, int nCol);
			BOOL IsCellVisible(CRavidSheetCellID cell);
			BOOL IsCellEditable(int nRow, int nCol);
			BOOL IsCellEditable(CRavidSheetCellID &cell);
			BOOL IsCellSelected(int nRow, int nCol);
			BOOL IsCellSelected(CRavidSheetCellID &cell);

			void SetRedraw(BOOL bAllowDraw, BOOL bResetScrollBars = FALSE);
			BOOL RedrawCell(int nRow, int nCol, CDC* pDC = NULL);
			BOOL RedrawCell(CRavidSheetCellID& cell, CDC* pDC = NULL);
			BOOL RedrawRow(int row);
			BOOL RedrawColumn(int col);

			CRavidSheetCellID GetRecentClickDownCell();

#ifndef _WIN32_WCE
			BOOL Save(LPCTSTR filename, TCHAR chSeparator = _T(','));
			BOOL Load(LPCTSTR filename, TCHAR chSeparator = _T(','));
#endif

		public:
			CRavidSheetCellRange GetRavidSheetCellRange();
			CRavidSheetCellRange GetSelectedRavidSheetCellRange();
			void SetSelectedRange(CRavidSheetCellRange& Range, BOOL bForceRepaint = FALSE, BOOL bSelectCells = TRUE);
			void SetSelectedRange(int nMinRow, int nMinCol, int nMaxRow, int nMaxCol,
								  BOOL bForceRepaint = FALSE, BOOL bSelectCells = TRUE);
			BOOL IsValid(int nRow, int nCol);
			BOOL IsValid(CRavidSheetCellID& cell);
			BOOL IsValid(CRavidSheetCellRange& range);

			std::vector<CRavidSheetCellID> GetSelectedCellID();

#ifndef GRIDCONTROL_NO_CLIPBOARD
			virtual void CutSelectedText();
			virtual COleDataSource* CopyTextFromGrid();
			virtual BOOL PasteTextToGrid(CRavidSheetCellID cell, COleDataObject* pDataObject, BOOL bSelectPastedCells = TRUE);
#endif

#ifndef GRIDCONTROL_NO_DRAGDROP
		public:
			virtual void OnBeginDrag();
			virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
			virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
			virtual void OnDragLeave();
			virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
#endif

#ifndef GRIDCONTROL_NO_CLIPBOARD
			virtual void OnEditCut();
			virtual void OnEditCopy();
			virtual void OnEditPaste();
#endif
			virtual void OnEditSelectAll();

		public:
			CRavidSheetCellID GetNextItem(CRavidSheetCellID& cell, int nFlags);

			BOOL SortItems(int nCol, BOOL bAscending, LPARAM data = 0);
			BOOL SortTextItems(int nCol, BOOL bAscending, LPARAM data = 0);
			BOOL SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data = 0);

			BOOL SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data, int low, int high);

			void SetCompareFunction(PFNLVCOMPARE pfnCompare);

			static int CALLBACK pfnCellTextCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
			static int CALLBACK pfnCellNumericCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

#if !defined(_WIN32_WCE_NO_PRINTING) && !defined(GRIDCONTROL_NO_PRINTING)
		public:
			void Print(CPrintDialog* pPrntDialog = NULL);

			void EnableWysiwygPrinting(BOOL bEnable = TRUE) { m_bWysiwygPrinting = bEnable; }
			BOOL GetWysiwygPrinting() { return m_bWysiwygPrinting; }

			void SetShadedPrintOut(BOOL bEnable = TRUE) { m_bShadedPrintOut = bEnable; }
			BOOL GetShadedPrintOut(void) { return m_bShadedPrintOut; }

			void SetPrintMarginInfo(int nHeaderHeight, int nFooterHeight,
									int nLeftMargin, int nRightMargin, int nTopMargin,
									int nBottomMargin, int nGap);

			void GetPrintMarginInfo(int &nHeaderHeight, int &nFooterHeight,
									int &nLeftMargin, int &nRightMargin, int &nTopMargin,
									int &nBottomMargin, int &nGap);

		public:
			virtual void OnBeginPrinting(CDC *pDC, CPrintInfo *pInfo);
			virtual void OnPrint(CDC *pDC, CPrintInfo *pInfo);
			virtual void OnEndPrinting(CDC *pDC, CPrintInfo *pInfo);

#endif // #if !defined(_WIN32_WCE_NO_PRINTING) && !defined(GRIDCONTROL_NO_PRINTING)

		public:
			virtual ~CRavidSheet();

			BOOL IsVisibleVScroll() { return ((m_nBarState & GVL_VERT) > 0); }
			BOOL IsVisibleHScroll() { return ((m_nBarState & GVL_HORZ) > 0); }

			void ResetSelectedRange();
			void ResetScrollBars();
			void EnableScrollBars(int nBar, BOOL bEnable = TRUE);
			int  GetScrollPos32(int nBar, BOOL bGetTrackPos = FALSE);
			BOOL SetScrollPos32(int nBar, int nPos, BOOL bRedraw = TRUE);

			bool SetScroll(int nBar, int nTarget);

			virtual bool ShiftData(int nExceptionRow, int nCount);
			virtual void TrackingNewData(bool bUse);

			bool UsingScrollBars(int nBar, bool bUsing);

		protected:
			BOOL RegisterWindowClass();
			BOOL Initialise();
			void SetupDefaultCells();

			LRESULT SendMessageToParent(int nRow, int nCol, int nMessage);
			LRESULT SendDisplayRequestToParent(GV_DISPINFO* pDisplayInfo);
			LRESULT SendCacheHintToParent(CRavidSheetCellRange& range);

			BOOL InvalidateCellRect(int row, int col);
			BOOL InvalidateCellRect(CRavidSheetCellID& cell);
			BOOL InvalidateCellRect(CRavidSheetCellRange& cellRange);
			void EraseBkgnd(CDC* pDC);

			BOOL GetRavidSheetCellRangeRect(CRavidSheetCellRange& cellRange, LPRECT lpRect);

			BOOL SetCell(int nRow, int nCol, CRavidSheetCellBase* pCell);

			int  SetMouseMode(int nMode) { int nOldMode = m_MouseMode; m_MouseMode = nMode; return nOldMode; }
			int  GetMouseMode() { return m_MouseMode; }

			BOOL MouseOverRowResizeArea(CPoint& point);
			BOOL MouseOverColumnResizeArea(CPoint& point);

			CRavidSheetCellID GetTopleftNonFixedCell(BOOL bForceRecalculation = FALSE);
			CRavidSheetCellRange GetUnobstructedNonFixedRavidSheetCellRange(BOOL bForceRecalculation = FALSE);
			CRavidSheetCellRange GetVisibleNonFixedRavidSheetCellRange(LPRECT pRect = NULL, BOOL bForceRecalculation = FALSE);
			CRavidSheetCellRange GetVisibleFixedRavidSheetCellRange(LPRECT pRect = NULL, BOOL bForceRecalculation = FALSE);

			CPoint GetPointClicked(int nRow, int nCol, CPoint& point);

			void ValidateAndModifyCellContents(int nRow, int nCol, LPCTSTR strText);

			long GetColumnStartPos(int nMinVisibleCol);
			long GetRowStartPos(int nMinVisibleRow);

		protected:
			virtual void PreSubclassWindow();

		protected:
#if !defined(_WIN32_WCE_NO_PRINTING) && !defined(GRIDCONTROL_NO_PRINTING)
			virtual void PrintFixedRowCells(int nStartColumn, int nStopColumn, int& row, CRect& rect,
											CDC *pDC, BOOL& bFirst);
			virtual void PrintColumnHeadings(CDC *pDC, CPrintInfo *pInfo);
			virtual void PrintHeader(CDC *pDC, CPrintInfo *pInfo);
			virtual void PrintFooter(CDC *pDC, CPrintInfo *pInfo);
			virtual void PrintRowButtons(CDC *pDC, CPrintInfo* /*pInfo*/);
#endif

#ifndef GRIDCONTROL_NO_DRAGDROP
			virtual CImageList* CreateDragImage(CPoint *pHotSpot);    // no longer necessary
#endif

			virtual void  OnFixedColumnClick(CRavidSheetCellID& cell);
			virtual void  OnFixedRowClick(CRavidSheetCellID& cell);

			virtual void  OnEditCell(int nRow, int nCol, CPoint point, UINT nChar);
			virtual void  OnEndEditCell(int nRow, int nCol, CString str);
			virtual BOOL  ValidateEdit(int nRow, int nCol, LPCTSTR str);
			virtual void  EndEditing();

			virtual void  OnDraw(CDC* pDC);

			virtual CRavidSheetCellBase* CreateCell(int nRow, int nCol);
			virtual void DestroyCell(int nRow, int nCol);


			int m_nLastMax;
		protected:
			COLORREF    m_crFixedTextColour, m_crFixedBkColour;
			COLORREF    m_crGridBkColour, m_crGridLineColour;
			COLORREF	m_crGridEdge;
			COLORREF    m_crWindowText, m_crWindowColour, m_cr3DFace,
				m_crShadow;
			COLORREF    m_crTTipBackClr, m_crTTipTextClr;            

			BOOL        m_bVirtualMode;
			LPARAM      m_lParam;                                    
			GRIDCALLBACK m_pfnCallback;                              

			int         m_nGridLines;
			BOOL        m_bEditable;
			BOOL        m_bModified;
			BOOL        m_bAllowDragAndDrop;
			BOOL        m_bListMode;
			BOOL        m_bSingleRowSelection;
			BOOL        m_bSingleColSelection;
			BOOL        m_bAllowDraw;
			BOOL        m_bEnableSelection;
			bool		m_bEnableMultiSelection;
			BOOL        m_bFixedRowSelection, m_bFixedColumnSelection;
			BOOL        m_bSortOnClick;
			BOOL        m_bHandleTabKey;
			BOOL        m_bDoubleBuffer;
			BOOL        m_bTitleTips;
			int         m_nBarState;
			BOOL        m_bWysiwygPrinting;
			BOOL        m_bHiddenColUnhide, m_bHiddenRowUnhide;
			BOOL        m_bAllowColHide, m_bAllowRowHide;
			BOOL        m_bAutoSizeSkipColHdr;
			BOOL        m_bTrackFocusCell;
			BOOL        m_bFrameFocus;
			UINT        m_nAutoSizeColumnStyle;

			int         m_nRows, m_nFixedRows, m_nCols, m_nFixedCols;
			CUIntArray  m_arRowHeights, m_arColWidths;
			int         m_nVScrollMax, m_nHScrollMax;

			CRuntimeClass*   m_pRtcDefault;
			CRavidSheetDefaultCell m_cellDefault;
			CRavidSheetDefaultCell m_cellFixedColDef, m_cellFixedRowDef, m_cellFixedRowColDef;
			CFont       m_PrinterFont;
			CImageList* m_pImageList;

			CTypedPtrArray<CObArray, GRID_ROW*> m_RowData;

			int         m_MouseMode;
			BOOL        m_bLMouseButtonDown, m_bRMouseButtonDown;
			CPoint      m_LeftClickDownPoint, m_LastMousePoint;
			CRavidSheetCellID     m_LeftClickDownCell, m_SelectionStartCell;
			CRavidSheetCellID     m_idCurrentCell, m_idTopLeftCell;
			INT_PTR     m_nTimerID;
			int         m_nTimerInterval;
			int         m_nResizeCaptureRange;
			BOOL        m_bAllowRowResize, m_bAllowColumnResize;
			int         m_nRowsPerWheelNotch;
			CMap<DWORD, DWORD, CRavidSheetCellID, CRavidSheetCellID&> m_SelectedCellMap, m_PrevSelectedCellMap;

#ifndef GRIDCONTROL_NO_TITLETIPS
			CRavidSheetTitleTip   m_TitleTip;
#endif

			CRavidSheetCellID     m_LastDragOverCell;
#ifndef GRIDCONTROL_NO_DRAGDROP
			CRavidSheetDropTarget m_DropTarget;
#endif

			CSize       m_CharSize;
			int         m_nPageHeight;
			CSize       m_LogicalPageSize,
				m_PaperSize;            
			int         m_nPageWidth;
			int         m_nPrintColumn;
			int         m_nCurrPrintRow;
			int         m_nNumPages;
			int         m_nPageMultiplier;

			int          m_bAscending;
			int          m_nSortColumn;
			PFNLVCOMPARE m_pfnCompare;

			BOOL        m_bShadedPrintOut;

			bool m_bTrackingNewData = false;
			bool m_bLastCursor = true;

			int         m_nHeaderHeight, m_nFooterHeight, m_nLeftMargin,
				m_nRightMargin, m_nTopMargin, m_nBottomMargin, m_nGap;

		protected:
			void SelectAllCells();
			void SelectColumns(CRavidSheetCellID currentCell, BOOL bForceRedraw = FALSE, BOOL bSelectCells = TRUE);
			void SelectRows(CRavidSheetCellID currentCell, BOOL bForceRedraw = FALSE, BOOL bSelectCells = TRUE);
			void SelectCells(CRavidSheetCellID currentCell, BOOL bForceRedraw = FALSE, BOOL bSelectCells = TRUE);
			void OnSelecting(CRavidSheetCellID& currentCell);

			afx_msg void OnPaint();
			afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
			afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
			afx_msg void OnSize(UINT nType, int cx, int cy);
			afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
			afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
			afx_msg void OnMouseMove(UINT nFlags, CPoint point);
			afx_msg void OnTimer(UINT_PTR nIDEvent);
			afx_msg UINT OnGetDlgCode();
			afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
			afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
			afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
			afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
			afx_msg BOOL OnEraseBkgnd(CDC* pDC);
			afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
			afx_msg void OnUpdateEditSelectAll(CCmdUI* pCmdUI);
#ifndef _WIN32_WCE_NO_CURSOR
			afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
#endif
#ifndef _WIN32_WCE
			afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
			afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
			afx_msg void OnSysColorChange();
#endif
#ifndef _WIN32_WCE_NO_CURSOR
			afx_msg void OnCaptureChanged(CWnd *pWnd);
#endif
#ifndef GRIDCONTROL_NO_CLIPBOARD
			afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
			afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
			afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
#endif
#if (_MFC_VER >= 0x0421) || (_WIN32_WCE >= 210)
			afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
#endif
#if !defined(_WIN32_WCE) && (_MFC_VER >= 0x0421)
			afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
#endif
			afx_msg LRESULT OnSetFont(WPARAM hFont, LPARAM lParam);
			afx_msg LRESULT OnGetFont(WPARAM hFont, LPARAM lParam);
			afx_msg LRESULT OnImeChar(WPARAM wCharCode, LPARAM lParam);
			afx_msg void OnEndRavidSheetInPlaceEdit(NMHDR* pNMHDR, LRESULT* pResult);
			afx_msg void OnCancelRavidSheetInPlaceEdit(NMHDR* pNMHDR, LRESULT* pResult);
			DECLARE_MESSAGE_MAP()

		public:
			enum eMouseModes
			{
				MOUSE_NOTHING, MOUSE_SELECT_ALL, MOUSE_SELECT_COL, MOUSE_SELECT_ROW,
				MOUSE_SELECT_CELLS, MOUSE_SCROLLING_CELLS,
				MOUSE_OVER_ROW_DIVIDE, MOUSE_SIZING_ROW,
				MOUSE_OVER_COL_DIVIDE, MOUSE_SIZING_COL,
				MOUSE_PREPARE_EDIT,
#ifndef GRIDCONTROL_NO_DRAGDROP
				MOUSE_PREPARE_DRAG, MOUSE_DRAGGING
#endif
			};
		public:
			typedef std::vector<int> intlist;
			void Reorder(int From, int To);
			void SetVirtualCompare(PVIRTUALCOMPARE VirtualCompare) { m_pfnVirtualCompare = VirtualCompare; }
			int m_CurCol;
			void AllowReorderColumn(bool b = true) { m_AllowReorderColumn = b; }
			void EnableDragRowMode(bool b = true) { m_bDragRowMode = b; if(b) EnableDragAndDrop(); }
			int GetLayer(int** pLayer);
			void SetLayer(int* pLayer);
			void ForceQuitFocusOnTab(bool b = true) { m_QuitFocusOnTab = b; }
			void AllowSelectRowInFixedCol(bool b = true) { m_AllowSelectRowInFixedCol = b; }
			intlist m_arRowOrder, m_arColOrder;
			static CRavidSheet* m_This;
		protected:
			virtual void AddSubVirtualRow(int Num, int Nb);
			bool m_bDragRowMode;
			int m_CurRow;
		private:
			void ResetVirtualOrder();
			PVIRTUALCOMPARE m_pfnVirtualCompare;
			static bool NotVirtualCompare(int c1, int c2);
			bool m_InDestructor;
			bool m_AllowReorderColumn;
			bool m_QuitFocusOnTab;
			bool m_AllowSelectRowInFixedCol;

			std::vector<int> m_vctDisabledColumnWidthResize;

			bool m_bUsingVertScrollBar = true;
			bool m_bUsingHorzScrollBar = true;
		};

		inline CRavidSheetCellBase* CRavidSheet::GetDefaultCell(BOOL bFixedRow, BOOL bFixedCol)
		{
			if(bFixedRow && bFixedCol) return (CRavidSheetCellBase*)&m_cellFixedRowColDef;
			if(bFixedRow)              return (CRavidSheetCellBase*)&m_cellFixedRowDef;
			if(bFixedCol)              return (CRavidSheetCellBase*)&m_cellFixedColDef;
			return (CRavidSheetCellBase*)&m_cellDefault;
		}

		inline CRavidSheetCellBase* CRavidSheet::GetCell(int nRow, int nCol)
		{
			if(nRow < 0 || nRow >= m_nRows || nCol < 0 || nCol >= m_nCols)
				return NULL;

			if(GetVirtualMode())
			{
				CRavidSheetCellBase* pCell = GetDefaultCell(nRow < m_nFixedRows, nCol < m_nFixedCols);
				static GV_DISPINFO gvdi;
				gvdi.item.nRow = nRow;
				gvdi.item.nCol = nCol;
				gvdi.item.uiMask = 0xFFFFFFFF;
				gvdi.item.uiState = 0;
				gvdi.item.dwFormat = pCell->GetFormat();
				gvdi.item.nImage = pCell->GetImage();
				gvdi.item.clrBackground = pCell->GetBackClr();
				gvdi.item.clrForground = pCell->GetTextClr();
				gvdi.item.lParam = pCell->GetData();
				memcpy(&gvdi.item.lfFont, pCell->GetFont(), sizeof(LOGFONT));
				gvdi.item.uiMargin = pCell->GetMargin();
				gvdi.item.strText.Empty();

				if(IsCellSelected(nRow, nCol))   gvdi.item.uiState |= ERavidSheetCellStatus_Selected;
				if(nRow < GetFixedRowCount())    gvdi.item.uiState |= (ERavidSheetCellStatus_Fixed | ERavidSheetCellStatus_FixedRow);
				if(nCol < GetFixedColumnCount()) gvdi.item.uiState |= (ERavidSheetCellStatus_Fixed | ERavidSheetCellStatus_FixedCol);
				if(GetFocusCell() == CRavidSheetCellID(nRow, nCol)) gvdi.item.uiState |= ERavidSheetCellStatus_Focused;
				if(!m_InDestructor)
				{
					gvdi.item.nRow = m_arRowOrder[nRow];
					gvdi.item.nCol = m_arColOrder[nCol];

					if(m_pfnCallback)
						m_pfnCallback(&gvdi, m_lParam);
					else
						SendDisplayRequestToParent(&gvdi);
					gvdi.item.nRow = nRow;
					gvdi.item.nCol = nCol;
				}
				static CRavidSheetCell cell;
				cell.SetState(gvdi.item.uiState);
				cell.SetFormat(gvdi.item.dwFormat);
				cell.SetImage(gvdi.item.nImage);
				cell.SetBackClr(gvdi.item.clrBackground);
				cell.SetTextClr(gvdi.item.clrForground);
				cell.SetData(gvdi.item.lParam);
				cell.SetFont(&(gvdi.item.lfFont));
				cell.SetMargin(gvdi.item.uiMargin);
				cell.SetText(gvdi.item.strText);
				cell.SetGrid((CRavidSheet*)this);

				return (CRavidSheetCellBase*)&cell;
			}

			GRID_ROW* pRow = m_RowData[nRow];
			if(!pRow) return NULL;
			return pRow->GetAt(m_arColOrder[nCol]);
		}

		inline BOOL CRavidSheet::SetCell(int nRow, int nCol, CRavidSheetCellBase* pCell)
		{
			if(GetVirtualMode())
				return FALSE;

			if(nRow < 0 || nRow >= m_nRows || nCol < 0 || nCol >= m_nCols)
				return FALSE;

			GRID_ROW* pRow = m_RowData[nRow];
			if(!pRow) return FALSE;

			pCell->SetCoords(nRow, nCol);
			pRow->SetAt(nCol, pCell);

			return TRUE;
		}
	}
}

