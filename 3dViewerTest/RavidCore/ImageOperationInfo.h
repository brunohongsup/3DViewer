#pragma once

namespace Ravid
{
	namespace Framework
	{
		class Framework::CMultipleVariable;
	}

	namespace Algorithms
	{
		class CInterImageOperationInfo;
		class CRavidImage;
		
		/**
		* 이미지 연산을 위한 연산자 클래스입니다.
		*/
		class AFX_EXT_CLASS CImageOperationInfo : public CRavidObject
		{
		public:
			/**
			* 연산 방법을 설정합니다.
			*/
			enum EOperationType
			{
				EOperationType_Add = 0,
				EOperationType_Subtract,
				EOperationType_Multiply,
				EOperationType_Divide,
				EOperationType_Modulo,
				EOperationType_ShiftLeft,
				EOperationType_ShiftRight,
				EOperationType_BitwiseAND,
				EOperationType_BitwiseOR,
				EOperationType_BitwiseXOR,
				EOperationType_LogicalAND,
				EOperationType_LogicalOR,
				EOperationType_LogicalXOR,
				EOperationType_Minimum,
				EOperationType_Maximum,
				EOperationType_Equal,
				EOperationType_NotEqual,
				EOperationType_GreaterOrEqual,
				EOperationType_LesserOrEqual,
				EOperationType_Greater,
				EOperationType_Lesser,
				EOperationType_MultiplyAdd,
			};
			/**
			* 연산 데이터를 설정합니다.
			*/
			enum EDataType
			{
				EDataType_Const = 0,	// 값으로 처리
				EDataType_Image,		// 이미지로 처리
			};

			CImageOperationInfo();
			virtual ~CImageOperationInfo();

			CImageOperationInfo(_In_ CImageOperationInfo& imgOper);
			CImageOperationInfo(_In_ CImageOperationInfo* pImgOper);
			CImageOperationInfo(_In_opt_ EOperationType eOperType, _In_opt_ EDataType eDataType, _In_ CRavidImage* pRI = nullptr, _In_ Framework::CMultipleVariable* pConst = nullptr);
			
			/**
			* 설정된 데이터를 초기화 합니다. 
			*/
			virtual void Clear();
			
			/**
			* 입력된 CImageOperationInfo를 깊은 복사합니다.
			*
			* param[in] imgOper : 메트릭스 정보 (Reference)
			*/
			void Copy(_In_ CImageOperationInfo& imgOper);
			
			/**
			* 입력된 CImageOperationInfo를 깊은 복사합니다.
			*
			* param[in] pImgOper : 메트릭스 정보 (Pointer)
			*/
			void Copy(_In_ CImageOperationInfo* pImgOper);
			
			/**
			* 입력된 CImageOperationInfo를 깊은 복사합니다.
			* eDataType(연산 데이터 설정) 에 따라 pConst 를 사용합니다.
			*
			* param[in] eOperType : 연산 방법
			* param[in] eDataType : 연산 데이터
			* param[in] pRI : 입력 이미지 (Pointer)
			* param[in] pConst : CMultipleVariable 데이터 값 (Pointer)
			*/
			void SetImageOperationInfo(_In_opt_ EOperationType eOperType, _In_opt_ EDataType eDataType, _In_ CRavidImage* pRI = nullptr, _In_ Framework::CMultipleVariable* pConst = nullptr);
			
			/**
			* 입력된 CImageOperationInfo를 깊은 복사합니다.
			* 연산 데이터를 값으로 처리 할 때 사용합니다.
			*
			* param[in] eOperType : 연산 방법
			* param[in] pConst : CMultipleVariable 데이터 값 (Pointer)
			*/
			void SetImageOperationInfo(_In_opt_ EOperationType eOperType, _In_ Framework::CMultipleVariable* pConst);
			
			/**
			* 입력된 CImageOperationInfo를 깊은 복사합니다.
			* 연산 데이터를 이미지로 처리 할 때 사용합니다.
			*
			* param[in] eOperType : 연산 방법
			* param[in] pRI : 입력 이미지 (Pointer)
			*/
			void SetImageOperationInfo(_In_opt_ EOperationType eOperType, _In_ CRavidImage* pRI);
			
			/**
			* 연산자 타입을 가져옵니다.
			*
			* return value : EOperationType 의 연산자를 반환합니다.
			*/
			EOperationType GetOperationType();
			
			/**
			* 연산자 타입을 설정합니다.
			*
			* param[in] eOperType : 연산 방법
			*/
			void SetOperationType(_In_opt_ EOperationType eOperType);
			
			/**
			* 연산 데이터를 가져옵니다.
			*
			* return value : EDataType 의 연산 데이터를 반환합니다.
			*/
			EDataType GetDataType();
			
			/**
			* 연산 데이터를 설정합니다.
			*
			* param[in] eDataType : 연산 데이터
			*/
			void SetDataType(_In_opt_ EDataType eDataType);
			
			/**
			* 연산 이미지를 가져옵니다.
			*
			* return value : CRavidImage* 의 연산 이미지를 반환합니다.
			*/
			CRavidImage* GetImage();

			/**
			* 연산 이미지를 설정합니다.
			*
			* param[in] pRI : 연산 이미지
			*/
			void SetImage(_In_ CRavidImage* pRI);
			
			/**
			* 연산 데이터 값을 가져옵니다.
			*
			* return value : CMultipleVariable* 의 연산 데이터를 반환합니다.
			*/
			Framework::CMultipleVariable* GetMultipleVariable();

			/**
			* 연산 데이터 값을 설정합니다.
			*
			* param[in] pConst : 연산 데이터 값
			*/
			void SetMultipleVariable(_In_ Framework::CMultipleVariable* pConst);

		protected:
			CInterImageOperationInfo* m_pInternal = nullptr;
			
		};
	}
}
