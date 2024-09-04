#pragma once

// 모델 변수 생성용 키워드
#define ModelParam(TYPE,VARNAME) void* _RTTI_##VARNAME##_ = (type_info*)&typeid(TYPE); TYPE VARNAME;

// 배열형 모델 변수 생성용 키워드
#define ModelParamArray(TYPE,VARNAME,COUNT) void* _RTTI_##VARNAME##_ = (type_info*)&typeid(TYPE[COUNT]); TYPE VARNAME[COUNT];

#pragma region Ravid Algorithm Select
enum ERavidParameterAlgorithmSelect
{
	ERavidParameterAlgorithmSelect_None,

	//Gauge
	ERavidParameterAlgorithmSelect_PointGauge,
	ERavidParameterAlgorithmSelect_LineGauge,
	ERavidParameterAlgorithmSelect_RectangleGauge,
	ERavidParameterAlgorithmSelect_QuadrangleGauge,
	ERavidParameterAlgorithmSelect_CircleGauge,
	ERavidParameterAlgorithmSelect_EllipseGauge,

	//Image Processing
	ERavidParameterAlgorithmSelect_Morphology,

	ERavidParameterAlgorithmSelect_SingleThreshold,
	ERavidParameterAlgorithmSelect_DoubleThreshold,
	ERavidParameterAlgorithmSelect_OtsuThreshold,
	ERavidParameterAlgorithmSelect_AdaptiveThreshold,
	ERavidParameterAlgorithmSelect_ISODATAThreshold,
	ERavidParameterAlgorithmSelect_MaximumEntropyThreshold,

	ERavidParameterAlgorithmSelect_HarrisCornersDetector,
	ERavidParameterAlgorithmSelect_HoughTransformCircle,
	ERavidParameterAlgorithmSelect_HoughTransformLine,
	ERavidParameterAlgorithmSelect_Move,
	ERavidParameterAlgorithmSelect_Rotate,
	ERavidParameterAlgorithmSelect_Scale,
	ERavidParameterAlgorithmSelect_Operation,
	ERavidParameterAlgorithmSelect_Projection,
	ERavidParameterAlgorithmSelect_Stretch,

	ERavidParameterAlgorithmSelect_Gain,
	
	//RGB 지원 보류
	//ERavidParameterAlgorithmSelect_SingleThresholdRGB,
	//ERavidParameterAlgorithmSelect_DoubleThresholdRGB,
	//ERavidParameterAlgorithmSelect_OtsuThresholdRGB,
	//ERavidParameterAlgorithmSelect_AdaptiveThresholdRGB,
	//ERavidParameterAlgorithmSelect_ISODATAThresholdRGB,
	//ERavidParameterAlgorithmSelect_MaximumEntropyThresholdRGB,

	//Object Blob
	ERavidParameterAlgorithmSelect_ConnectedComponentSingle,
	ERavidParameterAlgorithmSelect_ConnectedComponentDouble,
	ERavidParameterAlgorithmSelect_ConnectedComponentFilter,

	//Finder
	ERavidParameterAlgorithmSelect_ObjectFinder,
	ERavidParameterAlgorithmSelect_ShapeFinder,

	//Code (Decoder)
	ERavidParameterAlgorithmSelect_Barcode,
	ERavidParameterAlgorithmSelect_DatamatrixNormal,
	ERavidParameterAlgorithmSelect_DatamatrixExpert,
	ERavidParameterAlgorithmSelect_QRcodeNormal,
	ERavidParameterAlgorithmSelect_QRcodeExpert,




	ERavidParameterAlgorithmSelect_Count,
};

