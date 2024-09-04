#pragma once

#include "DeviceMotion.h"

namespace Ravid
{
	namespace Framework
	{
		class CGoogolDioViewer : public CRavidDialogBase
		{
			DECLARE_DYNAMIC(CGoogolDioViewer)

		public:
			CGoogolDioViewer(_In_opt_ CWnd* pParent = nullptr);
			virtual ~CGoogolDioViewer();

			virtual ptrdiff_t OnMessage(_In_ Ravid::Framework::CMessageBase* pMessage);

			virtual void DoDataExchange(CDataExchange* pDX);
			virtual BOOL OnInitDialog();

			virtual BOOL PreTranslateMessage(MSG* pMsg);

			virtual bool Create(_In_opt_ CWnd* pParent = nullptr);

			virtual void SetInitialize(_In_ bool bInit);
			virtual bool IsInitialized();

			virtual void SetLabelString(_In_ int nID, _In_ CString strFormat);

			//추가기능
			virtual bool WriteGenExtOutBit(_In_ int nBit, _In_ bool bOn);
			virtual bool ReadGenExtOutBit(_In_ int nBit);
			virtual bool ReadGenExtInputBit(_In_ int nBit);

			virtual void ClearExtOutport();

			afx_msg void OnBnClickedExtOutLed(_In_ UINT nID);
			afx_msg void OnDestroy();
			virtual void OnCancel();

			virtual void OnLedStatus();

		public:
			DECLARE_MESSAGE_MAP()

		public:
			std::vector<CWnd*> m_vctCtrl;

			bool m_bIsInitialize = false;

			int m_nDeviceID = 0;
			int m_nViewID = 0;

			static int m_nTotalID;

			volatile DWORD m_dwExtOutStatus;
		};
	}
}

#define CRD_OPERATION_DATA_EXT_MAX 2
#define INTERPOLATION_AXIS_MAX 4

namespace Ravid
{
	namespace Device
	{
		class CDeviceGoogolGTS : public CDeviceMotion
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceGoogolGTS);

			DECLARE_DYNAMIC(CDeviceGoogolGTS)
			enum eMotionStatus
			{
				eMotionStatus_ServoAlarm = 0x02,
				eMotionStatus_MotionError = 0x10,
				eMotionStatus_PositiveLimit = 0x20,
				eMotionStatus_NegativeLimit = 0x40,
				eMotionStatus_SmoothStopIO = 0x80,
				eMotionStatus_AbruptStopIO = 0x100,
				eMotionStatus_MotorActivation = 0x200,
				eMotionStatus_ProfileMotion = 0x400,
				eMotionStatus_Inposition = 0x800,
			};

			typedef struct sGTS_MotionInfo
			{
				bool bServo = false;
				double dblDis = 0.;
				double dblPos = 0.;
				double dblVel = 0.;
				double dblAcl = 0.;				
				double dblLastVel = 0.;				
			}SGTS_MotionInfo;

			typedef struct __gts_sTrapPrm
			{
				double acc;
				double dec;
				double velStart;
				short  smoothTime;
			} __gts_STTrapPrm;


			typedef struct __gts_sCrdPrm
			{
				short dimension;
				short profile[8];
				double synVelMax;
				double synAccMax;
				short evenTime;
				short setOriginFlag;
				long originPos[8];
			}__gts_STCrdPrm;

			typedef struct __gts_sCrdBufOperation
			{
				short flag;
				unsigned short delay;
				short doType;
				unsigned short doMask;
				unsigned short doValue;
				unsigned short dataExt[CRD_OPERATION_DATA_EXT_MAX];
			}__gts_STCrdBufOperation;

			typedef struct __gts_sCrdData
			{
				short motionType;
				short circlePlat;
				long pos[INTERPOLATION_AXIS_MAX];
				double radius;
				short circleDir;
				double center[3];
				double vel;
				double acc;
				short velEndZero;
				__gts_STCrdBufOperation operation;
				double cos[INTERPOLATION_AXIS_MAX];
				double velEnd;
				double velEndAdjust;
				double r;
			}__gts_STCrdData;
			
			typedef struct __gts_sVarInfo
			{
				short id;
				short dataType;
				char  name[32];
			} __gts_STVarInfo;

			typedef struct
			{
				short mode;
				short moveDir;
				short indexDir;
				short edge;
				short triggerIndex;
				short pad1[3];
				double velHigh;
				double velLow;
				double acc;
				double dec;
				short smoothTime;
				short pad2[3];
				long homeOffset;
				long searchHomeDistance;
				long searchIndexDistance;
				long escapeStep;
				long pad3[2];
			} __gts_STHomePrm;

			typedef struct
			{
				short run;
				short stage;
				short error;
				short pad1;
				long capturePos;
				long targetPos;
			} __gts_STHomeStatus;

			CDeviceGoogolGTS();
			virtual ~CDeviceGoogolGTS();

			virtual ptrdiff_t OnMessage(_In_ Ravid::Framework::CMessageBase* pMessage);

			virtual EDeviceInitializeResult Initialize() override;
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;
			
			virtual BOOL PreTranslateMessage(MSG* pMsg);

		public:			
			virtual EGoogolGtsGetFunction GetInitializeType(_Out_ EGoogolGtsInitializeType *pParam);
			virtual EGoogolGtsSetFunction SetInitializeType(_In_ EGoogolGtsInitializeType eParam);

			virtual EGoogolGtsGetFunction GetLoadFilePath(_Out_ CString *pParam);
			virtual EGoogolGtsSetFunction SetLoadFilePath(_In_ CString strParam);

			virtual EGoogolGtsGetFunction GetLoadExtFilePath(_Out_ CString *pParam);
			virtual EGoogolGtsSetFunction SetLoadExtFilePath(_In_ CString strParam);

			virtual EGoogolGtsGetFunction GetShowExternalIO1(_Out_ bool *pParam);
			virtual EGoogolGtsSetFunction SetShowExternalIO1(_In_ bool bParam);

			virtual EGoogolGtsGetFunction GetShowExternalIO2(_Out_ bool *pParam);
			virtual EGoogolGtsSetFunction SetShowExternalIO2(_In_ bool bParam);

			virtual EGoogolGtsGetFunction GetShowExternalIO3(_Out_ bool *pParam);
			virtual EGoogolGtsSetFunction SetShowExternalIO3(_In_ bool bParam);

