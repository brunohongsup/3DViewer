#pragma once

#include <vector>
#include <functional>

#define TViewThreadDisplayer std::function<void(void)>
#define TViewThreadDisplayerLamdaFormula [=](void) -> void
#define MakeViewThreadDisplayer(pFunc) TViewThreadDisplayer* pFunc = new TViewThreadDisplayer; *pFunc = TViewThreadDisplayerLamdaFormula

#define InitializeThreadDisplayer() this->SetTimer(TIMER_THREAD_DISPLAYER, 10, nullptr)
#define TerminateThreadDisplayer() this->KillTimer(TIMER_THREAD_DISPLAYER)
#define ImplementThreadDisplayer() if(nIDEvent == TIMER_THREAD_DISPLAYER) { while(IsDisplayerExisting()) { RunThreadDisplayer(); } }

#define DPISCALE []() -> double{ double dblScale = 0.; HDC hDC = ::GetDC(nullptr); if(hDC){ dblScale = (double)GetDeviceCaps(hDC, LOGPIXELSX) / 96.; ::ReleaseDC(nullptr, hDC); } return dblScale; }();

#define RAVID_IMAGE_VIEW_MAX_SCALE 1000.f

#define TryCatchBegin try {
#define TryCatchException } catch(...){
#define TryCatchEnd };

enum ERavidMainFrameToolBarID
{
	ERavidMainFrameToolBarID_OpenModelManager = 32773,
	ERavidMainFrameToolBarID_OpenDeviceManager = 32799,
	ERavidMainFrameToolBarID_LogIn = 32800,
	ERavidMainFrameToolBarID_LogOut = 32801,
	ERavidMainFrameToolBarID_OpenAuthorityManager = 32816,
	ERavidMainFrameToolBarID_ScreenSaverSet = 32817,
	ERavidMainFrameToolBarID_SequenceStart = 32774,
	ERavidMainFrameToolBarID_SequencePause = 32775,
	ERavidMainFrameToolBarID_SequenceStop = 32776,
	ERavidMainFrameToolBarID_RunUIConfiguration = 32819,
	ERavidMainFrameToolBarID_LogViewerOpen = 32783,
	ERavidMainFrameToolBarID_LogViewerClose = 32787,
	ERavidMainFrameToolBarID_ImageProcessingTool = 32802,
	ERavidMainFrameToolBarID_OpenViewTool = 32825,
	ERavidMainFrameToolBarID_OpenDeviceTool = 32818,
	ERavidMainFrameToolBarID_OpenFrameworkTool = 32831,
	ERavidMainFrameToolBarID_Security = 32833,
	ERavidMainFrameToolBarID_LanguageChange = 32836,
	ERavidMainFrameToolBarID_Count,
};

enum ERavidImageViewPenStyle
{
	ERavidImageViewPenStyle_SOLID = 0,
	ERavidImageViewPenStyle_DASH,			/* -------  */
	ERavidImageViewPenStyle_DOT,			/* .......  */
	ERavidImageViewPenStyle_DASHDOT,		/* _._._._  */
	ERavidImageViewPenStyle_DASHDOTDOT,		/* _.._.._  */
	ERavidImageViewPenStyle_NULL,
	ERavidImageViewPenStyle_INSIDEFRAME,
	ERavidImageViewPenStyle_USERSTYLE,
	ERavidImageViewPenStyle_ALTERNATE,
	ERavidImageViewPenStyle_Count,
};

enum ERavidCaptionButtonKind
{
	ERavidCaptionButtonKind_Close = 0,
	ERavidCaptionButtonKind_Maximum,
	ERavidCaptionButtonKind_Minimum,
	ERavidCaptionButtonKind_Count,
};

enum ERavidCaptionButtonStatus
{
	ERavidCaptionButtonStatus_Normal = 0,
	ERavidCaptionButtonStatus_Maximum,
};

enum  ERavidCaptionButtonAction
{
	ERavidCaptionButtonAction_Normal = 0,
	ERavidCaptionButtonAction_Hover,
	ERavidCaptionButtonAction_Click,
};


typedef struct sRavidMenuItemInfo
{
	bool bIsNeedUpdate = false;
	RECT rcMenuItemPosition = CRect(0, 0, 0, 0);
	UINT uiMenuItemStatus = 0;
	UINT uiMenuItemAction = 0;
	CString* pStrMenuItem = nullptr;
	HBITMAP hbmpChecked = 0;
	HBITMAP hbmpUnchecked = 0;
}
SRavidMenuItemInfo;

typedef struct sRavidCaptionButtonInfo
{
	bool bIsEnable = true;
	bool bIsNeedUpdate = false;
	RECT rcCaptionButtonPosition = CRect(0, 0, 0, 0);
	UINT uiCaptionButtonStatus = 0;
	UINT uiCaptionButtonAction = 0;
}
SRavidCaptionButtonInfo;

enum ERavidFrameSizeChangeMode
{
	ERavidFrameSizeChangeMode_None = 0,
	ERavidFrameSizeChangeMode_LeftTop,
	ERavidFrameSizeChangeMode_RightTop,
	ERavidFrameSizeChangeMode_RightBottom,
	ERavidFrameSizeChangeMode_LeftBottom,
	ERavidFrameSizeChangeMode_Top,
	ERavidFrameSizeChangeMode_Right,
	ERavidFrameSizeChangeMode_Bottom,
	ERavidFrameSizeChangeMode_Left,
	ERavidFrameSizeChangeMode_Move,
};

enum ERavidFrameNeighborPosition
{
	ERavidFrameNeighborPosition_None = 0,
	ERavidFrameNeighborPosition_Left,
	ERavidFrameNeighborPosition_Top,
	ERavidFrameNeighborPosition_Right,
	ERavidFrameNeighborPosition_Bottom,
	ERavidFrameNeighborPosition_Overlap,
};

typedef struct sRavidFrameNeighborChildFrame
{
	ERavidFrameNeighborPosition ePos = ERavidFrameNeighborPosition_None;
	LPVOID pParam = nullptr;
}
SRavidFrameNeighborChildFrame;


namespace Ravid
{
	namespace Framework
	{
		class CRavidChildFrameBase;

		typedef struct sRavidChildFrameRect
		{
			CRavidChildFrameBase* pChildFrame;
			RECT rcCurrentWindow;
			RECT rcNextWindow;
			bool bIsChanged;
			std::vector<sRavidFrameNeighborChildFrame> vctNeighborChildFrame;

			sRavidChildFrameRect()
			{
				pChildFrame = nullptr;
				ZeroMemory(&rcCurrentWindow, sizeof(rcCurrentWindow));
				ZeroMemory(&rcNextWindow, sizeof(rcNextWindow));
				bIsChanged = false;
				vctNeighborChildFrame.clear();
			}
		}
		SRavidChildFrameRect;
	}
}

enum ERavidSkinType
{
	ERavidSkinType_Dark = 0,
	ERavidSkinType_Bright,
	ERavidSkinType_Red,
	ERavidSkinType_Rand,
};


