#pragma once

#include "DeviceMotion.h"

#ifdef WIN32
#define WMX3APIFUNC long __stdcall
#define ECAPIFUNC long __stdcall
#else
#define WMX3APIFUNC HRESULT
#define ECAPIFUNC HRESULT
#endif

namespace wmx3Api
{
	class WMX3Api;
	class CoreMotion;
	class Config;
	class AxisControl;
	class Velocity;
	class Torque;
	class LogInput;

	namespace ecApi
	{
		class Ecat;
	}
}

namespace Ravid
{
	namespace Device
	{
		namespace __constants
		{
			static const int maxAxes = 128;
			static const int maxIoInSize = 8000;
			static const int maxIoOutSize = 8000;
			static const int moduleFileNameLen = 32;
			static const int moduleNameLen = 16;
			static const int moduleDescLen = 512;
			static const int moduleLen = 64;
			static const int maxDeviceName = 64;
			static const int maxDevices = 256;
			static const int maxInterrupts = 2;
			static const int maxEventInterpolationAxes = 8;


			static const int maxFlightRecorderBufferSize = 5000;
			static const int maxSyncGroup = 64;
			static const int maxTriggerEvents = 8;
			static const int maxDataChannelInstances = 256;
			static const unsigned long long int maxProfileUnsignedInput = 274877906943ULL;
		}

		namespace __constants_Ecat
		{
			static const int maxMasters = 2;
			static const int maxSlaves = 256;
			static const int maxSlaveAxes = 16;
			static const int maxEniFilePathLen = 256;
			static const int maxFoEFilePathLen = 256;
			static const int maxFoEFileNameLen = 64;
			static const int maxSdoInfoNameLen = 64;
			static const int maxSdoInfoODListSize = 450;
			static const int maxSdoInfoEDListSize = 50;
		}

		class AFX_EXT_CLASS CDeviceSoftservoWMX3 : public CDeviceMotion
		{

		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceSoftservoWMX3);

			DECLARE_DYNAMIC(CDeviceSoftservoWMX3)

			CDeviceSoftservoWMX3();
			virtual ~CDeviceSoftservoWMX3();

			virtual EDeviceInitializeResult Initialize() override;
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

		public:
			virtual ESoftservoGetFunction GetInitializetype(_Out_ EDeviceSoftservoWMX3InitializeType* pParam);
			virtual ESoftservoSetFunction SetInitializetype(_In_ EDeviceSoftservoWMX3InitializeType eParam);

			virtual ESoftservoGetFunction GetMotionInfoPath(_Out_ CString* pParam);
			virtual ESoftservoSetFunction SetMotionInfoPath(_In_ CString strParam);

			virtual ESoftservoGetFunction GetParamPath(_Out_ CString* pParam);
			virtual ESoftservoSetFunction SetParamPath(_In_ CString strParam);

			virtual ESoftservoGetFunction GetStartSpeed(_Out_ double* pParam);
			virtual ESoftservoSetFunction SetStartSpeed(_In_ double dblParam);

			virtual ESoftservoGetFunction GetEndSpeed(_Out_ double* pParam);
			virtual ESoftservoSetFunction SetEndSpeed(_In_ double dblParam);

			virtual ESoftservoGetFunction GetHomeLevel(_Out_ EDeviceSoftservoWMX3LevelMethod* pParam);
			virtual ESoftservoSetFunction SetHomeLevel(_In_ EDeviceSoftservoWMX3LevelMethod eParam);

			virtual ESoftservoGetFunction GetPLimitLevel(_Out_ EDeviceSoftservoWMX3LevelMethod* pParam);
			virtual ESoftservoSetFunction SetPLimitLevel(_In_ EDeviceSoftservoWMX3LevelMethod eParam);

			virtual ESoftservoGetFunction GetNearPLimitLevel(_Out_ EDeviceSoftservoWMX3LevelMethod* pParam);
			virtual ESoftservoSetFunction SetNearPLimitLevel(_In_ EDeviceSoftservoWMX3LevelMethod eParam);

			virtual ESoftservoGetFunction GetExternPLimitLevel(_Out_ EDeviceSoftservoWMX3LevelMethod* pParam);
			virtual ESoftservoSetFunction SetExternPLimitLevel(_In_ EDeviceSoftservoWMX3LevelMethod eParam);

			virtual ESoftservoGetFunction GetNLimitLevel(_Out_ EDeviceSoftservoWMX3LevelMethod* pParam);
			virtual ESoftservoSetFunction SetNLimitLevel(_In_ EDeviceSoftservoWMX3LevelMethod eParam);

			virtual ESoftservoGetFunction GetNearNLimitLevel(_Out_ EDeviceSoftservoWMX3LevelMethod* pParam);
			virtual ESoftservoSetFunction SetNearNLimitLevel(_In_ EDeviceSoftservoWMX3LevelMethod eParam);

			virtual ESoftservoGetFunction GetExternNLimitLevel(_Out_ EDeviceSoftservoWMX3LevelMethod* pParam);
			virtual ESoftservoSetFunction SetExternNLimitLevel(_In_ EDeviceSoftservoWMX3LevelMethod eParam);

			virtual ESoftservoGetFunction GetGearRatioDenominator(_Out_ DWORD* pParam);
			virtual ESoftservoSetFunction SetGearRatioDenominator(_In_ DWORD dwParam);

			virtual ESoftservoGetFunction GetGearRatioNumerator(_Out_ DWORD* pParam);
			virtual ESoftservoSetFunction SetGearRatioNumerator(_In_ DWORD dwParam);

			virtual ESoftservoGetFunction GetInposWidth(_Out_ long* pParam);
			virtual ESoftservoSetFunction SetInposWidth(_In_ long nParam);

			virtual ESoftservoGetFunction GetPosCompletedWidth(_Out_ long* pParam);
			virtual ESoftservoSetFunction SetPosCompletedWidth(_In_ long nParam);

			virtual ESoftservoGetFunction GetHomeType(_Out_ EDeviceSoftservoWMX3HomeType* pParam);
			virtual ESoftservoSetFunction SetHomeType(_In_ EDeviceSoftservoWMX3HomeType eParam);

			virtual ESoftservoGetFunction GetHomeDirection(_Out_ EDeviceSoftservoWMX3HomeDirection* pParam);
			virtual ESoftservoSetFunction SetHomeDirection(_In_ EDeviceSoftservoWMX3HomeDirection eParam);

			virtual ESoftservoGetFunction GetHomeFastVelocity(_Out_ double* pParam);
			virtual ESoftservoSetFunction SetHomeFastVelocity(_In_ double dblParam);

			virtual ESoftservoGetFunction GetHomeFastAccel(_Out_ double* pParam);
			virtual ESoftservoSetFunction SetHomeFastAccel(_In_ double dblParam);

			virtual ESoftservoGetFunction GetHomeFastDeceleration(_Out_ double* pParam);
			virtual ESoftservoSetFunction SetHomeFastDeceleration(_In_ double dblParam);

			virtual ESoftservoGetFunction GetHomeSlowVelocity(_Out_ double* pParam);
			virtual ESoftservoSetFunction SetHomeSlowVelocity(_In_ double dblParam);

			virtual ESoftservoGetFunction GetHomeSlowAccel(_Out_ double* pParam);
			virtual ESoftservoSetFunction SetHomeSlowAccel(_In_ double dblParam);

			virtual ESoftservoGetFunction GetHomeSlowDeceleration(_Out_ double* pParam);
			virtual ESoftservoSetFunction SetHomeSlowDeceleration(_In_ double dblParam);

			virtual ESoftservoGetFunction GetHomeShiftVelocity(_Out_ double* pParam);
			virtual ESoftservoSetFunction SetHomeShiftVelocity(_In_ double dblParam);

			virtual ESoftservoGetFunction GetHomeShiftAccel(_Out_ double* pParam);
			virtual ESoftservoSetFunction SetHomeShiftAccel(_In_ double dblParam);

			virtual ESoftservoGetFunction GetHomeShiftDeceleration(_Out_ double* pParam);
			virtual ESoftservoSetFunction SetHomeShiftDeceleration(_In_ double dblParam);

			virtual ESoftservoGetFunction GetHomeShiftDistance(_Out_ double* pParam);
			virtual ESoftservoSetFunction SetHomeShiftDistance(_In_ double dblParam);

			virtual bool GetServoStatus() override;
			virtual bool SetServoOn(_In_ bool bOn) override;

			virtual bool GetCommandPosition(_Out_ double *pPos) override;
			virtual bool SetCommandPositionClear() override;

			virtual bool GetActualPosition(_Out_ double *pPos) override;
			virtual bool SetActualPositionClear() override;

			virtual bool GetAlarmStatus() override;
			virtual bool SetAlarmClear() override;

			virtual bool GetInposition() override;
			virtual bool GetLimitSensorN() override;
			virtual bool GetLimitSensorP() override;
			virtual bool GetHomeSensor() override;

			virtual bool MovePosition(_In_ double dblPos, _In_ double dblVel, _In_ double dblAcc, _In_ bool bSCurve) override;
			virtual bool MoveDistance(_In_ double dblDist, _In_ double dblVel, _In_ double dblAcc, _In_ bool bSCurve) override;
			virtual bool MoveVelocity(_In_ double dblVel, _In_ double dblAcc, _In_ bool bSCurve) override;

			virtual bool MoveJog(_In_ double dblVel, _In_ double dblAcc, _In_ BOOL bSCurve) override;
			virtual bool StopJog() override;

			virtual bool IsMotionDone() override;

			virtual bool MotorStop() override;
			virtual bool MotorEStop() override;

			virtual bool ChangeSpeed(_In_ double dSpeed) override;

			virtual bool WriteGenOutBit(_In_ int nBit, _In_ bool bOn) override;
			virtual bool ReadGenOutBit(_In_ int nBit) override;
			virtual bool ReadGenInputBit(_In_ int nBit) override;

			virtual bool MoveToHome() override;

			virtual EDeviceMotionHommingStatus GetHommingStatus() override;

			wmx3Api::WMX3Api* GetDevice();
			wmx3Api::CoreMotion* GetCoreMotion();

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;

			bool GetOnlineAxisCount(int* pAxisCount);

		public:
			DECLARE_MESSAGE_MAP()

		public:
			class __DeviceType
			{
			public:
				enum T
				{
					DeviceTypeNormal = 0,
					DeviceTypeLowPriority = 0x1,
					DeviceTypeExitWOCnt = 0x02,
					DeviceTypeLowpriorityExitWOCnt = 0x03
				};
			};

			class __DeviceStatistics
			{
			public:
				__DeviceStatistics() { }

				unsigned int sysFailedCount;
				unsigned int apiSuccessCount;
				unsigned int apiFailedCount;

				unsigned int curApiTime;
				unsigned int avgApiTime;
				unsigned int minApiTime;
				unsigned int maxApiTime;
				LARGE_INTEGER sumApiTime;

				unsigned int rtCount;
				unsigned int curRtTime;
				unsigned int avgRtTime;
				unsigned int minRtTime;
				unsigned int maxRtTime;
				LARGE_INTEGER sumRtTime;

				unsigned int curFuncProcTime;
				unsigned int avgFuncProcTime;
				unsigned int minFuncProcTime;
				unsigned int maxFuncProcTime;
				LARGE_INTEGER sumFuncProcTime;
			};

			class __EngineInfo
			{
			public:
				__EngineInfo() { }

				int majorVersion;
				int minorVersion;
				int revisionVersion;
				int fixVersion;

				int imlibVersion;
				int imlibRevision;

				int numLicensedAxes;
				unsigned char options[64];
			};

			class __ModuleType
			{
			public:
				enum T
				{
					Motion,
					Platform,

					Unknown
				};
			};

			class __ModuleInfoA
			{
			public:
				__ModuleInfoA() { }

