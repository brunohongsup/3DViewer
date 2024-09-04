#pragma once

#include "RavidObjectEx.h"

#include <memory>

namespace Ravid
{
	namespace Algorithms
	{
		class CRavidImage;
	}

	namespace Device
	{
		class CDeviceBase;
	}

	namespace Framework
	{
		class CRavidLogViewer;
		class CPacket;
		class CTeachingObject;
		class CRavidImageProcessingTool;
		class CRavidImageView;
		class CModelInfo;
		class CViewObjectElement;

		class AFX_EXT_CLASS CEventHandlerBase : public CRavidObjectEx
		{
			RavidUseDynamicCreation();
			RavidPreventCopySelf(CEventHandlerBase);

		public:

			enum EEventHandlerCloseMethod
			{
				EEventHandlerCloseMethod_Default = 0x00,
				EEventHandlerCloseMethod_Exit = 0x01,
				EEventHandlerCloseMethod_Ask = 0x02,
				EEventHandlerCloseMethod_DoNotExit = 0x04,
			};


			CEventHandlerBase();
			virtual ~CEventHandlerBase();

			void SetInstance(_In_ void* pInstance);
			void* GetInstance();
			
			virtual void OnInspect(_In_ Ravid::Algorithms::CRavidImage* pImgInfo, _In_ int nInspectionType, _In_opt_ Ravid::Framework::CRavidImageView* pView = nullptr, _In_opt_ int nChannel = -1) { }
			virtual void OnTeach(_In_ Ravid::Algorithms::CRavidImage* pImgInfo, _In_ Ravid::Framework::CTeachingObject* pTeachingObject, _In_opt_ Ravid::Framework::CRavidImageView* pView = nullptr, _In_opt_ int nChannel = -1) { }
			virtual void OnDeleteElement(_In_ Ravid::Algorithms::CRavidImage* pImgInfo, _In_ Ravid::Framework::CTeachingObject* pTeachingObject, _In_ CViewObjectElement* pObjectElement, _In_opt_ Ravid::Framework::CRavidImageView* pView = nullptr, _In_opt_ int nChannel = -1) { }
			virtual void OnAcquisition(_In_ Ravid::Algorithms::CRavidImage* pImgInfo, _In_ int nChannel, _In_opt_ Ravid::Framework::CRavidImageView* pView = nullptr, _In_opt_ Ravid::Device::CDeviceBase* pDevice = nullptr) { }
			virtual void OnReceivingPacket(_In_ Ravid::Framework::CPacket* pPacket, _In_ Ravid::Device::CDeviceBase* pDevice) { }
			virtual void OnLogMessage(_In_ Ravid::Framework::SRavidLogData* pLogData) { }

			virtual void OnSequenceStart() { }
			virtual void OnSequencePause() { }
			virtual void OnSequenceStop() { }

			virtual void OnOpenModel(_In_ Ravid::Framework::CModelInfo* pModelInfo) { }
			virtual void OnCloseModel(_In_ Ravid::Framework::CModelInfo* pModelInfo) { }
			virtual void OnNewModel(_In_ Ravid::Framework::CModelInfo* pModelInfo) { }
			virtual void OnCopyModel(_In_ Ravid::Framework::CModelInfo* pModelInfo) { }
			virtual void OnDeleteModel(_In_ int nModelNumber) { }

			virtual void OnLogIn(_In_ CString strUserName, _In_ Ravid::Framework::EUserType eChangedUserType) { }
			virtual void OnLogOut() { }

			virtual void OnModelParameterSaveChanged() { }
			virtual void OnModelTableSaveChanged(_In_ ERavidTableEventModelManager eEvent, _In_ CModelInfo* pModelInfo, _In_ CString strPreValue) { }

			virtual void OnDeviceInitialized(_In_ Ravid::Device::CDeviceBase* pDevice) { }
			virtual void OnDeviceTerminated(_In_ Ravid::Device::CDeviceBase* pDevice) { }

			virtual void OnStartImageProcessingTool() { }
			virtual void OnEndImageProcessingTool() { }

			virtual EEventHandlerCloseMethod OnClose();

			virtual void OnUserToolbarEventRavidMainFrame(_In_ UINT ulResourceID) { }
			virtual void OnToolbarEventRavidMainFrame(_In_ Ravid::Framework::ERavidToolBarEventMainFrame eEvent) { }
			virtual void OnToolbarEventRavidChildFrameBase(_In_ CView* pView, _In_ Ravid::Framework::ERavidToolBarEventChildFrameBase eEvent) { }
			virtual void OnToolbarEventRavidImageView(_In_ Ravid::Framework::CRavidImageView* pView, _In_ Ravid::Framework::ERavidToolBarEventImageView eEvent) { }
			virtual void OnToolbarEventRavidLogViewer(_In_ Ravid::Framework::CRavidLogViewer* pView, _In_ Ravid::Framework::ERavidToolBarEventLogViewer eEvent) { }
			virtual void OnToolbarEventImageProcessingTool(_In_ Ravid::Framework::CRavidImageProcessingTool* pView, _In_ Ravid::Framework::ERavidToolBarImageProcessingTool eEvent) { }

			virtual void OnRunUIConfiguration(_In_ long nNumber) { }

			virtual void OnTerminate() { }

			virtual void OnCloseModelManager() { }
			virtual void OnCloseDeviceManager() { }
			virtual void OnCloseLogIn() { }
			virtual void OnCloseAuthorityManager() { }
			virtual void OnCloseScreenSaverSetting() { }

			virtual void OnToolbarEventRavidImageViewTeaching(_In_ Ravid::Framework::CRavidImageView* pView, _In_ int nSelectIdx) { }

		protected:
			void* m_pInstance = nullptr;
		};
	}
}