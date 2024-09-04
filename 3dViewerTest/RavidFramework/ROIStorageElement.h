#pragma once

#include "ViewObjectElement.h"

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CROIStorageElement : public CViewObjectElement
		{
		public:
			CROIStorageElement();
			~CROIStorageElement();

		public:
			virtual void SetTitle(_In_ CString strTitle);
			virtual bool SetGeometry(_In_ CString strShapeType, _In_ CString strTemplateType, _In_ CString strGeometryData, _In_ CString strGeometryExtra);

			virtual bool SetGeometryExclusiveRegion(_In_ CRavidGeometry* pRG, _In_ CString strGeometryExtra, _In_ CString strExclusiveName);
			virtual CRavidGeometry* CreateGeometry(_In_ CString strShapeType, _In_ CString strTemplateType, _In_ CString strGeometryData);

			virtual CString GetTitle();
			virtual CRavidGeometry* GetGeometry();

			virtual ERavidGeometryShapeType ConvertGeometryShapeTypeCStringToEnum(_In_ CString strShapeType);
			virtual ERavidGeometryTemplateType ConvertGeometryTemplateTypeCStringToEnum(_In_ CString strTemplateType);

			virtual CString ConvertGeometryShapeTypeEnumToString(_In_ ERavidGeometryShapeType eShapeType);
			virtual CString ConvertGeometryTemplateTypeEnumToString(_In_ ERavidGeometryTemplateType eTemplateType);

			virtual CString GetGeometryData(_In_ CRavidGeometry* pRG);
			virtual CString GetGeometryExtra(_In_ CRavidGeometry* pRG, _In_ CString strExclusive);

			virtual CRavidPoint<double> GetDrawPoint();

		protected:
			CRavidGeometry* m_pGeometry = nullptr;
			CString m_strTitle = _T("");
		};
	}
}


