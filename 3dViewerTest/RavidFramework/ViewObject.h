#pragma once

#include "RavidObjectEx.h"

#include <vector>

class CRavidGeometry;

namespace Ravid
{
	namespace Framework
	{
		class CRavidImageView;
		class CRavidImageViewLayer;
		class CViewObjectElement;

		class AFX_EXT_CLASS CViewObject : public CRavidObjectEx
		{
			RavidUseDynamicCreation(this);

		public:
			CViewObject();
			CViewObject(CViewObject& rto);
			CViewObject(CViewObject* pRto);

			virtual ~CViewObject();

			bool Copy(_In_ CViewObject& rto);
			bool Copy(_In_ CViewObject* pRto);

			virtual bool Add(_In_ CRavidGeometry& geometry);
			virtual bool Add(_In_ CRavidGeometry* pGeometry);

			virtual void SetTitle(_In_ CString strTitle);
			virtual CString GetTitle();

			virtual EViewObjectContextMenuType OnContextMenu(_In_ CRavidPoint<double> rp, _In_ double dblScale);

			virtual bool RegisterGripInfo();

			CRavidGeometry* GetAt(_In_ size_t stNumber);
			size_t GetCount();

			bool DeleteAt(_In_ size_t stNumber);

			void Clear();

			CViewObjectElement* GetElement(_In_ size_t stNumber);

			bool SetScale(double dblScale);

			void Select(_In_ bool bSelect);
			bool IsSelected();

			bool IsShapeValid();

			void SetAutoShape(_In_ bool bSet);
			bool IsAutoShape();

			void VisibleObject(_In_ bool bVisible);
			bool IsVisibled();

			void SetLastSelectedObjectNumber(_In_ size_t stNumber);
			size_t GetLastSelectedObjectNumber();

			void SetParentImageView(CRavidImageView* pImageView);
			CRavidImageView* GetParentImageView();

			bool Invalidate(_In_opt_ bool bErase = true);

			virtual bool SetTeachingObjectElement(_In_ CViewObjectElement* pVOE);

			void NumberObject();

		protected:

			CRavidImageView* m_pParentImageView = nullptr;

			bool m_bIsVisibled = true;

			CString m_strTitle;

			std::vector<CViewObjectElement*> m_vctTeachingObjectElement;

			bool m_bIsSelected = false;

			double m_dblScale = 1.0;

			bool m_bIsAutoShape = true;

			size_t m_stLastSelectedObjectNumber = 0;
		};
	}
}