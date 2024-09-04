#pragma once

#include "DeviceCamera.h"

namespace GenApi_3_2
{
	class CNodeMapRef;
}

namespace Ravid
{
	namespace Device
	{
		class AFX_EXT_CLASS CDeviceGenICam : public CDeviceCamera
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceGenICam);

			DECLARE_DYNAMIC(CDeviceGenICam)

		public:
			CDeviceGenICam();
			virtual ~CDeviceGenICam();

			virtual EDeviceInitializeResult Initialize() override;
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

			virtual EDeviceGrabResult Grab() override;
			virtual EDeviceLiveResult Live() override;
			virtual EDeviceStopResult Stop() override;

			virtual EDeviceTriggerResult Trigger() override;

		public:
			DECLARE_MESSAGE_MAP()

		public:
			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;

			EDeviceGenICamGetFunction GetInitializeMode(_Out_ EDeviceGenICamInitMode *pParam);
			EDeviceGenICamSetFunction SetInitializeMode(_In_ EDeviceGenICamInitMode eParam);

			virtual EDeviceGenICamGetFunction GetCtiFilePath(_Out_ CString* pParam);
			virtual EDeviceGenICamSetFunction SetCtiFilePath(_In_ CString strParam);

			virtual EDeviceGenICamGetFunction GetGainAuto(_Out_ EDeviceGenICamGainAuto* pParam);
			virtual EDeviceGenICamSetFunction SetGainAuto(_In_ EDeviceGenICamGainAuto eParam);

			virtual EDeviceGenICamGetFunction GetGainRaw(_Out_ int* pParam);
			virtual EDeviceGenICamSetFunction SetGainRaw(_In_ int nParam);

			virtual EDeviceGenICamGetFunction GetGainSelector(_Out_ EDeviceGenICamGainSelector* pParam);
			virtual EDeviceGenICamSetFunction SetGainSelector(_In_ EDeviceGenICamGainSelector eParam);

			virtual EDeviceGenICamGetFunction GetBlackLevelRaw(_Out_ int* pParam);
			virtual EDeviceGenICamSetFunction SetBlackLevelRaw(_In_ int nParam);

			virtual EDeviceGenICamGetFunction GetBlackLevelSelector(_Out_ EDeviceGenICamBlackLevelSelector* pParam);
			virtual EDeviceGenICamSetFunction SetBlackLevelSelector(_In_ EDeviceGenICamBlackLevelSelector eParam);

			virtual EDeviceGenICamGetFunction GetBalanceWhiteAuto(_Out_ EDeviceGenICamBalanceWhiteAuto* pParam);
			virtual EDeviceGenICamSetFunction SetBalanceWhiteAuto(_In_ EDeviceGenICamBalanceWhiteAuto eParam);

			virtual EDeviceGenICamGetFunction GetBalanceRatioSelector(_Out_ EDeviceGenICamBalanceRatioSelector* pParam);
			virtual EDeviceGenICamSetFunction SetBalanceRatioSelector(_In_ EDeviceGenICamBalanceRatioSelector eParam);

			virtual EDeviceGenICamGetFunction GetBalanceRatioAbs(_Out_ float* pParam);
			virtual EDeviceGenICamSetFunction SetBalanceRatioAbs(_In_ float fParam);

			virtual EDeviceGenICamGetFunction GetBalanceRatioRaw(_Out_ int* pParam);
			virtual EDeviceGenICamSetFunction SetBalanceRatioRaw(_In_ int nParam);

			virtual EDeviceGenICamGetFunction GetGammaEnable(_Out_ bool* pParam);
			virtual EDeviceGenICamSetFunction SetGammaEnable(_In_ bool bParam);

			virtual EDeviceGenICamGetFunction GetGamma(_Out_ float* pParam);
			virtual EDeviceGenICamSetFunction SetGamma(_In_ float fParam);

			virtual EDeviceGenICamGetFunction GetDigitalShift(_Out_ int* pParam);
			virtual EDeviceGenICamSetFunction SetDigitalShift(_In_ int nParam);

			virtual EDeviceGenICamGetFunction GetPixelFormat(_Out_ EDeviceGenICamPixelFormat* pParam);
			virtual EDeviceGenICamSetFunction SetPixelFormat(_In_ EDeviceGenICamPixelFormat eParam);

