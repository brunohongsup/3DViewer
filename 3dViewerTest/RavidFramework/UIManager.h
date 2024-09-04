#pragma once

#include "RavidObjectEx.h"

#include <vector>
#include <memory>
#include <map>
#include <d2d1.h>
#include <dwrite.h>

class CMultiDocTemplate;
class Ravid::Algorithms::CRavidImage;

// Multi Document Template 생성시 사용하는 전처리기
#define DOC_TEMPLATE(resid,doc,childfrm,view) resid,RUNTIME_CLASS(doc),RUNTIME_CLASS(childfrm),RUNTIME_CLASS(view)

namespace Ravid
{
	namespace Miscellaneous
	{
		class CPerformanceCounter;
	}

	namespace Framework
	{
		class CRavidLogViewer;
		class CRavidFormViewBase;
		class CRavidChildFrameLogViewer;
		class CRavidDialogBase;
		class CRavidInputBoxInfo;
		class CUIConfig;
		class CUIConfigElement;
		class CRavidImageView;
		class CRavidMainFrame;
		class CRavidImageProcessingTool;
		class CRavidChildFrameImageProcessingTool;
		class CRavidDlgModelManager;
		class CRavidDlgNotification;
		class CRavidDlgProgress;
		
		/**
		* 프레임워크 UI를 관리하는 클래스입니다.
		*/
		class AFX_EXT_CLASS CUIManager : public CRavidObjectEx
		{
		private:
			CUIManager();
		public:
			virtual ~CUIManager();

			static bool AddEasyConfigurationView(_In_ CRuntimeClass* pView, _In_opt_ CRuntimeClass* pFrame = nullptr);

			/**
			* 프레임워크에 등록된 UI 구성의 수를 가져옵니다.
			*/
			static size_t GetUIConfigurationCount();

			/**
			* 해당 번호의 UI 구성 정보를 가져옵니다.
			*/
			static CUIConfig* GetUIConfiguration(_In_ int nNumber);

			/**
			* 해당 번호의 UI 구성을 실행합니다.
			*/
			static bool RunUIConfiguration(_In_ long nNumber);

			/**
			* 현재 UI 구성 정보를 가져옵니다.
			*/
			static CUIConfig* GetCurrentUIConfiguration();

			/**
			* 현재 UI 구성 번호를 가져옵니다.
			*/
			static long GetCurrentUIConfigurationNumber();
			
			/**
			* 프레임워크에 등록된 모든 자식창의 리스트를 가져옵니다.
			*/
			static bool GetAllRavidChildFrame(_Out_ std::vector<CRavidChildFrameBase*>* pVctChildFrame);
			
			/**
			* 프레임워크에 등록된 모든 폼뷰의 리스트를 가져옵니다.
			*/
			static bool GetAllRavidFormView(_Out_ std::vector<CRavidFormViewBase*>* pVctFormView);
			
			/**
			* 프레임워크에 등록된 모든 이미지뷰의 리스트를 가져옵니다.
			*/
			static bool GetAllRavidImageView(_Out_ std::vector<CRavidImageView*>* pVctImageView);
			
			/**
			* 프레임워크에 등록된 모든 다이얼로그의 리스트를 가져옵니다.
			*/
			static bool GetAllRavidDialog(_Out_ std::vector<CRavidDialogBase*>* pVctDialog);

			/**
			* 해당 ID의 뷰를 가져옵니다.
			*/
			static CRavidObject* FindView(_In_ long nObjectID = -1);
						
			/**
			* 해당 클래스 & ID로 뷰를 가져옵니다.
			*/
			static CRavidObject* FindView(_In_ CRuntimeClass* pRuntimeClass, _In_ long nObjectID = -1);
						
			/**
			* 해당 클래스 타입 & ID로 뷰를 가져옵니다.
			*/
			static CRavidObject* FindView(_In_ const std::type_info& classType, _In_ long nObjectID = -1);
			
