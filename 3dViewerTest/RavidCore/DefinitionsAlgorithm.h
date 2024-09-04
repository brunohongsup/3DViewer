#pragma once

#include "MultipleVariable.h"

enum ELogicalCondition
{
	ELogicalCondition_Greater = 0,
	ELogicalCondition_GreaterEqual,
	ELogicalCondition_Equal,
	ELogicalCondition_NotEqual,
	ELogicalCondition_LessEqual,
	ELogicalCondition_Less,
};

enum EImageDepthRange
{
	EImageDepthRange_Min = -9999,
	EImageDepthRange_Mid = -9998,
	EImageDepthRange_Max = -9997,
};

namespace Ravid
{
	namespace Algorithms
	{
		enum EAlgorithmResult
		{
			EAlgorithmResult_OK = 0x00,
			EAlgorithmResult_UnknownError,
			EAlgorithmResult_NullPointer,
			EAlgorithmResult_OutOfRange,
			EAlgorithmResult_DivisionByZero,
			EAlgorithmResult_NotEnoughMemory,
			EAlgorithmResult_DifferentType,
			EAlgorithmResult_DifferentDepth,
			EAlgorithmResult_NoResult,
			EAlgorithmResult_EmptyElement,
			EAlgorithmResult_ExceededLength,
			EAlgorithmResult_InvalidParameter,
			EAlgorithmResult_UnsupportedRegionOfInterest,
			EAlgorithmResult_FailedToProcess,
			EAlgorithmResult_LearnedImageDoesNotExist,
			EAlgorithmResult_InvalidObject,
			EAlgorithmResult_BufferDoesNotExist,
			EAlgorithmResult_UnsupportedChannels,
			EAlgorithmResult_FailedToOpen,
			EAlgorithmResult_FailedToRead,
			EAlgorithmResult_FailedToWrite,
			EAlgorithmResult_InvalidFile,
			EAlgorithmResult_UnsupportedVersion,
			EAlgorithmResult_DifferentChannel,
			EAlgorithmResult_UnsupportedDepth,
			EAlgorithmResult_DifferentValueFormat,
			EAlgorithmResult_UnsupportedValueFormat,
			EAlgorithmResult_ErrorRegionOfInterest,
			EAlgorithmResult_Count,
		};

		namespace Comparators
		{
			// Comparison1 //

			// 1 Channel

			struct SComparison_Base_1
			{
				int m_nTarget1st;

				void SetTarget(const Framework::CMultipleVariable &rmv)
				{
					m_nTarget1st = static_cast<int>(rmv[0]);
				}
			};

			struct SComparison_Less_1_BYTE : public SComparison_Base_1
			{
				bool operator()(BYTE *pData)
				{
					return *pData < m_nTarget1st;
				}
			};

			struct SComparison_Greater_1_BYTE : public SComparison_Base_1
			{
				bool operator()(BYTE *pData)
				{
					return *pData > m_nTarget1st;
				}
			};

			struct SComparison_LessEqual_1_BYTE : public SComparison_Base_1
			{
				bool operator()(BYTE *pData)
				{
					return *pData <= m_nTarget1st;
				}
			};

			struct SComparison_GreaterEqual_1_BYTE : public SComparison_Base_1
			{
				bool operator()(BYTE *pData)
				{
					return *pData >= m_nTarget1st;
				}
			};

			struct SComparison_Equal_1_BYTE : public SComparison_Base_1
			{
				bool operator()(BYTE *pData)
				{
					return *pData == m_nTarget1st;
				}
			};

			struct SComparison_NotEqual_1_BYTE : public SComparison_Base_1
			{
				bool operator()(BYTE *pData)
				{
					return *pData != m_nTarget1st;
				}
			};

			struct SComparison_Less_1_WORD : public SComparison_Base_1
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData < m_nTarget1st;
				}
			};