			virtual EDeviceGenICamGetFunction GetReverseX(_Out_ bool* pParam);
			virtual EDeviceGenICamSetFunction SetReverseX(_In_ bool bParam);

			virtual EDeviceGenICamGetFunction GetReverseY(_Out_ bool* pParam);
			virtual EDeviceGenICamSetFunction SetReverseY(_In_ bool bParam);

			virtual EDeviceGenICamGetFunction GetSensorWidth(_Out_ int* pParam);
			virtual EDeviceGenICamSetFunction SetSensorWidth(_In_ int nParam);

			virtual EDeviceGenICamGetFunction GetWidth(_Out_ int* pParam);
			virtual EDeviceGenICamSetFunction SetWidth(_In_ int nParam);

			virtual EDeviceGenICamGetFunction GetHeight(_Out_ int* pParam);
			virtual EDeviceGenICamSetFunction SetHeight(_In_ int nParam);

			virtual EDeviceGenICamGetFunction GetOffsetX(_Out_ int* pParam);
			virtual EDeviceGenICamSetFunction SetOffsetX(_In_ int nParam);

			virtual EDeviceGenICamGetFunction GetOffsetY(_Out_ int* pParam);
			virtual EDeviceGenICamSetFunction SetOffsetY(_In_ int nParam);

			virtual EDeviceGenICamGetFunction GetBinningVertical(_Out_ int* pParam);
			virtual EDeviceGenICamSetFunction SetBinningVertical(_In_ int nParam);

			virtual EDeviceGenICamGetFunction GetBinningHorizontal(_Out_ int* pParam);
			virtual EDeviceGenICamSetFunction SetBinningHorizontal(_In_ int nParam);

			virtual EDeviceGenICamGetFunction GetSensorHeight(_Out_ int* pParam);
			virtual EDeviceGenICamSetFunction SetSensorHeight(_In_ int nParam);

			virtual EDeviceGenICamGetFunction GetLegacyBinningVertical(_Out_ EDeviceGenICamLegacyBinningVertical* pParam);
			virtual EDeviceGenICamSetFunction SetLegacyBinningVertical(_In_ EDeviceGenICamLegacyBinningVertical eParam);

			virtual EDeviceGenICamGetFunction GetEnableBurstAcquisition(_Out_ bool* pParam);
			virtual EDeviceGenICamSetFunction SetEnableBurstAcquisition(_In_ bool bParam);

			virtual EDeviceGenICamGetFunction GetAcquisitionMode(_Out_ EDeviceGenICamAcquisitionMode* pParam);
			virtual EDeviceGenICamSetFunction SetAcquisitionMode(_In_ EDeviceGenICamAcquisitionMode eParam);

			virtual EDeviceGenICamGetFunction GetTriggerSelector(_Out_ EDeviceGenICamTriggerSelector* pParam);
			virtual EDeviceGenICamSetFunction SetTriggerSelector(_In_ EDeviceGenICamTriggerSelector eParam);

			virtual EDeviceGenICamGetFunction GetTriggerMode(_Out_ EDeviceGenICamTriggerMode* pParam);
			virtual EDeviceGenICamSetFunction SetTriggerMode(_In_ EDeviceGenICamTriggerMode eParam);

			virtual EDeviceGenICamGetFunction GetTriggerSource(_Out_ EDeviceGenICamTriggerSource* pParam);
			virtual EDeviceGenICamSetFunction SetTriggerSource(_In_ EDeviceGenICamTriggerSource eParam);

			virtual EDeviceGenICamGetFunction GetTriggerActivation(_Out_ EDeviceGenICamTriggerActivation* pParam);
			virtual EDeviceGenICamSetFunction SetTriggerActivation(_In_ EDeviceGenICamTriggerActivation eParam);

			virtual EDeviceGenICamGetFunction GetTriggerDelayAbs(_Out_ float* pParam);
			virtual EDeviceGenICamSetFunction SetTriggerDelayAbs(_In_ float fParam);

			virtual EDeviceGenICamGetFunction GetExposureAuto(_Out_ EDeviceGenICamExposureAuto* pParam);
			virtual EDeviceGenICamSetFunction SetExposureAuto(_In_ EDeviceGenICamExposureAuto eParam);