			/**
			* 해당 클래스 이름 & ID로 뷰를 가져옵니다.
			*/
			static CRavidObject* FindView(_In_ CString strClassNameIncludingNameSpace, _In_ long nObjectID = -1);
			
			/**
			* 해당 위치를 캡쳐하여 이미지로 가져옵니다.
			*/
			static bool CaptureScreen(_Out_ Algorithms::CRavidImage& riDst, _In_opt_ CRavidRect<int> rrROI);
			
			/**
			* 해당 위치를 캡쳐하여 이미지로 가져옵니다.
			*/
			static bool CaptureScreen(_Out_ Algorithms::CRavidImage& riDst, _In_opt_ CRavidRect<int>* pRrROI = nullptr);
			
			/**
			* 해당 위치를 캡쳐하여 이미지로 가져옵니다.
			*/
			static bool CaptureScreen(_Out_ Algorithms::CRavidImage* pRiDst, _In_opt_ CRavidRect<int> rrROI);
			
			/**
			* 해당 위치를 캡쳐하여 이미지로 가져옵니다.
			*/
			static bool CaptureScreen(_Out_ Algorithms::CRavidImage* pRiDst, _In_opt_ CRavidRect<int>* pRrROI = nullptr);
			
			/**
			* 메인 프레임워크의 타이틀을 설정합니다.
			*/
			static bool SetRavidTitle(_In_ CString strTitle, _In_opt_ bool bHidePrefix = false);

			/**
			* 메인 프레임워크의 타이틀을 가져옵니다.
			*/
			static CString GetRavidTitle();
			
			/**
			* 프레임워크 전체 색 구성을 설정합니다.
			*/
			static bool SetSkinType(_In_ ERavidSkinType eSkin);
			
			/**
			* 프레임워크 전체 색 구성을 가져옵니다.
			*/
			static ERavidSkinType GetSkinType();
			
			/**
			* 메인 프레임워크의 프레임 색을 설정합니다.
			*/
			static bool SetRavidMainFrameColor(_In_ ERavidFrameColorType eRFCT, _In_ COLORREF clr);
			
			/**
			* 메인 프레임워크의 프레임 색을 가져옵니다.
			*/
			static COLORREF GetRavidMainFrameColor(_In_ ERavidFrameColorType eRFCT);
			
			/**
			* 메인 프레임워크의 캡션 색을 설정합니다.
			*/
			static bool SetRavidMainFrameCaptionColor(_In_ ERavidCaptionColorType eRCCT, _In_ COLORREF clr);
			
			/**
			* 메인 프레임워크의 캡션 색을 가져옵니다.
			*/
			static COLORREF GetRavidMainFrameCaptionColor(_In_ ERavidCaptionColorType eRCCT);
			
			/**
			* 메인 프레임워크의 툴바 색을 설정합니다.
			*/
			static bool SetRavidMainFrameToolBarColor(_In_ ERavidToolBarColorType eRTBCT, _In_ COLORREF clr);
			
			/**
			* 메인 프레임워크의 툴바 색을 가져옵니다.
			*/
			static COLORREF GetRavidMainFrameToolBarColor(_In_ ERavidToolBarColorType eRTBCT);
			
			/**
			* 자식 프레임의 색을 설정합니다.
			*/
			static bool SetRavidChildFrameColor(_In_ ERavidFrameColorType eRFCT, _In_ COLORREF clr);
			
			/**
			* 자식 프레임의 색을 가져옵니다.
			*/
			static COLORREF GetRavidChildFrameColor(_In_ ERavidFrameColorType eRFCT);
			
			/**
			* 자식 프레임의 툴바 색을 설정합니다.
			*/
			static bool SetRavidChildFrameToolBarColor(_In_ ERavidToolBarColorType eRTBCT, _In_ COLORREF clr);
			
			/**
			* 자식 프레임의 툴바 색을 가져옵니다.
			*/
			static COLORREF GetRavidChildFrameToolBarColor(_In_ ERavidToolBarColorType eRTBCT);
			
