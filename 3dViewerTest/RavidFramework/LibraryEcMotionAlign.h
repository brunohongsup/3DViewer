#pragma once

#include "RavidObjectEx.h"

// UVW Type Table Motor 상태
#define UVW_XTLYBLR			0		// XTL:Top Left,		YBL:Bottom Left,	YBR:Bottom Right
#define UVW_XBLYTLR			1		// XBL:Bottom Left,		YTL:Top Left,		YTR:Top Right
#define UVW_XTBRYBL			2		// XTR:Top Right,		XBR:Bottom Right,	YBL:Bottom Left
#define UVW_XTBLYBR			3		// XTL:Top Left,		XBL:Bottom Left,	YBR:Bottom Right
#define UVW_XTRYBLR			4		// XTL:Top Right,		YBL:Bottom Left,	YBR:Bottom Right
#define UVW_XBRYTLR			5		// XBL:Bottom Right,	YTL:Top Left,		YTR:Top Right
#define UVW_XTBRYTL			6		// XTR:Top Right,		XBR:Bottom Right,	YBL:Top Left
#define UVW_XTBLYTR			7		// XTL:Top Left,		XBL:Bottom Left,	YBR:Top Right


// Camera & Real Coordinate 관계
#define CAM_XY_SAME			0		// X, Y 방향 같음	( Camera and Real 같음 ) 
#define CAM_X_REVERSE		1		// X 방향 반대		( Camera and Real 같음 ) 
#define CAM_Y_REVERSE		2		// Y 방향 반대		( Camera and Real 같음 ) 
#define CAM_XY_REVERSE		3		// X, Y 방향 반대	( Camera and Real 같음 ) 
#define CAM_90XY_SAME		4		// X, Y 방향 같음	( Camera and Real X, Y 반대 )
#define CAM_90X_REVERSE		5		// X 방향 반대		( Camera and Real X, Y 반대 )
#define CAM_90Y_REVERSE		6		// Y 방향 반대		( Camera and Real X, Y 반대 )
#define CAM_90XY_REVERSE	7		// X, Y 방향 반대	( Camera and Real X, Y 반대 )

// Calibration Move 순서
#define CAL_X_MIN			0		// Real X Minimum Move Position
#define CAL_X_MAX			1		// Real X Maximum Move Position
#define CAL_Y_MIN			2		// Real Y Minimum Move Position
#define CAL_Y_MAX			3		// Real Y Maximum Move Position
#define CAL_T_MIN			4		// Real Theta Minimum Move Position
#define CAL_T_MAX			5		// Real Theta Maximum Move Position
#define CAL_BUFF			6		// 

// Coordinate 
#define COORD_1				0		// 1 사분면 방향
#define COORD_2				1		// 2 사분면 방향
#define COORD_3				2		// 3 사분면 방향
#define COORD_4				3		// 4 사분면 방향

// Alignment Method
#define ALG_H_CENTER		0		// Align Horizontal Center
#define ALG_H_LEFT			1		// Align Horizontal Left
#define ALG_H_RIGHT			2		// Align Horizontal Right
#define ALG_V_CENTER		3		// Align Vertical Center
#define ALG_V_TOP			4		// Align Vertical Top
#define ALG_V_BOTTOM		5		// Align Vertical Bottom
#define ALG_HV_CENTER		6		// Align Horizontal & Vertical Center
#define ALG_LEFTTOP			7		// Align Left & Top
#define ALG_RIGHTTOP		8		// Align Right & Top
#define ALG_LEFTBOTTOM		9		// Align Left & Bottom
#define ALG_RIGHTBOTTOM		10		// Align Right & Bottom

// Timer
#define	TIMER_RESET		0							// Resets a timer to zero.
#define	TIMER_READ		1							// Reads the time (in milliseconds) of the timer, since the last reset.

typedef struct __stPoint_2Int
{
	int X;
	int Y;
	__stPoint_2Int()
	{
		X = 0;
		Y = 0;
	}
}__stXYINT, __stXYLONG;

typedef struct __stPoint_3Int
{
	int X;
	int Y;
	int T;
	__stPoint_3Int()
	{
		X = 0;
		Y = 0;
		T = 0;
	}
}__stXYTINT, __stXYTLONG;

typedef struct __stPoint_4Int
{
	int X;
	int Y;
	int T;
	int Z;
	__stPoint_4Int()
	{
		X = 0;
		Y = 0;
		T = 0;
		Z = 0;
	}
}__stXYTZINT, __stXYTZLONG;

