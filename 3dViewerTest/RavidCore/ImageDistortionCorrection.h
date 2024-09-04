#pragma once

#include "RavidImage.h"
#include <vector>

namespace Ravid
{
	namespace Algorithms
	{
		class CInternalImageDistortionCorrection;
		
		/**
		* 이미지의 왜곡 보정을 제공하는 클래스입니다.
		*/
		class AFX_EXT_CLASS CImageDistortionCorrection : public CRavidObject
		{
		public:
			CImageDistortionCorrection();
			virtual ~CImageDistortionCorrection();

			RavidPreventCopySelf(CImageDistortionCorrection);
			
			/**
			* Image Distortion 정보를 저장합니다.
			*
			* param[in] strFile : 파일 경로
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Save(_In_ std::wstring strFile);
			
			/**
			* Image Distortion 정보를 읽어옵니다.
			*
			* param[in] strFile : 파일 경로
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Load(_In_ std::wstring strFile);
			
			/**
			* 알고리즘을 실행합니다.
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Encode();
						
			/**
			* 설정된 모든 정보를 초기화 합니다.
			*/
			void Clear();

			/**
			* 이미지 위치, 실제 위치 등 정보를 설정합니다.
			* 입력 데이터 조건 (차후 개선 목록)
			* 데이터 갯수는 Row, Col 모두 홀수개여야 가능
			* 데이터의 중앙과 이미지의 중앙이 가장 가까운 데이터여야한다.
			*
			* param[in] vctImageData : 이미지내 Grid 위치 정보 (Reference)
			* param[in] vctRealData : 실제 Grid 위치 정보 (Reference)
			* param[in] nImageSizeX : 이미지 가로 크기
			* param[in] nImageSizeY : 이미지 세로 크기
			* param[in] nImageWidthStep : 이미지 세로 간격 당 가로 데이터 수
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetCorrectData(_In_ std::vector<std::vector<CRavidPoint<double>>> &vctImageData, _In_ std::vector<std::vector<CRavidPoint<double>>> &vctRealData, _In_ int nImageSizeX, _In_ int nImageSizeY, _In_ int nImageWidthStep);
			
			/**
			* 설정된 이미지 위치, 실제 위치 등 정보를 가져옵니다.
			*
			* param[out] pResImagePos : 이미지내 Grid 위치 정보 (Pointer)
			* param[out] pResRealPos : 실제 Grid 위치 정보 (Pointer)
			* param[out] pImageSizeX : 이미지 가로 크기 (Pointer)
			* param[out] pImageSizeY : 이미지 세로 크기 (Pointer)
			* param[out] pImageWidthStep : 이미지 세로 간격 당 가로 데이터 수 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetCorrectData(_Out_ std::vector<std::vector<CRavidPoint<double>>> *pResImagePos, _Out_ std::vector<std::vector<CRavidPoint<double>>> *pResRealPos, _Out_opt_ int *pImageSizeX = nullptr, _Out_opt_ int *pImageSizeY = nullptr, _Out_opt_ int *pImageWidthStep = nullptr);
			
			/**
			* 그리드 외곽 영역의 데이터 추정을 통한 보정작업 진행 여부를 설정합니다.
			* 기존 데이터들의 변화 분포를 확인하는데, 중앙으로부터 몇 개의 데이터들의 변화량은 무시할지 결정(중앙부분의 왜곡이 없을 경우 변화량이 일정하지 않고 뒤죽박죽이 되어 추정 데이터에 악영향을 끼치는 경우가 발생할 수 있음)
			* Min(0) -> 전체 활용
			* Max(DataHalfCount-1) -> 중심 데이터와 최외곽 데이터만 활용
			*
			* param[inopt] bOutsideCorrection : 그리드 외곽 영역의 데이터 추정을 통한 보정작업 진행 여부
			* param[inopt] nUsedCenterCountRowData : 기존 데이터들의 변화 분포를 확인하여 무시 데이터 Row
			* param[inopt] nUsedCenterCountColData : 기존 데이터들의 변화 분포를 확인하여 무시 데이터 Col
			*/
			void SetConjectureOutsideData(_In_opt_ bool bOutsideCorrection = true, _In_opt_ int nUsedCenterCountRowData = 0, _In_opt_ int nUsedCenterCountColData = 0);
			