			/**
			* 폼뷰의 컨트롤 색을 설정합니다.
			*/
			static bool SetRavidFormViewControlColor(_In_ ERavidControlColorType eRCCT, _In_ COLORREF clr);
			
			/**
			* 폼뷰의 컨트롤 색을 가져옵니다.
			*/
			static COLORREF GetRavidFormViewControlColor(_In_ ERavidControlColorType eRCCT);
			
			/**
			* 다이얼로그의 프레임 색을 설정합니다.
			*/
			static bool SetRavidDialogFrameColor(_In_ ERavidFrameColorType eRFCT, _In_ COLORREF clr);
			
			/**
			* 다이얼로그의 프레임 색을 가져옵니다.
			*/
			static COLORREF GetRavidDialogFrameColor(_In_ ERavidFrameColorType eRFCT);
						
			/**
			* 다이얼로그의 캡션 색을 설정합니다.
			*/
			static bool SetRavidDialogCaptionColor(_In_ ERavidCaptionColorType eRCCT, _In_ COLORREF clr);
						
			/**
			* 다이얼로그의 캡션 색을 가져옵니다.
			*/
			static COLORREF GetRavidDialogCaptionColor(_In_ ERavidCaptionColorType eRCCT);
			
			/**
			* 다이얼로그의 컨트롤 색을 설정합니다.
			*/
			static bool SetRavidDialogControlColor(_In_ ERavidControlColorType eRCCT, _In_ COLORREF clr);
			
			/**
			* 다이얼로그의 컨트롤 색을 가져옵니다.
			*/
			static COLORREF GetRavidDialogControlColor(_In_ ERavidControlColorType eRCCT);
			
			/**
			* 프레임워크의 메뉴 색을 설정합니다.
			*/
			static bool SetRavidMenuColor(_In_ ERavidMenuColorType eRMCT, _In_ COLORREF clr);
			
			/**
			* 프레임워크의 메뉴 색을 가져옵니다.
			*/
			static COLORREF GetRavidMenuColor(_In_ ERavidMenuColorType eRMCT);
			
			/**
			* 프레임워크의 시트 색을 설정합니다.
			*/
			static bool SetRavidSheetColor(_In_ ERavidSheetColorType eRMCT, _In_ COLORREF clr);
			
			/**
			* 프레임워크의 시트 색을 가져옵니다.
			*/
			static COLORREF GetRavidSheetColor(_In_ ERavidSheetColorType eRMCT);
			
			/**
			* 로그 뷰어를 On / Off 설정합니다.
			*/
			static bool VisibleLogViewer(_In_ bool bVisible);
			
			/**
			* 로그 뷰어 On / Off 상태를 가져옵니다.
			*/
			static bool IsVisibleLogViewer();
			
			/**
			* 이미지 툴을 On / Off 설정합니다.
			*/
			static bool VisibleImageProcessingTool(_In_ bool bVisible);
			
			/**
			* 이미지 툴 On / Off 상태를 가져옵니다.
			*/
			static bool IsVisibleImageProcessingTool();
			
			/**
			* 스크린 세이버를 On / Off 설정합니다.
			*/
			static bool UseScreenSaver(_In_ bool bUse);
			
			/**
			* 스크린 세이버 On / Off 상태를 가져옵니다.
			*/
			static bool IsUsedScreenSaver();
			
			/**
			* 스크린 세이버 타이틀을 설정합니다.
			*/
			static bool SetScreenSaverTitle(_In_ CString strTitle);
			
			/**
			* 스크린 세이버 타이틀을 가져옵니다.
			*/
			static CString GetScreenSaverTitle();
			
			/**
			* 스크린 세이버 대기 시간을 설정합니다.
			*/
			static bool SetScreenSaverWaitingTime(_In_ DWORD dwTime);
			
			/**
			* 스크린 세이버 대기 시간을 가져옵니다.
			*/
			static DWORD GetScreenSaverWaitingTime();
			