			virtual EGoogolGtsGetFunction GetShowExternalIO4(_Out_ bool *pParam);
			virtual EGoogolGtsSetFunction SetShowExternalIO4(_In_ bool bParam);

			virtual EGoogolGtsGetFunction GetShowExternalIO5(_Out_ bool *pParam);
			virtual EGoogolGtsSetFunction SetShowExternalIO5(_In_ bool bParam);

			virtual EGoogolGtsGetFunction GetShowExternalIO6(_Out_ bool *pParam);
			virtual EGoogolGtsSetFunction SetShowExternalIO6(_In_ bool bParam);

			virtual EGoogolGtsGetFunction GetShowExternalIO7(_Out_ bool *pParam);
			virtual EGoogolGtsSetFunction SetShowExternalIO7(_In_ bool bParam);

			virtual EGoogolGtsGetFunction GetShowExternalIO8(_Out_ bool *pParam);
			virtual EGoogolGtsSetFunction SetShowExternalIO8(_In_ bool bParam);

			virtual EGoogolGtsGetFunction GetShowExternalIO9(_Out_ bool *pParam);
			virtual EGoogolGtsSetFunction SetShowExternalIO9(_In_ bool bParam);

			virtual EGoogolGtsGetFunction GetShowExternalIO10(_Out_ bool *pParam);
			virtual EGoogolGtsSetFunction SetShowExternalIO10(_In_ bool bParam);

			virtual EGoogolGtsGetFunction GetShowExternalIO11(_Out_ bool *pParam);
			virtual EGoogolGtsSetFunction SetShowExternalIO11(_In_ bool bParam);

			virtual EGoogolGtsGetFunction GetShowExternalIO12(_Out_ bool *pParam);
			virtual EGoogolGtsSetFunction SetShowExternalIO12(_In_ bool bParam);

			virtual EGoogolGtsGetFunction GetShowExternalIO13(_Out_ bool *pParam);
			virtual EGoogolGtsSetFunction SetShowExternalIO13(_In_ bool bParam);

			virtual EGoogolGtsGetFunction GetShowExternalIO14(_Out_ bool *pParam);
			virtual EGoogolGtsSetFunction SetShowExternalIO14(_In_ bool bParam);

			virtual EGoogolGtsGetFunction GetShowExternalIO15(_Out_ bool *pParam);
			virtual EGoogolGtsSetFunction SetShowExternalIO15(_In_ bool bParam);

			virtual EGoogolGtsGetFunction GetShowExternalIO16(_Out_ bool *pParam);
			virtual EGoogolGtsSetFunction SetShowExternalIO16(_In_ bool bParam);

			virtual EGoogolGtsGetFunction GetAxis1Band(_Out_ long *pParam);
			virtual EGoogolGtsSetFunction SetAxis1Band(_In_ long lParam);

			virtual EGoogolGtsGetFunction GetAxis1Time(_Out_ long *pParam);
			virtual EGoogolGtsSetFunction SetAxis1Time(_In_ long lParam);

			virtual EGoogolGtsGetFunction GetAxis1DirAndPos(_Out_ long *pParam);
			virtual EGoogolGtsSetFunction SetAxis1DirAndPos(_In_ long lParam);

			virtual EGoogolGtsGetFunction GetAxis1Velocity(_Out_ double *pParam);
			virtual EGoogolGtsSetFunction SetAxis1Velocity(_In_ double dblParam);

			virtual EGoogolGtsGetFunction GetAxis1Accel(_Out_ double *pParam);
			virtual EGoogolGtsSetFunction SetAxis1Accel(_In_ double dblParam);

			virtual EGoogolGtsGetFunction GetAxis1Offset(_Out_ long *pParam);
			virtual EGoogolGtsSetFunction SetAxis1Offset(_In_ long lParam);

			virtual EGoogolGtsGetFunction GetAxis1AbruptStop(_Out_ double*pParam);
			virtual EGoogolGtsSetFunction SetAxis1AbruptStop(_In_ double dblParam);

			virtual EGoogolGtsGetFunction GetAxis1EStop(_Out_ double*pParam);
			virtual EGoogolGtsSetFunction SetAxis1EStop(_In_ double dblParam);

			virtual EGoogolGtsGetFunction GetAxis2Band(_Out_ long *pParam);
			virtual EGoogolGtsSetFunction SetAxis2Band(_In_ long lParam);

			virtual EGoogolGtsGetFunction GetAxis2Time(_Out_ long *pParam);
			virtual EGoogolGtsSetFunction SetAxis2Time(_In_ long lParam);

			virtual EGoogolGtsGetFunction GetAxis2DirAndPos(_Out_ long *pParam);
			virtual EGoogolGtsSetFunction SetAxis2DirAndPos(_In_ long lParam);

			virtual EGoogolGtsGetFunction GetAxis2Velocity(_Out_ double *pParam);
			virtual EGoogolGtsSetFunction SetAxis2Velocity(_In_ double dblParam);

			virtual EGoogolGtsGetFunction GetAxis2Accel(_Out_ double *pParam);
			virtual EGoogolGtsSetFunction SetAxis2Accel(_In_ double dblParam);

			virtual EGoogolGtsGetFunction GetAxis2Offset(_Out_ long *pParam);
			virtual EGoogolGtsSetFunction SetAxis2Offset(_In_ long lParam);

			virtual EGoogolGtsGetFunction GetAxis2AbruptStop(_Out_ double*pParam);
			virtual EGoogolGtsSetFunction SetAxis2AbruptStop(_In_ double dblParam);

			virtual EGoogolGtsGetFunction GetAxis2EStop(_Out_ double*pParam);
			virtual EGoogolGtsSetFunction SetAxis2EStop(_In_ double dblParam);

			virtual EGoogolGtsGetFunction GetAxis3Band(_Out_ long *pParam);
			virtual EGoogolGtsSetFunction SetAxis3Band(_In_ long lParam);

			virtual EGoogolGtsGetFunction GetAxis3Time(_Out_ long *pParam);
			virtual EGoogolGtsSetFunction SetAxis3Time(_In_ long lParam);

			virtual EGoogolGtsGetFunction GetAxis3DirAndPos(_Out_ long *pParam);
			virtual EGoogolGtsSetFunction SetAxis3DirAndPos(_In_ long lParam);