typedef struct sRavidDialogAdaptiveScaleInfo
{
	CRect crDefault;
	CFont* pFont;
	long nFontSize;

	sRavidDialogAdaptiveScaleInfo()
	{
		crDefault = CRect(0, 0, 0, 0);
		nFontSize = -11;
		pFont = nullptr;
	}
}
SRavidDialogAdaptiveScaleInfo;

typedef struct sRavidDibInfo
{
	BITMAPINFOHEADER	header;
	RGBQUAD				acolors[256];
}
SRavidDibInfo;


enum ERavidToolBarButtonAlign
{
	ERavidToolBarButtonAlign_Left = 0,
	ERavidToolBarButtonAlign_Right,
};


enum
{
	ERavidImageViewLayerTransparencyColor = 0x7f7f7f7f,
};

enum EUIConfigElementPositionUnit
{
	EUIConfigElementPositionUnit_Percentage = 0,
	EUIConfigElementPositionUnit_PixelCoordinates,
};

namespace Ravid
{
	namespace Framework
	{
		enum ERavidToolBarEventMainFrame
		{
			ERavidToolBarEventMainFrame_OpenModelManager = 0,
			ERavidToolBarEventMainFrame_OpenDeviceManager,
			ERavidToolBarEventMainFrame_SeparatorManager,
			ERavidToolBarEventMainFrame_LogIn,
			ERavidToolBarEventMainFrame_LogOut,
			ERavidToolBarEventMainFrame_OpenAuthorityManager,
			ERavidToolBarEventMainFrame_ScreenSaverSet,
			ERavidToolBarEventMainFrame_SeparatorAuthority,
			ERavidToolBarEventMainFrame_SequenceStart,
			ERavidToolBarEventMainFrame_SequencePause,
			ERavidToolBarEventMainFrame_SequenceStop,
			ERavidToolBarEventMainFrame_SeparatorSequence,
			ERavidToolBarEventMainFrame_RunUIConfiguration,
			ERavidToolBarEventMainFrame_SeparatorUI,
			ERavidToolBarEventMainFrame_LogViewerOpen,
			ERavidToolBarEventMainFrame_LogViewerClose,
			ERavidToolBarEventMainFrame_SeparatorLogViewer,
			ERavidToolBarEventMainFrame_ImageProcessingTool,
			ERavidToolBarEventMainFrame_OpenViewTool,
			ERavidToolBarEventMainFrame_OpenDeviceTool,
			ERavidToolBarEventMainFrame_OpenFrameworkTool,
			ERavidToolBarEventMainFrame_Security,
			ERavidToolBarEventMainFrame_SeparatorLanguage,
			ERavidToolBarEventMainFrame_LanguageChange,
			ERavidToolBarEventMainFrame_Count,
		};

		enum ERavidToolBarEventChildFrameBase
		{
			ERavidToolBarEventChildFrameBase_Maximize = 0,
			ERavidToolBarEventChildFrameBase_Restore,
			ERavidToolBarEventChildFrameBase_Count,
		};

		enum ERavidToolBarEventImageView
		{
			ERavidToolBarEventImageView_ImageOpen = 0,
			ERavidToolBarEventImageView_ImageSave,
			ERavidToolBarEventImageView_ImageClose,
			ERavidToolBarEventImageView_Inspect,
			ERavidToolBarEventImageView_Teaching,
			ERavidToolBarEventImageView_EditTeaching,
			ERavidToolBarEventImageView_Arrow,
			ERavidToolBarEventImageView_ZoomIn,
			ERavidToolBarEventImageView_ZoomOut,
			ERavidToolBarEventImageView_ZoomAll,
			ERavidToolBarEventImageView_Move,
			ERavidToolBarEventImageView_Measure,
			ERavidToolBarEventImageView_LayerOnOff,
			ERavidToolBarEventImageView_GridOn,
			ERavidToolBarEventImageView_GridOff,
			ERavidToolBarEventImageView_GVColorOn,
			ERavidToolBarEventImageView_GVColorOff,
			ERavidToolBarEventImageView_GVOn,
			ERavidToolBarEventImageView_GVOff,
			ERavidToolBarEventImageView_StatusBarOn,
			ERavidToolBarEventImageView_StatusBarOff,
			ERavidToolBarEventImageView_ScrollBarOn,
			ERavidToolBarEventImageView_ScrollBarOff,
			ERavidToolBarEventImageView_InvalidateOn,
			ERavidToolBarEventImageView_InvalidateOff,
			ERavidToolBarEventImageView_AccuracySetting,
			ERavidToolBarEventImageView_Undo,
			ERavidToolBarEventImageView_Redo,
			ERavidToolBarEventImageView_Maximize,
			ERavidToolBarEventImageView_Restore,
			ERavidToolBarEventImageView_Count,
		};

		enum ERavidToolBarEventLogViewer
		{
			ERavidToolBarEventLogViewer_Maximize = 0,
			ERavidToolBarEventLogViewer_Restore,
			ERavidToolBarEventLogViewer_Close,
			ERavidToolBarEventLogViewer_Count,
		};

		enum ERavidToolBarImageProcessingTool
		{
			ERavidToolBarImageProcessingTool_Maximize = 0,
			ERavidToolBarImageProcessingTool_Restore,
			ERavidToolBarImageProcessingTool_Close,
			ERavidToolBarImageProcessingTool_Count,
		};

		enum ERavidToolBarEventGraphView
		{
			ERavidToolBarEventGraphView_Arrow = 6,
			ERavidToolBarEventGraphView_ZoomIn = 7,
			ERavidToolBarEventGraphView_ZoomOut = 8,
			ERavidToolBarEventGraphView_ZoomAll = 9,
			ERavidToolBarEventGraphView_Move = 10,
			ERavidToolBarEventGraphView_StatusBarOn = 19,
			ERavidToolBarEventGraphView_StatusBarOff = 20,
			ERavidToolBarEventGraphView_Maximize = 26,
			ERavidToolBarEventGraphView_Restore = 27,
		};

		enum ERavidToolBarEventThumbnailView
		{
			ERavidToolBarEventThumbnailView_Arrow = 6,
			ERavidToolBarEventThumbnailView_Move = 10,
			ERavidToolBarEventThumbnailView_Maximize = 26,
			ERavidToolBarEventThumbnailView_Restore = 27,
		};

		enum ERavidToolBarEventDialogView
		{
			ERavidToolBarEventDialogView_ImageOpen = 0,
			ERavidToolBarEventDialogView_ImageSave,
			ERavidToolBarEventDialogView_ImageClose,
			ERavidToolBarEventDialogView_Inspect,
			ERavidToolBarEventDialogView_Teaching,
			ERavidToolBarEventDialogView_EditTeaching,
			ERavidToolBarEventDialogView_Arrow,
			ERavidToolBarEventDialogView_ZoomIn,
			ERavidToolBarEventDialogView_ZoomOut,
			ERavidToolBarEventDialogView_ZoomAll,
			ERavidToolBarEventDialogView_Move,
			ERavidToolBarEventDialogView_Measure,
			ERavidToolBarEventDialogView_LayerOnOff,
			ERavidToolBarEventDialogView_GridOn,
			ERavidToolBarEventDialogView_GridOff,
			ERavidToolBarEventDialogView_GVColorOn,
			ERavidToolBarEventDialogView_GVColorOff,
			ERavidToolBarEventDialogView_GVOn,
			ERavidToolBarEventDialogView_GVOff,
			ERavidToolBarEventDialogView_StatusBarOn,
			ERavidToolBarEventDialogView_StatusBarOff,
			ERavidToolBarEventDialogView_ScrollBarOn,
			ERavidToolBarEventDialogView_ScrollBarOff,
			ERavidToolBarEventDialogView_AccuracySetting,
			ERavidToolBarEventDialogView_Maximize,
			ERavidToolBarEventDialogView_Restore,
			ERavidToolBarEventDialogView_Close,
			ERavidToolBarEventDialogView_Count,
		};
	}
}

