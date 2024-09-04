#pragma once

#include "RavidObjectEx.h"

#include <memory>

namespace Ravid
{
	namespace Device
	{
		class CDeviceBase;
	}

	namespace Framework
	{
		class CRavidLogViewer;
		class CPacket;
		class CTeachingObject;
		class CEventHandlerBase;
		class CRavidImageProcessingTool;
		class CRavidImageView;
		class CModelInfo;
		class CViewObjectElement;
		
		/**
		* 프레임워크 이벤트 관리 클래스입니다.
		*/
		class AFX_EXT_CLASS CEventHandlerManager : public CRavidObjectEx
		{
		private:
			CEventHandlerManager();

		public:
			virtual ~CEventHandlerManager();
			
			/**
			* 이벤트를 주고 받을 핸들러를 추가합니다.
			*/
			static bool AddEventHandler(_In_ CEventHandlerBase* pEventHandler, _In_opt_ bool bAutoDestroy = true);
			
			/**
			* 등록된 핸들러를 모두 해제합니다.
			*/
			static void ClearEventHandler();
			
			/**
			* 해당 인덱스의 핸들러를 가져옵니다.
			*/
			static CEventHandlerBase* GetEventHandler(_In_ long nIndex);
			
			/**
			* 등록된 핸들러의 수를 가져옵니다.
			*/
			static long GetEventHanderCount();
			
			/**
			* 등록된 핸들러에 OnInspect 이벤트를 발생합니다.
			*/
			static void BroadcastOnInspect(_In_ Algorithms::CRavidImage* pImgInfo, _In_ int nInspectionType, _In_opt_ CRavidImageView* pView = nullptr, _In_opt_ int nChannel = -1);
			
			/**
			* 등록된 핸들러에 OnLearn 이벤트를 발생합니다.
			*/
			static void BroadcastOnLearn(_In_ Algorithms::CRavidImage* pImgInfo, _In_ CTeachingObject* pTeachingObject, _In_opt_ CRavidImageView* pView = nullptr, _In_opt_ int nChannel = -1);
			
			/**
			* 등록된 핸들러에 OnDeleteElement 이벤트를 발생합니다.
			*/
			static void BroadcastOnDeleteElement(_In_ Algorithms::CRavidImage* pImgInfo, _In_ CTeachingObject* pTeachingObject, _In_ CViewObjectElement* pObjectElement, _In_opt_ CRavidImageView* pView = nullptr, _In_opt_ int nChannel = -1);
			
			/**
			* 등록된 핸들러에 OnAcquisition 이벤트를 발생합니다.
			*/
			static void BroadcastOnAcquisition(_In_ Algorithms::CRavidImage* pImgInfo, _In_ int nChannel, _In_opt_ CRavidImageView* pView = nullptr, Device::CDeviceBase* pDevice = nullptr);
			
			/**
			* 등록된 핸들러에 OnReceivingPacket 이벤트를 발생합니다.
			*/
			static void BroadcastOnReceivingPacket(_In_ CPacket* pPacket, _In_ Device::CDeviceBase* pDevice);
			
			/**
			* 등록된 핸들러에 OnLogMessage 이벤트를 발생합니다.
			*/
			static void BroadcastOnLogMessage(_In_ SRavidLogData* pLogData);
			
			/**
			* 등록된 핸들러에 OnSequenceStart 이벤트를 발생합니다.
			*/
			static void BroadcastOnSequenceStart();
			
			/**
			* 등록된 핸들러에 OnSequencePause 이벤트를 발생합니다.
			*/
			static void BroadcastOnSequencePause();
			
			/**
			* 등록된 핸들러에 OnSequenceStop 이벤트를 발생합니다.
			*/
			static void BroadcastOnSequenceStop();
			
			/**
			* 등록된 핸들러에 OnOpenModel 이벤트를 발생합니다.
			*/
			static void BroadcastOnOpenModel(_In_ CModelInfo* pModelInfo);
			
			/**
			* 등록된 핸들러에 OnCloseModel 이벤트를 발생합니다.
			*/
			static void BroadcastOnCloseModel(_In_ CModelInfo* pModelInfo);
			
			/**
			* 등록된 핸들러에 OnNewModel 이벤트를 발생합니다.
			*/
			static void BroadcastOnNewModel(_In_ CModelInfo* pModelInfo);
			
			/**
			* 등록된 핸들러에 OnCopyModel 이벤트를 발생합니다.
			*/
			static void BroadcastOnCopyModel(_In_ CModelInfo* pModelInfo);
			
			/**
			* 등록된 핸들러에 OnDeleteModel 이벤트를 발생합니다.
			*/
			static void BroadcastOnDeleteModel(_In_ int nModelNumber);
			
			/**
			* 등록된 핸들러에 OnLogIn 이벤트를 발생합니다.
			*/
			static void BroadcastOnLogIn(_In_ CString strUserName, _In_ EUserType eChangedUserType);
			
			/**
			* 등록된 핸들러에 OnLogOut 이벤트를 발생합니다.
			*/
			static void BroadcastOnLogOut();
			
			/**
			* 등록된 핸들러에 OnModelParameterSaveChanged 이벤트를 발생합니다.
			*/
			static void BroadcastOnModelParameterSaveChanged();
			
