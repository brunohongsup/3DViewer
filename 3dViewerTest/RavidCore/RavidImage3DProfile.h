#pragma once

#include "RavidImage.h"

namespace Ravid
{
	namespace Algorithms
	{
		/**
		* 3D 데이터를 이용하여 높이 변환 기능을 제공하는 클래스입니다.
		* 이 클래스는 아직 테스트가 완료되지 않았습니다.
		*/
		class AFX_EXT_CLASS CRavidImage3DProfile : public CRavidImage
		{
		public:
			CRavidImage3DProfile();
			virtual ~CRavidImage3DProfile();

			enum EInnermostPosition
			{
				EInnermostPosition_FirstPixel = 0,
				EInnermostPosition_LastPixel,
			};

			/**
			* 현재 객체를 입력된 크기로 초기화 합니다.
			* 내부 데이터는 float 타입으로 생성합니다.
			*
			* param[in] st3DSizeX : x 축 길이
			* param[in_opt] st3DSizeY : y축 길이 (Default : 1)
			*/
			virtual void Initialize(size_t st3DSizeX, size_t st3DSizeY = 1);
			
			/**
			* 현재 객체를 해제합니다.
			*/
			virtual void Terminate();
			
			/**
			* 현재 객체를 입력된 가로 길이를 가져옵니다.
			*
			* return value : size_t 형태로 반환합니다.
			*/
			size_t Get3DSizeX();
			
			/**
			* 현재 객체를 입력된 세로 길이를 가져옵니다.
			*
			* return value : size_t 형태로 반환합니다.
			*/
			size_t Get3DSizeY();
			
			/**
			* 현재 객체의 시작 데이터 위치를 가져옵니다.
			*
			* return value : float* 형태로 반환합니다.
			*/
			virtual float* GetRawData();
			
			/**
			* 아직 사용이 불가능합니다.
			*/
			virtual EAlgorithmResult CalculateData(_In_ double dblCameraAngle, _In_ double dblLaserAngle, _In_ double dblCameraAccuracyX, _In_ double dblMotionAccuracyY, _Out_ CRavidImage* pResult3D, _In_opt_ CRavidImage3DProfile* pBase = nullptr);
			
			/**
			* 아직 사용이 불가능합니다.
			*/
			virtual EAlgorithmResult CalculateDataForRotaryStage(_In_ double dblCameraAngle, _In_ double dblLaserAngle, _In_ double dblCameraAccuracy, _In_ double dblInnermostAccuracy, _In_ double dblInnermostRadius, _In_ EInnermostPosition eInnermostPosition, _Out_ CRavidImage* pResult3D, _In_opt_ CRavidImage3DProfile* pBase = nullptr);
			
			/**
			* 아직 사용이 불가능합니다.
			*/
			virtual EAlgorithmResult CalculateDataTest(_In_ double dblCameraAngle, _In_ double dblLaserAngle, _In_ double dblCameraAccuracy, _In_ double dblInnermostAccuracy, _In_ double dblInnermostRadius, _In_ EInnermostPosition eInnermostPosition, _Out_ CRavidImage* pResult3D, _In_opt_ CRavidImage3DProfile* pBase = nullptr);
			
		protected:
			size_t m_st3DSizeX = 0;
			size_t m_st3DSizeY = 0;
			
			float* m_pfPeak = nullptr;

		};
	}
}