			/**
			* 설정된 그리드 외곽 영역의 데이터 추정을 통한 보정작업 진행 여부를 가져옵니다.
			*
			* param[outopt] pOutsideCorrection : 그리드 외곽 영역의 데이터 추정을 통한 보정작업 진행 여부
			* param[outopt] pUsedCenterCountRowData : 기존 데이터들의 변화 분포를 확인하여 무시 데이터 Row
			* param[outopt] pUsedCenterCountColData : 기존 데이터들의 변화 분포를 확인하여 무시 데이터 Col
			*/
			void GetConjectureOutsideData(_Out_opt_ bool *pOutsideCorrection = nullptr, _Out_opt_ int *pUsedCenterCountRowData = nullptr, _Out_opt_ int *pUsedCenterCountColData = nullptr);
				
			/**
			* 캘리브레이션 적용 유무를 확인합니다.
			*
			* return value : 적용하지 않으면 false 를 반환합니다.
			*/
			bool IsValidCorrectData();

			/**
			* 입력된 이미지를 왜곡 보정합니다.
			* 이미지 왜곡 보정 (unsigned 8,16,32,64Bit, 4Channels 지원)
			*
			* param[in] pSrcImage : 입력 원본 이미지 (Pointer)
			* param[out] pDstImage : 출력 결과 이미지 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ImageCorrection(_In_ CRavidImage* pSrcImage, _Out_ CRavidImage *pDstImage);
			
			/**
			* 입력된 이미지를 왜곡 보정합니다.
			* 이미지 왜곡 보정 (unsigned 8,16,32,64Bit, 4Channels 지원)
			*
			* param[in] refSrcImage : 입력 원본 이미지 (Reference)
			* param[out] refDstImage : 출력 결과 이미지 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ImageCorrection(_In_ CRavidImage &refSrcImage, _Out_ CRavidImage &refDstImage);

			/**
			* 입력된 좌표를 보정된 좌표로 반환
			* 왜곡 정도를 계산하여 Image2Real or Real2Image 좌표를 반환
			*
			* param[in] refSrcImage : 원본 좌표 (Reference)
			* param[out] pDstGeometry : 보정 좌표 (Pointer)
			* param[inopt] bReal2Image : 실좌표를 이미지좌표 혹은 이미지 좌표를 실좌표로 선택 (Default = false)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult PointCorrection(_In_ CRavidPoint<double>& refSrcGeometry, _Out_ CRavidPoint<double>* pDstGeometry, _In_opt_ bool bReal2Image = false);

			/**
			* 입력된 좌표를 보정된 좌표로 반환
			* 왜곡 정도를 계산하여 Image2Real or Real2Image 좌표를 반환
			*
			* param[in] pSrcGeometry : 원본 좌표 (Pointer)
			* param[out] pDstGeometry : 보정 좌표 (Pointer)
			* param[inopt] bReal2Image : 실좌표를 이미지좌표 혹은 이미지 좌표를 실좌표로 선택 (Default = false)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult PointCorrection(_In_ CRavidPoint<double>* pSrcGeometry, _Out_ CRavidPoint<double>* pDstGeometry, _In_opt_ bool bReal2Image = false);

			/**
			* 입력된 좌표를 보정된 좌표로 반환
			* 왜곡 정도를 계산하여 Image2Real or Real2Image 좌표를 반환
			*
			* param[in] x : 원본 x 좌표
			* param[in] y : 원본 y 좌표
			* param[out] pDstGeometry : 보정 좌표 (Pointer)
			* param[inopt] bReal2Image : 실좌표를 이미지좌표 혹은 이미지 좌표를 실좌표로 선택 (Default = false)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult PointCorrection(_In_ double x, _In_ double y, _Out_ CRavidPoint<double>* pDstGeometry, _In_opt_ bool bReal2Image = false);

			/**
			* 입력된 좌표를 보정된 좌표로 반환
			* 왜곡 정도를 계산하여 Image2Real or Real2Image 좌표를 반환
			*
			* param[in] x : 원본 x 좌표
			* param[in] y : 원본 y 좌표
			* param[out] x : 보정 x 좌표 (Pointer)
			* param[out] y : 보정 y 좌표 (Pointer)
			* param[inopt] bReal2Image : 실좌표를 이미지좌표 혹은 이미지 좌표를 실좌표로 선택 (Default = false)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult PointCorrection(_In_ double x, _In_ double y, _Out_ double* pResX, _Out_ double* pResY, _In_opt_ bool bReal2Image = false);
						
		private:
			CInternalImageDistortionCorrection* m_pInternal = nullptr;
		};
	}
}