typedef struct sRavidPopupMenuInfo
{
	CWnd* pWndParent;
	int nNumber;

	sRavidPopupMenuInfo()
	{
		pWndParent = nullptr;
		int nNumber = -1;
	}
}
SRavidPopupMenuInfo;

enum EViewObjectGripType
{
	EViewObjectGripType_None = 0,
	EViewObjectGripType_Left,
	EViewObjectGripType_Top,
	EViewObjectGripType_Right,
	EViewObjectGripType_Bottom,
	EViewObjectGripType_LeftTop,
	EViewObjectGripType_RightTop,
	EViewObjectGripType_RightBottom,
	EViewObjectGripType_LeftBottom,
	EViewObjectGripType_Center,
	EViewObjectGripType_Point,
	EViewObjectGripType_Rotate,
	EViewObjectGripType_Line,
	EViewObjectGripType_Body,
	EViewObjectGripType_InnerLeft,
	EViewObjectGripType_InnerTop,
	EViewObjectGripType_InnerRight,
	EViewObjectGripType_InnerBottom,
	EViewObjectGripType_InnerLeftTop,
	EViewObjectGripType_InnerRightTop,
	EViewObjectGripType_InnerRightBottom,
	EViewObjectGripType_InnerLeftBottom,
	EViewObjectGripType_OuterLeft,
	EViewObjectGripType_OuterTop,
	EViewObjectGripType_OuterRight,
	EViewObjectGripType_OuterBottom,
	EViewObjectGripType_OuterLeftTop,
	EViewObjectGripType_OuterRightTop,
	EViewObjectGripType_OuterRightBottom,
	EViewObjectGripType_OuterLeftBottom,
	EViewObjectGripType_PolygonLine,
	EViewObjectGripType_PolygonArc,
	EViewObjectGripType_PolygonArcPeak,
	EViewObjectGripType_CircleGaugeInner,
	EViewObjectGripType_CircleGaugeOuter,
	EViewObjectGripType_EllipseGaugeInner,
	EViewObjectGripType_EllipseGaugeOuter,
};

enum ETeachingObjectColor
{
	ETeachingObjectColor_NoSelectionNoGroup = RGB(0, 0, 255),
	ETeachingObjectColor_NoSelectionGroup = RGB(0, 192, 0),
	ETeachingObjectColor_NoSelectionNoGroupHighlight = RGB(255, 255, 0),
	ETeachingObjectColor_NoSelectionGroupHighlight = RGB(255, 255, 0),
	ETeachingObjectColor_Selection = RGB(0, 255, 0),
	ETeachingObjectColor_SelectionHighlight = RGB(0, 255, 0),
	ETeachingObjectColor_SelectionHighlightLine = RGB(0, 255, 255),
	ETeachingObjectColor_Invalid = RGB(255, 0, 0),
	ETeachingObjectColor_InvalidHighlight = RGB(255, 128, 0),
	ETeachingObjectColor_InvalidHighlightLine = RGB(255, 192, 0),
};

enum EMeasurementObjectColor
{
	EMeasurementObjectColor_NoSelectionTool = RGB(255, 255, 255),
	EMeasurementObjectColor_SelectionTool = RGB(0, 255, 0),
	EMeasurementObjectColor_HoverTool = RGB(255, 255, 0),
	EMeasurementObjectColor_InvalidNoSelectionTool = RGB(255, 0, 0),
	EMeasurementObjectColor_InvalidSelectionTool = RGB(255, 128, 0),
	EMeasurementObjectColor_InvalidHoverTool = RGB(255, 192, 0),

	EMeasurementObjectColor_NoSelectionGuideShape = RGB(255, 255, 255),
	EMeasurementObjectColor_SelectionGuideShape = RGB(0, 255, 255),
	EMeasurementObjectColor_HoverGuideShape = RGB(255, 255, 0),

	EMeasurementObjectColor_NoSelectionGuideDot = RGB(192, 192, 192),
	EMeasurementObjectColor_SelectionGuideDot = RGB(108, 255, 255),
	EMeasurementObjectColor_HoverGuideDot = RGB(255, 255, 0),

	EMeasurementObjectColor_NoSelectionGripPoint = RGB(192, 192, 192),
	EMeasurementObjectColor_SelectionGripPoint = RGB(255, 64, 128),
	EMeasurementObjectColor_HoverGripPoint = RGB(255, 255, 0),

	EMeasurementObjectColor_NoSelectionCenterPoint = RGB(192, 192, 192),
	EMeasurementObjectColor_SelectionCenterPoint = RGB(255, 255, 192),
	EMeasurementObjectColor_HoverCenterPoint = RGB(255, 255, 0),

	EMeasurementObjectColor_ValidGripLine = RGB(0, 255, 255),
	EMeasurementObjectColor_InvalidGripLine = RGB(255, 192, 0)
};

enum EViewObjectContextMenuType
{
	EViewObjectContextMenuType_None = 0,
	EViewObjectContextMenuType_SetTeachingVaildShape,
	EViewObjectContextMenuType_SetTeachingInvaildShape,
	EViewObjectContextMenuType_RemovePolygonPoint,
	EViewObjectContextMenuType_ConvertPolygonLineToArc,
	EViewObjectContextMenuType_ConvertPolygonArcToLine,
	EViewObjectContextMenuType_CurrentShapeCircle,
	EViewObjectContextMenuType_CurrentShapeEllipse,
	EViewObjectContextMenuType_CurrentShapeArcNoClose,
	EViewObjectContextMenuType_CurrentShapeArcEachOther,
	EViewObjectContextMenuType_CurrentShapeArcCenter,
	EViewObjectContextMenuType_Delete,
};

namespace Ravid
{
	namespace Framework
	{
		enum ERavidMeasurementType
		{
			ERavidMeasurementType_None = 0,
			ERavidMeasurementType_Distance,
			ERavidMeasurementType_Displacement,
			ERavidMeasurementType_DistanceCircle,
			ERavidMeasurementType_CircumcenterOfTriangle,
			ERavidMeasurementType_ParallelDistance,
			ERavidMeasurementType_Angle,
			ERavidMeasurementType_Azimuth,
			ERavidMeasurementType_Gradient,
			ERavidMeasurementType_PolygonArea,
		};
	}
}

#define RAVID_SHEET_TITLETIP_CLASSNAME _T("CRavidSheetTitleTip")