			virtual EGoogolGtsGetFunction GetAxis3Velocity(_Out_ double *pParam);
			virtual EGoogolGtsSetFunction SetAxis3Velocity(_In_ double dblParam);

			virtual EGoogolGtsGetFunction GetAxis3Accel(_Out_ double *pParam);
			virtual EGoogolGtsSetFunction SetAxis3Accel(_In_ double dblParam);

			virtual EGoogolGtsGetFunction GetAxis3Offset(_Out_ long *pParam);
			virtual EGoogolGtsSetFunction SetAxis3Offset(_In_ long lParam);

			virtual EGoogolGtsGetFunction GetAxis3AbruptStop(_Out_ double*pParam);
			virtual EGoogolGtsSetFunction SetAxis3AbruptStop(_In_ double dblParam);

			virtual EGoogolGtsGetFunction GetAxis3EStop(_Out_ double*pParam);
			virtual EGoogolGtsSetFunction SetAxis3EStop(_In_ double dblParam);

			virtual EGoogolGtsGetFunction GetAxis4Band(_Out_ long *pParam);
			virtual EGoogolGtsSetFunction SetAxis4Band(_In_ long lParam);

			virtual EGoogolGtsGetFunction GetAxis4Time(_Out_ long *pParam);
			virtual EGoogolGtsSetFunction SetAxis4Time(_In_ long lParam);

			virtual EGoogolGtsGetFunction GetAxis4DirAndPos(_Out_ long *pParam);
			virtual EGoogolGtsSetFunction SetAxis4DirAndPos(_In_ long lParam);

			virtual EGoogolGtsGetFunction GetAxis4Velocity(_Out_ double *pParam);
			virtual EGoogolGtsSetFunction SetAxis4Velocity(_In_ double dblParam);

			virtual EGoogolGtsGetFunction GetAxis4Accel(_Out_ double *pParam);
			virtual EGoogolGtsSetFunction SetAxis4Accel(_In_ double dblParam);

			virtual EGoogolGtsGetFunction GetAxis4Offset(_Out_ long *pParam);
			virtual EGoogolGtsSetFunction SetAxis4Offset(_In_ long lParam);

			virtual EGoogolGtsGetFunction GetAxis4AbruptStop(_Out_ double*pParam);
			virtual EGoogolGtsSetFunction SetAxis4AbruptStop(_In_ double dblParam);

			virtual EGoogolGtsGetFunction GetAxis4EStop(_Out_ double*pParam);
			virtual EGoogolGtsSetFunction SetAxis4EStop(_In_ double dblParam);

			virtual EGoogolGtsGetFunction GetAxis5Band(_Out_ long *pParam);
			virtual EGoogolGtsSetFunction SetAxis5Band(_In_ long lParam);

			virtual EGoogolGtsGetFunction GetAxis5Time(_Out_ long *pParam);
			virtual EGoogolGtsSetFunction SetAxis5Time(_In_ long lParam);

			virtual EGoogolGtsGetFunction GetAxis5DirAndPos(_Out_ long *pParam);
			virtual EGoogolGtsSetFunction SetAxis5DirAndPos(_In_ long lParam);

			virtual EGoogolGtsGetFunction GetAxis5Velocity(_Out_ double *pParam);
			virtual EGoogolGtsSetFunction SetAxis5Velocity(_In_ double dblParam);

			virtual EGoogolGtsGetFunction GetAxis5Accel(_Out_ double *pParam);
			virtual EGoogolGtsSetFunction SetAxis5Accel(_In_ double dblParam);

			virtual EGoogolGtsGetFunction GetAxis5Offset(_Out_ long *pParam);
			virtual EGoogolGtsSetFunction SetAxis5Offset(_In_ long lParam);

			virtual EGoogolGtsGetFunction GetAxis5AbruptStop(_Out_ double*pParam);
			virtual EGoogolGtsSetFunction SetAxis5AbruptStop(_In_ double dblParam);

			virtual EGoogolGtsGetFunction GetAxis5EStop(_Out_ double*pParam);
			virtual EGoogolGtsSetFunction SetAxis5EStop(_In_ double dblParam);

			virtual EGoogolGtsGetFunction GetAxis6Band(_Out_ long *pParam);
			virtual EGoogolGtsSetFunction SetAxis6Band(_In_ long lParam);

			virtual EGoogolGtsGetFunction GetAxis6Time(_Out_ long *pParam);
			virtual EGoogolGtsSetFunction SetAxis6Time(_In_ long lParam);

			virtual EGoogolGtsGetFunction GetAxis6DirAndPos(_Out_ long *pParam);
			virtual EGoogolGtsSetFunction SetAxis6DirAndPos(_In_ long lParam);

			virtual EGoogolGtsGetFunction GetAxis6Velocity(_Out_ double *pParam);
			virtual EGoogolGtsSetFunction SetAxis6Velocity(_In_ double dblParam);

			virtual EGoogolGtsGetFunction GetAxis6Accel(_Out_ double *pParam);
			virtual EGoogolGtsSetFunction SetAxis6Accel(_In_ double dblParam);

			virtual EGoogolGtsGetFunction GetAxis6Offset(_Out_ long *pParam);
			virtual EGoogolGtsSetFunction SetAxis6Offset(_In_ long lParam);

			virtual EGoogolGtsGetFunction GetAxis6AbruptStop(_Out_ double*pParam);
			virtual EGoogolGtsSetFunction SetAxis6AbruptStop(_In_ double dblParam);

			virtual EGoogolGtsGetFunction GetAxis6EStop(_Out_ double*pParam);
			virtual EGoogolGtsSetFunction SetAxis6EStop(_In_ double dblParam);

			virtual EGoogolGtsGetFunction GetAxis7Band(_Out_ long *pParam);
			virtual EGoogolGtsSetFunction SetAxis7Band(_In_ long lParam);

			virtual EGoogolGtsGetFunction GetAxis7Time(_Out_ long *pParam);
			virtual EGoogolGtsSetFunction SetAxis7Time(_In_ long lParam);

			virtual EGoogolGtsGetFunction GetAxis7DirAndPos(_Out_ long *pParam);
			virtual EGoogolGtsSetFunction SetAxis7DirAndPos(_In_ long lParam);

			virtual EGoogolGtsGetFunction GetAxis7Velocity(_Out_ double *pParam);
			virtual EGoogolGtsSetFunction SetAxis7Velocity(_In_ double dblParam);

