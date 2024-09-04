#pragma once

namespace Ravid
{
	namespace Mathematics
	{
		class CMatrix;
	}

	namespace Algorithms
	{
		class CRavidImage;
		class CInternalPerspectiveProjection;
		/**
		* 두 사각형을 원근법을 적용하여 매핑하는 클래스입니다.
		*/
		class AFX_EXT_CLASS CPerspectiveProjection : public CRavidObject
		{
			RavidUseDynamicCreation();
			RavidPreventCopySelf(CPerspectiveProjection);

		public:
			CPerspectiveProjection();
			virtual ~CPerspectiveProjection();
			
			/**
			* 내부 설정된 값을 초기화 합니다.
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Clear();
			
			/**
			* 설정된 입력 영역을 가져옵니다.
			*
			* return value : CRavidQuadrangle 의 형태로 반환됩니다.
			*/
			CRavidQuadrangle<double>* GetSourceWorld();
			
			/**
			* 입력 영역을 설정합니다.
			*
			* param[in] refSourceWorld : CRavidQuadrangle 의 형태로 설정 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetSourceWorld(_In_ CRavidQuadrangle<int>& refSourceWorld);
			
			/**
			* 입력 영역을 설정합니다.
			*
			* param[in] pSourceWorld : CRavidQuadrangle 의 형태로 설정 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetSourceWorld(_In_ CRavidQuadrangle<int>* pSourceWorld);
			
			/**
			* 입력 영역을 설정합니다.
			*
			* param[in] refSourceWorld : CRavidQuadrangle 의 형태로 설정 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetSourceWorld(_In_ CRavidQuadrangle<long long>& refSourceWorld);
			
			/**
			* 입력 영역을 설정합니다.
			*
			* param[in] pSourceWorld : CRavidQuadrangle 의 형태로 설정 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetSourceWorld(_In_ CRavidQuadrangle<long long>* pSourceWorld);
			
			/**
			* 입력 영역을 설정합니다.
			*
			* param[in] refSourceWorld : CRavidQuadrangle 의 형태로 설정 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetSourceWorld(_In_ CRavidQuadrangle<float>& refSourceWorld);
			
			/**
			* 입력 영역을 설정합니다.
			*
			* param[in] pSourceWorld : CRavidQuadrangle 의 형태로 설정 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetSourceWorld(_In_ CRavidQuadrangle<float>* pSourceWorld);
			
			/**
			* 입력 영역을 설정합니다.
			*
			* param[in] refSourceWorld : CRavidQuadrangle 의 형태로 설정 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetSourceWorld(_In_ CRavidQuadrangle<double>& refSourceWorld);
			
			/**
			* 입력 영역을 설정합니다.
			*
			* param[in] pSourceWorld : CRavidQuadrangle 의 형태로 설정 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetSourceWorld(_In_ CRavidQuadrangle<double>* pSourceWorld);
			
			/**
			* 설정된 목표 영역을 가져옵니다.
			*
			* return value : CRavidQuadrangle 의 형태로 반환됩니다.
			*/
			CRavidQuadrangle<double>* GetTargetWorld();
			
			/**
			* 목표 영역을 설정합니다.
			*
			* param[in] refTargetWorld : CRavidQuadrangle 의 형태로 설정 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetTargetWorld(_In_ CRavidQuadrangle<int>& refTargetWorld);
			
			/**
			* 목표 영역을 설정합니다.
			*
			* param[in] refTargetWorld : CRavidQuadrangle 의 형태로 설정 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetTargetWorld(_In_ CRavidQuadrangle<int>* pTargetWorld);
			
			/**
			* 목표 영역을 설정합니다.
			*
			* param[in] refTargetWorld : CRavidQuadrangle 의 형태로 설정 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetTargetWorld(_In_ CRavidQuadrangle<long long>& refTargetWorld);
			
			/**
			* 목표 영역을 설정합니다.
			*
			* param[in] refTargetWorld : CRavidQuadrangle 의 형태로 설정 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetTargetWorld(_In_ CRavidQuadrangle<long long>* pTargetWorld);
			
			/**
			* 목표 영역을 설정합니다.
			*
			* param[in] refTargetWorld : CRavidQuadrangle 의 형태로 설정 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetTargetWorld(_In_ CRavidQuadrangle<float>& refTargetWorld);
			
			/**
			* 목표 영역을 설정합니다.
			*
			* param[in] refTargetWorld : CRavidQuadrangle 의 형태로 설정 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetTargetWorld(_In_ CRavidQuadrangle<float>* pTargetWorld);
			
			/**
			* 목표 영역을 설정합니다.
			*
			* param[in] refTargetWorld : CRavidQuadrangle 의 형태로 설정 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetTargetWorld(_In_ CRavidQuadrangle<double>& refTargetWorld);
			
			/**
			* 목표 영역을 설정합니다.
			*
			* param[in] refTargetWorld : CRavidQuadrangle 의 형태로 설정 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetTargetWorld(_In_ CRavidQuadrangle<double>* pTargetWorld);
			
			/**
			* 매핑 실행 후 사용합니다.
			* 두 객체간 원근 메트릭스를 가져옵니다.
			*
			* return value : CMatrix 의 형태로 계산된 원근법 메트릭스를 반환합니다.
			*/
			Ravid::Mathematics::CMatrix GetMatrix();
			