				unsigned char suspend;
				__ModuleType::T type;
				int id;
				int slot;
				char fileName[__constants::moduleFileNameLen];
				char moduleName[__constants::moduleNameLen];
				char moduleDesc[__constants::moduleDescLen];

				int majorVersion;
				int minorVersion;
				int revisionVersion;
				int fixVersion;

				int memory;
			};

			class __ModulesInfoA
			{
			public:
				__ModulesInfoA() { }

				__EngineInfo engine;

				int numOfModule;
				__ModuleInfoA modules[__constants::moduleLen];
			};

			class __ModuleInfoW
			{
			public:
				__ModuleInfoW() { }

				unsigned char suspend;
				__ModuleType::T type;
				int id;
				int slot;
				wchar_t fileName[__constants::moduleFileNameLen];
				wchar_t moduleName[__constants::moduleNameLen];
				wchar_t moduleDesc[__constants::moduleDescLen];

				int majorVersion;
				int minorVersion;
				int revisionVersion;
				int fixVersion;

				int memory;
			};

			class __ModulesInfoW
			{
			public:
				__ModulesInfoW() { }

				__EngineInfo engine;

				int numOfModule;
				__ModuleInfoW modules[__constants::moduleLen];
			};

			class __DeviceInfoA
			{
			public:
				__DeviceInfoA() { }

				unsigned int id;
				__DeviceType::T type;
				unsigned int watchdog;
				unsigned int watchdogCount;
				char name[__constants::maxDeviceName];
			};

			class __DeviceInfoW
			{
			public:
				__DeviceInfoW() { }

				unsigned int id;
				__DeviceType::T type;
				unsigned int watchdog;
				unsigned int watchdogCount;
				wchar_t name[__constants::maxDeviceName];
			};

			class __DevicesInfoA
			{
			public:
				__DevicesInfoA() { }

				unsigned int count;
				__DeviceInfoA devices[__constants::maxDevices];
			};

			class __DevicesInfoW
			{
			public:
				__DevicesInfoW() { }

				unsigned int count;
				__DeviceInfoW devices[__constants::maxDevices];
			};

			class __EngineState
			{
			public:
				enum T
				{
					Idle,
					Running,
					Communicating,
					Shutdown,

					Unknown
				};
			};

			class __EngineStatus
			{
			public:
				__EngineStatus() { }

				__EngineState::T state;
				unsigned int error;
			};

			class __ModuleSuspend
			{
			public:
				__ModuleSuspend() { }

				unsigned char module[__constants::moduleLen];
			};

			class __EStopLevel
			{
			public:
				enum T
				{
					Final,
					Level1
				};
			};

			class __AxisCompensation
			{
			public:
				__AxisCompensation() { }

				double pitchErrorCompensation;
				double pitchErrorCompensation2D;
				double backlashCompensation;
				double totalPosCompensation;
			};

			class __AxisSupportedFunction
			{
			public:
				__AxisSupportedFunction() { }
				bool posFeedbackSupport;
				bool posCommandSupport;
				bool velocityFeedbackSupport;
				bool velocityCommandSupport;
				bool velocityOffsetSupport;
				bool trqFeedbackSupport;
				bool trqCommandSupport;
				bool maxTrqLimitSupport;
				bool positiveTrqLimitSupport;
				bool negativeTrqLimitSupport;
				bool maxMotorSpeedSupport;
			};

			class __OperationState
			{
			public:
				enum T
				{
					Idle,
					Pos,
					Jog,
					Home,
					Sync,
					GantryHome,
					Stop,
					Intpl,
					Velocity,
					ConstLinearVelocity,
					Trq,
					DirectControl,
					PVT,
					ECAM,
					SyncCatchUp,
					DancerControl
				};
			};

			class __DetailOperationState
			{
			public:
				enum T
				{
					Idle = 0,
					Pos = 200,
					Pos_OverrideSetup,
					Pos_WaitingForTrigger,
					Jog = 300,
					Jog_OverrideSetup,
					Home = 400,
					Sync = 500,
					Sync_PhaseShift,
					Sync_GearShift,
					GantryHome = 600,
					Stop = 700,
					Stop_QStop,
					Stop_EStop,
					Intpl = 800,
					Intpl_Linear,
					Intpl_Circular,
					Intpl_Helical,
					Intpl_Spline,
					Intpl_Path,
					Intpl_PathWithRotation,
					Intpl_PathLookahead,
					Intpl_OverrideSetup,
					Intpl_OverrideSmoothing,
					Velocity = 900,
					Velocity_OverrideSetup,
					ConstLinearVelocity = 1100,
					Trq = 1200,
					DirectControl = 1300,
					PVT = 1400,
					ECAM = 1500,
					SyncCatchUp = 1600,
					DancerControl = 1700
				};
			};

			class __AxisCommandMode
			{
			public:
				enum T
				{
					Position,
					Velocity,
					Torque
				};
			};

			class __AxisSyncMode
			{
			public:
				enum T
				{
					NoSync,
					NoOffset,
					VelocityOffset,
					SymmetricVelocityOffset
				};
			};

			class __HomeState
			{
			public:
				enum T
				{
					Idle,
					ZPulseSearch,
					ZPulseSearchReverse,
					ZPulseSearchPauseReverse,
					HSSearch,
					HSSearchPause,
					HSAndZPulseSearch,
					HSAndZPulseSearchPause,
					HSOffSearch,
					HSOffSearchPause,
					HSOffAndZPulseSearch,
					HSOffAndZPulseSearchPause,
					LSSearch,
					LSSearchPause,
					HSClearReverse,
					HSClearReversePause,
					HSFallingEdgeSearchReverse,
					HSFallingEdgeSearchReversePause,
					LSFallingEdgeSearchReverse,
					LSFallingEdgeSearchReversePause,
					TouchProbeSearch,
					TouchProbeSearchPause,
					SecondHSSearch,
					SecondHSSearchPause,
					SecondTouchProbeSearch,
					SecondTouchProbeSearchPause,
					MechanicalEndDetection,
					HomeShift,
					HomeShiftPause,
					Cancel,
					Other
				};
			};

			class __HomeError
			{
			public:
				enum T
				{
					NoError,
					LSTriggered,
					MaxLSRevDistanceTraveled,
					MaxHSOnAtStartRevDistanceTraveled,
					ZPulseDistanceCheckError
				};
			};

			class __CoreMotionAxisStatus
			{
			public:
				__CoreMotionAxisStatus() { }
				bool servoOn;
				bool servoOffline;
				bool ampAlarm;
				int ampAlarmCode;
				int masterAxis;
				int secondMasterAxis;
				double posCmd;
				double actualPos;
				double compPosCmd;
				double compActualPos;
				double syncPosCmd;
				double syncActualPos;
				int encoderCommand;
				int encoderFeedback;
				long long accumulatedEncoderFeedback;
				double velocityCmd;
				double actualVelocity;
				double velocityLag;
				double torqueCmd;
				double actualTorque;
				double actualFollowingError;
				__AxisCompensation compensation;
				__AxisSupportedFunction axisSupportedFunction;
				__OperationState::T opState;
				__DetailOperationState::T detailOpState;
				__AxisCommandMode::T axisCommandMode;
				__AxisSyncMode::T axisSyncMode;
				double syncOffset;
				double syncPhaseOffset;
				double syncGearRatio;
				bool followingErrorAlarm;
				bool commandReady;
				bool motionPaused;
				double profileTotalMilliseconds;
				double profileAccMilliseconds;
				double profileCruiseMilliseconds;
				double profileDecMilliseconds;
				double profileRemainingMilliseconds;
				double profileCompletedMilliseconds;
				double profileTargetPos;
				double profileTotalDistance;
				double profileRemainingDistance;
				double profileCompletedDistance;
				double intplVelocity;
				int intplSegment;
				double cmdAcc;
				bool accFlag;
				bool decFlag;
				bool inPos;
				bool inPos2;
				bool inPos3;
				bool inPos4;
				bool inPos5;
				bool cmdDistributionEnd;
				bool posSet;
				bool delayedPosSet;
				unsigned int cmdDistributionEndDelayedPosSetDiff;
				bool positiveLS;
				bool negativeLS;
				bool nearPositiveLS;
				bool nearNegativeLS;
				bool externalPositiveLS;
				bool externalNegativeLS;
				bool positiveSoftLimit;
				bool negativeSoftLimit;
				__HomeState::T homeState;
				__HomeError::T homeError;
				bool homeSwitch;
				bool homeDone;
				bool homePaused;
				double homeOffset;
				bool cmdPosToFbPosFlag;
				bool execSuperimposedMotion;
				unsigned int singleTurnCounter;
				bool motionComplete;
			};

			class __CoreMotionStatus
			{
			public:
				__CoreMotionStatus() { }
				bool invalidLicenseError;
				__EngineState::T engineState;
				int numOfInterrupts;
				double cycleTimeMilliseconds[__constants::maxInterrupts];
				long long cycleCounter[__constants::maxInterrupts];
				bool emergencyStop;
				__EStopLevel::T emergencyStopLevel;
				__CoreMotionAxisStatus axesStatus[__constants::maxAxes];
			};

			class __AxisSelection
			{
			public:
				__AxisSelection() { }
				__AxisSelection(int axisCount, const int * axis);

				int axisCount;
				int axis[__constants::maxAxes];
			};

			/////////////////////////////////////

			class __VelocityMonitorSource
			{
			public:
				enum T
				{
					UseVelocityFeedback,
					CalculateFromPositionFeedback
				};
			};

			class __FeedbackParam
			{
			public:
				__FeedbackParam() { }
				double inPosWidth;
				double inPosWidth2;
				double inPosWidth3;
				double inPosWidth4;
				double inPosWidth5;
				__VelocityMonitorSource::T velocityMonitorSource;
				double posSetWidth;
				double delayedPosSetWidth;
				double delayedPosSetMilliseconds;
			};

			class __HomeDirection
			{
			public:
				enum T
				{
					Positive,
					Negative
				};
			};

			class __HomeType
			{
			public:
				enum T
				{
					CurrentPos,
					ZPulse,
					HS,
					HSHS,
					HSZPulse,
					HSReverseZPulse,
					HSOff,
					HSOffZPulse,
					HSOffReverseZPulse,
					LSReverseZPulse,
					NearLSReverseZPulse,
					ExternalLSReverseZPulse,
					TrqLimit,
					TouchProbe,
					HSTouchProbe,
					LS,
					NearLS,
					ExternalLS,
					MechanicalEndDetection,
					MechanicalEndDetectionHS,
					MechanicalEndDetectionLS,
					MechanicalEndDetectionReverseZPulse
				};
			};

			class __HomeParam
			{
			public:
				__HomeParam() { }
				__HomeType::T homeType;
				__HomeDirection::T homeDirection;
				double homingVelocitySlow;
				double homingVelocitySlowAcc;
				double homingVelocitySlowDec;
				double homingVelocityFast;
				double homingVelocityFastAcc;
				double homingVelocityFastDec;
				double homingReverseDistance;
				double homeShiftVelocity;
				double homeShiftAcc;
				double homeShiftDec;
				double homeShiftDistance;
				bool invertHSPolarity;
				unsigned int multipleZPulse;
				bool roundPosCmdAfterHoming;
				bool pauseMode;
				double maxHSOnAtStartReverseDistance;
				double maxLSReverseDistance;
				unsigned int zPulseDistanceCheck;
				double homePosition;
				bool gantryHomingUseSlaveHS;
				bool gantryHomingUseSlaveLS;
				bool gantryHomingUseSlaveZPulse;
				bool gantryHomingUseSlaveTouchProbe;
				bool gantryHomingUseSlaveMechanicalEnd;
				bool gantryHomingRetainSyncOffset;
				bool immediateStopAtLS;
				double mechanicalEndDetectionPosDiff;
				double mechanicalEndDetectionTimeMilliseconds;
				bool mechanicalEndDetectionIgnoreLS;
				double mechanicalEndDetectionFirstTorqueLimit;
				double mechanicalEndDetectionSecondTorqueLimit;
				bool openLoopHoming;
				bool clearHomeDoneOnServoOff;
				bool clearHomeDoneOnCommStop;
			};

