
#ifndef RT_FONTSEL_GLOBALS
#define RT_FONTSEL_GLOBALS

// Added Headers
#include "CommonRes.h"
#include <atlconv.h>

// Definitions
#define DX_BITMAP        20
#define DY_BITMAP        12
#define DSx				0x00660046L
#define DSna			0x00220326L
#ifndef _countof
	#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif
AFX_STATIC_DATA int _afxTTDefaults[] = { 8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 26, 28, 36, 48, 72};

// Stores all the information about a font
typedef struct RT_FONTSEL_OBJECT
{
	CString strName;
	CY  cySize;
	BOOL bBold;
	BOOL bItalic;
	BOOL bUnderline;
	BOOL bStrikethrough;
	short sWeight;
} FONTOBJECT;

// Merge objects are used when trying to consolidate multiple font properties.
// If the characteristics of these multiple properties differ then this is
// represented in the merge object.
typedef struct RT_FONTSEL_MERGEOBJECT
{
	BOOL bNameOK;
	BOOL bSizeOK;
	BOOL bStyleOK;
	BOOL bUnderlineOK;
	BOOL bStrikethroughOK;
} MERGEOBJECT;

BOOL _AfxStringFromCy(CString& str, CY& cy);
BOOL _AfxCyFromString(CY& cy, LPCTSTR psz);
void _AfxDrawMaskedBitmap(CDC* pDC, CBitmap* pbmp, CBitmap* pbmpMask, int x, int y, int cx, int cy);
void _AfxInitMaskFromBitmap(CBitmap* pbmp, CBitmap* pbmpMask);

#endif // !RT_FONTSEL_GLOBALS