			virtual EGoogolGtsGetFunction GetAxis7Accel(_Out_ double *pParam);
			virtual EGoogolGtsSetFunction SetAxis7Accel(_In_ double dblParam);

			virtual EGoogolGtsGetFunction GetAxis7Offset(_Out_ long *pParam);
			virtual EGoogolGtsSetFunction SetAxis7Offset(_In_ long lParam);

			virtual EGoogolGtsGetFunction GetAxis7AbruptStop(_Out_ double*pParam);
			virtual EGoogolGtsSetFunction SetAxis7AbruptStop(_In_ double dblParam);

			virtual EGoogolGtsGetFunction GetAxis7EStop(_Out_ double*pParam);
			virtual EGoogolGtsSetFunction SetAxis7EStop(_In_ double dblParam);

			virtual EGoogolGtsGetFunction GetAxis8Band(_Out_ long *pParam);
			virtual EGoogolGtsSetFunction SetAxis8Band(_In_ long lParam);

			virtual EGoogolGtsGetFunction GetAxis8Time(_Out_ long *pParam);
			virtual EGoogolGtsSetFunction SetAxis8Time(_In_ long lParam);

			virtual EGoogolGtsGetFunction GetAxis8DirAndPos(_Out_ long *pParam);
			virtual EGoogolGtsSetFunction SetAxis8DirAndPos(_In_ long lParam);

			virtual EGoogolGtsGetFunction GetAxis8Velocity(_Out_ double *pParam);
			virtual EGoogolGtsSetFunction SetAxis8Velocity(_In_ double dblParam);

			virtual EGoogolGtsGetFunction GetAxis8Accel(_Out_ double *pParam);
			virtual EGoogolGtsSetFunction SetAxis8Accel(_In_ double dblParam);

			virtual EGoogolGtsGetFunction GetAxis8Offset(_Out_ long *pParam);
			virtual EGoogolGtsSetFunction SetAxis8Offset(_In_ long lParam);

			virtual EGoogolGtsGetFunction GetAxis8AbruptStop(_Out_ double*pParam);
			virtual EGoogolGtsSetFunction SetAxis8AbruptStop(_In_ double dblParam);

			virtual EGoogolGtsGetFunction GetAxis8EStop(_Out_ double*pParam);
			virtual EGoogolGtsSetFunction SetAxis8EStop(_In_ double dblParam);

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

			virtual bool MovePosition(_In_ double dPos, _In_ double dVel, _In_ double dAcc, _In_ bool bSCurve) override;
			virtual bool MoveDistance(_In_ double dDist, _In_ double dVel, _In_ double dAcc, _In_ bool bSCurve) override;
			virtual bool MoveVelocity(_In_ double dVel, _In_ double dAcc, _In_ bool bSCurve) override;

			virtual bool MoveJog(_In_ double dVel, _In_ double dAcl, _In_ BOOL bSCurve) override;
			virtual bool StopJog() override;

			virtual bool IsMotionDone() override;

			virtual bool MotorStop() override;
			virtual bool MotorEStop() override;

			virtual bool ChangeSpeed(_In_ double dSpeed) override;

			virtual bool WriteGenOutBit(_In_ int nBit, _In_ bool bOn) override;
			virtual bool ReadGenOutBit(_In_ int nBit) override;
			virtual bool ReadGenInputBit(_In_ int nBit) override;

			virtual bool WriteGenExtOutBit(_In_ int nChannel, _In_ int nBit, _In_ bool bOn);
			virtual bool ReadGenExtOutBit(_In_ int nChannel, _In_ int nBit);
			virtual bool ReadGenExtInputBit(_In_ int nChannel, _In_ int nBit);

			virtual void ClearExtOutport();

			//////////////////////////////////////////////////////////////////////////

			virtual bool MoveToHome() override;

			virtual EDeviceMotionHommingStatus GetHommingStatus() override;

			// googol lib ctrl
			virtual bool GetServoStatus(_In_ int nAxis);
			virtual bool SetServoOn(_In_ int nAxis, _In_ bool bOn);
			
			virtual bool GetCommandPosition(_In_ int nAxis, _Out_ double *pPos);
			virtual bool SetCommandPositionClear(_In_ int nAxis);

			virtual bool GetActualPosition(_In_ int nAxis, _Out_ double *pPos);
			virtual bool SetActualPositionClear(_In_ int nAxis);
			
			virtual bool GetAlarmStatus(_In_ int nAxis);
			virtual bool SetAlarmClear(_In_ int nAxis);

			virtual bool GetInposition(_In_ int nAxis);
			virtual bool GetLimitSensorN(_In_ int nAxis);
			virtual bool GetLimitSensorP(_In_ int nAxis);
			virtual bool GetHomeSensor(_In_ int nAxis);
			
			virtual bool MovePosition(_In_ int nAxis, _In_ double dPos, _In_ double dVel, _In_ double dAcc, _In_ bool bSCurve);
			virtual bool MoveDistance(_In_ int nAxis, _In_ double dDist, _In_ double dVel, _In_ double dAcc, _In_ bool bSCurve);
			virtual bool MoveVelocity(_In_ int nAxis, _In_ double dVel, _In_ double dAcc, _In_ bool bSCurve);

			virtual bool MoveJog(_In_ int nAxis, _In_ double dVel, _In_ double dAcl, _In_ BOOL bSCurve);
			virtual bool StopJog(_In_ int nAxis);
			
			virtual bool IsMotionDone(_In_ int nAxis);

			virtual bool MotorStop(_In_ int nAxis);
			virtual bool MotorEStop(_In_ int nAxis);

			virtual bool ChangeSpeed(_In_ int nAxis, _In_ double dSpeed);
			
			virtual bool MoveToHome(_In_ int nAxis);

			virtual EDeviceMotionHommingStatus GetHommingStatus(_In_ int nAxis);
			
			virtual bool ClearOutPort();
			virtual bool ClearInPort();

			virtual long GetOutputPortCount();
			virtual long GetInputPortCount();

			virtual long GetExtIOPortCount();

			//add more func.

			virtual short __gts_GetVersion(char **pVersion);
			virtual short __gts_AlarmOff(short axis);
			virtual short __gts_AlarmOn(short axis);
			virtual short __gts_LmtsOn(short axis, short limitType = -1);
			virtual short __gts_LmtsOff(short axis, short limitType = -1);
			
