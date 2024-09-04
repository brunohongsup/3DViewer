#pragma once

#include "RavidObjectEx.h"

#include <memory>

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CPacket : public CRavidObjectEx
		{
			RavidUseDynamicCreation();
			RavidPreventCopySelf(CPacket);

		public:
			CPacket();
			virtual ~CPacket();

			void Clear();

			SOCKET GetSocket();
			void SetSocket(_In_ SOCKET hSocket);

			char* GetData(_Out_ size_t& stLength);
			bool SetData(_In_ char* pData, _In_ size_t stLength);

		protected:
			SOCKET m_hSocket = 0;
			char* m_pData = nullptr;
			size_t m_stLength = 0;

		};
	}
}

