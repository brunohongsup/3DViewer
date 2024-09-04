#pragma once

#include "RavidObjectEx.h"

#include "../RavidCore/RavidPoint.h"
#include "../RavidCore/RavidLine.h"
#include "../RavidCore/RavidQuadrangle.h"

#include <utility>
#include <deque>

namespace Ravid
{
	namespace Framework
	{
		class CViewObject;
		class CRavidImageViewLayer;
		class CRavidImageToolBase;
		class CRavidDlgGeometryScript;
		class CRavidDlgModifyGeometry;
		class CRavidDlgOperationGeometry;
		class CRavidBehavior;

		class AFX_EXT_CLASS CViewObjectElement : public CRavidObjectEx
		{
			RavidUseDynamicCreation(this);

			enum EKeyStatus
			{
				EKeyStatus_None = 0,
				EKeyStatus_Control,
				EKeyStatus_Shift,
				EKeyStatus_Both,
			};

			typedef struct sViewObjectGripInfo
			{
				CRavidPoint<double> rp;
				EViewObjectGripType eRTOGT = EViewObjectGripType_None;
				size_t stIndex = 0;
			}
			SViewObjectGripInfo;

		public:
			CViewObjectElement();
			CViewObjectElement(CViewObjectElement& rtoe);
			CViewObjectElement(CViewObjectElement* pRtoe);

			virtual ~CViewObjectElement();

			bool Copy(_In_ CViewObjectElement& rtoe);
			bool Copy(_In_ CViewObjectElement* pRtoe);

			virtual void Clear();

			virtual bool DrawLayer(_In_ CRavidImageViewLayer* pLayer, _In_ CRavidPoint<double>& rp, _In_ double& dblScale, _In_opt_ double dblAccuracyX = 1., _In_opt_ double dblAccuracyY = 1., _In_opt_ ERavidAccuracyUnit eAccuracyUnit = ERavidAccuracyUnit_mm);

			virtual CRavidGeometry* Get();
			virtual bool Set(_In_ CRavidGeometry* pRG);

			virtual bool Set(_In_ EViewObjectGripType eRTOGT, _In_ CRavidPoint<double> rpCur);

			virtual CRavidRect<double> GetRect();
			virtual bool SetRect(_In_ CRavidRect<double>* pRR);

			virtual bool SetEllipse(_In_ CRavidPoint<double>* pRPCenter, _In_ double dblRadius1, _In_ double dblRadius2);

			virtual CRavidPoint<double> GetDrawPoint();
			virtual void SetDrawPoint(CRavidPoint<double> rpPoint);

			virtual bool HitTest(_In_ CRavidPoint<double>& rp, _In_ double dblScale);
			virtual bool HitTest(_In_ CRavidPoint<double>* pRP, _In_ double dblScale);

			virtual bool HitRectTest(_In_ CRavidPoint<double>& rp, _In_ double dblScale);
			virtual bool HitRectTest(_In_ CRavidPoint<double>* pRP, _In_ double dblScale);

			virtual EViewObjectGripType HitGripTest(_In_ CRavidPoint<double>& rp, _In_ double dblScale, _Out_opt_ CRavidPoint<double>* pRPRef = nullptr);
			virtual EViewObjectGripType HitGripTest(_In_ CRavidPoint<double>* pRP, _In_ double dblScale, _Out_opt_ CRavidPoint<double>* pRPRef = nullptr);

			virtual bool HitLineTest(_In_ CRavidPoint<double>& rp, _In_ double dblScale);
			virtual bool HitLineTest(_In_ CRavidPoint<double>* pRP, _In_ double dblScale);

			virtual bool OnLButtonDown(_In_ UINT nFlags, _In_ CRavidPoint<double>& rp, _In_ double dblScale);
			virtual bool OnLButtonDown(_In_ UINT nFlags, _In_ CRavidPoint<double>* pRP, _In_ double dblScale);