			class __LimitSwitchType
			{
			public:
				enum T
				{
					None,
					ServoOff,
					DecServoOff,
					Dec,
					SlowDecServoOff,
					SlowDec,
					SeparatePositiveLSNegativeLS
				};
			};

			class __LimitSwitchDirection
			{
			public:
				enum T
				{
					Normal,
					Reverse
				};
			};

			class __LimitParam
			{
			public:
				__LimitParam() { }
				__LimitSwitchType::T lsType;
				__LimitSwitchType::T positiveLSType;
				__LimitSwitchType::T negativeLSType;
				bool invertPositiveLSPolarity;
				bool invertNegativeLSPolarity;
				__LimitSwitchType::T nearLSType;
				__LimitSwitchType::T nearPositiveLSType;
				__LimitSwitchType::T nearNegativeLSType;
				int nearPositiveLSByte;
				int nearPositiveLSBit;
				bool invertNearPositiveLSPolarity;
				int nearNegativeLSByte;
				int nearNegativeLSBit;
				bool invertNearNegativeLSPolarity;
				__LimitSwitchType::T externalLSType;
				__LimitSwitchType::T externalPositiveLSType;
				__LimitSwitchType::T externalNegativeLSType;
				int externalPositiveLSByte;
				int externalPositiveLSBit;
				bool invertExternalPositiveLSPolarity;
				int externalNegativeLSByte;
				int externalNegativeLSBit;
				bool invertExternalNegativeLSPolarity;
				__LimitSwitchType::T softLimitType;
				__LimitSwitchType::T positiveSoftLimitType;
				__LimitSwitchType::T negativeSoftLimitType;
				double softLimitPositivePos;
				double softLimitNegativePos;
				double lsDec;
				double lsSlowDec;
				bool allLSDuringHoming;
				__LimitSwitchDirection::T lsDirection;
			};

			class __ProhibitOvertravelType
			{
			public:
				enum T
				{
					Disabled,
					ChangeDeceleration,
					ChangeInitialVelocity
				};
			};

			class __LinearIntplOverrideType
			{
			public:
				enum T
				{
					Smoothing,
					Blending,
					FastBlending
				};
			};

			class __LinearIntplProfileCalcMode
			{
			public:
				enum T
				{
					AxisLimit,
					MatchSlowestAxis,
					MatchFarthestAxis
				};
			};

			class __CircularIntplOverrideType
			{
			public:
				enum T
				{
					Blending
				};
			};

			class __MotionParam
			{
			public:
				__MotionParam() { }
				double quickStopDec;
				__ProhibitOvertravelType::T prohibitOvertravel;
				__LinearIntplOverrideType::T linearIntplOverrideType;
				unsigned int linearIntplOverrideSmoothPercent;
				__CircularIntplOverrideType::T circularIntplOverrideType;
				bool interruptedIntplUseQuickStop;
				bool singleTurnReduceToHalfTurn;
				bool enableGlobalStartingVelocity;
				double globalStartingVelocity;
				bool enableGlobalEndVelocity;
				double globalEndVelocity;
				bool enableGlobalMinVelocity;
				double globalMinVelocity;
				bool enableGlobalMovingAverageProfileTimeMilliseconds;
				double globalMovingAverageProfileTimeMilliseconds;
				bool apiWaitUntilMotionStart;
				__LinearIntplProfileCalcMode::T linearIntplProfileCalcMode;
			};

			class __FollowingErrorAlarmType
			{
			public:
				enum T
				{
					NoAction,
					QuickStop
				};
			};

			class __VelocityFollowingErrorAlarmType
			{
			public:
				enum T
				{
					NoAction,
					QuickStop,
					Warning
				};
			};

			class __AlarmParam
			{
			public:
				__AlarmParam() { }
				double followingErrorStopped;
				double followingErrorMoving;
				__FollowingErrorAlarmType::T followingErrorType;
				double velocityFollowingErrorStopped;
				double velocityFollowingErrorStoppedMilliseconds;
				double velocityFollowingErrorMoving;
				double velocityFollowingErrorMovingMilliseconds;
				__VelocityFollowingErrorAlarmType::T velocityFollowingErrorType;
				bool servoOffDuringAmpAlarm;
				int servoOnFollowingError;
			};

			class __SyncCompensationMode
			{
			public:
				enum T
				{
					None,
					VelocityOffset,
					SymmetricVelocityOffset
				};
			};

			class __MasterDesyncType
			{
			public:
				enum T
				{
					NoAction,
					ServoOff,
					DecServoOff,
					Dec
				};
			};

			class __SlaveDesyncType
			{
			public:
				enum T
				{
					NoAction,
					ServoOff,
					ResolveSync
				};
			};

			class __SyncParam
			{
			public:
				__SyncParam() { }
				int inSyncWidth;
				double syncGain;
				__SyncCompensationMode::T syncCompensationMode;
				__MasterDesyncType::T masterDesyncType;
				double masterDesyncDec;
				__SlaveDesyncType::T slaveDesyncType;
				double slaveDesyncDec;
				bool matchPos;
			};

			class __FlightRecorderParam
			{
			public:
				__FlightRecorderParam() { }
				bool enableFlightRecorder;
				bool flightRecorderTimeStamp;
				bool collectAxisFlightRecorderData[__constants::maxAxes];
				bool triggerFlightRecorderOnAmpAlarm;
			};

			class __EStopSignalSource
			{
			public:
				enum T
				{
					Input,
					Output,
					UserMemory
				};
			};

			class __EStopStatusSignalDestination
			{
			public:
				enum T
				{
					Output,
					UserMemory
				};
			};

			class __EStopLevel1Type
			{
			public:
				enum T
				{
					Dec,
					DecServoOff
				};
			};

			class __EmergencyStopParam
			{
			public:
				__EmergencyStopParam() { }

				double eStopDec[__constants::maxAxes];
				bool enableEStopSignal;
				__EStopSignalSource::T eStopSignalSource;
				__EStopLevel::T eStopSignalLevel;
				bool invertEStopSignalPolarity;
				unsigned int eStopSignalByteAddress;
				unsigned char eStopSignalBitAddress;
				bool enableEStopStatusSignal;
				__EStopStatusSignalDestination::T eStopStatusSignalDestination;
				bool invertEStopStatusSignalPolarity;
				unsigned int eStopStatusSignalByteAddress;
				unsigned char eStopStatusSignalBitAddress;
				__EStopLevel1Type::T eStopLevel1Type;
			};

			class __SystemParam
			{
			public:
				__SystemParam() { }
				__FeedbackParam feedbackParam[__constants::maxAxes];
				__HomeParam homeParam[__constants::maxAxes];
				__LimitParam limitParam[__constants::maxAxes];
				__MotionParam motionParam[__constants::maxAxes];
				__AlarmParam alarmParam[__constants::maxAxes];
				__SyncParam syncParam[__constants::maxAxes];
				__FlightRecorderParam flightRecorderParam;
				__EmergencyStopParam emergencyStopParam;

			};

			class __AxisParam
			{
			public:
				__AxisParam() { }
				__AxisCommandMode::T axisCommandMode[__constants::maxAxes];
				double gearRatioNumerator[__constants::maxAxes];
				double gearRatioDenominator[__constants::maxAxes];
				bool singleTurnMode[__constants::maxAxes];
				unsigned int singleTurnEncoderCount[__constants::maxAxes];
				double maxTrqLimit[__constants::maxAxes];
				double negativeTrqLimit[__constants::maxAxes];
				double positiveTrqLimit[__constants::maxAxes];
				double axisUnit[__constants::maxAxes];
				double velocityFeedforwardGain[__constants::maxAxes];
				char axisPolarity[__constants::maxAxes];
				double maxMotorSpeed[__constants::maxAxes];
				bool absoluteEncoderMode[__constants::maxAxes];
				double absoluteEncoderHomeOffset[__constants::maxAxes];
			};

			class __SyncOptions
			{
			public:
				__SyncOptions() { }
				bool useMasterFeedback;
			};

			class __ProfileType
			{
			public:
				enum T
				{
					Trapezoidal,
					SCurve,
					JerkRatio,
					Parabolic,
					Sin,
					AdvancedS,
					TrapezoidalMAT,
					JerkLimited,
					JerkLimitedSCurve,
					JerkLimitedAdvancedS,
					TwoVelocityTrapezoidal,
					TwoVelocitySCurve,
					TwoVelocityJerkRatio,
					TimeAccTrapezoidal,
					TimeAccSCurve,
					TimeAccJerkRatio,
					TimeAccParabolic,
					TimeAccSin,
					TimeAccAdvancedS,
					ConstantDec,
					JerkRatioFixedVelocityT,
					JerkRatioFixedVelocityS,
					JerkLimitedFixedVelocityT,
					JerkLimitedFixedVelocityS
				};
			};

			class __Profile
			{
			public:
				__Profile() { }

				__ProfileType::T type;
				double velocity;
				double acc;
				double dec;
				double jerkAcc;
				double jerkDec;
				double jerkAccRatio;
				double jerkDecRatio;
				double accTimeMilliseconds;
				double decTimeMilliseconds;
				double startingVelocity;
				double endVelocity;
				double secondVelocity;
				double movingAverageTimeMilliseconds;

				/* Using parameters for each profile type
				static __Profile __stdcall SetupTrapezoidal(double velocity, double acc, double dec, double startingVelocity = 0, double endVelocity = 0);
				static __Profile __stdcall SetupSCurve(double velocity, double acc, double dec, double startingVelocity = 0, double endVelocity = 0);
				static __Profile __stdcall SetupJerkRatio(double velocity, double acc, double dec, double jerkAccRatio, double jerkDecRatio, double startingVelocity = 0, double endVelocity = 0);
				static __Profile __stdcall SetupParabolic(double velocity, double acc, double dec, double startingVelocity = 0, double endVelocity = 0);
				static __Profile __stdcall SetupSin(double velocity, double acc, double dec, double startingVelocity = 0, double endVelocity = 0);
				static __Profile __stdcall SetupAdvancedS(double velocity, double acc, double dec, double jerkAccRatio, double jerkDecRatio, double startingVelocity = 0, double endVelocity = 0);
				static __Profile __stdcall SetupTrapezoidalMAT(double velocity, double acc, double dec, double movingAverageTimeMilliseconds, double startingVelocity = 0, double endVelocity = 0);
				static __Profile __stdcall SetupJerkLimited(double velocity, double acc, double dec, double jerkAcc, double jerkDec, double startingVelocity = 0, double endVelocity = 0);
				static __Profile __stdcall SetupJerkLimitedSCurve(double velocity, double acc, double dec, double jerkAcc, double jerkDec, double startingVelocity = 0, double endVelocity = 0);
				static __Profile __stdcall SetupJerkLimitedAdvancedS(double velocity, double acc, double dec, double jerkAcc, double jerkDec, double startingVelocity = 0, double endVelocity = 0);
				static __Profile __stdcall SetupTwoVelocityTrapezoidal(double velocity, double acc, double dec, double secondVelocity, double startingVelocity = 0, double endVelocity = 0);
				static __Profile __stdcall SetupTwoVelocitySCurve(double velocity, double acc, double dec, double secondVelocity, double startingVelocity = 0, double endVelocity = 0);
				static __Profile __stdcall SetupTwoVelocityJerkRatio(double velocity, double acc, double dec, double jerkAccRatio, double jerkDecRatio, double secondVelocity, double startingVelocity = 0, double endVelocity = 0);
				static __Profile __stdcall SetupTimeAccTrapezoidal(double velocity, double accTimeMilliseconds, double decTimeMilliseconds, double startingVelocity = 0, double endVelocity = 0);
				static __Profile __stdcall SetupTimeAccSCurve(double velocity, double accTimeMilliseconds, double decTimeMilliseconds, double startingVelocity = 0, double endVelocity = 0);
				static __Profile __stdcall SetupTimeAccJerkRatio(double velocity, double accTimeMilliseconds, double decTimeMilliseconds, double jerkAccRatio, double jerkDecRatio, double startingVelocity = 0, double endVelocity = 0);
				static __Profile __stdcall SetupTimeAccParabolic(double velocity, double accTimeMilliseconds, double decTimeMilliseconds, double startingVelocity = 0, double endVelocity = 0);
				static __Profile __stdcall SetupTimeAccSin(double velocity, double accTimeMilliseconds, double decTimeMilliseconds, double startingVelocity = 0, double endVelocity = 0);
				static __Profile __stdcall SetupTimeAccAdvancedS(double velocity, double accTimeMilliseconds, double decTimeMilliseconds, double jerkAccRatio, double jerkDecRatio, double startingVelocity = 0, double endVelocity = 0);
				static __Profile __stdcall SetupConstantDec(double startingVelocity, double endVelocity);
				static __Profile __stdcall SetupJerkRatioFixedVelocityT(double velocity, double acc, double dec, double jerkAccRatio, double jerkDecRatio, double startingVelocity = 0, double endVelocity = 0);
				static __Profile __stdcall SetupJerkRatioFixedVelocityS(double velocity, double acc, double dec, double jerkAccRatio, double jerkDecRatio, double startingVelocity = 0, double endVelocity = 0);
				static __Profile __stdcall SetupJerkLimitedFixedVelocityT(double velocity, double acc, double dec, double jerkAcc, double jerkDec, double startingVelocity = 0, double endVelocity = 0);
				static __Profile __stdcall SetupJerkLimitedFixedVelocityS(double velocity, double acc, double dec, double jerkAcc, double jerkDec, double startingVelocity = 0, double endVelocity = 0);
				*/
			};