			virtual EDeviceGenICamGetFunction GetExposureMode(_Out_ EDeviceGenICamExposureMode* pParam);
			virtual EDeviceGenICamSetFunction SetExposureMode(_In_ EDeviceGenICamExposureMode eParam);

			virtual EDeviceGenICamGetFunction GetExposureTimeRaw(_Out_ int* pParam);
			virtual EDeviceGenICamSetFunction SetExposureTimeRaw(_In_ int nParam);

			virtual EDeviceGenICamGetFunction GetExposureTimeAbs(_Out_ float* pParam);
			virtual EDeviceGenICamSetFunction SetExposureTimeAbs(_In_ float fParam);

			virtual EDeviceGenICamGetFunction GetExposureTimeBaseAbs(_Out_ float* pParam);
			virtual EDeviceGenICamSetFunction SetExposureTimeBaseAbs(_In_ float fParam);

			virtual EDeviceGenICamGetFunction GetExposureTimeBaseAbsEnable(_Out_ bool* pParam);
			virtual EDeviceGenICamSetFunction SetExposureTimeBaseAbsEnable(_In_ bool bParam);

			virtual EDeviceGenICamGetFunction GetAcquisitionLineRateRaw(_Out_ int* pParam);
			virtual EDeviceGenICamSetFunction SetAcquisitionLineRateRaw(_In_ int nParam);

			virtual EDeviceGenICamGetFunction GetAcquisitionLineRateAbs(_Out_ float* pParam);
			virtual EDeviceGenICamSetFunction SetAcquisitionLineRateAbs(_In_ float fParam);

			virtual EDeviceGenICamGetFunction GetResultingLineRateAbs(_Out_ float* pParam);
			virtual EDeviceGenICamSetFunction SetResultingLineRateAbs(_In_ float fParam);

			virtual EDeviceGenICamGetFunction GetAcquisitionFrameRate(_Out_ float* pParam);
			virtual EDeviceGenICamSetFunction SetAcquisitionFrameRate(_In_ float fParam);

			virtual EDeviceGenICamGetFunction GetAcquisitionFrameRateEnable(_Out_ bool* pParam);
			virtual EDeviceGenICamSetFunction SetAcquisitionFrameRateEnable(_In_ bool bParam);

			virtual EDeviceGenICamGetFunction GetResultingFrameRateAbs(_Out_ float* pParam);
			virtual EDeviceGenICamSetFunction SetResultingFrameRateAbs(_In_ float fParam);

			virtual EDeviceGenICamGetFunction GetAcquisitionStatusSelector(_Out_ EDeviceGenICamAcquisitionStatusSelector* pParam);
			virtual EDeviceGenICamSetFunction SetAcquisitionStatusSelector(_In_ EDeviceGenICamAcquisitionStatusSelector eParam);

			virtual EDeviceGenICamGetFunction GetAcquisitionStatus(_Out_ bool* pParam);
			virtual EDeviceGenICamSetFunction SetAcquisitionStatus(_In_ bool bParam);

			virtual EDeviceGenICamGetFunction GetLineSelector(_Out_ EDeviceGenICamLineSelector* pParam);
			virtual EDeviceGenICamSetFunction SetLineSelector(_In_ EDeviceGenICamLineSelector eParam);

			virtual EDeviceGenICamGetFunction GetLineMode_Line1(_Out_ EDeviceGenICamLineMode* pParam);
			virtual EDeviceGenICamSetFunction SetLineMode_Line1(_In_ EDeviceGenICamLineMode eParam);

			virtual EDeviceGenICamGetFunction GetLineSource_Line1(_Out_ EDeviceGenICamLineSource* pParam);
			virtual EDeviceGenICamSetFunction SetLineSource_Line1(_In_ EDeviceGenICamLineSource eParam);

			virtual EDeviceGenICamGetFunction GetLineFormat_Line1(_Out_ EDeviceGenICamLineFormat* pParam);
			virtual EDeviceGenICamSetFunction SetLineFormat_Line1(_In_ EDeviceGenICamLineFormat eParam);

			virtual EDeviceGenICamGetFunction GetLineInverter_Line1(_Out_ bool* pParam);
			virtual EDeviceGenICamSetFunction SetLineInverter_Line1(_In_ bool bParam);