			/**
			* 원본 영역과 목표 영역을 설정합니다.
			*
			* param[in] refSourceWorld : CRavidQuadrangle 의 형태로 설정 (Reference)
			* param[in] refTargetWorld : CRavidQuadrangle 의 형태로 설정 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Mapping(_In_ CRavidQuadrangle<int>& refSourceWorld, _In_ CRavidQuadrangle<int>& refTargetWorld);
			
			/**
			* 원본 영역과 목표 영역을 설정합니다.
			*
			* param[in] refSourceWorld : CRavidQuadrangle 의 형태로 설정 (Pointer)
			* param[in] refTargetWorld : CRavidQuadrangle 의 형태로 설정 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Mapping(_In_ CRavidQuadrangle<int>* pSourceWorld, _In_ CRavidQuadrangle<int>* pTargetWorld);
			
			/**
			* 원본 영역과 목표 영역을 설정합니다.
			*
			* param[in] refSourceWorld : CRavidQuadrangle 의 형태로 설정 (Reference)
			* param[in] refTargetWorld : CRavidQuadrangle 의 형태로 설정 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Mapping(_In_ CRavidQuadrangle<long long>& refSourceWorld, _In_ CRavidQuadrangle<long long>& refTargetWorld);
			
			/**
			* 원본 영역과 목표 영역을 설정합니다.
			*
			* param[in] refSourceWorld : CRavidQuadrangle 의 형태로 설정 (Pointer)
			* param[in] refTargetWorld : CRavidQuadrangle 의 형태로 설정 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Mapping(_In_ CRavidQuadrangle<long long>* pSourceWorld, _In_ CRavidQuadrangle<long long>* pTargetWorld);
			
			/**
			* 원본 영역과 목표 영역을 설정합니다.
			*
			* param[in] refSourceWorld : CRavidQuadrangle 의 형태로 설정 (Reference)
			* param[in] refTargetWorld : CRavidQuadrangle 의 형태로 설정 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Mapping(_In_ CRavidQuadrangle<float>& refSourceWorld, _In_ CRavidQuadrangle<float>& refTargetWorld);
			
			/**
			* 원본 영역과 목표 영역을 설정합니다.
			*
			* param[in] refSourceWorld : CRavidQuadrangle 의 형태로 설정 (Pointer)
			* param[in] refTargetWorld : CRavidQuadrangle 의 형태로 설정 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Mapping(_In_ CRavidQuadrangle<float>* pSourceWorld, _In_ CRavidQuadrangle<float>* pTargetWorld);
			
			/**
			* 원본 영역과 목표 영역을 설정합니다.
			*
			* param[in] refSourceWorld : CRavidQuadrangle 의 형태로 설정 (Reference)
			* param[in] refTargetWorld : CRavidQuadrangle 의 형태로 설정 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Mapping(_In_ CRavidQuadrangle<double>& refSourceWorld, _In_ CRavidQuadrangle<double>& refTargetWorld);
			
			/**
			* 원본 영역과 목표 영역을 설정합니다.
			*
			* param[in] refSourceWorld : CRavidQuadrangle 의 형태로 설정 (Pointer)
			* param[in] refTargetWorld : CRavidQuadrangle 의 형태로 설정 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Mapping(_In_ CRavidQuadrangle<double>* pSourceWorld, _In_ CRavidQuadrangle<double>* pTargetWorld);
			
			/**
			* 설정된 두 영역을 매핑합니다.
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Mapping();
			
			/**
			* 입력 좌표를 목표 좌표로 변환합니다.
			*
			* param[inout] refPoint : CRavidPoint 의 입출력 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertCoordinate(_Inout_ CRavidPoint<double>& refPoint);
			
			/**
			* 입력 좌표를 목표 좌표로 변환합니다.
			*
			* param[inout] pPoint : CRavidPoint 의 입출력 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertCoordinate(_Inout_ CRavidPoint<double>* pPoint);
			
			/**
			* 입력 좌표를 목표 좌표로 변환합니다.
			*
			* param[in] refSource : CRavidPoint 의 입력 (Reference)
			* param[out] refTarget : CRavidPoint 의 출력 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertCoordinate(_In_ CRavidPoint<int>& refSource, _Out_ CRavidPoint<double>& refTarget);
			
			/**
			* 입력 좌표를 목표 좌표로 변환합니다.
			*
			* param[in] pSource : CRavidPoint 의 입력 (Pointer)
			* param[out] pTarget : CRavidPoint 의 출력 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertCoordinate(_In_ CRavidPoint<int>* pSource, _Out_ CRavidPoint<double>* pTarget);
			
			/**
			* 입력 좌표를 목표 좌표로 변환합니다.
			*
			* param[inout] refPoint : CRavidPoint 의 입출력 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertCoordinate(_Inout_ CRavidPoint<long long>& refPoint);
			
			/**
			* 입력 좌표를 목표 좌표로 변환합니다.
			*
			* param[inout] pPoint : CRavidPoint 의 입출력 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertCoordinate(_Inout_ CRavidPoint<long long>* pSource);
			
			/**
			* 입력 좌표를 목표 좌표로 변환합니다.
			*
			* param[in] refSource : CRavidPoint 의 입력 (Reference)
			* param[out] refTarget : CRavidPoint 의 출력 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertCoordinate(_In_ CRavidPoint<long long>& refSource, _Out_ CRavidPoint<double>& refTarget);
			
			/**
			* 입력 좌표를 목표 좌표로 변환합니다.
			*
			* param[in] pSource : CRavidPoint 의 입력 (Pointer)
			* param[out] pTarget : CRavidPoint 의 출력 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertCoordinate(_In_ CRavidPoint<long long>* pSource, _Out_ CRavidPoint<double>* pTarget);
			
			/**
			* 입력 좌표를 목표 좌표로 변환합니다.
			*
			* param[in] refSource : CRavidPoint 의 입력 (Reference)
			* param[out] refTarget : CRavidPoint 의 출력 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertCoordinate(_In_ CRavidPoint<float>& refSource, _Out_ CRavidPoint<double>& refTarget);
			
			/**
			* 입력 좌표를 목표 좌표로 변환합니다.
			*
			* param[in] pSource : CRavidPoint 의 입력 (Pointer)
			* param[out] pTarget : CRavidPoint 의 출력 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertCoordinate(_In_ CRavidPoint<float>* pSource, _Out_ CRavidPoint<double>* pTarget);
			
			/**
			* 입력 좌표를 목표 좌표로 변환합니다.
			*
			* param[in] refSource : CRavidPoint 의 입력 (Reference)
			* param[out] refTarget : CRavidPoint 의 출력 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertCoordinate(_In_ CRavidPoint<double>& refSource, _Out_ CRavidPoint<double>& refTarget);
			
			/**
			* 입력 좌표를 목표 좌표로 변환합니다.
			*
			* param[in] pSource : CRavidPoint 의 입력 (Pointer)
			* param[out] pTarget : CRavidPoint 의 출력 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertCoordinate(_In_ CRavidPoint<double>* pSource, _Out_ CRavidPoint<double>* pTarget);
						
			/**
			* 입력 이미지를 목표 이미지로 변환합니다.
			*
			* param[in] pImgSrc : CRavidImage 의 입력 (Pointer)
			* param[out] pImgDst : CRavidImage 의 출력 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertImage(_In_ CRavidImage* pImgSrc, _Out_ CRavidImage* pImgDst);
						
			/**
			* 입력 이미지를 목표 이미지로 변환합니다.
			* 테스트 버전입니다.
			*
			* param[in] pImgSrc : CRavidImage 의 입력 (Pointer)
			* param[out] pImgDst : CRavidImage 의 출력 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertImage_(_In_ CRavidImage* pImgSrc, _Out_ CRavidImage* pImgDst);

		private:
			CInternalPerspectiveProjection* m_pInternal = nullptr;
		};
	}
}