			class __SyncGroupStartupType
			{
			public:
				enum T
				{
					Normal,
					CatchUp
				};
			};

			class __SyncGroup
			{
			public:
				__SyncGroup() { }
				int masterAxis;
				int slaveAxisCount;
				int slaveAxis[__constants::maxAxes - 1];
				unsigned char servoOnOffSynchronization;
				__SyncGroupStartupType::T startupType;
				unsigned int gantryLoopCycleRatio;
				double maxCatchUpDistance;
				double catchUpVelocity;
				double catchUpAcc;
				double syncErrorTolerance;
				unsigned char useMasterFeedback;
			};

			class __SyncGroupStatus
			{
			public:
				__SyncGroupStatus() { }
				unsigned char enabled;
				unsigned char homeDone;
				unsigned char catchUpError;
				unsigned char syncError;
				unsigned char axisError;
				unsigned char interruptError;
				unsigned char singleTurnMismatchError;
			};

			class __SyncCombineOptions
			{
			public:
				__SyncCombineOptions() { }
				bool useMaster1Feedback;
				bool useMaster2Feedback;
			};

			class __AxisHomeData
			{
			public:
				__AxisHomeData() { }
				double distHStoZPulse;
				double distLStoZPulse;
				double latchedZPulseEncoder;
				double latchedLimitSwitchEncoder;
				double latchedHomeSwitchEncoder;
				double latchedTouchProbeEncoder;
				double distZPulseToMasterZPulse;
				double distLSToMasterLS;
				double distHSToMasterHS;
			};

			class __HomeData
			{
			public:
				__HomeData() { }
				__AxisHomeData axesHomeData[__constants::maxAxes];
			};

			//WMX3Api
			WMX3APIFUNC __StartEngine(char* path, unsigned int waitTimeMilliseconds = 0, int core = -1, DWORD_PTR affinityMask = 0);
			WMX3APIFUNC __StartEngine(wchar_t* path, unsigned int waitTimeMilliseconds = 0, int core = -1, DWORD_PTR affinityMask = 0);
			WMX3APIFUNC __RestartEngine(char* path, unsigned int waitTimeMilliseconds = 0, int core = -1, DWORD_PTR affinityMask = 0);
			WMX3APIFUNC __RestartEngine(wchar_t* path, unsigned int waitTimeMilliseconds = 0, int core = -1, DWORD_PTR affinityMask = 0);
			WMX3APIFUNC __StopEngine(unsigned int waitTimeMilliseconds = 0);

			WMX3APIFUNC __CreateDevice(char* path, __DeviceType::T type = __DeviceType::T::DeviceTypeNormal, unsigned int waitTimeMilliseconds = 0, int core = -1, DWORD_PTR affinityMask = 0);
			WMX3APIFUNC __CreateDevice(wchar_t* path, __DeviceType::T type = __DeviceType::T::DeviceTypeNormal, unsigned int waitTimeMilliseconds = 0, int core = -1, DWORD_PTR affinityMask = 0);
			WMX3APIFUNC __CloseDevice();
			WMX3APIFUNC __GetDeviceID(int *id);


			WMX3APIFUNC __AutoQuitWithoutActiveDev(bool quit);
			WMX3APIFUNC __SetWatchdog(unsigned int watchdog);
			WMX3APIFUNC __GetWatchdog(unsigned int* watchdog, unsigned int* watchdogCount = NULL);
			WMX3APIFUNC __ResetWatchdogTimer();

			WMX3APIFUNC __SetStatistic(bool enable);
			WMX3APIFUNC __GetStatistic(__DeviceStatistics* statistics);

			WMX3APIFUNC __SetInterruptId(unsigned int interruptId);
			WMX3APIFUNC __GetInterruptId(unsigned int* interruptId);

			WMX3APIFUNC __StartCommunication(unsigned int waitTimeMilliseconds = 0);
			WMX3APIFUNC __StopCommunication(unsigned int waitTimeMilliseconds = 0);
			WMX3APIFUNC __GetModulesInfo(__ModulesInfoA* modules);
			WMX3APIFUNC __GetModulesInfo(__ModulesInfoW* modules);
			WMX3APIFUNC __GetModuleInfo(int moduleId, __ModuleInfoA* module);
			WMX3APIFUNC __GetModuleInfo(int moduleId, __ModuleInfoW* module);
			WMX3APIFUNC __GetEngineStatus(__EngineStatus* status);
			WMX3APIFUNC __GetAllDevices(__DevicesInfoA* devices);
			WMX3APIFUNC __GetAllDevices(__DevicesInfoW* devices);
			WMX3APIFUNC __SetDeviceName(char* name);
			WMX3APIFUNC __SetDeviceName(wchar_t* name);
			WMX3APIFUNC __GetDeviceName(char* nameBuf, unsigned int bufSize);
			WMX3APIFUNC __GetDeviceName(wchar_t* nameBuf, unsigned int bufSize);

			WMX3APIFUNC __SetDeviceWaitEvent(int errCode);
			WMX3APIFUNC __ResetDeviceWaitEvent();
			WMX3APIFUNC __WaitForDeviceWaitEvent(unsigned int waitTime, int* errCode);

			WMX3APIFUNC __SetModuleSuspend(__ModuleSuspend* suspend);
			WMX3APIFUNC __GetModuleSuspend(__ModuleSuspend* suspend);
			WMX3APIFUNC __AbortModuleSuspend();

			WMX3APIFUNC __SleepAtEngine(unsigned long microSecond);

			WMX3APIFUNC __RecordWindowsUpdates(unsigned int waitTimeMilliseconds = 0);
			WMX3APIFUNC __CompareWindowsUpdates(unsigned int waitTimeMilliseconds = 0);

			//Core Motion
			WMX3APIFUNC __GetStatus(__CoreMotionStatus* status);
			WMX3APIFUNC __ExecEStop(__EStopLevel::T level);
			WMX3APIFUNC __ReleaseEStop();
			WMX3APIFUNC __TriggerFlightRecorder();
			WMX3APIFUNC __ResetFlightRecorder();

			//Axis Control
			WMX3APIFUNC __SetServoOn(int axis, int newStatus);
			WMX3APIFUNC __SetServoOn(__AxisSelection* pAxisSelection, int newStatus);
			WMX3APIFUNC __ClearAmpAlarm(int axis);
			WMX3APIFUNC __ClearAmpAlarm(__AxisSelection* pAxisSelection);
			WMX3APIFUNC __ClearAxisAlarm(int axis);
			WMX3APIFUNC __ClearAxisAlarm(__AxisSelection* pAxisSelection);
			WMX3APIFUNC __SetAxisCommandMode(int axis, __AxisCommandMode::T mode);
			WMX3APIFUNC __SetAxisCommandMode(__AxisSelection* pAxisSelection, __AxisCommandMode::T mode);
			WMX3APIFUNC __GetAxisCommandMode(int axis, __AxisCommandMode::T *pMode);
			WMX3APIFUNC __GetPosCommand(int axis, double *pPosition);
			WMX3APIFUNC __GetPosFeedback(int axis, double *pPosition);
			WMX3APIFUNC __GetVelCommand(int axis, double *pVelocity);
			WMX3APIFUNC __GetVelFeedback(int axis, double *pVelocity);

