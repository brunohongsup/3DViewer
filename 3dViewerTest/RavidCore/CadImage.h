#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <vector>

#include "RavidLine.h"
#include "RavidRect.h"
#include "RavidCircle.h"
#include "RavidEllipse.h"
#include "RavidPolygon.h"

#include "DxfReader.h"

using namespace Ravid;
using namespace Ravid::Algorithms;
using namespace Ravid::Framework;
using namespace Ravid::Miscellaneous;

enum ECadImageDxfEntityType
{
	ECadImageDxfEntityType_Point = 0,
	ECadImageDxfEntityType_Line,
	ECadImageDxfEntityType_Arc,
	ECadImageDxfEntityType_Circle,
	ECadImageDxfEntityType_Ellipse,
	ECadImageDxfEntityType_HatchMain,
	ECadImageDxfEntityType_HatchSub,
};

enum ELayerColor
{
	EColor_black = 250,
	EColor_green = 3,
	EColor_red = 1,
	EColor_brown = 15,
	EColor_yellow = 2,
	EColor_cyan = 4,
	EColor_magenta = 6,
	EColor_gray = 8,
	EColor_blue = 5,
	EColor_l_blue = 163,
	EColor_l_green = 121,
	EColor_l_cyan = 131,
	EColor_l_red = 23,
	EColor_l_magenta = 221,
	EColor_l_gray = 252,
	EColor_white = 7,
	EColor_bylayer = 256,
	EColor_byblock = 0
};

/**
* DXF 파일을 제어하는 클래스입니다.
* DXF 파일을 생성, 수정합니다.
* 현재 개발 진행중입니다.
*/
class AFX_EXT_CLASS CCadImage
{
public:
	CCadImage::CCadImage()
	{
	};

	virtual CCadImage::~CCadImage()
	{
		ClearDxf();
	};

	// DXF 파서
	CDxfReader dxf;

	void ClearDxf();

	bool OpenDxf();
	bool OpenDxf(CString filename);

	// DXF 파싱 직후 점을 회전
	void RotatePoint(double angle, CRavidPoint<double> &point);

	// DXF 객체가 저장된 정보를 ROI 로 사용할 수 있도록 변환
	// 일반 객체 및 HATCH 객체 변환 가능
	void ConvertDxfToRavidPolygon(double dxfScale, double dxfAngle, CRavidPoint<double> dxfOffset);


	// 파싱 직후 저장되는 HATCH 객체 정보
	std::vector<std::vector<CRavidGeometry*>> vctVctDxfHatch;

	// 각 원소를 이루는 객체의 내부 포인터가 vctVctDxfHatch 에 해당하는 포인터와 동일하므로, 직접 메모리 접근 시 주의하여야 한다.
	// convert 된 HATCH 객체 정보
	// 각 Array의 0번: 가장 외곽의 객체
	// 각 Array의 1번 부터: 외곽의 객체를 뚫어주는 객체
	std::vector<CRavidGeometryArray> vctGaDxfHatch;

	// ROI 형성을 위한 객체의 방향을 바꾸는 함수
	// 기존 구현에서 Erase 대신 flag 삭제 방식으로 변경
	void ProcDxf(std::vector<CRavidGeometry*> &vctGeometrys, CRavidGeometryArray &gaDxfRsult);
	// ROI를 Teaching Object 등록시 생기는 문제인 중복 점을 제거하는 함수
	void TerminateDuplcatedPoint(CRavidGeometryArray &ga);
	
//////////////////////////////////////////////////////////////////////////
/*
기존 Save 함수 -> DXF file 저장 가능(AutoCad 에서 Open 가능), Layer 구분 불가
함수 이름과 기능은 기존과 동일하나, 변경 된 구조로 생성된 정보 가져와서 쓰는 방식
*/
//////////////////////////////////////////////////////////////////////////
private:
	std::ofstream m_fOut_OldVer;

public:
	bool SaveDxf(CString file);

protected:
	bool WriteDxfHeader();
	bool WriteDxfTail();

