#pragma	once

#include "RavidObjectEx.h"
#include "../Ravid3D/3DViewer.h"

class CRavid3DData;
namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CRavid3DViewer : public C3DViewer, public CRavidObjectEx
		{
			RavidUseDynamicCreation();

			DECLARE_DYNCREATE(CRavid3DViewer)

		protected:
			CRavid3DViewer(void);
			virtual ~CRavid3DViewer(void);

		public:
			class CRavidDoc* GetDocument(void) const;

#ifdef _DEBUG
			virtual void AssertValid() const;
			virtual void Dump(CDumpContext& dc) const;
#endif

		public:
			virtual bool AddMap(_In_ CRavid3DData* pData);

			virtual bool Clear();

			DECLARE_MESSAGE_MAP()
		protected:
			virtual void OnDraw(CDC* pDC);

			virtual void OnInitialUpdate();
		};
	}
}