			//Parameter
			WMX3APIFUNC __SetParam(__SystemParam *pParam, __SystemParam *pParamError = NULL);
			WMX3APIFUNC __GetParam(__SystemParam *pParam);
			WMX3APIFUNC __SetParam(int axis, __SystemParam *pParam, __SystemParam *pParamError = NULL);
			WMX3APIFUNC __GetParam(int axis, __SystemParam *pParam);
			WMX3APIFUNC __SetAxisParam(__AxisParam *pParam, __AxisParam *pParamError = NULL);
			WMX3APIFUNC __GetAxisParam(__AxisParam *pParam);
			WMX3APIFUNC __SetAxisParam(int axis, __AxisParam *pParam, __AxisParam *pParamError = NULL);
			WMX3APIFUNC __GetAxisParam(int axis, __AxisParam *pParam);
			WMX3APIFUNC __SetGearRatio(int axis, double numerator, double denominator);
			WMX3APIFUNC __SetSingleTurn(int axis, bool enable, unsigned int encoderCount);
			WMX3APIFUNC __SetMovingAverageProfileTime(int axis, double milliseconds);
			WMX3APIFUNC __SetAxisUnit(int axis, double unit);
			WMX3APIFUNC __SetVelocityFeedforwardGain(int axis, double gain);
			WMX3APIFUNC __SetAxisPolarity(int axis, char polarity);
			WMX3APIFUNC __SetAbsoluteEncoderMode(int axis, bool enable);
			WMX3APIFUNC __SetAbsoluteEncoderHomeOffset(int axis, double offset);
			WMX3APIFUNC __GetGearRatio(int axis, double *pNumerator, double *pDenominator);
			WMX3APIFUNC __GetSingleTurn(int axis, bool *pEnable, unsigned int *pEncoderCount);
			WMX3APIFUNC __GetMovingAverageProfileTime(int axis, double *pMilliseconds);
			WMX3APIFUNC __GetAxisUnit(int axis, double *pUnit);
			WMX3APIFUNC __GetVelocityFeedforwardGain(int axis, double *pGain);
			WMX3APIFUNC __GetAxisPolarity(int axis, char *pPolarity);
			WMX3APIFUNC __GetAbsoluteEncoderMode(int axis, bool *pEnable);
			WMX3APIFUNC __GetAbsoluteEncoderHomeOffset(int axis, double *pOffset);
			WMX3APIFUNC __SetFeedbackParam(int axis, __FeedbackParam *pParam, __FeedbackParam *pParamError = NULL);
			WMX3APIFUNC __SetHomeParam(int axis, __HomeParam *pParam, __HomeParam *pParamError = NULL);
			WMX3APIFUNC __SetLimitParam(int axis, __LimitParam *pParam, __LimitParam *pParamError = NULL);
			WMX3APIFUNC __SetMotionParam(int axis, __MotionParam *pParam, __MotionParam *pParamError = NULL);
			WMX3APIFUNC __SetAlarmParam(int axis, __AlarmParam *pParam, __AlarmParam *pParamError = NULL);
			WMX3APIFUNC __SetSyncParam(int axis, __SyncParam *pParam, __SyncParam *pParamError = NULL);
			WMX3APIFUNC __SetFlightRecorderParam(__FlightRecorderParam *pParam, __FlightRecorderParam *pParamError = NULL);
			WMX3APIFUNC __SetFlightRecorderPath(char *pPath);
			WMX3APIFUNC __SetFlightRecorderPath(wchar_t *pPath);
			WMX3APIFUNC __SetEmergencyStopParam(__EmergencyStopParam *pParam, __EmergencyStopParam *pParamError = NULL);
			WMX3APIFUNC __GetFeedbackParam(int axis, __FeedbackParam *pParam);
			WMX3APIFUNC __GetHomeParam(int axis, __HomeParam *pParam);
			WMX3APIFUNC __GetLimitParam(int axis, __LimitParam *pParam);
			WMX3APIFUNC __GetMotionParam(int axis, __MotionParam *pParam);
			WMX3APIFUNC __GetAlarmParam(int axis, __AlarmParam *pParam);
			WMX3APIFUNC __GetSyncParam(int axis, __SyncParam *pParam);
			WMX3APIFUNC __GetFlightRecorderParam(__FlightRecorderParam *pParam);
			WMX3APIFUNC __GetEmergencyStopParam(__EmergencyStopParam *pParam);
			WMX3APIFUNC __GetDefaultParam(__SystemParam *pParam);
			WMX3APIFUNC __GetDefaultAxisParam(__AxisParam *pAxisParam);
			WMX3APIFUNC __Export(char *pPath, __SystemParam *pParam);
			WMX3APIFUNC __Export(wchar_t *pPath, __SystemParam *pParam);
			WMX3APIFUNC __Export(char *pPath, __AxisParam *pAxisParam);
			WMX3APIFUNC __Export(wchar_t *pPath, __AxisParam *pAxisParam);
			WMX3APIFUNC __Export(char *pPath, __SystemParam *pParam, __AxisParam *pAxisParam);
			WMX3APIFUNC __Export(wchar_t *pPath, __SystemParam *pParam, __AxisParam *pAxisParam);
			WMX3APIFUNC __Export(char *pPath, __SystemParam *pParam, int axis);
			WMX3APIFUNC __Export(wchar_t *pPath, __SystemParam *pParam, int axis);
			WMX3APIFUNC __Export(char *pPath, __AxisParam *pAxisParam, int axis);
			WMX3APIFUNC __Export(wchar_t *pPath, __AxisParam *pAxisParam, int axis);
			WMX3APIFUNC __Export(char *pPath, __SystemParam *pParam, __AxisParam *pAxisParam, int axis);
			WMX3APIFUNC __Export(wchar_t *pPath, __SystemParam *pParam, __AxisParam *pAxisParam, int axis);
			WMX3APIFUNC __Import(char *pPath, __SystemParam *pParam);
			WMX3APIFUNC __Import(wchar_t *pPath, __SystemParam *pParam);
			WMX3APIFUNC __Import(char *pPath, __AxisParam *pAxisParam);
			WMX3APIFUNC __Import(wchar_t *pPath, __AxisParam *pAxisParam);
			WMX3APIFUNC __Import(char *pPath, __SystemParam *pParam, __AxisParam *pAxisParam);
			WMX3APIFUNC __Import(wchar_t *pPath, __SystemParam *pParam, __AxisParam *pAxisParam);
			WMX3APIFUNC __Import(char *pPath, __SystemParam *pParam, int axis, __AxisSelection *copyParamToAxes = NULL);
			WMX3APIFUNC __Import(wchar_t *pPath, __SystemParam *pParam, int axis, __AxisSelection *copyParamToAxes = NULL);
			WMX3APIFUNC __Import(char *pPath, __AxisParam *pAxisParam, int axis, __AxisSelection *copyParamToAxes = NULL);
			WMX3APIFUNC __Import(wchar_t *pPath, __AxisParam *pAxisParam, int axis, __AxisSelection *copyParamToAxes = NULL);
			WMX3APIFUNC __Import(char *pPath, __SystemParam *pParam, __AxisParam *pAxisParam, int axis, __AxisSelection *copyParamToAxes = NULL);
			WMX3APIFUNC __Import(wchar_t *pPath, __SystemParam *pParam, __AxisParam *pAxisParam, int axis, __AxisSelection *copyParamToAxes = NULL);
			WMX3APIFUNC __GetAndExportAll(char *pPath);
			WMX3APIFUNC __GetAndExportAll(wchar_t *pPath);
			WMX3APIFUNC __ImportAndSetAll(char *pPath, __SystemParam *pParamError = NULL, __AxisParam *pAxisParamError = NULL);
			WMX3APIFUNC __ImportAndSetAll(wchar_t *pPath, __SystemParam *pParamError = NULL, __AxisParam *pAxisParamError = NULL);

			//Sync
			WMX3APIFUNC __SetSyncMasterSlave(int masterAxis, int slaveAxis, __SyncOptions *pSyncOptions = NULL);
			WMX3APIFUNC __SetSyncCombine(int masterAxis1, int masterAxis2, int slaveAxis, __SyncCombineOptions *pSyncCombineOptions = NULL);
			WMX3APIFUNC __SetAbsoluteSyncPhase(int slaveAxis, double phase, __Profile *pProfile);
			WMX3APIFUNC __AddRelativeSyncPhase(int slaveAxis, double phase, __Profile *pProfile);
			WMX3APIFUNC __SetSyncGearRatio(int slaveAxis, double gearRatio, __Profile *pProfile);
			WMX3APIFUNC __SetSyncGearRatio(int masterAxis, int slaveAxis, double gearRatio, __Profile *pProfile);
			WMX3APIFUNC __SyncToJog(int slaveAxis, __Profile *pProfile);
			WMX3APIFUNC __SyncToJog(int slaveAxis);
			WMX3APIFUNC __ResolveSync(int slaveAxis);
			WMX3APIFUNC __SetSyncGroup(int groupId, __SyncGroup syncGroup);
			WMX3APIFUNC __GetSyncGroup(int groupId, __SyncGroup *pSyncGroup);
			WMX3APIFUNC __AddAxisToSyncGroup(int groupId, int axis, unsigned char isMaster);
			WMX3APIFUNC __RemoveAxisFromSyncGroup(int groupId, int axis);
			WMX3APIFUNC __EnableSyncGroup(int groupId, unsigned char enable);
			WMX3APIFUNC __GetSyncGroupStatus(int groupId, __SyncGroupStatus *pStatus);

			//Home
			WMX3APIFUNC __StartHome(int axis);
			WMX3APIFUNC __StartHome(__AxisSelection *pAxisSelection);
			WMX3APIFUNC __Continue(int axis);
			WMX3APIFUNC __Continue(__AxisSelection *pAxisSelection);
			WMX3APIFUNC __Cancel(int axis);
			WMX3APIFUNC __Cancel(__AxisSelection *pAxisSelection);
			WMX3APIFUNC __SetCommandPos(int axis, double position);
			WMX3APIFUNC __SetFeedbackPos(int axis, double position);
			WMX3APIFUNC __SetHomeDone(int axis, unsigned char value);
			WMX3APIFUNC __SetCommandPosToFeedbackPos(int axis, double posChangePerCycle = 0);
			WMX3APIFUNC __GetHomeData(__HomeData *pHomeData);

			class __VelCommand
			{
			public:
				__VelCommand() { }

				int axis;
				__Profile profile;
			};

			class __TimedVelCommand
			{
			public:
				__TimedVelCommand() { }

				int axis;
				double runTimeMilliseconds;
				__Profile profile;
			};

			class __TriggerType
			{
			public:
				enum T
				{
					RemainingTime,
					RemainingDistance,
					SameTimeCompletion,
					CompletedTime,
					CompletedDistance,
					StaggeredTimeCompletion,
					StaggeredDistanceCompletion
				};
			};

			class __Trigger
			{
			public:
				__Trigger() { }

				int triggerAxis;
				__TriggerType::T triggerType;
				double triggerValue;
			};

			class __TriggerVelCommand
			{
			public:
				__TriggerVelCommand() { }

				int axis;
				__Profile profile;
				__Trigger trigger;
			};

			class __TriggerEventInputFunction
			{
			public:
				enum T
				{
					IOBit,
					NotIOBit,
					OrIOBit,
					AndIOBit,
					XorIOBit,
					NandIOBit,
					NorIOBit,
					XnorIOBit,

					MBit,
					NotMBit,
					OrMBit,
					AndMBit,
					XorMBit,
					NandMBit,
					NorMBit,
					XnorMBit,

					Reg,
					NotReg,
					OrReg,
					AndReg,
					XorReg,
					NandReg,
					NorReg,
					XnorReg,

					EqualPos,
					GreaterPos,
					LessPos,
					EqualVelocity,
					GreaterVelocity,
					LessVelocity,
					EqualTrq,
					GreaterTrq,
					LessTrq,

					RemainingTime,
					RemainingDistance,
					SameTimeCompletion,
					CompletedTime,
					CompletedDistance,
					StaggeredTimeCompletion,
					StaggeredDistanceCompletion
				};
			};

			class __IOSourceType
			{
			public:
				enum T
				{
					Input,
					Output
				};
			};

			class __TriggerEventOutputFunction
			{
			public:
				enum T
				{
					TriggerMotion,
					SetReg
				};
			};

			class __TriggerEvent
			{
			public:
				__TriggerEvent() { }
				__TriggerEventInputFunction::T inputFunction;

				union __TriggerEventInputFunctionArguments
				{
					__TriggerEventInputFunctionArguments() { }

					struct IOBit
					{
						unsigned int byteAddress;
						unsigned char bitAddress;
						unsigned char invert;
						__IOSourceType::T ioSourceType;
					}ioBit;
					struct NotIOBit
					{
						unsigned int byteAddress;
						unsigned char bitAddress;
						unsigned char invert;
						__IOSourceType::T ioSourceType;
					}notIOBit;
					struct OrIOBit
					{
						unsigned int byteAddress[2];
						unsigned char bitAddress[2];
						unsigned char invert[2];
						__IOSourceType::T ioSourceType[2];
					}orIOBit;
					struct AndIOBit
					{
						unsigned int byteAddress[2];
						unsigned char bitAddress[2];
						unsigned char invert[2];
						__IOSourceType::T ioSourceType[2];
					}andIOBit;
					struct XorIOBit
					{
						unsigned int byteAddress[2];
						unsigned char bitAddress[2];
						unsigned char invert[2];
						__IOSourceType::T ioSourceType[2];
					}xorIOBit;
					struct NandIOBit
					{
						unsigned int byteAddress[2];
						unsigned char bitAddress[2];
						unsigned char invert[2];
						__IOSourceType::T ioSourceType[2];
					}nandIOBit;
					struct NorIOBit
					{
						unsigned int byteAddress[2];
						unsigned char bitAddress[2];
						unsigned char invert[2];
						__IOSourceType::T ioSourceType[2];
					}norIOBit;
					struct XnorIOBit
					{
						unsigned int byteAddress[2];
						unsigned char bitAddress[2];
						unsigned char invert[2];
						__IOSourceType::T ioSourceType[2];
					}xnorIOBit;