static LPCTSTR GetRavidParameterAlgorithmSelectToString(_In_ ERavidParameterAlgorithmSelect eSelection)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmSelect_Count] =
	{
		_T("None"),

		//Gauge
		_T("Point Gauge"),
		_T("Line Gauge"),
		_T("Rectangle Gauge"),
		_T("Quadrangle Gauge"),
		_T("Circle Gauge"),
		_T("Ellipse Gauge"),

		//Image Processing
		_T("Morphology"),
		_T("Single Threshold"),
		_T("Double Threshold"),
		_T("Otsu Threshold"),
		_T("Adaptive Threshold"),
		_T("ISODATA Threshold"),
		_T("Maximum Entropy Threshold"),
		_T("Harris Corners Detector"),
		_T("Hough Transform Circle"),
		_T("Hough Transform Line"),
		_T("Move"),
		_T("Rotate"),
		_T("Scale"),
		_T("Operation"),
		_T("Projection"),
		_T("Stretch"),
		_T("Gain"),

		//_T("Single Threshold_RGB"),
		//_T("Double Threshold_RGB"),
		//_T("Otsu Threshold_RGB"),
		//_T("Adaptive Threshold_RGB"),
		//_T("ISODATA Threshold_RGB"),
		//_T("Maximum Entropy Threshold_RGB"),

		//Object Blob
		_T("Connected Component Single"),
		_T("Connected Component Double"),
		_T("Connected Component Filter"),

		//Finder
		_T("Object Finder"),
		_T("Shape Finder"),

		//Code
		_T("Barcode"),
		_T("Datamatrix Normal"),
		_T("Datamatrix Expert"),
		_T("QRcode Normal"),
		_T("QRcode Expert"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSelection;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmSelect_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}
static ERavidParameterAlgorithmSelect GetRavidParameterAlgorithmStringToSelectNum(_In_ CString strSelection)
{
	ERavidParameterAlgorithmSelect eReturn = ERavidParameterAlgorithmSelect_None;

	for(int i = 0; i < ERavidParameterAlgorithmSelect_Count; ++i)
	{
		if(!strSelection.CompareNoCase(GetRavidParameterAlgorithmSelectToString((ERavidParameterAlgorithmSelect)i)))
		{
			eReturn = (ERavidParameterAlgorithmSelect)i;
			break;
		}
	}

	return eReturn;
}
#pragma endregion

#pragma region Show Struct Type
static LPCTSTR GetRavidParameterAlgorithmSelectShowStruct(_In_ ERavidParameterAlgorithmSelect eSelection)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmSelect_Count] =
	{
		_T("//User didn't Select the Ravid Algorithm."),

		_T("SRavidAlgorithmPointGaugeParam"),
		_T("SRavidAlgorithmLineGaugeParam"),
		_T("SRavidAlgorithmRectangleGaugeParam"),
		_T("SRavidAlgorithmQuadrangleGaugeParam"),
		_T("SRavidAlgorithmCircleGaugeParam"),
		_T("SRavidAlgorithmEllipseGaugeParam"),

		_T("SRavidAlgorithmMorphologyParam"),
		_T("SRavidAlgorithmSingleThresholdParam"),
		_T("SRavidAlgorithmDoubleThresholdParam"),
		_T("SRavidAlgorithmOtsuThresholdParam"),
		_T("SRavidAlgorithmAdaptiveThresholdParam"),
		_T("SRavidAlgorithmISODATAThresholdParam"),
		_T("SRavidAlgorithmMaximumEntropyThresholdParam"),
		_T("SRavidAlgorithmHarrisCornersDetectorParam"),
		_T("SRavidAlgorithmHoughTransformCircleParam"),
		_T("SRavidAlgorithmHoughTransformLineParam"),
		_T("SRavidAlgorithmMoveParam"),
		_T("SRavidAlgorithmRotateParam"),
		_T("SRavidAlgorithmScaleParam"),
		_T("SRavidAlgorithmOperationParam"),
		_T("SRavidAlgorithmProjectionParam"),
		_T("SRavidAlgorithmStretchParam"),
		_T("SRavidAlgorithmGainParam"),


		//_T("SRavidAlgorithmSingleThresholdRGBParam"),
		//_T("SRavidAlgorithmDoubleThresholdRGBParam"),
		//_T("SRavidAlgorithmOtsuThresholdRGBParam"),
		//_T("SRavidAlgorithmAdaptiveThresholdRGBParam"),
		//_T("SRavidAlgorithmISODATAThresholdRGBParam"),
		//_T("SRavidAlgorithmMaximumEntropyThresholdRGBParam"),

		_T("SRavidAlgorithmConnectedComponentSingleParam"),
		_T("SRavidAlgorithmConnectedComponentDoubleParam"),
		_T("SRavidAlgorithmConnectedComponentFilterParam"),

		_T("SRavidAlgorithmObjectFinderParam"),
		_T("SRavidAlgorithmShapeFinderParam"),

		_T("SRavidAlgorithmBarcodeParam"),
		_T("SRavidAlgorithmDatamatrixNormalParam"),
		_T("SRavidAlgorithmDatamatrixExpertParam"),
		_T("SRavidAlgorithmQRcodeNormalParam"),
		_T("SRavidAlgorithmQRcodeExpertParam"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSelection;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmSelect_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}
#pragma endregion

#pragma region Gauge

#pragma region Point Gauge
enum ERavidParameterAlgorithmPointGauge
{
	ERavidParameterAlgorithmPointGauge_TransitionType,
	ERavidParameterAlgorithmPointGauge_TransitionChoice,
	ERavidParameterAlgorithmPointGauge_DoubleThreshold,
	ERavidParameterAlgorithmPointGauge_MinimunAmplitude,
	ERavidParameterAlgorithmPointGauge_Thickness,
	ERavidParameterAlgorithmPointGauge_Tolerance,
	ERavidParameterAlgorithmPointGauge_Count,
};
static LPCTSTR GetRavidParameterAlgorithmPointGaugeParamNameString(_In_ ERavidParameterAlgorithmPointGauge eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmPointGauge_Count] =
	{
	_T("Transition Type"),
	_T("Transition Choice"),
	_T("DoubleThreshold"),
	_T("Minimun Amplitude"),
	_T("Thickness"),
	_T("Tolerance"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmPointGauge_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmPointGaugeParamDataTypeString(_In_ ERavidParameterAlgorithmPointGauge eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmPointGauge_Count] =
	{
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("DOUBLE"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmPointGauge_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmPointGaugeParamInputBoxTypeString(_In_ ERavidParameterAlgorithmPointGauge eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmPointGauge_Count] =
	{
		_T("COMBO"),
		_T("COMBO"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmPointGauge_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmPointGaugeParamExtraDataString(_In_ ERavidParameterAlgorithmPointGauge eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmPointGauge_Count] =
	{
		_T("Black to White;White to Black;Black to White or White to Black;Black to White to Black;White to Black to White"),
		_T("Begin;End;Largest Amplitude;Largest Area;Closest;All"),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmPointGauge_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

#pragma pack(push, 1)
typedef struct sRavidAlgorithmPointGaugeParam
{
	ModelParam(long, nTransitionType);
	ModelParam(long, nTransitionChoice);
	ModelParam(long, nDoubleThreshold);
	ModelParam(long, nMinimunAmplitude);
	ModelParam(long, nThickness);
	ModelParam(double, dblTolerance);
}
SRavidAlgorithmPointGaugeParam;
#pragma pack(pop)
#pragma endregion

#pragma region Line Gauge
enum ERavidParameterAlgorithmLineGauge
{
	ERavidParameterAlgorithmLineGauge_TransitionType,
	ERavidParameterAlgorithmLineGauge_TransitionChoice,
	ERavidParameterAlgorithmLineGauge_DoubleThreshold,
	ERavidParameterAlgorithmLineGauge_MinimunAmplitude,
	ERavidParameterAlgorithmLineGauge_Thickness,
	ERavidParameterAlgorithmLineGauge_SamplingStep,
	ERavidParameterAlgorithmLineGauge_OutliersDoubleThreshold,
	ERavidParameterAlgorithmLineGauge_NumOfPass,
	ERavidParameterAlgorithmLineGauge_Tolerance,
	ERavidParameterAlgorithmLineGauge_Count,
};
static LPCTSTR GetRavidParameterAlgorithmLineGaugeParamNameString(_In_ ERavidParameterAlgorithmLineGauge eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmLineGauge_Count] =
	{
	_T("Transition Type"),
	_T("Transition Choice"),
	_T("DoubleThreshold"),
	_T("Minimun Amplitude"),
	_T("Thickness"),
	_T("Sampling Step"),
	_T("Outliers DoubleThreshold"),
	_T("Num of Pass"),
	_T("Tolerance"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmLineGauge_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmLineGaugeParamDataTypeString(_In_ ERavidParameterAlgorithmLineGauge eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmLineGauge_Count] =
	{
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("DOUBLE"),
		_T("DOUBLE"),
		_T("LONG"),
		_T("DOUBLE"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmLineGauge_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmLineGaugeParamInputBoxTypeString(_In_ ERavidParameterAlgorithmLineGauge eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmLineGauge_Count] =
	{
		_T("COMBO"),
		_T("COMBO"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmLineGauge_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmLineGaugeParamExtraDataString(_In_ ERavidParameterAlgorithmLineGauge eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmLineGauge_Count] =
	{
		_T("Black to White;White to Black;Black to White or White to Black;Black to White to Black;White to Black to White"),
		_T("Begin;End;Largest Amplitude;Largest Area;Closest"),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmLineGauge_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

#pragma pack(push, 1)
typedef struct sRavidAlgorithmLineGaugeParam
{
	ModelParam(long, nTransitionType);
	ModelParam(long, nTransitionChoice);
	ModelParam(long, nDoubleThreshold);
	ModelParam(long, nMinimunAmplitude);
	ModelParam(long, nThickness);
	ModelParam(double, dblSamplingStep);
	ModelParam(double, dblOutliersDoubleThreshold);
	ModelParam(long, nNumOfPass);
	ModelParam(double, dblTolerance);
}
SRavidAlgorithmLineGaugeParam;
#pragma pack(pop)
#pragma endregion

#pragma region Rectangle Gauge
enum ERavidParameterAlgorithmRectangleGauge
{
	ERavidParameterAlgorithmRectangleGauge_TransitionType,
	ERavidParameterAlgorithmRectangleGauge_TransitionChoice,
	ERavidParameterAlgorithmRectangleGauge_DoubleThreshold,
	ERavidParameterAlgorithmRectangleGauge_MinimunAmplitude,
	ERavidParameterAlgorithmRectangleGauge_Thickness,
	ERavidParameterAlgorithmRectangleGauge_SamplingStep,
	ERavidParameterAlgorithmRectangleGauge_OutliersDoubleThreshold,
	ERavidParameterAlgorithmRectangleGauge_NumOfPass,
	ERavidParameterAlgorithmRectangleGauge_Tolerance,
	ERavidParameterAlgorithmRectangleGauge_Count,
};
static LPCTSTR GetRavidParameterAlgorithmRectangleGaugeParamNameString(_In_ ERavidParameterAlgorithmRectangleGauge eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmRectangleGauge_Count] =
	{
	_T("Transition Type"),
	_T("Transition Choice"),
	_T("DoubleThreshold"),
	_T("Minimun Amplitude"),
	_T("Thickness"),
	_T("Sampling Step"),
	_T("Outliers DoubleThreshold"),
	_T("Num of Pass"),
	_T("Tolerance"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmRectangleGauge_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmRectangleGaugeParamDataTypeString(_In_ ERavidParameterAlgorithmRectangleGauge eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmRectangleGauge_Count] =
	{
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("DOUBLE"),
		_T("DOUBLE"),
		_T("LONG"),
		_T("DOUBLE"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmRectangleGauge_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmRectangleGaugeParamInputBoxTypeString(_In_ ERavidParameterAlgorithmRectangleGauge eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmRectangleGauge_Count] =
	{
		_T("COMBO"),
		_T("COMBO"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmRectangleGauge_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmRectangleGaugeParamExtraDataString(_In_ ERavidParameterAlgorithmRectangleGauge eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmRectangleGauge_Count] =
	{
		_T("Black to White;White to Black;Black to White or White to Black;Black to White to Black;White to Black to White"),
		_T("Begin;End;Largest Amplitude;Largest Area;Closest"),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmRectangleGauge_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

#pragma pack(push, 1)
typedef struct sRavidAlgorithmRectangleGaugeParam
{
	ModelParam(long, nTransitionType);
	ModelParam(long, nTransitionChoice);
	ModelParam(long, nDoubleThreshold);
	ModelParam(long, nMinimunAmplitude);
	ModelParam(long, nThickness);
	ModelParam(double, dblSamplingStep);
	ModelParam(double, dblOutliersDoubleThreshold);
	ModelParam(long, nNumOfPass);
	ModelParam(double, dblTolerance);
}
SRavidAlgorithmRectangleGaugeParam;
#pragma pack(pop)
#pragma endregion

#pragma region Quadrangle Gauge
enum ERavidParameterAlgorithmQuadrangleGauge
{
	ERavidParameterAlgorithmQuadrangleGauge_TransitionType,
	ERavidParameterAlgorithmQuadrangleGauge_TransitionChoice,
	ERavidParameterAlgorithmQuadrangleGauge_DoubleThreshold,
	ERavidParameterAlgorithmQuadrangleGauge_MinimunAmplitude,
	ERavidParameterAlgorithmQuadrangleGauge_Thickness,
	ERavidParameterAlgorithmQuadrangleGauge_SamplingStep,
	ERavidParameterAlgorithmQuadrangleGauge_OutliersDoubleThreshold,
	ERavidParameterAlgorithmQuadrangleGauge_NumOfPass,
	ERavidParameterAlgorithmQuadrangleGauge_Tolerance,
	ERavidParameterAlgorithmQuadrangleGauge_Count,
};
static LPCTSTR GetRavidParameterAlgorithmQuadrangleGaugeParamNameString(_In_ ERavidParameterAlgorithmQuadrangleGauge eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmQuadrangleGauge_Count] =
	{
	_T("Transition Type"),
	_T("Transition Choice"),
	_T("DoubleThreshold"),
	_T("Minimun Amplitude"),
	_T("Thickness"),
	_T("Sampling Step"),
	_T("Outliers DoubleThreshold"),
	_T("Num of Pass"),
	_T("Tolerance"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmQuadrangleGauge_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmQuadrangleGaugeParamDataTypeString(_In_ ERavidParameterAlgorithmQuadrangleGauge eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmQuadrangleGauge_Count] =
	{
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("DOUBLE"),
		_T("DOUBLE"),
		_T("LONG"),
		_T("DOUBLE"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmQuadrangleGauge_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmQuadrangleGaugeParamInputBoxTypeString(_In_ ERavidParameterAlgorithmQuadrangleGauge eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmQuadrangleGauge_Count] =
	{
		_T("COMBO"),
		_T("COMBO"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmQuadrangleGauge_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmQuadrangleGaugeParamExtraDataString(_In_ ERavidParameterAlgorithmQuadrangleGauge eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmQuadrangleGauge_Count] =
	{
		_T("Black to White;White to Black;Black to White or White to Black;Black to White to Black;White to Black to White"),
		_T("Begin;End;Largest Amplitude;Largest Area;Closest"),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmQuadrangleGauge_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

#pragma pack(push, 1)
typedef struct sRavidAlgorithmQuadrangleGaugeParam
{
	ModelParam(long, nTransitionType);
	ModelParam(long, nTransitionChoice);
	ModelParam(long, nDoubleThreshold);
	ModelParam(long, nMinimunAmplitude);
	ModelParam(long, nThickness);
	ModelParam(double, dblSamplingStep);
	ModelParam(double, dblOutliersDoubleThreshold);
	ModelParam(long, nNumOfPass);
	ModelParam(double, dblTolerance);
}
SRavidAlgorithmQuadrangleGaugeParam;
#pragma pack(pop)
#pragma endregion

#pragma region Circle Gauge
enum ERavidParameterAlgorithmCircleGauge
{
	ERavidParameterAlgorithmCircleGauge_TransitionType,
	ERavidParameterAlgorithmCircleGauge_TransitionChoice,
	ERavidParameterAlgorithmCircleGauge_DoubleThreshold,
	ERavidParameterAlgorithmCircleGauge_MinimunAmplitude,
	ERavidParameterAlgorithmCircleGauge_Thickness,
	ERavidParameterAlgorithmCircleGauge_SamplingStep,
	ERavidParameterAlgorithmCircleGauge_OutliersDoubleThreshold,
	ERavidParameterAlgorithmCircleGauge_NumOfPass,
	ERavidParameterAlgorithmCircleGauge_Tolerance,
	ERavidParameterAlgorithmCircleGauge_Count,
};
static LPCTSTR GetRavidParameterAlgorithmCircleGaugeParamNameString(_In_ ERavidParameterAlgorithmCircleGauge eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmCircleGauge_Count] =
	{
	_T("Transition Type"),
	_T("Transition Choice"),
	_T("DoubleThreshold"),
	_T("Minimun Amplitude"),
	_T("Thickness"),
	_T("Sampling Step"),
	_T("Outliers DoubleThreshold"),
	_T("Num of Pass"),
	_T("Tolerance"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmCircleGauge_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmCircleGaugeParamDataTypeString(_In_ ERavidParameterAlgorithmCircleGauge eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmCircleGauge_Count] =
	{
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("DOUBLE"),
		_T("DOUBLE"),
		_T("LONG"),
		_T("DOUBLE"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmCircleGauge_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmCircleGaugeParamInputBoxTypeString(_In_ ERavidParameterAlgorithmCircleGauge eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmCircleGauge_Count] =
	{
		_T("COMBO"),
		_T("COMBO"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmCircleGauge_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmCircleGaugeParamExtraDataString(_In_ ERavidParameterAlgorithmCircleGauge eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmCircleGauge_Count] =
	{
		_T("Black to White;White to Black;Black to White or White to Black;Black to White to Black;White to Black to White"),
		_T("Begin;End;Largest Amplitude;Largest Area;Closest"),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmCircleGauge_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

#pragma pack(push, 1)
typedef struct sRavidAlgorithmCircleGaugeParam
{
	ModelParam(long, nTransitionType);
	ModelParam(long, nTransitionChoice);
	ModelParam(long, nDoubleThreshold);
	ModelParam(long, nMinimunAmplitude);
	ModelParam(long, nThickness);
	ModelParam(double, dblSamplingStep);
	ModelParam(double, dblOutliersDoubleThreshold);
	ModelParam(long, nNumOfPass);
	ModelParam(double, dblTolerance);
}
SRavidAlgorithmCircleGaugeParam;
#pragma pack(pop)
#pragma endregion

#pragma region Ellipse Gauge
enum ERavidParameterAlgorithmEllipseGauge
{
	ERavidParameterAlgorithmEllipseGauge_TransitionType,
	ERavidParameterAlgorithmEllipseGauge_TransitionChoice,
	ERavidParameterAlgorithmEllipseGauge_DoubleThreshold,
	ERavidParameterAlgorithmEllipseGauge_MinimunAmplitude,
	ERavidParameterAlgorithmEllipseGauge_Thickness,
	ERavidParameterAlgorithmEllipseGauge_SamplingStep,
	ERavidParameterAlgorithmEllipseGauge_OutliersDoubleThreshold,
	ERavidParameterAlgorithmEllipseGauge_NumOfPass,
	ERavidParameterAlgorithmEllipseGauge_Tolerance,
	ERavidParameterAlgorithmEllipseGauge_Count,
};
static LPCTSTR GetRavidParameterAlgorithmEllipseGaugeParamNameString(_In_ ERavidParameterAlgorithmEllipseGauge eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmEllipseGauge_Count] =
	{
	_T("Transition Type"),
	_T("Transition Choice"),
	_T("DoubleThreshold"),
	_T("Minimun Amplitude"),
	_T("Thickness"),
	_T("Sampling Step"),
	_T("Outliers DoubleThreshold"),
	_T("Num of Pass"),
	_T("Tolerance"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmEllipseGauge_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmEllipseGaugeParamDataTypeString(_In_ ERavidParameterAlgorithmEllipseGauge eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmEllipseGauge_Count] =
	{
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("DOUBLE"),
		_T("DOUBLE"),
		_T("LONG"),
		_T("DOUBLE"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmEllipseGauge_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmEllipseGaugeParamInputBoxTypeString(_In_ ERavidParameterAlgorithmEllipseGauge eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmEllipseGauge_Count] =
	{
		_T("COMBO"),
		_T("COMBO"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmEllipseGauge_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmEllipseGaugeParamExtraDataString(_In_ ERavidParameterAlgorithmEllipseGauge eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmEllipseGauge_Count] =
	{
		_T("Black to White;White to Black;Black to White or White to Black;Black to White to Black;White to Black to White"),
		_T("Begin;End;Largest Amplitude;Largest Area;Closest"),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmEllipseGauge_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

#pragma pack(push, 1)
typedef struct sRavidAlgorithmEllipseGaugeParam
{
	ModelParam(long, nTransitionType);
	ModelParam(long, nTransitionChoice);
	ModelParam(long, nDoubleThreshold);
	ModelParam(long, nMinimunAmplitude);
	ModelParam(long, nThickness);
	ModelParam(double, dblSamplingStep);
	ModelParam(double, dblOutliersDoubleThreshold);
	ModelParam(long, nNumOfPass);
	ModelParam(double, dblTolerance);
}
SRavidAlgorithmEllipseGaugeParam;
#pragma pack(pop)
#pragma endregion

#pragma endregion

#pragma region Image Processing

#pragma region Morphology
enum ERavidParameterAlgorithmMorphology
{
	ERavidParameterAlgorithmMorphology_MorphologyType,
	ERavidParameterAlgorithmMorphology_HalfWidth,
	ERavidParameterAlgorithmMorphology_HalfHeight,
	ERavidParameterAlgorithmMorphology_Count,
};
static LPCTSTR GetRavidParameterAlgorithmMorphologyParamNameString(_In_ ERavidParameterAlgorithmMorphology eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmMorphology_Count] =
	{
	_T("Morphology Type"),
	_T("Half Width"),
	_T("Half Height"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmMorphology_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmMorphologyParamDataTypeString(_In_ ERavidParameterAlgorithmMorphology eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmMorphology_Count] =
	{
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmMorphology_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmMorphologyParamInputBoxTypeString(_In_ ERavidParameterAlgorithmMorphology eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmMorphology_Count] =
	{
		_T("COMBO"),
		_T("EDIT"),
		_T("EDIT"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmMorphology_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmMorphologyParamExtraDataString(_In_ ERavidParameterAlgorithmMorphology eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmMorphology_Count] =
	{
		_T("Erode;Dilate;Open;Close;Gradient;White Top Hat;Black Top Hat;Median 3x3 (HalfWidth and HalfHeight : -1);Median 5x5 (HalfWidth and HalfHeight : -1);Median 7x7 (HalfWidth and HalfHeight : -1);Median MxN"),
		_T(""),
		_T(""),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmMorphology_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

#pragma pack(push, 1)
typedef struct sRavidAlgorithmMorphologyParam
{
	ModelParam(long, nMorphologyType);
	ModelParam(long, nHalfWidth);
	ModelParam(long, nHalfHeigtht);
}
SRavidAlgorithmMorphologyParam;
#pragma pack(pop)
#pragma endregion

#pragma region SingleThreshold
enum ERavidParameterAlgorithmSingleThreshold
{
	ERavidParameterAlgorithmSingleThreshold_Threshold,
	ERavidParameterAlgorithmSingleThreshold_Condition,
	ERavidParameterAlgorithmSingleThreshold_RangeFillValue,
	ERavidParameterAlgorithmSingleThreshold_OutOfRangeFillValue,
	ERavidParameterAlgorithmSingleThreshold_LogicalOperation,
	ERavidParameterAlgorithmSingleThreshold_Count,
};
static LPCTSTR GetRavidParameterAlgorithmSingleThresholdParamNameString(_In_ ERavidParameterAlgorithmSingleThreshold eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmSingleThreshold_Count] =
	{
	_T("Threshold"),
	_T("Condition"),
	_T("Range Fill Value"),
	_T("Out of Range Fill Value"),
	_T("Logical Operation"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmSingleThreshold_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmSingleThresholdParamDataTypeString(_In_ ERavidParameterAlgorithmSingleThreshold eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmSingleThreshold_Count] =
	{
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmSingleThreshold_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmSingleThresholdParamInputBoxTypeString(_In_ ERavidParameterAlgorithmSingleThreshold eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmSingleThreshold_Count] =
	{
		_T("EDIT"),
		_T("COMBO"),
		_T("EDIT"),
		_T("EDIT"),
		_T("COMBO"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmSingleThreshold_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmSingleThresholdParamExtraDataString(_In_ ERavidParameterAlgorithmSingleThreshold eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmSingleThreshold_Count] =
	{
		_T(""),
		_T("Greater;GreaterEqual;Equal;NotEqual;LessEqual;Less"),
		_T(""),
		_T(""),
		_T("None;AND;OR"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmSingleThreshold_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

#pragma pack(push, 1)
typedef struct sRavidAlgorithmSingleThresholdParam
{
	ModelParam(long, nThreshold);
	ModelParam(long, nCondition);
	ModelParam(long, nRangeFillValue);
	ModelParam(long, nOutOfRangeFillValue);
	ModelParam(long, nLogicalOpertion);
}
SRavidAlgorithmSingleThresholdParam;
#pragma pack(pop)
#pragma endregion

#pragma region DoubleThreshold
enum ERavidParameterAlgorithmDoubleThreshold
{
	ERavidParameterAlgorithmDoubleThreshold_LowThreshold,
	ERavidParameterAlgorithmDoubleThreshold_HighThreshold,
	ERavidParameterAlgorithmDoubleThreshold_RangeFillValue,
	ERavidParameterAlgorithmDoubleThreshold_LessThanRangeFillValue,
	ERavidParameterAlgorithmDoubleThreshold_GreaterThanRangeFillValue,
	ERavidParameterAlgorithmDoubleThreshold_Count,
};
static LPCTSTR GetRavidParameterAlgorithmDoubleThresholdParamNameString(_In_ ERavidParameterAlgorithmDoubleThreshold eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmDoubleThreshold_Count] =
	{
	_T("Low Threshold"),
	_T("High Threshold"),
	_T("Range Fill Value"),
	_T("Less than Range Fill Value"),
	_T("Greater than Range Fill Value"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmDoubleThreshold_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmDoubleThresholdParamDataTypeString(_In_ ERavidParameterAlgorithmDoubleThreshold eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmDoubleThreshold_Count] =
	{
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmDoubleThreshold_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmDoubleThresholdParamInputBoxTypeString(_In_ ERavidParameterAlgorithmDoubleThreshold eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmDoubleThreshold_Count] =
	{
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmDoubleThreshold_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmDoubleThresholdParamExtraDataString(_In_ ERavidParameterAlgorithmDoubleThreshold eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmDoubleThreshold_Count] =
	{
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmDoubleThreshold_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

#pragma pack(push, 1)
typedef struct sRavidAlgorithmDoubleThresholdParam
{
	ModelParam(long, nLowThreshold);
	ModelParam(long, nHighThreshold);
	ModelParam(long, nRangeFillValue);
	ModelParam(long, nLessThanRangeFillValue);
	ModelParam(long, nGreaterThanRangeFillValue);
}
SRavidAlgorithmDoubleThresholdParam;
#pragma pack(pop)
#pragma endregion

#pragma region OtsuThreshold
enum ERavidParameterAlgorithmOtsuThreshold
{
	ERavidParameterAlgorithmOtsuThreshold_Condition,
	ERavidParameterAlgorithmOtsuThreshold_RangeFillValue,
	ERavidParameterAlgorithmOtsuThreshold_OutOfRangeFillValue,
	ERavidParameterAlgorithmOtsuThreshold_Count,
};
static LPCTSTR GetRavidParameterAlgorithmOtsuThresholdParamNameString(_In_ ERavidParameterAlgorithmOtsuThreshold eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmOtsuThreshold_Count] =
	{
	_T("Condition"),
	_T("Range Fill Value"),
	_T("Out of Range Fill Value"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmOtsuThreshold_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmOtsuThresholdParamDataTypeString(_In_ ERavidParameterAlgorithmOtsuThreshold eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmOtsuThreshold_Count] =
	{
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmOtsuThreshold_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmOtsuThresholdParamInputBoxTypeString(_In_ ERavidParameterAlgorithmOtsuThreshold eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmOtsuThreshold_Count] =
	{
		_T("COMBO"),
		_T("EDIT"),
		_T("EDIT"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmOtsuThreshold_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmOtsuThresholdParamExtraDataString(_In_ ERavidParameterAlgorithmOtsuThreshold eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmOtsuThreshold_Count] =
	{
		_T("Greater;GreaterEqual;Equal;NotEqual;LessEqual;Less"),
		_T(""),
		_T(""),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmOtsuThreshold_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

#pragma pack(push, 1)
typedef struct sRavidAlgorithmOtsuThresholdParam
{
	ModelParam(long, nCondition);
	ModelParam(long, nRangeFillValue);
	ModelParam(long, nOutOfRangeFillValue);
}
SRavidAlgorithmOtsuThresholdParam;
#pragma pack(pop)
#pragma endregion

#pragma region AdaptiveThreshold
enum ERavidParameterAlgorithmAdaptiveThreshold
{
	ERavidParameterAlgorithmAdaptiveThreshold_ThresholdType,
	ERavidParameterAlgorithmAdaptiveThreshold_ThresholdMethod,
	ERavidParameterAlgorithmAdaptiveThreshold_MaxValue,
	ERavidParameterAlgorithmAdaptiveThreshold_BlockSize,
	ERavidParameterAlgorithmAdaptiveThreshold_Constant,
	ERavidParameterAlgorithmAdaptiveThreshold_Count,
};
static LPCTSTR GetRavidParameterAlgorithmAdaptiveThresholdParamNameString(_In_ ERavidParameterAlgorithmAdaptiveThreshold eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmAdaptiveThreshold_Count] =
	{
	_T("Threshold Type"),
	_T("Threshold Method"),
	_T("Max Value"),
	_T("Block Size"),
	_T("Constant"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmAdaptiveThreshold_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmAdaptiveThresholdParamDataTypeString(_In_ ERavidParameterAlgorithmAdaptiveThreshold eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmAdaptiveThreshold_Count] =
	{
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmAdaptiveThreshold_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmAdaptiveThresholdParamInputBoxTypeString(_In_ ERavidParameterAlgorithmAdaptiveThreshold eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmAdaptiveThreshold_Count] =
	{
		_T("COMBO"),
		_T("COMBO"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmAdaptiveThreshold_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmAdaptiveThresholdParamExtraDataString(_In_ ERavidParameterAlgorithmAdaptiveThreshold eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmAdaptiveThreshold_Count] =
	{
		_T("Binary;Binary_Inverse"),
		_T("Mean;Gaussian"),
		_T(""),
		_T(""),
		_T(""),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmAdaptiveThreshold_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

#pragma pack(push, 1)
typedef struct sRavidAlgorithmAdaptiveThresholdParam
{
	ModelParam(long, nThresholdType);
	ModelParam(long, nThresholdMethod);
	ModelParam(long, nMaxValue);
	ModelParam(long, nBlockSize);
	ModelParam(long, nConstant);
}
SRavidAlgorithmAdaptiveThresholdParam;
#pragma pack(pop)
#pragma endregion

#pragma region ISODATAThreshold
enum ERavidParameterAlgorithmISODATAThreshold
{
	ERavidParameterAlgorithmISODATAThreshold_Condition,
	ERavidParameterAlgorithmISODATAThreshold_RangeFillValue,
	ERavidParameterAlgorithmISODATAThreshold_OutOfRangeFillValue,
	ERavidParameterAlgorithmISODATAThreshold_Count,
};
static LPCTSTR GetRavidParameterAlgorithmISODATAThresholdParamNameString(_In_ ERavidParameterAlgorithmISODATAThreshold eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmISODATAThreshold_Count] =
	{
	_T("Condition"),
	_T("Range Fill Value"),
	_T("Out of Range Fill Value"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmISODATAThreshold_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmISODATAThresholdParamDataTypeString(_In_ ERavidParameterAlgorithmISODATAThreshold eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmISODATAThreshold_Count] =
	{
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmISODATAThreshold_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmISODATAThresholdParamInputBoxTypeString(_In_ ERavidParameterAlgorithmISODATAThreshold eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmISODATAThreshold_Count] =
	{
		_T("COMBO"),
		_T("EDIT"),
		_T("EDIT"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmISODATAThreshold_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmISODATAThresholdParamExtraDataString(_In_ ERavidParameterAlgorithmISODATAThreshold eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmISODATAThreshold_Count] =
	{
		_T("Greater;GreaterEqual;Equal;NotEqual;LessEqual;Less"),
		_T(""),
		_T(""),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmISODATAThreshold_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

#pragma pack(push, 1)
typedef struct sRavidAlgorithmISODATAThresholdParam
{
	ModelParam(long, nCondition);
	ModelParam(long, nRangeFillValue);
	ModelParam(long, nOutOfRangeFillValue);
}
SRavidAlgorithmISODATAThresholdParam;
#pragma pack(pop)
#pragma endregion

#pragma region MaximumEntropyThreshold
enum ERavidParameterAlgorithmMaximumEntropyThreshold
{
	ERavidParameterAlgorithmMaximumEntropyThreshold_Condition,
	ERavidParameterAlgorithmMaximumEntropyThreshold_RangeFillValue,
	ERavidParameterAlgorithmMaximumEntropyThreshold_OutOfRangeFillValue,
	ERavidParameterAlgorithmMaximumEntropyThreshold_Count,
};
static LPCTSTR GetRavidParameterAlgorithmMaximumEntropyThresholdParamNameString(_In_ ERavidParameterAlgorithmMaximumEntropyThreshold eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmMaximumEntropyThreshold_Count] =
	{
	_T("Condition"),
	_T("Range Fill Value"),
	_T("Out of Range Fill Value"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmMaximumEntropyThreshold_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmMaximumEntropyThresholdParamDataTypeString(_In_ ERavidParameterAlgorithmMaximumEntropyThreshold eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmMaximumEntropyThreshold_Count] =
	{
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmMaximumEntropyThreshold_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmMaximumEntropyThresholdParamInputBoxTypeString(_In_ ERavidParameterAlgorithmMaximumEntropyThreshold eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmMaximumEntropyThreshold_Count] =
	{
		_T("COMBO"),
		_T("EDIT"),
		_T("EDIT"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmMaximumEntropyThreshold_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmMaximumEntropyThresholdParamExtraDataString(_In_ ERavidParameterAlgorithmMaximumEntropyThreshold eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmMaximumEntropyThreshold_Count] =
	{
		_T("Greater;GreaterEqual;Equal;NotEqual;LessEqual;Less"),
		_T(""),
		_T(""),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmMaximumEntropyThreshold_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

#pragma pack(push, 1)
typedef struct sRavidAlgorithmMaximumEntropyThresholdParam
{
	ModelParam(long, nCondition);
	ModelParam(long, nRangeFillValue);
	ModelParam(long, nOutOfRangeFillValue);
}
SRavidAlgorithmMaximumEntropyThresholdParam;
#pragma pack(pop)
#pragma endregion

#pragma region Harris Corners Detector
enum ERavidParameterAlgorithmHarrisCornersDetector
{
	ERavidParameterAlgorithmHarrisCornersDetector_ThresholdType,
	ERavidParameterAlgorithmHarrisCornersDetector_Threshold,
	ERavidParameterAlgorithmHarrisCornersDetector_CornerCoefficient,
	ERavidParameterAlgorithmHarrisCornersDetector_Count,
};
static LPCTSTR GetRavidParameterAlgorithmHarrisCornersDetectorParamNameString(_In_ ERavidParameterAlgorithmHarrisCornersDetector eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmHarrisCornersDetector_Count] =
	{
	_T("Threshold Type"),
	_T("Threshold"),
	_T("Corner Coefficient"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmHarrisCornersDetector_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmHarrisCornersDetectorParamDataTypeString(_In_ ERavidParameterAlgorithmHarrisCornersDetector eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmHarrisCornersDetector_Count] =
	{
		_T("LONG"),
		_T("LONG"),
		_T("DOUBLE"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmHarrisCornersDetector_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmHarrisCornersDetectorParamInputBoxTypeString(_In_ ERavidParameterAlgorithmHarrisCornersDetector eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmHarrisCornersDetector_Count] =
	{
		_T("COMBO"),
		_T("EDIT"),
		_T("EDIT"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmHarrisCornersDetector_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmHarrisCornersDetectorParamExtraDataString(_In_ ERavidParameterAlgorithmHarrisCornersDetector eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmHarrisCornersDetector_Count] =
	{
		_T("Absolute;Relative"),
		_T(""),
		_T(""),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmHarrisCornersDetector_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

#pragma pack(push, 1)
typedef struct sRavidAlgorithmHarrisCornersDetectorParam
{
	ModelParam(long, nThresholdType);
	ModelParam(long, nThreshold);
	ModelParam(double, dblCornerCoefficient);
}
SRavidAlgorithmHarrisCornersDetectorParam;
#pragma pack(pop)
#pragma endregion

#pragma region Hough Transform Circle
enum ERavidParameterAlgorithmHoughTransformCircle
{
	ERavidParameterAlgorithmHoughTransformCircle_Threshold,
	ERavidParameterAlgorithmHoughTransformCircle_MaxCircleCount,
	ERavidParameterAlgorithmHoughTransformCircle_RegionalRange,
	ERavidParameterAlgorithmHoughTransformCircle_MinRadius,
	ERavidParameterAlgorithmHoughTransformCircle_MaxRadius,
	ERavidParameterAlgorithmHoughTransformCircle_Count,
};
static LPCTSTR GetRavidParameterAlgorithmHoughTransformCircleParamNameString(_In_ ERavidParameterAlgorithmHoughTransformCircle eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmHoughTransformCircle_Count] =
	{
		_T("Threshold"),
		_T("Max Circle Count"),
		_T("Regional Range"),
		_T("Min Radius"),
		_T("Max Radius"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmHoughTransformCircle_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmHoughTransformCircleParamDataTypeString(_In_ ERavidParameterAlgorithmHoughTransformCircle eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmHoughTransformCircle_Count] =
	{
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("DOUBLE"),
		_T("DOUBLE"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmHoughTransformCircle_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmHoughTransformCircleParamInputBoxTypeString(_In_ ERavidParameterAlgorithmHoughTransformCircle eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmHoughTransformCircle_Count] =
	{
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmHoughTransformCircle_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmHoughTransformCircleParamExtraDataString(_In_ ERavidParameterAlgorithmHoughTransformCircle eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmHoughTransformCircle_Count] =
	{
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmHoughTransformCircle_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

#pragma pack(push, 1)
typedef struct sRavidAlgorithmHoughTransformCircleParam
{
	ModelParam(long, nThreshold);
	ModelParam(long, nMaxCircleCount);
	ModelParam(long, nRegionalRange);
	ModelParam(double, dblMinRadius);
	ModelParam(double, dblMaxRadius);
}
SRavidAlgorithmHoughTransformCircleParam;
#pragma pack(pop)
#pragma endregion

#pragma region Hough Transform Line
enum ERavidParameterAlgorithmHoughTransformLine
{
	ERavidParameterAlgorithmHoughTransformLine_Threshold,
	ERavidParameterAlgorithmHoughTransformLine_MaxLineCount,
	ERavidParameterAlgorithmHoughTransformLine_RegionalRange,
	ERavidParameterAlgorithmHoughTransformLine_Count,
};
static LPCTSTR GetRavidParameterAlgorithmHoughTransformLineParamNameString(_In_ ERavidParameterAlgorithmHoughTransformLine eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmHoughTransformLine_Count] =
	{
		_T("Threshold"),
		_T("Max Line Count"),
		_T("Regional Range"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmHoughTransformLine_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmHoughTransformLineParamDataTypeString(_In_ ERavidParameterAlgorithmHoughTransformLine eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmHoughTransformLine_Count] =
	{
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmHoughTransformLine_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmHoughTransformLineParamInputBoxTypeString(_In_ ERavidParameterAlgorithmHoughTransformLine eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmHoughTransformLine_Count] =
	{
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmHoughTransformLine_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmHoughTransformLineParamExtraDataString(_In_ ERavidParameterAlgorithmHoughTransformLine eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmHoughTransformLine_Count] =
	{
		_T(""),
		_T(""),
		_T(""),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmHoughTransformLine_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

#pragma pack(push, 1)
typedef struct sRavidAlgorithmHoughTransformLineParam
{
	ModelParam(long, nThreshold);
	ModelParam(long, nMaxLineCount);
	ModelParam(long, nRegionalRange);
}
SRavidAlgorithmHoughTransformLineParam;
#pragma pack(pop)
#pragma endregion

#pragma region Move
enum ERavidParameterAlgorithmMove
{
	ERavidParameterAlgorithmMove_FillValue,
	ERavidParameterAlgorithmMove_InterpolationMethod,
	ERavidParameterAlgorithmMove_DeltaX,
	ERavidParameterAlgorithmMove_DeltaY,
	ERavidParameterAlgorithmMove_Count,
};
static LPCTSTR GetRavidParameterAlgorithmMoveParamNameString(_In_ ERavidParameterAlgorithmMove eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmMove_Count] =
	{
		_T("Fill Value"),
		_T("Interpolation Method"),
		_T("Delta X"),
		_T("Delta Y"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmMove_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmMoveParamDataTypeString(_In_ ERavidParameterAlgorithmMove eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmMove_Count] =
	{
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmMove_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmMoveParamInputBoxTypeString(_In_ ERavidParameterAlgorithmMove eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmMove_Count] =
	{
		_T("EDIT"),
		_T("COMBO"),
		_T("EDIT"),
		_T("EDIT"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmMove_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmMoveParamExtraDataString(_In_ ERavidParameterAlgorithmMove eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmMove_Count] =
	{
		_T(""),
		_T("NearestNeightbor;Bilinear;Bicubic"),
		_T(""),
		_T(""),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmMove_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

#pragma pack(push, 1)
typedef struct sRavidAlgorithmMoveParam
{
	ModelParam(long, nFillValue);
	ModelParam(long, nInterpolationMethod);
	ModelParam(long, nDeltaX);
	ModelParam(long, nDeltaY);
}
SRavidAlgorithmMoveParam;
#pragma pack(pop)
#pragma endregion

#pragma region Rotate
enum ERavidParameterAlgorithmRotate
{
	ERavidParameterAlgorithmRotate_Angle,
	ERavidParameterAlgorithmRotate_UseResize,
	ERavidParameterAlgorithmRotate_FillValue,
	ERavidParameterAlgorithmRotate_InterpolationMethod,
	ERavidParameterAlgorithmRotate_UsePivot,
	ERavidParameterAlgorithmRotate_PivotPointX,
	ERavidParameterAlgorithmRotate_PivotPointY,
	ERavidParameterAlgorithmRotate_Count,
};
static LPCTSTR GetRavidParameterAlgorithmRotateParamNameString(_In_ ERavidParameterAlgorithmRotate eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmRotate_Count] =
	{
		_T("Angle"),
		_T("Use Resize"),
		_T("Fill Value"),
		_T("Interpolation Method"),
		_T("Use Pivot"),
		_T("Pivot Point X"),
		_T("Pivot Point Y"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmRotate_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmRotateParamDataTypeString(_In_ ERavidParameterAlgorithmRotate eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmRotate_Count] =
	{
		_T("DOUBLE"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("DOUBLE"),
		_T("DOUBLE"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmRotate_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmRotateParamInputBoxTypeString(_In_ ERavidParameterAlgorithmRotate eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmRotate_Count] =
	{
		_T("EDIT"),
		_T("CHECK"),
		_T("EDIT"),
		_T("COMBO"),
		_T("CHECK"),
		_T("EDIT"),
		_T("EDIT"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmRotate_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmRotateParamExtraDataString(_In_ ERavidParameterAlgorithmRotate eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmRotate_Count] =
	{
		_T(""),
		_T(""),
		_T(""),
		_T("NearestNeightbor;Bilinear;Bicubic"),
		_T(""),
		_T(""),
		_T(""),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmRotate_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

#pragma pack(push, 1)
typedef struct sRavidAlgorithmRotateParam
{
	ModelParam(double, dblAngle);
	ModelParam(long, nUseResize);
	ModelParam(long, nFillValue);
	ModelParam(long, nInterpoloationMethod);
	ModelParam(long, nUsePivot);
	ModelParam(double, dblPivotPointX);
	ModelParam(double, dblPivotPointY);
}
SRavidAlgorithmRotateParam;
#pragma pack(pop)
#pragma endregion

#pragma region Scale
enum ERavidParameterAlgorithmScale
{
	ERavidParameterAlgorithmScale_RatioX,
	ERavidParameterAlgorithmScale_RatioY,
	ERavidParameterAlgorithmScale_UseResize,
	ERavidParameterAlgorithmScale_FillValue,
	ERavidParameterAlgorithmScale_InterpolationMethod,
	ERavidParameterAlgorithmScale_UsePivot,
	ERavidParameterAlgorithmScale_PivotPointX,
	ERavidParameterAlgorithmScale_PivotPointY,
	ERavidParameterAlgorithmScale_Count,
};
static LPCTSTR GetRavidParameterAlgorithmScaleParamNameString(_In_ ERavidParameterAlgorithmScale eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmScale_Count] =
	{
		_T("Ratio X"),
		_T("Ratio Y"),
		_T("Use Resize"),
		_T("Fill Value"),
		_T("Interpolation Method"),
		_T("Use Pivot"),
		_T("Pivot Point X"),
		_T("Pivot Point Y"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmScale_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmScaleParamDataTypeString(_In_ ERavidParameterAlgorithmScale eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmScale_Count] =
	{
		_T("DOUBLE"),
		_T("DOUBLE"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("DOUBLE"),
		_T("DOUBLE"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmScale_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmScaleParamInputBoxTypeString(_In_ ERavidParameterAlgorithmScale eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmScale_Count] =
	{
		_T("EDIT"),
		_T("EDIT"),
		_T("CHECK"),
		_T("EDIT"),
		_T("COMBO"),
		_T("CHECK"),
		_T("EDIT"),
		_T("EDIT"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmScale_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmScaleParamExtraDataString(_In_ ERavidParameterAlgorithmScale eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmScale_Count] =
	{
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T("NearestNeightbor;Bilinear;Bicubic"),
		_T(""),
		_T(""),
		_T(""),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmScale_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

#pragma pack(push, 1)
typedef struct sRavidAlgorithmScaleParam
{
	ModelParam(double, dblRatioX);
	ModelParam(double, dblRatioY);
	ModelParam(long, nUseResize);
	ModelParam(long, nFillValue);
	ModelParam(long, nInterpoloationMethod);
	ModelParam(long, nUsePivot);
	ModelParam(double, dblPivotPointX);
	ModelParam(double, dblPivotPointY);
}
SRavidAlgorithmScaleParam;
#pragma pack(pop)
#pragma endregion

#pragma region Operation
enum ERavidParameterAlgorithmOperation
{
	ERavidParameterAlgorithmOperation_DataType,
	ERavidParameterAlgorithmOperation_OperationType,
	ERavidParameterAlgorithmOperation_ConstValue,
	ERavidParameterAlgorithmOperation_Count,
};
static LPCTSTR GetRavidParameterAlgorithmOperationParamNameString(_In_ ERavidParameterAlgorithmOperation eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmOperation_Count] =
	{
		_T("Data Type"),
		_T("Operation Type"),
		_T("Const Value"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmOperation_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmOperationParamDataTypeString(_In_ ERavidParameterAlgorithmOperation eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmOperation_Count] =
	{
		_T("LONG"),
		_T("LONG"),
		_T("DOUBLE"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmOperation_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmOperationParamInputBoxTypeString(_In_ ERavidParameterAlgorithmOperation eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmOperation_Count] =
	{
		_T("COMBO"),
		_T("COMBO"),
		_T("EDIT"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmOperation_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmOperationParamExtraDataString(_In_ ERavidParameterAlgorithmOperation eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmOperation_Count] =
	{
		_T("Const;Image (Don't use the const value)"),
		_T("Add;Subtract;Multiply;Divide;Modulo;ShiftLeft;ShiftRight;BitwiseAND;BitwiseOR;BitwiswXOR;LogicalAND;LogicalOR;LogicalXOR;Minimum;Equal;NotEqual;GreaterOrEqual;LessOrEqual;Greater;Lesser"),
		_T(""),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmOperation_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

#pragma pack(push, 1)
typedef struct sRavidAlgorithmOperationParam
{
	ModelParam(long, nDataType);
	ModelParam(long, nOperationType);
	ModelParam(double, dblConstValue);
}
SRavidAlgorithmOperationParam;
#pragma pack(pop)
#pragma endregion

#pragma region Projection
enum ERavidParameterAlgorithmProjection
{
	ERavidParameterAlgorithmProjection_Direction,
	ERavidParameterAlgorithmProjection_Angle,
	ERavidParameterAlgorithmProjection_Count,
};
static LPCTSTR GetRavidParameterAlgorithmProjectionParamNameString(_In_ ERavidParameterAlgorithmProjection eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmProjection_Count] =
	{
		_T("Direction"),
		_T("Angle"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmProjection_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmProjectionParamDataTypeString(_In_ ERavidParameterAlgorithmProjection eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmProjection_Count] =
	{
		_T("LONG"),
		_T("DOUBLE"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmProjection_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmProjectionParamInputBoxTypeString(_In_ ERavidParameterAlgorithmProjection eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmProjection_Count] =
	{
		_T("COMBO"),
		_T("EDIT"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmProjection_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmProjectionParamExtraDataString(_In_ ERavidParameterAlgorithmProjection eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmProjection_Count] =
	{
		_T("Row (Angle : 0);Column (Angle : 0);Angle"),
		_T(""),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmProjection_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

#pragma pack(push, 1)
typedef struct sRavidAlgorithmProjectionParam
{
	ModelParam(long, nDirection);
	ModelParam(double, dblAngle);
}
SRavidAlgorithmProjectionParam;
#pragma pack(pop)
#pragma endregion

#pragma region Stretch
enum ERavidParameterAlgorithmStretch
{
	ERavidParameterAlgorithmStretch_StretchMode,
	ERavidParameterAlgorithmStretch_StretchMin,
	ERavidParameterAlgorithmStretch_StretchMax,
	ERavidParameterAlgorithmStretch_Count,
};
static LPCTSTR GetRavidParameterAlgorithmStretchParamNameString(_In_ ERavidParameterAlgorithmStretch eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmStretch_Count] =
	{
		_T("Stretch Mode"),
		_T("Stretch Min"),
		_T("Stretch Max"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmStretch_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmStretchParamDataTypeString(_In_ ERavidParameterAlgorithmStretch eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmStretch_Count] =
	{
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmStretch_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmStretchParamInputBoxTypeString(_In_ ERavidParameterAlgorithmStretch eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmStretch_Count] =
	{
		_T("COMBO"),
		_T("EDIT"),
		_T("EDIT"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmStretch_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmStretchParamExtraDataString(_In_ ERavidParameterAlgorithmStretch eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmStretch_Count] =
	{
		_T("Auto (Don't use the Min, Max value);Manual"),
		_T(""),
		_T(""),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmStretch_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

#pragma pack(push, 1)
typedef struct sRavidAlgorithmStretchParam
{
	ModelParam(long, nStretchMode);
	ModelParam(long, nStretchMin);
	ModelParam(long, nStretchMax);
}
SRavidAlgorithmStretchParam;
#pragma pack(pop)
#pragma endregion

#pragma region Gain
enum ERavidParameterAlgorithmGain
{
	ERavidParameterAlgorithmGain_Gain,
	ERavidParameterAlgorithmGain_Offset,
	ERavidParameterAlgorithmGain_Count,
};
static LPCTSTR GetRavidParameterAlgorithmGainParamNameString(_In_ ERavidParameterAlgorithmGain eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmGain_Count] =
	{
		_T("Gain Value"),
		_T("Offset Value"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmGain_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmGainParamDataTypeString(_In_ ERavidParameterAlgorithmGain eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmGain_Count] =
	{
		_T("DOUBLE"),
		_T("LONG"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmGain_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmGainParamInputBoxTypeString(_In_ ERavidParameterAlgorithmGain eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmGain_Count] =
	{
		_T("EDIT"),
		_T("EDIT"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmGain_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmGainParamExtraDataString(_In_ ERavidParameterAlgorithmGain eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmGain_Count] =
	{
		_T(""),
		_T(""),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmGain_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

#pragma pack(push, 1)
typedef struct sRavidAlgorithmGainParam
{
	ModelParam(double, dblGain);
	ModelParam(long, nOffset);
}
SRavidAlgorithmGainParam;
#pragma pack(pop)
#pragma endregion

//#pragma region SingleThreshold_RGB
//enum ERavidParameterAlgorithmSingleThresholdRGB
//{
//	ERavidParameterAlgorithmSingleThresholdRGB_Threshold1,
//	ERavidParameterAlgorithmSingleThresholdRGB_Threshold2,
//	ERavidParameterAlgorithmSingleThresholdRGB_Threshold3,
//	ERavidParameterAlgorithmSingleThresholdRGB_Threshold4,
//	ERavidParameterAlgorithmSingleThresholdRGB_Condition1,
//	ERavidParameterAlgorithmSingleThresholdRGB_Condition2,
//	ERavidParameterAlgorithmSingleThresholdRGB_Condition3,
//	ERavidParameterAlgorithmSingleThresholdRGB_Condition4,
//	ERavidParameterAlgorithmSingleThresholdRGB_RangeFillValue1,
//	ERavidParameterAlgorithmSingleThresholdRGB_RangeFillValue2,
//	ERavidParameterAlgorithmSingleThresholdRGB_RangeFillValue3,
//	ERavidParameterAlgorithmSingleThresholdRGB_RangeFillValue4,
//	ERavidParameterAlgorithmSingleThresholdRGB_OutOfRangeFillValue1,
//	ERavidParameterAlgorithmSingleThresholdRGB_OutOfRangeFillValue2,
//	ERavidParameterAlgorithmSingleThresholdRGB_OutOfRangeFillValue3,
//	ERavidParameterAlgorithmSingleThresholdRGB_OutOfRangeFillValue4,
//	ERavidParameterAlgorithmSingleThresholdRGB_LogicalOperation,
//	ERavidParameterAlgorithmSingleThresholdRGB_Count,
//};
//static LPCTSTR GetRavidParameterAlgorithmSingleThresholdRGBParamNameString(_In_ ERavidParameterAlgorithmSingleThresholdRGB eSrc)
//{
//	static LPCTSTR lpsz[ERavidParameterAlgorithmSingleThresholdRGB_Count] =
//	{
//	_T("Threshold1"),
//	_T("Threshold2"),
//	_T("Threshold3"),
//	_T("Threshold4"),
//	_T("Condition1"),
//	_T("Condition2"),
//	_T("Condition3"),
//	_T("Condition4"),
//	_T("Range Fill Value1"),
//	_T("Range Fill Value2"),
//	_T("Range Fill Value3"),
//	_T("Range Fill Value4"),
//	_T("Out of Range Fill Value1"),
//	_T("Out of Range Fill Value2"),
//	_T("Out of Range Fill Value3"),
//	_T("Out of Range Fill Value4"),
//	_T("Logical Operation"),
//	};
//
//	LPCTSTR lpszReturn = nullptr;
//
//	do
//	{
//		int nIdx = eSrc;
//
//		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmSingleThresholdRGB_Count)
//			break;
//
//		lpszReturn = lpsz[nIdx];
//	}
//	while(false);
//
//	return lpszReturn;
//}
//
//static LPCTSTR GetRavidParameterAlgorithmSingleThresholdRGBParamDataTypeString(_In_ ERavidParameterAlgorithmSingleThresholdRGB eSrc)
//{
//	static LPCTSTR lpsz[ERavidParameterAlgorithmSingleThresholdRGB_Count] =
//	{
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//	};
//
//	LPCTSTR lpszReturn = nullptr;
//
//	do
//	{
//		int nIdx = eSrc;
//
//		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmSingleThresholdRGB_Count)
//			break;
//
//		lpszReturn = lpsz[nIdx];
//	}
//	while(false);
//
//	return lpszReturn;
//}
//
//static LPCTSTR GetRavidParameterAlgorithmSingleThresholdRGBParamInputBoxTypeString(_In_ ERavidParameterAlgorithmSingleThresholdRGB eSrc)
//{
//	static LPCTSTR lpsz[ERavidParameterAlgorithmSingleThresholdRGB_Count] =
//	{
//		_T("EDIT"),
//		_T("EDIT"),
//		_T("EDIT"),
//		_T("EDIT"),
//		_T("COMBO"),
//		_T("COMBO"),
//		_T("COMBO"),
//		_T("COMBO"),
//		_T("EDIT"),
//		_T("EDIT"),
//		_T("EDIT"),
//		_T("EDIT"),
//		_T("EDIT"),
//		_T("EDIT"),
//		_T("EDIT"),
//		_T("EDIT"),
//		_T("COMBO"),
//	};
//
//	LPCTSTR lpszReturn = nullptr;
//
//	do
//	{
//		int nIdx = eSrc;
//
//		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmSingleThresholdRGB_Count)
//			break;
//
//		lpszReturn = lpsz[nIdx];
//	}
//	while(false);
//
//	return lpszReturn;
//}
//
//static LPCTSTR GetRavidParameterAlgorithmSingleThresholdRGBParamExtraDataString(_In_ ERavidParameterAlgorithmSingleThresholdRGB eSrc)
//{
//	static LPCTSTR lpsz[ERavidParameterAlgorithmSingleThresholdRGB_Count] =
//	{
//		_T(""),
//		_T(""),
//		_T(""),
//		_T(""),
//		_T("Greater;GreaterEqual;Equal;NotEqual;LessEqual;Less"),
//		_T("Greater;GreaterEqual;Equal;NotEqual;LessEqual;Less"),
//		_T("Greater;GreaterEqual;Equal;NotEqual;LessEqual;Less"),
//		_T("Greater;GreaterEqual;Equal;NotEqual;LessEqual;Less"),
//		_T(""),
//		_T(""),
//		_T(""),
//		_T(""),
//		_T(""),
//		_T(""),
//		_T(""),
//		_T(""),
//		_T("None;AND;OR"),
//	};
//
//	LPCTSTR lpszReturn = nullptr;
//
//	do
//	{
//		int nIdx = eSrc;
//
//		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmSingleThresholdRGB_Count)
//			break;
//
//		lpszReturn = lpsz[nIdx];
//	}
//	while(false);
//
//	return lpszReturn;
//}
//
//#pragma pack(push, 1)
//typedef struct sRavidAlgorithmSingleThresholdRGBParam
//{
//	ModelParam(long, nThreshold1);
//	ModelParam(long, nThreshold2);
//	ModelParam(long, nThreshold3);
//	ModelParam(long, nThreshold4);
//	ModelParam(long, nCondition1);
//	ModelParam(long, nCondition2);
//	ModelParam(long, nCondition3);
//	ModelParam(long, nCondition4);
//	ModelParam(long, nRangeFillValue1);
//	ModelParam(long, nRangeFillValue2);
//	ModelParam(long, nRangeFillValue3);
//	ModelParam(long, nRangeFillValue4);
//	ModelParam(long, nOutOfRangeFillValue1);
//	ModelParam(long, nOutOfRangeFillValue2);
//	ModelParam(long, nOutOfRangeFillValue3);
//	ModelParam(long, nOutOfRangeFillValue4);
//	ModelParam(long, nLogicalOpertion);
//}
//SRavidAlgorithmSingleThresholdRGBParam;
//#pragma pack(pop)
//#pragma endregion
//
//#pragma region DoubleThreshold_RGB
//enum ERavidParameterAlgorithmDoubleThresholdRGB
//{
//	ERavidParameterAlgorithmDoubleThresholdRGB_LowThreshold1,
//	ERavidParameterAlgorithmDoubleThresholdRGB_LowThreshold2,
//	ERavidParameterAlgorithmDoubleThresholdRGB_LowThreshold3,
//	ERavidParameterAlgorithmDoubleThresholdRGB_LowThreshold4,
//	ERavidParameterAlgorithmDoubleThresholdRGB_HighThreshold1,
//	ERavidParameterAlgorithmDoubleThresholdRGB_HighThreshold2,
//	ERavidParameterAlgorithmDoubleThresholdRGB_HighThreshold3,
//	ERavidParameterAlgorithmDoubleThresholdRGB_HighThreshold4,
//	ERavidParameterAlgorithmDoubleThresholdRGB_RangeFillValue1,
//	ERavidParameterAlgorithmDoubleThresholdRGB_RangeFillValue2,
//	ERavidParameterAlgorithmDoubleThresholdRGB_RangeFillValue3,
//	ERavidParameterAlgorithmDoubleThresholdRGB_RangeFillValue4,
//	ERavidParameterAlgorithmDoubleThresholdRGB_LessThanRangeFillValue1,
//	ERavidParameterAlgorithmDoubleThresholdRGB_LessThanRangeFillValue2,
//	ERavidParameterAlgorithmDoubleThresholdRGB_LessThanRangeFillValue3,
//	ERavidParameterAlgorithmDoubleThresholdRGB_LessThanRangeFillValue4,
//	ERavidParameterAlgorithmDoubleThresholdRGB_GreaterThanRangeFillValue1,
//	ERavidParameterAlgorithmDoubleThresholdRGB_GreaterThanRangeFillValue2,
//	ERavidParameterAlgorithmDoubleThresholdRGB_GreaterThanRangeFillValue3,
//	ERavidParameterAlgorithmDoubleThresholdRGB_GreaterThanRangeFillValue4,
//	ERavidParameterAlgorithmDoubleThresholdRGB_Count,
//};
//static LPCTSTR GetRavidParameterAlgorithmDoubleThresholdRGBParamNameString(_In_ ERavidParameterAlgorithmDoubleThresholdRGB eSrc)
//{
//	static LPCTSTR lpsz[ERavidParameterAlgorithmDoubleThresholdRGB_Count] =
//	{
//	_T("Low Threshold1"),
//	_T("Low Threshold2"),
//	_T("Low Threshold3"),
//	_T("Low Threshold4"),
//	_T("High Threshold1"),
//	_T("High Threshold2"),
//	_T("High Threshold3"),
//	_T("High Threshold4"),
//	_T("Range Fill Value1"),
//	_T("Range Fill Value2"),
//	_T("Range Fill Value3"),
//	_T("Range Fill Value4"),
//	_T("Less than Range Fill Value1"),
//	_T("Less than Range Fill Value2"),
//	_T("Less than Range Fill Value3"),
//	_T("Less than Range Fill Value4"),
//	_T("Greater than Range Fill Value1"),
//	_T("Greater than Range Fill Value2"),
//	_T("Greater than Range Fill Value3"),
//	_T("Greater than Range Fill Value4"),
//	};
//
//	LPCTSTR lpszReturn = nullptr;
//
//	do
//	{
//		int nIdx = eSrc;
//
//		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmDoubleThresholdRGB_Count)
//			break;
//
//		lpszReturn = lpsz[nIdx];
//	}
//	while(false);
//
//	return lpszReturn;
//}
//
//static LPCTSTR GetRavidParameterAlgorithmDoubleThresholdRGBParamDataTypeString(_In_ ERavidParameterAlgorithmDoubleThresholdRGB eSrc)
//{
//	static LPCTSTR lpsz[ERavidParameterAlgorithmDoubleThresholdRGB_Count] =
//	{
//		_T("LONG"),_T("LONG"),_T("LONG"),_T("LONG"),
//		_T("LONG"),_T("LONG"),_T("LONG"),_T("LONG"),
//		_T("LONG"),_T("LONG"),_T("LONG"),_T("LONG"),
//		_T("LONG"),_T("LONG"),_T("LONG"),_T("LONG"),
//		_T("LONG"),_T("LONG"),_T("LONG"),_T("LONG"),
//	};
//
//	LPCTSTR lpszReturn = nullptr;
//
//	do
//	{
//		int nIdx = eSrc;
//
//		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmDoubleThresholdRGB_Count)
//			break;
//
//		lpszReturn = lpsz[nIdx];
//	}
//	while(false);
//
//	return lpszReturn;
//}
//
//static LPCTSTR GetRavidParameterAlgorithmDoubleThresholdRGBParamInputBoxTypeString(_In_ ERavidParameterAlgorithmDoubleThresholdRGB eSrc)
//{
//	static LPCTSTR lpsz[ERavidParameterAlgorithmDoubleThresholdRGB_Count] =
//	{
//		_T("EDIT"),_T("EDIT"), _T("EDIT"),_T("EDIT"),
//		_T("EDIT"),_T("EDIT"), _T("EDIT"),_T("EDIT"),
//		_T("EDIT"),_T("EDIT"), _T("EDIT"),_T("EDIT"),
//		_T("EDIT"),_T("EDIT"), _T("EDIT"),_T("EDIT"),
//		_T("EDIT"),_T("EDIT"), _T("EDIT"),_T("EDIT"),
//	};
//
//	LPCTSTR lpszReturn = nullptr;
//
//	do
//	{
//		int nIdx = eSrc;
//
//		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmDoubleThresholdRGB_Count)
//			break;
//
//		lpszReturn = lpsz[nIdx];
//	}
//	while(false);
//
//	return lpszReturn;
//}
//
//static LPCTSTR GetRavidParameterAlgorithmDoubleThresholdRGBParamExtraDataString(_In_ ERavidParameterAlgorithmDoubleThresholdRGB eSrc)
//{
//	static LPCTSTR lpsz[ERavidParameterAlgorithmDoubleThresholdRGB_Count] =
//	{
//		_T(""),_T(""),_T(""),_T(""),
//		_T(""),_T(""),_T(""),_T(""),
//		_T(""),_T(""),_T(""),_T(""),
//		_T(""),_T(""),_T(""),_T(""),
//		_T(""),_T(""),_T(""),_T(""),
//	};
//
//	LPCTSTR lpszReturn = nullptr;
//
//	do
//	{
//		int nIdx = eSrc;
//
//		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmDoubleThresholdRGB_Count)
//			break;
//
//		lpszReturn = lpsz[nIdx];
//	}
//	while(false);
//
//	return lpszReturn;
//}
//
//#pragma pack(push, 1)
//typedef struct sRavidAlgorithmDoubleThresholdRGBParam
//{
//	ModelParam(long, nLowThreshold1);
//	ModelParam(long, nLowThreshold2);
//	ModelParam(long, nLowThreshold3);
//	ModelParam(long, nLowThreshold4);
//	ModelParam(long, nHighThreshold1);
//	ModelParam(long, nHighThreshold2);
//	ModelParam(long, nHighThreshold3);
//	ModelParam(long, nHighThreshold4);
//	ModelParam(long, nRangeFillValue1);
//	ModelParam(long, nRangeFillValue2);
//	ModelParam(long, nRangeFillValue3);
//	ModelParam(long, nRangeFillValue4);
//	ModelParam(long, nLessThanRangeFillValue1);
//	ModelParam(long, nLessThanRangeFillValue2);
//	ModelParam(long, nLessThanRangeFillValue3);
//	ModelParam(long, nLessThanRangeFillValue4);
//	ModelParam(long, nGreaterThanRangeFillValue1);
//	ModelParam(long, nGreaterThanRangeFillValue2);
//	ModelParam(long, nGreaterThanRangeFillValue3);
//	ModelParam(long, nGreaterThanRangeFillValue4);
//}
//SRavidAlgorithmDoubleThresholdRGBParam;
//#pragma pack(pop)
//#pragma endregion
//
//#pragma region OtsuThreshold_RGB
//enum ERavidParameterAlgorithmOtsuThresholdRGB
//{
//	ERavidParameterAlgorithmOtsuThresholdRGB_Condition1,
//	ERavidParameterAlgorithmOtsuThresholdRGB_Condition2,
//	ERavidParameterAlgorithmOtsuThresholdRGB_Condition3,
//	ERavidParameterAlgorithmOtsuThresholdRGB_Condition4,
//	ERavidParameterAlgorithmOtsuThresholdRGB_RangeFillValue1,
//	ERavidParameterAlgorithmOtsuThresholdRGB_RangeFillValue2,
//	ERavidParameterAlgorithmOtsuThresholdRGB_RangeFillValue3,
//	ERavidParameterAlgorithmOtsuThresholdRGB_RangeFillValue4,
//	ERavidParameterAlgorithmOtsuThresholdRGB_OutOfRangeFillValue1,
//	ERavidParameterAlgorithmOtsuThresholdRGB_OutOfRangeFillValue2,
//	ERavidParameterAlgorithmOtsuThresholdRGB_OutOfRangeFillValue3,
//	ERavidParameterAlgorithmOtsuThresholdRGB_OutOfRangeFillValue4,
//	ERavidParameterAlgorithmOtsuThresholdRGB_Count,
//};
//static LPCTSTR GetRavidParameterAlgorithmOtsuThresholdRGBParamNameString(_In_ ERavidParameterAlgorithmOtsuThresholdRGB eSrc)
//{
//	static LPCTSTR lpsz[ERavidParameterAlgorithmOtsuThresholdRGB_Count] =
//	{
//	_T("Condition1"),
//	_T("Condition2"),
//	_T("Condition3"),
//	_T("Condition4"),
//	_T("Range Fill Value1"),
//	_T("Range Fill Value2"),
//	_T("Range Fill Value3"),
//	_T("Range Fill Value4"),
//	_T("Out of Range Fill Value1"),
//	_T("Out of Range Fill Value2"),
//	_T("Out of Range Fill Value3"),
//	_T("Out of Range Fill Value4"),
//	};
//
//	LPCTSTR lpszReturn = nullptr;
//
//	do
//	{
//		int nIdx = eSrc;
//
//		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmOtsuThresholdRGB_Count)
//			break;
//
//		lpszReturn = lpsz[nIdx];
//	}
//	while(false);
//
//	return lpszReturn;
//}
//
//static LPCTSTR GetRavidParameterAlgorithmOtsuThresholdRGBParamDataTypeString(_In_ ERavidParameterAlgorithmOtsuThresholdRGB eSrc)
//{
//	static LPCTSTR lpsz[ERavidParameterAlgorithmOtsuThresholdRGB_Count] =
//	{
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//	};
//
//	LPCTSTR lpszReturn = nullptr;
//
//	do
//	{
//		int nIdx = eSrc;
//
//		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmOtsuThresholdRGB_Count)
//			break;
//
//		lpszReturn = lpsz[nIdx];
//	}
//	while(false);
//
//	return lpszReturn;
//}
//
//static LPCTSTR GetRavidParameterAlgorithmOtsuThresholdRGBParamInputBoxTypeString(_In_ ERavidParameterAlgorithmOtsuThresholdRGB eSrc)
//{
//	static LPCTSTR lpsz[ERavidParameterAlgorithmOtsuThresholdRGB_Count] =
//	{
//		_T("COMBO"),
//		_T("COMBO"),
//		_T("COMBO"),
//		_T("COMBO"),
//		_T("EDIT"),
//		_T("EDIT"),
//		_T("EDIT"),
//		_T("EDIT"),
//		_T("EDIT"),
//		_T("EDIT"),
//		_T("EDIT"),
//		_T("EDIT"),
//	};
//
//	LPCTSTR lpszReturn = nullptr;
//
//	do
//	{
//		int nIdx = eSrc;
//
//		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmOtsuThresholdRGB_Count)
//			break;
//
//		lpszReturn = lpsz[nIdx];
//	}
//	while(false);
//
//	return lpszReturn;
//}
//
//static LPCTSTR GetRavidParameterAlgorithmOtsuThresholdRGBParamExtraDataString(_In_ ERavidParameterAlgorithmOtsuThresholdRGB eSrc)
//{
//	static LPCTSTR lpsz[ERavidParameterAlgorithmOtsuThresholdRGB_Count] =
//	{
//		_T("Greater;GreaterEqual;Equal;NotEqual;LessEqual;Less"),
//		_T("Greater;GreaterEqual;Equal;NotEqual;LessEqual;Less"),
//		_T("Greater;GreaterEqual;Equal;NotEqual;LessEqual;Less"),
//		_T("Greater;GreaterEqual;Equal;NotEqual;LessEqual;Less"),
//		_T(""),
//		_T(""),
//		_T(""),
//		_T(""),
//		_T(""),
//		_T(""),
//		_T(""),
//		_T(""),
//	};
//
//	LPCTSTR lpszReturn = nullptr;
//
//	do
//	{
//		int nIdx = eSrc;
//
//		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmOtsuThresholdRGB_Count)
//			break;
//
//		lpszReturn = lpsz[nIdx];
//	}
//	while(false);
//
//	return lpszReturn;
//}
//
//#pragma pack(push, 1)
//typedef struct sRavidAlgorithmOtsuThresholdRGBParam
//{
//	ModelParam(long, nCondition1);
//	ModelParam(long, nCondition2);
//	ModelParam(long, nCondition3);
//	ModelParam(long, nCondition4);
//	ModelParam(long, nRangeFillValue1);
//	ModelParam(long, nRangeFillValue2);
//	ModelParam(long, nRangeFillValue3);
//	ModelParam(long, nRangeFillValue4);
//	ModelParam(long, nOutOfRangeFillValue1);
//	ModelParam(long, nOutOfRangeFillValue2);
//	ModelParam(long, nOutOfRangeFillValue3);
//	ModelParam(long, nOutOfRangeFillValue4);
//}
//SRavidAlgorithmOtsuThresholdRGBParam;
//#pragma pack(pop)
//#pragma endregion
//
//#pragma region AdaptiveThreshold_RGB
//enum ERavidParameterAlgorithmAdaptiveThresholdRGB
//{
//	ERavidParameterAlgorithmAdaptiveThresholdRGB_ThresholdType,
//	ERavidParameterAlgorithmAdaptiveThresholdRGB_ThresholdMethod,
//	ERavidParameterAlgorithmAdaptiveThresholdRGB_MaxValue1,
//	ERavidParameterAlgorithmAdaptiveThresholdRGB_MaxValue2,
//	ERavidParameterAlgorithmAdaptiveThresholdRGB_MaxValue3,
//	ERavidParameterAlgorithmAdaptiveThresholdRGB_MaxValue4,
//	ERavidParameterAlgorithmAdaptiveThresholdRGB_BlockSize,
//	ERavidParameterAlgorithmAdaptiveThresholdRGB_Constant1,
//	ERavidParameterAlgorithmAdaptiveThresholdRGB_Constant2,
//	ERavidParameterAlgorithmAdaptiveThresholdRGB_Constant3,
//	ERavidParameterAlgorithmAdaptiveThresholdRGB_Constant4,
//	ERavidParameterAlgorithmAdaptiveThresholdRGB_Count,
//};
//static LPCTSTR GetRavidParameterAlgorithmAdaptiveThresholdRGBParamNameString(_In_ ERavidParameterAlgorithmAdaptiveThresholdRGB eSrc)
//{
//	static LPCTSTR lpsz[ERavidParameterAlgorithmAdaptiveThresholdRGB_Count] =
//	{
//	_T("Threshold Type"),
//	_T("Threshold Method"),
//	_T("Max Value1"),
//	_T("Max Value2"),
//	_T("Max Value3"),
//	_T("Max Value4"),
//	_T("Block Size"),
//	_T("Constant1"),
//	_T("Constant2"),
//	_T("Constant3"),
//	_T("Constant4"),
//	};
//
//	LPCTSTR lpszReturn = nullptr;
//
//	do
//	{
//		int nIdx = eSrc;
//
//		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmAdaptiveThresholdRGB_Count)
//			break;
//
//		lpszReturn = lpsz[nIdx];
//	}
//	while(false);
//
//	return lpszReturn;
//}
//
//static LPCTSTR GetRavidParameterAlgorithmAdaptiveThresholdRGBParamDataTypeString(_In_ ERavidParameterAlgorithmAdaptiveThresholdRGB eSrc)
//{
//	static LPCTSTR lpsz[ERavidParameterAlgorithmAdaptiveThresholdRGB_Count] =
//	{
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//	};
//
//	LPCTSTR lpszReturn = nullptr;
//
//	do
//	{
//		int nIdx = eSrc;
//
//		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmAdaptiveThresholdRGB_Count)
//			break;
//
//		lpszReturn = lpsz[nIdx];
//	}
//	while(false);
//
//	return lpszReturn;
//}
//
//static LPCTSTR GetRavidParameterAlgorithmAdaptiveThresholdRGBParamInputBoxTypeString(_In_ ERavidParameterAlgorithmAdaptiveThresholdRGB eSrc)
//{
//	static LPCTSTR lpsz[ERavidParameterAlgorithmAdaptiveThresholdRGB_Count] =
//	{
//		_T("COMBO"),
//		_T("COMBO"),
//		_T("EDIT"),
//		_T("EDIT"),
//		_T("EDIT"),
//		_T("EDIT"),
//		_T("EDIT"),
//		_T("EDIT"),
//		_T("EDIT"),
//		_T("EDIT"),
//		_T("EDIT"),
//	};
//
//	LPCTSTR lpszReturn = nullptr;
//
//	do
//	{
//		int nIdx = eSrc;
//
//		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmAdaptiveThresholdRGB_Count)
//			break;
//
//		lpszReturn = lpsz[nIdx];
//	}
//	while(false);
//
//	return lpszReturn;
//}
//
//static LPCTSTR GetRavidParameterAlgorithmAdaptiveThresholdRGBParamExtraDataString(_In_ ERavidParameterAlgorithmAdaptiveThresholdRGB eSrc)
//{
//	static LPCTSTR lpsz[ERavidParameterAlgorithmAdaptiveThresholdRGB_Count] =
//	{
//		_T("Binary;Binary_Inverse"),
//		_T("Mean;Gaussian"),
//		_T(""),_T(""),_T(""),_T(""),
//		_T(""),
//		_T(""),_T(""),_T(""),_T(""),
//	};
//
//	LPCTSTR lpszReturn = nullptr;
//
//	do
//	{
//		int nIdx = eSrc;
//
//		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmAdaptiveThresholdRGB_Count)
//			break;
//
//		lpszReturn = lpsz[nIdx];
//	}
//	while(false);
//
//	return lpszReturn;
//}
//
//#pragma pack(push, 1)
//typedef struct sRavidAlgorithmAdaptiveThresholdRGBParam
//{
//	ModelParam(long, nThresholdType);
//	ModelParam(long, nThresholdMethod);
//	ModelParam(long, nMaxValue1);
//	ModelParam(long, nMaxValue2);
//	ModelParam(long, nMaxValue3);
//	ModelParam(long, nMaxValue4);
//	ModelParam(long, nBlockSize);
//	ModelParam(long, nConstant1);
//	ModelParam(long, nConstant2);
//	ModelParam(long, nConstant3);
//	ModelParam(long, nConstant4);
//}
//SRavidAlgorithmAdaptiveThresholdRGBParam;
//#pragma pack(pop)
//#pragma endregion
//
//#pragma region ISODATAThreshold_RGB
//enum ERavidParameterAlgorithmISODATAThresholdRGB
//{
//	ERavidParameterAlgorithmISODATAThresholdRGB_Condition1,
//	ERavidParameterAlgorithmISODATAThresholdRGB_Condition2,
//	ERavidParameterAlgorithmISODATAThresholdRGB_Condition3,
//	ERavidParameterAlgorithmISODATAThresholdRGB_Condition4,
//	ERavidParameterAlgorithmISODATAThresholdRGB_RangeFillValue1,
//	ERavidParameterAlgorithmISODATAThresholdRGB_RangeFillValue2,
//	ERavidParameterAlgorithmISODATAThresholdRGB_RangeFillValue3,
//	ERavidParameterAlgorithmISODATAThresholdRGB_RangeFillValue4,
//	ERavidParameterAlgorithmISODATAThresholdRGB_OutOfRangeFillValue1,
//	ERavidParameterAlgorithmISODATAThresholdRGB_OutOfRangeFillValue2,
//	ERavidParameterAlgorithmISODATAThresholdRGB_OutOfRangeFillValue3,
//	ERavidParameterAlgorithmISODATAThresholdRGB_OutOfRangeFillValue4,
//	ERavidParameterAlgorithmISODATAThresholdRGB_Count,
//};
//static LPCTSTR GetRavidParameterAlgorithmISODATAThresholdRGBParamNameString(_In_ ERavidParameterAlgorithmISODATAThresholdRGB eSrc)
//{
//	static LPCTSTR lpsz[ERavidParameterAlgorithmISODATAThresholdRGB_Count] =
//	{
//	_T("Condition1"),
//	_T("Condition2"),
//	_T("Condition3"),
//	_T("Condition4"),
//	_T("Range Fill Value1"),
//	_T("Range Fill Value2"),
//	_T("Range Fill Value3"),
//	_T("Range Fill Value4"),
//	_T("Out of Range Fill Value1"),
//	_T("Out of Range Fill Value2"),
//	_T("Out of Range Fill Value3"),
//	_T("Out of Range Fill Value4"),
//	};
//
//	LPCTSTR lpszReturn = nullptr;
//
//	do
//	{
//		int nIdx = eSrc;
//
//		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmISODATAThresholdRGB_Count)
//			break;
//
//		lpszReturn = lpsz[nIdx];
//	}
//	while(false);
//
//	return lpszReturn;
//}
//
//static LPCTSTR GetRavidParameterAlgorithmISODATAThresholdRGBParamDataTypeString(_In_ ERavidParameterAlgorithmISODATAThresholdRGB eSrc)
//{
//	static LPCTSTR lpsz[ERavidParameterAlgorithmISODATAThresholdRGB_Count] =
//	{
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//	};
//
//	LPCTSTR lpszReturn = nullptr;
//
//	do
//	{
//		int nIdx = eSrc;
//
//		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmISODATAThresholdRGB_Count)
//			break;
//
//		lpszReturn = lpsz[nIdx];
//	}
//	while(false);
//
//	return lpszReturn;
//}
//
//static LPCTSTR GetRavidParameterAlgorithmISODATAThresholdRGBParamInputBoxTypeString(_In_ ERavidParameterAlgorithmISODATAThresholdRGB eSrc)
//{
//	static LPCTSTR lpsz[ERavidParameterAlgorithmISODATAThresholdRGB_Count] =
//	{
//		_T("COMBO"),
//		_T("COMBO"),
//		_T("COMBO"),
//		_T("COMBO"),
//		_T("EDIT"),
//		_T("EDIT"),
//		_T("EDIT"),
//		_T("EDIT"),
//		_T("EDIT"),
//		_T("EDIT"),
//		_T("EDIT"),
//		_T("EDIT"),
//	};
//
//	LPCTSTR lpszReturn = nullptr;
//
//	do
//	{
//		int nIdx = eSrc;
//
//		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmISODATAThresholdRGB_Count)
//			break;
//
//		lpszReturn = lpsz[nIdx];
//	}
//	while(false);
//
//	return lpszReturn;
//}
//
//static LPCTSTR GetRavidParameterAlgorithmISODATAThresholdRGBParamExtraDataString(_In_ ERavidParameterAlgorithmISODATAThresholdRGB eSrc)
//{
//	static LPCTSTR lpsz[ERavidParameterAlgorithmISODATAThresholdRGB_Count] =
//	{
//		_T("Greater;GreaterEqual;Equal;NotEqual;LessEqual;Less"),
//		_T("Greater;GreaterEqual;Equal;NotEqual;LessEqual;Less"),
//		_T("Greater;GreaterEqual;Equal;NotEqual;LessEqual;Less"),
//		_T("Greater;GreaterEqual;Equal;NotEqual;LessEqual;Less"),
//		_T(""),
//		_T(""),
//		_T(""),
//		_T(""),
//		_T(""),
//		_T(""),
//		_T(""),
//		_T(""),
//	};
//
//	LPCTSTR lpszReturn = nullptr;
//
//	do
//	{
//		int nIdx = eSrc;
//
//		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmISODATAThresholdRGB_Count)
//			break;
//
//		lpszReturn = lpsz[nIdx];
//	}
//	while(false);
//
//	return lpszReturn;
//}
//
//#pragma pack(push, 1)
//typedef struct sRavidAlgorithmISODATAThresholdRGBParam
//{
//	ModelParam(long, nCondition1);
//	ModelParam(long, nCondition2);
//	ModelParam(long, nCondition3);
//	ModelParam(long, nCondition4);
//	ModelParam(long, nRangeFillValue1);
//	ModelParam(long, nRangeFillValue2);
//	ModelParam(long, nRangeFillValue3);
//	ModelParam(long, nRangeFillValue4);
//	ModelParam(long, nOutOfRangeFillValue1);
//	ModelParam(long, nOutOfRangeFillValue2);
//	ModelParam(long, nOutOfRangeFillValue3);
//	ModelParam(long, nOutOfRangeFillValue4);
//}
//SRavidAlgorithmISODATAThresholdRGBParam;
//#pragma pack(pop)
//#pragma endregion
//
//#pragma region MaximumEntropyThreshold_RGB
//enum ERavidParameterAlgorithmMaximumEntropyThresholdRGB
//{
//	ERavidParameterAlgorithmMaximumEntropyThresholdRGB_Condition1,
//	ERavidParameterAlgorithmMaximumEntropyThresholdRGB_Condition2,
//	ERavidParameterAlgorithmMaximumEntropyThresholdRGB_Condition3,
//	ERavidParameterAlgorithmMaximumEntropyThresholdRGB_Condition4,
//	ERavidParameterAlgorithmMaximumEntropyThresholdRGB_RangeFillValue1,
//	ERavidParameterAlgorithmMaximumEntropyThresholdRGB_RangeFillValue2,
//	ERavidParameterAlgorithmMaximumEntropyThresholdRGB_RangeFillValue3,
//	ERavidParameterAlgorithmMaximumEntropyThresholdRGB_RangeFillValue4,
//	ERavidParameterAlgorithmMaximumEntropyThresholdRGB_OutOfRangeFillValue1,
//	ERavidParameterAlgorithmMaximumEntropyThresholdRGB_OutOfRangeFillValue2,
//	ERavidParameterAlgorithmMaximumEntropyThresholdRGB_OutOfRangeFillValue3,
//	ERavidParameterAlgorithmMaximumEntropyThresholdRGB_OutOfRangeFillValue4,
//	ERavidParameterAlgorithmMaximumEntropyThresholdRGB_Count,
//};
//static LPCTSTR GetRavidParameterAlgorithmMaximumEntropyThresholdRGBParamNameString(_In_ ERavidParameterAlgorithmMaximumEntropyThresholdRGB eSrc)
//{
//	static LPCTSTR lpsz[ERavidParameterAlgorithmMaximumEntropyThresholdRGB_Count] =
//	{
//	_T("Condition1"),
//	_T("Condition2"),
//	_T("Condition3"),
//	_T("Condition4"),
//	_T("Range Fill Value1"),
//	_T("Range Fill Value2"),
//	_T("Range Fill Value3"),
//	_T("Range Fill Value4"),
//	_T("Out of Range Fill Value1"),
//	_T("Out of Range Fill Value2"),
//	_T("Out of Range Fill Value3"),
//	_T("Out of Range Fill Value4"),
//	};
//
//	LPCTSTR lpszReturn = nullptr;
//
//	do
//	{
//		int nIdx = eSrc;
//
//		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmMaximumEntropyThresholdRGB_Count)
//			break;
//
//		lpszReturn = lpsz[nIdx];
//	}
//	while(false);
//
//	return lpszReturn;
//}
//
//static LPCTSTR GetRavidParameterAlgorithmMaximumEntropyThresholdRGBParamDataTypeString(_In_ ERavidParameterAlgorithmMaximumEntropyThresholdRGB eSrc)
//{
//	static LPCTSTR lpsz[ERavidParameterAlgorithmMaximumEntropyThresholdRGB_Count] =
//	{
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//		_T("LONG"),
//	};
//
//	LPCTSTR lpszReturn = nullptr;
//
//	do
//	{
//		int nIdx = eSrc;
//
//		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmMaximumEntropyThresholdRGB_Count)
//			break;
//
//		lpszReturn = lpsz[nIdx];
//	}
//	while(false);
//
//	return lpszReturn;
//}
//
//static LPCTSTR GetRavidParameterAlgorithmMaximumEntropyThresholdRGBParamInputBoxTypeString(_In_ ERavidParameterAlgorithmMaximumEntropyThresholdRGB eSrc)
//{
//	static LPCTSTR lpsz[ERavidParameterAlgorithmMaximumEntropyThresholdRGB_Count] =
//	{
//		_T("COMBO"),
//		_T("COMBO"),
//		_T("COMBO"),
//		_T("COMBO"),
//		_T("EDIT"),
//		_T("EDIT"),
//		_T("EDIT"),
//		_T("EDIT"),
//		_T("EDIT"),
//		_T("EDIT"),
//		_T("EDIT"),
//		_T("EDIT"),
//	};
//
//	LPCTSTR lpszReturn = nullptr;
//
//	do
//	{
//		int nIdx = eSrc;
//
//		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmMaximumEntropyThresholdRGB_Count)
//			break;
//
//		lpszReturn = lpsz[nIdx];
//	}
//	while(false);
//
//	return lpszReturn;
//}
//
//static LPCTSTR GetRavidParameterAlgorithmMaximumEntropyThresholdRGBParamExtraDataString(_In_ ERavidParameterAlgorithmMaximumEntropyThresholdRGB eSrc)
//{
//	static LPCTSTR lpsz[ERavidParameterAlgorithmMaximumEntropyThresholdRGB_Count] =
//	{
//		_T("Greater;GreaterEqual;Equal;NotEqual;LessEqual;Less"),
//		_T("Greater;GreaterEqual;Equal;NotEqual;LessEqual;Less"),
//		_T("Greater;GreaterEqual;Equal;NotEqual;LessEqual;Less"),
//		_T("Greater;GreaterEqual;Equal;NotEqual;LessEqual;Less"),
//		_T(""),
//		_T(""),
//		_T(""),
//		_T(""),
//		_T(""),
//		_T(""),
//		_T(""),
//		_T(""),
//	};
//
//	LPCTSTR lpszReturn = nullptr;
//
//	do
//	{
//		int nIdx = eSrc;
//
//		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmMaximumEntropyThresholdRGB_Count)
//			break;
//
//		lpszReturn = lpsz[nIdx];
//	}
//	while(false);
//
//	return lpszReturn;
//}
//
//#pragma pack(push, 1)
//typedef struct sRavidAlgorithmMaximumEntropyThresholdRGBParam
//{
//	ModelParam(long, nCondition1);
//	ModelParam(long, nCondition2);
//	ModelParam(long, nCondition3);
//	ModelParam(long, nCondition4);
//	ModelParam(long, nRangeFillValue1);
//	ModelParam(long, nRangeFillValue2);
//	ModelParam(long, nRangeFillValue3);
//	ModelParam(long, nRangeFillValue4);
//	ModelParam(long, nOutOfRangeFillValue1);
//	ModelParam(long, nOutOfRangeFillValue2);
//	ModelParam(long, nOutOfRangeFillValue3);
//	ModelParam(long, nOutOfRangeFillValue4);
//}
//SRavidAlgorithmMaximumEntropyThresholdRGBParam;
//#pragma pack(pop)
//#pragma endregion

#pragma endregion

#pragma region Object Blob

#pragma region Connected Component - Single
enum ERavidParameterAlgorithmConnectedComponentSingle
{
	ERavidParameterAlgorithmConnectedComponentSingle_LogicalCondition,
	ERavidParameterAlgorithmConnectedComponentSingle_Threshold,
	ERavidParameterAlgorithmConnectedComponentSingle_Count,
};
static LPCTSTR GetRavidParameterAlgorithmConnectedComponentSingleParamNameString(_In_ ERavidParameterAlgorithmConnectedComponentSingle eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmConnectedComponentSingle_Count] =
	{
	_T("Logical Condition"),
	_T("Threshold"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmConnectedComponentSingle_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmConnectedComponentSingleParamDataTypeString(_In_ ERavidParameterAlgorithmConnectedComponentSingle eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmConnectedComponentSingle_Count] =
	{
		_T("LONG"),
		_T("LONG"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmConnectedComponentSingle_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmConnectedComponentSingleParamInputBoxTypeString(_In_ ERavidParameterAlgorithmConnectedComponentSingle eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmConnectedComponentSingle_Count] =
	{
		_T("COMBO"),
		_T("EDIT"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmConnectedComponentSingle_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmConnectedComponentSingleParamExtraDataString(_In_ ERavidParameterAlgorithmConnectedComponentSingle eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmConnectedComponentSingle_Count] =
	{
		_T("Greater;GreaterEqual;Equal;NotEqual;LessEqual;Less"),
		_T(""),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmConnectedComponentSingle_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

#pragma pack(push, 1)
typedef struct sRavidAlgorithmConnectedComponentSingleParam
{
	ModelParam(long, nLogicalCondition);
	ModelParam(long, nThreshold);
}
SRavidAlgorithmConnectedComponentSingleParam;
#pragma pack(pop)
#pragma endregion

#pragma region Connected Component - Double
enum ERavidParameterAlgorithmConnectedComponentDouble
{
	ERavidParameterAlgorithmConnectedComponentDouble_LogicalCondition,
	ERavidParameterAlgorithmConnectedComponentDouble_MinThreshold,
	ERavidParameterAlgorithmConnectedComponentDouble_MaxThreshold,
	ERavidParameterAlgorithmConnectedComponentDouble_Count,
};
static LPCTSTR GetRavidParameterAlgorithmConnectedComponentDoubleParamNameString(_In_ ERavidParameterAlgorithmConnectedComponentDouble eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmConnectedComponentDouble_Count] =
	{
	_T("Logical Condition"),
	_T("Min Threshold"),
	_T("Max Threshold"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmConnectedComponentDouble_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmConnectedComponentDoubleParamDataTypeString(_In_ ERavidParameterAlgorithmConnectedComponentDouble eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmConnectedComponentDouble_Count] =
	{
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmConnectedComponentDouble_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmConnectedComponentDoubleParamInputBoxTypeString(_In_ ERavidParameterAlgorithmConnectedComponentDouble eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmConnectedComponentDouble_Count] =
	{
		_T("COMBO"),
		_T("EDIT"),
		_T("EDIT"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmConnectedComponentDouble_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmConnectedComponentDoubleParamExtraDataString(_In_ ERavidParameterAlgorithmConnectedComponentDouble eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmConnectedComponentDouble_Count] =
	{
		_T("Equal;NotEqual"),
		_T(""),
		_T(""),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmConnectedComponentDouble_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

#pragma pack(push, 1)
typedef struct sRavidAlgorithmConnectedComponentDoubleParam
{
	ModelParam(long, nLogicalCondition);
	ModelParam(long, nMinThreshold);
	ModelParam(long, nMaxThreshold);
}
SRavidAlgorithmConnectedComponentDoubleParam;
#pragma pack(pop)
#pragma endregion

#pragma region Connected Component Filter
enum ERavidParameterAlgorithmConnectedComponentFilter
{
	ERavidParameterAlgorithmConnectedComponentFilter_Item,
	ERavidParameterAlgorithmConnectedComponentFilter_Condition,
	ERavidParameterAlgorithmConnectedComponentFilter_Value,
	ERavidParameterAlgorithmConnectedComponentFilter_Count,
};
static LPCTSTR GetRavidParameterAlgorithmConnectedComponentFilterParamNameString(_In_ ERavidParameterAlgorithmConnectedComponentFilter eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmConnectedComponentFilter_Count] =
	{
	_T("Item"),
	_T("Condition"),
	_T("Value"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmConnectedComponentFilter_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmConnectedComponentFilterParamDataTypeString(_In_ ERavidParameterAlgorithmConnectedComponentFilter eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmConnectedComponentFilter_Count] =
	{
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmConnectedComponentFilter_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmConnectedComponentFilterParamInputBoxTypeString(_In_ ERavidParameterAlgorithmConnectedComponentFilter eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmConnectedComponentFilter_Count] =
	{
		_T("COMBO"),
		_T("COMBO"),
		_T("EDIT"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmConnectedComponentFilter_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmConnectedComponentFilterParamExtraDataString(_In_ ERavidParameterAlgorithmConnectedComponentFilter eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmConnectedComponentFilter_Count] =
	{
		_T("Area;Angle;BoundingBoxCenterX;BoundingBoxCenterY;BoundingBoxWidth;BoundingBoxHeight;GravityCenterX;GravityCenterY;MinimumEnclosingRectangleCenterX;MinimumEnclosingRectangleCenterY;MinimumEnclosingRectangleHeight;MinimumEnclosingRectangleWidth;MinimumEnclosingRectangleCircularity"),
		_T("Greater;GreaterEqual;Equal;NotEqual;LessEqual;Less"),
		_T(""),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmConnectedComponentFilter_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

#pragma pack(push, 1)
typedef struct sRavidAlgorithmConnectedComponentFilterParam
{
	ModelParam(long, nItem);
	ModelParam(long, nCondition);
	ModelParam(long, nValue);
}
SRavidAlgorithmConnectedComponentFilterParam;
#pragma pack(pop)
#pragma endregion

#pragma endregion

#pragma region Finder

#pragma region Object Finder
enum ERavidParameterAlgorithmObjectFinder
{
	ERavidParameterAlgorithmObjectFinder_MaxInstance,
	ERavidParameterAlgorithmObjectFinder_MinimalDetectingScore,
	ERavidParameterAlgorithmObjectFinder_AngleBias,
	ERavidParameterAlgorithmObjectFinder_AngleTolerance,
	ERavidParameterAlgorithmObjectFinder_ScaleBias,
	ERavidParameterAlgorithmObjectFinder_ScaleTolerance,
	ERavidParameterAlgorithmObjectFinder_Interpolation,
	ERavidParameterAlgorithmObjectFinder_Count,
};
static LPCTSTR GetRavidParameterAlgorithmObjectFinderParamNameString(_In_ ERavidParameterAlgorithmObjectFinder eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmObjectFinder_Count] =
	{
		_T("Max Instance"),
		_T("Minimal Detecting Score"),
		_T("Angle Bias"),
		_T("Angle Tolerance"),
		_T("Scale Bias"),
		_T("Scale Tolerance"),
		_T("Interpolation"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmObjectFinder_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmObjectFinderParamDataTypeString(_In_ ERavidParameterAlgorithmObjectFinder eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmObjectFinder_Count] =
	{
		_T("LONG"),
		_T("DOUBLE"),
		_T("DOUBLE"),
		_T("DOUBLE"),
		_T("DOUBLE"),
		_T("DOUBLE"),
		_T("LONG"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmObjectFinder_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmObjectFinderParamInputBoxTypeString(_In_ ERavidParameterAlgorithmObjectFinder eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmObjectFinder_Count] =
	{
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
		_T("CHECK"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmObjectFinder_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmObjectFinderParamExtraDataString(_In_ ERavidParameterAlgorithmObjectFinder eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmObjectFinder_Count] =
	{
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmObjectFinder_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

#pragma pack(push, 1)
typedef struct sRavidAlgorithmObjectFinderParam
{
	ModelParam(long, nMaxInstance);
	ModelParam(double, dblMinimalDetectingScore);
	ModelParam(double, dblAngleBias);
	ModelParam(double, dblAngleTolerance);
	ModelParam(double, dblScaleBias);
	ModelParam(double, dblScaleTolerance);
	ModelParam(long, nInterpolation);
}
SRavidAlgorithmObjectFinderParam;
#pragma pack(pop)
#pragma endregion

#pragma region Shape Finder
enum ERavidParameterAlgorithmShapeFinder
{
	ERavidParameterAlgorithmShapeFinder_InstanceNumber,
	ERavidParameterAlgorithmShapeFinder_MinScore,
	ERavidParameterAlgorithmShapeFinder_AngleStart,
	ERavidParameterAlgorithmShapeFinder_AngleEnd,
	ERavidParameterAlgorithmShapeFinder_ScaleMin,
	ERavidParameterAlgorithmShapeFinder_ScaleMax,
	ERavidParameterAlgorithmShapeFinder_ExtendedArea,
	ERavidParameterAlgorithmShapeFinder_InvertContrast,
	ERavidParameterAlgorithmShapeFinder_MaxLevel,
	ERavidParameterAlgorithmShapeFinder_MinFeaturePoints,
	ERavidParameterAlgorithmShapeFinder_MaxFeaturePoints,
	ERavidParameterAlgorithmShapeFinder_StrengthModeAuto,
	ERavidParameterAlgorithmShapeFinder_Strength,
	ERavidParameterAlgorithmShapeFinder_Count,
};
static LPCTSTR GetRavidParameterAlgorithmShapeFinderParamNameString(_In_ ERavidParameterAlgorithmShapeFinder eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmShapeFinder_Count] =
	{
		_T("Instance Number"),
		_T("Min Score"),
		_T("Angle Start (Degree)"),
		_T("Angle End (Degree)"),
		_T("Scale Min"),
		_T("Scale Max"),
		_T("Extended Area"),
		_T("Invert Contrast"),
		_T("Max Level"),
		_T("Min Feature Points"),
		_T("Max Feature Points"),
		_T("Strength Mode Auto"),
		_T("Strength"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmShapeFinder_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmShapeFinderParamDataTypeString(_In_ ERavidParameterAlgorithmShapeFinder eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmShapeFinder_Count] =
	{
		_T("LONG"),
		_T("DOUBLE"),
		_T("DOUBLE"),
		_T("DOUBLE"),
		_T("DOUBLE"),
		_T("DOUBLE"),
		_T("DOUBLE"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("DOUBLE"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmShapeFinder_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmShapeFinderParamInputBoxTypeString(_In_ ERavidParameterAlgorithmShapeFinder eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmShapeFinder_Count] =
	{
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
		_T("COMBO"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
		_T("COMBO"),
		_T("EDIT"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmShapeFinder_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmShapeFinderParamExtraDataString(_In_ ERavidParameterAlgorithmShapeFinder eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmShapeFinder_Count] =
	{
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T("False;True"),
		_T(""),
		_T(""),
		_T(""),
		_T("False;True"),
		_T(""),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmShapeFinder_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

#pragma pack(push, 1)
typedef struct sRavidAlgorithmShapeFinderParam
{
	ModelParam(long, nInstanceNumber);
	ModelParam(double, dblMinScore);
	ModelParam(double, dblAngleStart);
	ModelParam(double, dblAngleEnd);
	ModelParam(double, dblScaleMin);
	ModelParam(double, dblScaleMax);
	ModelParam(double, dblExtendedArea);
	ModelParam(long, nInvertContrast);
	ModelParam(long, nMaxLevel);
	ModelParam(long, nMinFeaturePoints);
	ModelParam(long, nMaxFeaturePoints);
	ModelParam(long, nStrengthModeAuto);
	ModelParam(double, dblStrength);
}
SRavidAlgorithmShapeFinderParam;
#pragma pack(pop)
#pragma endregion

#pragma endregion

#pragma region Code

#pragma region Barcode
enum ERavidParameterAlgorithmBarcode
{
	ERavidParameterAlgorithmBarcode_Type,
	ERavidParameterAlgorithmBarcode_FindingCount,
	ERavidParameterAlgorithmBarcode_MinimumLength,
	ERavidParameterAlgorithmBarcode_Count,
};
static LPCTSTR GetRavidParameterAlgorithmBarcodeParamNameString(_In_ ERavidParameterAlgorithmBarcode eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmBarcode_Count] =
	{
		_T("Type"),
		_T("Finding Count"),
		_T("Minimum Length"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmBarcode_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmBarcodeParamDataTypeString(_In_ ERavidParameterAlgorithmBarcode eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmBarcode_Count] =
	{
		_T("LONG"),
		_T("LONG"),
		_T("DOUBLE"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmBarcode_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmBarcodeParamInputBoxTypeString(_In_ ERavidParameterAlgorithmBarcode eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmBarcode_Count] =
	{
		_T("COMBO"),
		_T("COMBO"),
		_T("EDIT"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmBarcode_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmBarcodeParamExtraDataString(_In_ ERavidParameterAlgorithmBarcode eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmBarcode_Count] =
	{
		_T("Unknown;EAN8;EAN13;EAN128;CODE11;CODE39;CODE128;MSI;UPC-A;UPC-E;Interleaved 2 of 5;Codabar"),
		_T("Single;All"),
		_T(""),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmBarcode_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

#pragma pack(push, 1)
typedef struct sRavidAlgorithmBarcodeParam
{
	ModelParam(long, nType);
	ModelParam(long, nFindingCount);
	ModelParam(double, dblMinimumLength);
}
SRavidAlgorithmBarcodeParam;
#pragma pack(pop)
#pragma endregion

#pragma region DatamatrixNormal
enum ERavidParameterAlgorithmDatamatrixNormal
{
	ERavidParameterAlgorithmDatamatrixNormal_ColorPriority,
	ERavidParameterAlgorithmDatamatrixNormal_FindingCount,
	ERavidParameterAlgorithmDatamatrixNormal_Count,
};
static LPCTSTR GetRavidParameterAlgorithmDatamatrixNormalParamNameString(_In_ ERavidParameterAlgorithmDatamatrixNormal eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmDatamatrixNormal_Count] =
	{
		_T("Color Priority"),
		_T("Finding Count"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmDatamatrixNormal_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmDatamatrixNormalParamDataTypeString(_In_ ERavidParameterAlgorithmDatamatrixNormal eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmDatamatrixNormal_Count] =
	{
		_T("LONG"),
		_T("LONG"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmDatamatrixNormal_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmDatamatrixNormalParamInputBoxTypeString(_In_ ERavidParameterAlgorithmDatamatrixNormal eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmDatamatrixNormal_Count] =
	{
		_T("COMBO"),
		_T("COMBO"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmDatamatrixNormal_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmDatamatrixNormalParamExtraDataString(_In_ ERavidParameterAlgorithmDatamatrixNormal eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmDatamatrixNormal_Count] =
	{
		_T("Black;White;Auto [White->Black];Auto [Black->White]"),
		_T("Single;All"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmDatamatrixNormal_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

#pragma pack(push, 1)
typedef struct sRavidAlgorithmDatamatrixNormalParam
{
	ModelParam(long, nColorPriority);
	ModelParam(long, nFindingCount);
}
SRavidAlgorithmDatamatrixNormalParam;
#pragma pack(pop)
#pragma endregion

#pragma region Datamatrix Expert
enum ERavidParameterAlgorithmDatamatrixExpert
{
	ERavidParameterAlgorithmDatamatrixExpert_ColorPriority,
	ERavidParameterAlgorithmDatamatrixExpert_FindingCount,
	ERavidParameterAlgorithmDatamatrixExpert_MinimumLength,
	ERavidParameterAlgorithmDatamatrixExpert_AngleRatio,
	ERavidParameterAlgorithmDatamatrixExpert_AngleUnit,
	ERavidParameterAlgorithmDatamatrixExpert_FlippingRegion,
	ERavidParameterAlgorithmDatamatrixExpert_FittingLine,
	ERavidParameterAlgorithmDatamatrixExpert_CleanImage,
	ERavidParameterAlgorithmDatamatrixExpert_UsingGaussianProcess,
	ERavidParameterAlgorithmDatamatrixExpert_UsingExplicitSize,
	ERavidParameterAlgorithmDatamatrixExpert_Row,
	ERavidParameterAlgorithmDatamatrixExpert_Column,
	ERavidParameterAlgorithmDatamatrixExpert_DetectingThresholdMethod,
	ERavidParameterAlgorithmDatamatrixExpert_OtsuThreshold,
	ERavidParameterAlgorithmDatamatrixExpert_AdaptiveThreshold,
	ERavidParameterAlgorithmDatamatrixExpert_ISODATAThreshold,
	ERavidParameterAlgorithmDatamatrixExpert_MaximumEntropyThreshold,
	ERavidParameterAlgorithmDatamatrixExpert_Count,
};
static LPCTSTR GetRavidParameterAlgorithmDatamatrixExpertParamNameString(_In_ ERavidParameterAlgorithmDatamatrixExpert eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmDatamatrixExpert_Count] =
	{
		_T("Color Priority"),
		_T("Finding Count"),
		_T("Minimum Length"),
		_T("Angle Ratio"),
		_T("Angle Unit"),
		_T("Flipping Region"),
		_T("Fitting Line"),
		_T("Clean Image"),
		_T("Using Gaussian Process"),
		_T("Using Explicit Size"),
		_T("Row"),
		_T("Column"),
		_T("Detecting Threshold Method"),
		_T("Otsu Threshold"),
		_T("Adaptive Threshold"),
		_T("ISODATA Threshold"),
		_T("Maximum Entropy Threshold"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmDatamatrixExpert_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmDatamatrixExpertParamDataTypeString(_In_ ERavidParameterAlgorithmDatamatrixExpert eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmDatamatrixExpert_Count] =
	{
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("DOUBLE"),
		_T("DOUBLE"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),

	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmDatamatrixExpert_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmDatamatrixExpertParamInputBoxTypeString(_In_ ERavidParameterAlgorithmDatamatrixExpert eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmDatamatrixExpert_Count] =
	{
		_T("COMBO"),
		_T("COMBO"),
		_T("EDIT"),
		_T("EDIT"),
		_T("EDIT"),
		_T("COMBO"),
		_T("COMBO"),
		_T("CHECK"),
		_T("CHECK"),
		_T("COMBO"),
		_T("EDIT"),
		_T("EDIT"),
		_T("COMBO"),
		_T("CHECK"),
		_T("CHECK"),
		_T("CHECK"),
		_T("CHECK"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmDatamatrixExpert_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmDatamatrixExpertParamExtraDataString(_In_ ERavidParameterAlgorithmDatamatrixExpert eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmDatamatrixExpert_Count] =
	{
		_T("Black;White;Auto [White->Black];Auto [Black->White]"),
		_T("Single;All"),
		_T(""),
		_T(""),
		_T(""),
		_T("No;Yes;Auto"),
		_T("Cluster;Hough Transform;RANSAC"),
		_T(""),
		_T(""),
		_T("No (Row and Column : -1);Yes"),
		_T(""),
		_T(""),
		_T("All (Don't use the threshold);Selection"),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmDatamatrixExpert_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

#pragma pack(push, 1)
typedef struct sRavidAlgorithmDatamatrixExpertParam
{
		ModelParam(long, nColorPriority);
		ModelParam(long, nFindingCount);
		ModelParam(long, nMinimumLength);
		ModelParam(double, dblAngleRatio);
		ModelParam(double, dblAngleUnit);
		ModelParam(long, nFlippingRegion);
		ModelParam(long, nFittingLine);
		ModelParam(long, nCleanImage);
		ModelParam(long, nUsingGaussianProcess);
		ModelParam(long, nUsingExplicitSize);
		ModelParam(long, nRow);
		ModelParam(long, nColumn);
		ModelParam(long, nDetectingThresholdMethod);
		ModelParam(long, nOtsuThreshold);
		ModelParam(long, nAdaptiveThreshold);
		ModelParam(long, nISODATAThreshold);
		ModelParam(long, nMaximumEntropyThreshold);
}
SRavidAlgorithmDatamatrixExpertParam;
#pragma pack(pop)
#pragma endregion

#pragma region QRcode Normal
enum ERavidParameterAlgorithmQRcodeNormal
{
	ERavidParameterAlgorithmQRcodeNormal_CodeColor,
	ERavidParameterAlgorithmQRcodeNormal_Count,
};
static LPCTSTR GetRavidParameterAlgorithmQRcodeNormalParamNameString(_In_ ERavidParameterAlgorithmQRcodeNormal eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmQRcodeNormal_Count] =
	{
		_T("Code Color"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmQRcodeNormal_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmQRcodeNormalParamDataTypeString(_In_ ERavidParameterAlgorithmQRcodeNormal eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmQRcodeNormal_Count] =
	{
		_T("LONG"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmQRcodeNormal_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmQRcodeNormalParamInputBoxTypeString(_In_ ERavidParameterAlgorithmQRcodeNormal eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmQRcodeNormal_Count] =
	{
		_T("COMBO"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmQRcodeNormal_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmQRcodeNormalParamExtraDataString(_In_ ERavidParameterAlgorithmQRcodeNormal eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmQRcodeNormal_Count] =
	{
		_T("Black;White;Auto [White->Black];Auto [Black->White]"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmQRcodeNormal_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

#pragma pack(push, 1)
typedef struct sRavidAlgorithmQRcodeNormalParam
{
	ModelParam(long, nCodeColor);
}
SRavidAlgorithmQRcodeNormalParam;
#pragma pack(pop)
#pragma endregion

#pragma region QRcode Expert
enum ERavidParameterAlgorithmQRcodeExpert
{
	ERavidParameterAlgorithmQRcodeExpert_CodeColor,
	ERavidParameterAlgorithmQRcodeExpert_Flip,
	ERavidParameterAlgorithmQRcodeExpert_UsingGaussianBlur,
	ERavidParameterAlgorithmQRcodeExpert_CleanImage,
	ERavidParameterAlgorithmQRcodeExpert_StickVersion,
	ERavidParameterAlgorithmQRcodeExpert_DetectingThresholdMethod,
	ERavidParameterAlgorithmQRcodeExpert_OtsuThreshold,
	ERavidParameterAlgorithmQRcodeExpert_ISODATAThreshold,
	ERavidParameterAlgorithmQRcodeExpert_MaximumEntropyThreshold,
	ERavidParameterAlgorithmQRcodeExpert_Count,
};
static LPCTSTR GetRavidParameterAlgorithmQRcodeExpertParamNameString(_In_ ERavidParameterAlgorithmQRcodeExpert eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmQRcodeExpert_Count] =
	{
		_T("Code Color"),
		_T("Flip"),
		_T("Using Gaussian Blur"),
		_T("Clean Image"),
		_T("Stick Version"),
		_T("Detecting Threshold Method"),
		_T("Otsu Threshold"),
		_T("ISODATA Threshold"),
		_T("Maximum Entropy Threshold"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmQRcodeExpert_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmQRcodeExpertParamDataTypeString(_In_ ERavidParameterAlgorithmQRcodeExpert eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmQRcodeExpert_Count] =
	{
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),
		_T("LONG"),

	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmQRcodeExpert_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmQRcodeExpertParamInputBoxTypeString(_In_ ERavidParameterAlgorithmQRcodeExpert eSrc)
{
	static LPCTSTR lpsz[ERavidParameterAlgorithmQRcodeExpert_Count] =
	{
		_T("COMBO"),
		_T("COMBO"),
		_T("CHECK"),
		_T("CHECK"),
		_T("COMBO"),
		_T("COMBO"),
		_T("CHECK"),
		_T("CHECK"),
		_T("CHECK"),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmQRcodeExpert_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

static LPCTSTR GetRavidParameterAlgorithmQRcodeExpertParamExtraDataString(_In_ ERavidParameterAlgorithmQRcodeExpert eSrc)
{
	CString strStickVersion = _T("Auto");
	for(int i = 1; i <= 5; ++i)
		strStickVersion.AppendFormat(_T(";Version%02d"), i);
	LPCTSTR lpszTemp = strStickVersion;

	static LPCTSTR lpsz[ERavidParameterAlgorithmQRcodeExpert_Count] =
	{
		_T("Black;White;Auto [White->Black];Auto [Black->White]"),
		_T("No;Yes;Auto"),
		_T(""),
		_T(""),
		lpszTemp,
		_T("All (Don't use the threshold);Selection"),
		_T(""),
		_T(""),
		_T(""),
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eSrc;

		if(nIdx < 0 || nIdx >= ERavidParameterAlgorithmQRcodeExpert_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

#pragma pack(push, 1)
typedef struct sRavidAlgorithmQRcodeExpertParam
{
		ModelParam(long, nCodeColor);
		ModelParam(long, nFlip);
		ModelParam(long, nUsingGaussianBlur);
		ModelParam(long, nCleanImage);
		ModelParam(long, nStickVersion);
		ModelParam(long, nDetectingThresholdMethod);
		ModelParam(long, nOtsuThreshold);
		ModelParam(long, nISODATAThreshold);
		ModelParam(long, nMaximumEntropyThreshold);
}
SRavidAlgorithmQRcodeExpertParam;
#pragma pack(pop)
#pragma endregion

#pragma endregion

#pragma region Insert to DB Query Data
static bool GetRavidParameterAlgorithmDataOfQuery(_In_ ERavidParameterAlgorithmSelect eSelection, _In_ int nParamNum, _Out_ CString &strParamName, _Out_ CString &strDataTypeName, _Out_ CString &strInputBoxType, _Out_ CString &strExtraData)
{
	bool bReturn = false;

	do
	{
		if(eSelection <= ERavidParameterAlgorithmSelect_None || eSelection >= ERavidParameterAlgorithmSelect_Count)
			break;

		bReturn = true;
		switch(eSelection)
		{
			//Gauge
		case ERavidParameterAlgorithmSelect_PointGauge:
			strParamName = GetRavidParameterAlgorithmPointGaugeParamNameString((ERavidParameterAlgorithmPointGauge)nParamNum);
			strDataTypeName = GetRavidParameterAlgorithmPointGaugeParamDataTypeString((ERavidParameterAlgorithmPointGauge)nParamNum);
			strInputBoxType = GetRavidParameterAlgorithmPointGaugeParamInputBoxTypeString((ERavidParameterAlgorithmPointGauge)nParamNum);
			strExtraData = GetRavidParameterAlgorithmPointGaugeParamExtraDataString((ERavidParameterAlgorithmPointGauge)nParamNum);
			break;
		case ERavidParameterAlgorithmSelect_LineGauge:
			strParamName = GetRavidParameterAlgorithmLineGaugeParamNameString((ERavidParameterAlgorithmLineGauge)nParamNum);
			strDataTypeName = GetRavidParameterAlgorithmLineGaugeParamDataTypeString((ERavidParameterAlgorithmLineGauge)nParamNum);
			strInputBoxType = GetRavidParameterAlgorithmLineGaugeParamInputBoxTypeString((ERavidParameterAlgorithmLineGauge)nParamNum);
			strExtraData = GetRavidParameterAlgorithmLineGaugeParamExtraDataString((ERavidParameterAlgorithmLineGauge)nParamNum);
			break;
		case ERavidParameterAlgorithmSelect_RectangleGauge:
			strParamName = GetRavidParameterAlgorithmRectangleGaugeParamNameString((ERavidParameterAlgorithmRectangleGauge)nParamNum);
			strDataTypeName = GetRavidParameterAlgorithmRectangleGaugeParamDataTypeString((ERavidParameterAlgorithmRectangleGauge)nParamNum);
			strInputBoxType = GetRavidParameterAlgorithmRectangleGaugeParamInputBoxTypeString((ERavidParameterAlgorithmRectangleGauge)nParamNum);
			strExtraData = GetRavidParameterAlgorithmRectangleGaugeParamExtraDataString((ERavidParameterAlgorithmRectangleGauge)nParamNum);
			break;
		case ERavidParameterAlgorithmSelect_QuadrangleGauge:
			strParamName = GetRavidParameterAlgorithmQuadrangleGaugeParamNameString((ERavidParameterAlgorithmQuadrangleGauge)nParamNum);
			strDataTypeName = GetRavidParameterAlgorithmQuadrangleGaugeParamDataTypeString((ERavidParameterAlgorithmQuadrangleGauge)nParamNum);
			strInputBoxType = GetRavidParameterAlgorithmQuadrangleGaugeParamInputBoxTypeString((ERavidParameterAlgorithmQuadrangleGauge)nParamNum);
			strExtraData = GetRavidParameterAlgorithmQuadrangleGaugeParamExtraDataString((ERavidParameterAlgorithmQuadrangleGauge)nParamNum);
			break;
		case ERavidParameterAlgorithmSelect_CircleGauge:
			strParamName = GetRavidParameterAlgorithmCircleGaugeParamNameString((ERavidParameterAlgorithmCircleGauge)nParamNum);
			strDataTypeName = GetRavidParameterAlgorithmCircleGaugeParamDataTypeString((ERavidParameterAlgorithmCircleGauge)nParamNum);
			strInputBoxType = GetRavidParameterAlgorithmCircleGaugeParamInputBoxTypeString((ERavidParameterAlgorithmCircleGauge)nParamNum);
			strExtraData = GetRavidParameterAlgorithmCircleGaugeParamExtraDataString((ERavidParameterAlgorithmCircleGauge)nParamNum);
			break;
		case ERavidParameterAlgorithmSelect_EllipseGauge:
			strParamName = GetRavidParameterAlgorithmEllipseGaugeParamNameString((ERavidParameterAlgorithmEllipseGauge)nParamNum);
			strDataTypeName = GetRavidParameterAlgorithmEllipseGaugeParamDataTypeString((ERavidParameterAlgorithmEllipseGauge)nParamNum);
			strInputBoxType = GetRavidParameterAlgorithmEllipseGaugeParamInputBoxTypeString((ERavidParameterAlgorithmEllipseGauge)nParamNum);
			strExtraData = GetRavidParameterAlgorithmEllipseGaugeParamExtraDataString((ERavidParameterAlgorithmEllipseGauge)nParamNum);
			break;

			//Image Processing
		case ERavidParameterAlgorithmSelect_Morphology:
			strParamName = GetRavidParameterAlgorithmMorphologyParamNameString((ERavidParameterAlgorithmMorphology)nParamNum);
			strDataTypeName = GetRavidParameterAlgorithmMorphologyParamDataTypeString((ERavidParameterAlgorithmMorphology)nParamNum);
			strInputBoxType = GetRavidParameterAlgorithmMorphologyParamInputBoxTypeString((ERavidParameterAlgorithmMorphology)nParamNum);
			strExtraData = GetRavidParameterAlgorithmMorphologyParamExtraDataString((ERavidParameterAlgorithmMorphology)nParamNum);
			break;

		case ERavidParameterAlgorithmSelect_SingleThreshold:
			strParamName = GetRavidParameterAlgorithmSingleThresholdParamNameString((ERavidParameterAlgorithmSingleThreshold)nParamNum);
			strDataTypeName = GetRavidParameterAlgorithmSingleThresholdParamDataTypeString((ERavidParameterAlgorithmSingleThreshold)nParamNum);
			strInputBoxType = GetRavidParameterAlgorithmSingleThresholdParamInputBoxTypeString((ERavidParameterAlgorithmSingleThreshold)nParamNum);
			strExtraData = GetRavidParameterAlgorithmSingleThresholdParamExtraDataString((ERavidParameterAlgorithmSingleThreshold)nParamNum);
			break;
		case ERavidParameterAlgorithmSelect_DoubleThreshold:
			strParamName = GetRavidParameterAlgorithmDoubleThresholdParamNameString((ERavidParameterAlgorithmDoubleThreshold)nParamNum);
			strDataTypeName = GetRavidParameterAlgorithmDoubleThresholdParamDataTypeString((ERavidParameterAlgorithmDoubleThreshold)nParamNum);
			strInputBoxType = GetRavidParameterAlgorithmDoubleThresholdParamInputBoxTypeString((ERavidParameterAlgorithmDoubleThreshold)nParamNum);
			strExtraData = GetRavidParameterAlgorithmDoubleThresholdParamExtraDataString((ERavidParameterAlgorithmDoubleThreshold)nParamNum);
			break;
		case ERavidParameterAlgorithmSelect_OtsuThreshold:
			strParamName = GetRavidParameterAlgorithmOtsuThresholdParamNameString((ERavidParameterAlgorithmOtsuThreshold)nParamNum);
			strDataTypeName = GetRavidParameterAlgorithmOtsuThresholdParamDataTypeString((ERavidParameterAlgorithmOtsuThreshold)nParamNum);
			strInputBoxType = GetRavidParameterAlgorithmOtsuThresholdParamInputBoxTypeString((ERavidParameterAlgorithmOtsuThreshold)nParamNum);
			strExtraData = GetRavidParameterAlgorithmOtsuThresholdParamExtraDataString((ERavidParameterAlgorithmOtsuThreshold)nParamNum);
			break;
		case ERavidParameterAlgorithmSelect_AdaptiveThreshold:
			strParamName = GetRavidParameterAlgorithmAdaptiveThresholdParamNameString((ERavidParameterAlgorithmAdaptiveThreshold)nParamNum);
			strDataTypeName = GetRavidParameterAlgorithmAdaptiveThresholdParamDataTypeString((ERavidParameterAlgorithmAdaptiveThreshold)nParamNum);
			strInputBoxType = GetRavidParameterAlgorithmAdaptiveThresholdParamInputBoxTypeString((ERavidParameterAlgorithmAdaptiveThreshold)nParamNum);
			strExtraData = GetRavidParameterAlgorithmAdaptiveThresholdParamExtraDataString((ERavidParameterAlgorithmAdaptiveThreshold)nParamNum);
			break;
		case ERavidParameterAlgorithmSelect_ISODATAThreshold:
			strParamName = GetRavidParameterAlgorithmISODATAThresholdParamNameString((ERavidParameterAlgorithmISODATAThreshold)nParamNum);
			strDataTypeName = GetRavidParameterAlgorithmISODATAThresholdParamDataTypeString((ERavidParameterAlgorithmISODATAThreshold)nParamNum);
			strInputBoxType = GetRavidParameterAlgorithmISODATAThresholdParamInputBoxTypeString((ERavidParameterAlgorithmISODATAThreshold)nParamNum);
			strExtraData = GetRavidParameterAlgorithmISODATAThresholdParamExtraDataString((ERavidParameterAlgorithmISODATAThreshold)nParamNum);
			break;
		case ERavidParameterAlgorithmSelect_MaximumEntropyThreshold:
			strParamName = GetRavidParameterAlgorithmMaximumEntropyThresholdParamNameString((ERavidParameterAlgorithmMaximumEntropyThreshold)nParamNum);
			strDataTypeName = GetRavidParameterAlgorithmMaximumEntropyThresholdParamDataTypeString((ERavidParameterAlgorithmMaximumEntropyThreshold)nParamNum);
			strInputBoxType = GetRavidParameterAlgorithmMaximumEntropyThresholdParamInputBoxTypeString((ERavidParameterAlgorithmMaximumEntropyThreshold)nParamNum);
			strExtraData = GetRavidParameterAlgorithmMaximumEntropyThresholdParamExtraDataString((ERavidParameterAlgorithmMaximumEntropyThreshold)nParamNum);
			break;

		case ERavidParameterAlgorithmSelect_HarrisCornersDetector:
			strParamName = GetRavidParameterAlgorithmHarrisCornersDetectorParamNameString((ERavidParameterAlgorithmHarrisCornersDetector)nParamNum);
			strDataTypeName = GetRavidParameterAlgorithmHarrisCornersDetectorParamDataTypeString((ERavidParameterAlgorithmHarrisCornersDetector)nParamNum);
			strInputBoxType = GetRavidParameterAlgorithmHarrisCornersDetectorParamInputBoxTypeString((ERavidParameterAlgorithmHarrisCornersDetector)nParamNum);
			strExtraData = GetRavidParameterAlgorithmHarrisCornersDetectorParamExtraDataString((ERavidParameterAlgorithmHarrisCornersDetector)nParamNum);
			break;
		case ERavidParameterAlgorithmSelect_HoughTransformCircle:
			strParamName = GetRavidParameterAlgorithmHoughTransformCircleParamNameString((ERavidParameterAlgorithmHoughTransformCircle)nParamNum);
			strDataTypeName = GetRavidParameterAlgorithmHoughTransformCircleParamDataTypeString((ERavidParameterAlgorithmHoughTransformCircle)nParamNum);
			strInputBoxType = GetRavidParameterAlgorithmHoughTransformCircleParamInputBoxTypeString((ERavidParameterAlgorithmHoughTransformCircle)nParamNum);
			strExtraData = GetRavidParameterAlgorithmHoughTransformCircleParamExtraDataString((ERavidParameterAlgorithmHoughTransformCircle)nParamNum);
			break;
		case ERavidParameterAlgorithmSelect_HoughTransformLine:
			strParamName = GetRavidParameterAlgorithmHoughTransformLineParamNameString((ERavidParameterAlgorithmHoughTransformLine)nParamNum);
			strDataTypeName = GetRavidParameterAlgorithmHoughTransformLineParamDataTypeString((ERavidParameterAlgorithmHoughTransformLine)nParamNum);
			strInputBoxType = GetRavidParameterAlgorithmHoughTransformLineParamInputBoxTypeString((ERavidParameterAlgorithmHoughTransformLine)nParamNum);
			strExtraData = GetRavidParameterAlgorithmHoughTransformLineParamExtraDataString((ERavidParameterAlgorithmHoughTransformLine)nParamNum);
			break;
		case ERavidParameterAlgorithmSelect_Move:
			strParamName = GetRavidParameterAlgorithmMoveParamNameString((ERavidParameterAlgorithmMove)nParamNum);
			strDataTypeName = GetRavidParameterAlgorithmMoveParamDataTypeString((ERavidParameterAlgorithmMove)nParamNum);
			strInputBoxType = GetRavidParameterAlgorithmMoveParamInputBoxTypeString((ERavidParameterAlgorithmMove)nParamNum);
			strExtraData = GetRavidParameterAlgorithmMoveParamExtraDataString((ERavidParameterAlgorithmMove)nParamNum);
			break;
		case ERavidParameterAlgorithmSelect_Rotate:
			strParamName = GetRavidParameterAlgorithmRotateParamNameString((ERavidParameterAlgorithmRotate)nParamNum);
			strDataTypeName = GetRavidParameterAlgorithmRotateParamDataTypeString((ERavidParameterAlgorithmRotate)nParamNum);
			strInputBoxType = GetRavidParameterAlgorithmRotateParamInputBoxTypeString((ERavidParameterAlgorithmRotate)nParamNum);
			strExtraData = GetRavidParameterAlgorithmRotateParamExtraDataString((ERavidParameterAlgorithmRotate)nParamNum);
			break;
		case ERavidParameterAlgorithmSelect_Scale:
			strParamName = GetRavidParameterAlgorithmScaleParamNameString((ERavidParameterAlgorithmScale)nParamNum);
			strDataTypeName = GetRavidParameterAlgorithmScaleParamDataTypeString((ERavidParameterAlgorithmScale)nParamNum);
			strInputBoxType = GetRavidParameterAlgorithmScaleParamInputBoxTypeString((ERavidParameterAlgorithmScale)nParamNum);
			strExtraData = GetRavidParameterAlgorithmScaleParamExtraDataString((ERavidParameterAlgorithmScale)nParamNum);
			break;
		case ERavidParameterAlgorithmSelect_Operation:
			strParamName = GetRavidParameterAlgorithmOperationParamNameString((ERavidParameterAlgorithmOperation)nParamNum);
			strDataTypeName = GetRavidParameterAlgorithmOperationParamDataTypeString((ERavidParameterAlgorithmOperation)nParamNum);
			strInputBoxType = GetRavidParameterAlgorithmOperationParamInputBoxTypeString((ERavidParameterAlgorithmOperation)nParamNum);
			strExtraData = GetRavidParameterAlgorithmOperationParamExtraDataString((ERavidParameterAlgorithmOperation)nParamNum);
			break;
		case ERavidParameterAlgorithmSelect_Projection:
			strParamName = GetRavidParameterAlgorithmProjectionParamNameString((ERavidParameterAlgorithmProjection)nParamNum);
			strDataTypeName = GetRavidParameterAlgorithmProjectionParamDataTypeString((ERavidParameterAlgorithmProjection)nParamNum);
			strInputBoxType = GetRavidParameterAlgorithmProjectionParamInputBoxTypeString((ERavidParameterAlgorithmProjection)nParamNum);
			strExtraData = GetRavidParameterAlgorithmProjectionParamExtraDataString((ERavidParameterAlgorithmProjection)nParamNum);
			break;
		case ERavidParameterAlgorithmSelect_Stretch:
			strParamName = GetRavidParameterAlgorithmStretchParamNameString((ERavidParameterAlgorithmStretch)nParamNum);
			strDataTypeName = GetRavidParameterAlgorithmStretchParamDataTypeString((ERavidParameterAlgorithmStretch)nParamNum);
			strInputBoxType = GetRavidParameterAlgorithmStretchParamInputBoxTypeString((ERavidParameterAlgorithmStretch)nParamNum);
			strExtraData = GetRavidParameterAlgorithmStretchParamExtraDataString((ERavidParameterAlgorithmStretch)nParamNum);
			break;
		case ERavidParameterAlgorithmSelect_Gain:
			strParamName = GetRavidParameterAlgorithmGainParamNameString((ERavidParameterAlgorithmGain)nParamNum);
			strDataTypeName = GetRavidParameterAlgorithmGainParamDataTypeString((ERavidParameterAlgorithmGain)nParamNum);
			strInputBoxType = GetRavidParameterAlgorithmGainParamInputBoxTypeString((ERavidParameterAlgorithmGain)nParamNum);
			strExtraData = GetRavidParameterAlgorithmGainParamExtraDataString((ERavidParameterAlgorithmGain)nParamNum);
			break;

		//case ERavidParameterAlgorithmSelect_SingleThresholdRGB:
		//	strParamName = GetRavidParameterAlgorithmSingleThresholdRGBParamNameString((ERavidParameterAlgorithmSingleThresholdRGB)nParamNum);
		//	strDataTypeName = GetRavidParameterAlgorithmSingleThresholdRGBParamDataTypeString((ERavidParameterAlgorithmSingleThresholdRGB)nParamNum);
		//	strInputBoxType = GetRavidParameterAlgorithmSingleThresholdRGBParamInputBoxTypeString((ERavidParameterAlgorithmSingleThresholdRGB)nParamNum);
		//	strExtraData = GetRavidParameterAlgorithmSingleThresholdRGBParamExtraDataString((ERavidParameterAlgorithmSingleThresholdRGB)nParamNum);
		//	break;
		//case ERavidParameterAlgorithmSelect_DoubleThresholdRGB:
		//	strParamName = GetRavidParameterAlgorithmDoubleThresholdRGBParamNameString((ERavidParameterAlgorithmDoubleThresholdRGB)nParamNum);
		//	strDataTypeName = GetRavidParameterAlgorithmDoubleThresholdRGBParamDataTypeString((ERavidParameterAlgorithmDoubleThresholdRGB)nParamNum);
		//	strInputBoxType = GetRavidParameterAlgorithmDoubleThresholdRGBParamInputBoxTypeString((ERavidParameterAlgorithmDoubleThresholdRGB)nParamNum);
		//	strExtraData = GetRavidParameterAlgorithmDoubleThresholdRGBParamExtraDataString((ERavidParameterAlgorithmDoubleThresholdRGB)nParamNum);
		//	break;
		//case ERavidParameterAlgorithmSelect_OtsuThresholdRGB:
		//	strParamName = GetRavidParameterAlgorithmOtsuThresholdRGBParamNameString((ERavidParameterAlgorithmOtsuThresholdRGB)nParamNum);
		//	strDataTypeName = GetRavidParameterAlgorithmOtsuThresholdRGBParamDataTypeString((ERavidParameterAlgorithmOtsuThresholdRGB)nParamNum);
		//	strInputBoxType = GetRavidParameterAlgorithmOtsuThresholdRGBParamInputBoxTypeString((ERavidParameterAlgorithmOtsuThresholdRGB)nParamNum);
		//	strExtraData = GetRavidParameterAlgorithmOtsuThresholdRGBParamExtraDataString((ERavidParameterAlgorithmOtsuThresholdRGB)nParamNum);
		//	break;
		//case ERavidParameterAlgorithmSelect_AdaptiveThresholdRGB:
		//	strParamName = GetRavidParameterAlgorithmAdaptiveThresholdRGBParamNameString((ERavidParameterAlgorithmAdaptiveThresholdRGB)nParamNum);
		//	strDataTypeName = GetRavidParameterAlgorithmAdaptiveThresholdRGBParamDataTypeString((ERavidParameterAlgorithmAdaptiveThresholdRGB)nParamNum);
		//	strInputBoxType = GetRavidParameterAlgorithmAdaptiveThresholdRGBParamInputBoxTypeString((ERavidParameterAlgorithmAdaptiveThresholdRGB)nParamNum);
		//	strExtraData = GetRavidParameterAlgorithmAdaptiveThresholdRGBParamExtraDataString((ERavidParameterAlgorithmAdaptiveThresholdRGB)nParamNum);
		//	break;
		//case ERavidParameterAlgorithmSelect_ISODATAThresholdRGB:
		//	strParamName = GetRavidParameterAlgorithmISODATAThresholdRGBParamNameString((ERavidParameterAlgorithmISODATAThresholdRGB)nParamNum);
		//	strDataTypeName = GetRavidParameterAlgorithmISODATAThresholdRGBParamDataTypeString((ERavidParameterAlgorithmISODATAThresholdRGB)nParamNum);
		//	strInputBoxType = GetRavidParameterAlgorithmISODATAThresholdRGBParamInputBoxTypeString((ERavidParameterAlgorithmISODATAThresholdRGB)nParamNum);
		//	strExtraData = GetRavidParameterAlgorithmISODATAThresholdRGBParamExtraDataString((ERavidParameterAlgorithmISODATAThresholdRGB)nParamNum);
		//	break;
		//case ERavidParameterAlgorithmSelect_MaximumEntropyThresholdRGB:
		//	strParamName = GetRavidParameterAlgorithmMaximumEntropyThresholdRGBParamNameString((ERavidParameterAlgorithmMaximumEntropyThresholdRGB)nParamNum);
		//	strDataTypeName = GetRavidParameterAlgorithmMaximumEntropyThresholdRGBParamDataTypeString((ERavidParameterAlgorithmMaximumEntropyThresholdRGB)nParamNum);
		//	strInputBoxType = GetRavidParameterAlgorithmMaximumEntropyThresholdRGBParamInputBoxTypeString((ERavidParameterAlgorithmMaximumEntropyThresholdRGB)nParamNum);
		//	strExtraData = GetRavidParameterAlgorithmMaximumEntropyThresholdRGBParamExtraDataString((ERavidParameterAlgorithmMaximumEntropyThresholdRGB)nParamNum);
		//	break;

			//Object Blob
		case ERavidParameterAlgorithmSelect_ConnectedComponentSingle:
			strParamName = GetRavidParameterAlgorithmConnectedComponentSingleParamNameString((ERavidParameterAlgorithmConnectedComponentSingle)nParamNum);
			strDataTypeName = GetRavidParameterAlgorithmConnectedComponentSingleParamDataTypeString((ERavidParameterAlgorithmConnectedComponentSingle)nParamNum);
			strInputBoxType = GetRavidParameterAlgorithmConnectedComponentSingleParamInputBoxTypeString((ERavidParameterAlgorithmConnectedComponentSingle)nParamNum);
			strExtraData = GetRavidParameterAlgorithmConnectedComponentSingleParamExtraDataString((ERavidParameterAlgorithmConnectedComponentSingle)nParamNum);
			break;
		case ERavidParameterAlgorithmSelect_ConnectedComponentDouble:
			strParamName = GetRavidParameterAlgorithmConnectedComponentDoubleParamNameString((ERavidParameterAlgorithmConnectedComponentDouble)nParamNum);
			strDataTypeName = GetRavidParameterAlgorithmConnectedComponentDoubleParamDataTypeString((ERavidParameterAlgorithmConnectedComponentDouble)nParamNum);
			strInputBoxType = GetRavidParameterAlgorithmConnectedComponentDoubleParamInputBoxTypeString((ERavidParameterAlgorithmConnectedComponentDouble)nParamNum);
			strExtraData = GetRavidParameterAlgorithmConnectedComponentDoubleParamExtraDataString((ERavidParameterAlgorithmConnectedComponentDouble)nParamNum);
			break;
		case ERavidParameterAlgorithmSelect_ConnectedComponentFilter:
			strParamName = GetRavidParameterAlgorithmConnectedComponentFilterParamNameString((ERavidParameterAlgorithmConnectedComponentFilter)nParamNum);
			strDataTypeName = GetRavidParameterAlgorithmConnectedComponentFilterParamDataTypeString((ERavidParameterAlgorithmConnectedComponentFilter)nParamNum);
			strInputBoxType = GetRavidParameterAlgorithmConnectedComponentFilterParamInputBoxTypeString((ERavidParameterAlgorithmConnectedComponentFilter)nParamNum);
			strExtraData = GetRavidParameterAlgorithmConnectedComponentFilterParamExtraDataString((ERavidParameterAlgorithmConnectedComponentFilter)nParamNum);
			break;

			//Finder
		case ERavidParameterAlgorithmSelect_ObjectFinder:
			strParamName = GetRavidParameterAlgorithmObjectFinderParamNameString((ERavidParameterAlgorithmObjectFinder)nParamNum);
			strDataTypeName = GetRavidParameterAlgorithmObjectFinderParamDataTypeString((ERavidParameterAlgorithmObjectFinder)nParamNum);
			strInputBoxType = GetRavidParameterAlgorithmObjectFinderParamInputBoxTypeString((ERavidParameterAlgorithmObjectFinder)nParamNum);
			strExtraData = GetRavidParameterAlgorithmObjectFinderParamExtraDataString((ERavidParameterAlgorithmObjectFinder)nParamNum);
			break;
		case ERavidParameterAlgorithmSelect_ShapeFinder:
			strParamName = GetRavidParameterAlgorithmShapeFinderParamNameString((ERavidParameterAlgorithmShapeFinder)nParamNum);
			strDataTypeName = GetRavidParameterAlgorithmShapeFinderParamDataTypeString((ERavidParameterAlgorithmShapeFinder)nParamNum);
			strInputBoxType = GetRavidParameterAlgorithmShapeFinderParamInputBoxTypeString((ERavidParameterAlgorithmShapeFinder)nParamNum);
			strExtraData = GetRavidParameterAlgorithmShapeFinderParamExtraDataString((ERavidParameterAlgorithmShapeFinder)nParamNum);
			break;

			//Code
		case ERavidParameterAlgorithmSelect_Barcode:
			strParamName = GetRavidParameterAlgorithmBarcodeParamNameString((ERavidParameterAlgorithmBarcode)nParamNum);
			strDataTypeName = GetRavidParameterAlgorithmBarcodeParamDataTypeString((ERavidParameterAlgorithmBarcode)nParamNum);
			strInputBoxType = GetRavidParameterAlgorithmBarcodeParamInputBoxTypeString((ERavidParameterAlgorithmBarcode)nParamNum);
			strExtraData = GetRavidParameterAlgorithmBarcodeParamExtraDataString((ERavidParameterAlgorithmBarcode)nParamNum);
			break;
		case ERavidParameterAlgorithmSelect_DatamatrixNormal:
			strParamName = GetRavidParameterAlgorithmDatamatrixNormalParamNameString((ERavidParameterAlgorithmDatamatrixNormal)nParamNum);
			strDataTypeName = GetRavidParameterAlgorithmDatamatrixNormalParamDataTypeString((ERavidParameterAlgorithmDatamatrixNormal)nParamNum);
			strInputBoxType = GetRavidParameterAlgorithmDatamatrixNormalParamInputBoxTypeString((ERavidParameterAlgorithmDatamatrixNormal)nParamNum);
			strExtraData = GetRavidParameterAlgorithmDatamatrixNormalParamExtraDataString((ERavidParameterAlgorithmDatamatrixNormal)nParamNum);
			break;
		case ERavidParameterAlgorithmSelect_DatamatrixExpert:
			strParamName = GetRavidParameterAlgorithmDatamatrixExpertParamNameString((ERavidParameterAlgorithmDatamatrixExpert)nParamNum);
			strDataTypeName = GetRavidParameterAlgorithmDatamatrixExpertParamDataTypeString((ERavidParameterAlgorithmDatamatrixExpert)nParamNum);
			strInputBoxType = GetRavidParameterAlgorithmDatamatrixExpertParamInputBoxTypeString((ERavidParameterAlgorithmDatamatrixExpert)nParamNum);
			strExtraData = GetRavidParameterAlgorithmDatamatrixExpertParamExtraDataString((ERavidParameterAlgorithmDatamatrixExpert)nParamNum);
			break;
		case ERavidParameterAlgorithmSelect_QRcodeNormal:
			strParamName = GetRavidParameterAlgorithmQRcodeNormalParamNameString((ERavidParameterAlgorithmQRcodeNormal)nParamNum);
			strDataTypeName = GetRavidParameterAlgorithmQRcodeNormalParamDataTypeString((ERavidParameterAlgorithmQRcodeNormal)nParamNum);
			strInputBoxType = GetRavidParameterAlgorithmQRcodeNormalParamInputBoxTypeString((ERavidParameterAlgorithmQRcodeNormal)nParamNum);
			strExtraData = GetRavidParameterAlgorithmQRcodeNormalParamExtraDataString((ERavidParameterAlgorithmQRcodeNormal)nParamNum);
			break;
		case ERavidParameterAlgorithmSelect_QRcodeExpert:
			strParamName = GetRavidParameterAlgorithmQRcodeExpertParamNameString((ERavidParameterAlgorithmQRcodeExpert)nParamNum);
			strDataTypeName = GetRavidParameterAlgorithmQRcodeExpertParamDataTypeString((ERavidParameterAlgorithmQRcodeExpert)nParamNum);
			strInputBoxType = GetRavidParameterAlgorithmQRcodeExpertParamInputBoxTypeString((ERavidParameterAlgorithmQRcodeExpert)nParamNum);
			strExtraData = GetRavidParameterAlgorithmQRcodeExpertParamExtraDataString((ERavidParameterAlgorithmQRcodeExpert)nParamNum);
			break;

		default:
			bReturn = false;
			break;
		}

	}
	while(false);

	return bReturn;
}
#pragma endregion
