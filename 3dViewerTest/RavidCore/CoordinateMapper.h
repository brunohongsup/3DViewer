#pragma once
#include <vector>

namespace Ravid
{
	namespace Algorithms
	{
		struct SCMPoint
		{
			double x;
			double y;
		};

		struct SCMLine
		{
			SCMPoint cmPoint[2];
		};

#define CMP_COPY(_a, _b) _a.x = _b.x; _a.y = _b.y
#define CMP_PLUSCOPY(_a, _b) CMP_PLUS(_a, _a, _b)

#define CMP_PLUS(_a, _b, _c) _a.x = _b.x + _c.x; _a.y = _b.y + _c.y
#define CMP_MINUS(_a, _b, _c) _a.x = _b.x - _c.x; _a.y = _b.y - _c.y
#define CMP_MULTI(_a, _b, _c) _a.x = _b.x * _c; _a.y = _b.y * _c

		class CInternalCoordinateMapper;
		
		/**
		* 두 사각형을 매핑하여 보간하는 클래스입니다.
		*/
		class AFX_EXT_CLASS CCoordinateMapper : public CRavidObject
		{
			RavidUseDynamicCreation();
			RavidPreventCopySelf(CCoordinateMapper);

		public:
			CCoordinateMapper();
			virtual ~CCoordinateMapper();
			
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
			* 입력 좌표를 목표 좌표로 변환합니다.
			*
			* param[inout] refPoint : CRavidPoint 의 입출력 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Convert(_Inout_ CRavidPoint<double>& refPoint);
			
			/**
			* 입력 좌표를 목표 좌표로 변환합니다.
			*
			* param[inout] pPoint : CRavidPoint 의 입출력 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Convert(_Inout_ CRavidPoint<double>* pPoint);
			
			/**
			* 입력 좌표를 목표 좌표로 변환합니다.
			*
			* param[in] refSource : CRavidPoint 의 입력 (Reference)
			* param[out] refTarget : CRavidPoint 의 출력 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Convert(_In_ CRavidPoint<int>& refSource, _Out_ CRavidPoint<double>& refTarget);
			
			/**
			* 입력 좌표를 목표 좌표로 변환합니다.
			*
			* param[in] pSource : CRavidPoint 의 입력 (Pointer)
			* param[out] pTarget : CRavidPoint 의 출력 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Convert(_In_ CRavidPoint<int>* pSource, _Out_ CRavidPoint<double>* pTarget);
			
			/**
			* 입력 좌표를 목표 좌표로 변환합니다.
			*
			* param[inout] refPoint : CRavidPoint 의 입출력 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Convert(_Inout_ CRavidPoint<long long>& refSource);
			
			/**
			* 입력 좌표를 목표 좌표로 변환합니다.
			*
			* param[inout] pPoint : CRavidPoint 의 입출력 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Convert(_Inout_ CRavidPoint<long long>* pSource);
			
			/**
			* 입력 좌표를 목표 좌표로 변환합니다.
			*
			* param[in] refSource : CRavidPoint 의 입력 (Reference)
			* param[out] refTarget : CRavidPoint 의 출력 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Convert(_In_ CRavidPoint<long long>& refSource, _Out_ CRavidPoint<double>& refTarget);
			
			/**
			* 입력 좌표를 목표 좌표로 변환합니다.
			*
			* param[in] pSource : CRavidPoint 의 입력 (Pointer)
			* param[out] pTarget : CRavidPoint 의 출력 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Convert(_In_ CRavidPoint<long long>* pSource, _Out_ CRavidPoint<double>* pTarget);
			
			/**
			* 입력 좌표를 목표 좌표로 변환합니다.
			*
			* param[in] refSource : CRavidPoint 의 입력 (Reference)
			* param[out] refTarget : CRavidPoint 의 출력 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Convert(_In_ CRavidPoint<float>& refSource, _Out_ CRavidPoint<double>& refTarget);
			
			/**
			* 입력 좌표를 목표 좌표로 변환합니다.
			*
			* param[in] pSource : CRavidPoint 의 입력 (Pointer)
			* param[out] pTarget : CRavidPoint 의 출력 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Convert(_In_ CRavidPoint<float>* pSource, _Out_ CRavidPoint<double>* pTarget);
			
			/**
			* 입력 좌표를 목표 좌표로 변환합니다.
			*
			* param[in] refSource : CRavidPoint 의 입력 (Reference)
			* param[out] refTarget : CRavidPoint 의 출력 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Convert(_In_ CRavidPoint<double>& refSource, _Out_ CRavidPoint<double>& refTarget);
			
			/**
			* 입력 좌표를 목표 좌표로 변환합니다.
			*
			* param[in] pSource : CRavidPoint 의 입력 (Pointer)
			* param[out] pTarget : CRavidPoint 의 출력 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Convert(_In_ CRavidPoint<double>* pSource, _Out_ CRavidPoint<double>* pTarget);			
			
			/**
			* 입력 좌표 리스트를 목표 좌표리스트로 변환합니다.
			*
			* param[in] pVctSource : CRavidPoint 의 벡터로 입력 (Pointer)
			* param[out] pVctTarget : CRavidPoint 의 벡터로 출력 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Convert(std::vector<CRavidPoint<double>>* pVctSource, std::vector<CRavidPoint<double>>* pVctTarget);
			
			/**
			* 입력 좌표 리스트를 목표 좌표리스트로 변환합니다.
			*
			* param[in] dblSrcX : double 의 x축 입력
			* param[in] dblSrcY : double 의 y축 입력
			* param[out] dblDstX : double 의 x축 출력 (Pointer)
			* param[out] dblDstY : double 의 y축 출력 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Convert(double dblSrcX, double dblSrcY, double *dblDstX, double *dblDstY);

		private:

			CInternalCoordinateMapper* m_pInternal = nullptr;
		};
	}
}