					struct MBit
					{
						unsigned int byteAddress;
						unsigned char bitAddress;
						unsigned char invert;
					}mBit;
					struct NotMBit
					{
						unsigned int byteAddress;
						unsigned char bitAddress;
						unsigned char invert;
					}notMBit;
					struct OrMBit
					{
						unsigned int byteAddress[2];
						unsigned char bitAddress[2];
						unsigned char invert[2];
					}orMBit;
					struct AndMBit
					{
						unsigned int byteAddress[2];
						unsigned char bitAddress[2];
						unsigned char invert[2];
					}andMBit;
					struct XorMBit
					{
						unsigned int byteAddress[2];
						unsigned char bitAddress[2];
						unsigned char invert[2];
					}xorMBit;
					struct NandMBit
					{
						unsigned int byteAddress[2];
						unsigned char bitAddress[2];
						unsigned char invert[2];
					}nandMBit;
					struct NorMBit
					{
						unsigned int byteAddress[2];
						unsigned char bitAddress[2];
						unsigned char invert[2];
					}norMBit;
					struct XnorMBit
					{
						unsigned int byteAddress[2];
						unsigned char bitAddress[2];
						unsigned char invert[2];
					}xnorMBit;

					struct Reg
					{
						unsigned int regNumber;
						unsigned char invert;
					}reg;
					struct NotReg
					{
						unsigned int regNumber;
						unsigned char invert;
					}notReg;
					struct OrReg
					{
						unsigned int regNumber[2];
						unsigned char invert[2];
					}orReg;
					struct AndReg
					{
						unsigned int regNumber[2];
						unsigned char invert[2];
					}andReg;
					struct XorReg
					{
						unsigned int regNumber[2];
						unsigned char invert[2];
					}xorReg;
					struct NandReg
					{
						unsigned int regNumber[2];
						unsigned char invert[2];
					}nandReg;
					struct NorReg
					{
						unsigned int regNumber[2];
						unsigned char invert[2];
					}norReg;
					struct XnorReg
					{
						unsigned int regNumber[2];
						unsigned char invert[2];
					}xnorReg;

					struct EqualPos
					{
						unsigned int axis;
						double pos;
						unsigned char disablePositiveDirection;
						unsigned char disableNegativeDirection;
						unsigned char useFeedback;
						unsigned char invert;
						unsigned char enableTolerance;
						double tolerance;
					}equalPos;
					struct GreaterPos
					{
						unsigned int axis;
						double pos;
						unsigned char disablePositiveDirection;
						unsigned char disableNegativeDirection;
						unsigned char useFeedback;
						unsigned char invert;
					}greaterPos;
					struct LessPos
					{
						unsigned int axis;
						double pos;
						unsigned char disablePositiveDirection;
						unsigned char disableNegativeDirection;
						unsigned char useFeedback;
						unsigned char invert;
					}lessPos;
					struct EqualVelocity
					{
						unsigned int axis;
						double velocity;
						unsigned char useFeedback;
						unsigned char invert;
						unsigned char enableTolerance;
						double tolerance;
						unsigned char enableUnsigned;
					}equalVelocity;
					struct GreaterVelocity
					{
						unsigned int axis;
						double velocity;
						unsigned char useFeedback;
						unsigned char invert;
						unsigned char enableUnsigned;
					}greaterVelocity;
					struct LessVelocity
					{
						unsigned int axis;
						double velocity;
						unsigned char useFeedback;
						unsigned char invert;
						unsigned char enableUnsigned;
					}lessVelocity;
					struct EqualTrq
					{
						unsigned int axis;
						double trq;
						unsigned char invert;
						unsigned char enableTolerance;
						double tolerance;
						unsigned char enableUnsigned;
					}equalTrq;
					struct GreaterTrq
					{
						unsigned int axis;
						double trq;
						unsigned char invert;
						unsigned char enableUnsigned;
					}greaterTrq;
					struct LessTrq
					{
						unsigned int axis;
						double trq;
						unsigned char invert;
						unsigned char enableUnsigned;
					}lessTrq;

					struct RemainingTime
					{
						unsigned int axis;
						unsigned char invert;
						double timeMilliseconds;
					}remainingTime;
					struct RemainingDistance
					{
						unsigned int axis;
						unsigned char invert;
						double distance;
					}remainingDistance;
					struct SameTimeCompletion
					{
						unsigned int axis;
					}sameTimeCompletion;
					struct CompletedTime
					{
						unsigned int axis;
						unsigned char invert;
						double timeMilliseconds;
					}completedTime;
					struct CompletedDistance
					{
						unsigned int axis;
						unsigned char invert;
						double distance;
					}completedDistance;
					struct StaggeredTimeCompletion
					{
						unsigned int axis;
						double timeMilliseconds;
					}staggeredTimeCompletion;
					struct StaggeredDistanceCompletion
					{
						unsigned int axis;
						double distance;
					}staggeredDistanceCompletion;

				}input;

				__TriggerEventOutputFunction::T outputFunction;
				union __TriggerEventOutputFunctionArguments
				{
					__TriggerEventOutputFunctionArguments() { }

					struct TriggerMotion
					{
						unsigned int reserved;
					}triggerMotion;
					struct SetReg
					{
						unsigned int regNumber;

					}setReg;
				}output;

			};

			class __TriggerEvents
			{
			public:
				__TriggerEvents() { }
				unsigned int numEvents;
				__TriggerEvent event[__constants::maxTriggerEvents];
			};

			class __TrqCommand
			{
			public:
				__TrqCommand() { }

				int axis;
				double torque;
			};

			class __TriggerTrqCommand
			{
			public:
				__TriggerTrqCommand() { }

				int axis;
				double torque;
				__Trigger trigger;
			};

			class __PosCommand
			{
			public:
				__PosCommand() { }

				int axis;
				double target;
				__Profile profile;
			};

			class __TriggerPosCommand
			{
			public:
				__TriggerPosCommand() { }

				int axis;
				double target;
				__Profile profile;
				__Trigger trigger;
			};

			class __LinearIntplCommand
			{
			public:
				__LinearIntplCommand() { }

				unsigned int axisCount;
				int axis[__constants::maxAxes];
				double target[__constants::maxAxes];
				double maxVelocity[__constants::maxAxes];
				double maxAcc[__constants::maxAxes];
				double maxDec[__constants::maxAxes];
				double maxJerkAcc[__constants::maxAxes];
				double maxJerkDec[__constants::maxAxes];
				__Profile profile;
			};

			class __CenterAndLengthCircularIntplCommand
			{
			public:
				__CenterAndLengthCircularIntplCommand() { }
				int axis[2];
				double centerPos[2];
				double arcLengthDegree;
				unsigned char clockwise;
				__Profile profile;
			};

			class __CenterAndEndCircularIntplCommand
			{
			public:
				__CenterAndEndCircularIntplCommand() { }
				int axis[2];
				double centerPos[2];
				double endPos[2];
				unsigned char clockwise;
				__Profile profile;
			};

			class __ThroughAndEndCircularIntplCommand
			{
			public:
				__ThroughAndEndCircularIntplCommand() { }
				int axis[2];
				double throughPos[2];
				double endPos[2];
				__Profile profile;
			};

			class __LengthAndEndCircularIntplCommand
			{
			public:
				__LengthAndEndCircularIntplCommand() { }
				int axis[2];
				double arcLengthDegree;
				unsigned char clockwise;
				double endPos[2];
				__Profile profile;
			};

			class __RadiusAndEndCircularIntplCommand
			{
			public:
				__RadiusAndEndCircularIntplCommand() { }
				int axis[2];
				double radius;
				unsigned char clockwise;
				double endPos[2];
				__Profile profile;
			};

			class __ThroughAndEnd3DCircularIntplCommand
			{
			public:
				__ThroughAndEnd3DCircularIntplCommand() { }
				int axis[3];
				double throughPos[3];
				double endPos[3];
				__Profile profile;
			};

			class __HelicalIntplProfileType
			{
			public:
				enum T
				{
					Helical,
					Circular,
					Linear
				};
			};

			class __HelicalIntplCommand
			{
			public:
				__HelicalIntplCommand() { }
				int axis[2];
				int zAxis;
				double centerPos[2];
				double zEndPos;
				double arcLengthDegree;
				unsigned char clockwise;
				__HelicalIntplProfileType::T helicalProfileType;
				__Profile profile;
			};

			class __JogCommand
			{
			public:
				__JogCommand() { }

				int axis;
				__Profile profile;
			};

			class __TimedJogCommand
			{
			public:
				__TimedJogCommand() { }

				int axis;
				double runTimeMilliseconds;
				__Profile profile;
			};

			class __TriggerJogCommand
			{
			public:
				__TriggerJogCommand() { }

				int axis;
				__Profile profile;
				__Trigger trigger;
			};

			class __PosToJogCommand
			{
			public:
				__PosToJogCommand() { }

				int axis;
				double target;
				unsigned char unsignedVelocity;
				__Profile posProfile;
				__Profile jogProfile;
				__Trigger trigger;
			};

			class __StopCommand
			{
			public:
				__StopCommand() { }

				int axis;
				double dec;
			};

			class __TimeCommand
			{
			public:
				__TimeCommand() { }

				int axis;
				double timeMilliseconds;
			};

			class __WaitConditionType
			{
			public:
				enum T
				{
					MotionFinished,
					MotionStarted,
					Event,
					MotionStartedOverrideReady
				};
			};

			class __WaitCondition
			{
			public:
				__WaitCondition() { }

				__WaitConditionType::T waitConditionType;
				int axisCount;
				int axis[__constants::maxAxes];
				unsigned int eventID;
			};

			class __SimulatePosCommand
			{
			public:
				__SimulatePosCommand() { }

				__PosCommand posCommand;
				unsigned char setStartPos;
				double startPos;
			};

			class __SimulateLinearIntplCommand
			{
			public:
				__SimulateLinearIntplCommand() { }

				__LinearIntplCommand linearIntplCommand;
				unsigned char setStartPos[__constants::maxAxes];
				double startPos[__constants::maxAxes];
			};

						//Velocity
			WMX3APIFUNC __StartVel(__VelCommand *pVelCommand);
			WMX3APIFUNC __StartVel(__TimedVelCommand *pTimedVelCommand);
			WMX3APIFUNC __StartVel(__VelCommand *pVelCommand, double maxTrqLimit);

			WMX3APIFUNC __StartVel(unsigned int numCommands, __VelCommand *pVelCommand);
			WMX3APIFUNC __StartVel(unsigned int numCommands, __TimedVelCommand *pTimedVelCommand);

			WMX3APIFUNC __StopVel(int axis);
			WMX3APIFUNC __StopVel(__AxisSelection *pAxisSelection);
			WMX3APIFUNC __ExecQuickStopVel(int axis);
			WMX3APIFUNC __ExecQuickStopVel(__AxisSelection *pAxisSelection);

			WMX3APIFUNC __SetMaxMotorSpeed(int axis, double speed);
			WMX3APIFUNC __GetMaxMotorSpeed(int axis, double *pSpeed);

			WMX3APIFUNC __OverrideVel(__VelCommand *pVelCommand);
			WMX3APIFUNC __OverrideVel(unsigned int numCommands, __VelCommand *pVelCommand);

			WMX3APIFUNC __StartPosToVel(__TriggerVelCommand *pTriggerVelCommand);
			WMX3APIFUNC __StartPosToVel(unsigned int numCommands, __TriggerVelCommand *pTriggerVelCommand);
			WMX3APIFUNC __StartPosToVel(__VelCommand *pVelCommand, __TriggerEvents *pTriggerEvents);
			WMX3APIFUNC __StartPosToVel(unsigned int numCommands, __VelCommand *pVelCommand, __TriggerEvents *pTriggerEvents);

			//Torque Control
			WMX3APIFUNC __SetMaxTrqLimit(int axis, double torque);
			WMX3APIFUNC __GetMaxTrqLimit(int axis, double *pTorque);
			WMX3APIFUNC __SetPositiveTrqLimit(int axis, double torque);
			WMX3APIFUNC __GetPositiveTrqLimit(int axis, double *pTorque);
			WMX3APIFUNC __SetNegativeTrqLimit(int axis, double torque);
			WMX3APIFUNC __GetNegativeTrqLimit(int axis, double *pTorque);