			virtual short __gts_LmtSns(unsigned short sense);
			virtual short __gts_GetSts(short axis, long *pSts, short count = 1, unsigned long *pClock = NULL);
			virtual short __gts_ClrSts(short axis, short count = 1);
			virtual short __gts_ZeroPos(short axis, short count = 1);
			
			virtual short __gts_SetSoftLimit(short axis, long positive, long negative);
			virtual short __gts_GetSoftLimit(short axis, long *pPositive, long *pNegative);
			
			virtual short __gts_PrfTrap(short profile);
			virtual short __gts_SetTrapPrm(short profile, __gts_STTrapPrm *pPrm);
			virtual short __gts_GetTrapPrm(short profile, __gts_STTrapPrm *pPrm);
			
			virtual short __gts_Update(long mask);
			virtual short __gts_SetPos(short profile, long pos);
			virtual short __gts_GetPos(short profile, long *pPos);
			virtual short __gts_SetVel(short profile, double vel);
			virtual short __gts_GetVel(short profile, double *pVel);

			//virtual bool __gts
			virtual short __gts_SetCrdPrm(short crd, __gts_STCrdPrm *pCrdPrm);
			virtual short __gts_GetCrdPrm(short crd, __gts_STCrdPrm *pCrdPrm);
			virtual short __gts_CrdSpace(short crd, long *pSpace, short fifo = 0);
			virtual short __gts_CrdData(short crd, __gts_STCrdData *pCrdData, short fifo = 0);
			virtual short __gts_CrdDataCircle(short crd, __gts_STCrdData *pCrdData, short fifo = 0);
			virtual short __gts_LnXY(short crd, long x, long y, double synVel, double synAcc, double velEnd = 0, short fifo = 0);
			virtual short __gts_LnXYZ(short crd, long x, long y, long z, double synVel, double synAcc, double velEnd = 0, short fifo = 0);
			virtual short __gts_LnXYZA(short crd, long x, long y, long z, long a, double synVel, double synAcc, double velEnd = 0, short fifo = 0);
			virtual short __gts_LnXYG0(short crd, long x, long y, double synVel, double synAcc, short fifo = 0);
			virtual short __gts_LnXYZG0(short crd, long x, long y, long z, double synVel, double synAcc, short fifo = 0);
			virtual short __gts_LnXYZAG0(short crd, long x, long y, long z, long a, double synVel, double synAcc, short fifo = 0);
			virtual short __gts_ArcXYR(short crd, long x, long y, double radius, short circleDir, double synVel, double synAcc, double velEnd = 0, short fifo = 0);
			virtual short __gts_ArcXYC(short crd, long x, long y, double xCenter, double yCenter, short circleDir, double synVel, double synAcc, double velEnd = 0, short fifo = 0);
			virtual short __gts_ArcYZR(short crd, long y, long z, double radius, short circleDir, double synVel, double synAcc, double velEnd = 0, short fifo = 0);
			virtual short __gts_ArcYZC(short crd, long y, long z, double yCenter, double zCenter, short circleDir, double synVel, double synAcc, double velEnd = 0, short fifo = 0);
			virtual short __gts_ArcZXR(short crd, long z, long x, double radius, short circleDir, double synVel, double synAcc, double velEnd = 0, short fifo = 0);
			virtual short __gts_ArcZXC(short crd, long z, long x, double zCenter, double xCenter, short circleDir, double synVel, double synAcc, double velEnd = 0, short fifo = 0);
			virtual short __gts_ArcXYZ(short crd, long x, long y, long z, double interX, double interY, double interZ, double synVel, double synAcc, double velEnd = 0, short fifo = 0);
			virtual short __gts_LnXYOverride2(short crd, long x, long y, double synVel, double synAcc, double velEnd = 0, short fifo = 0);
			virtual short __gts_LnXYZOverride2(short crd, long x, long y, long z, double synVel, double synAcc, double velEnd = 0, short fifo = 0);
			virtual short __gts_LnXYZAOverride2(short crd, long x, long y, long z, long a, double synVel, double synAcc, double velEnd = 0, short fifo = 0);
			virtual short __gts_LnXYG0Override2(short crd, long x, long y, double synVel, double synAcc, short fifo = 0);
			virtual short __gts_LnXYZG0Override2(short crd, long x, long y, long z, double synVel, double synAcc, short fifo = 0);
			virtual short __gts_LnXYZAG0Override2(short crd, long x, long y, long z, long a, double synVel, double synAcc, short fifo = 0);
			virtual short __gts_ArcXYROverride2(short crd, long x, long y, double radius, short circleDir, double synVel, double synAcc, double velEnd = 0, short fifo = 0);
			virtual short __gts_ArcXYCOverride2(short crd, long x, long y, double xCenter, double yCenter, short circleDir, double synVel, double synAcc, double velEnd = 0, short fifo = 0);
			virtual short __gts_ArcYZROverride2(short crd, long y, long z, double radius, short circleDir, double synVel, double synAcc, double velEnd = 0, short fifo = 0);
			virtual short __gts_ArcYZCOverride2(short crd, long y, long z, double yCenter, double zCenter, short circleDir, double synVel, double synAcc, double velEnd = 0, short fifo = 0);
			virtual short __gts_ArcZXROverride2(short crd, long z, long x, double radius, short circleDir, double synVel, double synAcc, double velEnd = 0, short fifo = 0);
			virtual short __gts_ArcZXCOverride2(short crd, long z, long x, double zCenter, double xCenter, short circleDir, double synVel, double synAcc, double velEnd = 0, short fifo = 0);
			virtual short __gts_HelixXYRZ(short crd, long x, long y, long z, double radius, short circleDir, double synVel, double synAcc, double velEnd, short fifo = 0);
			virtual short __gts_HelixXYCZ(short crd, long x, long y, long z, double xCenter, double yCenter, short circleDir, double synVel, double synAcc, double velEnd = 0, short fifo = 0);
			virtual short __gts_HelixYZRX(short crd, long x, long y, long z, double radius, short circleDir, double synVel, double synAcc, double velEnd = 0, short fifo = 0);
			virtual short __gts_HelixYZCX(short crd, long x, long y, long z, double yCenter, double zCenter, short circleDir, double synVel, double synAcc, double velEnd = 0, short fifo = 0);
			virtual short __gts_HelixZXRY(short crd, long x, long y, long z, double radius, short circleDir, double synVel, double synAcc, double velEnd = 0, short fifo = 0);
			virtual short __gts_HelixZXCY(short crd, long x, long y, long z, double zCenter, double xCenter, short circleDir, double synVel, double synAcc, double velEnd = 0, short fifo = 0);
			virtual short __gts_HelixXYRZOverride2(short crd, long x, long y, long z, double radius, short circleDir, double synVel, double synAcc, double velEnd = 0, short fifo = 0);
			virtual short __gts_HelixXYCZOverride2(short crd, long x, long y, long z, double xCenter, double yCenter, short circleDir, double synVel, double synAcc, double velEnd = 0, short fifo = 0);
			virtual short __gts_HelixYZRXOverride2(short crd, long x, long y, long z, double radius, short circleDir, double synVel, double synAcc, double velEnd = 0, short fifo = 0);
			virtual short __gts_HelixYZCXOverride2(short crd, long x, long y, long z, double yCenter, double zCenter, short circleDir, double synVel, double synAcc, double velEnd = 0, short fifo = 0);
			virtual short __gts_HelixZXRYOverride2(short crd, long x, long y, long z, double radius, short circleDir, double synVel, double synAcc, double velEnd = 0, short fifo = 0);
			virtual short __gts_HelixZXCYOverride2(short crd, long x, long y, long z, double zCenter, double xCenter, short circleDir, double synVel, double synAcc, double velEnd = 0, short fifo = 0);
			virtual short __gts_LnXYWN(short crd, long x, long y, double synVel, double synAcc, double velEnd = 0, long segNum = 0, short fifo = 0);
			virtual short __gts_LnXYZWN(short crd, long x, long y, long z, double synVel, double synAcc, double velEnd = 0, long segNum = 0, short fifo = 0);
			virtual short __gts_LnXYZAWN(short crd, long x, long y, long z, long a, double synVel, double synAcc, double velEnd = 0, long segNum = 0, short fifo = 0);
			virtual short __gts_LnXYG0WN(short crd, long x, long y, double synVel, double synAcc, long segNum = 0, short fifo = 0);
			virtual short __gts_LnXYZG0WN(short crd, long x, long y, long z, double synVel, double synAcc, long segNum = 0, short fifo = 0);
			virtual short __gts_LnXYZAG0WN(short crd, long x, long y, long z, long a, double synVel, double synAcc, long segNum = 0, short fifo = 0);
			virtual short __gts_ArcXYRWN(short crd, long x, long y, double radius, short circleDir, double synVel, double synAcc, double velEnd = 0, long segNum = 0, short fifo = 0);
			virtual short __gts_ArcXYCWN(short crd, long x, long y, double xCenter, double yCenter, short circleDir, double synVel, double synAcc, double velEnd = 0, long segNum = 0, short fifo = 0);
			virtual short __gts_ArcYZRWN(short crd, long y, long z, double radius, short circleDir, double synVel, double synAcc, double velEnd = 0, long segNum = 0, short fifo = 0);
			virtual short __gts_ArcYZCWN(short crd, long y, long z, double yCenter, double zCenter, short circleDir, double synVel, double synAcc, double velEnd = 0, long segNum = 0, short fifo = 0);
			virtual short __gts_ArcZXRWN(short crd, long z, long x, double radius, short circleDir, double synVel, double synAcc, double velEnd = 0, long segNum = 0, short fifo = 0);
			virtual short __gts_ArcZXCWN(short crd, long z, long x, double zCenter, double xCenter, short circleDir, double synVel, double synAcc, double velEnd = 0, long segNum = 0, short fifo = 0);
			virtual short __gts_ArcXYZWN(short crd, long x, long y, long z, double interX, double interY, double interZ, double synVel, double synAcc, double velEnd = 0, long segNum = 0, short fifo = 0);
			virtual short __gts_LnXYOverride2WN(short crd, long x, long y, double synVel, double synAcc, double velEnd = 0, long segNum = 0, short fifo = 0);
			virtual short __gts_LnXYZOverride2WN(short crd, long x, long y, long z, double synVel, double synAcc, double velEnd = 0, long segNum = 0, short fifo = 0);
			virtual short __gts_LnXYZAOverride2WN(short crd, long x, long y, long z, long a, double synVel, double synAcc, double velEnd = 0, long segNum = 0, short fifo = 0);
			virtual short __gts_LnXYG0Override2WN(short crd, long x, long y, double synVel, double synAcc, long segNum = 0, short fifo = 0);
			virtual short __gts_LnXYZG0Override2WN(short crd, long x, long y, long z, double synVel, double synAcc, long segNum = 0, short fifo = 0);
			virtual short __gts_LnXYZAG0Override2WN(short crd, long x, long y, long z, long a, double synVel, double synAcc, long segNum = 0, short fifo = 0);
			virtual short __gts_ArcXYROverride2WN(short crd, long x, long y, double radius, short circleDir, double synVel, double synAcc, double velEnd = 0, long segNum = 0, short fifo = 0);
			virtual short __gts_ArcXYCOverride2WN(short crd, long x, long y, double xCenter, double yCenter, short circleDir, double synVel, double synAcc, double velEnd = 0, long segNum = 0, short fifo = 0);
			virtual short __gts_ArcYZROverride2WN(short crd, long y, long z, double radius, short circleDir, double synVel, double synAcc, double velEnd = 0, long segNum = 0, short fifo = 0);
			virtual short __gts_ArcYZCOverride2WN(short crd, long y, long z, double yCenter, double zCenter, short circleDir, double synVel, double synAcc, double velEnd = 0, long segNum = 0, short fifo = 0);
			virtual short __gts_ArcZXROverride2WN(short crd, long z, long x, double radius, short circleDir, double synVel, double synAcc, double velEnd = 0, long segNum = 0, short fifo = 0);
			virtual short __gts_ArcZXCOverride2WN(short crd, long z, long x, double zCenter, double xCenter, short circleDir, double synVel, double synAcc, double velEnd = 0, long segNum = 0, short fifo = 0);
			virtual short __gts_HelixXYRZWN(short crd, long x, long y, long z, double radius, short circleDir, double synVel, double synAcc, double velEnd = 0, long segNum = 0, short fifo = 0);
			virtual short __gts_HelixXYCZWN(short crd, long x, long y, long z, double xCenter, double yCenter, short circleDir, double synVel, double synAcc, double velEnd = 0, long segNum = 0, short fifo = 0);
			virtual short __gts_HelixYZRXWN(short crd, long x, long y, long z, double radius, short circleDir, double synVel, double synAcc, double velEnd = 0, long segNum = 0, short fifo = 0);
			virtual short __gts_HelixYZCXWN(short crd, long x, long y, long z, double yCenter, double zCenter, short circleDir, double synVel, double synAcc, double velEnd = 0, long segNum = 0, short fifo = 0);
			virtual short __gts_HelixZXRYWN(short crd, long x, long y, long z, double radius, short circleDir, double synVel, double synAcc, double velEnd = 0, long segNum = 0, short fifo = 0);
			virtual short __gts_HelixZXCYWN(short crd, long x, long y, long z, double zCenter, double xCenter, short circleDir, double synVel, double synAcc, double velEnd = 0, long segNum = 0, short fifo = 0);
			virtual short __gts_HelixXYRZOverride2WN(short crd, long x, long y, long z, double radius, short circleDir, double synVel, double synAcc, double velEnd = 0, long segNum = 0, short fifo = 0);
			virtual short __gts_HelixXYCZOverride2WN(short crd, long x, long y, long z, double xCenter, double yCenter, short circleDir, double synVel, double synAcc, double velEnd = 0, long segNum = 0, short fifo = 0);
			virtual short __gts_HelixYZRXOverride2WN(short crd, long x, long y, long z, double radius, short circleDir, double synVel, double synAcc, double velEnd = 0, long segNum = 0, short fifo = 0);
			virtual short __gts_HelixYZCXOverride2WN(short crd, long x, long y, long z, double yCenter, double zCenter, short circleDir, double synVel, double synAcc, double velEnd = 0, long segNum = 0, short fifo = 0);
			virtual short __gts_HelixZXRYOverride2WN(short crd, long x, long y, long z, double radius, short circleDir, double synVel, double synAcc, double velEnd = 0, long segNum = 0, short fifo = 0);
			virtual short __gts_HelixZXCYOverride2WN(short crd, long x, long y, long z, double zCenter, double xCenter, short circleDir, double synVel, double synAcc, double velEnd = 0, long segNum = 0, short fifo = 0);
			virtual short __gts_BufIO(short crd, unsigned short doType, unsigned short doMask, unsigned short doValue, short fifo = 0);
			virtual short __gts_BufEnableDoBitPulse(short crd, short doType, short doIndex, unsigned short highLevelTime, unsigned short lowLevelTime, long pulseNum, short firstLevel, short fifo = 0);
			virtual short __gts_BufDisableDoBitPulse(short crd, short doType, short doIndex, short fifo = 0);
			virtual short __gts_BufDelay(short crd, unsigned short delayTime, short fifo = 0);
			virtual short __gts_BufComparePulse(short crd, short level, short outputType, short time, short fifo = 0);
			virtual short __gts_BufDA(short crd, short chn, short daValue, short fifo = 0);
			virtual short __gts_BufLmtsOn(short crd, short axis, short limitType, short fifo = 0);
			virtual short __gts_BufLmtsOff(short crd, short axis, short limitType, short fifo = 0);
			virtual short __gts_BufSetStopIo(short crd, short axis, short stopType, short inputType, short inputIndex, short fifo = 0);
			virtual short __gts_BufMove(short crd, short moveAxis, long pos, double vel, double acc, short modal, short fifo = 0);
			virtual short __gts_BufGear(short crd, short gearAxis, long pos, short fifo = 0);
			virtual short __gts_BufGearPercent(short crd, short gearAxis, long pos, short accPercent, short decPercent, short fifo = 0);
			virtual short __gts_BufStopMotion(short crd, short fifo = 0);
			virtual short __gts_BufSetVarValue(short crd, short pageId, __gts_STVarInfo *pVarInfo, double value, short fifo = 0);
			virtual short __gts_BufJumpNextSeg(short crd, short axis, short limitType, short fifo = 0);
			virtual short __gts_BufSynchPrfPos(short crd, short encoder, short profile, short fifo = 0);
			virtual short __gts_BufVirtualToActual(short crd, short fifo = 0);
			virtual short __gts_BufSetLongVar(short crd, short index, long value, short fifo = 0);
			virtual short __gts_BufSetDoubleVar(short crd, short index, double value, short fifo = 0);
			virtual short __gts_CrdStart(short mask, short option);
			virtual short __gts_CrdStartStep(short mask, short option);
			virtual short __gts_CrdStepMode(short mask, short option);
			virtual short __gts_SetOverride(short crd, double synVelRatio);
			virtual short __gts_SetOverride2(short crd, double synVelRatio);
			virtual short __gts_InitLookAhead(short crd, short fifo, double T, double accMax, short n, __gts_STCrdData *pLookAheadBuf);
			virtual short __gts_GetLookAheadSpace(short crd, long *pSpace, short fifo = 0);
			virtual short __gts_GetLookAheadSegCount(short crd, long *pSegCount, short fifo = 0);
			virtual short __gts_CrdClear(short crd, short fifo);
			virtual short __gts_CrdStatus(short crd, short *pRun, long *pSegment, short fifo = 0);
			virtual short __gts_SetUserSegNum(short crd, long segNum, short fifo = 0);
			virtual short __gts_GetUserSegNum(short crd, long *pSegment, short fifo = 0);
			virtual short __gts_GetUserSegNumWN(short crd, long *pSegment, short fifo = 0);
			virtual short __gts_GetRemainderSegNum(short crd, long *pSegment, short fifo = 0);
			virtual short __gts_SetCrdStopDec(short crd, double decSmoothStop, double decAbruptStop);
			virtual short __gts_GetCrdStopDec(short crd, double *pDecSmoothStop, double *pDecAbruptStop);
			virtual short __gts_SetCrdLmtStopMode(short crd, short lmtStopMode);
			virtual short __gts_GetCrdLmtStopMode(short crd, short *pLmtStopMode);
			virtual short __gts_GetUserTargetVel(short crd, double *pTargetVel);
			virtual short __gts_GetSegTargetPos(short crd, long *pTargetPos);
			virtual short __gts_GetCrdPos(short crd, double *pPos);
			virtual short __gts_GetCrdVel(short crd, double *pSynVel);
			virtual short __gts_SetCrdSingleMaxVel(short crd, double *pMaxVel);
			virtual short __gts_GetCrdSingleMaxVel(short crd, double *pMaxVel);
			virtual short __gts_BufLaserOn(short crd, short fifo = 0, short channel = 0);
			virtual short __gts_BufLaserOff(short crd, short fifo = 0, short channel = 0);
			virtual short __gts_BufLaserPrfCmd(short crd, double laserPower, short fifo = 0, short channel = 0);
			virtual short __gts_BufLaserFollowRatio(short crd, double ratio, double minPower, double maxPower, short fifo = 0, short channel = 0);
			virtual short __gts_BufLaserFollowMode(short crd, short source = 0, short fifo = 0, short channel = 0, double startPower = 0);
			virtual short __gts_BufLaserFollowOff(short crd, short fifo = 0, short channel = 0);
			virtual short __gts_BufLaserFollowSpline(short crd, short tableId, double minPower, double maxPower, short fifo = 0, short channel = 0);