			virtual EDeviceGenICamGetFunction GetLineTermination_Line1(_Out_ bool* pParam);
			virtual EDeviceGenICamSetFunction SetLineTermination_Line1(_In_ bool bParam);

			virtual EDeviceGenICamGetFunction GetLineDebouncerTimeRaw_Line1(_Out_ int* pParam);
			virtual EDeviceGenICamSetFunction SetLineDebouncerTimeRaw_Line1(_In_ int nParam);

			virtual EDeviceGenICamGetFunction GetLineDebouncerTimeAbs_Line1(_Out_ float* pParam);
			virtual EDeviceGenICamSetFunction SetLineDebouncerTimeAbs_Line1(_In_ float fParam);

			virtual EDeviceGenICamGetFunction GetLineMode_Line2(_Out_ EDeviceGenICamLineMode* pParam);
			virtual EDeviceGenICamSetFunction SetLineMode_Line2(_In_ EDeviceGenICamLineMode eParam);

			virtual EDeviceGenICamGetFunction GetLineSource_Line2(_Out_ EDeviceGenICamLineSource* pParam);
			virtual EDeviceGenICamSetFunction SetLineSource_Line2(_In_ EDeviceGenICamLineSource eParam);

			virtual EDeviceGenICamGetFunction GetLineFormat_Line2(_Out_ EDeviceGenICamLineFormat* pParam);
			virtual EDeviceGenICamSetFunction SetLineFormat_Line2(_In_ EDeviceGenICamLineFormat eParam);

			virtual EDeviceGenICamGetFunction GetLineInverter_Line2(_Out_ bool* pParam);
			virtual EDeviceGenICamSetFunction SetLineInverter_Line2(_In_ bool bParam);

			virtual EDeviceGenICamGetFunction GetLineTermination_Line2(_Out_ bool* pParam);
			virtual EDeviceGenICamSetFunction SetLineTermination_Line2(_In_ bool bParam);

			virtual EDeviceGenICamGetFunction GetLineDebouncerTimeRaw_Line2(_Out_ int* pParam);
			virtual EDeviceGenICamSetFunction SetLineDebouncerTimeRaw_Line2(_In_ int nParam);

			virtual EDeviceGenICamGetFunction GetLineDebouncerTimeAbs_Line2(_Out_ float* pParam);
			virtual EDeviceGenICamSetFunction SetLineDebouncerTimeAbs_Line2(_In_ float fParam);

			virtual EDeviceGenICamGetFunction GetLineMode_Out1(_Out_ EDeviceGenICamLineMode* pParam);
			virtual EDeviceGenICamSetFunction SetLineMode_Out1(_In_ EDeviceGenICamLineMode eParam);

			virtual EDeviceGenICamGetFunction GetLineSource_Out1(_Out_ EDeviceGenICamLineSource* pParam);
			virtual EDeviceGenICamSetFunction SetLineSource_Out1(_In_ EDeviceGenICamLineSource eParam);

			virtual EDeviceGenICamGetFunction GetLineFormat_Out1(_Out_ EDeviceGenICamLineFormat* pParam);
			virtual EDeviceGenICamSetFunction SetLineFormat_Out1(_In_ EDeviceGenICamLineFormat eParam);

			virtual EDeviceGenICamGetFunction GetLineInverter_Out1(_Out_ bool* pParam);
			virtual EDeviceGenICamSetFunction SetLineInverter_Out1(_In_ bool bParam);

			virtual EDeviceGenICamGetFunction GetLineTermination_Out1(_Out_ bool* pParam);
			virtual EDeviceGenICamSetFunction SetLineTermination_Out1(_In_ bool bParam);

			virtual EDeviceGenICamGetFunction GetLineDebouncerTimeRaw_Out1(_Out_ int* pParam);
			virtual EDeviceGenICamSetFunction SetLineDebouncerTimeRaw_Out1(_In_ int nParam);

			virtual EDeviceGenICamGetFunction GetLineDebouncerTimeAbs_Out1(_Out_ float* pParam);
			virtual EDeviceGenICamSetFunction SetLineDebouncerTimeAbs_Out1(_In_ float fParam);