enum ERavidSheetCellStatus
{
	ERavidSheetCellStatus_Focused = 0x0001,
	ERavidSheetCellStatus_Selected = 0x0002,
	ERavidSheetCellStatus_DropHilited = 0x0004,
	ERavidSheetCellStatus_ReadOnly = 0x0008,
	ERavidSheetCellStatus_Fixed = 0x0010,
	ERavidSheetCellStatus_FixedRow = 0x0020,
	ERavidSheetCellStatus_FixedCol = 0x0040,
	ERavidSheetCellStatus_Modified = 0x0080,
};

enum ERavidSheetCellDataMask
{
	ERavidSheetCellDataMask_Text = LVIF_TEXT,
	ERavidSheetCellDataMask_Image = LVIF_IMAGE,
	ERavidSheetCellDataMask_Param = LVIF_PARAM,
	ERavidSheetCellDataMask_State = LVIF_STATE,
	ERavidSheetCellDataMask_BKCLR = (LVIF_STATE << 1),
	ERavidSheetCellDataMask_FGCLR = (LVIF_STATE << 2),
	ERavidSheetCellDataMask_Format = (LVIF_STATE << 3),
	ERavidSheetCellDataMask_Font = (LVIF_STATE << 4),
	ERavidSheetCellDataMask_Margin = (LVIF_STATE << 5),
	ERavidSheetCellDataMask_All = (ERavidSheetCellDataMask_Text | ERavidSheetCellDataMask_Image | ERavidSheetCellDataMask_Param
	| ERavidSheetCellDataMask_State | ERavidSheetCellDataMask_BKCLR | ERavidSheetCellDataMask_FGCLR
								   | ERavidSheetCellDataMask_Format | ERavidSheetCellDataMask_Font | ERavidSheetCellDataMask_Margin)
};


typedef struct sRavidSheetCellInfo
{
	__int64 nRow = 0;
	__int64 nCol = 0;
	UINT uiMask = 0;
	UINT     uiState = 0;
	DWORD    dwFormat = 0;
	int      nImage = 0;
	COLORREF clrBackground = 0;
	COLORREF clrForground = 0;
	LPARAM   lParam = 0;
	LOGFONT  lfFont;
	UINT     uiMargin = 0;
	CString  strText = _T("");
}
SRavidSheetCellInfo;

#pragma pack( push )
#pragma pack( 2 )
typedef struct
{
	BYTE	bWidth;               // Width of the image
	BYTE	bHeight;              // Height of the image (times 2)
	BYTE	bColorCount;          // Number of colors in image (0 if >=8bpp)
	BYTE	bReserved;            // Reserved
	WORD	wPlanes;              // Color Planes
	WORD	wBitCount;            // Bits per pixel
	DWORD	dwBytesInRes;         // how many bytes in this resource?
	WORD	nID;                  // the ID
} MEMICONDIRENTRY, *LPMEMICONDIRENTRY;

typedef struct
{
	WORD			idReserved;   // Reserved
	WORD			idType;       // resource type (1 for icons)
	WORD			idCount;      // how many images?
	MEMICONDIRENTRY	idEntries[1]; // the entries for each image
} MEMICONDIR, *LPMEMICONDIR;
#pragma pack( pop )

typedef struct
{
	UINT			Width, Height, Colors; // Width, Height and bpp
	LPBYTE			lpBits;                // ptr to DIB bits
	DWORD			dwNumBytes;            // how many bytes?
	LPBITMAPINFO	lpbi;                  // ptr to header
	LPBYTE			lpXOR;                 // ptr to XOR image bits
	LPBYTE			lpAND;                 // ptr to AND image bits
} ICONIMAGE, *LPICONIMAGE;

typedef struct
{
	BOOL		bHasChanged;                     // Has image changed?
	UINT		nNumImages;                      // How many images?
	ICONIMAGE	IconImages[1];                   // Image entries
} ICONRESOURCE, *LPICONRESOURCE;

typedef struct
{
	BYTE	bWidth;               // Width of the image
	BYTE	bHeight;              // Height of the image (times 2)
	BYTE	bColorCount;          // Number of colors in image (0 if >=8bpp)
	BYTE	bReserved;            // Reserved
	WORD	wPlanes;              // Color Planes
	WORD	wBitCount;            // Bits per pixel
	DWORD	dwBytesInRes;         // how many bytes in this resource?
	DWORD	dwImageOffset;        // where in the file is this image
} ICONDIRENTRY, *LPICONDIRENTRY;


enum ERavidFrameColorType
{
	ERavidFrameColorType_Background = 0,
	ERavidFrameColorType_EdgeNormal,
	ERavidFrameColorType_EdgeFocus,
	ERavidFrameColorType_Count,
};

enum ERavidCaptionColorType
{
	ERavidCaptionColorType_Background = 0,
	ERavidCaptionColorType_NonactivateBackground,
	ERavidCaptionColorType_TitleText,
	ERavidCaptionColorType_Button,
	ERavidCaptionColorType_ButtonHoverBackground,
	ERavidCaptionColorType_ButtonClickBackground,
	ERavidCaptionColorType_Count,
};

enum ERavidToolBarColorType
{
	ERavidToolBarColorType_Background = 0,
	ERavidToolBarColorType_TitleText,
	ERavidToolBarColorType_Edge,
	ERavidToolBarColorType_ButtonNormal,
	ERavidToolBarColorType_ButtonDisable,
	ERavidToolBarColorType_ButtonHoverBackground,
	ERavidToolBarColorType_ButtonClickBackground,
	ERavidToolBarColorType_ButtonCheckedBackground,
	ERavidToolBarColorType_ExtendButton,
	ERavidToolBarColorType_Count,
};

enum ERavidMenuColorType
{
	ERavidMenuColorType_Background = 0,
	ERavidMenuColorType_ItemNormal,
	ERavidMenuColorType_ItemHover,
	ERavidMenuColorType_ItemClick,
	ERavidMenuColorType_ItemTextNormal,
	ERavidMenuColorType_ItemTextDisable,
	ERavidMenuColorType_Count,
};

enum ERavidControlColorType
{
	ERavidControlColorType_Background = 0,
	ERavidControlColorType_ControlBackground,
	ERavidControlColorType_ControlText,
	ERavidControlColorType_Count,
};

enum ERavidSheetColorType
{
	ERavidSheetColorType_EnableText = 0,
	ERavidSheetColorType_DisableText,
	ERavidSheetColorType_TextBackground,
	ERavidSheetColorType_CategoryBackground,
	ERavidSheetColorType_DataBackground,
	ERavidSheetColorType_StaticBackground,
	ERavidSheetColorType_GridBackground,
	ERavidSheetColorType_Count,
};

typedef struct sWindowElement
{
	CWnd* pWndThis = nullptr;
	std::vector<sWindowElement> vctChildWnd;
}
SWindowElement;