			WMX3APIFUNC __StartTrq(__TrqCommand *pTrqCommand);
			WMX3APIFUNC __StartTrq(unsigned int numCommands, __TrqCommand *pTrqCommand);
			WMX3APIFUNC __StartTrq(__TrqCommand *pTrqCommand, double maxMotorSpeed);
			WMX3APIFUNC __StartRampTimeTrq(__TrqCommand *pTrqCommand, unsigned int rampCycleTime);
			WMX3APIFUNC __StartRampTimeTrq(__TrqCommand *pTrqCommand, unsigned int rampCycleTime, double maxMotorSpeed);
			WMX3APIFUNC __StartRampRateTrq(__TrqCommand *pTrqCommand, double rampRate);
			WMX3APIFUNC __StartRampRateTrq(__TrqCommand *pTrqCommand, double rampRate, double maxMotorSpeed);

			WMX3APIFUNC __StopTrq(int axis);
			WMX3APIFUNC __StopTrq(__AxisSelection *pAxisSelection);

			WMX3APIFUNC __StartPosToTrq(__TriggerTrqCommand *pTriggerTrqCommand);
			WMX3APIFUNC __StartPosToTrq(unsigned int numCommands, __TriggerTrqCommand *pTriggerTrqCommand);
			WMX3APIFUNC __StartPosToTrq(__TrqCommand *pTrqCommand, __TriggerEvents *pTriggerEvents);
			WMX3APIFUNC __StartPosToTrq(unsigned int numCommands, __TrqCommand *pTrqCommand, __TriggerEvents *pTriggerEvents);

			WMX3APIFUNC __StartVelToTrq(__TriggerTrqCommand *pTriggerTrqCommand);
			WMX3APIFUNC __StartVelToTrq(unsigned int numCommands, __TriggerTrqCommand *pTriggerTrqCommand);
			WMX3APIFUNC __StartVelToTrq(__TrqCommand *pTrqCommand, __TriggerEvents *pTriggerEvents);
			WMX3APIFUNC __StartVelToTrq(unsigned int numCommands, __TrqCommand *pTrqCommand, __TriggerEvents *pTriggerEvents);

			//Motion
			WMX3APIFUNC __StartPos(__PosCommand *pPosCommand);
			WMX3APIFUNC __StartMov(__PosCommand *pPosCommand);
			WMX3APIFUNC __StartPos(__TriggerPosCommand *pPosCommand);
			WMX3APIFUNC __StartMov(__TriggerPosCommand *pPosCommand);
			WMX3APIFUNC __StartPos(__PosCommand *pPosCommand, __TriggerEvents *pTriggerEvents);
			WMX3APIFUNC __StartMov(__PosCommand *pPosCommand, __TriggerEvents *pTriggerEvents);

			WMX3APIFUNC __StartPos(unsigned int numCommands, __PosCommand *pPosCommand);
			WMX3APIFUNC __StartMov(unsigned int numCommands, __PosCommand *pPosCommand);
			WMX3APIFUNC __StartPos(unsigned int numCommands, __TriggerPosCommand *pPosCommand);
			WMX3APIFUNC __StartMov(unsigned int numCommands, __TriggerPosCommand *pPosCommand);
			WMX3APIFUNC __StartPos(unsigned int numCommands, __PosCommand *pPosCommand, __TriggerEvents *pTriggerEvents);
			WMX3APIFUNC __StartMov(unsigned int numCommands, __PosCommand *pPosCommand, __TriggerEvents *pTriggerEvents);

			WMX3APIFUNC __StartLinearIntplPos(__LinearIntplCommand *pLinearIntplCommand);
			WMX3APIFUNC __StartLinearIntplMov(__LinearIntplCommand *pLinearIntplCommand);
			WMX3APIFUNC __StartLinearIntplPos(__LinearIntplCommand *pLinearIntplCommand, __Trigger *pTrigger);
			WMX3APIFUNC __StartLinearIntplMov(__LinearIntplCommand *pLinearIntplCommand, __Trigger *pTrigger);
			WMX3APIFUNC __StartLinearIntplPos(__LinearIntplCommand *pLinearIntplCommand, __TriggerEvents *pTriggerEvents);
			WMX3APIFUNC __StartLinearIntplMov(__LinearIntplCommand *pLinearIntplCommand, __TriggerEvents *pTriggerEvents);

			WMX3APIFUNC __StartCircularIntplPos(__CenterAndLengthCircularIntplCommand *pCircularIntplCommand);
			WMX3APIFUNC __StartCircularIntplMov(__CenterAndLengthCircularIntplCommand *pCircularIntplCommand);
			WMX3APIFUNC __StartCircularIntplPos(__CenterAndEndCircularIntplCommand *pCircularIntplCommand);
			WMX3APIFUNC __StartCircularIntplMov(__CenterAndEndCircularIntplCommand *pCircularIntplCommand);
			WMX3APIFUNC __StartCircularIntplPos(__ThroughAndEndCircularIntplCommand *pCircularIntplCommand);
			WMX3APIFUNC __StartCircularIntplMov(__ThroughAndEndCircularIntplCommand *pCircularIntplCommand);
			WMX3APIFUNC __StartCircularIntplPos(__LengthAndEndCircularIntplCommand *pCircularIntplCommand);
			WMX3APIFUNC __StartCircularIntplMov(__LengthAndEndCircularIntplCommand *pCircularIntplCommand);
			WMX3APIFUNC __StartCircularIntplPos(__RadiusAndEndCircularIntplCommand *pCircularIntplCommand);
			WMX3APIFUNC __StartCircularIntplMov(__RadiusAndEndCircularIntplCommand *pCircularIntplCommand);
			WMX3APIFUNC __StartCircularIntplPos(__ThroughAndEnd3DCircularIntplCommand *pCircularIntplCommand);
			WMX3APIFUNC __StartCircularIntplMov(__ThroughAndEnd3DCircularIntplCommand *pCircularIntplCommand);

			WMX3APIFUNC __StartCircularIntplPos(__CenterAndLengthCircularIntplCommand *pCircularIntplCommand, __Trigger *pTrigger);
			WMX3APIFUNC __StartCircularIntplMov(__CenterAndLengthCircularIntplCommand *pCircularIntplCommand, __Trigger *pTrigger);
			WMX3APIFUNC __StartCircularIntplPos(__CenterAndEndCircularIntplCommand *pCircularIntplCommand, __Trigger *pTrigger);
			WMX3APIFUNC __StartCircularIntplMov(__CenterAndEndCircularIntplCommand *pCircularIntplCommand, __Trigger *pTrigger);
			WMX3APIFUNC __StartCircularIntplPos(__ThroughAndEndCircularIntplCommand *pCircularIntplCommand, __Trigger *pTrigger);
			WMX3APIFUNC __StartCircularIntplMov(__ThroughAndEndCircularIntplCommand *pCircularIntplCommand, __Trigger *pTrigger);
			WMX3APIFUNC __StartCircularIntplPos(__LengthAndEndCircularIntplCommand *pCircularIntplCommand, __Trigger *pTrigger);
			WMX3APIFUNC __StartCircularIntplMov(__LengthAndEndCircularIntplCommand *pCircularIntplCommand, __Trigger *pTrigger);
			WMX3APIFUNC __StartCircularIntplPos(__RadiusAndEndCircularIntplCommand *pCircularIntplCommand, __Trigger *pTrigger);
			WMX3APIFUNC __StartCircularIntplMov(__RadiusAndEndCircularIntplCommand *pCircularIntplCommand, __Trigger *pTrigger);
			WMX3APIFUNC __StartCircularIntplPos(__ThroughAndEnd3DCircularIntplCommand *pCircularIntplCommand, __Trigger *pTrigger);
			WMX3APIFUNC __StartCircularIntplMov(__ThroughAndEnd3DCircularIntplCommand *pCircularIntplCommand, __Trigger *pTrigger);

			WMX3APIFUNC __StartCircularIntplPos(__CenterAndLengthCircularIntplCommand *pCircularIntplCommand, __TriggerEvents *pTriggerEvents);
			WMX3APIFUNC __StartCircularIntplMov(__CenterAndLengthCircularIntplCommand *pCircularIntplCommand, __TriggerEvents *pTriggerEvents);
			WMX3APIFUNC __StartCircularIntplPos(__CenterAndEndCircularIntplCommand *pCircularIntplCommand, __TriggerEvents *pTriggerEvents);
			WMX3APIFUNC __StartCircularIntplMov(__CenterAndEndCircularIntplCommand *pCircularIntplCommand, __TriggerEvents *pTriggerEvents);
			WMX3APIFUNC __StartCircularIntplPos(__ThroughAndEndCircularIntplCommand *pCircularIntplCommand, __TriggerEvents *pTriggerEvents);
			WMX3APIFUNC __StartCircularIntplMov(__ThroughAndEndCircularIntplCommand *pCircularIntplCommand, __TriggerEvents *pTriggerEvents);
			WMX3APIFUNC __StartCircularIntplPos(__LengthAndEndCircularIntplCommand *pCircularIntplCommand, __TriggerEvents *pTriggerEvents);
			WMX3APIFUNC __StartCircularIntplMov(__LengthAndEndCircularIntplCommand *pCircularIntplCommand, __TriggerEvents *pTriggerEvents);
			WMX3APIFUNC __StartCircularIntplPos(__RadiusAndEndCircularIntplCommand *pCircularIntplCommand, __TriggerEvents *pTriggerEvents);
			WMX3APIFUNC __StartCircularIntplMov(__RadiusAndEndCircularIntplCommand *pCircularIntplCommand, __TriggerEvents *pTriggerEvents);
			WMX3APIFUNC __StartCircularIntplPos(__ThroughAndEnd3DCircularIntplCommand *pCircularIntplCommand, __TriggerEvents *pTriggerEvents);
			WMX3APIFUNC __StartCircularIntplMov(__ThroughAndEnd3DCircularIntplCommand *pCircularIntplCommand, __TriggerEvents *pTriggerEvents);

			WMX3APIFUNC __StartHelicalIntplPos(__HelicalIntplCommand *pHelicalIntplCommand);
			WMX3APIFUNC __StartHelicalIntplMov(__HelicalIntplCommand *pHelicalIntplCommand);
			WMX3APIFUNC __StartHelicalIntplPos(__HelicalIntplCommand *pHelicalIntplCommand, __Trigger *pTrigger);
			WMX3APIFUNC __StartHelicalIntplMov(__HelicalIntplCommand *pHelicalIntplCommand, __Trigger *pTrigger);
			WMX3APIFUNC __StartHelicalIntplPos(__HelicalIntplCommand *pHelicalIntplCommand, __TriggerEvents *pTriggerEvents);
			WMX3APIFUNC __StartHelicalIntplMov(__HelicalIntplCommand *pHelicalIntplCommand, __TriggerEvents *pTriggerEvents);

			WMX3APIFUNC __StartJog(__JogCommand *pJogCommand);
			WMX3APIFUNC __StartJog(__TimedJogCommand *pTimedJogCommand);

			WMX3APIFUNC __StartJog(unsigned int numCommands, __JogCommand *pJogCommand);
			WMX3APIFUNC __StartJog(unsigned int numCommands, __TimedJogCommand *pTimedJogCommand);

			WMX3APIFUNC __StartPosToJog(__TriggerJogCommand *pTriggerJogCommand);
			WMX3APIFUNC __StartPosToJog(unsigned int numCommands, __TriggerJogCommand *pTriggerJogCommand);
			WMX3APIFUNC __StartPosToJog(__JogCommand *pJogCommand, __TriggerEvents *pTriggerEvents);
			WMX3APIFUNC __StartPosToJog(unsigned int numCommands, __JogCommand *pJogCommand, __TriggerEvents *pTriggerEvents);
			WMX3APIFUNC __StartPosToJog(__PosToJogCommand *pPosToJogCommand);
			WMX3APIFUNC __StartPosToJog(unsigned int numCommands, __PosToJogCommand *pPosToJogCommand);
			WMX3APIFUNC __StartMovToJog(__PosToJogCommand *pPosToJogCommand);
			WMX3APIFUNC __StartMovToJog(unsigned int numCommands, __PosToJogCommand *pPosToJogCommand);