	// Basic(Hatch가 아닌 일반 객체) 과 Hatch 로 크게 나뉨
	void PrintDxfLine(std::ofstream &dxfOut, CRavidPoint<double> *rpPoint, int &index, int groupId);
	void PrintDxfCircle(std::ofstream &dxfOut, CRavidPoint<double> *rpCenter, double radius, int &index, int groupId);
	void PrintDxfArc(std::ofstream &dxfOut, CRavidPoint<double> *rpPoint, double radius, double *pRange, int &index, int groupId);
	void PrintDxfHatch(std::ofstream &dxfOut, std::vector<CRavidGeometryArray> *pVctGroup, int &index, int groupId);
	void PrintDxfBasic(std::ofstream &dxfOut, /*CRavidGeometryArray *pGaDxf, */int &index, int groupId); // 기존 CRavidGeometryArray 로 구성 되어 있던 걸 바꿔줌







//////////////////////////////////////////////////////////////////////////
/*
수정 된 구조로 재구성 -> DXF file 저장 시 도면층(Layer) 구분 가능 : CString 값으로 구분
해당 file 을 읽어와서 Ravid Geometry 로 변환 가능하나,

AutoCad 에서 Open 불가능!!!!!!!!!!!! (File Format 검토 중)
*/
//////////////////////////////////////////////////////////////////////////
private:
	//////////////////////////////////////////////////////////////////////////
	// 레이어 이름을 같이 갖고 있어야 해서 구조 바꿈
	// Pair 구성
	//  - CString : dxf 내 Layer 고유 이름
	//  - CRavidGeometry : 객체를 이루는 하나하나의 원소
	//  - CRavidPolygon : Geometry 를 통해 이어진 원소를 붙힌 객체
	std::vector<std::pair<CString, CRavidGeometry*>> m_vctDxfEliment;
	std::vector<std::pair<CString, CRavidPolygon*>> m_vctDxfObjectInfo;

public:
	// SetDxfEliment 호출 시 내부에서 m_vctDxfObjectInfo 생성함
	// 주의 : Geometry 객체를 따로 할당하지 않기 때문에, 이 함수 호출부에서 메모리 해제를 하면 안된다.
	bool SetDxfEliment(std::vector<std::pair<CString, CRavidGeometry*>>* pDxfEliment);

	// Dxf Entities 관련 변수 복사 반환
	bool GetDxfEliment(_Out_ std::vector<std::pair<CString, CRavidGeometry*>>* pDxfEliment);
	bool GetDxfEliment(_Out_ std::vector<std::pair<CString, CRavidGeometry*>>& vctDxfEliment);

	bool GetDxfObject(_Out_ std::vector<std::pair<CString, CRavidPolygon*>>* pDxfObjet);
	bool GetDxfObject(_Out_ std::vector<std::pair<CString, CRavidPolygon*>>& vctDxfObjet);

	// SetDxfEliment 와는 다르게 Geometry 객체를 할당하여 추가한다.
	bool AddDxfEliment(CString strLayerName, CRavidLine<double>* prlEliment);
	bool AddDxfEliment(CString strLayerName, CRavidLine<double>& rlEliment);
	bool AddDxfEliment(CString strLayerName, CRavidCircle<double>* prcEliment);
	bool AddDxfEliment(CString strLayerName, CRavidCircle<double>& rcEliment);
	bool AddDxfEliment(CString strLayerName, CRavidEllipse<double>* preEliment);	//radius1 == radius2 인 경우만 구현이 되어 있음
	bool AddDxfEliment(CString strLayerName, CRavidEllipse<double>& reEliment);		//radius1 == radius2 인 경우만 구현이 되어 있음

	// Layer Name 에 해당하는 Geometry 를 반환
	bool GetDxfEntityEliment(CString strLayerName, _Out_ std::vector<CRavidGeometry*>* pDxfEliment);
	bool GetDxfEntityEliment(CString strLayerName, _Out_ std::vector<std::pair<CString, CRavidGeometry*>>* pDxfEliment);

	bool GetDxfLayerNames(_Out_ std::vector<CString>* pvctLayerName);

	bool DeleteDxfEliment(CString strLayerName);
	bool DeleteDxfEliment(int nDxfElimentIndex);
	bool DeleteDxfEliment(int nDeleteRangeBeginIdx, int nDeleteRangeEndIdx);

	void ClearDxf_NewVer();
	void ClearDxf_DxfEliment();
	void ClearDxf_DxfObject();

	size_t GetDxfElimentCount();

protected:
	//////////////////////////////////////////////////////////////////////////
	// 기존 함수와 이름, 기능은 동일함 (매개변수 -> 새로 구성한 멤버변수로 변경)
	void ProcDxf();
	void TerminateDuplcatedPoint();

	// 기존 함수에서 사용하는 의도는 모르겠지만 일단 구조가 바꼈으니 변경
	void GetMinMax_NewVer(bool bFirstFlag, double &leftMin, double &topMin, double &rightMax, double &bottomMax);

//////////////////////////////////////////////////////////////////////////
//	기존 save 함수에 멤버변수를 전달인자로 쓰는것도 많고 하드코딩 난무해서 도대체가 이해할수가 없음. 싹 다 뜯어 고침
//		-> 버그 발생 : AutoCad 에서 저장 FILE OPEN 불가
public:
	bool SaveDxf_NewVer(CString strFilePathName, bool bOverWriteFile = true);
private:
	mutable std::ofstream m_fSaveFile;
	   
protected:
	bool SaveDxf_OpenFile(std::string strPath);
	bool SaveDxf_OpenFile(CString strPath);
	bool SaveDxf_CloseFile();

	void SaveDxf_WriteHeader();
	void SaveDxf_WriteHeader_Variables(const char* strVariable);

	void SaveDxf_WriteClasses();