enum EAdaptiveScaleAlign
{
	EAdaptiveScaleAlign_Center = 0,
	EAdaptiveScaleAlign_Left,
	EAdaptiveScaleAlign_LeftTop,
	EAdaptiveScaleAlign_Top,
	EAdaptiveScaleAlign_RightTop,
	EAdaptiveScaleAlign_Right,
	EAdaptiveScaleAlign_RightBottom,
	EAdaptiveScaleAlign_Bottom,
	EAdaptiveScaleAlign_LeftBottom,
};

namespace Ravid
{
	namespace Algorithms
	{
		class CRavidImage;
	}

	namespace Framework
	{
		class CRavidImageToolObject;
		class CRavidImageView;
		class CRavidImageToolBase;
		class CImageProcessingToolImageInfo;
		class CImageProcessingToolViewInfo;


		enum ERavidImageToolSelectedTreeType
		{
			ERavidImageToolSelectedTreeType_None = 0,
			ERavidImageToolSelectedTreeType_View,
			ERavidImageToolSelectedTreeType_Image,
			ERavidImageToolSelectedTreeType_ROI,
		};

		typedef struct sImageToolTreeInfo
		{
			ERavidImageToolSelectedTreeType eSelectedType = ERavidImageToolSelectedTreeType_None;

			CRavidImageView* pImageView = nullptr;
			Algorithms::CRavidImage* pImage = nullptr;
			CRavidImageToolObject* pImageToolObject = nullptr;

			CImageProcessingToolImageInfo* pImageInfo = nullptr;
			CImageProcessingToolViewInfo* pViewInfo = nullptr;

			HTREEITEM hViewTreeItem = nullptr;
			HTREEITEM hImageTreeItem = nullptr;

			HTREEITEM hTreeItem = nullptr;
		}
		SImageToolTreeInfo;
	}
}



enum ERavidInputBoxFieldType
{
	ERavidInputBoxFieldType_Edit = 0,
	ERavidInputBoxFieldType_Combo,
	ERavidInputBoxFieldType_Static,
};

typedef struct sRavidInputConditionInfo
{
	int nIndex = -1;
	CString strCheckData = _T("");
	ELogicalCondition eCondition = ELogicalCondition_Equal;
	int nTargetIndex = -1;

	bool bSetHide = false;
	bool bHide = false;
	bool bSetChangeFieldType = false;
	ERavidInputBoxFieldType eChangeFieldType = ERavidInputBoxFieldType_Edit;
	bool bSetChangeTitle = false;
	CString strChangeTitle = _T("");
	bool bSetChangeData = false;
	CString strChangeData = _T("");
	bool bSetChageComboList = false;
	CString strChangeComboList = _T("");
	bool bSetChangeBlindText = false;
	bool bChangeBlindText = false;
}
SRavidInputConditionInfo;

typedef struct sRavidInputDlgDataInfo
{
	CString strTitle = _T("");
	CString strData = _T("");
	ERavidInputBoxFieldType eType = ERavidInputBoxFieldType_Edit;
	bool bIsOptBlind = false;
	CString strOptComboList = _T("");
	bool bIsHide = false;
}
SRavidInputDlgDataInfo;

enum ERavidImageToolObjectColor
{
	ERavidImageToolObjectColor_NoSelectionNoGroup = RGB(0, 0, 255),
	ERavidImageToolObjectColor_NoSelectionGroup = RGB(0, 192, 0),
	ERavidImageToolObjectColor_NoSelectionNoGroupHighlight = RGB(255, 255, 0),
	ERavidImageToolObjectColor_NoSelectionGroupHighlight = RGB(255, 255, 0),
	ERavidImageToolObjectColor_Selection = RGB(0, 255, 0),
	ERavidImageToolObjectColor_SelectionHighlight = RGB(0, 255, 0),
	ERavidImageToolObjectColor_SelectionHighlightLine = RGB(0, 255, 255),
	ERavidImageToolObjectColor_Invalid = RGB(255, 0, 0),
	ERavidImageToolObjectColor_InvalidHighlight = RGB(255, 128, 0),
	ERavidImageToolObjectColor_InvalidHighlightLine = RGB(255, 192, 0),
};

enum ERavidImageToolObjectForGaugeColor
{
	ERavidImageToolObjectForGaugeColor_NoSelectionNoGroup = RGB(0, 0, 255),
	ERavidImageToolObjectForGaugeColor_NoSelectionGroup = RGB(0, 192, 0),
	ERavidImageToolObjectForGaugeColor_NoSelectionNoGroupHighlight = RGB(255, 255, 0),
	ERavidImageToolObjectForGaugeColor_NoSelectionGroupHighlight = RGB(255, 255, 0),
	ERavidImageToolObjectForGaugeColor_Selection = RGB(255, 64, 128),
	ERavidImageToolObjectForGaugeColor_SelectionHighlight = RGB(255, 64, 128),
	ERavidImageToolObjectForGaugeColor_SelectionHighlightLine = RGB(255, 255, 0),
	ERavidImageToolObjectForGaugeColor_Invalid = RGB(255, 0, 0),
	ERavidImageToolObjectForGaugeColor_InvalidHighlight = RGB(255, 128, 0),
	ERavidImageToolObjectForGaugeColor_InvalidHighlightLine = RGB(255, 192, 0),
	ERavidImageToolObjectForGaugeColor_GripPoint = RGB(255, 64, 128),
};

enum ERavidRavidTreeViewInputBoxType
{
	ERavidRavidTreeViewInputBoxType_None = 0,
	ERavidRavidTreeViewInputBoxType_Edit,
	ERavidRavidTreeViewInputBoxType_Static,
	ERavidRavidTreeViewInputBoxType_Combo,
	ERavidRavidTreeViewInputBoxType_Radio,
	ERavidRavidTreeViewInputBoxType_Check,
	ERavidRavidTreeViewInputBoxType_Spinner,
	ERavidRavidTreeViewInputBoxType_IPAddress,
	ERavidRavidTreeViewInputBoxType_Count,
};

static LPCTSTR GetRavidRavidTreeViewInputBoxTypeString(_In_ ERavidRavidTreeViewInputBoxType eROTIBT)
{
	static LPCTSTR lpsz[ERavidRavidTreeViewInputBoxType_Count] =
	{
		_T("NONE"),
		_T("EDIT"),
		_T("STATIC"),
		_T("COMBO"),
		_T("RADIO"),
		_T("CHECK"),
		_T("SPINNER"),
		_T("IPADDRESS")
	};

	LPCTSTR lpszReturn = nullptr;

	do
	{
		int nIdx = eROTIBT;

		if(nIdx < 0 || nIdx >= ERavidRavidTreeViewInputBoxType_Count)
			break;

		lpszReturn = lpsz[nIdx];
	}
	while(false);

	return lpszReturn;
}

enum ERavidRavidTreeViewColor
{
	ERavidRavidTreeViewColor_Category = RGB(250, 210, 140),
	ERavidRavidTreeViewColor_Data = RGB(195, 225, 255),
	ERavidRavidTreeViewColor_StaticBackground = RGB(222, 222, 222),
};

typedef struct sRavidImageToolParamInfo
{
	int nDepth = 0;
	CString strTitle = _T("");
	CString strData = _T("");
	CString strDescription = _T("");
	ERavidRavidTreeViewInputBoxType eType = ERavidRavidTreeViewInputBoxType_None;

	CString strExtraData = _T("");
}
SRavidImageToolParamInfo;