			/**
			* 등록된 핸들러에 OnModelTableSaveChanged 이벤트를 발생합니다.
			*/
			static void BroadcastOnModelTableSaveChanged(_In_ ERavidTableEventModelManager eEvent, _In_ CModelInfo* pModelInfo, _In_ CString strPreValue);
			
			/**
			* 등록된 핸들러에 OnStartImageProcessingTool 이벤트를 발생합니다.
			*/
			static void BroadcastOnStartImageProcessingTool();
			
			/**
			* 등록된 핸들러에 OnEndImageProcessingTool 이벤트를 발생합니다.
			*/
			static void BroadcastOnEndImageProcessingTool();
			
			/**
			* 등록된 핸들러에 OnDeviceInitialized 이벤트를 발생합니다.
			*/
			static void BroadcastOnDeviceInitialized(_In_ Device::CDeviceBase* pDevice);
			
			/**
			* 등록된 핸들러에 OnDeviceTerminated 이벤트를 발생합니다.
			*/
			static void BroadcastOnDeviceTerminated(_In_ Device::CDeviceBase* pDevice); 
			
			/**
			* 등록된 핸들러에 OnClose 이벤트를 발생합니다.
			*/
			static UINT BroadcastOnClose();
			
			/**
			* 등록된 핸들러에 OnUserToolbarEventRavidMainFrame 이벤트를 발생합니다.
			*/
			static void BroadcastOnUserToolbarEventRavidMainFrame(_In_ UINT ulResourceID);
			
			/**
			* 등록된 핸들러에 OnToolbarEventRavidMainFrame 이벤트를 발생합니다.
			*/
			static void BroadcastOnToolbarEventRavidMainFrame(_In_ ERavidToolBarEventMainFrame eEvent);
			
			/**
			* 등록된 핸들러에 OnToolbarEventRavidChildFrameBase 이벤트를 발생합니다.
			*/
			static void BroadcastOnToolbarEventRavidChildFrameBase(_In_ CView* pView, _In_ ERavidToolBarEventChildFrameBase eEvent);
			
			/**
			* 등록된 핸들러에 OnToolbarEventRavidImageView 이벤트를 발생합니다.
			*/
			static void BroadcastOnToolbarEventRavidImageView(_In_ CRavidImageView* pView, _In_ ERavidToolBarEventImageView eEvent);
			
			/**
			* 등록된 핸들러에 OnToolbarEventRavidLogViewer 이벤트를 발생합니다.
			*/
			static void BroadcastOnToolbarEventRavidLogViewer(_In_ CRavidLogViewer* pView, _In_ ERavidToolBarEventLogViewer eEvent);
			
			/**
			* 등록된 핸들러에 OnToolbarEventRavidImageProcessingTool 이벤트를 발생합니다.
			*/
			static void BroadcastOnToolbarEventRavidImageProcessingTool(_In_ CRavidImageProcessingTool* pView, _In_ ERavidToolBarImageProcessingTool eEvent);
			
			/**
			* 등록된 핸들러에 RunUIConfiguration 이벤트를 발생합니다.
			*/
			static void BroadcastRunUIConfiguration(_In_ long nNumber);
			
			/**
			* 등록된 핸들러에 OnTerminate 이벤트를 발생합니다.
			*/
			static void BroadcastOnTerminate();
			
			/**
			* 등록된 핸들러에 OnCloseModelManager 이벤트를 발생합니다.
			*/
			static void BroadcastOnCloseModelManager();
			
			/**
			* 등록된 핸들러에 OnCloseDeviceManager 이벤트를 발생합니다.
			*/
			static void BroadcastOnCloseDeviceManager();
			
			/**
			* 등록된 핸들러에 OnCloseLogIn 이벤트를 발생합니다.
			*/
			static void BroadcastOnCloseLogIn();
			
			/**
			* 등록된 핸들러에 OnCloseAuthorityManager 이벤트를 발생합니다.
			*/
			static void BroadcastOnCloseAuthorityManager();
			
			/**
			* 등록된 핸들러에 OnCloseScreenSaverSetting 이벤트를 발생합니다.
			*/
			static void BroadcastOnCloseScreenSaverSetting();
			
			/**
			* 등록된 핸들러에 OnToolbarEventRavidImageViewTeaching 이벤트를 발생합니다.
			*/
			static void BroadcastOnToolbarEventRavidImageViewTeaching(_In_ CRavidImageView* pView, _In_ int nSelectIdx);

		private:
			static bool AddFrameworkEventHandler(_In_ CEventHandlerBase* pEventHandler, _In_opt_ bool bAutoDestroy = true);
			static void ClearFrameworkEventHandler();
			static CEventHandlerManager* GetInstance();
			static CEventHandlerBase* GetFrameworkEventHandler(_In_ long nIndex);
			static long GetFrameworkEventHanderCount();

			friend class CUIManager;

		protected:
			typedef struct sEventHandlerData
			{
				CEventHandlerBase* pEventHandler;
				bool bAutoDestroy;
			}
			SEventHandlerData;


			std::vector<SEventHandlerData> m_vctEventHandlers;
			std::vector<SEventHandlerData> m_vctFrameworkEventHandlers;

		};
	}
}

