#pragma once

#include "RavidLine.h"
#include "RavidQuadrangle.h"
#include "RavidCircle.h"
#include "RavidEllipse.h"
#include "RavidPolygon.h"

namespace Ravid
{
	namespace Miscellaneous
	{
		class CInternalDxfReader;
		
		/**
		* DXF 포맷 파일을 읽기 위한 클래스입니다.
		*/
		class AFX_EXT_CLASS CDxfReader : public CRavidObject
		{
			RavidUseDynamicCreation();
			RavidPreventCopySelf(CDxfReader);

		public:
			CDxfReader(void);
			virtual ~CDxfReader(void);
			
			/**
			* DXF 파일을 로드 완료를 가져옵니다.
			*
			* return value : bool 의 상태를 반환합니다.
			*/
			bool IsLoaded();
			
			/**
			* DXF 파일을 읽어옵니다.
			*
			* param[in] lpszFileName : 로드 파일 이름
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			virtual bool Load(_In_ LPCTSTR lpszFileName);
			
			/**
			* 내부 데이터를 초기화합니다.
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			virtual bool Clear();
			
			/**
			* Load 실행 후 사용합니다.
			* 해당 인덱스의 스플라인 객체 정보를 가져옵니다.
			*
			* param[in] nIndex : 스플라인 인덱스
			*
			* return value : std::tuple<CString, CRavidPolygon> 의 내부 데이터를 반환합니다.
			*	CString 에는 객체를 정의한 이름입니다.
			*/
			std::tuple<CString, CRavidPolygon> GetSpline(_In_ int nIndex);
			
			/**
			* Load 실행 후 사용합니다.
			* 스플라인 객체 전체 개수를 가져옵니다.
			*
			* return value : size_t 의 스플라인 객체 개수를 반환합니다.
			*/
			size_t GetSplineCount();
			
			/**
			* Load 실행 후 사용합니다.
			* 해당 인덱스의 Polyline 객체 정보를 가져옵니다.
			*
			* param[in] nIndex : Polyline 인덱스
			*
			* return value : std::tuple<CString, CRavidPolygon> 의 내부 데이터를 반환합니다.
			*	CString 에는 객체를 정의한 이름입니다.
			*/
			std::tuple<CString, CRavidPolygon> GetPolyline(_In_ int nIndex);
			
			/**
			* Load 실행 후 사용합니다.
			* Polyline 객체 전체 개수를 가져옵니다.
			*
			* return value : size_t 의 Polyline 객체 개수를 반환합니다.
			*/
			size_t GetPolylineCount();
			
			/**
			* Load 실행 후 사용합니다.
			* 해당 인덱스의 Arc 객체 정보를 가져옵니다.
			*
			* param[in] nIndex : Arc 인덱스
			*
			* return value : std::tuple<CString, CRavidEllipse<double>> 의 내부 데이터를 반환합니다.
			*	CString 에는 객체를 정의한 이름입니다.
			*/
			std::tuple<CString, CRavidEllipse<double> > GetArc(_In_ int nIndex);
			
			/**
			* Load 실행 후 사용합니다.
			* Arc 객체 전체 개수를 가져옵니다.
			*
			* return value : size_t 의 Arc 객체 개수를 반환합니다.
			*/
			size_t GetArcCount();
			
			/**
			* Load 실행 후 사용합니다.
			* 해당 인덱스의 Circle 객체 정보를 가져옵니다.
			*
			* param[in] nIndex : Circle 인덱스
			*
			* return value : std::tuple<CString, CRavidEllipse<double>> 의 내부 데이터를 반환합니다.
			*	CString 에는 객체를 정의한 이름입니다.
			*/
			std::tuple<CString, CRavidEllipse<double> > GetCircle(_In_ int nIndex);
			
			/**
			* Load 실행 후 사용합니다.
			* Circle 객체 전체 개수를 가져옵니다.
			*
			* return value : size_t 의 Circle 객체 개수를 반환합니다.
			*/
			size_t GetCircleCount();
			
			/**
			* Load 실행 후 사용합니다.
			* 해당 인덱스의 Line 객체 정보를 가져옵니다.
			*
			* param[in] nIndex : Line 인덱스
			*
			* return value : std::tuple<CString, CRavidLine<double>> 의 내부 데이터를 반환합니다.
			*	CString 에는 객체를 정의한 이름입니다.
			*/
			std::tuple<CString, CRavidLine<double> > GetLine(_In_ int nIndex);
			
			/**
			* Load 실행 후 사용합니다.
			* Line 객체 전체 개수를 가져옵니다.
			*
			* return value : size_t 의 Line 객체 개수를 반환합니다.
			*/
			size_t GetLineCount();
			
			/**
			* Load 실행 후 사용합니다.
			* 해당 인덱스의 Hatch 객체 정보를 가져옵니다.
			*
			* param[in] nIndex : Hatch 인덱스
			*
			* return value : std::tuple<CString, CRavidPolygon> 의 내부 데이터를 반환합니다.
			*	CString 에는 객체를 정의한 이름입니다.
			*/
			std::tuple<CString, CRavidPolygon> GetHatch(_In_ int nIndex);
			
			/**
			* Load 실행 후 사용합니다.
			* Hatch 객체 전체 개수를 가져옵니다.
			*
			* return value : size_t 의 Hatch 객체 개수를 반환합니다.
			*/
			size_t GetHatchCount();
			
			/**
			* Load 실행 후 사용합니다.
			* 해당 인덱스의 Solid 객체 정보를 가져옵니다.
			*
			* param[in] nIndex : Solid 인덱스
			*
			* return value : std::tuple<CString, CRavidQuadrangle<double>> 의 내부 데이터를 반환합니다.
			*	CString 에는 객체를 정의한 이름입니다.
			*/
			std::tuple<CString, CRavidQuadrangle<double> > GetSolid(_In_ int nIndex);
			
			/**
			* Load 실행 후 사용합니다.
			* Solid 객체 전체 개수를 가져옵니다.
			*
			* return value : size_t 의 Solid 객체 개수를 반환합니다.
			*/
			size_t GetSolidCount();
			
			/**
			* Load 실행 후 사용합니다.
			* 해당 인덱스의 Ellipse 객체 정보를 가져옵니다.
			*
			* param[in] nIndex : Ellipse 인덱스
			*
			* return value : std::tuple<CString, CRavidEllipse<double>> 의 내부 데이터를 반환합니다.
			*	CString 에는 객체를 정의한 이름입니다.
			*/
			std::tuple<CString, CRavidEllipse<double> > GetEllipse(_In_ int nIndex);
			
			/**
			* Load 실행 후 사용합니다.
			* Ellipse 객체 전체 개수를 가져옵니다.
			*
			* return value : size_t 의 Ellipse 객체 개수를 반환합니다.
			*/
			size_t GetEllipseCount();

		private:
			CInternalDxfReader* m_pInternal = nullptr;
		};
	}
}