	void SaveDxf_WriteTable(std::vector<CString>& vctLayerNames, std::vector<CString>& vctBlockRecordName);
	void SaveDxf_WriteTable_LineType(CString strDataName);
	void SaveDxf_WriteTable_LineType(std::string strDataName);
	void SaveDxf_WriteTable_Layer(CString strLayerName);
	void SaveDxf_WriteTable_Layer(std::string strLayerName);
	//bool SaveDxf_WriteHeader_LayerSetting(std::ofstream &fOut, std::vector<CString>& vctLayerNames, int &nIndex);
	void SaveDxf_WriteTable_Style(unsigned long &styleHandleStd);
	void SaveDxf_WriteTable_View();
	void SaveDxf_WriteTable_Ucs();
	void SaveDxf_WriteTable_Appid();
	void SaveDxf_WriteTable_DimStyle(unsigned long styleHandleStd);
	void SaveDxf_WriteTable_BlockRecord(std::vector<CString> vctBlockRecordNames);
	void SaveDxf_WriteTable_BlockRecord(std::vector<std::string> vctBlockRecordNames);

	void SaveDxf_WriteBlocks(std::vector<CString>& vctBlockRecordNames);
	void SaveDxf_WriteBlocks_Block(CString strBlockName);
	void SaveDxf_WriteBlocks_Block(std::string strBlockName);

	//기존거 안건드림. 해치는 제대로 안될 가능성 높음
	bool SaveDxf_WriteHatch(int &nIndex);
	bool SaveDxf_WriteHatch_Head(int nPolygonElimentCountOfTargetEntity, CString strEntitiesLayerName, int &nIndex);
	bool SaveDxf_WriteHatch_Line(CRavidLine<double> *prl);
	bool SaveDxf_WriteHatch_Arc(CRavidCircle<double> *prc);
	bool SaveDxf_WriteHatch_Tail();

	void SaveDxf_WriteEntities(std::vector<std::pair<CString, CRavidGeometry *>>& vctEntities);
	void SaveDxf_WriteEntities_Line(CRavidLine<double> *prl, CString strEntitiesLayerName);
	void SaveDxf_WriteEntities_Circle(CRavidCircle<double> *prc, CString strEntitiesLayerName);
	void SaveDxf_WriteEntities_Circle(CRavidPoint<double> *prcCenterPoint, double dblRadius, CString strEntitiesLayerName);	//Ellipse 로 정의 된 Circle 이 있어서 오버로딩
	void SaveDxf_WriteEntities_Arc(CRavidCircle<double> *prc, double *pAngleRange, CString strEntitiesLayerName);
	void SaveDxf_WriteEntities_Arc(CRavidPoint<double> *prcCenterPoint, double dblRadius, double *pAngleRange, CString strEntitiesLayerName);	//Ellipse 인자로 구성 된 Arc 가 있어서 오버로딩
	void SaveDxf_WriteEntities_Entity_LayerType(CString strEntitiesLayerName);
	void SaveDxf_WriteEntities_Entity_LayerType(std::string strEntitiesLayerName);

	//bool SaveDxf_WriteBasic(std::ofstream &fOut, int &nIndex);
	//bool SaveDxf_WriteBasic_Line(std::ofstream &fOut, CRavidLine<double> *prl, CString strEntitiesLayerName, int&nIndex);
	//bool SaveDxf_WriteBasic_Circle(std::ofstream &fOut, CRavidCircle<double> *prc, CString strEntitiesLayerName, int&nIndex);
	//bool SaveDxf_WriteBasic_Circle(std::ofstream &fOut, CRavidPoint<double> *prcCenterPoint, double dblRadius, CString strEntitiesLayerName, int&nIndex);	//Ellipse 로 정의 된 Circle 이 있어서 오버로딩
	//bool SaveDxf_WriteBasic_Arc(std::ofstream &fOut, CRavidCircle<double> *prc, double *pAngleRange, CString strEntitiesLayerName, int&nIndex);
	//bool SaveDxf_WriteBasic_Arc(std::ofstream &fOut, CRavidPoint<double> *prcCenterPoint, double dblRadius, double *pAngleRange, CString strEntitiesLayerName, int&nIndex);	//Ellipse 인자로 구성 된 Arc 가 있어서 오버로딩

	void SaveDxf_WriteObject(CString strAppDictionaryName);
	void SaveDxf_WriteObject(std::string strAppDictionaryName);
	
	void SaveDxf_WriteEnd();

	//bool SaveDxf_WriteTail(std::ofstream &fOut);

private:
	void dxfReal(int gc, double value) const;
	void dxfInt(int gc, int value) const;
	void dxfHex(int gc, int value) const;
	void dxfString(int gc, const char* value) const;
	void dxfString(int gc, const std::string& value) const;
	void comment(const char* text) const;

	unsigned long m_ulHandle = 0x30;
	unsigned long dxfHandle(int nGroupCode = 5);

	unsigned long m_ulAppDictionaryHandle = 0;
};