			/**
			* 해당 뷰어의 x 축 해상도를 설정합니다.
			*/
			static bool SetAccuracyX(_In_ long nViewObjectID, _In_ double dblAccuracyX);
			
			/**
			* 해당 뷰어의 y 축 해상도를 설정합니다.
			*/
			static bool SetAccuracyY(_In_ long nViewObjectID, _In_ double dblAccuracyY);
			
			/**
			* 사용자가 입력한 Box를 실행합니다.
			*/
			static bool RunRavidInputBox(_In_ CRavidInputBoxInfo* pRavidInputBoxInfo, _In_opt_ CWnd* pWndParent = nullptr);
			
			/**
			* 경고창을 실행합니다.
			*/
			static bool RunRavidNotification(_In_ CString strMessage, _In_opt_ ERavidNotificationType eNotification = ERavidNotificationType_Information, _In_opt_ ERavidModalType eModalType = ERavidModalType_Modal);
			
			/**
			* 경고창을 종료합니다.
			*/
			static void EndRavidNotification(_In_ CRavidDlgNotification* pNotificationDlg, _In_ ERavidNotificationButtonType  eNotificationButton = ERavidNotificationButtonType_AllOk);
			
			/**
			* 프로세스 진행상태 창을 실행합니다.
			*/
			static bool RunRavidProgressDialog(_In_ CString strMessage, _In_ double dblMaxValue, _In_opt_ ERavidModalType eModalType = ERavidModalType_Modal, _In_opt_ COLORREF clrTextColor = RGB(0, 0, 0), _In_opt_ bool bShowProgressBar = true, _In_opt_ COLORREF clrProgressBarColor = LIME, _In_opt_ CBitmap* pBmpBackground = nullptr, _In_opt_ int nAlphaBlendValue = 127);

			/**
			* 프로세스 진행상태 창을 종료합니다.
			*/
			static void EndRavidProgressDialog();
			
			/**
			* 프로세스 진행상태 창의 메시지를 업데이트합니다.
			*/
			static bool UpdateProgressDialogMessage(_In_ CString strMessage);
			
			/**
			* 프로세스 진행상태 창의 진행 상태를 업데이트합니다.
			*/
			static bool UpdateProgressDialogBar(_In_ double dblState);
			
			/**
			* 프로세스 진행상태 창의 메시지와 진행 상태를 업데이트합니다.
			*/
			static bool UpdateMessageAndBar(_In_ CString strMessage, _In_ double dblState);
			
			/**
			* 자식 창에 ID 출력을 설정합니다.
			*/
			static void ShowChildFrameObjectID(_In_ bool bShow);
			
			/**
			* 자식 창에 ID 출력 상태를 가져옵니다.
			*/
			static bool IsShowChildFrameObjectID();
			
			/**
			* 메인프레임 툴바 출력을 설정합니다.
			*/
			static void ShowMainFrameToolBar(_In_ bool bShow);
			
			/**
			* 메인프레임 툴바 출력 상태를 가져옵니다.
			*/
			static bool IsShowMainFrameToolBar();
			
			/**
			* 자식 창의 위치 및 크기를 변환합니다.
			*/
			static bool RavidChildFrameMoveWindow(_In_ long nObjectID, CRavidRect<int> rrPosition);
			
			/**
			* 자식 창의 위치 및 크기를 변환합니다.
			*/
			static bool RavidChildFrameMoveWindow(_In_ long nObjectID, _In_ long nLeft, _In_ long nTop, _In_ long nRight, _In_ long nBottom);
			
			/**
			* 사용자 이름 출력을 설정합니다.
			*/
			static void ShowUserName(_In_ bool bShow);
			
			/**
			* 사용자 이름 출력 상태를 가져옵니다.
			*/
			static bool IsShowUserName();

			// Only framework use

			static bool SetRavidMainFrameToolBarBottomEdgeDraw(_In_ bool bDraw);
			static bool GetRavidMainFrameToolBarBottomEdgeDraw();