typedef struct __stPoint_2Double
{
	double X;
	double Y;
	__stPoint_2Double()
	{
		X = 0.0;
		Y = 0.0;
	}
}__stXYDB, __stXYFL;

typedef struct __stPoint_3Double
{
	double X;
	double Y;
	double T;
	__stPoint_3Double()
	{
		X = 0.0;
		Y = 0.0;
		T = 0.0;
	}
}__stXYTDB, __stXYTFL;

typedef struct __stPoint_4Double
{
	double X;
	double Y;
	double T;
	double Z;
	__stPoint_4Double()
	{
		X = 0.0;
		Y = 0.0;
		T = 0.0;
		Z = 0.0;
	}
}__stXYTZDB, __stXYTZFL;


// 직선을 표현한 Structure
// y = mx + b	m : 기울기, b : y절편(직선이 x축과 만나는 점(0,b)의 y 값
// 직선을 일반식 ax + by + c = 0로 표현함
typedef struct __stLine
{
	double	a;				// X의 계수
	double	b;				// Y의 계수
	double	c;				// 상수항

	__stLine()
	{
		a = 0.0;
		b = 0.0;
		c = 0.0;
	}
}__stLINE;

// Calibration 관련
typedef struct __stCalibration
{
	long	iCamCoordinate;		// 변화해야 하는 Camera의 좌표 내용
	long	iRealCoordinate;	// Real 좌표상의 Camera 위치
	__stXYDB	stCamRd[10];		// Camera Read Point
	__stXYTDB	stMtMv[10];			// Motor Move Point
	__stXYTDB	stCamCail;			// X, Y Resolution, Camera Angle
	__stXYDB	stRealOrigin;		// Camera Center의 Real 좌표값
	__stXYDB	stCamOrigin;		// Camera Center의 mm 좌표값 (Camera 기준)
	__stXYDB	stCamSize;			// Camera Size (Width, Height)
	__stCalibration()
	{
		iCamCoordinate = CAM_XY_SAME;
		iRealCoordinate = COORD_1;
	}
}__stCALI;

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CLibraryEcMotionAlign : public CRavidObjectEx
		{
			RavidUseDynamicCreation();
			RavidPreventCopySelf(CLibraryEcMotionAlign);

		public:
			CLibraryEcMotionAlign(void);
			virtual ~CLibraryEcMotionAlign(void);

			enum eSystemInfo
			{
				eAlignCamCount = 4,
				eCamCalibrationPoint = 6,
			};

			enum eAlignTableType
			{
				eUVW_XTLYBLR = 0,
				eUVW_XBLYTLR,
				eUVW_XTBRYBL,
				eUVW_XTBLYBR,
				eUVW_XTRYBLR,
				eUVW_XBRYTLR,
				eUVW_XTBRYTL,
				eUVW_XTBLYTR,

				//2013 inchul Add
				eTable_1XP3XN2YP = -1,
				eTable_1XP3XP4YN = -2,
				eTable_1XP4XP2YP = -3,
				eTable_2XN3XN4YN = -4,
				eTable_2XP3XP4YP = -5,

				eTable_2XP3YN4YN = -7,
				eTable_1XN3YP4YP = -8,
				eTable_3XN1YN2YN = -9,


				eTable_2XN3XN1YP = 0,
				eTable_1XP4XP3YN = 1,
				eTable_2XN3YN4YN = 2,
				eTable_4XP1YP2YP = 3,
				eTable_2XP3XP4YN = 4,

		//		eTable_2XN3XN1YP = 7,	->0번과 중복
				eTable_2XN4XP1YP = 8,
				eTable_2XP4XP1YN = 9,
				eTable_2XN4XP3YN = 10,
				eTable_2XP4XP3YN = 11,
				eTable_1XP3XN4YN = 12,
				eTable_2XN3XN4YP = 13,
				eTable_1XP3YN4YN = 14,
				eTable_3XN1YP2YP = 15,
				eTable_2XN1YP3YN = 16,
				eTable_2XP3XP1YP = 17,
				eTable_2XN1YP3YP = 18,
				eTable_4XN1YN2YN = 19,

				eUVW_3IDLE_90XP225YN315YN = 31,		//3개아이들, 90도XP,3YN, 4YN 

				eXYT = 100,
			};

			enum eAlignMethod
			{
				eALG_H_CENTER = 0,
				eALG_H_LEFT,
				eALG_H_RIGHT,
				eALG_V_CENTER,
				eALG_V_TOP,
				eALG_V_BOTTOM,
				eALG_HV_CENTER,
				eALG_LEFTTOP,
				eALG_RIGHTTOP,
				eALG_LEFTBOTTOM,
				eALG_RIGHTBOTTOM,
			};
			
			enum eDirection
			{
				eDirection_LtoR,
				eDirection_RtoL,
				eDirection_TtoB,
				eDirection_BtoT,
				eDirection_LtoRtoL,
				eDirection_LtoRtoB,
				eDirection_LtoRtoT,
				eDirection_RtoLtoR,
				eDirection_RtoLtoB,
				eDirection_RtoLtoT,
				eDirection_TtoBtoT,
				eDirection_TtoBtoL,
				eDirection_TtoBtoR,
				eDirection_BtoTtoB,
				eDirection_BtoTtoL,
				eDirection_BtoTtoR,
			};

			typedef struct sDPOINT
			{
				double	x;
				double	y;

				operator POINT()
				{
					POINT pt;

					pt.x = (long)x;
					pt.y = (long)y;

					return pt;
				}

				sDPOINT& operator =(POINT& pt)
				{
					x = pt.x;
					y = pt.y;

					return *this;
				}
			}
			DPOINT, *LPDPOINT;

			typedef struct sPOINT_XYT
			{
				long x;
				long y;
				long t;

				operator POINT()
				{
					POINT pt;

					pt.x = x;
					pt.y = y;

					return pt;
				}

				operator DPOINT()
				{
					DPOINT dpt;

					dpt.x = x;
					dpt.y = y;

					return dpt;
				}

				sPOINT_XYT& operator =(POINT& pt)
				{
					x = pt.x;
					y = pt.y;
					t = 0;

					return *this;
				}

				sPOINT_XYT& operator =(DPOINT& dpt)
				{
					x = (long)dpt.x;
					y = (long)dpt.y;
					t = (long)0;

					return *this;
				}
			}
			POINT_XYT, *LPPOINT_XYT;

			typedef struct sDPOINT_XYT
			{
				double x;
				double y;
				double t;

				operator POINT()
				{
					POINT pt;

					pt.x = (long)x;
					pt.y = (long)y;

					return pt;
				}

				operator sDPOINT()
				{
					sDPOINT dpt;

					dpt.x = x;
					dpt.y = y;

					return dpt;
				}

				operator POINT_XYT()
				{
					DPOINT_XYT pt;

					pt.x = (long)x;
					pt.y = (long)y;
					pt.t = (long)t;

					return pt;
				}

				sDPOINT_XYT& operator =(sDPOINT& dpt)
				{
					x = dpt.x;
					y = dpt.y;
					t = 0;

					return *this;
				}

				sDPOINT_XYT& operator =(POINT& pt)
				{
					x = pt.x;
					y = pt.y;
					t = 0;

					return *this;
				}

				sDPOINT_XYT& operator =(POINT_XYT& pt)
				{
					x = pt.x;
					y = pt.y;
					t = pt.t;

					return *this;
				}
			}
			DPOINT_XYT, *LPDPOINT_XYT;

			typedef struct sDRECT
			{
				double left;
				double top;
				double right;
				double bottom;

				operator RECT()
				{
					RECT rc;

					rc.left = (long)left;
					rc.top = (long)top;
					rc.right = (long)right;
					rc.bottom = (long)bottom;

					return rc;
				}

				sDRECT& operator =(RECT& rc)
				{
					left = rc.left;
					top = rc.top;
					right = rc.right;
					bottom = rc.bottom;

					return *this;
				}

				void Offset(DPOINT& dptOffset)
				{
					left += dptOffset.x;
					top += dptOffset.y;
					right += dptOffset.x;
					bottom += dptOffset.y;
				}

				void Offset(POINT& ptOffset)
				{
					left += ptOffset.x;
					top += ptOffset.y;
					right += ptOffset.x;
					bottom += ptOffset.y;
				}

				void Offset(int nX, int nY)
				{
					left += nX;
					top += nY;
					right += nX;
					bottom += nY;
				}

				void Offset(double dblX, double dblY)
				{
					left += dblX;
					top += dblY;
					right += dblX;
					bottom += dblY;
				}
			}
			DRECT, *LPDRECT;

			typedef struct sQUADRANGLE
			{
				POINT	ptKnot[4];

				operator RECT()
				{
					RECT rc;

					rc.left = __min(__min(__min(ptKnot[0].x, ptKnot[1].x), ptKnot[2].x), ptKnot[3].x);
					rc.top = __min(__min(__min(ptKnot[0].y, ptKnot[1].y), ptKnot[2].y), ptKnot[3].y);
					rc.right = __max(__max(__max(ptKnot[0].x, ptKnot[1].x), ptKnot[2].x), ptKnot[3].x);
					rc.bottom = __max(__max(__max(ptKnot[0].y, ptKnot[1].y), ptKnot[2].y), ptKnot[3].y);

					return rc;
				}

				sQUADRANGLE& operator =(RECT& rc)
				{
					ptKnot[0].x = rc.left;
					ptKnot[0].y = rc.top;
					ptKnot[1].x = rc.right;
					ptKnot[1].y = rc.top;
					ptKnot[2].x = rc.right;
					ptKnot[2].y = rc.bottom;
					ptKnot[3].x = rc.left;
					ptKnot[3].y = rc.bottom;

					return *this;
				}

				operator sDRECT()
				{
					sDRECT drc;

					drc.left = __min(__min(__min(ptKnot[0].x, ptKnot[1].x), ptKnot[2].x), ptKnot[3].x);
					drc.top = __min(__min(__min(ptKnot[0].y, ptKnot[1].y), ptKnot[2].y), ptKnot[3].y);
					drc.right = __max(__max(__max(ptKnot[0].x, ptKnot[1].x), ptKnot[2].x), ptKnot[3].x);
					drc.bottom = __max(__max(__max(ptKnot[0].y, ptKnot[1].y), ptKnot[2].y), ptKnot[3].y);

					return drc;
				}

				sQUADRANGLE& operator =(sDRECT& drc)
				{
					ptKnot[0].x = (long)drc.left;
					ptKnot[0].y = (long)drc.top;
					ptKnot[1].x = (long)drc.right;
					ptKnot[1].y = (long)drc.top;
					ptKnot[2].x = (long)drc.right;
					ptKnot[2].y = (long)drc.bottom;
					ptKnot[3].x = (long)drc.left;
					ptKnot[3].y = (long)drc.bottom;

					return *this;
				}

				void Offset(DPOINT& dptOffset)
				{
					for(int i = 0; i < 4; ++i)
					{
						ptKnot[i].x += (LONG)dptOffset.x;
						ptKnot[i].y += (LONG)dptOffset.y;
					}
				}

				void Offset(POINT& ptOffset)
				{
					for(int i = 0; i < 4; ++i)
					{
						ptKnot[i].x += ptOffset.x;
						ptKnot[i].y += ptOffset.y;
					}
				}

				void Offset(int nX, int nY)
				{
					for(int i = 0; i < 4; ++i)
					{
						ptKnot[i].x += nX;
						ptKnot[i].y += nY;
					}
				}

				void Offset(double dblX, double dblY)
				{
					for(int i = 0; i < 4; ++i)
					{
						ptKnot[i].x += (LONG)dblX;
						ptKnot[i].y += (LONG)dblY;
					}
				}
			}
			QUADRANGLE, *LPQUADRANGLE;

			typedef struct sDQUADRANGLE
			{
				DPOINT	ptKnot[4];

				operator RECT()
				{
					RECT rc;

					rc.left = (long)__min(__min(__min(ptKnot[0].x, ptKnot[1].x), ptKnot[2].x), ptKnot[3].x);
					rc.top = (long)__min(__min(__min(ptKnot[0].y, ptKnot[1].y), ptKnot[2].y), ptKnot[3].y);
					rc.right = (long)__max(__max(__max(ptKnot[0].x, ptKnot[1].x), ptKnot[2].x), ptKnot[3].x);
					rc.bottom = (long)__max(__max(__max(ptKnot[0].y, ptKnot[1].y), ptKnot[2].y), ptKnot[3].y);

					return rc;
				}

				sDQUADRANGLE& operator =(RECT& rc)
				{
					ptKnot[0].x = rc.left;
					ptKnot[0].y = rc.top;
					ptKnot[1].x = rc.right;
					ptKnot[1].y = rc.top;
					ptKnot[2].x = rc.right;
					ptKnot[2].y = rc.bottom;
					ptKnot[3].x = rc.left;
					ptKnot[3].y = rc.bottom;

					return *this;
				}

				operator sDRECT()
				{
					sDRECT drc;

					drc.left = __min(__min(__min(ptKnot[0].x, ptKnot[1].x), ptKnot[2].x), ptKnot[3].x);
					drc.top = __min(__min(__min(ptKnot[0].y, ptKnot[1].y), ptKnot[2].y), ptKnot[3].y);
					drc.right = __max(__max(__max(ptKnot[0].x, ptKnot[1].x), ptKnot[2].x), ptKnot[3].x);
					drc.bottom = __max(__max(__max(ptKnot[0].y, ptKnot[1].y), ptKnot[2].y), ptKnot[3].y);

					return drc;
				}

				sDQUADRANGLE& operator =(sDRECT& drc)
				{
					ptKnot[0].x = drc.left;
					ptKnot[0].y = drc.top;
					ptKnot[1].x = drc.right;
					ptKnot[1].y = drc.top;
					ptKnot[2].x = drc.right;
					ptKnot[2].y = drc.bottom;
					ptKnot[3].x = drc.left;
					ptKnot[3].y = drc.bottom;

					return *this;
				}

				operator sQUADRANGLE()
				{
					sQUADRANGLE qdr;

					qdr.ptKnot[0] = ptKnot[0];
					qdr.ptKnot[1] = ptKnot[1];
					qdr.ptKnot[2] = ptKnot[2];
					qdr.ptKnot[3] = ptKnot[3];

					return qdr;
				}

				sDQUADRANGLE& operator =(sQUADRANGLE& qdr)
				{
					ptKnot[0] = qdr.ptKnot[0];
					ptKnot[1] = qdr.ptKnot[1];
					ptKnot[2] = qdr.ptKnot[2];
					ptKnot[3] = qdr.ptKnot[3];

					return *this;
				}

				void Offset(DPOINT& dptOffset)
				{
					for(int i = 0; i < 4; ++i)
					{
						ptKnot[i].x += dptOffset.x;
						ptKnot[i].y += dptOffset.y;
					}
				}

				void Offset(POINT& ptOffset)
				{
					for(int i = 0; i < 4; ++i)
					{
						ptKnot[i].x += ptOffset.x;
						ptKnot[i].y += ptOffset.y;
					}
				}

				void Offset(int nX, int nY)
				{
					for(int i = 0; i < 4; ++i)
					{
						ptKnot[i].x += nX;
						ptKnot[i].y += nY;
					}
				}

				void Offset(double dblX, double dblY)
				{
					for(int i = 0; i < 4; ++i)
					{
						ptKnot[i].x += dblX;
						ptKnot[i].y += dblY;
					}
				}

			}
			DQUADRANGLE, *LPDQUADRANGLE;
			
			typedef struct sEllipse
			{
				DPOINT dptCenter;
				float fWidth;
				float fHeight;
				float fAngle;

				sEllipse()
				{
					dptCenter.x = 0.;
					dptCenter.y = 0.;
					fWidth = 0.f;
					fHeight = 0.f;
					fAngle = 0.f;
				}
			}
			SEllipse, *LPEllipse;

			typedef struct sCircle
			{
				DPOINT dptCenter;
				float fRadius;

				sCircle()
				{
					dptCenter.x = 0.;
					dptCenter.y = 0.;
					fRadius = 0.f;
				}
			}
			SCircle, *LPCircle;

			typedef struct sLine
			{
				DPOINT dptStart;
				DPOINT dptEnd;

				sLine()
				{
					dptStart.x = 0.;
					dptStart.y = 0.;
					dptEnd.x = 0.;
					dptEnd.y = 0.;
				}
			}
			SLine, *LPLine;

			typedef struct sArc : public sCircle
			{
				float fStartAngle;
				float fEndAngle;

				sArc()
				{
					fStartAngle = 0.f;
					fEndAngle = 0.f;
				}
			}
			SArc, *LPArc;

			typedef struct sPolyline
			{
				long nCounts;
				DPOINT* pDptPoints;

				sPolyline()
				{
					nCounts = 0;
					pDptPoints = nullptr;
				}
			}
			SPolyline, *LPPolyline;

			typedef struct sSpline : public sPolyline
			{
				sSpline()
				{
				}
			}
			SSpline, *LPSpline;
			
			typedef struct sAlignSystemInfo
			{
				eAlignTableType eTableType;

				double dblTableSizeW;
				double dblTableSizeH;

				long nCamSizeX;
				long nCamSizeY;

			} 
			SAlignSystemInfo, *LPAlignSystemInfo;

			bool InitializeEC();
			bool TerminateEC();

			bool IsInitializeEC();

			bool LoadCalibrationData(LPCTSTR lpFileName);
			bool SaveCalibrationData(LPCTSTR lpFileName);

			bool SetAlignSystemInfo(LPAlignSystemInfo pAlignInfo);

			bool SetCalibMotorPosition(long nIndex, DPOINT_XYT dptDist);
			bool SetCalibVisionPosition(long nIndex, DPOINT dptFindC1, DPOINT dptFindC2);
			bool SetCalibVisionPosition_All(long nIndex, DPOINT dptFindC1, DPOINT dptFindC2, DPOINT dptFindC3, DPOINT dptFindC4);
			bool SetCalibVisionPosition(int nUnit, long nIndex, DPOINT dptFindC1, DPOINT dptFindC2);
			bool ApplyCalibVisionPosition();
			
			bool SetCameraInfo(long nImageSizeX, long nImageSizeY);

			bool SetTableInfo(eAlignTableType eTableType);

			bool SetUserOffset(DPOINT_XYT dptXYT);

			bool GetAlignmentPosition(eAlignMethod eMothod, DPOINT dptTargetC1, DPOINT dptTargetC2, DPOINT dptFindC1, DPOINT dptFindC2, DPOINT_XYT dptAlignOffset, DPOINT_XYT dptCurrentOffset, double* pdblMt1, double* pdblMt2, double* pdblMt3, LPDPOINT_XYT pdptCenterDist);

			bool GetAlignmentPosition(eAlignMethod eMothod, DPOINT dptTargetC1, DPOINT dptTargetC2, DPOINT dptFindC1, DPOINT dptFindC2, DPOINT_XYT dptAlignOffset, DPOINT_XYT dptCurrentOffset, double* pdblMt1, double* pdblMt2, double* pdblMt3, LPDPOINT_XYT pdptCenterDist, int nUnit);

			bool GetAlignmentPosition(eAlignMethod eMothod, DPOINT dptTargetC1, DPOINT dptTargetC2, DPOINT dptFindC1, DPOINT dptFindC2, DPOINT_XYT dptAlignOffset, DPOINT_XYT dptCurrentOffset, double* pdblMt1, double* pdblMt2, double* pdblMt3, LPDPOINT_XYT pdptCenterDist, double* pRotateAngle);

			bool GetAlignmentPosition2(eAlignMethod eMothod, DPOINT dptTargetC1, DPOINT dptTargetC2, DPOINT dptFindC1, DPOINT dptFindC2, DPOINT_XYT dptAlignOffset, DPOINT_XYT dptCurrentOffset, double* pdblMt1, double* pdblMt2, double* pdblMt3, LPDPOINT_XYT pdptCenterDist, int nUnit);

			bool GetAlignmentPositionXYT(eAlignMethod eMothod, DPOINT dptTargetC1, DPOINT dptTargetC2, DPOINT dptFindC1, DPOINT dptFindC2, DPOINT_XYT dptAlignOffset, DPOINT_XYT dptCurrentOffset, LPDPOINT_XYT pdptCenterDist);

			bool GetRotateDistance(double dblAngle, double dblCurrentAngle, double* pdblMt1, double* pdblMt2, double* pdblMt3);
			
			double GetCalibAccuracy(int nCam);

		protected:
			virtual bool DoesModuleExist();

			SAlignSystemInfo m_sAlignInfo;

			double m_dblCurrentAngle;

			bool GetCameraCalibration(__stCALI* pCalData);
			bool GetAlignDistance(eAlignMethod eMothod, __stXYTDB sTargetC1, __stXYTDB sTargetC2, __stXYTDB sFindC1, __stXYTDB sFindC2, __stXYTDB sCurrentOffset, __stXYTDB* pRtnDist);

			bool GetRealPosition(__stXYTDB* pRealPt1, __stXYTDB* pRealPt2);
			bool GetRealPositionUnit(__stXYTDB* pRealPt1, __stXYTDB* pRealPt2, int nUnit);
			bool GetRealPosition4Cam(__stXYTDB* pRealPt1, __stXYTDB* pRealPt2, int nUnit);

			__stCALI m_sCamCalibration[eAlignCamCount];
			__stCALI m_sCamCalibrationOrg[eAlignCamCount];
			__stCALI m_sCamCalibrationTmp[eAlignCamCount];

			bool m_bInitialize;
		};
	}
}