			virtual EDeviceGenICamGetFunction GetLineMode_Out2(_Out_ EDeviceGenICamLineMode* pParam);
			virtual EDeviceGenICamSetFunction SetLineMode_Out2(_In_ EDeviceGenICamLineMode eParam);

			virtual EDeviceGenICamGetFunction GetLineSource_Out2(_Out_ EDeviceGenICamLineSource* pParam);
			virtual EDeviceGenICamSetFunction SetLineSource_Out2(_In_ EDeviceGenICamLineSource eParam);

			virtual EDeviceGenICamGetFunction GetLineFormat_Out2(_Out_ EDeviceGenICamLineFormat* pParam);
			virtual EDeviceGenICamSetFunction SetLineFormat_Out2(_In_ EDeviceGenICamLineFormat eParam);

			virtual EDeviceGenICamGetFunction GetLineInverter_Out2(_Out_ bool* pParam);
			virtual EDeviceGenICamSetFunction SetLineInverter_Out2(_In_ bool bParam);

			virtual EDeviceGenICamGetFunction GetLineTermination_Out2(_Out_ bool* pParam);
			virtual EDeviceGenICamSetFunction SetLineTermination_Out2(_In_ bool bParam);

			virtual EDeviceGenICamGetFunction GetLineDebouncerTimeRaw_Out2(_Out_ int* pParam);
			virtual EDeviceGenICamSetFunction SetLineDebouncerTimeRaw_Out2(_In_ int nParam);

			virtual EDeviceGenICamGetFunction GetLineDebouncerTimeAbs_Out2(_Out_ float* pParam);
			virtual EDeviceGenICamSetFunction SetLineDebouncerTimeAbs_Out2(_In_ float fParam);

			virtual EDeviceGenICamGetFunction GetLineMode_Out3(_Out_ EDeviceGenICamLineMode* pParam);
			virtual EDeviceGenICamSetFunction SetLineMode_Out3(_In_ EDeviceGenICamLineMode eParam);

			virtual EDeviceGenICamGetFunction GetLineSource_Out3(_Out_ EDeviceGenICamLineSource* pParam);
			virtual EDeviceGenICamSetFunction SetLineSource_Out3(_In_ EDeviceGenICamLineSource eParam);

			virtual EDeviceGenICamGetFunction GetLineFormat_Out3(_Out_ EDeviceGenICamLineFormat* pParam);
			virtual EDeviceGenICamSetFunction SetLineFormat_Out3(_In_ EDeviceGenICamLineFormat eParam);

			virtual EDeviceGenICamGetFunction GetLineInverter_Out3(_Out_ bool* pParam);
			virtual EDeviceGenICamSetFunction SetLineInverter_Out3(_In_ bool bParam);

			virtual EDeviceGenICamGetFunction GetLineTermination_Out3(_Out_ bool* pParam);
			virtual EDeviceGenICamSetFunction SetLineTermination_Out3(_In_ bool bParam);

			virtual EDeviceGenICamGetFunction GetLineDebouncerTimeRaw_Out3(_Out_ int* pParam);
			virtual EDeviceGenICamSetFunction SetLineDebouncerTimeRaw_Out3(_In_ int nParam);

			virtual EDeviceGenICamGetFunction GetLineDebouncerTimeAbs_Out3(_Out_ float* pParam);
			virtual EDeviceGenICamSetFunction SetLineDebouncerTimeAbs_Out3(_In_ float fParam);

			virtual EDeviceGenICamGetFunction GetLineMode_Out4(_Out_ EDeviceGenICamLineMode* pParam);
			virtual EDeviceGenICamSetFunction SetLineMode_Out4(_In_ EDeviceGenICamLineMode eParam);

			virtual EDeviceGenICamGetFunction GetLineSource_Out4(_Out_ EDeviceGenICamLineSource* pParam);
			virtual EDeviceGenICamSetFunction SetLineSource_Out4(_In_ EDeviceGenICamLineSource eParam);

			virtual EDeviceGenICamGetFunction GetLineFormat_Out4(_Out_ EDeviceGenICamLineFormat* pParam);
			virtual EDeviceGenICamSetFunction SetLineFormat_Out4(_In_ EDeviceGenICamLineFormat eParam);