			static bool SetRavidMainFrameToolBarTitleMinimumDisplayWidth(_In_ long nWidth);
			static bool GetRavidMainFrameToolBarTitleMinimumDisplayWidth();

			static bool SetRavidMainFrameToolBarTitleStartOffsetX(_In_ long nOffsetX);
			static long GetRavidMainFrameToolBarTitleStartOffsetX();


			static bool SetRavidChildFrameToolBarBottomEdgeDraw(_In_ bool bDraw);
			static bool GetRavidChildFrameToolBarBottomEdgeDraw();

			static bool SetRavidChildFrameToolBarTitleMinimumDisplayWidth(_In_ long nWidth);
			static bool GetRavidChildFrameToolBarTitleMinimumDisplayWidth();

			static bool SetRavidChildFrameToolBarTitleStartOffsetX(_In_ long nOffsetX);
			static long GetRavidChildFrameToolBarTitleStartOffsetX();

			static CUIConfigElement* GetUIConfigurationElement(_In_ int nID, _In_ CString strName);
			
			/**
			* 로그 뷰어의 포인터를 가져옵니다.
			*/
			static CRavidLogViewer* GetLogViewer();
			
			/**
			* 로그 뷰어의 자식 창의 포인터를 가져옵니다.
			*/
			static CRavidChildFrameLogViewer* GetLogViewerFrame();
			
			/**
			* 이미지 툴 뷰어의 포인터를 가져옵니다.
			*/
			static CRavidImageProcessingTool* GetImageProcessingTool();
			
			/**
			* 이미지 툴 뷰어의 자식 창의 포인터를 가져옵니다.
			*/
			static CRavidChildFrameImageProcessingTool* GetImageProcessingToolFrame();
			
			/**
			* 모델 관리 뷰어의 포인터를 가져옵니다.
			*/
			static CRavidDlgModelManager* GetDlgModelManager();
			
			/**
			* 이미지 툴 초기화 상태를 가져옵니다.
			*/
			static bool IsImageProcessingToolInitilize();
			
			/**
			* Ravid에서 기본적으로 제공하는 RavidMainFrame을 생성합니다.
			*/
			static bool CreateRavidMainFrame(_In_opt_ UINT uiToolBarResourceEnableID = 0, _In_opt_ UINT uiToolBarResourceDisableID = 0);
			
			/**
			* 데이터 베이스에 저장된 UI 구성을 등록합니다.
			*/
			static bool RegisterUIConfiguration();
			
			/**
			* UI 에 필요한 리소스를 초기화합니다.
			*/
			static bool Initialize();
			
			/**
			* UI 에 사용한 리소스를 해제합니다.
			*/
			static bool Terminate();
			
			/**
			* 등록된 자식 프레임의 크기를 엽데이트합니다.
			*/
			static bool UpdateChildFrameSize();
			
			/**
			* 해당 프레임의 분해능을 설정합니다.
			*/
			static bool SetImageViewExtraData(_In_ long nViewObjectID, _In_ ERavidImageViewExtraDataType eType, _In_ CString strData);
			
			/**
			* 해당 프레임의 분해능을 가져옵니다.
			*/
			static bool GetImageViewExtraData(_In_ long nViewObjectID, _In_ ERavidImageViewExtraDataType eType, _Out_ CString& strData);
			
			/**
			* 메인 프레임의 크기를 가져옵니다.
			*/
			static CRavidRect<int> GetMainFrameClientRect();
			
			/**
			* 메인 프레임의 툴바를 업데이트합니다.
			*/
			static bool UpdateRavidMainFrameToolbar();
			
			/**
			* RAVID의 UI 리소스를 생성합니다.
			*/
			static void BeginRavidUIResource();
			
			/**
			* RAVID의 UI 리소스를 반납합니다.
			*/
			static void EndRavidUIResource(_In_opt_ bool bSynchronization = true);

			static HMODULE GetRavidFrameworkResource();
			static HMODULE GetBackupRavidFrameworkResource();

			static void SetResourceHandle(_In_ HMODULE hResource);

			static void CloseAllMDIClient();