enum ERavidImageToolObjectType
{
	ERavidImageToolObjectType_None = 0,
	ERavidImageToolObjectType_RotatePivot,
	ERavidImageToolObjectType_PointGauge,
	ERavidImageToolObjectType_LineGauge,
	ERavidImageToolObjectType_RectangleGauge,
	ERavidImageToolObjectType_QuadrangleGauge,
	ERavidImageToolObjectType_CircleGauge,
	ERavidImageToolObjectType_EllipseGauge,
};

enum ERavidImageToolOperateParameter
{
	ERavidImageToolOperateParameter_None = 0,
	ERavidImageToolOperateParameter_PivotPointX,
	ERavidImageToolOperateParameter_PivotPointY,
	ERavidImageToolOperateParameter_CenterPointX,
	ERavidImageToolOperateParameter_CenterPointY,
	ERavidImageToolOperateParameter_Tolerance,
	ERavidImageToolOperateParameter_Length,
	ERavidImageToolOperateParameter_Width,
	ERavidImageToolOperateParameter_Height,
	ERavidImageToolOperateParameter_Radius,
	ERavidImageToolOperateParameter_Radius1,
	ERavidImageToolOperateParameter_Radius2,
	ERavidImageToolOperateParameter_StartDegree,
	ERavidImageToolOperateParameter_DeltaDegree,
	ERavidImageToolOperateParameter_Angle,
	ERavidImageToolOperateParameter_DeltaX,
	ERavidImageToolOperateParameter_DeltaY,
	ERavidImageToolOperateParameter_Point0X,
	ERavidImageToolOperateParameter_Point0Y,
	ERavidImageToolOperateParameter_Point1X,
	ERavidImageToolOperateParameter_Point1Y,
	ERavidImageToolOperateParameter_Point2X,
	ERavidImageToolOperateParameter_Point2Y,
	ERavidImageToolOperateParameter_Point3X,
	ERavidImageToolOperateParameter_Point3Y,
	ERavidImageToolOperateParameter_Count,
};

enum ERavidImageViewPopupMenu
{
	ERavidImageViewPopupMenu_ClearViewObject = 0,
	ERavidImageViewPopupMenu_ImageOpen,
	ERavidImageViewPopupMenu_ImageSave,
	ERavidImageViewPopupMenu_ImageClose,
	ERavidImageViewPopupMenu_CreateImage,
	ERavidImageViewPopupMenu_CreateROI,
	ERavidImageViewPopupMenu_DeleteROI,
	ERavidImageViewPopupMenu_Rename,
	ERavidImageViewPopupMenu_Inspect,
	ERavidImageViewPopupMenu_Teaching,
	ERavidImageViewPopupMenu_EditTeaching,
	ERavidImageViewPopupMenu_Arrow,
	ERavidImageViewPopupMenu_ZoomIn,
	ERavidImageViewPopupMenu_ZoomOut,
	ERavidImageViewPopupMenu_ZoomAll,
	ERavidImageViewPopupMenu_Panning,
	ERavidImageViewPopupMenu_Measure,
	ERavidImageViewPopupMenu_Layer,
	ERavidImageViewPopupMenu_GridOn,
	ERavidImageViewPopupMenu_GridOff,
	ERavidImageViewPopupMenu_SpectrumOn,
	ERavidImageViewPopupMenu_SpectrumOff,
	ERavidImageViewPopupMenu_AccuracySetting,
	ERavidImageViewPopupMenu_StatusBarOn,
	ERavidImageViewPopupMenu_StatusBarOff,
	ERavidImageViewPopupMenu_PixelValueOn,
	ERavidImageViewPopupMenu_PixelValueOff,
	ERavidImageViewPopupMenu_Maximize,
	ERavidImageViewPopupMenu_Restore,
	ERavidImageViewPopupMenu_Script,
	ERavidImageViewPopupMenu_BoundaryRect,
	ERavidImageViewPopupMenu_ModifyGeometry,
	ERavidImageViewPopupMenu_BringToUp,
	ERavidImageViewPopupMenu_BringToBottom,
	ERavidImageViewPopupMenu_TypeChange,
	ERavidImageViewPopupMenu_OK,
	ERavidImageViewPopupMenu_Cancel,
	ERavidImageViewPopupMenu_Convert,
	ERavidImageViewPopupMenu_RemoveToPoint,
	ERavidImageViewPopupMenu_CopyTo,
	ERavidImageViewPopupMenu_CopyFrom,
	ERavidImageViewPopupMenu_CopyImage,
	ERavidImageViewPopupMenu_CopyImageFromThe,
	ERavidImageViewPopupMenu_CopyImageToThe,
	ERavidImageViewPopupMenu_TransferTo,
	ERavidImageViewPopupMenu_GeometryCopy,
	ERavidImageViewPopupMenu_TransferImage,
	ERavidImageViewPopupMenu_BringImage,
	ERavidImageViewPopupMenu_Operation,
	ERavidImageViewPopupMenu_BringGeometry,
	ERavidImageViewPopupMenu_Area,
	ERavidImageViewPopupMenu_AngleMeasurement,
	ERavidImageViewPopupMenu_AngleAzimuth,
	ERavidImageViewPopupMenu_AngleGradient,
	ERavidImageViewPopupMenu_Distance,
	ERavidImageViewPopupMenu_DistanceCircle,
	ERavidImageViewPopupMenu_DistanceCircumcenter,
	ERavidImageViewPopupMenu_DistanceParallel,
	ERavidImageViewPopupMenu_Displacement,
	ERavidImageViewPopupMenu_ShowROIStorage,
	ERavidImageViewPopupMenu_SaveROIStorage,
	ERavidImageViewPopupMenu_ScrollBarOff,
	ERavidImageViewPopupMenu_ScrollBarOn,
	ERavidImageViewPopupMenu_InvalidateOn,
	ERavidImageViewPopupMenu_InvalidateOff,
	ERavidImageViewPopupMenu_Count,
};