			virtual EDeviceGenICamGetFunction GetLineInverter_Out4(_Out_ bool* pParam);
			virtual EDeviceGenICamSetFunction SetLineInverter_Out4(_In_ bool bParam);

			virtual EDeviceGenICamGetFunction GetLineTermination_Out4(_Out_ bool* pParam);
			virtual EDeviceGenICamSetFunction SetLineTermination_Out4(_In_ bool bParam);

			virtual EDeviceGenICamGetFunction GetLineDebouncerTimeRaw_Out4(_Out_ int* pParam);
			virtual EDeviceGenICamSetFunction SetLineDebouncerTimeRaw_Out4(_In_ int nParam);

			virtual EDeviceGenICamGetFunction GetLineDebouncerTimeAbs_Out4(_Out_ float* pParam);
			virtual EDeviceGenICamSetFunction SetLineDebouncerTimeAbs_Out4(_In_ float fParam);

			virtual EDeviceGenICamGetFunction GetUserOutputSelector(_Out_ EDeviceGenICamUserOutputSelector* pParam);
			virtual EDeviceGenICamSetFunction SetUserOutputSelector(_In_ EDeviceGenICamUserOutputSelector eParam);

			virtual EDeviceGenICamGetFunction GetTimerDurationTimebaseAbs(_Out_ float* pParam);
			virtual EDeviceGenICamSetFunction SetTimerDurationTimebaseAbs(_In_ float fParam);

			virtual EDeviceGenICamGetFunction GetTimerDelayTimebaseAbs(_Out_ float* pParam);
			virtual EDeviceGenICamSetFunction SetTimerDelayTimebaseAbs(_In_ float fParam);

			virtual EDeviceGenICamGetFunction GetTimerSelector(_Out_ EDeviceGenICamTimerSelector* pParam);
			virtual EDeviceGenICamSetFunction SetTimerSelector(_In_ EDeviceGenICamTimerSelector eParam);

			virtual EDeviceGenICamGetFunction GetTimerDurationAbs(_Out_ float* pParam);
			virtual EDeviceGenICamSetFunction SetTimerDurationAbs(_In_ float fParam);

			virtual EDeviceGenICamGetFunction GetTimerDurationRaw(_Out_ int* pParam);
			virtual EDeviceGenICamSetFunction SetTimerDurationRaw(_In_ int nParam);

			virtual EDeviceGenICamGetFunction GetTimerDelayAbs(_Out_ float* pParam);
			virtual EDeviceGenICamSetFunction SetTimerDelayAbs(_In_ float fParam);

			virtual EDeviceGenICamGetFunction GetTimerDelayRaw(_Out_ int* pParam);
			virtual EDeviceGenICamSetFunction SetTimerDelayRaw(_In_ int nParam);

			virtual EDeviceGenICamGetFunction GetTimerTriggerSource(_Out_ EDeviceGenICamTimerTriggerSource* pParam);
			virtual EDeviceGenICamSetFunction SetTimerTriggerSource(_In_ EDeviceGenICamTimerTriggerSource eParam);

			virtual EDeviceGenICamGetFunction GetTimerTriggerActivation(_Out_ EDeviceGenICamTimerTriggerActivation* pParam);
			virtual EDeviceGenICamSetFunction SetTimerTriggerActivation(_In_ EDeviceGenICamTimerTriggerActivation eParam);

			virtual EDeviceGenICamGetFunction GetTimerSequenceEnable(_Out_ bool* pParam);
			virtual EDeviceGenICamSetFunction SetTimerSequenceEnable(_In_ bool bParam);

			virtual EDeviceGenICamGetFunction GetTimerSequenceLastEntryIndex(_Out_ int* pParam);
			virtual EDeviceGenICamSetFunction SetTimerSequenceLastEntryIndex(_In_ int nParam);

			virtual EDeviceGenICamGetFunction GetTimerSequenceCurrentEntryIndex(_Out_ int* pParam);
			virtual EDeviceGenICamSetFunction SetTimerSequenceCurrentEntryIndex(_In_ int nParam);

			virtual EDeviceGenICamGetFunction GetTimerSequenceEntrySelector(_Out_ EDeviceGenICamTimerSequenceEntrySelector* pParam);
			virtual EDeviceGenICamSetFunction SetTimerSequenceEntrySelector(_In_ EDeviceGenICamTimerSequenceEntrySelector eParam);