			virtual bool OnLButtonUp(_In_ UINT nFlags, _In_ CRavidPoint<double>& rp, _In_ double dblScale);
			virtual bool OnLButtonUp(_In_ UINT nFlags, _In_ CRavidPoint<double>* pRP, _In_ double dblScale);

			virtual bool OnMouseMove(_In_ UINT nFlags, _In_ CRavidPoint<double>& rp, _In_ double dblScale);
			virtual bool OnMouseMove(_In_ UINT nFlags, _In_ CRavidPoint<double>* pRP, _In_ double dblScale);


			virtual bool SetOffset(_In_ CRavidPoint<double> rpOffset);
			virtual bool SetRotate(_In_ CRavidPoint<double> rpCenter, double dblAngle);


			virtual bool CheckMouseHover(_In_ CRavidPoint<double> rp, _In_ double dblScale);

			virtual EViewObjectContextMenuType OnContextMenu(_In_ CRavidPoint<double> rp, _In_ double dblScale);

			virtual bool ConvertEllipse(CRavidPoint<double> rp, _In_opt_ bool bIsArc = false, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_NoClose);

			virtual void SetParent(_In_ CViewObject* pParent);
			virtual CViewObject* GetParent();

			virtual EViewObjectGripType GetSelectedTeachingObjectGripType();

			virtual void SetScale(_In_ double dblScale);

			virtual void Select(_In_ bool bSelect);
			virtual bool IsSelected();

			virtual void SetMouseHover(_In_ bool bHover);
			virtual bool IsMouseHover();

			virtual bool IsVisibled();

			virtual bool GetIntersectPolygonSectionInfo(_In_ CRavidPoint<double>* pRP, _Out_ size_t& stIndex, _Out_ EViewObjectGripType& eRTOGT);

			virtual bool ChangePolyLine(size_t stIndex);
			virtual bool ChangePolyLineOfSelectedIdx();

			virtual bool RemovePolygonPoint(size_t stIndex);
			virtual bool RemovePolygonPointOfSelectedIdx();

			virtual bool AddPolygonPoint(_In_ CRavidPoint<double>& rp);
			virtual bool AddPolygonPoint(_In_ CRavidPoint<double>* pRP);

			virtual void SetAbleToRotate(_In_ bool bAble);
			virtual bool IsAbleToRotate();

			virtual bool RegisterGripInfo();

			virtual bool RunScript();

			virtual bool UpdateScript(_In_ std::vector<std::pair<COLORREF, CString> >* pVctScript);

			virtual bool CloseScript();

			virtual bool UpdateScriptName();

			virtual CString GetScriptVariableName();
			virtual CString GetScripTitle();

			virtual bool ConvertToBoundaryRect();

			virtual bool ChangeGeometryType();

			virtual bool ModifyGeometry(_In_ bool bResetData = true);
			virtual bool UpdateModifyGeometryName();
			virtual bool CloseModifyGeometry();
			CRavidDlgModifyGeometry* const GetModifyGeometry();

			CRavidDlgGeometryScript* const GetGeometryScript();

			CRavidDlgOperationGeometry* const GetOperationGeometry();

			virtual CString GetGeometryName();

			virtual bool RegisterPreviewGeometry(_In_ CRavidGeometry* pRG);
			virtual bool UnregisterPreviewGeometry();

			virtual bool RegisterHighlightOperationGeometry(_In_ CRavidGeometry* pRG);
			virtual void UnregisterHighlightOperationGeometry();

			virtual bool RegisterSelectedOperationGeometry(_In_ CRavidGeometry* pRG);
			virtual void UnregisterSelectedOperationGeometry();

			virtual CRavidGeometry* GetPreviewGeometry();
			virtual CRavidPoint<double> GetDrawPreviewPoint();

			virtual bool Invalidate(bool bErase);

			CString ConvertGeometryShapeTypeEnumToString(_In_ ERavidGeometryShapeType eType);
			CString ConvertGeometryTemplateTypeEnumToString(_In_ ERavidGeometryTemplateType eType);