			virtual short __gts_PrfPvt(short profile);
			virtual short __gts_SetPvtLoop(short profile, long loop);
			virtual short __gts_GetPvtLoop(short profile, long *pLoopCount, long *pLoop);
			virtual short __gts_PvtStatus(short profile, short *pTableId, double *pTime, short count = 1);
			virtual short __gts_PvtStart(long mask);
			virtual short __gts_PvtTableSelect(short profile, short tableId);

			virtual short __gts_PvtTable(short tableId, long count, double *pTime, double *pPos, double *pVel);
			virtual short __gts_PvtTableEx(short tableId, long count, double *pTime, double *pPos, double *pVelBegin, double *pVelEnd);
			virtual short __gts_PvtTableComplete(short tableId, long count, double *pTime, double *pPos, double *pA, double *pB, double *pC, double velBegin = 0, double velEnd = 0);
			virtual short __gts_PvtTablePercent(short tableId, long count, double *pTime, double *pPos, double *pPercent, double velBegin = 0);
			virtual short __gts_PvtPercentCalculate(long n, double *pTime, double *pPos, double *pPercent, double velBegin, double *pVel);
			virtual short __gts_PvtTableContinuous(short tableId, long count, double *pPos, double *pVel, double *pPercent, double *pVelMax, double *pAcc, double *pDec, double timeBegin);
			virtual short __gts_PvtContinuousCalculate(long n, double *pPos, double *pVel, double *pPercent, double *pVelMax, double *pAcc, double *pDec, double *pTime);
			virtual short __gts_PvtTableMovePercent(short tableId, long distance, double vm, double acc, double pa1, double pa2, double dec, double pd1, double pd2, double *pVel, double *pAcc, double *pDec, double *pTime);
			virtual short __gts_HomeInit();
			virtual short __gts_Home(short axis, long pos, double vel, double acc, long offset);
			virtual short __gts_Index(short axis, long pos, long offset);
			virtual short __gts_HomeStop(short axis, long pos, double vel, double acc);
			virtual short __gts_HomeSts(short axis, unsigned short *pStatus);