			virtual EDeviceGenICamGetFunction GetTimerSequenceTimerSelector(_Out_ EDeviceGenICamTimerSequenceTimerSelector* pParam);
			virtual EDeviceGenICamSetFunction SetTimerSequenceTimerSelector(_In_ EDeviceGenICamTimerSequenceTimerSelector eParam);

			virtual EDeviceGenICamGetFunction GetTimerSequenceTimerEnable(_Out_ bool* pParam);
			virtual EDeviceGenICamSetFunction SetTimerSequenceTimerEnable(_In_ bool bParam);

			virtual EDeviceGenICamGetFunction GetTimerSequenceTimerInverter(_Out_ bool* pParam);
			virtual EDeviceGenICamSetFunction SetTimerSequenceTimerInverter(_In_ bool bParam);

			virtual EDeviceGenICamGetFunction GetTimerSequenceTimerDelayRaw(_Out_ int* pParam);
			virtual EDeviceGenICamSetFunction SetTimerSequenceTimerDelayRaw(_In_ int nParam);

			virtual EDeviceGenICamGetFunction GetTimerSequenceTimerDurationRaw(_Out_ int* pParam);
			virtual EDeviceGenICamSetFunction SetTimerSequenceTimerDurationRaw(_In_ int nParam);

			virtual EDeviceGenICamGetFunction GetCounterSelector(_Out_ EDeviceGenICamCounterSelector* pParam);
			virtual EDeviceGenICamSetFunction SetCounterSelector(_In_ EDeviceGenICamCounterSelector eParam);

			virtual EDeviceGenICamGetFunction GetCounterEventSource(_Out_ EDeviceGenICamCounterEventSource* pParam);
			virtual EDeviceGenICamSetFunction SetCounterEventSource(_In_ EDeviceGenICamCounterEventSource eParam);

			virtual EDeviceGenICamGetFunction GetCounterResetSource(_Out_ EDeviceGenICamCounterResetSource* pParam);
			virtual EDeviceGenICamSetFunction SetCounterResetSource(_In_ EDeviceGenICamCounterResetSource eParam);

			virtual EDeviceGenICamGetFunction GetLUTEnable(_Out_ bool* pParam);
			virtual EDeviceGenICamSetFunction SetLUTEnable(_In_ bool bParam);

			virtual EDeviceGenICamGetFunction GetLUTIndex(_Out_ int* pParam);
			virtual EDeviceGenICamSetFunction SetLUTIndex(_In_ int nParam);

			virtual EDeviceGenICamGetFunction GetLUTValue(_Out_ int* pParam);
			virtual EDeviceGenICamSetFunction SetLUTValue(_In_ int nParam);

			virtual EDeviceGenICamGetFunction GetLUTSelector(_Out_ EDeviceGenICamLUTSelector* pParam);
			virtual EDeviceGenICamSetFunction SetLUTSelector(_In_ EDeviceGenICamLUTSelector eParam);

			virtual EDeviceGenICamGetFunction GetGevLinkSpeed(_Out_ int* pParam);
			virtual EDeviceGenICamSetFunction SetGevLinkSpeed(_In_ int nParam);

			virtual EDeviceGenICamGetFunction GetGevHeartbeatTimeout(_Out_ int* pParam);
			virtual EDeviceGenICamSetFunction SetGevHeartbeatTimeout(_In_ int nParam);

			virtual EDeviceGenICamGetFunction GetGevSCPSPacketSize(_Out_ int* pParam);
			virtual EDeviceGenICamSetFunction SetGevSCPSPacketSize(_In_ int nParam);

			void UpdateDeviceToParameter();

		protected:
			virtual bool DoesModuleExist();

			bool SplitLocalUrl(const std::string& strLocal, std::string& fullFilename, uint64_t& address, size_t& length);

			static UINT LiveThread(_In_ LPVOID pParam);

		protected:
			CWinThread* m_pLiveThread = nullptr;

			HMODULE m_hModule = nullptr;

			GenApi_3_2::CNodeMapRef* m_pNodeMapDevice = nullptr;
		};
	}
}

