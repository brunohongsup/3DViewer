#pragma once

#include <vector>

namespace Ravid
{
	namespace Miscellaneous
	{
		/**
		* 파일 시스템을 컨트롤 하는 클래스입니다.
		*/
		class AFX_EXT_CLASS CFileUtility : public CRavidObject
		{
			RavidUseDynamicCreation();
			RavidPreventCopySelf(CFileUtility);

		public:
			CFileUtility();
			virtual ~CFileUtility();

			/**
			* 입력된 파일을 탐색합니다.
			* 특정 포맷을 검색하기 위해서 "*.txt" 와 같은 형태로 입력하면 모든 txt 파일 목록을 찾습니다.
			*
			* param[in] strFindingFileName : 폴더 탐색을 위한 Root 경로를 포함하는 파일명
			* param[out] pVctFoundFile : 찾은 파일 이름 리스트 (Pointer)
			* param[in_opt] bRecursive : 재귀적 하위 폴더 탐색을 설정
			*
			* return value : bool 의 실행 결과를 반환합니다.
			*/
			bool FindFile(_In_ CString strFindingFileName, _Out_ std::vector<CString>* pVctFoundFile, _In_opt_ bool bRecursive = true);
			
			/**
			* 해당 폴더명이 지정된 위치에 존재하는지 확인합니다.
			*
			* param[in] strFolderName : 폴더명
			*
			* return value : bool 의 실행 결과를 반환합니다.
			*/
			bool IsFolderExist(_In_ CString strFolderName);
			
			/**
			* 입력된 폴더까지 하위로 탐색하며 없으면 모두 생성합니다.
			* 재귀적으로 폴더를 생성합니다.
			*
			* param[in] strFolderName : 폴더명
			*
			* return value : bool 의 실행 결과를 반환합니다.
			*/
			bool CreateFolder(_In_ CString strFolderName);
			
			/**
			* 폴더를 입력하면 재귀적으로 하위 파일 및 폴더를 삭제합니다.
			* 파일을 입력하면 해당 파일만 삭제합니다.
			*
			* param[in] strFile : 폴더명 또는 파일명
			*
			* return value : bool 의 실행 결과를 반환합니다.
			*/
			bool DeleteFile(_In_ CString strFile);
			
			/**
			* 원본 폴더를 입력하고, 상대 폴더를 입력하면 재귀적으로 하위 파일 및 폴더를 복사(생성)합니다.
			* 원본 파일을 입력하면, 상대 폴더를 입력하면 상대 폴더까지 생성하여 복사(생성)합니다.
			*
			* param[in] strSrcFile : 복사 할 폴더명 또는 파일명
			* param[in] strDstFile : 붙여넣을 폴더명 또는 파일명
			*
			* return value : bool 의 실행 결과를 반환합니다.
			*/
			bool CopyFile(_In_ CString strSrcFile, _In_ CString strDstFile);
		};
	}
}