			static bool RegisterMFCToolBarImages(_In_ HINSTANCE hResourceHandle, _In_ UINT uiID);
			static CMFCToolBarImages* GetRegisterMFCToolBarImages(_In_ HINSTANCE hResourceHandle, _In_ UINT uiID);

			static bool SetLastFocusChildFrame(_In_ CRavidChildFrameBase* pChildFrame);
			static CRavidChildFrameBase* GetLastFocusChildFrame();

			static ID2D1Factory* GetD2DFactory();
			static IDWriteFactory* GetDWriteFactory();

			static ID2D1StrokeStyle* GetD2DStrokeStyle(_In_ ERavidImageViewPenStyle ePenStyle);

			static bool SetImageViewDrawingElapsedTime(_In_ CRavidImageView* pImageView);
			static double GetImageViewDrawingIntervalTime();

			static bool AddNotificationDlg(CRavidDlgNotification* pDlg);

			static bool InternalClearNotifications();
			
			/**
			* UI 를 화면 크기에 적응적 변화를 하도록 설정합니다.
			*/
			static bool SetRelativeLayout(_In_ bool bEnable = true);
						
			/**
			* UI 를 화면 크기에 적응적 변화를 하는 설정을 가져옵니다.
			*/
			static bool GetRelativeLayout();

			static bool SetImageViewEnableScrollBar(_In_ long nViewObjectID, _In_ CString strData);
			static bool GetImageViewEnableScrollBar(_In_ long nViewObjectID);


			static bool ConfigurationSave();
			static bool ConfigurationSaveAs(_In_ CString strTitle);
			static ERavidConfigurationTitle ConfigurationRename(_In_ CString strTitle);
			static bool ConfigurationDelete(_In_ int nIndex);

			static bool HideConfiguration();
			static bool MakeUIConfig(_In_ CString strTitle, _In_ std::vector<SConfigurationElementInfo> vctCEI, _Out_ CUIConfig& uc);

			static void SetMainFrameBackgroundBitmap(_In_ CBitmap* pBmp);
			static CBitmap* GetMainFrameBackgroundBitmap();
			
			/**
			* 메인 프레임 클래스를 가져옵니다.
			*/
			static CRavidMainFrame* GetMainFrame();

			static void EnableShowMainFrameTitleBar(bool bEnable);
			static bool IsEnabledShowMainFrameTitleBar();

			static void EnableShowMainFrameTitleBarButton(ERavidCaptionButtonKind eKind, bool bEnable);
			static bool IsEnabledShowMainFrameTitleBarButton(ERavidCaptionButtonKind eKind);

			static void EnableMoveWindowWithMainFrameTitleBar(bool bEnable);
			static bool IsEnabledMoveWindowWithMainFrameTitleBar();

		private:
			static bool FindChildWindow(_Inout_ SWindowElement& sWE);

			static bool AddUIConfiguration(_In_ CUIConfig& ravidUIConfig);
			static bool DeleteUIConfiguration(_In_ CUIConfig* pUIConfig);

			static bool ClearUIConfiguration();

			static CMultiDocTemplate* AddMultiDocTemplate(_In_ UINT nResourceID, _In_ CRuntimeClass* pDocument, _In_ CRuntimeClass* pChildFrame, _In_ CRuntimeClass* pView, _In_opt_ bool bDLLResource = false);

			static CUIManager* GetInstance();

			static CRuntimeClass* DecideChildFrame(CRuntimeClass* pView);

			static bool FindDialog(_In_ SWindowElement& sW, _Out_ std::vector<CRavidDialogBase*>* pVctDialog);

			static std::vector<SEasyConfigurationView>* GetEasyConfigurationView();

			static bool IsAvailableRegisterViewInfos();

			static bool CreateRegisterViewInfosTable(_In_opt_ bool bSave = true);

			static bool DropRegisterViewInfosTable(_In_opt_ bool bSave = true);