enum ERavidImageViewPopupMenuID
{
	ERavidImageViewPopupMenuID_ClearViewObject = 1024,
	ERavidImageViewPopupMenuID_ImageOpen,
	ERavidImageViewPopupMenuID_ImageSave,
	ERavidImageViewPopupMenuID_ImageClose,
	ERavidImageViewPopupMenuID_CreateImage,
	ERavidImageViewPopupMenuID_CreateROI,
	ERavidImageViewPopupMenuID_RenameImage,
	ERavidImageViewPopupMenuID_Inspect = 2000,
	ERavidImageViewPopupMenuID_Teaching = 3000,
	ERavidImageViewPopupMenuID_EditTeaching = 4000,
	ERavidImageViewPopupMenuID_Arrow = 5000,
	ERavidImageViewPopupMenuID_ZoomIn,
	ERavidImageViewPopupMenuID_ZoomOut,
	ERavidImageViewPopupMenuID_ZoomAll,
	ERavidImageViewPopupMenuID_Panning,
	ERavidImageViewPopupMenuID_Distance,
	ERavidImageViewPopupMenuID_Displacement,
	ERavidImageViewPopupMenuID_DistanceCircle,
	ERavidImageViewPopupMenuID_CircumcenterOfTriangle,
	ERavidImageViewPopupMenuID_ParallelDistance,
	ERavidImageViewPopupMenuID_AngleMeasurement,
	ERavidImageViewPopupMenuID_Azimuth,
	ERavidImageViewPopupMenuID_Gradient,
	ERavidImageViewPopupMenuID_PolygonArea,
	ERavidImageViewPopupMenuID_LayerOnOff = 6000,
	ERavidImageViewPopupMenuID_GridOn = 7000,
	ERavidImageViewPopupMenuID_GridOff,
	ERavidImageViewPopupMenuID_SpectrumOn,
	ERavidImageViewPopupMenuID_SpectrumOff,
	ERavidImageViewPopupMenuID_PixelValueOn,
	ERavidImageViewPopupMenuID_PixelValueOff,
	ERavidImageViewPopupMenuID_StatusBarOn,
	ERavidImageViewPopupMenuID_StatusBarOff,
	ERavidImageViewPopupMenuID_AccuracySetting,
	ERavidImageViewPopupMenuID_Maximize,
	ERavidImageViewPopupMenuID_Restore,
	ERavidImageViewPopupMenuID_IPTCopyFromProject,
	ERavidImageViewPopupMenuID_ProjectCopyFromProject,
	ERavidImageViewPopupMenuID_CopyImageToTheIPT,
	ERavidImageViewPopupMenuID_CopyImageToTheProject,
	ERavidImageViewPopupMenuID_CopyImageFromTheIPT,
	ERavidImageViewPopupMenuID_CopyImageFromTheProject,
	ERavidImageViewPopupMenuID_IPTTransferImageIPT,
	ERavidImageViewPopupMenuID_IPTTransferImageProject,
	ERavidImageViewPopupMenuID_ProjectTransferImageIPT,
	ERavidImageViewPopupMenuID_ProjectTransferImageProject,
	ERavidImageViewPopupMenuID_ProjectBringImageIPT,
	ERavidImageViewPopupMenuID_ProjectBringImageProject,
	ERavidImageViewPopupMenuID_IPTBringImageProject,
	ERavidImageViewPopupMenuID_IPTBringImageIPT,
	ERavidImageViewPopupMenuID_IPTCopyFromIPT,
	ERavidImageViewPopupMenuID_ProjectCopyFromIPT,
	ERavidImageViewPopupMenuID_IPTBringFromIPT,
	ERavidImageViewPopupMenuID_IPTBringFromProject,
	ERavidImageViewPopupMenuID_ProjectBringFromProject,
	ERavidImageViewPopupMenuID_ProjectBringFromIPT,
	ERavidImageViewPopupMenuID_ShowROIStorage,
	ERavidImageViewPopupMenuID_ScrollBarOn,
	ERavidImageViewPopupMenuID_ScrollBarOff,
	ERavidImageViewPopupMenuID_InvalidateOn,
	ERavidImageViewPopupMenuID_InvalidateOff,
};

enum ERavidGraphViewPopupMenuID
{
	ERavidGraphViewPopupMenuID_ShowTitle = 1024,
	ERavidGraphViewPopupMenuID_ShowScaleX,
	ERavidGraphViewPopupMenuID_ShowScaleY,
	ERavidGraphViewPopupMenuID_ShowUnitX,
	ERavidGraphViewPopupMenuID_ShowUnitY,
	ERavidGraphViewPopupMenuID_ShowGridX,
	ERavidGraphViewPopupMenuID_ShowGridY,
	ERavidGraphViewPopupMenuID_ShowCrossHair,
	ERavidGraphViewPopupMenuID_ShowToolTip,
	ERavidGraphViewPopupMenuID_ShowHighLightX,
	ERavidGraphViewPopupMenuID_ShowHighLightY,
	ERavidGraphViewPopupMenuID_ShowHighLightXY,
	ERavidGraphViewPopupMenuID_ShowIndicatorX,
	ERavidGraphViewPopupMenuID_ShowIndicatorY,
	ERavidGraphViewPopupMenuID_ShowPlotPoint,
	ERavidGraphViewPopupMenuID_ShowLine,
	ERavidGraphViewPopupMenuID_SetEquation,
	ERavidGraphViewPopupMenuID_ClearEquation,
	ERavidGraphViewPopupMenuID_AddLeastSquaresData,
	ERavidGraphViewPopupMenuID_RemoveLastLeastSquaresData,
	ERavidGraphViewPopupMenuID_ClearLeastSquaresData,
	ERavidGraphViewPopupMenuID_ShowLeastSquares,
	ERavidGraphViewPopupMenuID_HideLeastSquares,
};

enum ERavidImageViewCursorMode
{
	ERavidImageViewCursorMode_Arrow = 0,
	ERavidImageViewCursorMode_ZoomIn,
	ERavidImageViewCursorMode_ZoomOut,
	ERavidImageViewCursorMode_ZoomAll,
	ERavidImageViewCursorMode_Panning,
	ERavidImageViewCursorMode_Count,
};

enum ERavidImageViewMessage
{
	ERavidImageViewMessage_UpdateToolbarStatus = WM_USER + 2000,
	ERavidImageViewMessage_Maximize,
	ERavidImageViewMessage_Restore,
	ERavidImageViewMessage_SetTitle,
	ERavidImageViewMessage_UpdateCursorModeButton,
	ERavidImageViewMessage_UpdateScrollBar,
	ERavidImageViewMessage_EnableScrollBar,
};	
enum ERegisterViewType
{
	ERegisterViewType_Image = 0,
	ERegisterViewType_Form,
	ERegisterViewType_Graph,
	ERegisterViewType_3D,
	ERegisterViewType_3DViewer,
	ERegisterViewType_Count,
	ERegisterViewType_None,
};

typedef struct sEasyConfigurationView
{
	CRuntimeClass* pRCView = nullptr;
	CRavidObject* pObjectView = nullptr;

	CRuntimeClass* pRCFrame = nullptr;
	CRavidObject* pObjectFrame = nullptr;
}
SEasyConfigurationView;

typedef struct sRegisterTeachingMenuElement
{
	ERavidGeometryShapeType eShapeType = ERavidGeometryShapeType_Unknown;
	ERavidGeometryTemplateType eTemplateType = ERavidGeometryTemplateType_Unknown;
	int nCount = 1;
	bool bAbleRotate = true;

	HTREEITEM hItem = nullptr;
}
SRegisterTeachingMenuElement;

typedef struct sRegisterTeachingMenuInfo
{
	CString strTitle = _T("");

	HTREEITEM hItem = nullptr;

	std::vector<SRegisterTeachingMenuElement> vctElement;
}
SRegisterTeachingMenuInfo;

typedef struct sRegisterViewInfo
{
	int nSequence = 0;
	CString strViewClass = _T("");
	CString strViewName = _T("");

	ERegisterViewType eType = ERegisterViewType_None;
	CRavidObject* pObjectView = nullptr;
	HTREEITEM hItem = nullptr;

	std::vector<CString> vctInspectionMenu;
	std::vector<SRegisterTeachingMenuInfo> vctTeachingMenuInfo;
}
SRegisterViewInfo;