			WMX3APIFUNC __Stop(int axis);
			WMX3APIFUNC __Stop(__AxisSelection *pAxisSelection);
			WMX3APIFUNC __Stop(int axis, double dec);
			WMX3APIFUNC __Stop(unsigned int numCommands, __StopCommand *pStopCommand);
			WMX3APIFUNC __ExecQuickStop(int axis);
			WMX3APIFUNC __ExecQuickStop(__AxisSelection *pAxisSelection);
			WMX3APIFUNC __ExecTimedStop(int axis, double timeMilliseconds);
			WMX3APIFUNC __ExecTimedStop(__AxisSelection *pAxisSelection, double timeMilliseconds);
			WMX3APIFUNC __ExecTimedStop(unsigned int numCommands, __TimeCommand *pTimeCommand);
			WMX3APIFUNC __Wait(int axis);
			WMX3APIFUNC __Wait(__AxisSelection *pAxisSelection);
			WMX3APIFUNC __Wait(__WaitCondition *pWaitCondition);
			WMX3APIFUNC __Wait(int axis, unsigned int waitTimeMilliseconds);
			WMX3APIFUNC __Wait(__AxisSelection *pAxisSelection, unsigned int waitTimeMilliseconds);
			WMX3APIFUNC __Wait(__WaitCondition *pWaitCondition, unsigned int waitTimeMilliseconds);
			WMX3APIFUNC __Pause(int axis);
			WMX3APIFUNC __Pause(__AxisSelection *pAxisSelection);
			WMX3APIFUNC __Resume(int axis);
			WMX3APIFUNC __Resume(__AxisSelection *pAxisSelection);

			WMX3APIFUNC __OverridePos(__PosCommand *pPosCommand);
			WMX3APIFUNC __OverrideMov(__PosCommand *pPosCommand);
			WMX3APIFUNC __OverrideVel(__PosCommand *pPosCommand);
			WMX3APIFUNC __OverrideAcc(__PosCommand *pPosCommand);
			WMX3APIFUNC __OverrideDec(__PosCommand *pPosCommand);
			WMX3APIFUNC __OverrideJerkAcc(__PosCommand *pPosCommand);
			WMX3APIFUNC __OverrideJerkDec(__PosCommand *pPosCommand);
			WMX3APIFUNC __OverrideProfile(__PosCommand *pPosCommand);

			WMX3APIFUNC __OverridePos(unsigned int numCommands, __PosCommand *pPosCommand);
			WMX3APIFUNC __OverrideMov(unsigned int numCommands, __PosCommand *pPosCommand);
			WMX3APIFUNC __OverrideVel(unsigned int numCommands, __PosCommand *pPosCommand);
			WMX3APIFUNC __OverrideAcc(unsigned int numCommands, __PosCommand *pPosCommand);
			WMX3APIFUNC __OverrideDec(unsigned int numCommands, __PosCommand *pPosCommand);
			WMX3APIFUNC __OverrideJerkAcc(unsigned int numCommands, __PosCommand *pPosCommand);
			WMX3APIFUNC __OverrideJerkDec(unsigned int numCommands, __PosCommand *pPosCommand);
			WMX3APIFUNC __OverrideProfile(unsigned int numCommands, __PosCommand *pPosCommand);

			WMX3APIFUNC __StopJogAtPos(__PosCommand *pPosCommand);
			WMX3APIFUNC __StopJogAtPos(unsigned int numCommands, __PosCommand *pPosCommand);

			WMX3APIFUNC __SuperimposeMov(__PosCommand *pPosCommand);
			WMX3APIFUNC __SuperimposeMov(unsigned int numCommands, __PosCommand *pPosCommand);
			WMX3APIFUNC __StopSuperimpose(int axis);
			WMX3APIFUNC __StopSuperimpose(__AxisSelection *pAxisSelection);

			WMX3APIFUNC __SimulatePos(__SimulatePosCommand *pSimulatePosCommand, double *pPeakVelocity, double *pTotalTimeMilliseconds, double *pAccelerationTimeMilliseconds, double *pCruiseTimeMilliseconds, double *pDecelerationTimeMilliseconds);
			WMX3APIFUNC __SimulateLinearIntplPos(__SimulateLinearIntplCommand *pSimulateLinearIntplCommand, double *pPeakVelocity, double *pTotalTimeMilliseconds, double *pAccelerationTimeMilliseconds, double *pCruiseTimeMilliseconds, double *pDecelerationTimeMilliseconds);
			WMX3APIFUNC __SimulatePosAtTime(__SimulatePosCommand *pSimulatePosCommand, int returnType, double timeMilliseconds, double *pCalculatedPos, double *pCalculatedVel);
			WMX3APIFUNC __SimulatePosAtTime(__SimulateLinearIntplCommand *pSimulateLinearIntplCommand, double timeMilliseconds, double *pPosArray, double *pMoveDistance, double *pRemainDistance, double *pTotalDistance);
			WMX3APIFUNC __SimulateTimeAtPos(__SimulatePosCommand *pSimulatePosCommand, double specificPos, double *pMoveTimeMilliseconds, double *pRemainTimeMilliseconds);
			WMX3APIFUNC __SimulateTimeAtDist(__SimulateLinearIntplCommand *pSimulateLinearIntplCommand, double specificDistance, double *pMoveTimeMilliseconds, double *pRemainTimeMilliseconds, double *pTotalTimeMilliseconds);

			//ECApi

			class __EcStateMachine
			{
			public:
				enum T
				{
					None = 0x00,
					Init = 0x01,
					Preop = 0x02,
					Boot = 0x03,
					Safeop = 0x04,
					Op = 0x08
				};
			};

			class __EcSlavePdo
			{
			public:
				bool enable;
				unsigned char size;
				unsigned short offset;
			};

			class __EcSlavePdoInfo
			{
			public:
				__EcSlavePdo statusWord;
				__EcSlavePdo modesOfOperationDisplay;
				__EcSlavePdo positionActualValue;
				__EcSlavePdo velocityActualValue;
				__EcSlavePdo torqueActualValue;
				__EcSlavePdo errorCode;
				__EcSlavePdo touchProbeStatus;
				__EcSlavePdo touchProbePositionPos1;
				__EcSlavePdo touchProbePositionPos2;
				__EcSlavePdo followingError;
				__EcSlavePdo digitalInput;
				
				__EcSlavePdo controlWord;
				__EcSlavePdo modesOfOperation;
				__EcSlavePdo targetPosition;
				__EcSlavePdo targetVelocity;
				__EcSlavePdo targetTorque;
				__EcSlavePdo maxTorque;
				__EcSlavePdo positiveTorqueLimit;
				__EcSlavePdo negativeTorqueLimit;
				__EcSlavePdo touchProbeFunction;
				__EcSlavePdo velocityOffset;
				__EcSlavePdo torqueOffset;
				__EcSlavePdo maxMotorSpeed;
				__EcSlavePdo digitalOutput;
			};

			class __EcOperationMode
			{
			public:
				enum T
				{
					NONE = 0x00,
					PP = 0x01,
					VL = 0x02,
					PV = 0x03,
					TQ = 0x04,
					HM = 0x06,
					IP = 0x07,
					CSP = 0x08,
					CSV = 0x09,
					CST = 0x0a,
					CSTCA = 0x0b,

					Unknown
				};
			};

			class __EcSlaveAxisInfo
			{
			public:
				int axisIndex;
				short digitalInputAddr;
				unsigned short digitalInputSize;
				short digitalOutputAddr;
				unsigned short digitalOutputSize;
				__EcSlavePdoInfo pdoInfo;
				__EcOperationMode::T operationMode;
				unsigned char targetReached;
				unsigned char followingError;
				unsigned char homingError;
				unsigned char homingAttained;
				unsigned char halt;
			};

			class __EcSlaveInfo
			{
			public:
				int id;

				short position;
				short address;

				unsigned int vendorId;
				unsigned int productCode;
				unsigned int revisionNo;
				unsigned int serialNo;
				unsigned int alias;
				unsigned int numOfAxes;
				__EcSlaveAxisInfo axisInfo[__constants_Ecat::maxSlaveAxes];

				__EcStateMachine::T state;
				unsigned short alStatusCode;

				unsigned char offline;
				unsigned char inaccessible;
				unsigned char newSlave;
				unsigned char reverseSlave;

				unsigned char dcSupport;
				unsigned char dcWidth;          //0: none, 1: 32bit, 2: 64bit
				unsigned char portState[4];     //bit0: physical link, bit1: Loop, bit2: Communication
				int neighbor[4];
				unsigned int propagaDelay;
				unsigned long long systemTime;
				unsigned long long systemTimeOffset;
				unsigned long long startTime;

				unsigned short inputAddr;
				unsigned short inputSize;
				unsigned short outputAddr;
				unsigned short outputSize;

				unsigned int rxLogicAddr;
				unsigned int rxPhyAddr;
				unsigned int rxSize;
				unsigned int txLogicAddr;
				unsigned int txPhyAddr;
				unsigned int txSize;
			};

			class __EcHotconnectState
			{
			public:
				enum T
				{
					Idle = 0x00,
					Running,
					Abort,

					Unknown
				};
			};

			class __EcHotconnectAbortCode
			{
			public:
				enum T
				{
					None = 0x00,
					PortOpenError,
					Timeout,
					ScanNetworkFailed,
					ApplyNewTopologyError,
					InitNewSlavesError,
					RecalculateDcError,
					ClearDLEventError,

					Unknown
				};
			};

			class __EcMasterStatisticsInfo
			{
			public:
				long commPeriod;
				long long cycleCounter;
				double interruptIntervalRatio[9];
				double cycleProcessTimeRatio[6];
				double processTimeRatio[6];
				double commandProcessTimeRatio[6];
				double feedbackProcessTimeRatio[6];
				double communicateProcessTimeRatio[6];

				unsigned int txDelay;
				unsigned int minTxDelay;
				unsigned int maxTxDelay;
				unsigned int packetLoss;
				unsigned int packetTimeout;
				unsigned int packetLossMode[4];
				unsigned int packetTimeoutMode[4];

				unsigned long long minRefClockInterval;
				unsigned long long maxRefClockInterval;
				unsigned long long avgRefClockInterval;

				unsigned long long curRefClock;
				unsigned long long nextDcClock;

				int diffFromNextDcClock;
				int minDiffFromNextDcClock;
				int maxDiffFromNextDcClock;

				unsigned int totalAxesNum;

				unsigned int totalRxPdoSize;
				unsigned int totalTxPdoSize;
				unsigned int totalInputSize;
				unsigned int totalOutputSize;

				unsigned int overCycle;

				unsigned int ringNum;

				unsigned int numOfCyclicFrames;

				__EcHotconnectState::T hcState;
				__EcHotconnectAbortCode::T hcAbortCode;
				long hcErrorCode;
			};

			class __EcMasterInfo
			{
			public:
				int id;
				__EcStateMachine::T state;
				unsigned int numOfSlaves;
				__EcSlaveInfo slaves[__constants_Ecat::maxSlaves];

				__EcMasterStatisticsInfo statisticsInfo;
			};

			ECAPIFUNC __ScanNetwork();
			ECAPIFUNC __GetMasterInfo(__EcMasterInfo* pMasterInfo);

		protected:
			virtual bool DoesModuleExist();

		protected:
			double m_dblLastVelocity = 0.;

			wmx3Api::WMX3Api* m_pLibModule = nullptr;
			wmx3Api::CoreMotion* m_pCMModule = nullptr;
		};
	}
}