			static bool GetRegisterViewInfos(_Out_ std::vector<SRegisterViewInfo>& vctInfos);
			static bool GetRegisterViewInfos(_Out_ std::vector<SRegisterViewInfo>* pVctInfos);

			static bool GetConfigurationElementInfo(_Out_ std::vector<std::pair<CString, std::vector<SConfigurationElementInfo> > >& vctInfos);
			static bool GetConfigurationElementInfo(_Out_ std::vector<std::pair<CString, std::vector<SConfigurationElementInfo> > >* pVctInfos);

			static bool GetConfigurationElementInfoPx(_Out_ std::vector<std::pair<CString, std::vector<SConfigurationElementInfo> > >& vctInfos);
			static bool GetConfigurationElementInfoPx(_Out_ std::vector<std::pair<CString, std::vector<SConfigurationElementInfo> > >* pVctInfos);

			static bool UpdateRegisterViewInfosTable(_In_ std::vector<SRegisterViewInfo>& vctInfos);
			static bool UpdateRegisterViewInfosTable(_In_ std::vector<SRegisterViewInfo>* pVctInfos);

			static bool UpdateConfigurationElementInfo(_In_ std::vector<std::pair<CString, std::vector<SConfigurationElementInfo> > >& vctInfos);
			static bool UpdateConfigurationElementInfo(_In_ std::vector<std::pair<CString, std::vector<SConfigurationElementInfo> > >* pVctInfos);

			static bool UpdateConfigurationElementInfoPx(_In_ std::vector<std::pair<CString, std::vector<SConfigurationElementInfo> > >& vctInfos);
			static bool UpdateConfigurationElementInfoPx(_In_ std::vector<std::pair<CString, std::vector<SConfigurationElementInfo> > >* pVctInfos);

			static bool UpdateRegisterViewInspectionMenuTable(_In_ int nSequence, _In_ ERegisterViewType eType, _In_ std::vector<CString>& vctMenu);
			static bool UpdateRegisterViewTeachingMenuTable(_In_ int nSequence, _In_ ERegisterViewType eType, _In_ std::vector<SRegisterTeachingMenuInfo>& vctMenu);

			static bool GetRegisterViewInspectionMenu(_In_ int nSequence, _Out_ std::vector<CString>& vctMenu);
			static bool GetRegisterViewTeachingMenu(_In_ int nSequence, _Out_ std::vector<SRegisterTeachingMenuInfo>& vctMenu);

			static ERegisterViewType GetViewType(_In_ CRavidObject* pObject);
			static ERegisterViewType GetViewType(_In_ CString strClass);

			static bool SetConfigurationData(_In_ EUIManagerConfigurationDataType eType, _In_ CString strData, _In_opt_ CString strSubTitle = _T(""));
			static bool GetConfigurationData(_In_ EUIManagerConfigurationDataType eType, _Out_ CString& strData, _In_opt_ CString strSubTitle = _T(""));

			static bool GetRelativeLayoutTable();

			static bool UpdateSkinType();

			friend class CAuthorityManager;
			friend class CRavidDlgViewConfigurationTool;
			friend class CRavidImageProcessingTool;
			friend class CRavidDlgFrameworkConfigurationTool;
			friend class CRavidDlgConfigurationNew;

		protected:
			virtual ptrdiff_t OnMessage(Ravid::Framework::CMessageBase* pMessage);

		private:
			static bool InitViewConfiguration();
			static bool GetViewConfiguration(_Out_ std::vector<SViewConfiguration>& vctInfos);
			static bool GetViewConfiguration(_Out_ std::vector<SViewConfiguration>* pVctInfos);
			static bool InsertViewConfiguration(_In_ std::vector<SViewConfiguration>& vctInfos);
			static bool InsertViewConfiguration(_In_ std::vector<SViewConfiguration>* pVctInfos);
			static bool DeleteViewConfiguration(_In_ int nSequence);