			struct SComparison_Greater_1_WORD : public SComparison_Base_1
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData > m_nTarget1st;
				}
			};

			struct SComparison_LessEqual_1_WORD : public SComparison_Base_1
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData <= m_nTarget1st;
				}
			};

			struct SComparison_GreaterEqual_1_WORD : public SComparison_Base_1
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData >= m_nTarget1st;
				}
			};

			struct SComparison_Equal_1_WORD : public SComparison_Base_1
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData == m_nTarget1st;
				}
			};

			struct SComparison_NotEqual_1_WORD : public SComparison_Base_1
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData != m_nTarget1st;
				}
			};

			// 2 Channel

			struct SComparison_Base_2
			{
				int m_nTarget1st;
				int m_nTarget2nd;

				void SetTarget(const Framework::CMultipleVariable &rmv)
				{
					m_nTarget1st = static_cast<int>(rmv[0]);
					m_nTarget2nd = static_cast<int>(rmv[1]);
				}
			};

			struct SComparison_Less_2_OR_BYTE : public SComparison_Base_2
			{
				bool operator()(BYTE *pData)
				{
					return *pData < m_nTarget1st || *(pData + 1) < m_nTarget2nd;
				}
			};

			struct SComparison_Greater_2_OR_BYTE : public SComparison_Base_2
			{
				bool operator()(BYTE *pData)
				{
					return *pData > m_nTarget1st || *(pData + 1) > m_nTarget2nd;
				}
			};

			struct SComparison_LessEqual_2_OR_BYTE : public SComparison_Base_2
			{
				bool operator()(BYTE *pData)
				{
					return *pData <= m_nTarget1st || *(pData + 1) <= m_nTarget2nd;
				}
			};

			struct SComparison_GreaterEqual_2_OR_BYTE : public SComparison_Base_2
			{
				bool operator()(BYTE * pData)
				{
					return *pData >= m_nTarget1st || *(pData + 1) >= m_nTarget2nd;
				}
			};

			struct SComparison_Equal_2_OR_BYTE : public SComparison_Base_2
			{
				bool operator()(BYTE *pData)
				{
					return *pData == m_nTarget1st || *(pData + 1) == m_nTarget2nd;
				}
			};

			struct SComparison_NotEqual_2_OR_BYTE : public SComparison_Base_2
			{
				bool operator()(BYTE *pData)
				{
					return *pData != m_nTarget1st || *(pData + 1) != m_nTarget2nd;
				}
			};

			struct SComparison_Less_2_AND_BYTE : public SComparison_Base_2
			{
				bool operator()(BYTE *pData)
				{
					return *pData < m_nTarget1st && *(pData + 1) < m_nTarget2nd;
				}
			};

			struct SComparison_Greater_2_AND_BYTE : public SComparison_Base_2
			{
				bool operator()(BYTE *pData)
				{
					return *pData > m_nTarget1st && *(pData + 1) > m_nTarget2nd;
				}
			};

			struct SComparison_LessEqual_2_AND_BYTE : public SComparison_Base_2
			{
				bool operator()(BYTE *pData)
				{
					return *pData <= m_nTarget1st && *(pData + 1) <= m_nTarget2nd;
				}
			};

			struct SComparison_GreaterEqual_2_AND_BYTE : public SComparison_Base_2
			{
				bool operator()(BYTE *pData)
				{
					return *pData >= m_nTarget1st && *(pData + 1) >= m_nTarget2nd;
				}
			};

			struct SComparison_Equal_2_AND_BYTE : public SComparison_Base_2
			{
				bool operator()(BYTE *pData)
				{
					return *pData == m_nTarget1st && *(pData + 1) == m_nTarget2nd;
				}
			};

			struct SComparison_NotEqual_2_AND_BYTE : public SComparison_Base_2
			{
				bool operator()(BYTE *pData)
				{
					return *pData != m_nTarget1st && *(pData + 1) != m_nTarget2nd;
				}
			};

			struct SComparison_Less_2_OR_WORD : public SComparison_Base_2
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData < m_nTarget1st || *((WORD*)pData + 1) < m_nTarget2nd;
				}
			};

			struct SComparison_Greater_2_OR_WORD : public SComparison_Base_2
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData > m_nTarget1st || *((WORD*)pData + 1) > m_nTarget2nd;
				}
			};

			struct SComparison_LessEqual_2_OR_WORD : public SComparison_Base_2
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData <= m_nTarget1st || *((WORD*)pData + 1) <= m_nTarget2nd;
				}
			};

			struct SComparison_GreaterEqual_2_OR_WORD : public SComparison_Base_2
			{
				bool operator()(BYTE * pData)
				{
					return *(WORD*)pData >= m_nTarget1st || *((WORD*)pData + 1) >= m_nTarget2nd;
				}
			};

			struct SComparison_Equal_2_OR_WORD : public SComparison_Base_2
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData == m_nTarget1st || *((WORD*)pData + 1) == m_nTarget2nd;
				}
			};

			struct SComparison_NotEqual_2_OR_WORD : public SComparison_Base_2
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData != m_nTarget1st || *((WORD*)pData + 1) != m_nTarget2nd;
				}
			};

			struct SComparison_Less_2_AND_WORD : public SComparison_Base_2
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData < m_nTarget1st && *((WORD*)pData + 1) < m_nTarget2nd;
				}
			};

			struct SComparison_Greater_2_AND_WORD : public SComparison_Base_2
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData > m_nTarget1st && *((WORD*)pData + 1) > m_nTarget2nd;
				}
			};

			struct SComparison_LessEqual_2_AND_WORD : public SComparison_Base_2
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData <= m_nTarget1st && *((WORD*)pData + 1) <= m_nTarget2nd;
				}
			};

			struct SComparison_GreaterEqual_2_AND_WORD : public SComparison_Base_2
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData >= m_nTarget1st && *((WORD*)pData + 1) >= m_nTarget2nd;
				}
			};

			struct SComparison_Equal_2_AND_WORD : public SComparison_Base_2
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData == m_nTarget1st && *((WORD*)pData + 1) == m_nTarget2nd;
				}
			};

			struct SComparison_NotEqual_2_AND_WORD : public SComparison_Base_2
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData != m_nTarget1st && *((WORD*)pData + 1) != m_nTarget2nd;
				}
			};


			// 3 Channel

			struct SComparison_Base_3
			{
				int m_nTarget1st;
				int m_nTarget2nd;
				int m_nTarget3rd;

				void SetTarget(const Framework::CMultipleVariable &rmv)
				{
					m_nTarget1st = static_cast<int>(rmv[0]);
					m_nTarget2nd = static_cast<int>(rmv[1]);
					m_nTarget3rd = static_cast<int>(rmv[2]);
				}
			};

			struct SComparison_Less_3_OR_BYTE : public SComparison_Base_3
			{
				bool operator()(BYTE *pData)
				{
					return *pData < m_nTarget1st || *(pData + 1) < m_nTarget2nd || *(pData + 2) < m_nTarget3rd;
				}
			};

			struct SComparison_Greater_3_OR_BYTE : public SComparison_Base_3
			{
				bool operator()(BYTE *pData)
				{
					return *pData > m_nTarget1st || *(pData + 1) > m_nTarget2nd || *(pData + 2) > m_nTarget3rd;
				}
			};

			struct SComparison_LessEqual_3_OR_BYTE : public SComparison_Base_3
			{
				bool operator()(BYTE *pData)
				{
					return *pData <= m_nTarget1st || *(pData + 1) <= m_nTarget2nd || *(pData + 2) <= m_nTarget3rd;
				}
			};

			struct SComparison_GreaterEqual_3_OR_BYTE : public SComparison_Base_3
			{
				bool operator()(BYTE * pData)
				{
					return *pData >= m_nTarget1st || *(pData + 1) >= m_nTarget2nd || *(pData + 2) >= m_nTarget3rd;
				}
			};

			struct SComparison_Equal_3_OR_BYTE : public SComparison_Base_3
			{
				bool operator()(BYTE *pData)
				{
					return *pData == m_nTarget1st || *(pData + 1) == m_nTarget2nd || *(pData + 2) == m_nTarget3rd;
				}
			};

			struct SComparison_NotEqual_3_OR_BYTE : public SComparison_Base_3
			{
				bool operator()(BYTE *pData)
				{
					return *pData != m_nTarget1st || *(pData + 1) != m_nTarget2nd || *(pData + 2) != m_nTarget3rd;
				}
			};

			struct SComparison_Less_3_AND_BYTE : public SComparison_Base_3
			{
				bool operator()(BYTE *pData)
				{
					return *pData < m_nTarget1st && *(pData + 1) < m_nTarget2nd && *(pData + 2) < m_nTarget3rd;
				}
			};

			struct SComparison_Greater_3_AND_BYTE : public SComparison_Base_3
			{
				bool operator()(BYTE *pData)
				{
					return *pData > m_nTarget1st && *(pData + 1) > m_nTarget2nd && *(pData + 2) > m_nTarget3rd;
				}
			};

			struct SComparison_LessEqual_3_AND_BYTE : public SComparison_Base_3
			{
				bool operator()(BYTE *pData)
				{
					return *pData <= m_nTarget1st && *(pData + 1) <= m_nTarget2nd && *(pData + 2) <= m_nTarget3rd;
				}
			};

			struct SComparison_GreaterEqual_3_AND_BYTE : public SComparison_Base_3
			{
				bool operator()(BYTE *pData)
				{
					return *pData >= m_nTarget1st && *(pData + 1) >= m_nTarget2nd && *(pData + 2) >= m_nTarget3rd;
				}
			};

			struct SComparison_Equal_3_AND_BYTE : public SComparison_Base_3
			{
				bool operator()(BYTE *pData)
				{
					return *pData == m_nTarget1st && *(pData + 1) == m_nTarget2nd && *(pData + 2) == m_nTarget3rd;
				}
			};

			struct SComparison_NotEqual_3_AND_BYTE : public SComparison_Base_3
			{
				bool operator()(BYTE *pData)
				{
					return *pData != m_nTarget1st && *(pData + 1) != m_nTarget2nd && *(pData + 2) != m_nTarget3rd;
				}
			};

			struct SComparison_Less_3_OR_WORD : public SComparison_Base_3
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData < m_nTarget1st || *((WORD*)pData + 1) < m_nTarget2nd || *((WORD*)pData + 2) < m_nTarget3rd;
				}
			};

			struct SComparison_Greater_3_OR_WORD : public SComparison_Base_3
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData > m_nTarget1st || *((WORD*)pData + 1) > m_nTarget2nd || *((WORD*)pData + 2) > m_nTarget3rd;
				}
			};

			struct SComparison_LessEqual_3_OR_WORD : public SComparison_Base_3
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData <= m_nTarget1st || *((WORD*)pData + 1) <= m_nTarget2nd || *((WORD*)pData + 2) <= m_nTarget3rd;
				}
			};

			struct SComparison_GreaterEqual_3_OR_WORD : public SComparison_Base_3
			{
				bool operator()(BYTE * pData)
				{
					return *(WORD*)pData >= m_nTarget1st || *((WORD*)pData + 1) >= m_nTarget2nd || *((WORD*)pData + 2) >= m_nTarget3rd;
				}
			};

			struct SComparison_Equal_3_OR_WORD : public SComparison_Base_3
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData == m_nTarget1st || *((WORD*)pData + 1) == m_nTarget2nd || *((WORD*)pData + 2) == m_nTarget3rd;
				}
			};

			struct SComparison_NotEqual_3_OR_WORD : public SComparison_Base_3
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData != m_nTarget1st || *((WORD*)pData + 1) != m_nTarget2nd || *((WORD*)pData + 2) != m_nTarget3rd;
				}
			};

			struct SComparison_Less_3_AND_WORD : public SComparison_Base_3
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData < m_nTarget1st && *((WORD*)pData + 1) < m_nTarget2nd && *((WORD*)pData + 2) < m_nTarget3rd;
				}
			};

			struct SComparison_Greater_3_AND_WORD : public SComparison_Base_3
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData > m_nTarget1st && *((WORD*)pData + 1) > m_nTarget2nd && *((WORD*)pData + 2) > m_nTarget3rd;
				}
			};

			struct SComparison_LessEqual_3_AND_WORD : public SComparison_Base_3
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData <= m_nTarget1st && *((WORD*)pData + 1) <= m_nTarget2nd && *((WORD*)pData + 2) <= m_nTarget3rd;
				}
			};

			struct SComparison_GreaterEqual_3_AND_WORD : public SComparison_Base_3
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData >= m_nTarget1st && *((WORD*)pData + 1) >= m_nTarget2nd && *((WORD*)pData + 2) >= m_nTarget3rd;
				}
			};

			struct SComparison_Equal_3_AND_WORD : public SComparison_Base_3
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData == m_nTarget1st && *((WORD*)pData + 1) == m_nTarget2nd && *((WORD*)pData + 2) == m_nTarget3rd;
				}
			};

			struct SComparison_NotEqual_3_AND_WORD : public SComparison_Base_3
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData != m_nTarget1st && *((WORD*)pData + 1) != m_nTarget2nd && *((WORD*)pData + 2) != m_nTarget3rd;
				}
			};

			// 4 Channel

			struct SComparison_Base_4
			{
				int m_nTarget1st;
				int m_nTarget2nd;
				int m_nTarget3rd;
				int m_nTarget4th;

				void SetTarget(const Framework::CMultipleVariable &rmv)
				{
					m_nTarget1st = static_cast<int>(rmv[0]);
					m_nTarget2nd = static_cast<int>(rmv[1]);
					m_nTarget3rd = static_cast<int>(rmv[2]);
					m_nTarget4th = static_cast<int>(rmv[3]);
				}
			};

			struct SComparison_Less_4_OR_BYTE : public SComparison_Base_4
			{
				bool operator()(BYTE *pData)
				{
					return *pData < m_nTarget1st || *(pData + 1) < m_nTarget2nd || *(pData + 2) < m_nTarget3rd || *(pData + 3) < m_nTarget4th;
				}
			};

			struct SComparison_Greater_4_OR_BYTE : public SComparison_Base_4
			{
				bool operator()(BYTE *pData)
				{
					return *pData > m_nTarget1st || *(pData + 1) > m_nTarget2nd || *(pData + 2) > m_nTarget3rd || *(pData + 3) > m_nTarget4th;
				}
			};

			struct SComparison_LessEqual_4_OR_BYTE : public SComparison_Base_4
			{
				bool operator()(BYTE *pData)
				{
					return *pData <= m_nTarget1st || *(pData + 1) <= m_nTarget2nd || *(pData + 2) <= m_nTarget3rd || *(pData + 3) <= m_nTarget4th;
				}
			};

			struct SComparison_GreaterEqual_4_OR_BYTE : public SComparison_Base_4
			{
				bool operator()(BYTE *pData)
				{
					return *pData >= m_nTarget1st || *(pData + 1) >= m_nTarget2nd || *(pData + 2) >= m_nTarget3rd || *(pData + 3) >= m_nTarget4th;
				}
			};

			struct SComparison_Equal_4_OR_BYTE : public SComparison_Base_4
			{
				bool operator()(BYTE *pData)
				{
					return *pData == m_nTarget1st || *(pData + 1) == m_nTarget2nd || *(pData + 2) == m_nTarget3rd || *(pData + 3) == m_nTarget4th;
				}
			};

			struct SComparison_NotEqual_4_OR_BYTE : public SComparison_Base_4
			{
				bool operator()(BYTE *pData)
				{
					return *pData != m_nTarget1st || *(pData + 1) != m_nTarget2nd || *(pData + 2) != m_nTarget3rd || *(pData + 3) != m_nTarget4th;
				}
			};

			struct SComparison_Less_4_AND_BYTE : public SComparison_Base_4
			{
				bool operator()(BYTE *pData)
				{
					return *pData < m_nTarget1st && *(pData + 1) < m_nTarget2nd && *(pData + 2) < m_nTarget3rd && *(pData + 3) < m_nTarget4th;
				}
			};

			struct SComparison_Greater_4_AND_BYTE : public SComparison_Base_4
			{
				bool operator()(BYTE *pData)
				{
					return *pData > m_nTarget1st && *(pData + 1) > m_nTarget2nd && *(pData + 2) > m_nTarget3rd && *(pData + 3) > m_nTarget4th;
				}
			};

			struct SComparison_LessEqual_4_AND_BYTE : public SComparison_Base_4
			{
				bool operator()(BYTE *pData)
				{
					return *pData <= m_nTarget1st && *(pData + 1) <= m_nTarget2nd && *(pData + 2) <= m_nTarget3rd && *(pData + 3) <= m_nTarget4th;
				}
			};

			struct SComparison_GreaterEqual_4_AND_BYTE : public SComparison_Base_4
			{
				bool operator()(BYTE *pData)
				{
					return *pData >= m_nTarget1st && *(pData + 1) >= m_nTarget2nd && *(pData + 2) >= m_nTarget3rd && *(pData + 3) >= m_nTarget4th;
				}
			};

			struct SComparison_Equal_4_AND_BYTE : public SComparison_Base_4
			{
				bool operator()(BYTE *pData)
				{
					return *pData == m_nTarget1st && *(pData + 1) == m_nTarget2nd && *(pData + 2) == m_nTarget3rd && *(pData + 3) == m_nTarget4th;
				}
			};

			struct SComparison_NotEqual_4_AND_BYTE : public SComparison_Base_4
			{
				bool operator()(BYTE *pData)
				{
					return *pData != m_nTarget1st && *(pData + 1) != m_nTarget2nd && *(pData + 2) != m_nTarget3rd && *(pData + 3) != m_nTarget4th;
				}
			};

			struct SComparison_Less_4_OR_WORD : public SComparison_Base_4
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData < m_nTarget1st || *((WORD*)pData + 1) < m_nTarget2nd || *((WORD*)pData + 2) < m_nTarget3rd || *((WORD*)pData + 3) < m_nTarget4th;
				}
			};

			struct SComparison_Greater_4_OR_WORD : public SComparison_Base_4
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData > m_nTarget1st || *((WORD*)pData + 1) > m_nTarget2nd || *((WORD*)pData + 2) > m_nTarget3rd || *((WORD*)pData + 3) > m_nTarget4th;
				}
			};

			struct SComparison_LessEqual_4_OR_WORD : public SComparison_Base_4
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData <= m_nTarget1st || *((WORD*)pData + 1) <= m_nTarget2nd || *((WORD*)pData + 2) <= m_nTarget3rd || *((WORD*)pData + 3) <= m_nTarget4th;
				}
			};

			struct SComparison_GreaterEqual_4_OR_WORD : public SComparison_Base_4
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData >= m_nTarget1st || *((WORD*)pData + 1) >= m_nTarget2nd || *((WORD*)pData + 2) >= m_nTarget3rd || *((WORD*)pData + 3) >= m_nTarget4th;
				}
			};

			struct SComparison_Equal_4_OR_WORD : public SComparison_Base_4
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData == m_nTarget1st || *((WORD*)pData + 1) == m_nTarget2nd || *((WORD*)pData + 2) == m_nTarget3rd || *((WORD*)pData + 3) == m_nTarget4th;
				}
			};

			struct SComparison_NotEqual_4_OR_WORD : public SComparison_Base_4
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData != m_nTarget1st || *((WORD*)pData + 1) != m_nTarget2nd || *((WORD*)pData + 2) != m_nTarget3rd || *((WORD*)pData + 3) != m_nTarget4th;
				}
			};

			struct SComparison_Less_4_AND_WORD : public SComparison_Base_4
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData < m_nTarget1st && *((WORD*)pData + 1) < m_nTarget2nd && *((WORD*)pData + 2) < m_nTarget3rd && *((WORD*)pData + 3) < m_nTarget4th;
				}
			};

			struct SComparison_Greater_4_AND_WORD : public SComparison_Base_4
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData > m_nTarget1st && *((WORD*)pData + 1) > m_nTarget2nd && *((WORD*)pData + 2) > m_nTarget3rd && *((WORD*)pData + 3) > m_nTarget4th;
				}
			};

			struct SComparison_LessEqual_4_AND_WORD : public SComparison_Base_4
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData <= m_nTarget1st && *((WORD*)pData + 1) <= m_nTarget2nd && *((WORD*)pData + 2) <= m_nTarget3rd && *((WORD*)pData + 3) <= m_nTarget4th;
				}
			};

			struct SComparison_GreaterEqual_4_AND_WORD : public SComparison_Base_4
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData >= m_nTarget1st && *((WORD*)pData + 1) >= m_nTarget2nd && *((WORD*)pData + 2) >= m_nTarget3rd && *((WORD*)pData + 3) >= m_nTarget4th;
				}
			};

			struct SComparison_Equal_4_AND_WORD : public SComparison_Base_4
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData == m_nTarget1st && *((WORD*)pData + 1) == m_nTarget2nd && *((WORD*)pData + 2) == m_nTarget3rd && *((WORD*)pData + 3) == m_nTarget4th;
				}
			};

			struct SComparison_NotEqual_4_AND_WORD : public SComparison_Base_4
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData != m_nTarget1st && *((WORD*)pData + 1) != m_nTarget2nd && *((WORD*)pData + 2) != m_nTarget3rd && *((WORD*)pData + 3) != m_nTarget4th;
				}
			};

			// Comparison2 //

			// 1 Channel

			struct SComparison2_Base_1
			{
				int m_nTarget1stMin;
				int m_nTarget1stMax;

				void SetTarget(const Framework::CMultipleVariable &rmv)
				{
					m_nTarget1stMin = static_cast<int>(rmv[0]);
					m_nTarget1stMax = static_cast<int>(rmv[1]);
				}
			};

			struct SComparison2_Equal_1_BYTE : public SComparison2_Base_1
			{
				bool operator()(BYTE *pData)
				{
					return *pData >= m_nTarget1stMin && *pData <= m_nTarget1stMax;
				}
			};

			struct SComparison2_NotEqual_1_BYTE : public SComparison2_Base_1
			{
				bool operator()(BYTE *pData)
				{
					return *pData < m_nTarget1stMin || *pData > m_nTarget1stMax;
				}
			};

			struct SComparison2_Equal_1_WORD : public SComparison2_Base_1
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData >= m_nTarget1stMin && *(WORD*)pData <= m_nTarget1stMax;
				}
			};

			struct SComparison2_NotEqual_1_WORD : public SComparison2_Base_1
			{
				bool operator()(BYTE *pData)
				{
					return *(WORD*)pData < m_nTarget1stMin || *(WORD*)pData > m_nTarget1stMax;
				}
			};

			// 2 Channel

			struct SComparison2_Base_2
			{
				int m_nTarget1stMin;
				int m_nTarget1stMax;
				int m_nTarget2ndMin;
				int m_nTarget2ndMax;

				void SetTarget(const Framework::CMultipleVariable &rmv)
				{
					m_nTarget1stMin = static_cast<int>(rmv[0]);
					m_nTarget2ndMin = static_cast<int>(rmv[1]);

					m_nTarget1stMax = static_cast<int>(rmv[2]);
					m_nTarget2ndMax = static_cast<int>(rmv[3]);
				}
			};

			struct SComparison2_Equal_2_OR_BYTE : public SComparison2_Base_2
			{
				bool operator()(BYTE *pData)
				{
					return (*pData >= m_nTarget1stMin && *pData <= m_nTarget1stMax) ||
						(*(pData + 1) >= m_nTarget2ndMin && *(pData + 1) <= m_nTarget2ndMax);
				}
			};

			struct SComparison2_NotEqual_2_OR_BYTE : public SComparison2_Base_2
			{
				bool operator()(BYTE *pData)
				{
					return (*pData < m_nTarget1stMin || *pData > m_nTarget1stMax) ||
						(*(pData + 1) < m_nTarget2ndMin || *(pData + 1) > m_nTarget2ndMax);
				}
			};

			struct SComparison2_Equal_2_AND_BYTE : public SComparison2_Base_2
			{
				bool operator()(BYTE *pData)
				{
					return (*pData >= m_nTarget1stMin && *pData <= m_nTarget1stMax) &&
						(*(pData + 1) >= m_nTarget2ndMin && *(pData + 1) <= m_nTarget2ndMax);
				}
			};

			struct SComparison2_NotEqual_2_AND_BYTE : public SComparison2_Base_2
			{
				bool operator()(BYTE *pData)
				{
					return (*pData < m_nTarget1stMin || *pData > m_nTarget1stMax) &&
						(*(pData + 1) < m_nTarget2ndMin || *(pData + 1) > m_nTarget2ndMax);
				}
			};

			struct SComparison2_Equal_2_OR_WORD : public SComparison2_Base_2
			{
				bool operator()(BYTE *pData)
				{
					return (*(WORD*)pData >= m_nTarget1stMin && *(WORD*)pData <= m_nTarget1stMax) ||
						(*((WORD*)pData + 1) >= m_nTarget2ndMin && *((WORD*)pData + 1) <= m_nTarget2ndMax);
				}
			};

			struct SComparison2_NotEqual_2_OR_WORD : public SComparison2_Base_2
			{
				bool operator()(BYTE *pData)
				{
					return (*(WORD*)pData < m_nTarget1stMin || *(WORD*)pData > m_nTarget1stMax) ||
						(*((WORD*)pData + 1) < m_nTarget2ndMin || *((WORD*)pData + 1) > m_nTarget2ndMax);
				}
			};

			struct SComparison2_Equal_2_AND_WORD : public SComparison2_Base_2
			{
				bool operator()(BYTE *pData)
				{
					return (*(WORD*)pData >= m_nTarget1stMin && *(WORD*)pData <= m_nTarget1stMax) &&
						(*((WORD*)pData + 1) >= m_nTarget2ndMin && *((WORD*)pData + 1) <= m_nTarget2ndMax);
				}
			};

			struct SComparison2_NotEqual_2_AND_WORD : public SComparison2_Base_2
			{
				bool operator()(BYTE *pData)
				{
					return (*(WORD*)pData < m_nTarget1stMin || *(WORD*)pData > m_nTarget1stMax) &&
						(*((WORD*)pData + 1) < m_nTarget2ndMin || *((WORD*)pData + 1) > m_nTarget2ndMax);
				}
			};

			// 3 Channel

			struct SComparison2_Base_3
			{
				int m_nTarget1stMin;
				int m_nTarget1stMax;
				int m_nTarget2ndMin;
				int m_nTarget2ndMax;
				int m_nTarget3rdMin;
				int m_nTarget3rdMax;

				void SetTarget(const Framework::CMultipleVariable &rmv)
				{
					m_nTarget1stMin = static_cast<int>(rmv[0]);
					m_nTarget2ndMin = static_cast<int>(rmv[1]);
					m_nTarget3rdMin = static_cast<int>(rmv[2]);

					m_nTarget1stMax = static_cast<int>(rmv[3]);
					m_nTarget2ndMax = static_cast<int>(rmv[4]);
					m_nTarget3rdMax = static_cast<int>(rmv[5]);
				}
			};

			struct SComparison2_Equal_3_OR_BYTE : public SComparison2_Base_3
			{
				bool operator()(BYTE *pData)
				{
					return (*pData >= m_nTarget1stMin && *pData <= m_nTarget1stMax) ||
						(*(pData + 1) >= m_nTarget2ndMin && *(pData + 1) <= m_nTarget2ndMax) ||
						(*(pData + 2) >= m_nTarget3rdMin && *(pData + 2) <= m_nTarget3rdMax);
				}
			};

			struct SComparison2_NotEqual_3_OR_BYTE : public SComparison2_Base_3
			{
				bool operator()(BYTE *pData)
				{
					return (*pData < m_nTarget1stMin || *pData > m_nTarget1stMax) ||
						(*(pData + 1) < m_nTarget2ndMin || *(pData + 1) > m_nTarget2ndMax) ||
						(*(pData + 2) < m_nTarget3rdMin || *(pData + 2) > m_nTarget3rdMax);
				}
			};

			struct SComparison2_Equal_3_AND_BYTE : public SComparison2_Base_3
			{
				bool operator()(BYTE *pData)
				{
					return (*pData >= m_nTarget1stMin && *pData <= m_nTarget1stMax) &&
						(*(pData + 1) >= m_nTarget2ndMin && *(pData + 1) <= m_nTarget2ndMax) &&
						(*(pData + 2) >= m_nTarget3rdMin && *(pData + 2) <= m_nTarget3rdMax);
				}
			};

			struct SComparison2_NotEqual_3_AND_BYTE : public SComparison2_Base_3
			{
				bool operator()(BYTE *pData)
				{
					return (*pData < m_nTarget1stMin || *pData > m_nTarget1stMax) &&
						(*(pData + 1) < m_nTarget2ndMin || *(pData + 1) > m_nTarget2ndMax) &&
						(*(pData + 2) < m_nTarget3rdMin || *(pData + 2) > m_nTarget3rdMax);
				}
			};

			struct SComparison2_Equal_3_OR_WORD : public SComparison2_Base_3
			{
				bool operator()(BYTE *pData)
				{
					return (*(WORD*)pData >= m_nTarget1stMin && *(WORD*)pData <= m_nTarget1stMax) ||
						(*((WORD*)pData + 1) >= m_nTarget2ndMin && *((WORD*)pData + 1) <= m_nTarget2ndMax) ||
						(*((WORD*)pData + 2) >= m_nTarget3rdMin && *((WORD*)pData + 2) <= m_nTarget3rdMax);
				}
			};

			struct SComparison2_NotEqual_3_OR_WORD : public SComparison2_Base_3
			{
				bool operator()(BYTE *pData)
				{
					return (*(WORD*)pData < m_nTarget1stMin || *(WORD*)pData > m_nTarget1stMax) ||
						(*((WORD*)pData + 1) < m_nTarget2ndMin || *((WORD*)pData + 1) > m_nTarget2ndMax) ||
						(*((WORD*)pData + 2) < m_nTarget3rdMin || *((WORD*)pData + 2) > m_nTarget3rdMax);
				}
			};

			struct SComparison2_Equal_3_AND_WORD : public SComparison2_Base_3
			{
				bool operator()(BYTE *pData)
				{
					return (*(WORD*)pData >= m_nTarget1stMin && *(WORD*)pData <= m_nTarget1stMax) &&
						(*((WORD*)pData + 1) >= m_nTarget2ndMin && *((WORD*)pData + 1) <= m_nTarget2ndMax) &&
						(*((WORD*)pData + 2) >= m_nTarget3rdMin && *((WORD*)pData + 2) <= m_nTarget3rdMax);
				}
			};

			struct SComparison2_NotEqual_3_AND_WORD : public SComparison2_Base_3
			{
				bool operator()(BYTE *pData)
				{
					return (*(WORD*)pData < m_nTarget1stMin || *(WORD*)pData > m_nTarget1stMax) &&
						(*((WORD*)pData + 1) < m_nTarget2ndMin || *((WORD*)pData + 1) > m_nTarget2ndMax) &&
						(*((WORD*)pData + 2) < m_nTarget3rdMin || *((WORD*)pData + 2) > m_nTarget3rdMax);
				}
			};

			// 4 Channel

			struct SComparison2_Base_4
			{
				int m_nTarget1stMin;
				int m_nTarget1stMax;
				int m_nTarget2ndMin;
				int m_nTarget2ndMax;
				int m_nTarget3rdMin;
				int m_nTarget3rdMax;
				int m_nTarget4thMin;
				int m_nTarget4thMax;

				void SetTarget(const Framework::CMultipleVariable &rmv)
				{
					m_nTarget1stMin = static_cast<int>(rmv[0]);
					m_nTarget2ndMin = static_cast<int>(rmv[1]);
					m_nTarget3rdMin = static_cast<int>(rmv[2]);
					m_nTarget4thMin = static_cast<int>(rmv[3]);

					m_nTarget1stMax = static_cast<int>(rmv[4]);
					m_nTarget2ndMax = static_cast<int>(rmv[5]);
					m_nTarget3rdMax = static_cast<int>(rmv[6]);
					m_nTarget4thMax = static_cast<int>(rmv[7]);
				}
			};

			struct SComparison2_Equal_4_OR_BYTE : public SComparison2_Base_4
			{
				bool operator()(BYTE *pData)
				{
					return (*pData >= m_nTarget1stMin && *pData <= m_nTarget1stMax) ||
						(*(pData + 1) >= m_nTarget2ndMin && *(pData + 1) <= m_nTarget2ndMax) ||
						(*(pData + 2) >= m_nTarget3rdMin && *(pData + 2) <= m_nTarget3rdMax) ||
						(*(pData + 3) >= m_nTarget4thMin && *(pData + 3) <= m_nTarget4thMax);
				}
			};

			struct SComparison2_NotEqual_4_OR_BYTE : public SComparison2_Base_4
			{
				bool operator()(BYTE *pData)
				{
					return (*pData < m_nTarget1stMin || *pData > m_nTarget1stMax) ||
						(*(pData + 1) < m_nTarget2ndMin || *(pData + 1) > m_nTarget2ndMax) ||
						(*(pData + 2) < m_nTarget3rdMin || *(pData + 2) > m_nTarget3rdMax) ||
						(*(pData + 3) < m_nTarget4thMin || *(pData + 3) > m_nTarget4thMax);
				}
			};

			struct SComparison2_Equal_4_AND_BYTE : public SComparison2_Base_4
			{
				bool operator()(BYTE *pData)
				{
					return (*pData >= m_nTarget1stMin && *pData <= m_nTarget1stMax) &&
						(*(pData + 1) >= m_nTarget2ndMin && *(pData + 1) <= m_nTarget2ndMax) &&
						(*(pData + 2) >= m_nTarget3rdMin && *(pData + 2) <= m_nTarget3rdMax) &&
						(*(pData + 3) >= m_nTarget4thMin && *(pData + 3) <= m_nTarget4thMax);
				}
			};

			struct SComparison2_NotEqual_4_AND_BYTE : public SComparison2_Base_4
			{
				bool operator()(BYTE *pData)
				{
					return (*pData < m_nTarget1stMin || *pData > m_nTarget1stMax) &&
						(*(pData + 1) < m_nTarget2ndMin || *(pData + 1) > m_nTarget2ndMax) &&
						(*(pData + 2) < m_nTarget3rdMin || *(pData + 2) > m_nTarget3rdMax) &&
						(*(pData + 3) < m_nTarget4thMin || *(pData + 3) > m_nTarget4thMax);
				}
			};

			struct SComparison2_Equal_4_OR_WORD : public SComparison2_Base_4
			{
				bool operator()(BYTE *pData)
				{
					return (*(WORD*)pData >= m_nTarget1stMin && *pData <= m_nTarget1stMax) ||
						(*((WORD*)pData + 1) >= m_nTarget2ndMin && *((WORD*)pData + 1) <= m_nTarget2ndMax) ||
						(*((WORD*)pData + 2) >= m_nTarget3rdMin && *((WORD*)pData + 2) <= m_nTarget3rdMax) ||
						(*((WORD*)pData + 3) >= m_nTarget4thMin && *((WORD*)pData + 3) <= m_nTarget4thMax);
				}
			};

			struct SComparison2_NotEqual_4_OR_WORD : public SComparison2_Base_4
			{
				bool operator()(BYTE *pData)
				{
					return (*(WORD*)pData < m_nTarget1stMin || *pData > m_nTarget1stMax) ||
						(*((WORD*)pData + 1) < m_nTarget2ndMin || *((WORD*)pData + 1) > m_nTarget2ndMax) ||
						(*((WORD*)pData + 2) < m_nTarget3rdMin || *((WORD*)pData + 2) > m_nTarget3rdMax) ||
						(*((WORD*)pData + 3) < m_nTarget4thMin || *((WORD*)pData + 3) > m_nTarget4thMax);
				}
			};

			struct SComparison2_Equal_4_AND_WORD : public SComparison2_Base_4
			{
				bool operator()(BYTE *pData)
				{
					return (*(WORD*)pData >= m_nTarget1stMin && *pData <= m_nTarget1stMax) &&
						(*((WORD*)pData + 1) >= m_nTarget2ndMin && *((WORD*)pData + 1) <= m_nTarget2ndMax) &&
						(*((WORD*)pData + 2) >= m_nTarget3rdMin && *((WORD*)pData + 2) <= m_nTarget3rdMax) &&
						(*((WORD*)pData + 3) >= m_nTarget4thMin && *((WORD*)pData + 3) <= m_nTarget4thMax);
				}
			};

			struct SComparison2_NotEqual_4_AND_WORD : public SComparison2_Base_4
			{
				bool operator()(BYTE *pData)
				{
					return (*(WORD*)pData < m_nTarget1stMin || *pData > m_nTarget1stMax) &&
						(*((WORD*)pData + 1) < m_nTarget2ndMin || *((WORD*)pData + 1) > m_nTarget2ndMax) &&
						(*((WORD*)pData + 2) < m_nTarget3rdMin || *((WORD*)pData + 2) > m_nTarget3rdMax) &&
						(*((WORD*)pData + 3) < m_nTarget4thMin || *((WORD*)pData + 3) > m_nTarget4thMax);
				}
			};
		}
	}
}