			virtual bool ResizeGeometry(_Inout_ CRavidGeometry* pRG, _In_ double dblDstImgSizeX, _In_ double dblDstImgSizeY);

			bool OperationGeometry(_In_ CRavidGeometry* pSrcRG, _In_ ERavidGeometryOperationType eType = ERavidGeometryOperationType_Unknown);

			virtual bool RunOperation(_In_ std::vector<CViewObjectElement*> vctViewObject, _In_ std::vector<CString> vctGeometryTitle);
			virtual bool CloseOperation();

			virtual bool UpdateOperationList(_In_ std::vector<CViewObjectElement*> vctViewObject, _In_ std::vector<CString> vctGeometryTitle);
			virtual bool UpdateOperationName(_In_ CString strGeometryName);

			virtual void UseUserDefiendShapeColor(_In_ bool bSet);
			virtual bool IsUsedUserDefiendShapeColor();

			virtual void SetUserDefiendShapeColor(_In_ COLORREF clrShape);
			virtual COLORREF GetUserDefiendShapeColor();

			virtual bool AddUndoData();
			virtual bool BackupUndoData(_In_ CRavidBehavior* pBehavior);

			virtual bool ClearUndoData();
			virtual bool ClearRedoData();

			virtual bool RangeUndoData();
			virtual bool RangeRedoData();

			virtual bool Undo();
			virtual bool Redo();

		protected:

			virtual bool SetCursor(EViewObjectGripType eRTOGT);

			virtual bool InternalSetCorner(_In_ EViewObjectGripType eRTOGT, _In_ CRavidPoint<double> rpCur);
			virtual bool InternalSetLine(_In_ EViewObjectGripType eRTOGT, _In_ CRavidPoint<double> rpCur);
			virtual bool InternalSetOuterCorner(_In_ EViewObjectGripType eRTOGT, _In_ CRavidPoint<double> rpCur);
			virtual bool InternalSetOuterLine(_In_ EViewObjectGripType eRTOGT, _In_ CRavidPoint<double> rpCur);
			virtual bool InternalSetPoint(_In_ CRavidPoint<double> rpCur);
			virtual bool InternalSetRotate(_In_ CRavidPoint<double> rpCur);
			virtual bool InternalSetOffset(_In_ CRavidPoint<double> rpCur);
			virtual bool InternalSetPolygonArc(_In_ CRavidPoint<double> rpCur);

			virtual CRavidPoint<double> GetTextDrawPoint(_In_ CRavidPoint<double> rpRef, _In_ CString strText, _In_ CRavidImageViewLayer* pLayer);

			virtual CRavidPoint<double>* GetTargetPoint(_In_ CRavidGeometry* pRG, _In_ CRavidPoint<double>* pRPTarget);
			virtual CRavidPoint<double>* GetPoint(_In_ CRavidGeometry* pRG, _In_ size_t stIndex);

			virtual bool QuadrangleMoveToLine(long nPtIdx1, long nPtIdx2, _In_ CRavidPoint<double>* pRP, _Inout_ CRavidQuadrangle<double>* pRQ);

			virtual bool SetMinimumQuadrangle(_In_ CRavidQuadrangle<double>* pRQOrg, _In_ long nTargetPoint, _Inout_ CRavidQuadrangle<double>* pRQDst);

			virtual bool SetMinimumQuadrangle(_In_ CRavidQuadrangle<double>* pRQOrg, _Inout_ CRavidQuadrangle<double>* pRQDst, _In_ EViewObjectGripType eType, _In_ double dblMinimumWidth, _In_ double dblMinimumHeight);

			virtual bool IsEqualDirectionDotProduct(_In_ CRavidPoint<double> rpDP1, _In_ CRavidPoint<double> rpDP2);

			virtual bool GetArcCenter(_In_ CRavidPoint<double> rp1, _In_ CRavidPoint<double> rp2, _In_ double dblRadius, _Out_ CRavidPoint<double>* pRPCenter1, _Out_ CRavidPoint<double>* pRPCenter2);