			virtual short __gts_HandwheelInit();
			virtual short __gts_SetHandwheelStopDec(short slave, double decSmoothStop, double decAbruptStop);
			virtual short __gts_StartHandwheel(short slave, short master, short masterEven, short slaveEven, short intervalTime, double acc, double dec, double vel, short stopWaitTime);
			virtual short __gts_EndHandwheel(short slave);


			virtual short __gts_GoHome(short axis, __gts_STHomePrm *pHomePrm);
			virtual short __gts_GetHomePrm(short axis, __gts_STHomePrm *pHomePrm);
			virtual short __gts_GetHomeStatus(short axis, __gts_STHomeStatus *pHomeStatus);
			
			long GetStatus(_In_ int nAxis);

			bool SetHomeCaptureMode(_In_ int nAxis);
			bool GetCaptureStatus(_In_ int nAxis, _Out_ double* dblCapturePosition);

			afx_msg void OnTimer(_In_ UINT_PTR nIDEvent);
			afx_msg void OnDestroy();
			virtual void OnCancel();

			virtual BOOL OnInitDialog();

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;

		public:
			DECLARE_MESSAGE_MAP()

		protected:
			void OnBnClickedOutLed(_In_ UINT nID);

			// 추가 기능
			void OnBnClickedExtOutLed(_In_ UINT nID);
			//////////////////////////////////////////////////////////////////////////