typedef struct sConfigurationElementInfo
{
	int nSequence = 0;

	double dblRatioLeft = 0.;
	double dblRatioTop = 0.;
	double dblRatioRight = 20.;
	double dblRatioBottom = 20.;

	SRegisterViewInfo* pRegisterView = nullptr;

	HTREEITEM hItem = nullptr;

	CRect crPosition = CRect(0, 0, 0, 0);
	CRect crCoordinate = CRect(0, 0, 0, 0);
}
SConfigurationElementInfo;

typedef struct sRegisterMFCToolBarImagesInfo
{
	HINSTANCE hResourceHandle = nullptr;
	UINT uiID = 0;
	CMFCToolBarImages* pMFCToolBarImages = nullptr;
}
SRegisterMFCToolBarImagesInfo;

typedef struct sViewConfiguration
{
	sViewConfiguration(long Number = 0, CString Title = _T(""), std::vector<SConfigurationElementInfo> CEI = {}) :
		nNumber { Number }, strTitle { Title }, vctCEI { CEI } { };

	long nNumber = -1;
	CString strTitle = _T("");

	std::vector<SConfigurationElementInfo> vctCEI;
}
SViewConfiguration;

enum ERavidImageProcessingToolROIType
{
	ERavidImageProcessingToolROIType_Rect = 0,
	ERavidImageProcessingToolROIType_Quadrangle,
	ERavidImageProcessingToolROIType_Circle,
	ERavidImageProcessingToolROIType_Ellipse,
	ERavidImageProcessingToolROIType_Polygon,
	ERavidImageProcessingToolROIType_Count
};

enum ERavidImageProcessingToolROITemplateType
{
	ERavidImageProcessingToolROITemplateType_Int = 0,
	ERavidImageProcessingToolROITemplateType_LongLong,
	ERavidImageProcessingToolROITemplateType_Float,
	ERavidImageProcessingToolROITemplateType_Double,
	ERavidImageProcessingToolROITemplateType_Count
};

enum ERavidImageViewExtraDataType
{
	ERavidImageViewExtraDataType_AccuracyX = 0,
	ERavidImageViewExtraDataType_AccuracyY,
	ERavidImageViewExtraDataType_Count,
};

enum ERavidGraphViewPlotType
{
	ERavidGraphViewPlotType_Dot = 0,
	ERavidGraphViewPlotType_Line,
	ERavidGraphViewPlotType_DotWithLine,
};

enum EUIManagerConfigurationDataType
{
	EUIManagerConfigurationDataType_LayerDrawingMethod = 0,
	EUIManagerConfigurationDataType_Count,
};

enum ERavidImageToolAvailableLevel
{
	ERavidImageToolAvailableLevel_None = 0,
	ERavidImageToolAvailableLevel_View,
	ERavidImageToolAvailableLevel_Image,
	ERavidImageToolAvailableLevel_ROI,
};

enum ERavidNotificationType
{
	ERavidNotificationType_Information = 0,
	ERavidNotificationType_Warning,
	ERavidNotificationType_Error,
};

enum ERavidModalType
{
	ERavidModalType_Modal = 0,
	ERavidModalType_Modeless,
};

enum ERavidNotificationButtonType
{
	ERavidNotificationButtonType_Ok = 0,
	ERavidNotificationButtonType_AllOk,
};

enum ERavidFrameworkMessage
{
	ERavidFrameworkMessage_RunNotification = WM_USER + 1,
	ERavidFrameworkMessage_ClearNotification,
};

enum ERavidConfigurationTitle
{
	ERavidConfigurationTitle_Ok = 0,
	ERavidConfigurationTitle_UnkownErr,
	ERavidConfigurationTitle_Empty,
	ERavidConfigurationTitle_Same,
};

enum ERavidAccuracyUnit
{
	ERavidAccuracyUnit_nm = 0,
	ERavidAccuracyUnit_um,
	ERavidAccuracyUnit_mm,
	ERavidAccuracyUnit_cm,
	ERavidAccuracyUnit_m,
};


typedef struct sRavidDrawingStruct
{
	long long m_llShapeGroupIndex; // -1 이면 개별 객체입니다. 이외의 숫자는 그룹화 처리 됩니다.
	// 예시 : 사각형, 폴리곤 객체는 하나의 인덱스를 공유합니다.
	// 타원, 원, 선, 점은 개별 객체입니다.

	bool m_IsGetExclusiveROI;
	bool m_IsExclusive;

	ERavidGeometryShapeType m_eType;

	double m_fROIL;
	double m_fROIT;
	double m_fROIR;
	double m_fROIB;

	//Point, Line, Rect, Quad param
	double m_fptX1;
	double m_fptY1;
	//Line
	double m_fptX2;
	double m_fptY2;
	//Rect, Quad
	double m_fptX3;
	double m_fptY3;
	double m_fptX4;
	double m_fptY4;

	//Circle param
	double m_frpCirCenterX;
	double m_frpCirCenterY;
	double m_fCirRadius;
	double m_fCirAngle;
	double m_fCirStartDeg;
	double m_fCirDeltaDeg;

	//Ellipse param
	double m_frpEllCenterX;
	double m_frpEllCenterY;
	double m_fEllRadius1;
	double m_fEllRadius2;
	double m_fEllAngle;
	double m_fEllStartDeg;
	double m_fEllDeltaDeg;

	//Circle, Ellipse param
	ERavidArcClosingMethod m_eMethod;

	double m_fptStartX;
	double m_fptStartY;
	double m_fptEndX;
	double m_fptEndY;
	double m_fptCenterX;
	double m_fptCenterY;

	double m_fr1;
	double m_fr2;
	double m_fstart;
	double m_fdelta;

	double m_fangle;

	int m_nPolyType;

	// 이외 스타일 정보
	ERavidImageViewPenStyle m_ePenStyle;
	int m_nWidth;

	COLORREF m_crLineColor;
	double m_crLineR;
	double m_crLineG;
	double m_crLineB;

	COLORREF m_crFillColor;
	double m_crFillR;
	double m_crFillG;
	double m_crFillB;

	double m_fLineColorAlphaBlendingRatio;
	double m_fFillColorAlphaBlendingRatio;
}
SRavidDrawingStruct, *LPRavidDrawingStruct;

enum ERavidTableEventModelManager
{
	ERavidTableEventModelManager_Number = 0,
	ERavidTableEventModelManager_Name,
	ERavidTableEventModelManager_Description,
	ERavidTableEventModelManager_Count,
};

typedef struct sRavidFontInfo
{
	LONG m_Weight;
	LONG m_Height;
	CString m_FaceName;
	sRavidFontInfo()
	{
		m_Weight = FW_BOLD;
		m_Height = 17;
		m_FaceName = _T("Tahoma");
	}

	sRavidFontInfo(LONG weight, LONG height, CString facename)
	{
		m_Weight = weight;
		m_Height = height;
		m_FaceName = facename;
	}
}SRavidFontInfo;