			static bool UpdateConfiguration(_In_ int nSequence, _In_ std::vector<SConfigurationElementInfo> vctCEI);
			static bool AllUpdateViewConfiguration(_In_ std::vector<SViewConfiguration>& vctInfos);
			static bool AllUpdateViewConfiguration(_In_ std::vector<SViewConfiguration>* pVctInfos);


		protected:
			ERavidSkinType m_eSkinType = ERavidSkinType_Bright;
			CRavidMainFrame* m_pMainWnd = nullptr;
			HMODULE m_hResource = 0;

			CString m_strRavidTitle = _T("Ravid");

			std::vector<CMultiDocTemplate*>* m_pVctTemplate = nullptr;

			HINSTANCE m_hInstBackup = 0;

			std::map<CStringA, CMultiDocTemplate*> m_mapDocTemplates;

			CUIConfig* m_pCurrentUIConfig = nullptr;
			std::vector<CUIConfig*> m_vctUIConfigs;

			COLORREF m_clrRavidMainFrame[ERavidFrameColorType_Count] = { 0, };
			COLORREF m_clrRavidMainFrameCaption[ERavidCaptionColorType_Count] = { 0, };
			COLORREF m_clrRavidMainFrameToolBar[ERavidToolBarColorType_Count] = { 0, };

			COLORREF m_clrRavidChildFrame[ERavidFrameColorType_Count] = { 0, };
			COLORREF m_clrRavidChildFrameToolBar[ERavidToolBarColorType_Count] = { 0, };

			COLORREF m_clrRavidFormViewControl[ERavidControlColorType_Count] = { 0, };

			COLORREF m_clrRavidDialogFrame[ERavidFrameColorType_Count] = { 0, };
			COLORREF m_clrRavidDialogCaption[ERavidCaptionColorType_Count] = { 0, };
			COLORREF m_clrRavidDialogControl[ERavidControlColorType_Count] = { 0, };

			COLORREF m_clrRavidMenu[ERavidMenuColorType_Count] = { 0, };

			COLORREF m_clrRavidSheet[ERavidSheetColorType_Count] = { 0, };

			bool m_bIsRavidMainFrameToolBarBottomEdge = false;
			bool m_bIsRavidChildFrameToolBarBottomEdge = false;

			long m_nRavidMainFrameToolBarTitleMinimumDisplayWidth = 0;
			long m_nRavidChildFrameToolBarTitleMinimumDisplayWidth = 0;

			long m_nRavidMainFrameToolBarTitleStartOffsetX = 0;
			long m_nRavidChildFrameToolBarTitleStartOffsetX = 0;

			std::vector<SEasyConfigurationView> m_vctEasyConfigurationView;

			std::vector<SRegisterMFCToolBarImagesInfo> m_vctRegisterMFCToolBarImages;

			static LPCTSTR m_lpszRavidImageViewExtraDataType[ERavidImageViewExtraDataType_Count];

			CRavidChildFrameBase* m_pLastFocuseChildFrame = nullptr;

			bool m_bIsShowChildFrameObjectID = true;

			bool m_bIsShowMainFrameToolBar = true;

			bool m_bIsShowUserName = true;

			ID2D1Factory* m_pD2DFactory = nullptr;
			IDWriteFactory* m_pDWriteFactory = nullptr;

			ID2D1StrokeStyle* m_pArrD2DStrokeStyle[ERavidImageViewPenStyle_Count] = { nullptr, };

			static LPCTSTR m_lpszConfigurationDataType[EUIManagerConfigurationDataType_Count];

			std::map<CRavidImageView*, Miscellaneous::CPerformanceCounter*> m_mapImageViewDrawingElapsedTime;

			CCriticalSection m_csNotification;
			std::vector<CRavidDlgNotification*> m_vctModelessNotificationDlgs;
			std::vector<CRavidDlgNotification*> m_vctModalNotificationDlgs;

			CRavidDlgProgress* m_pProgressDlg = nullptr;

			bool m_bRelativeLayout = true;

			CRavidChildFrameBase* m_pRCFB;

			CBitmap m_bmpMainFrameBackground;
		};
	}
}