			virtual bool GetNearstEllipsePoint(_In_ CRavidEllipse<double>* pRE, _In_ CRavidPoint<double>* pRPTarget, _Out_ CRavidPoint<double>* pNearstPoint);

			virtual bool GetNearstQuadranglePoint(_In_ CRavidQuadrangle<double>* pRQ, _In_ long nIdx, _In_ CRavidPoint<double>* pRPTarget, _Out_ CRavidPoint<double>* pNearstPoint);

			virtual bool DoesIntersectPoint(_In_ CRavidPoint<double>* pRP, _In_ CRavidPoint<double>* pRPTarget, _In_ double dblOffset);

			virtual bool DoesIntersectLine(_In_ CRavidLine<double>* pRL, _In_ CRavidPoint<double>* pRPTarget, _In_ double dblOffset);
			virtual bool DoesIntersectLine(_In_ CRavidPoint<double>* pRP1, _In_ CRavidPoint<double>* pRP2, _In_ CRavidPoint<double>* pRPTarget, _In_ double dblOffset);

			virtual bool DoesIntersectEllipse(_In_ CRavidEllipse<double>* pRE, _In_ CRavidPoint<double>* pRPTarget, _In_ double dblOffset);

			virtual EViewObjectGripType DoesIntersectQuadrangleLine(_In_ CRavidQuadrangle<double>* pRQ, _In_ CRavidPoint<double>* pRPTarget, _In_ double dblOffset);

			virtual EViewObjectGripType DoesIntersectRectLine(_In_ CRavidRect<double>* pRR, _In_ CRavidPoint<double>* pRPTarget, _In_ double dblOffset);

			virtual bool DoesIntersectEllipseLine(_In_ CRavidEllipse<double>* pRE, _In_ CRavidPoint<double>* pRPTarget, _In_ double dblOffset);

			virtual bool DoesIntersectPolygonLine(_In_ CRavidPolygon* pRPG, _In_ CRavidPoint<double>* pRPTarget, _In_ double dblOffset);

			virtual bool EllipseToQuadrangle(_In_ CRavidEllipse<double>* pRE, _Out_ CRavidQuadrangle<double>* pRQ);

			virtual bool QuadrangleToEllipse(_In_ CRavidQuadrangle<double>* pRQ, _Out_ CRavidEllipse<double>* pRE);

			virtual bool NoQuadrangleToRect(_In_ CRavidQuadrangle<double>* pRQ, _Out_ CRavidRect<double>* pRR);

			virtual bool QuadrangleToRect(_In_ CRavidQuadrangle<double>* pRQ, _Out_ CRavidRect<double>* pRR);

			virtual bool RectToQuadrangle(_In_ CRavidRect<double>* pRR, _Out_ CRavidQuadrangle<double>* pRQ);

			virtual bool DrawEdgeShape(_In_ CRavidImageViewLayer* pLayer, _In_ CRavidGeometry& rgObject, _In_opt_ COLORREF crLineColor = 0, _In_opt_ COLORREF crEdgeColor = 0, _In_opt_ long nLineWidth = 1, _In_opt_ long nEdgeWidth = 3, _In_opt_ COLORREF crFillColor = ERavidImageViewLayerTransparencyColor, _In_opt_ ERavidImageViewPenStyle ePenStyle = ERavidImageViewPenStyle_SOLID);
			virtual bool DrawEdgeShape(_In_ CRavidImageViewLayer* pLayer, _In_ CRavidGeometry* pRgObject, _In_opt_ COLORREF crLineColor = 0, _In_opt_ COLORREF crEdgeColor = 0, _In_opt_ long nLineWidth = 1, long nEdgeWidth = 3, _In_opt_ COLORREF crFillColor = ERavidImageViewLayerTransparencyColor, _In_opt_ ERavidImageViewPenStyle ePenStyle = ERavidImageViewPenStyle_SOLID);