			virtual void BroadcastViewInit();

			virtual bool AddControls() override;

			virtual void AddIOs(_In_ double dblDPIScale, _In_ CFont* pBaseFont, _In_ CRavidPoint<int> rpLeftTopPt, _In_ CRavidPoint<int> rpRightTopPt, _Out_ CRavidRect<int>& rrResultGroupBox);
			virtual void AddExtIOs(_In_ Ravid::Framework::CGoogolDioViewer* pViewr, _In_ double dblDPIScale, _In_ int nStartID, _In_ CFont* pBaseFont, _In_ CRavidPoint<int> rpLeftTopPt, _In_ CRavidPoint<int> rpRightTopPt, _Out_ CRavidRect<int>& rrResultGroupBox);
			virtual void AddMotions(_In_ double dblDPIScale, _In_ CFont* pBaseFont, _In_ CRavidPoint<int> rpLeftTopPt, _In_ CRavidPoint<int> rpRightTopPt, _Out_ CRavidRect<int>& rrResultGroupBox);

			virtual bool UpdateControls() override;

			virtual bool DoesModuleExist();

		protected:
			volatile DWORD m_dwOutStatus;

			bool m_bCreateMotionUI = false;

			bool* m_pBUsingInputArray = nullptr;
			bool* m_pBUsingOutputArray = nullptr;

			CString* m_pStrInputNameArray = nullptr;
			CString* m_pStrOutputNameArray = nullptr;

			CString** m_ppStrExtInputName = nullptr;
			CString** m_ppStrExtOutputName = nullptr;

			Ravid::Framework::CGoogolDioViewer* m_pDlgExtDioViewer = nullptr;

			CDeviceGoogolGTS::SGTS_MotionInfo m_sMotionInfo[8];
		};
	}
}
