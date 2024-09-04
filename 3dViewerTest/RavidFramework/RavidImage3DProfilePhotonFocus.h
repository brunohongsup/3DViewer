#pragma once

#include "../RavidCore/RavidImage3DProfile.h"

namespace Ravid
{
	namespace Algorithms
	{
		class AFX_EXT_CLASS CRavidImage3DProfilePhotonFocus : public CRavidImage3DProfile
		{
		public:
			CRavidImage3DProfilePhotonFocus();
			virtual ~CRavidImage3DProfilePhotonFocus();

			virtual void Initialize(size_t st3DSizeX, size_t st3DSizeY = 1);
			virtual void Terminate();

			virtual void UpdateProfile();

			virtual BYTE GetPeakInByte(int nIndex);
			virtual BYTE GetWidth(int nIndex);
			virtual BYTE GetQuality(int nIndex);

			virtual BYTE* GetPeak();
			virtual BYTE* GetWidth();
			virtual BYTE* GetQuality();

			void Set3DOffset(ptrdiff_t pdtOffset);
			ptrdiff_t Get3DOffset();

		protected:
			ptrdiff_t m_pdt3DOffset = 0;

			BYTE* m_pcPeak = nullptr;
			BYTE* m_pcWidth = nullptr;
			BYTE* m_pcQuality = nullptr;

			bool m_bIsInitialized = false;
		};
	}
}