			virtual CRavidGeometry* CopyGeometry(_In_ CRavidGeometry* pRGSrc, _In_opt_ ERavidGeometryTemplateType eType = ERavidGeometryTemplateType_Unknown);
			virtual ERavidGeometryTemplateType GetGeometryTemplateType(_In_ CRavidGeometry* pRG);

			virtual EKeyStatus GetKeyStatus(_Out_opt_ bool* pControlkey = nullptr, _Out_opt_ bool* pShiftKey = nullptr);

			virtual bool GetFixedRatioPoint(_In_ CRavidPoint<double> rpTarget, CRavidPoint<double> rpOrg1, _In_ CRavidPoint<double> rpOrg2, _Out_ CRavidPoint<double>& rpResult);

			virtual bool GetFixedRatioPointForMinimumsizeCheck(_In_ double dblWidth, _In_ double dblHeight, _In_ CRavidPoint<double> rpTarget, CRavidPoint<double> rpOrg1, _In_ CRavidPoint<double> rpOrg2, _Out_ CRavidPoint<double>& rpResult, _Out_ double& dblMinimumWidth, _Out_ double& dblMinimumHeight);

			virtual bool IsIntegerGeometry(_In_ CRavidGeometry* pRG);
			virtual CRavidPoint<int> GetIntegerPoint(_In_ CRavidPoint<double>& rp);
			virtual int GetIntegerValue(_In_ double dblValue);

			virtual bool GetScript(_In_ CRavidGeometry* pRG, std::vector<std::pair<COLORREF, CString> >* pVctScript, _In_opt_ CString* strVariableSubName = nullptr, _In_opt_ CString* strVariableName = nullptr);

			virtual double GetCrossHairSize(_In_ CRavidRect<double> rr);

		protected:
			CRavidDlgModifyGeometry* m_pModifyGeometry = nullptr;
			CRavidGeometry* m_pElementGeometry = nullptr;
			CRavidGeometry* m_pOrgGeometry = nullptr;
			CRavidDlgGeometryScript* m_pGeometryScript = nullptr;
			CRavidGeometry* m_pPreviewGeometry = nullptr;
			bool m_bModifyGeometryValueReset = false;

			bool m_bSelectedOperation = false;

			CRavidDlgOperationGeometry* m_pOperationGeometry = nullptr;
			CRavidGeometry* m_pHighlightOperationGeometry = nullptr;
			CRavidGeometry* m_pSelectedOperationGeometry = nullptr;

			CRavidPoint<double> m_rpRefCoord;
			CRavidPoint<double> m_rpCurCoord;

			std::vector<SViewObjectGripInfo> m_vctTeachingObjectGripInfo;

			EViewObjectGripType m_eSelectedGripType = EViewObjectGripType_None;

			bool m_bIsSelected = false;

			double m_dblScale = 1.;

			double m_dblGripPointOffset = 6.;
			double m_dblGripRotateOffset = 11.;

			double m_dblGripOffset1 = 5.;
			double m_dblGripOffset2 = 10.;
			double m_dblMinimumSize = 5.;
			double m_dblMagnetSize = 5.;
			double m_dblMagnetAnglePx = 3.;
			double m_dblAccumulateMagnetAngle = 0.;

			size_t m_stSelectedPointIdx = 0;

			CViewObject* m_pParent = nullptr;

			bool m_bIsMouseHover = false;

			CRavidLine<double> m_rlGuide;

			bool m_bAbleToRotate = true;

			bool m_bUsedUserDefiendShapeColor = false;
			COLORREF m_clrUserDefiendShapeColor = RGB(0, 0, 0);

			bool m_bIsMove = false;

			CRavidPoint<double> m_rpDrawPoint;

			std::deque<CRavidBehavior*> m_deqUndoBuffer;
			std::deque<CRavidBehavior*> m_deqRedoBuffer;

			CRavidBehavior* m_pBackupUndoData = nullptr;
		};
	}
}