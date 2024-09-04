#include "stdafx.h"
#include "resource.h"

#include "DeviceGoogolGts.h"

#include "../RavidCore/RavidRect.h"
#include "../RavidCore/RavidImage.h"
#include "../RavidCore/RavidDatabase.h"
#include "../RavidCore/RavidRecordset.h"
#include "../RavidCore/PerformanceCounter.h"

#include "LogManager.h"
#include "DeviceManager.h"
#include "LibraryManager.h"
#include "SequenceManager.h"
#include "AuthorityManager.h"
#include "FrameworkManager.h"
#include "EventHandlerManager.h"
#include "MultiLanguageManager.h"

#include "RavidTreeView.h"
#include "MessageBase.h"
#include "RavidLedCtrl.h"
#include "RavidImageView.h"

#ifndef _WIN64
#include "../Libraries/Includes/Googol/GTS/32bit/multi/gts.h"
#else
#include "../Libraries/Includes/Googol/GTS/64bit/multi/gts.h"
#endif

using namespace Ravid;
using namespace Ravid::Device;
using namespace Ravid::Database;
using namespace Ravid::Framework;
using namespace Ravid::Algorithms;
using namespace Ravid::Miscellaneous;

enum
{
	GTS_EXTIO_PORT = 16,
	GTS_EXTIO_MODULES = 16,
	GTS_DIO_OUT_PORT = 16,
	GTS_DIO_IN_PORT = 16,
	GTS_DIO_EXTOUT_PORT = 256,
	GTS_DIO_EXTIN_PORT = 256,
	GTS_MOTION_MAX = 8,
};

IMPLEMENT_DYNAMIC(CDeviceGoogolGTS, CDeviceMotion)

BEGIN_MESSAGE_MAP(CDeviceGoogolGTS, CDeviceMotion)
	ON_COMMAND_RANGE(EDeviceAddEnumeratedControlID_OutputLedStart, EDeviceAddEnumeratedControlID_OutputLedEnd, OnBnClickedOutLed)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static LPCTSTR g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Count] =
{
	_T("DeviceID"),
	_T("Initialize type"),
	_T("Load file path"),
	_T("Load Ext file path"),
	_T("Motion setting"),
	_T("Axis1 Band"),
	_T("Axis1 Time"),
	_T("Axis1 AbruptStop"),
	_T("Axis1 EStop"),
	_T("Axis2 Band"),
	_T("Axis2 Time"),
	_T("Axis2 AbruptStop"),
	_T("Axis2 EStop"),
	_T("Axis3 Band"),
	_T("Axis3 Time"),
	_T("Axis3 AbruptStop"),
	_T("Axis3 EStop"),
	_T("Axis4 Band"),
	_T("Axis4 Time"),
	_T("Axis4 AbruptStop"),
	_T("Axis4 EStop"),
	_T("Axis5 Band"),
	_T("Axis5 Time"),
	_T("Axis5 AbruptStop"),
	_T("Axis5 EStop"),
	_T("Axis6 Band"),
	_T("Axis6 Time"),
	_T("Axis6 AbruptStop"),
	_T("Axis6 EStop"),
	_T("Axis7 Band"),
	_T("Axis7 Time"),
	_T("Axis7 AbruptStop"),
	_T("Axis7 EStop"),
	_T("Axis8 Band"),
	_T("Axis8 Time"),
	_T("Axis8 AbruptStop"),
	_T("Axis8 EStop"),
	_T("Home"),
	_T("Axis1 DirAndPos"),
	_T("Axis1 Velocity"),
	_T("Axis1 Accel"),
	_T("Axis1 Offset"),
	_T("Axis2 DirAndPos"),
	_T("Axis2 Velocity"),
	_T("Axis2 Accel"),
	_T("Axis2 Offset"),
	_T("Axis3 DirAndPos"),
	_T("Axis3 Velocity"),
	_T("Axis3 Accel"),
	_T("Axis3 Offset"),
	_T("Axis4 DirAndPos"),
	_T("Axis4 Velocity"),
	_T("Axis4 Accel"),
	_T("Axis4 Offset"),
	_T("Axis5 DirAndPos"),
	_T("Axis5 Velocity"),
	_T("Axis5 Accel"),
	_T("Axis5 Offset"),
	_T("Axis6 DirAndPos"),
	_T("Axis6 Velocity"),
	_T("Axis6 Accel"),
	_T("Axis6 Offset"),
	_T("Axis7 DirAndPos"),
	_T("Axis7 Velocity"),
	_T("Axis7 Accel"),
	_T("Axis7 Offset"),
	_T("Axis8 DirAndPos"),
	_T("Axis8 Velocity"),
	_T("Axis8 Accel"),
	_T("Axis8 Offset"),
	_T("Input status"),
	_T("Use Input No. 0"),
	_T("Use Input No. 1"),
	_T("Use Input No. 2"),
	_T("Use Input No. 3"),
	_T("Use Input No. 4"),
	_T("Use Input No. 5"),
	_T("Use Input No. 6"),
	_T("Use Input No. 7"),
	_T("Use Input No. 8"),
	_T("Use Input No. 9"),
	_T("Use Input No. 10"),
	_T("Use Input No. 11"),
	_T("Use Input No. 12"),
	_T("Use Input No. 13"),
	_T("Use Input No. 14"),
	_T("Use Input No. 15"),
	_T("Output status"),
	_T("Use Output No. 0"),
	_T("Use Output No. 1"),
	_T("Use Output No. 2"),
	_T("Use Output No. 3"),
	_T("Use Output No. 4"),
	_T("Use Output No. 5"),
	_T("Use Output No. 6"),
	_T("Use Output No. 7"),
	_T("Use Output No. 8"),
	_T("Use Output No. 9"),
	_T("Use Output No. 10"),
	_T("Use Output No. 11"),
	_T("Use Output No. 12"),
	_T("Use Output No. 13"),
	_T("Use Output No. 14"),
	_T("Use Output No. 15"),
	_T("Input Name"),
	_T("Input No. 0 Name"),
	_T("Input No. 1 Name"),
	_T("Input No. 2 Name"),
	_T("Input No. 3 Name"),
	_T("Input No. 4 Name"),
	_T("Input No. 5 Name"),
	_T("Input No. 6 Name"),
	_T("Input No. 7 Name"),
	_T("Input No. 8 Name"),
	_T("Input No. 9 Name"),
	_T("Input No. 10 Name"),
	_T("Input No. 11 Name"),
	_T("Input No. 12 Name"),
	_T("Input No. 13 Name"),
	_T("Input No. 14 Name"),
	_T("Input No. 15 Name"),
	_T("Output Name"),
	_T("Output No. 0 Name"),
	_T("Output No. 1 Name"),
	_T("Output No. 2 Name"),
	_T("Output No. 3 Name"),
	_T("Output No. 4 Name"),
	_T("Output No. 5 Name"),
	_T("Output No. 6 Name"),
	_T("Output No. 7 Name"),
	_T("Output No. 8 Name"),
	_T("Output No. 9 Name"),
	_T("Output No. 10 Name"),
	_T("Output No. 11 Name"),
	_T("Output No. 12 Name"),
	_T("Output No. 13 Name"),
	_T("Output No. 14 Name"),
	_T("Output No. 15 Name"),
	_T("External IO status"),
	_T("Using Ext 1"),
	_T("Using Ext 2"),
	_T("Using Ext 3"),
	_T("Using Ext 4"),
	_T("Using Ext 5"),
	_T("Using Ext 6"),
	_T("Using Ext 7"),
	_T("Using Ext 8"),
	_T("Using Ext 9"),
	_T("Using Ext 10"),
	_T("Using Ext 11"),
	_T("Using Ext 12"),
	_T("Using Ext 13"),
	_T("Using Ext 14"),
	_T("Using Ext 15"),
	_T("Using Ext 16"),
	_T("[Ext 1] Input Name"),
	_T("[Ext 1] Input No. 0 Name"),
	_T("[Ext 1] Input No. 1 Name"),
	_T("[Ext 1] Input No. 2 Name"),
	_T("[Ext 1] Input No. 3 Name"),
	_T("[Ext 1] Input No. 4 Name"),
	_T("[Ext 1] Input No. 5 Name"),
	_T("[Ext 1] Input No. 6 Name"),
	_T("[Ext 1] Input No. 7 Name"),
	_T("[Ext 1] Input No. 8 Name"),
	_T("[Ext 1] Input No. 9 Name"),
	_T("[Ext 1] Input No. 10 Name"),
	_T("[Ext 1] Input No. 11 Name"),
	_T("[Ext 1] Input No. 12 Name"),
	_T("[Ext 1] Input No. 13 Name"),
	_T("[Ext 1] Input No. 14 Name"),
	_T("[Ext 1] Input No. 15 Name"),
	_T("[Ext 2] Input Name"),
	_T("[Ext 2] Input No. 0 Name"),
	_T("[Ext 2] Input No. 1 Name"),
	_T("[Ext 2] Input No. 2 Name"),
	_T("[Ext 2] Input No. 3 Name"),
	_T("[Ext 2] Input No. 4 Name"),
	_T("[Ext 2] Input No. 5 Name"),
	_T("[Ext 2] Input No. 6 Name"),
	_T("[Ext 2] Input No. 7 Name"),
	_T("[Ext 2] Input No. 8 Name"),
	_T("[Ext 2] Input No. 9 Name"),
	_T("[Ext 2] Input No. 10 Name"),
	_T("[Ext 2] Input No. 11 Name"),
	_T("[Ext 2] Input No. 12 Name"),
	_T("[Ext 2] Input No. 13 Name"),
	_T("[Ext 2] Input No. 14 Name"),
	_T("[Ext 2] Input No. 15 Name"),
	_T("[Ext 3] Input Name"),
	_T("[Ext 3] Input No. 0 Name"),
	_T("[Ext 3] Input No. 1 Name"),
	_T("[Ext 3] Input No. 2 Name"),
	_T("[Ext 3] Input No. 3 Name"),
	_T("[Ext 3] Input No. 4 Name"),
	_T("[Ext 3] Input No. 5 Name"),
	_T("[Ext 3] Input No. 6 Name"),
	_T("[Ext 3] Input No. 7 Name"),
	_T("[Ext 3] Input No. 8 Name"),
	_T("[Ext 3] Input No. 9 Name"),
	_T("[Ext 3] Input No. 10 Name"),
	_T("[Ext 3] Input No. 11 Name"),
	_T("[Ext 3] Input No. 12 Name"),
	_T("[Ext 3] Input No. 13 Name"),
	_T("[Ext 3] Input No. 14 Name"),
	_T("[Ext 3] Input No. 15 Name"),
	_T("[Ext 4] Input Name"),
	_T("[Ext 4] Input No. 0 Name"),
	_T("[Ext 4] Input No. 1 Name"),
	_T("[Ext 4] Input No. 2 Name"),
	_T("[Ext 4] Input No. 3 Name"),
	_T("[Ext 4] Input No. 4 Name"),
	_T("[Ext 4] Input No. 5 Name"),
	_T("[Ext 4] Input No. 6 Name"),
	_T("[Ext 4] Input No. 7 Name"),
	_T("[Ext 4] Input No. 8 Name"),
	_T("[Ext 4] Input No. 9 Name"),
	_T("[Ext 4] Input No. 10 Name"),
	_T("[Ext 4] Input No. 11 Name"),
	_T("[Ext 4] Input No. 12 Name"),
	_T("[Ext 4] Input No. 13 Name"),
	_T("[Ext 4] Input No. 14 Name"),
	_T("[Ext 4] Input No. 15 Name"),
	_T("[Ext 5] Input Name"),
	_T("[Ext 5] Input No. 0 Name"),
	_T("[Ext 5] Input No. 1 Name"),
	_T("[Ext 5] Input No. 2 Name"),
	_T("[Ext 5] Input No. 3 Name"),
	_T("[Ext 5] Input No. 4 Name"),
	_T("[Ext 5] Input No. 5 Name"),
	_T("[Ext 5] Input No. 6 Name"),
	_T("[Ext 5] Input No. 7 Name"),
	_T("[Ext 5] Input No. 8 Name"),
	_T("[Ext 5] Input No. 9 Name"),
	_T("[Ext 5] Input No. 10 Name"),
	_T("[Ext 5] Input No. 11 Name"),
	_T("[Ext 5] Input No. 12 Name"),
	_T("[Ext 5] Input No. 13 Name"),
	_T("[Ext 5] Input No. 14 Name"),
	_T("[Ext 5] Input No. 15 Name"),
	_T("[Ext 6] Input Name"),
	_T("[Ext 6] Input No. 0 Name"),
	_T("[Ext 6] Input No. 1 Name"),
	_T("[Ext 6] Input No. 2 Name"),
	_T("[Ext 6] Input No. 3 Name"),
	_T("[Ext 6] Input No. 4 Name"),
	_T("[Ext 6] Input No. 5 Name"),
	_T("[Ext 6] Input No. 6 Name"),
	_T("[Ext 6] Input No. 7 Name"),
	_T("[Ext 6] Input No. 8 Name"),
	_T("[Ext 6] Input No. 9 Name"),
	_T("[Ext 6] Input No. 10 Name"),
	_T("[Ext 6] Input No. 11 Name"),
	_T("[Ext 6] Input No. 12 Name"),
	_T("[Ext 6] Input No. 13 Name"),
	_T("[Ext 6] Input No. 14 Name"),
	_T("[Ext 6] Input No. 15 Name"),
	_T("[Ext 7] Input Name"),
	_T("[Ext 7] Input No. 0 Name"),
	_T("[Ext 7] Input No. 1 Name"),
	_T("[Ext 7] Input No. 2 Name"),
	_T("[Ext 7] Input No. 3 Name"),
	_T("[Ext 7] Input No. 4 Name"),
	_T("[Ext 7] Input No. 5 Name"),
	_T("[Ext 7] Input No. 6 Name"),
	_T("[Ext 7] Input No. 7 Name"),
	_T("[Ext 7] Input No. 8 Name"),
	_T("[Ext 7] Input No. 9 Name"),
	_T("[Ext 7] Input No. 10 Name"),
	_T("[Ext 7] Input No. 11 Name"),
	_T("[Ext 7] Input No. 12 Name"),
	_T("[Ext 7] Input No. 13 Name"),
	_T("[Ext 7] Input No. 14 Name"),
	_T("[Ext 7] Input No. 15 Name"),
	_T("[Ext 8] Input Name"),
	_T("[Ext 8] Input No. 0 Name"),
	_T("[Ext 8] Input No. 1 Name"),
	_T("[Ext 8] Input No. 2 Name"),
	_T("[Ext 8] Input No. 3 Name"),
	_T("[Ext 8] Input No. 4 Name"),
	_T("[Ext 8] Input No. 5 Name"),
	_T("[Ext 8] Input No. 6 Name"),
	_T("[Ext 8] Input No. 7 Name"),
	_T("[Ext 8] Input No. 8 Name"),
	_T("[Ext 8] Input No. 9 Name"),
	_T("[Ext 8] Input No. 10 Name"),
	_T("[Ext 8] Input No. 11 Name"),
	_T("[Ext 8] Input No. 12 Name"),
	_T("[Ext 8] Input No. 13 Name"),
	_T("[Ext 8] Input No. 14 Name"),
	_T("[Ext 8] Input No. 15 Name"),
	_T("[Ext 9] Input Name"),
	_T("[Ext 9] Input No. 0 Name"),
	_T("[Ext 9] Input No. 1 Name"),
	_T("[Ext 9] Input No. 2 Name"),
	_T("[Ext 9] Input No. 3 Name"),
	_T("[Ext 9] Input No. 4 Name"),
	_T("[Ext 9] Input No. 5 Name"),
	_T("[Ext 9] Input No. 6 Name"),
	_T("[Ext 9] Input No. 7 Name"),
	_T("[Ext 9] Input No. 8 Name"),
	_T("[Ext 9] Input No. 9 Name"),
	_T("[Ext 9] Input No. 10 Name"),
	_T("[Ext 9] Input No. 11 Name"),
	_T("[Ext 9] Input No. 12 Name"),
	_T("[Ext 9] Input No. 13 Name"),
	_T("[Ext 9] Input No. 14 Name"),
	_T("[Ext 9] Input No. 15 Name"),
	_T("[Ext 10] Input Name"),
	_T("[Ext 10] Input No. 0 Name"),
	_T("[Ext 10] Input No. 1 Name"),
	_T("[Ext 10] Input No. 2 Name"),
	_T("[Ext 10] Input No. 3 Name"),
	_T("[Ext 10] Input No. 4 Name"),
	_T("[Ext 10] Input No. 5 Name"),
	_T("[Ext 10] Input No. 6 Name"),
	_T("[Ext 10] Input No. 7 Name"),
	_T("[Ext 10] Input No. 8 Name"),
	_T("[Ext 10] Input No. 9 Name"),
	_T("[Ext 10] Input No. 10 Name"),
	_T("[Ext 10] Input No. 11 Name"),
	_T("[Ext 10] Input No. 12 Name"),
	_T("[Ext 10] Input No. 13 Name"),
	_T("[Ext 10] Input No. 14 Name"),
	_T("[Ext 10] Input No. 15 Name"),
	_T("[Ext 11] Input Name"),
	_T("[Ext 11] Input No. 0 Name"),
	_T("[Ext 11] Input No. 1 Name"),
	_T("[Ext 11] Input No. 2 Name"),
	_T("[Ext 11] Input No. 3 Name"),
	_T("[Ext 11] Input No. 4 Name"),
	_T("[Ext 11] Input No. 5 Name"),
	_T("[Ext 11] Input No. 6 Name"),
	_T("[Ext 11] Input No. 7 Name"),
	_T("[Ext 11] Input No. 8 Name"),
	_T("[Ext 11] Input No. 9 Name"),
	_T("[Ext 11] Input No. 10 Name"),
	_T("[Ext 11] Input No. 11 Name"),
	_T("[Ext 11] Input No. 12 Name"),
	_T("[Ext 11] Input No. 13 Name"),
	_T("[Ext 11] Input No. 14 Name"),
	_T("[Ext 11] Input No. 15 Name"),
	_T("[Ext 12] Input Name"),
	_T("[Ext 12] Input No. 0 Name"),
	_T("[Ext 12] Input No. 1 Name"),
	_T("[Ext 12] Input No. 2 Name"),
	_T("[Ext 12] Input No. 3 Name"),
	_T("[Ext 12] Input No. 4 Name"),
	_T("[Ext 12] Input No. 5 Name"),
	_T("[Ext 12] Input No. 6 Name"),
	_T("[Ext 12] Input No. 7 Name"),
	_T("[Ext 12] Input No. 8 Name"),
	_T("[Ext 12] Input No. 9 Name"),
	_T("[Ext 12] Input No. 10 Name"),
	_T("[Ext 12] Input No. 11 Name"),
	_T("[Ext 12] Input No. 12 Name"),
	_T("[Ext 12] Input No. 13 Name"),
	_T("[Ext 12] Input No. 14 Name"),
	_T("[Ext 12] Input No. 15 Name"),
	_T("[Ext 13] Input Name"),
	_T("[Ext 13] Input No. 0 Name"),
	_T("[Ext 13] Input No. 1 Name"),
	_T("[Ext 13] Input No. 2 Name"),
	_T("[Ext 13] Input No. 3 Name"),
	_T("[Ext 13] Input No. 4 Name"),
	_T("[Ext 13] Input No. 5 Name"),
	_T("[Ext 13] Input No. 6 Name"),
	_T("[Ext 13] Input No. 7 Name"),
	_T("[Ext 13] Input No. 8 Name"),
	_T("[Ext 13] Input No. 9 Name"),
	_T("[Ext 13] Input No. 10 Name"),
	_T("[Ext 13] Input No. 11 Name"),
	_T("[Ext 13] Input No. 12 Name"),
	_T("[Ext 13] Input No. 13 Name"),
	_T("[Ext 13] Input No. 14 Name"),
	_T("[Ext 13] Input No. 15 Name"),
	_T("[Ext 14] Input Name"),
	_T("[Ext 14] Input No. 0 Name"),
	_T("[Ext 14] Input No. 1 Name"),
	_T("[Ext 14] Input No. 2 Name"),
	_T("[Ext 14] Input No. 3 Name"),
	_T("[Ext 14] Input No. 4 Name"),
	_T("[Ext 14] Input No. 5 Name"),
	_T("[Ext 14] Input No. 6 Name"),
	_T("[Ext 14] Input No. 7 Name"),
	_T("[Ext 14] Input No. 8 Name"),
	_T("[Ext 14] Input No. 9 Name"),
	_T("[Ext 14] Input No. 10 Name"),
	_T("[Ext 14] Input No. 11 Name"),
	_T("[Ext 14] Input No. 12 Name"),
	_T("[Ext 14] Input No. 13 Name"),
	_T("[Ext 14] Input No. 14 Name"),
	_T("[Ext 14] Input No. 15 Name"),
	_T("[Ext 15] Input Name"),
	_T("[Ext 15] Input No. 0 Name"),
	_T("[Ext 15] Input No. 1 Name"),
	_T("[Ext 15] Input No. 2 Name"),
	_T("[Ext 15] Input No. 3 Name"),
	_T("[Ext 15] Input No. 4 Name"),
	_T("[Ext 15] Input No. 5 Name"),
	_T("[Ext 15] Input No. 6 Name"),
	_T("[Ext 15] Input No. 7 Name"),
	_T("[Ext 15] Input No. 8 Name"),
	_T("[Ext 15] Input No. 9 Name"),
	_T("[Ext 15] Input No. 10 Name"),
	_T("[Ext 15] Input No. 11 Name"),
	_T("[Ext 15] Input No. 12 Name"),
	_T("[Ext 15] Input No. 13 Name"),
	_T("[Ext 15] Input No. 14 Name"),
	_T("[Ext 15] Input No. 15 Name"),
	_T("[Ext 16] Input Name"),
	_T("[Ext 16] Input No. 0 Name"),
	_T("[Ext 16] Input No. 1 Name"),
	_T("[Ext 16] Input No. 2 Name"),
	_T("[Ext 16] Input No. 3 Name"),
	_T("[Ext 16] Input No. 4 Name"),
	_T("[Ext 16] Input No. 5 Name"),
	_T("[Ext 16] Input No. 6 Name"),
	_T("[Ext 16] Input No. 7 Name"),
	_T("[Ext 16] Input No. 8 Name"),
	_T("[Ext 16] Input No. 9 Name"),
	_T("[Ext 16] Input No. 10 Name"),
	_T("[Ext 16] Input No. 11 Name"),
	_T("[Ext 16] Input No. 12 Name"),
	_T("[Ext 16] Input No. 13 Name"),
	_T("[Ext 16] Input No. 14 Name"),
	_T("[Ext 16] Input No. 15 Name"),		
	_T("[Ext 1] Output Name"),
	_T("[Ext 1] Output No. 0 Name"),
	_T("[Ext 1] Output No. 1 Name"),
	_T("[Ext 1] Output No. 2 Name"),
	_T("[Ext 1] Output No. 3 Name"),
	_T("[Ext 1] Output No. 4 Name"),
	_T("[Ext 1] Output No. 5 Name"),
	_T("[Ext 1] Output No. 6 Name"),
	_T("[Ext 1] Output No. 7 Name"),
	_T("[Ext 1] Output No. 8 Name"),
	_T("[Ext 1] Output No. 9 Name"),
	_T("[Ext 1] Output No. 10 Name"),
	_T("[Ext 1] Output No. 11 Name"),
	_T("[Ext 1] Output No. 12 Name"),
	_T("[Ext 1] Output No. 13 Name"),
	_T("[Ext 1] Output No. 14 Name"),
	_T("[Ext 1] Output No. 15 Name"),
	_T("[Ext 2] Output Name"),
	_T("[Ext 2] Output No. 0 Name"),
	_T("[Ext 2] Output No. 1 Name"),
	_T("[Ext 2] Output No. 2 Name"),
	_T("[Ext 2] Output No. 3 Name"),
	_T("[Ext 2] Output No. 4 Name"),
	_T("[Ext 2] Output No. 5 Name"),
	_T("[Ext 2] Output No. 6 Name"),
	_T("[Ext 2] Output No. 7 Name"),
	_T("[Ext 2] Output No. 8 Name"),
	_T("[Ext 2] Output No. 9 Name"),
	_T("[Ext 2] Output No. 10 Name"),
	_T("[Ext 2] Output No. 11 Name"),
	_T("[Ext 2] Output No. 12 Name"),
	_T("[Ext 2] Output No. 13 Name"),
	_T("[Ext 2] Output No. 14 Name"),
	_T("[Ext 2] Output No. 15 Name"),
	_T("[Ext 3] Output Name"),
	_T("[Ext 3] Output No. 0 Name"),
	_T("[Ext 3] Output No. 1 Name"),
	_T("[Ext 3] Output No. 2 Name"),
	_T("[Ext 3] Output No. 3 Name"),
	_T("[Ext 3] Output No. 4 Name"),
	_T("[Ext 3] Output No. 5 Name"),
	_T("[Ext 3] Output No. 6 Name"),
	_T("[Ext 3] Output No. 7 Name"),
	_T("[Ext 3] Output No. 8 Name"),
	_T("[Ext 3] Output No. 9 Name"),
	_T("[Ext 3] Output No. 10 Name"),
	_T("[Ext 3] Output No. 11 Name"),
	_T("[Ext 3] Output No. 12 Name"),
	_T("[Ext 3] Output No. 13 Name"),
	_T("[Ext 3] Output No. 14 Name"),
	_T("[Ext 3] Output No. 15 Name"),
	_T("[Ext 4] Output Name"),
	_T("[Ext 4] Output No. 0 Name"),
	_T("[Ext 4] Output No. 1 Name"),
	_T("[Ext 4] Output No. 2 Name"),
	_T("[Ext 4] Output No. 3 Name"),
	_T("[Ext 4] Output No. 4 Name"),
	_T("[Ext 4] Output No. 5 Name"),
	_T("[Ext 4] Output No. 6 Name"),
	_T("[Ext 4] Output No. 7 Name"),
	_T("[Ext 4] Output No. 8 Name"),
	_T("[Ext 4] Output No. 9 Name"),
	_T("[Ext 4] Output No. 10 Name"),
	_T("[Ext 4] Output No. 11 Name"),
	_T("[Ext 4] Output No. 12 Name"),
	_T("[Ext 4] Output No. 13 Name"),
	_T("[Ext 4] Output No. 14 Name"),
	_T("[Ext 4] Output No. 15 Name"),
	_T("[Ext 5] Output Name"),
	_T("[Ext 5] Output No. 0 Name"),
	_T("[Ext 5] Output No. 1 Name"),
	_T("[Ext 5] Output No. 2 Name"),
	_T("[Ext 5] Output No. 3 Name"),
	_T("[Ext 5] Output No. 4 Name"),
	_T("[Ext 5] Output No. 5 Name"),
	_T("[Ext 5] Output No. 6 Name"),
	_T("[Ext 5] Output No. 7 Name"),
	_T("[Ext 5] Output No. 8 Name"),
	_T("[Ext 5] Output No. 9 Name"),
	_T("[Ext 5] Output No. 10 Name"),
	_T("[Ext 5] Output No. 11 Name"),
	_T("[Ext 5] Output No. 12 Name"),
	_T("[Ext 5] Output No. 13 Name"),
	_T("[Ext 5] Output No. 14 Name"),
	_T("[Ext 5] Output No. 15 Name"),
	_T("[Ext 6] Output Name"),
	_T("[Ext 6] Output No. 0 Name"),
	_T("[Ext 6] Output No. 1 Name"),
	_T("[Ext 6] Output No. 2 Name"),
	_T("[Ext 6] Output No. 3 Name"),
	_T("[Ext 6] Output No. 4 Name"),
	_T("[Ext 6] Output No. 5 Name"),
	_T("[Ext 6] Output No. 6 Name"),
	_T("[Ext 6] Output No. 7 Name"),
	_T("[Ext 6] Output No. 8 Name"),
	_T("[Ext 6] Output No. 9 Name"),
	_T("[Ext 6] Output No. 10 Name"),
	_T("[Ext 6] Output No. 11 Name"),
	_T("[Ext 6] Output No. 12 Name"),
	_T("[Ext 6] Output No. 13 Name"),
	_T("[Ext 6] Output No. 14 Name"),
	_T("[Ext 6] Output No. 15 Name"),
	_T("[Ext 7] Output Name"),
	_T("[Ext 7] Output No. 0 Name"),
	_T("[Ext 7] Output No. 1 Name"),
	_T("[Ext 7] Output No. 2 Name"),
	_T("[Ext 7] Output No. 3 Name"),
	_T("[Ext 7] Output No. 4 Name"),
	_T("[Ext 7] Output No. 5 Name"),
	_T("[Ext 7] Output No. 6 Name"),
	_T("[Ext 7] Output No. 7 Name"),
	_T("[Ext 7] Output No. 8 Name"),
	_T("[Ext 7] Output No. 9 Name"),
	_T("[Ext 7] Output No. 10 Name"),
	_T("[Ext 7] Output No. 11 Name"),
	_T("[Ext 7] Output No. 12 Name"),
	_T("[Ext 7] Output No. 13 Name"),
	_T("[Ext 7] Output No. 14 Name"),
	_T("[Ext 7] Output No. 15 Name"),
	_T("[Ext 8] Output Name"),
	_T("[Ext 8] Output No. 0 Name"),
	_T("[Ext 8] Output No. 1 Name"),
	_T("[Ext 8] Output No. 2 Name"),
	_T("[Ext 8] Output No. 3 Name"),
	_T("[Ext 8] Output No. 4 Name"),
	_T("[Ext 8] Output No. 5 Name"),
	_T("[Ext 8] Output No. 6 Name"),
	_T("[Ext 8] Output No. 7 Name"),
	_T("[Ext 8] Output No. 8 Name"),
	_T("[Ext 8] Output No. 9 Name"),
	_T("[Ext 8] Output No. 10 Name"),
	_T("[Ext 8] Output No. 11 Name"),
	_T("[Ext 8] Output No. 12 Name"),
	_T("[Ext 8] Output No. 13 Name"),
	_T("[Ext 8] Output No. 14 Name"),
	_T("[Ext 8] Output No. 15 Name"),
	_T("[Ext 9] Output Name"),
	_T("[Ext 9] Output No. 0 Name"),
	_T("[Ext 9] Output No. 1 Name"),
	_T("[Ext 9] Output No. 2 Name"),
	_T("[Ext 9] Output No. 3 Name"),
	_T("[Ext 9] Output No. 4 Name"),
	_T("[Ext 9] Output No. 5 Name"),
	_T("[Ext 9] Output No. 6 Name"),
	_T("[Ext 9] Output No. 7 Name"),
	_T("[Ext 9] Output No. 8 Name"),
	_T("[Ext 9] Output No. 9 Name"),
	_T("[Ext 9] Output No. 10 Name"),
	_T("[Ext 9] Output No. 11 Name"),
	_T("[Ext 9] Output No. 12 Name"),
	_T("[Ext 9] Output No. 13 Name"),
	_T("[Ext 9] Output No. 14 Name"),
	_T("[Ext 9] Output No. 15 Name"),
	_T("[Ext 10] Output Name"),
	_T("[Ext 10] Output No. 0 Name"),
	_T("[Ext 10] Output No. 1 Name"),
	_T("[Ext 10] Output No. 2 Name"),
	_T("[Ext 10] Output No. 3 Name"),
	_T("[Ext 10] Output No. 4 Name"),
	_T("[Ext 10] Output No. 5 Name"),
	_T("[Ext 10] Output No. 6 Name"),
	_T("[Ext 10] Output No. 7 Name"),
	_T("[Ext 10] Output No. 8 Name"),
	_T("[Ext 10] Output No. 9 Name"),
	_T("[Ext 10] Output No. 10 Name"),
	_T("[Ext 10] Output No. 11 Name"),
	_T("[Ext 10] Output No. 12 Name"),
	_T("[Ext 10] Output No. 13 Name"),
	_T("[Ext 10] Output No. 14 Name"),
	_T("[Ext 10] Output No. 15 Name"),
	_T("[Ext 11] Output Name"),
	_T("[Ext 11] Output No. 0 Name"),
	_T("[Ext 11] Output No. 1 Name"),
	_T("[Ext 11] Output No. 2 Name"),
	_T("[Ext 11] Output No. 3 Name"),
	_T("[Ext 11] Output No. 4 Name"),
	_T("[Ext 11] Output No. 5 Name"),
	_T("[Ext 11] Output No. 6 Name"),
	_T("[Ext 11] Output No. 7 Name"),
	_T("[Ext 11] Output No. 8 Name"),
	_T("[Ext 11] Output No. 9 Name"),
	_T("[Ext 11] Output No. 10 Name"),
	_T("[Ext 11] Output No. 11 Name"),
	_T("[Ext 11] Output No. 12 Name"),
	_T("[Ext 11] Output No. 13 Name"),
	_T("[Ext 11] Output No. 14 Name"),
	_T("[Ext 11] Output No. 15 Name"),
	_T("[Ext 12] Output Name"),
	_T("[Ext 12] Output No. 0 Name"),
	_T("[Ext 12] Output No. 1 Name"),
	_T("[Ext 12] Output No. 2 Name"),
	_T("[Ext 12] Output No. 3 Name"),
	_T("[Ext 12] Output No. 4 Name"),
	_T("[Ext 12] Output No. 5 Name"),
	_T("[Ext 12] Output No. 6 Name"),
	_T("[Ext 12] Output No. 7 Name"),
	_T("[Ext 12] Output No. 8 Name"),
	_T("[Ext 12] Output No. 9 Name"),
	_T("[Ext 12] Output No. 10 Name"),
	_T("[Ext 12] Output No. 11 Name"),
	_T("[Ext 12] Output No. 12 Name"),
	_T("[Ext 12] Output No. 13 Name"),
	_T("[Ext 12] Output No. 14 Name"),
	_T("[Ext 12] Output No. 15 Name"),
	_T("[Ext 13] Output Name"),
	_T("[Ext 13] Output No. 0 Name"),
	_T("[Ext 13] Output No. 1 Name"),
	_T("[Ext 13] Output No. 2 Name"),
	_T("[Ext 13] Output No. 3 Name"),
	_T("[Ext 13] Output No. 4 Name"),
	_T("[Ext 13] Output No. 5 Name"),
	_T("[Ext 13] Output No. 6 Name"),
	_T("[Ext 13] Output No. 7 Name"),
	_T("[Ext 13] Output No. 8 Name"),
	_T("[Ext 13] Output No. 9 Name"),
	_T("[Ext 13] Output No. 10 Name"),
	_T("[Ext 13] Output No. 11 Name"),
	_T("[Ext 13] Output No. 12 Name"),
	_T("[Ext 13] Output No. 13 Name"),
	_T("[Ext 13] Output No. 14 Name"),
	_T("[Ext 13] Output No. 15 Name"),
	_T("[Ext 14] Output Name"),
	_T("[Ext 14] Output No. 0 Name"),
	_T("[Ext 14] Output No. 1 Name"),
	_T("[Ext 14] Output No. 2 Name"),
	_T("[Ext 14] Output No. 3 Name"),
	_T("[Ext 14] Output No. 4 Name"),
	_T("[Ext 14] Output No. 5 Name"),
	_T("[Ext 14] Output No. 6 Name"),
	_T("[Ext 14] Output No. 7 Name"),
	_T("[Ext 14] Output No. 8 Name"),
	_T("[Ext 14] Output No. 9 Name"),
	_T("[Ext 14] Output No. 10 Name"),
	_T("[Ext 14] Output No. 11 Name"),
	_T("[Ext 14] Output No. 12 Name"),
	_T("[Ext 14] Output No. 13 Name"),
	_T("[Ext 14] Output No. 14 Name"),
	_T("[Ext 14] Output No. 15 Name"),
	_T("[Ext 15] Output Name"),
	_T("[Ext 15] Output No. 0 Name"),
	_T("[Ext 15] Output No. 1 Name"),
	_T("[Ext 15] Output No. 2 Name"),
	_T("[Ext 15] Output No. 3 Name"),
	_T("[Ext 15] Output No. 4 Name"),
	_T("[Ext 15] Output No. 5 Name"),
	_T("[Ext 15] Output No. 6 Name"),
	_T("[Ext 15] Output No. 7 Name"),
	_T("[Ext 15] Output No. 8 Name"),
	_T("[Ext 15] Output No. 9 Name"),
	_T("[Ext 15] Output No. 10 Name"),
	_T("[Ext 15] Output No. 11 Name"),
	_T("[Ext 15] Output No. 12 Name"),
	_T("[Ext 15] Output No. 13 Name"),
	_T("[Ext 15] Output No. 14 Name"),
	_T("[Ext 15] Output No. 15 Name"),
	_T("[Ext 16] Output Name"),
	_T("[Ext 16] Output No. 0 Name"),
	_T("[Ext 16] Output No. 1 Name"),
	_T("[Ext 16] Output No. 2 Name"),
	_T("[Ext 16] Output No. 3 Name"),
	_T("[Ext 16] Output No. 4 Name"),
	_T("[Ext 16] Output No. 5 Name"),
	_T("[Ext 16] Output No. 6 Name"),
	_T("[Ext 16] Output No. 7 Name"),
	_T("[Ext 16] Output No. 8 Name"),
	_T("[Ext 16] Output No. 9 Name"),
	_T("[Ext 16] Output No. 10 Name"),
	_T("[Ext 16] Output No. 11 Name"),
	_T("[Ext 16] Output No. 12 Name"),
	_T("[Ext 16] Output No. 13 Name"),
	_T("[Ext 16] Output No. 14 Name"),
	_T("[Ext 16] Output No. 15 Name"),
};

static LPCTSTR g_lpszGoogolGTSSwitch[EGoogolGtsSwitch_Count] =
{
	_T("Off"),
	_T("On"),
};

static LPCTSTR g_lpszGoogolGTSInitializeType[EGoogolGtsInitializeType_Count] = 
{
	_T("Not Ext Mode"),
	_T("Only Ext Mode"),
	_T("Motion and Ext Mode"),
};

CDeviceGoogolGTS::CDeviceGoogolGTS()
{
	m_sMotionInfo[0] = { 0 };
	m_sMotionInfo[1] = { 0 };
	m_sMotionInfo[2] = { 0 };
	m_sMotionInfo[3] = { 0 };
	m_sMotionInfo[4] = { 0 };
	m_sMotionInfo[5] = { 0 };
	m_sMotionInfo[6] = { 0 };
	m_sMotionInfo[7] = { 0 };
	m_bCreateMotionUI = false;
	m_dwOutStatus = 0;

	if(!m_pBUsingInputArray)
		m_pBUsingInputArray = (bool*)calloc(GTS_DIO_IN_PORT, sizeof(bool));

	if(!m_pBUsingOutputArray)
		m_pBUsingOutputArray = (bool*)calloc(GTS_DIO_OUT_PORT, sizeof(bool));

	if(!m_pStrInputNameArray)
		m_pStrInputNameArray = new CString[GTS_DIO_IN_PORT];

	if(!m_pStrOutputNameArray)
		m_pStrOutputNameArray = new CString[GTS_DIO_OUT_PORT];

	if(!m_ppStrExtInputName)
		m_ppStrExtInputName = new CString*[GTS_EXTIO_MODULES];

	for(int i = 0; i < GTS_EXTIO_MODULES; ++i)
		*(m_ppStrExtInputName + i) = new CString[GTS_EXTIO_PORT];

	if(!m_ppStrExtOutputName)
		m_ppStrExtOutputName = new CString*[GTS_EXTIO_MODULES];

	for(int i = 0; i < GTS_EXTIO_MODULES; ++i)
		*(m_ppStrExtOutputName + i) = new CString[GTS_EXTIO_PORT];
}


CDeviceGoogolGTS::~CDeviceGoogolGTS()
{
	Terminate();

	if(m_pBUsingInputArray)
		delete[] m_pBUsingInputArray;
	m_pBUsingInputArray = nullptr;

	if(m_pBUsingOutputArray)
		delete[] m_pBUsingOutputArray;
	m_pBUsingOutputArray = nullptr;

	if(m_pStrInputNameArray)
		delete[] m_pStrInputNameArray;
	m_pStrInputNameArray = nullptr;

	if(m_pStrOutputNameArray)
		delete[] m_pStrOutputNameArray;
	m_pStrOutputNameArray = nullptr;

	if(m_pDlgExtDioViewer)
		delete[] m_pDlgExtDioViewer;
	m_pDlgExtDioViewer = nullptr;

	if(m_ppStrExtInputName)
	{
		for(int i = 0; i < GTS_EXTIO_MODULES; ++i)
		{
			if((*(m_ppStrExtInputName + i)))
				delete[](*(m_ppStrExtInputName + i));
			(*(m_ppStrExtInputName + i)) = nullptr;
		}

		delete m_ppStrExtInputName;
		m_ppStrExtInputName = nullptr;
	}

	if(m_ppStrExtOutputName)
	{
		for(int i = 0; i < GTS_EXTIO_MODULES; ++i)
		{
			if((*(m_ppStrExtOutputName + i)))
				delete[](*(m_ppStrExtOutputName + i));
			(*(m_ppStrExtOutputName + i)) = nullptr;
		}

		delete m_ppStrExtOutputName;
		m_ppStrExtOutputName = nullptr;
	}
}

ptrdiff_t CDeviceGoogolGTS::OnMessage(CMessageBase * pMessage)
{
	__super::OnMessage(pMessage);

	do
	{
		if(!pMessage)
			break;

		if(pMessage->GetMessage() != EMessage_LanguageChange)
			break;

		if(!IsWindow(GetSafeHwnd()))
			break;

		CWnd* pCtrlUI = nullptr;

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Input));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 1);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Output));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 2);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Axis) + _T(" ") + CMultiLanguageManager::GetString(ELanguageParameter_Selector));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 3);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Axis));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 4);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_MotorControlling));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 5);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_SensorStatus));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 6);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_ServoMode));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 7);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Home));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 8);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Velocity) + _T(" :"));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 9);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Acceleration) + _T(" :"));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 10);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Distance) + _T(" :"));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 11);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Position) + _T(" :"));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 12);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_LimitPlus));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 13);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Home));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 14);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_LimitMinus));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 15);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Alarm));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 16);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Inposition));

		pCtrlUI = GetDlgItem(EDeviceAddControlID_BtnHomming);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Home));

		pCtrlUI = GetDlgItem(EDeviceAddControlID_BtnServoModeOff);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Off));

		pCtrlUI = GetDlgItem(EDeviceAddControlID_BtnServoModeOn);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_On));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 17);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_CommandPosition));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 18);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_ActualPosition));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 19);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Alarm));

		pCtrlUI = GetDlgItem(EDeviceAddControlID_CommandPosition);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Value) + _T(" : 0.0000"));

		pCtrlUI = GetDlgItem(EDeviceAddControlID_BtnCommandReset);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Reset));

		pCtrlUI = GetDlgItem(EDeviceAddControlID_ActualPosition);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Value) + _T(" : 0.0000"));

		pCtrlUI = GetDlgItem(EDeviceAddControlID_BtnActualReset);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Reset));

		pCtrlUI = GetDlgItem(EDeviceAddControlID_BtnAlarmReset);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Reset));
	}
	while(false);

	return 0;
}

EDeviceInitializeResult CDeviceGoogolGTS::Initialize()
{
	EDeviceInitializeResult eReturn = EDeviceInitializeResult_UnknownError;

	CString strMessage, strStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("GoogolGTS"));

		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);
		SetStatus(strStatus);

		return EDeviceInitializeResult_NotFoundApiError;
	}

	do 
	{
		if(IsInitialized())
		{
			strStatus = CMultiLanguageManager::GetString(ELanguageParameter_Initialized);
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Thedevicehasbeenalreadyinitialized);
			eReturn = EDeviceInitializeResult_AlreadyInitializedError;
			break;
		}
		
		EGoogolGtsInitializeType eInitType = EGoogolGtsInitializeType_Count;

		if(GetInitializeType(&eInitType))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Initialize Type"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		int nDeviceID = _ttoi(GetDeviceID());

		bool bError = true;

		switch (eInitType)
		{
		case EGoogolGtsInitializeType_NotExtMode:
			{
				if(GT_Open(nDeviceID))
				{
					strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthelibrary);
					eReturn = EDeviceInitializeResult_NotCreateDeviceError;
					break;
				}

				if(GT_Reset(nDeviceID))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Reset controller"));
					eReturn = EDeviceInitializeResult_NotResetDevice;
					break;
				}

				CString strLoadFile;

				if(GetLoadFilePath(&strLoadFile))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("load file path"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(GT_LoadConfig(nDeviceID, CStringA(strLoadFile).GetBuffer()))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Load file"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				if(GT_ClrSts(nDeviceID, 1, 8))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Clear control"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				bool bMotionBand = false;

				for(int i = 0; i < GTS_MOTION_MAX; ++i)
				{
					if(GT_SetAxisBand(nDeviceID, i + 1, 20, 5))
					{
						bMotionBand = true;
						break;
					}
				}

				if(bMotionBand)
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Update band"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				if(GT_HomeInit(nDeviceID))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Home init"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				ClearOutPort();

				bError = false;
			}
			break;
		case EGoogolGtsInitializeType_OnlyExtMode:
			{
				if(GT_OpenExtMdl(nDeviceID))
				{
					strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthelibrary);
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				if (GT_ResetExtMdl(nDeviceID))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Reset ext controller"));
					eReturn = EDeviceInitializeResult_NotResetDevice;
					break;
				}

				CString strExtPath;

				if(GetLoadExtFilePath(&strExtPath))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("load ext file path"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(GT_LoadExtConfig(nDeviceID, CStringA(strExtPath).GetBuffer()))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Load ext file"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				ClearExtOutport();

				bError = false;
			}
			break;
		case EGoogolGtsInitializeType_MotionAndExtMode:
			{
				if(GT_Open(nDeviceID))
				{
					strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthelibrary);
					eReturn = EDeviceInitializeResult_NotCreateDeviceError;
					break;
				}

				if(GT_Reset(nDeviceID))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Reset controller"));
					eReturn = EDeviceInitializeResult_NotResetDevice;
					break;
				}

				if (GT_OpenExtMdl(nDeviceID))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Load ext controller"));
					eReturn = EDeviceInitializeResult_NotCreateDeviceError;
					break;
				}

				if (GT_ResetExtMdl(nDeviceID))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Reset ext controller"));
					eReturn = EDeviceInitializeResult_NotResetDevice;
					break;
				}

				CString strLoadFile;

				if(GetLoadExtFilePath(&strLoadFile))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("load ext file path"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(GT_LoadExtConfig(nDeviceID, CStringA(strLoadFile).GetBuffer()))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Load ext file"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				if(GT_ClrSts(nDeviceID, 1, 8))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Clear control"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				bool bMotionBand = false;

				for(int i = 0; i < GTS_MOTION_MAX; ++i)
				{
					if(GT_SetAxisBand(nDeviceID, i + 1, 20, 5))
					{
						bMotionBand = true;
						break;
					}
				}

				if(bMotionBand)
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Update band"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				if(GT_HomeInit(nDeviceID))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Home init"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				ClearOutPort();
				ClearExtOutport();

				bError = false;
			}
			break;
		default:
			break;
		}

		if(bError)
			break;

		m_bIsInitialized = true;

		strStatus = CMultiLanguageManager::GetString(ELanguageParameter_Initialized);

		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyinitialized);

		eReturn = EDeviceInitializeResult_OK;

		BroadcastViewInit();

		CEventHandlerManager::BroadcastOnDeviceInitialized(this);
	} 
	while(false);

	if(!IsInitialized())
		Terminate();

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	SetStatus(strStatus);

	return eReturn;
}

EDeviceTerminateResult CDeviceGoogolGTS::Terminate()
{
	EDeviceTerminateResult eReturn = EDeviceTerminateResult_UnknownError;

	CString strMessage;

	SetStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("GoogolGTS"));
		
		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);

		return EDeviceTerminateResult_NotFoundApiError;
	}

	do 
	{
		EGoogolGtsInitializeType eInitType = EGoogolGtsInitializeType_Count;

		GetInitializeType(&eInitType);

		int nDeviceID = _ttoi(GetDeviceID());

		bool bError = true;

		switch(eInitType)
		{
		case EGoogolGtsInitializeType_NotExtMode:
			ClearOutPort();
			GT_Close(nDeviceID);
			break;
		case EGoogolGtsInitializeType_OnlyExtMode:
			ClearExtOutport();
			GT_CloseExtMdl(nDeviceID);
			break;
		case EGoogolGtsInitializeType_MotionAndExtMode:
			ClearOutPort();
			ClearExtOutport();
			GT_CloseExtMdl(nDeviceID);
			GT_Close(nDeviceID);
			break;		
		}
		
		m_bIsInitialized = false;

		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyterminated);

		eReturn = EDeviceTerminateResult_OK;

		BroadcastViewInit();

		CEventHandlerManager::BroadcastOnDeviceTerminated(this);
	} 
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceGoogolGTS::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();

	do
	{
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_DeviceID, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_DeviceID], _T("0"), EParameterFieldType_Edit);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_InitializeType, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_InitializeType], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGoogolGTSInitializeType, EGoogolGtsInitializeType_Count));
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_LoadfilePath, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_LoadfilePath], _T(""), EParameterFieldType_Edit);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_LoadExtFilePath, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_LoadExtFilePath], _T(""), EParameterFieldType_Edit);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_MotionSetting, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_MotionSetting], _T("0"), EParameterFieldType_None);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis1Band, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis1Band], _T("20"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis1Time, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis1Time], _T("5"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis1AbruptStop, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis1AbruptStop], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis1EStop, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis1EStop], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis2Band, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis2Band], _T("20"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis2Time, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis2Time], _T("5"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis2AbruptStop, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis2AbruptStop], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis2EStop, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis2EStop], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis3Band, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis3Band], _T("20"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis3Time, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis3Time], _T("5"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis3AbruptStop, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis3AbruptStop], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis3EStop, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis3EStop], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis4Band, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis4Band], _T("20"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis4Time, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis4Time], _T("5"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis4AbruptStop, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis4AbruptStop], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis4EStop, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis4EStop], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis5Band, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis5Band], _T("20"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis5Time, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis5Time], _T("5"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis5AbruptStop, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis5AbruptStop], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis5EStop, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis5EStop], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis6Band, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis6Band], _T("20"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis6Time, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis6Time], _T("5"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis6AbruptStop, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis6AbruptStop], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis6EStop, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis6EStop], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis7Band, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis7Band], _T("20"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis7Time, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis7Time], _T("5"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis7AbruptStop, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis7AbruptStop], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis7EStop, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis7EStop], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis8Band, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis8Band], _T("20"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis8Time, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis8Time], _T("5"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis8AbruptStop, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis8AbruptStop], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis8EStop, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis8EStop], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);



		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Home, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Home], _T("0"), EParameterFieldType_None);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis1DirAndPos, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis1DirAndPos], _T("20000"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis1Velocity, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis1Velocity], _T("50"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis1Accel, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis1Accel], _T("0.5"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis1Offset, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis1Offset], _T("2000"), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis2DirAndPos, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis2DirAndPos], _T("20000"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis2Velocity, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis2Velocity], _T("50"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis2Accel, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis2Accel], _T("0.5"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis2Offset, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis2Offset], _T("2000"), EParameterFieldType_Edit, nullptr, nullptr, 1);


		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis3DirAndPos, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis3DirAndPos], _T("20000"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis3Velocity, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis3Velocity], _T("50"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis3Accel, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis3Accel], _T("0.5"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis3Offset, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis3Offset], _T("2000"), EParameterFieldType_Edit, nullptr, nullptr, 1);


		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis4DirAndPos, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis4DirAndPos], _T("20000"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis4Velocity, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis4Velocity], _T("50"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis4Accel, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis4Accel], _T("0.5"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis4Offset, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis4Offset], _T("2000"), EParameterFieldType_Edit, nullptr, nullptr, 1);


		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis5DirAndPos, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis5DirAndPos], _T("20000"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis5Velocity, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis5Velocity], _T("50"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis5Accel, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis5Accel], _T("0.5"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis5Offset, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis5Offset], _T("2000"), EParameterFieldType_Edit, nullptr, nullptr, 1);


		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis6DirAndPos, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis6DirAndPos], _T("20000"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis6Velocity, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis6Velocity], _T("50"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis6Accel, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis6Accel], _T("0.5"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis6Offset, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis6Offset], _T("2000"), EParameterFieldType_Edit, nullptr, nullptr, 1);


		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis7DirAndPos, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis7DirAndPos], _T("20000"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis7Velocity, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis7Velocity], _T("50"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis7Accel, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis7Accel], _T("0.5"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis7Offset, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis7Offset], _T("2000"), EParameterFieldType_Edit, nullptr, nullptr, 1);


		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis8DirAndPos, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis8DirAndPos], _T("20000"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis8Velocity, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis8Velocity], _T("50"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis8Accel, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis8Accel], _T("0.5"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Axis8Offset, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Axis8Offset], _T("2000"), EParameterFieldType_Edit, nullptr, nullptr, 1);

		
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_InputStatus, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_InputStatus], _T("0"), EParameterFieldType_None, nullptr, nullptr, 0);
		for(int i = 0; i < GTS_DIO_IN_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_UseInputNo0 + i, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_UseInputNo0 + i], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_OutputStatus, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_OutputStatus], _T("0"), EParameterFieldType_None, nullptr, nullptr, 0);
		for(int i = 0; i < GTS_DIO_OUT_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_UseOutputNo0 + i, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_UseOutputNo0 + i], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_InputName, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_InputName], _T("0"), EParameterFieldType_None, nullptr, nullptr, 0);
		for(int i = 0; i < GTS_DIO_IN_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_InputNo0Name + i, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_InputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_OutputName, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_OutputName], _T("0"), EParameterFieldType_None, nullptr, nullptr, 0);
		for(int i = 0; i < GTS_DIO_OUT_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_OutputNo0Name + i, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_OutputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);
		
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_ExternalIOStatus, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_ExternalIOStatus], _T("1"), EParameterFieldType_None);
		for(int i = 0;i < GTS_EXTIO_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_ShowExt1 + i, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_ShowExt1 + i], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);
		
		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext1InputName, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext1InputName], _T("0"), EParameterFieldType_None);
		for(int i = 0; i < GTS_EXTIO_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext1InputNo0Name + i, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext1InputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext2InputName, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext2InputName], _T("0"), EParameterFieldType_None);
		for(int i = 0; i < GTS_EXTIO_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext2InputNo0Name + i, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext2InputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext3InputName, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext3InputName], _T("0"), EParameterFieldType_None);
		for(int i = 0; i < GTS_EXTIO_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext3InputNo0Name + i, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext3InputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext4InputName, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext4InputName], _T("0"), EParameterFieldType_None);
		for(int i = 0; i < GTS_EXTIO_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext4InputNo0Name + i, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext4InputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext5InputName, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext5InputName], _T("0"), EParameterFieldType_None);
		for(int i = 0; i < GTS_EXTIO_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext5InputNo0Name + i, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext5InputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext6InputName, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext6InputName], _T("0"), EParameterFieldType_None);
		for(int i = 0; i < GTS_EXTIO_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext6InputNo0Name + i, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext6InputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext7InputName, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext7InputName], _T("0"), EParameterFieldType_None);
		for(int i = 0; i < GTS_EXTIO_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext7InputNo0Name + i, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext7InputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext8InputName, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext8InputName], _T("0"), EParameterFieldType_None);
		for(int i = 0; i < GTS_EXTIO_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext8InputNo0Name + i, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext8InputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext9InputName, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext9InputName], _T("0"), EParameterFieldType_None);
		for(int i = 0; i < GTS_EXTIO_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext9InputNo0Name + i, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext9InputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext10InputName, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext10InputName], _T("0"), EParameterFieldType_None);
		for(int i = 0; i < GTS_EXTIO_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext10InputNo0Name + i, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext10InputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext11InputName, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext11InputName], _T("0"), EParameterFieldType_None);
		for(int i = 0; i < GTS_EXTIO_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext11InputNo0Name + i, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext11InputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext12InputName, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext12InputName], _T("0"), EParameterFieldType_None);
		for(int i = 0; i < GTS_EXTIO_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext12InputNo0Name + i, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext12InputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext13InputName, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext13InputName], _T("0"), EParameterFieldType_None);
		for(int i = 0; i < GTS_EXTIO_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext13InputNo0Name + i, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext13InputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext14InputName, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext14InputName], _T("0"), EParameterFieldType_None);
		for(int i = 0; i < GTS_EXTIO_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext14InputNo0Name + i, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext14InputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext15InputName, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext15InputName], _T("0"), EParameterFieldType_None);
		for(int i = 0; i < GTS_EXTIO_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext15InputNo0Name + i, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext15InputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext16InputName, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext16InputName], _T("0"), EParameterFieldType_None);
		for(int i = 0; i < GTS_EXTIO_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext16InputNo0Name + i, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext16InputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext1OutputName, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext1OutputName], _T("0"), EParameterFieldType_None);
		for(int i = 0; i < GTS_EXTIO_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext1OutputNo0Name + i, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext1OutputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext2OutputName, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext2OutputName], _T("0"), EParameterFieldType_None);
		for(int i = 0; i < GTS_EXTIO_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext2OutputNo0Name + i, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext2OutputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext3OutputName, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext3OutputName], _T("0"), EParameterFieldType_None);
		for(int i = 0; i < GTS_EXTIO_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext3OutputNo0Name + i, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext3OutputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext4OutputName, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext4OutputName], _T("0"), EParameterFieldType_None);
		for(int i = 0; i < GTS_EXTIO_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext4OutputNo0Name + i, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext4OutputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext5OutputName, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext5OutputName], _T("0"), EParameterFieldType_None);
		for(int i = 0; i < GTS_EXTIO_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext5OutputNo0Name + i, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext5OutputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext6OutputName, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext6OutputName], _T("0"), EParameterFieldType_None);
		for(int i = 0; i < GTS_EXTIO_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext6OutputNo0Name + i, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext6OutputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext7OutputName, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext7OutputName], _T("0"), EParameterFieldType_None);
		for(int i = 0; i < GTS_EXTIO_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext7OutputNo0Name + i, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext7OutputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext8OutputName, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext8OutputName], _T("0"), EParameterFieldType_None);
		for(int i = 0; i < GTS_EXTIO_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext8OutputNo0Name + i, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext8OutputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext9OutputName, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext9OutputName], _T("0"), EParameterFieldType_None);
		for(int i = 0; i < GTS_EXTIO_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext9OutputNo0Name + i, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext9OutputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext10OutputName, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext10OutputName], _T("0"), EParameterFieldType_None);
		for(int i = 0; i < GTS_EXTIO_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext10OutputNo0Name + i, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext10OutputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext11OutputName, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext11OutputName], _T("0"), EParameterFieldType_None);
		for(int i = 0; i < GTS_EXTIO_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext11OutputNo0Name + i, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext11OutputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext12OutputName, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext12OutputName], _T("0"), EParameterFieldType_None);
		for(int i = 0; i < GTS_EXTIO_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext12OutputNo0Name + i, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext12OutputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext13OutputName, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext13OutputName], _T("0"), EParameterFieldType_None);
		for(int i = 0; i < GTS_EXTIO_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext13OutputNo0Name + i, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext13OutputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext14OutputName, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext14OutputName], _T("0"), EParameterFieldType_None);
		for(int i = 0; i < GTS_EXTIO_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext14OutputNo0Name + i, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext14OutputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext15OutputName, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext15OutputName], _T("0"), EParameterFieldType_None);
		for(int i = 0; i < GTS_EXTIO_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext15OutputNo0Name + i, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext15OutputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext16OutputName, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext16OutputName], _T("0"), EParameterFieldType_None);

		for(int i = 0; i < GTS_EXTIO_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterGoogolGTS_Ext16OutputNo0Name + i, g_lpszParamGoogolGTS[EDeviceParameterGoogolGTS_Ext16OutputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

		bReturn = __super::LoadSettings();

		int nID = _ttoi(GetDeviceID());

		if(!m_pDlgExtDioViewer)
			m_pDlgExtDioViewer = new Ravid::Framework::CGoogolDioViewer[GTS_EXTIO_MODULES];

		if(m_pDlgExtDioViewer)
		{
			int nPosX = 0;
			int nPosY = 0;

			for(int i = 0; i < GTS_EXTIO_MODULES; ++i)
			{
				(m_pDlgExtDioViewer + i)->Create(this->GetParent());
				(m_pDlgExtDioViewer + i)->SetWindowPos(nullptr, nPosX * GTS_EXTIO_MODULES, nPosY * GTS_EXTIO_MODULES, 0, 0, SWP_NOSIZE);
				(m_pDlgExtDioViewer + i)->m_nDeviceID = nID;
				++nPosX;

				nPosY = (nPosX % 5);
			}
		}

		for(int i = EDeviceParameterGoogolGTS_UseInputNo0; i < EDeviceParameterGoogolGTS_OutputStatus; ++i)
			m_pBUsingInputArray[i - EDeviceParameterGoogolGTS_UseInputNo0] = _ttoi(m_vctParameterFieldConfigurations[i].strParameterValue);

		for(int i = EDeviceParameterGoogolGTS_UseOutputNo0; i < EDeviceParameterGoogolGTS_InputName; ++i)
			m_pBUsingOutputArray[i - EDeviceParameterGoogolGTS_UseOutputNo0] = _ttoi(m_vctParameterFieldConfigurations[i].strParameterValue);
		
		for(int i = EDeviceParameterGoogolGTS_InputNo0Name; i < EDeviceParameterGoogolGTS_OutputName; ++i)
			m_pStrInputNameArray[i - EDeviceParameterGoogolGTS_InputNo0Name] = m_vctParameterFieldConfigurations[i].strParameterValue;

		for(int i = EDeviceParameterGoogolGTS_OutputNo0Name; i < EDeviceParameterGoogolGTS_ExternalIOStatus; ++i)
			m_pStrOutputNameArray[i - EDeviceParameterGoogolGTS_OutputNo0Name] = m_vctParameterFieldConfigurations[i].strParameterValue;
		
		for(int i = EDeviceParameterGoogolGTS_Ext1InputNo0Name; i < EDeviceParameterGoogolGTS_Ext2InputName; ++i)
			m_ppStrExtInputName[0][i - EDeviceParameterGoogolGTS_Ext1InputNo0Name] = m_vctParameterFieldConfigurations[i].strParameterValue;

		for(int i = EDeviceParameterGoogolGTS_Ext2InputNo0Name; i < EDeviceParameterGoogolGTS_Ext3InputName; ++i)
			m_ppStrExtInputName[1][i - EDeviceParameterGoogolGTS_Ext2InputNo0Name] = m_vctParameterFieldConfigurations[i].strParameterValue;

		for(int i = EDeviceParameterGoogolGTS_Ext3InputNo0Name; i < EDeviceParameterGoogolGTS_Ext4InputName; ++i)
			m_ppStrExtInputName[2][i - EDeviceParameterGoogolGTS_Ext3InputNo0Name] = m_vctParameterFieldConfigurations[i].strParameterValue;

		for(int i = EDeviceParameterGoogolGTS_Ext4InputNo0Name; i < EDeviceParameterGoogolGTS_Ext5InputName; ++i)
			m_ppStrExtInputName[3][i - EDeviceParameterGoogolGTS_Ext4InputNo0Name] = m_vctParameterFieldConfigurations[i].strParameterValue;

		for(int i = EDeviceParameterGoogolGTS_Ext5InputNo0Name; i < EDeviceParameterGoogolGTS_Ext6InputName; ++i)
			m_ppStrExtInputName[4][i - EDeviceParameterGoogolGTS_Ext5InputNo0Name] = m_vctParameterFieldConfigurations[i].strParameterValue;

		for(int i = EDeviceParameterGoogolGTS_Ext6InputNo0Name; i < EDeviceParameterGoogolGTS_Ext7InputName; ++i)
			m_ppStrExtInputName[5][i - EDeviceParameterGoogolGTS_Ext6InputNo0Name] = m_vctParameterFieldConfigurations[i].strParameterValue;

		for(int i = EDeviceParameterGoogolGTS_Ext7InputNo0Name; i < EDeviceParameterGoogolGTS_Ext8InputName; ++i)
			m_ppStrExtInputName[6][i - EDeviceParameterGoogolGTS_Ext7InputNo0Name] = m_vctParameterFieldConfigurations[i].strParameterValue;

		for(int i = EDeviceParameterGoogolGTS_Ext8InputNo0Name; i < EDeviceParameterGoogolGTS_Ext9InputName; ++i)
			m_ppStrExtInputName[7][i - EDeviceParameterGoogolGTS_Ext8InputNo0Name] = m_vctParameterFieldConfigurations[i].strParameterValue;

		for(int i = EDeviceParameterGoogolGTS_Ext9InputNo0Name; i < EDeviceParameterGoogolGTS_Ext10InputName; ++i)
			m_ppStrExtInputName[8][i - EDeviceParameterGoogolGTS_Ext9InputNo0Name] = m_vctParameterFieldConfigurations[i].strParameterValue;

		for(int i = EDeviceParameterGoogolGTS_Ext10InputNo0Name; i < EDeviceParameterGoogolGTS_Ext11InputName; ++i)
			m_ppStrExtInputName[9][i - EDeviceParameterGoogolGTS_Ext10InputNo0Name] = m_vctParameterFieldConfigurations[i].strParameterValue;

		for(int i = EDeviceParameterGoogolGTS_Ext11InputNo0Name; i < EDeviceParameterGoogolGTS_Ext12InputName; ++i)
			m_ppStrExtInputName[10][i - EDeviceParameterGoogolGTS_Ext11InputNo0Name] = m_vctParameterFieldConfigurations[i].strParameterValue;

		for(int i = EDeviceParameterGoogolGTS_Ext12InputNo0Name; i < EDeviceParameterGoogolGTS_Ext13InputName; ++i)
			m_ppStrExtInputName[11][i - EDeviceParameterGoogolGTS_Ext12InputNo0Name] = m_vctParameterFieldConfigurations[i].strParameterValue;

		for(int i = EDeviceParameterGoogolGTS_Ext13InputNo0Name; i < EDeviceParameterGoogolGTS_Ext14InputName; ++i)
			m_ppStrExtInputName[12][i - EDeviceParameterGoogolGTS_Ext13InputNo0Name] = m_vctParameterFieldConfigurations[i].strParameterValue;

		for(int i = EDeviceParameterGoogolGTS_Ext14InputNo0Name; i < EDeviceParameterGoogolGTS_Ext15InputName; ++i)
			m_ppStrExtInputName[13][i - EDeviceParameterGoogolGTS_Ext14InputNo0Name] = m_vctParameterFieldConfigurations[i].strParameterValue;

		for(int i = EDeviceParameterGoogolGTS_Ext15InputNo0Name; i < EDeviceParameterGoogolGTS_Ext16InputName; ++i)
			m_ppStrExtInputName[14][i - EDeviceParameterGoogolGTS_Ext15InputNo0Name] = m_vctParameterFieldConfigurations[i].strParameterValue;

		for(int i = EDeviceParameterGoogolGTS_Ext16InputNo0Name; i < EDeviceParameterGoogolGTS_Ext1OutputName; ++i)
			m_ppStrExtInputName[15][i - EDeviceParameterGoogolGTS_Ext16InputNo0Name] = m_vctParameterFieldConfigurations[i].strParameterValue;
		
		for(int i = EDeviceParameterGoogolGTS_Ext1OutputNo0Name; i < EDeviceParameterGoogolGTS_Ext2OutputName; ++i)
			m_ppStrExtOutputName[0][i - EDeviceParameterGoogolGTS_Ext1OutputNo0Name] = m_vctParameterFieldConfigurations[i].strParameterValue;

		for(int i = EDeviceParameterGoogolGTS_Ext2OutputNo0Name; i < EDeviceParameterGoogolGTS_Ext3OutputName; ++i)
			m_ppStrExtOutputName[1][i - EDeviceParameterGoogolGTS_Ext2OutputNo0Name] = m_vctParameterFieldConfigurations[i].strParameterValue;

		for(int i = EDeviceParameterGoogolGTS_Ext3OutputNo0Name; i < EDeviceParameterGoogolGTS_Ext4OutputName; ++i)
			m_ppStrExtOutputName[2][i - EDeviceParameterGoogolGTS_Ext3OutputNo0Name] = m_vctParameterFieldConfigurations[i].strParameterValue;

		for(int i = EDeviceParameterGoogolGTS_Ext4OutputNo0Name; i < EDeviceParameterGoogolGTS_Ext5OutputName; ++i)
			m_ppStrExtOutputName[3][i - EDeviceParameterGoogolGTS_Ext4OutputNo0Name] = m_vctParameterFieldConfigurations[i].strParameterValue;

		for(int i = EDeviceParameterGoogolGTS_Ext5OutputNo0Name; i < EDeviceParameterGoogolGTS_Ext6OutputName; ++i)
			m_ppStrExtOutputName[4][i - EDeviceParameterGoogolGTS_Ext5OutputNo0Name] = m_vctParameterFieldConfigurations[i].strParameterValue;

		for(int i = EDeviceParameterGoogolGTS_Ext6OutputNo0Name; i < EDeviceParameterGoogolGTS_Ext7OutputName; ++i)
			m_ppStrExtOutputName[5][i - EDeviceParameterGoogolGTS_Ext6OutputNo0Name] = m_vctParameterFieldConfigurations[i].strParameterValue;

		for(int i = EDeviceParameterGoogolGTS_Ext7OutputNo0Name; i < EDeviceParameterGoogolGTS_Ext8OutputName; ++i)
			m_ppStrExtOutputName[6][i - EDeviceParameterGoogolGTS_Ext7OutputNo0Name] = m_vctParameterFieldConfigurations[i].strParameterValue;

		for(int i = EDeviceParameterGoogolGTS_Ext8OutputNo0Name; i < EDeviceParameterGoogolGTS_Ext9OutputName; ++i)
			m_ppStrExtOutputName[7][i - EDeviceParameterGoogolGTS_Ext8OutputNo0Name] = m_vctParameterFieldConfigurations[i].strParameterValue;

		for(int i = EDeviceParameterGoogolGTS_Ext9OutputNo0Name; i < EDeviceParameterGoogolGTS_Ext10OutputName; ++i)
			m_ppStrExtOutputName[8][i - EDeviceParameterGoogolGTS_Ext9OutputNo0Name] = m_vctParameterFieldConfigurations[i].strParameterValue;

		for(int i = EDeviceParameterGoogolGTS_Ext10OutputNo0Name; i < EDeviceParameterGoogolGTS_Ext11OutputName; ++i)
			m_ppStrExtOutputName[9][i - EDeviceParameterGoogolGTS_Ext10OutputNo0Name] = m_vctParameterFieldConfigurations[i].strParameterValue;

		for(int i = EDeviceParameterGoogolGTS_Ext11OutputNo0Name; i < EDeviceParameterGoogolGTS_Ext12OutputName; ++i)
			m_ppStrExtOutputName[10][i - EDeviceParameterGoogolGTS_Ext11OutputNo0Name] = m_vctParameterFieldConfigurations[i].strParameterValue;

		for(int i = EDeviceParameterGoogolGTS_Ext12OutputNo0Name; i < EDeviceParameterGoogolGTS_Ext13OutputName; ++i)
			m_ppStrExtOutputName[11][i - EDeviceParameterGoogolGTS_Ext12OutputNo0Name] = m_vctParameterFieldConfigurations[i].strParameterValue;

		for(int i = EDeviceParameterGoogolGTS_Ext13OutputNo0Name; i < EDeviceParameterGoogolGTS_Ext14OutputName; ++i)
			m_ppStrExtOutputName[12][i - EDeviceParameterGoogolGTS_Ext13OutputNo0Name] = m_vctParameterFieldConfigurations[i].strParameterValue;

		for(int i = EDeviceParameterGoogolGTS_Ext14OutputNo0Name; i < EDeviceParameterGoogolGTS_Ext15OutputName; ++i)
			m_ppStrExtOutputName[13][i - EDeviceParameterGoogolGTS_Ext14OutputNo0Name] = m_vctParameterFieldConfigurations[i].strParameterValue;

		for(int i = EDeviceParameterGoogolGTS_Ext15OutputNo0Name; i < EDeviceParameterGoogolGTS_Ext16OutputName; ++i)
			m_ppStrExtOutputName[14][i - EDeviceParameterGoogolGTS_Ext15OutputNo0Name] = m_vctParameterFieldConfigurations[i].strParameterValue;

		for(int i = EDeviceParameterGoogolGTS_Ext16OutputNo0Name; i < EDeviceParameterGoogolGTS_Count; ++i)
			m_ppStrExtOutputName[15][i - EDeviceParameterGoogolGTS_Ext16OutputNo0Name] = m_vctParameterFieldConfigurations[i].strParameterValue;

		bool bShowDlg = false;

		for(int i = 0; i < GTS_EXTIO_PORT; ++i)
			bShowDlg |= _ttoi(m_vctParameterFieldConfigurations[EDeviceParameterGoogolGTS_ShowExt1 + i].strParameterValue);

		UpdateParameterList();

		bReturn = true;
	}
	while(false);

	return bReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetInitializeType(EGoogolGtsInitializeType* pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = (EGoogolGtsInitializeType)_ttoi(GetParamValue(EDeviceParameterGoogolGTS_InitializeType));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetInitializeType(EGoogolGtsInitializeType eParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_InitializeType;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EGoogolGtsSetFunction_InitializedDeviceError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], g_lpszGoogolGTSInitializeType[lPreValue], g_lpszGoogolGTSInitializeType[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetLoadFilePath(CString * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}
		
		*pParam = GetParamValue(EDeviceParameterGoogolGTS_LoadfilePath);

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetLoadFilePath(CString strParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_LoadfilePath;

	CString strPreValuse = (GetParamValue(eSaveID));	
		
	do
	{
		if(IsInitialized())
		{
			eReturn = EGoogolGtsSetFunction_InitializedDeviceError;
			break;
		}

		CString strData;
		strData.Format(_T("%s"), strParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], strPreValuse, strParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetLoadExtFilePath(CString * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = GetParamValue(EDeviceParameterGoogolGTS_LoadExtFilePath);

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetLoadExtFilePath(CString strParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_LoadExtFilePath;

	CString strPreValuse = (GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EGoogolGtsSetFunction_InitializedDeviceError;
			break;
		}

		CString strData;
		strData.Format(_T("%s"), strParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], strPreValuse, strParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetShowExternalIO1(bool* pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_ShowExt1));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetShowExternalIO1(bool bParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_ShowExt1;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], g_lpszGoogolGTSSwitch[lPreValue], g_lpszGoogolGTSSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetShowExternalIO2(bool* pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_ShowExt2));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetShowExternalIO2(bool bParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_ShowExt2;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], g_lpszGoogolGTSSwitch[lPreValue], g_lpszGoogolGTSSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetShowExternalIO3(bool* pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_ShowExt3));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetShowExternalIO3(bool bParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_ShowExt3;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], g_lpszGoogolGTSSwitch[lPreValue], g_lpszGoogolGTSSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetShowExternalIO4(bool* pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_ShowExt4));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetShowExternalIO4(bool bParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_ShowExt4;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], g_lpszGoogolGTSSwitch[lPreValue], g_lpszGoogolGTSSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetShowExternalIO5(bool* pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_ShowExt5));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetShowExternalIO5(bool bParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_ShowExt5;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], g_lpszGoogolGTSSwitch[lPreValue], g_lpszGoogolGTSSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetShowExternalIO6(bool* pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_ShowExt6));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetShowExternalIO6(bool bParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_ShowExt6;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], g_lpszGoogolGTSSwitch[lPreValue], g_lpszGoogolGTSSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetShowExternalIO7(bool* pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_ShowExt7));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetShowExternalIO7(bool bParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_ShowExt7;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], g_lpszGoogolGTSSwitch[lPreValue], g_lpszGoogolGTSSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetShowExternalIO8(bool* pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_ShowExt8));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetShowExternalIO8(bool bParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_ShowExt8;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], g_lpszGoogolGTSSwitch[lPreValue], g_lpszGoogolGTSSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetShowExternalIO9(bool* pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_ShowExt9));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetShowExternalIO9(bool bParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_ShowExt9;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], g_lpszGoogolGTSSwitch[lPreValue], g_lpszGoogolGTSSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetShowExternalIO10(bool* pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_ShowExt10));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetShowExternalIO10(bool bParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_ShowExt10;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], g_lpszGoogolGTSSwitch[lPreValue], g_lpszGoogolGTSSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetShowExternalIO11(bool* pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_ShowExt11));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetShowExternalIO11(bool bParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_ShowExt11;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], g_lpszGoogolGTSSwitch[lPreValue], g_lpszGoogolGTSSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetShowExternalIO12(bool* pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_ShowExt12));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetShowExternalIO12(bool bParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_ShowExt12;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], g_lpszGoogolGTSSwitch[lPreValue], g_lpszGoogolGTSSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetShowExternalIO13(bool* pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_ShowExt13));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetShowExternalIO13(bool bParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_ShowExt13;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], g_lpszGoogolGTSSwitch[lPreValue], g_lpszGoogolGTSSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetShowExternalIO14(bool* pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_ShowExt14));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetShowExternalIO14(bool bParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_ShowExt14;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], g_lpszGoogolGTSSwitch[lPreValue], g_lpszGoogolGTSSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetShowExternalIO15(bool* pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_ShowExt15));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetShowExternalIO15(bool bParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_ShowExt15;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], g_lpszGoogolGTSSwitch[lPreValue], g_lpszGoogolGTSSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetShowExternalIO16(bool* pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_ShowExt16));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetShowExternalIO16(bool bParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_ShowExt16;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], g_lpszGoogolGTSSwitch[lPreValue], g_lpszGoogolGTSSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis1Band(long * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis1Band));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis1Band(long lParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis1Band;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EGoogolGtsSetFunction_InitializedDeviceError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), lParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], lPreValue, lParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis1Time(long * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis1Time));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis1Time(long lParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis1Time;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EGoogolGtsSetFunction_InitializedDeviceError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), lParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], lPreValue, lParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis1DirAndPos(long* pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis1DirAndPos));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis1DirAndPos(long lParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis1DirAndPos;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%d"), lParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], lPreValue, lParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis1Velocity(double * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterGoogolGTS_Axis1Velocity));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis1Velocity(double dblParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis1Velocity;

	double fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], fPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis1Accel(double * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterGoogolGTS_Axis1Accel));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis1Accel(double dblParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis1Accel;

	double fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], fPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis1Offset(long* pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis1Offset));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis1Offset(long lParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis1Offset;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%d"), lParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], lPreValue, lParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis1AbruptStop(double * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis1AbruptStop));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis1AbruptStop(double dblParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis1AbruptStop;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EGoogolGtsSetFunction_InitializedDeviceError;
			break;
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis1EStop(double * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis1EStop));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis1EStop(double dblParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis1EStop;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EGoogolGtsSetFunction_InitializedDeviceError;
			break;
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis2Band(long * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis2Band));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis2Band(long lParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis2Band;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EGoogolGtsSetFunction_InitializedDeviceError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), lParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], lPreValue, lParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis2Time(long * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis2Time));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis2Time(long lParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis2Time;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EGoogolGtsSetFunction_InitializedDeviceError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), lParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], lPreValue, lParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis2DirAndPos(long* pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis2DirAndPos));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis2DirAndPos(long lParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis2DirAndPos;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%d"), lParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], lPreValue, lParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis2Velocity(double * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterGoogolGTS_Axis2Velocity));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis2Velocity(double dblParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis2Velocity;

	double fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], fPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis2Accel(double * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterGoogolGTS_Axis2Accel));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis2Accel(double dblParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis2Accel;

	double fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], fPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis2Offset(long* pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis2Offset));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis2Offset(long lParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis2Offset;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%d"), lParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], lPreValue, lParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis2AbruptStop(double * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis2AbruptStop));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis2AbruptStop(double dblParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis2AbruptStop;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EGoogolGtsSetFunction_InitializedDeviceError;
			break;
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis2EStop(double * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis2EStop));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis2EStop(double dblParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis2EStop;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EGoogolGtsSetFunction_InitializedDeviceError;
			break;
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis3Band(long * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis3Band));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis3Band(long lParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis3Band;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EGoogolGtsSetFunction_InitializedDeviceError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), lParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], lPreValue, lParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis3Time(long * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis3Time));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis3Time(long lParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis3Time;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EGoogolGtsSetFunction_InitializedDeviceError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), lParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], lPreValue, lParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis3DirAndPos(long* pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis3DirAndPos));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis3DirAndPos(long lParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis3DirAndPos;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%d"), lParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], lPreValue, lParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis3Velocity(double * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterGoogolGTS_Axis3Velocity));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis3Velocity(double dblParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis3Velocity;

	double fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], fPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis3Accel(double * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterGoogolGTS_Axis3Accel));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis3Accel(double dblParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis3Accel;

	double fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], fPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis3Offset(long* pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis3Offset));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis3Offset(long lParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis3Offset;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%d"), lParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], lPreValue, lParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis3AbruptStop(double * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis3AbruptStop));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis3AbruptStop(double dblParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis3AbruptStop;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EGoogolGtsSetFunction_InitializedDeviceError;
			break;
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis3EStop(double * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis3EStop));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis3EStop(double dblParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis3EStop;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EGoogolGtsSetFunction_InitializedDeviceError;
			break;
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis4Band(long * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis4Band));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis4Band(long lParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis4Band;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EGoogolGtsSetFunction_InitializedDeviceError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), lParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], lPreValue, lParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis4Time(long * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis4Time));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis4Time(long lParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis4Time;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EGoogolGtsSetFunction_InitializedDeviceError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), lParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], lPreValue, lParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis4DirAndPos(long* pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis4DirAndPos));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis4DirAndPos(long lParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis4DirAndPos;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%d"), lParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], lPreValue, lParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis4Velocity(double * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterGoogolGTS_Axis4Velocity));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis4Velocity(double dblParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis4Velocity;

	double fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], fPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis4Accel(double * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterGoogolGTS_Axis4Accel));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis4Accel(double dblParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis4Accel;

	double fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], fPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis4Offset(long* pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis4Offset));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis4Offset(long lParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis4Offset;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%d"), lParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], lPreValue, lParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis4AbruptStop(double * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis4AbruptStop));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis4AbruptStop(double dblParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis4AbruptStop;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EGoogolGtsSetFunction_InitializedDeviceError;
			break;
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis4EStop(double * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis4EStop));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis4EStop(double dblParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis4EStop;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EGoogolGtsSetFunction_InitializedDeviceError;
			break;
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis5Band(long * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis5Band));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis5Band(long lParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis5Band;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EGoogolGtsSetFunction_InitializedDeviceError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), lParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], lPreValue, lParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis5Time(long * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis5Time));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis5Time(long lParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis5Time;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EGoogolGtsSetFunction_InitializedDeviceError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), lParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], lPreValue, lParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis5DirAndPos(long* pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis5DirAndPos));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis5DirAndPos(long lParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis5DirAndPos;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%d"), lParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], lPreValue, lParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis5Velocity(double * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterGoogolGTS_Axis5Velocity));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis5Velocity(double dblParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis5Velocity;

	double fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], fPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis5Accel(double * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterGoogolGTS_Axis5Accel));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis5Accel(double dblParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis5Accel;

	double fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], fPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis5Offset(long* pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis5Offset));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis5Offset(long lParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis5Offset;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%d"), lParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], lPreValue, lParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis5AbruptStop(double * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis5AbruptStop));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis5AbruptStop(double dblParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis5AbruptStop;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EGoogolGtsSetFunction_InitializedDeviceError;
			break;
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis5EStop(double * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis5EStop));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis5EStop(double dblParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis5EStop;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EGoogolGtsSetFunction_InitializedDeviceError;
			break;
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis6Band(long * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis6Band));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis6Band(long lParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis6Band;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EGoogolGtsSetFunction_InitializedDeviceError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), lParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], lPreValue, lParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis6Time(long * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis6Time));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis6Time(long lParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis6Time;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EGoogolGtsSetFunction_InitializedDeviceError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), lParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], lPreValue, lParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis6DirAndPos(long* pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis6DirAndPos));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis6DirAndPos(long lParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis6DirAndPos;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%d"), lParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], lPreValue, lParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis6Velocity(double * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterGoogolGTS_Axis6Velocity));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis6Velocity(double dblParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis6Velocity;

	double fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], fPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis6Accel(double * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterGoogolGTS_Axis6Accel));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis6Accel(double dblParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis6Accel;

	double fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], fPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis6Offset(long* pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis6Offset));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis6Offset(long lParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis6Offset;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%d"), lParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], lPreValue, lParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis6AbruptStop(double * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis6AbruptStop));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis6AbruptStop(double dblParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis6AbruptStop;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EGoogolGtsSetFunction_InitializedDeviceError;
			break;
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis6EStop(double * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis6EStop));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis6EStop(double dblParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis6EStop;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EGoogolGtsSetFunction_InitializedDeviceError;
			break;
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis7Band(long * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis7Band));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis7Band(long lParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis7Band;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EGoogolGtsSetFunction_InitializedDeviceError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), lParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], lPreValue, lParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis7Time(long * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis7Time));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis7Time(long lParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis7Time;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EGoogolGtsSetFunction_InitializedDeviceError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), lParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], lPreValue, lParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis7DirAndPos(long* pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis7DirAndPos));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis7DirAndPos(long lParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis7DirAndPos;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%d"), lParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], lPreValue, lParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis7Velocity(double * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterGoogolGTS_Axis7Velocity));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis7Velocity(double dblParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis7Velocity;

	double fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], fPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis7Accel(double * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterGoogolGTS_Axis7Accel));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis7Accel(double dblParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis7Accel;

	double fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], fPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis7Offset(long* pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis7Offset));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis7Offset(long lParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis7Offset;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%d"), lParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], lPreValue, lParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis7AbruptStop(double * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis7AbruptStop));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis7AbruptStop(double dblParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis7AbruptStop;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EGoogolGtsSetFunction_InitializedDeviceError;
			break;
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis7EStop(double * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis7EStop));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis7EStop(double dblParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis7EStop;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EGoogolGtsSetFunction_InitializedDeviceError;
			break;
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis8Band(long * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis8Band));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis8Band(long lParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis8Band;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EGoogolGtsSetFunction_InitializedDeviceError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), lParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], lPreValue, lParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis8Time(long * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis8Time));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis8Time(long lParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis8Time;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EGoogolGtsSetFunction_InitializedDeviceError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), lParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], lPreValue, lParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis8DirAndPos(long* pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis8DirAndPos));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis8DirAndPos(long lParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis8DirAndPos;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%d"), lParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], lPreValue, lParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis8Velocity(double * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterGoogolGTS_Axis8Velocity));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis8Velocity(double dblParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis8Velocity;

	double fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], fPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis8Accel(double * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterGoogolGTS_Axis8Accel));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis8Accel(double dblParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis8Accel;

	double fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], fPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis8Offset(long* pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis8Offset));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis8Offset(long lParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis8Offset;

	long lPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%d"), lParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], lPreValue, lParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis8AbruptStop(double * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis8AbruptStop));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis8AbruptStop(double dblParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis8AbruptStop;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EGoogolGtsSetFunction_InitializedDeviceError;
			break;
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGoogolGtsGetFunction CDeviceGoogolGTS::GetAxis8EStop(double * pParam)
{
	EGoogolGtsGetFunction eReturn = EGoogolGtsGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGoogolGtsGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGoogolGTS_Axis8EStop));

		eReturn = EGoogolGtsGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EGoogolGtsSetFunction CDeviceGoogolGTS::SetAxis8EStop(double dblParam)
{
	EGoogolGtsSetFunction eReturn = EGoogolGtsSetFunction_UnknownError;

	EDeviceParameterGoogolGTS eSaveID = EDeviceParameterGoogolGTS_Axis8EStop;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EGoogolGtsSetFunction_InitializedDeviceError;
			break;
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EGoogolGtsSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EGoogolGtsSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceGoogolGTS::GetServoStatus()
{
	bool bReturn = false;

	do
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(EDeviceAddControlID_ComboAxisSelect);
		if(!pCombo)
			break;

		int nSelected = pCombo->GetCurSel();

		bReturn = GetServoStatus(nSelected);
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::SetServoOn(bool bOn)
{
	bool bReturn = false;

	do
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(EDeviceAddControlID_ComboAxisSelect);
		if(!pCombo)
			break;

		int nSelected = pCombo->GetCurSel();

		bReturn = SetServoOn(nSelected, bOn);
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::GetCommandPosition(double* pPos)
{
	bool bReturn = false;

	do
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(EDeviceAddControlID_ComboAxisSelect);
		if(!pCombo)
			break;

		int nSelected = pCombo->GetCurSel();

		bReturn = GetCommandPosition(nSelected, pPos);
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::SetCommandPositionClear()
{
	bool bReturn = false;

	do
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(EDeviceAddControlID_ComboAxisSelect);
		if(!pCombo)
			break;

		int nSelected = pCombo->GetCurSel();

		bReturn = SetCommandPositionClear(nSelected);
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::GetActualPosition(double * pPos)
{
	bool bReturn = false;

	do
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(EDeviceAddControlID_ComboAxisSelect);
		if(!pCombo)
			break;

		int nSelected = pCombo->GetCurSel();

		bReturn = GetActualPosition(nSelected, pPos);
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::SetActualPositionClear()
{
	bool bReturn = false;

	do
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(EDeviceAddControlID_ComboAxisSelect);
		if(!pCombo)
			break;

		int nSelected = pCombo->GetCurSel();

		bReturn = SetActualPositionClear(nSelected);
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::GetAlarmStatus()
{
	bool bReturn = false;

	do
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(EDeviceAddControlID_ComboAxisSelect);
		if(!pCombo)
			break;

		int nSelected = pCombo->GetCurSel();

		bReturn = GetAlarmStatus(nSelected);
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::SetAlarmClear()
{
	bool bReturn = false;

	do
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(EDeviceAddControlID_ComboAxisSelect);
		if(!pCombo)
			break;

		int nSelected = pCombo->GetCurSel();

		bReturn = SetAlarmClear(nSelected);
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::GetInposition()
{
	bool bReturn = false;

	do
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(EDeviceAddControlID_ComboAxisSelect);
		if(!pCombo)
			break;

		int nSelected = pCombo->GetCurSel();

		bReturn = GetInposition(nSelected);
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::GetLimitSensorN()
{
	bool bReturn = false;

	do
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(EDeviceAddControlID_ComboAxisSelect);
		if(!pCombo)
			break;

		int nSelected = pCombo->GetCurSel();

		bReturn = GetLimitSensorN(nSelected);
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::GetLimitSensorP()
{
	bool bReturn = false;

	do
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(EDeviceAddControlID_ComboAxisSelect);
		if(!pCombo)
			break;

		int nSelected = pCombo->GetCurSel();

		bReturn = GetLimitSensorP(nSelected);
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::GetHomeSensor()
{
	bool bReturn = false;

	do
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(EDeviceAddControlID_ComboAxisSelect);
		if(!pCombo)
			break;

		int nSelected = pCombo->GetCurSel();

		bReturn = GetHomeSensor(nSelected);
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::MovePosition(double dPos, double dVel, double dAcc, bool bSCurve)
{
	bool bReturn = false;

	do
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(EDeviceAddControlID_ComboAxisSelect);
		if(!pCombo)
			break;

		int nSelected = pCombo->GetCurSel();

		bReturn = MovePosition(nSelected, dPos, dVel, dAcc, bSCurve);
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::MoveDistance(double dDist, double dVel, double dAcc, bool bSCurve)
{
	bool bReturn = false;

	do
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(EDeviceAddControlID_ComboAxisSelect);
		if(!pCombo)
			break;

		int nSelected = pCombo->GetCurSel();

		bReturn = MoveDistance(nSelected, dDist, dVel, dAcc, bSCurve);
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::MoveVelocity(double dVel, double dAcc, bool bSCurve)
{
	return MoveJog(dVel, dAcc, bSCurve);
}

bool CDeviceGoogolGTS::MoveJog(double dVel, double dAcl, BOOL bSCurve)
{
	bool bReturn = false;

	do
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(EDeviceAddControlID_ComboAxisSelect);
		if(!pCombo)
			break;

		int nSelected = pCombo->GetCurSel();

		bReturn = MoveJog(nSelected, dVel, dAcl, bSCurve);
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::StopJog()
{
	bool bReturn = false;

	do
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(EDeviceAddControlID_ComboAxisSelect);
		if(!pCombo)
			break;

		int nSelected = pCombo->GetCurSel();

		bReturn = StopJog(nSelected);
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::IsMotionDone()
{
	bool bReturn = false;

	do
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(EDeviceAddControlID_ComboAxisSelect);
		if(!pCombo)
			break;

		int nSelected = pCombo->GetCurSel();

		bReturn = IsMotionDone(nSelected);
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::MotorStop()
{
	bool bReturn = false;

	do
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(EDeviceAddControlID_ComboAxisSelect);
		if(!pCombo)
			break;

		int nSelected = pCombo->GetCurSel();

		bReturn = MotorStop(nSelected);
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::MotorEStop()
{
	bool bReturn = false;

	do
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(EDeviceAddControlID_ComboAxisSelect);
		if(!pCombo)
			break;

		int nSelected = pCombo->GetCurSel();

		bReturn = MotorEStop(nSelected);
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::ChangeSpeed(double dSpeed)
{
	bool bReturn = false;

	do
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(EDeviceAddControlID_ComboAxisSelect);
		if(!pCombo)
			break;

		int nSelected = pCombo->GetCurSel();

		bReturn = ChangeSpeed(nSelected, dSpeed);
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::WriteGenOutBit(int nBit, bool bOn)
{
	bool bReturn = false;
	
	do
	{
		if(!IsInitialized())
			break;

		if(nBit < 0 || nBit >= GTS_DIO_OUT_PORT)
			break;

		short sValue = !bOn;

		if(GT_SetDoBit(_ttoi(GetDeviceID()), MC_GPO, nBit + 1, sValue))
			break;

		if(bOn)
			m_dwOutStatus |= (int)pow((double)2, nBit);
		else
			m_dwOutStatus &= ~(int)pow((double)2, nBit);

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::ReadGenOutBit(int nBit)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nBit >= GTS_DIO_OUT_PORT)
			break;

		bReturn = (m_dwOutStatus >> nBit) & 0x01;
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::ReadGenInputBit(int nBit)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nBit < 0 || nBit >= GTS_DIO_IN_PORT)
			break;

		long nValue = 0;

		if(GT_GetDi(_ttoi(GetDeviceID()), MC_GPI, &nValue))
			break;

		bReturn = (nValue >> nBit) & 0x01;

		bReturn = !bReturn;
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::WriteGenExtOutBit(int nChannel, int nBit, bool bOn)
{
	bool bReturn = false;

	do 
	{
		if(!m_pDlgExtDioViewer)
			break;
		
		bReturn = (m_pDlgExtDioViewer + nChannel)->WriteGenExtOutBit(nBit, bOn);
	} 
	while(false);
	
	return bReturn;
}

bool CDeviceGoogolGTS::ReadGenExtOutBit(int nChannel, int nBit)
{
	bool bReturn = false;

	do
	{
		if(!m_pDlgExtDioViewer)
			break;

		bReturn = (m_pDlgExtDioViewer + nChannel)->ReadGenExtOutBit(nBit);
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::ReadGenExtInputBit(int nChannel, int nBit)
{
	bool bReturn = false;

	do
	{
		if(!m_pDlgExtDioViewer)
			break;

		bReturn = (m_pDlgExtDioViewer + nChannel)->ReadGenExtInputBit(nBit);
	}
	while(false);

	return bReturn;
}

void CDeviceGoogolGTS::ClearExtOutport()
{
	do
	{
		if(!IsInitialized())
			break;

		for(int i = 0; i < GTS_EXTIO_MODULES; ++i)
		{
			(m_pDlgExtDioViewer + i)->ClearExtOutport();
		}
	}
	while (false);
}

bool CDeviceGoogolGTS::MoveToHome()
{
	bool bReturn = false;

	do
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(EDeviceAddControlID_ComboAxisSelect);
		if(!pCombo)
			break;

		int nSelected = pCombo->GetCurSel();

		bReturn = MoveToHome(nSelected);
	}
	while(false);

	return bReturn;
}

EDeviceMotionHommingStatus CDeviceGoogolGTS::GetHommingStatus()
{
	EDeviceMotionHommingStatus eStatus = EDeviceMotionHommingStatus_Error;

	do
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(EDeviceAddControlID_ComboAxisSelect);
		if(!pCombo)
			break;

		int nSelected = pCombo->GetCurSel();

		eStatus = GetHommingStatus(nSelected);
	}
	while(false);

	return eStatus;
}

bool CDeviceGoogolGTS::GetServoStatus(int nAxis)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nAxis > GTS_MOTION_MAX)
			break;

		long nStatus = 0;

		if(GT_GetSts(_ttoi(GetDeviceID()), nAxis + 1, &nStatus))
			break;

		if(nStatus & eMotionStatus_MotorActivation)
			bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::SetServoOn(int nAxis, bool bOn)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nAxis > GTS_MOTION_MAX)
			break;

		if(bOn)
		{
			if(GT_AxisOn(_ttoi(GetDeviceID()), nAxis + 1))
				break;
		}
		else
		{
			if(GT_AxisOff(_ttoi(GetDeviceID()), nAxis + 1))
				break;
		}

		m_sMotionInfo[nAxis].bServo = bOn;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::GetCommandPosition(int nAxis, double * pPos)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nAxis > GTS_MOTION_MAX)
			break;

		if(!pPos)
			break;

		double dblPos = 0.;

		if(GT_GetPrfPos(_ttoi(GetDeviceID()), nAxis + 1, &dblPos))
			break;

		*pPos = dblPos;

		bReturn = true;

	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::SetCommandPositionClear(int nAxis)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nAxis > GTS_MOTION_MAX)
			break;

		if(GT_SetPrfPos(_ttoi(GetDeviceID()), nAxis + 1, 0))
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::GetActualPosition(int nAxis, double * pPos)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nAxis > GTS_MOTION_MAX)
			break;

		if(!pPos)
			break;

		double dblPos = 0;

		if(GT_GetEncPos(_ttoi(GetDeviceID()), nAxis + 1, &dblPos))
			break;

		*pPos = dblPos;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::SetActualPositionClear(int nAxis)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nAxis > GTS_MOTION_MAX)
			break;

		if(GT_SetEncPos(_ttoi(GetDeviceID()), nAxis + 1, 0))
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::GetAlarmStatus(int nAxis)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nAxis > GTS_MOTION_MAX)
			break;

		long nStatus = 0;

		if(GT_GetSts(_ttoi(GetDeviceID()), nAxis + 1, &nStatus))
			break;

		if((nStatus & eMotionStatus_ServoAlarm) || (nStatus & eMotionStatus_MotionError))
			bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::SetAlarmClear(int nAxis)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nAxis > GTS_MOTION_MAX)
			break;

		if(GT_ClrSts(_ttoi(GetDeviceID()), nAxis + 1))
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::GetInposition(int nAxis)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nAxis > GTS_MOTION_MAX)
			break;

		long nStatus = 0;

		if(GT_GetSts(_ttoi(GetDeviceID()), nAxis + 1, &nStatus))
			break;

		if((nStatus & eMotionStatus_Inposition))
			bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::GetLimitSensorN(int nAxis)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nAxis > GTS_MOTION_MAX)
			break;

		long nValue = 0;

		if(GT_GetDi(_ttoi(GetDeviceID()), MC_LIMIT_NEGATIVE, &nValue))
			break;

		if(((nValue >> (nAxis)) & 0x01))
			bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::GetLimitSensorP(int nAxis)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nAxis > GTS_MOTION_MAX)
			break;

		long nValue = 0;

		if(GT_GetDi(_ttoi(GetDeviceID()), MC_LIMIT_POSITIVE, &nValue))
			break;

		if(((nValue >> (nAxis)) & 0x01))
			bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::GetHomeSensor(int nAxis)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nAxis > GTS_MOTION_MAX)
			break;

		long nValue = 0;

		if(GT_GetDi(_ttoi(GetDeviceID()), MC_HOME, &nValue))
			break;

		if(((nValue >> (nAxis)) & 0x01))
			bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::MovePosition(int nAxis, double dPos, double dVel, double dAcc, bool bSCurve)
{
	bool bReturn = false;

	do 
	{
		if(!IsInitialized())
			break;

		if(nAxis > GTS_MOTION_MAX)
			break;

		m_sMotionInfo[nAxis].dblLastVel = dVel;

		if(GT_PrfTrap(_ttoi(GetDeviceID()), nAxis + 1))
			break;

		SetAlarmClear(nAxis);

		TTrapPrm trapPrm;
		ZeroMemory(&trapPrm, sizeof(trapPrm));

		if(GT_GetTrapPrm(_ttoi(GetDeviceID()), nAxis + 1, &trapPrm))
			break;

		trapPrm.acc = dAcc;
		trapPrm.dec = dAcc;

		if(GT_SetTrapPrm(_ttoi(GetDeviceID()), nAxis + 1, &trapPrm))
			break;

		if(GT_SetVel(_ttoi(GetDeviceID()), nAxis + 1, dVel))
			break;

		if(GT_SetPos(_ttoi(GetDeviceID()), nAxis + 1, dPos))
			break;

		if(GT_Update(_ttoi(GetDeviceID()), 1 << (nAxis)))
			break;

		m_sMotionInfo[nAxis].dblPos = dPos;
		m_sMotionInfo[nAxis].dblVel = dVel;
		m_sMotionInfo[nAxis].dblAcl = dAcc;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::MoveDistance(int nAxis, double dDist, double dVel, double dAcc, bool bSCurve)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nAxis > GTS_MOTION_MAX)
			break;

		SetAlarmClear(nAxis);

		m_sMotionInfo[nAxis].dblLastVel = dVel;

		if(GT_PrfTrap(_ttoi(GetDeviceID()), nAxis + 1))
			break;

		TTrapPrm trapPrm;
		ZeroMemory(&trapPrm, sizeof(trapPrm));

		if(GT_GetTrapPrm(_ttoi(GetDeviceID()), nAxis + 1, &trapPrm))
			break;

		trapPrm.acc = dAcc;
		trapPrm.dec = dAcc;

		if(GT_SetTrapPrm(_ttoi(GetDeviceID()), nAxis + 1, &trapPrm))
			break;

		if(GT_SetVel(_ttoi(GetDeviceID()), nAxis + 1, dVel))
			break;

		long nPos = 0;

		if(GT_GetPos(_ttoi(GetDeviceID()), nAxis + 1, &nPos))
			break;

		if(GT_SetPos(_ttoi(GetDeviceID()), nAxis + 1, nPos + dDist))
			break;

		if(GT_Update(_ttoi(GetDeviceID()), 1 << (nAxis)))
			break;

		m_sMotionInfo[nAxis].dblDis = dDist;
		m_sMotionInfo[nAxis].dblVel = dVel;
		m_sMotionInfo[nAxis].dblAcl = dAcc;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::MoveVelocity(int nAxis, double dVel, double dAcc, bool bSCurve)
{
	bool bReturn = false;

	do 
	{
		if(!MoveJog(nAxis, dVel, dAcc, bSCurve))
			break;

		bReturn = true;
	} 
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::MoveJog(int nAxis, double dVel, double dAcl, BOOL bSCurve)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nAxis > GTS_MOTION_MAX)
			break;

		SetAlarmClear(nAxis);

		m_sMotionInfo[nAxis].dblLastVel = dVel;

		if(GT_PrfJog(_ttoi(GetDeviceID()), nAxis + 1))
			break;

		TJogPrm jog;
		ZeroMemory(&jog, sizeof(jog));

		if(GT_GetJogPrm(_ttoi(GetDeviceID()), nAxis + 1, &jog))
			break;

		jog.acc = dAcl;
		jog.dec = dAcl;

		if(GT_SetJogPrm(_ttoi(GetDeviceID()), nAxis + 1, &jog))
			break;

		if(GT_SetVel(_ttoi(GetDeviceID()), nAxis + 1, dVel))
			break;

		if(GT_Update(_ttoi(GetDeviceID()), 1 << (nAxis)))
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::StopJog(int nAxis)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nAxis > GTS_MOTION_MAX)
			break;

		double dblAbruptstop = 0.;

		switch(nAxis)
		{
		case 0:
			GetAxis1AbruptStop(&dblAbruptstop);
			break;
		case 1:
			GetAxis2AbruptStop(&dblAbruptstop);
			break;
		case 2:
			GetAxis3AbruptStop(&dblAbruptstop);
			break;
		case 3:
			GetAxis4AbruptStop(&dblAbruptstop);
			break;
		case 4:
			GetAxis5AbruptStop(&dblAbruptstop);
			break;
		case 5:
			GetAxis6AbruptStop(&dblAbruptstop);
			break;
		case 6:
			GetAxis7AbruptStop(&dblAbruptstop);
			break;
		case 7:
			GetAxis8AbruptStop(&dblAbruptstop);
			break;
		default:
			break;
		}

		if(GT_SetStopDec(_ttoi(GetDeviceID()), nAxis + 1, m_sMotionInfo[nAxis].dblLastVel, dblAbruptstop))
			break;

		if(GT_Stop(_ttoi(GetDeviceID()), 1 << (nAxis), 0))
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::IsMotionDone(int nAxis)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nAxis > GTS_MOTION_MAX)
			break;
		
		long nStatus = 0;

		if(GT_GetSts(_ttoi(GetDeviceID()), nAxis + 1, &nStatus))
			break;

		if(!(nStatus & eMotionStatus_ProfileMotion))
			bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::MotorStop(int nAxis)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		double dblAbruptstop = 0.;

		switch(nAxis)
		{
		case 0:
			GetAxis1AbruptStop(&dblAbruptstop);
			break;
		case 1:
			GetAxis2AbruptStop(&dblAbruptstop);
			break;
		case 2:
			GetAxis3AbruptStop(&dblAbruptstop);
			break;
		case 3:
			GetAxis4AbruptStop(&dblAbruptstop);
			break;
		case 4:
			GetAxis5AbruptStop(&dblAbruptstop);
			break;
		case 5:
			GetAxis6AbruptStop(&dblAbruptstop);
			break;
		case 6:
			GetAxis7AbruptStop(&dblAbruptstop);
			break;
		case 7:
			GetAxis8AbruptStop(&dblAbruptstop);
			break;
		default:
			break;
		}

		if(GT_SetStopDec(_ttoi(GetDeviceID()), nAxis + 1, m_sMotionInfo[nAxis].dblLastVel, dblAbruptstop))
			break;

		if(GT_Stop(_ttoi(GetDeviceID()), 1 << (nAxis), 0))
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::MotorEStop(int nAxis)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		double dblAbruptstop = 0.;
		double dblEstop = 0.;

		switch(nAxis)
		{
		case 0:
			GetAxis1EStop(&dblEstop);
			GetAxis1AbruptStop(&dblAbruptstop);
			break;
		case 1:
			GetAxis2EStop(&dblEstop);
			GetAxis2AbruptStop(&dblAbruptstop);
			break;
		case 2:
			GetAxis3EStop(&dblEstop);
			GetAxis3AbruptStop(&dblAbruptstop);
			break;
		case 3:
			GetAxis4EStop(&dblEstop);
			GetAxis4AbruptStop(&dblAbruptstop);
			break;
		case 4:
			GetAxis5EStop(&dblEstop);
			GetAxis5AbruptStop(&dblAbruptstop);
			break;
		case 5:
			GetAxis6EStop(&dblEstop);
			GetAxis6AbruptStop(&dblAbruptstop);
			break;
		case 6:
			GetAxis7EStop(&dblEstop);
			GetAxis7AbruptStop(&dblAbruptstop);
			break;
		case 7:
			GetAxis8EStop(&dblEstop);
			GetAxis8AbruptStop(&dblAbruptstop);
			break;
		default:
			break;
		}

		if(GT_SetStopDec(_ttoi(GetDeviceID()), nAxis + 1, dblEstop, dblAbruptstop))
			break;

		if(GT_Stop(_ttoi(GetDeviceID()), 1 << (nAxis), 1))
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::ChangeSpeed(int nAxis, double dSpeed)
{
	ASSERT(0);

	return false;
}

bool CDeviceGoogolGTS::MoveToHome(int nAxis)
{
	bool bReturn = false;

	do 
	{
		if(!IsInitialized())
			break;

		if(nAxis > GTS_MOTION_MAX)
			break;

		long lPosition = 0.;
		double dblVelocity = 0.;
		double dblAccel = 0.;
		long lOffset = 0.;

		bool bSelectedMotor = false;

		switch(nAxis)
		{
		case 0:
			{
				GetAxis1DirAndPos(&lPosition);
				GetAxis1Velocity(&dblVelocity);
				GetAxis1Accel(&dblAccel);
				GetAxis1Offset(&lOffset);
			}
			break;
		case 1:
			{
				GetAxis2DirAndPos(&lPosition);
				GetAxis2Velocity(&dblVelocity);
				GetAxis2Accel(&dblAccel);
				GetAxis2Offset(&lOffset);
			}
			break;
		case 2:
			{
				GetAxis3DirAndPos(&lPosition);
				GetAxis3Velocity(&dblVelocity);
				GetAxis3Accel(&dblAccel);
				GetAxis3Offset(&lOffset);
			}
			break;
		case 3:
			{
				GetAxis4DirAndPos(&lPosition);
				GetAxis4Velocity(&dblVelocity);
				GetAxis4Accel(&dblAccel);
				GetAxis4Offset(&lOffset);
			}
			break;
		case 4:
			{
				GetAxis5DirAndPos(&lPosition);
				GetAxis5Velocity(&dblVelocity);
				GetAxis5Accel(&dblAccel);
				GetAxis5Offset(&lOffset);
			}
			break;
		case 5:
			{
				GetAxis6DirAndPos(&lPosition);
				GetAxis6Velocity(&dblVelocity);
				GetAxis6Accel(&dblAccel);
				GetAxis6Offset(&lOffset);
			}
			break;
		case 6:
			{
				GetAxis7DirAndPos(&lPosition);
				GetAxis7Velocity(&dblVelocity);
				GetAxis7Accel(&dblAccel);
				GetAxis7Offset(&lOffset);
			}
			break;
		case 7:
			{
				GetAxis8DirAndPos(&lPosition);
				GetAxis8Velocity(&dblVelocity);
				GetAxis8Accel(&dblAccel);
				GetAxis8Offset(&lOffset);
			}
			break;
		default:
			bSelectedMotor = true;
			break;
		}

		if(bSelectedMotor)
			break;
		
		unsigned short shStatus = 0;

		if(GT_HomeSts(_ttoi(GetDeviceID()), nAxis + 1, &shStatus))
			break;

		if(shStatus)
		{
			if(GT_HomeStop(_ttoi(GetDeviceID()), nAxis + 1, lPosition, dblVelocity, dblAccel))
				break;
		}
		else
		{
			if(GT_Home(_ttoi(GetDeviceID()), nAxis + 1, lPosition, dblVelocity, dblAccel, lOffset))
				break;
		}
		

		bReturn = true;
	} 
	while(false);

	return bReturn;
}

EDeviceMotionHommingStatus CDeviceGoogolGTS::GetHommingStatus(int nAxis)
{
	EDeviceMotionHommingStatus eReturn = EDeviceMotionHommingStatus_Error;

	do 
	{
		unsigned short shStatus = 0;

		if(GT_HomeSts(_ttoi(GetDeviceID()), nAxis + 1, &shStatus))
			break;

		switch(shStatus)
		{
		case 0:
			eReturn = EDeviceMotionHommingStatus_Searching;
			break;
		case 1:
		case 2:
			eReturn = EDeviceMotionHommingStatus_Success;
			break;
		default:
			break;
		}			
	} 
	while(false);
	

	return eReturn;
}

bool CDeviceGoogolGTS::ClearOutPort()
{
	bool bReturn = false;

	do
	{
		if (!IsInitialized())
			break;

		bReturn = true;

		for (int i = 0; i < GTS_DIO_OUT_PORT; ++i)
			bReturn &= WriteGenOutBit(i, false);
	} 
	while (false);

	return bReturn;
}

bool CDeviceGoogolGTS::ClearInPort()
{
	return false;
}

long CDeviceGoogolGTS::GetStatus(int nAxis)
{
	long nReturn = 0;

	do
	{
		if(!IsInitialized())
			break;

		long nStatus = 0;

		if(GT_GetSts(_ttoi(GetDeviceID()), nAxis + 1, &nStatus))
			break;

		nReturn = nStatus;
	}
	while(false);

	return nReturn;
}

bool CDeviceGoogolGTS::SetHomeCaptureMode(int nAxis)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(GT_SetCaptureMode(_ttoi(GetDeviceID()), nAxis + 1, CAPTURE_HOME))
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::GetCaptureStatus(int nAxis, double* dblCapturePosition)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		short sCaptureStatus = 0;
		long nCaptuerPosition = 0;

		if(GT_GetCaptureStatus(_ttoi(GetDeviceID()), nAxis + 1, &sCaptureStatus, &nCaptuerPosition))
			break;

		bReturn = (bool)sCaptureStatus;

		if(bReturn)
			*dblCapturePosition = (double)nCaptuerPosition;
	}
	while(false);

	return bReturn;
}

bool CDeviceGoogolGTS::UpdateControls()
{
	return __super::UpdateControls();
}

bool CDeviceGoogolGTS::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;

	bool bFoundID = false;

	bool bUIUpdate = false;

	CString strMessage;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterGoogolGTS_DeviceID:
			{
				bReturn = !SetDeviceID(strValue);
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_InitializeType:
			{
				bReturn = !SetInitializeType((EGoogolGtsInitializeType)_ttoi(strValue));
				bFoundID = true;

				if(bReturn)
					bUIUpdate = true;
			}
			break;
		case EDeviceParameterGoogolGTS_LoadfilePath:
			{
				bReturn = !SetLoadFilePath(strValue);
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_LoadExtFilePath:
			{
				bReturn = !SetLoadExtFilePath(strValue);
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_ShowExt1:
			{
				bReturn = !SetShowExternalIO1(_ttoi(strValue));
				
				if(bReturn)
					bUIUpdate = true;

				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_ShowExt2:
			{
				bReturn = !SetShowExternalIO2(_ttoi(strValue));

				if(bReturn)
					bUIUpdate = true;

				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_ShowExt3:
			{
				bReturn = !SetShowExternalIO3(_ttoi(strValue));

				if(bReturn)
					bUIUpdate = true;

				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_ShowExt4:
			{
				bReturn = !SetShowExternalIO4(_ttoi(strValue));

				if(bReturn)
					bUIUpdate = true;

				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_ShowExt5:
			{
				bReturn = !SetShowExternalIO5(_ttoi(strValue));

				if(bReturn)
					bUIUpdate = true;

				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_ShowExt6:
			{
				bReturn = !SetShowExternalIO6(_ttoi(strValue));

				if(bReturn)
					bUIUpdate = true;

				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_ShowExt7:
			{
				bReturn = !SetShowExternalIO7(_ttoi(strValue));

				if(bReturn)
					bUIUpdate = true;

				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_ShowExt8:
			{
				bReturn = !SetShowExternalIO8(_ttoi(strValue));

				if(bReturn)
					bUIUpdate = true;

				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_ShowExt9:
			{
				bReturn = !SetShowExternalIO9(_ttoi(strValue));

				if(bReturn)
					bUIUpdate = true;

				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_ShowExt10:
			{
				bReturn = !SetShowExternalIO10(_ttoi(strValue));

				if(bReturn)
					bUIUpdate = true;

				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_ShowExt11:
			{
				bReturn = !SetShowExternalIO11(_ttoi(strValue));

				if(bReturn)
					bUIUpdate = true;

				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_ShowExt12:
			{
				bReturn = !SetShowExternalIO12(_ttoi(strValue));

				if(bReturn)
					bUIUpdate = true;

				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_ShowExt13:
			{
				bReturn = !SetShowExternalIO13(_ttoi(strValue));

				if(bReturn)
					bUIUpdate = true;

				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_ShowExt14:
			{
				bReturn = !SetShowExternalIO14(_ttoi(strValue));

				if(bReturn)
					bUIUpdate = true;

				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_ShowExt15:
			{
				bReturn = !SetShowExternalIO15(_ttoi(strValue));

				if(bReturn)
					bUIUpdate = true;

				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_ShowExt16:
			{
				bReturn = !SetShowExternalIO16(_ttoi(strValue));

				if(bReturn)
					bUIUpdate = true;

				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis1Band:
			{
				bReturn = !SetAxis1Band(_ttoi(strValue));

				if(bReturn)
					bUIUpdate = true;

				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis1Time:
			{
				bReturn = !SetAxis1Time(_ttoi(strValue));

				if(bReturn)
					bUIUpdate = true;

				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis1DirAndPos:
			{
				bReturn = !SetAxis1DirAndPos(_ttoi(strValue));

				if(bReturn)
					bUIUpdate = true;

				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis1Velocity:
			{
				bReturn = !SetAxis1Velocity(_ttof(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis1Accel:
			{
				bReturn = !SetAxis1Accel(_ttof(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis1Offset:
			{
				bReturn = !SetAxis1Offset(_ttoi(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis1AbruptStop:
			{
				bReturn = !SetAxis1AbruptStop(_ttof(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis1EStop:
			{
				bReturn = !SetAxis1EStop(_ttof(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis2Band:
			{
				bReturn = !SetAxis2Band(_ttoi(strValue));

				if(bReturn)
					bUIUpdate = true;

				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis2Time:
			{
				bReturn = !SetAxis2Time(_ttoi(strValue));

				if(bReturn)
					bUIUpdate = true;

				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis2DirAndPos:
			{
				bReturn = !SetAxis2DirAndPos(_ttoi(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis2Velocity:
			{
				bReturn = !SetAxis2Velocity(_ttof(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis2Accel:
			{
				bReturn = !SetAxis2Accel(_ttof(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis2Offset:
			{
				bReturn = !SetAxis2Offset(_ttoi(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis2AbruptStop:
			{
				bReturn = !SetAxis2AbruptStop(_ttof(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis2EStop:
			{
				bReturn = !SetAxis2EStop(_ttof(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis3Band:
			{
				bReturn = !SetAxis3Band(_ttoi(strValue));

				if(bReturn)
					bUIUpdate = true;

				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis3Time:
			{
				bReturn = !SetAxis3Time(_ttoi(strValue));

				if(bReturn)
					bUIUpdate = true;

				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis3DirAndPos:
			{
				bReturn = !SetAxis3DirAndPos(_ttoi(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis3Velocity:
			{
				bReturn = !SetAxis3Velocity(_ttof(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis3Accel:
			{
				bReturn = !SetAxis3Accel(_ttof(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis3Offset:
			{
				bReturn = !SetAxis3Offset(_ttoi(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis3AbruptStop:
			{
				bReturn = !SetAxis3AbruptStop(_ttof(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis3EStop:
			{
				bReturn = !SetAxis3EStop(_ttof(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis4Band:
			{
				bReturn = !SetAxis4Band(_ttoi(strValue));

				if(bReturn)
					bUIUpdate = true;

				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis4Time:
			{
				bReturn = !SetAxis4Time(_ttoi(strValue));

				if(bReturn)
					bUIUpdate = true;

				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis4DirAndPos:
			{
				bReturn = !SetAxis4DirAndPos(_ttoi(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis4Velocity:
			{
				bReturn = !SetAxis4Velocity(_ttof(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis4Accel:
			{
				bReturn = !SetAxis4Accel(_ttof(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis4Offset:
			{
				bReturn = !SetAxis4Offset(_ttoi(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis4AbruptStop:
			{
				bReturn = !SetAxis4AbruptStop(_ttof(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis4EStop:
			{
				bReturn = !SetAxis4EStop(_ttof(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis5Band:
			{
				bReturn = !SetAxis5Band(_ttoi(strValue));

				if(bReturn)
					bUIUpdate = true;

				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis5Time:
			{
				bReturn = !SetAxis5Time(_ttoi(strValue));

				if(bReturn)
					bUIUpdate = true;

				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis5DirAndPos:
			{
				bReturn = !SetAxis5DirAndPos(_ttoi(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis5Velocity:
			{
				bReturn = !SetAxis5Velocity(_ttof(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis5Accel:
			{
				bReturn = !SetAxis5Accel(_ttof(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis5Offset:
			{
				bReturn = !SetAxis5Offset(_ttoi(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis5AbruptStop:
			{
				bReturn = !SetAxis5AbruptStop(_ttof(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis5EStop:
			{
				bReturn = !SetAxis5EStop(_ttof(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis6Band:
			{
				bReturn = !SetAxis6Band(_ttoi(strValue));

				if(bReturn)
					bUIUpdate = true;

				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis6Time:
			{
				bReturn = !SetAxis6Time(_ttoi(strValue));

				if(bReturn)
					bUIUpdate = true;

				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis6DirAndPos:
			{
				bReturn = !SetAxis6DirAndPos(_ttoi(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis6Velocity:
			{
				bReturn = !SetAxis6Velocity(_ttof(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis6Accel:
			{
				bReturn = !SetAxis6Accel(_ttof(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis6Offset:
			{
				bReturn = !SetAxis6Offset(_ttoi(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis6AbruptStop:
			{
				bReturn = !SetAxis6AbruptStop(_ttof(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis6EStop:
			{
				bReturn = !SetAxis6EStop(_ttof(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis7Band:
			{
				bReturn = !SetAxis7Band(_ttoi(strValue));

				if(bReturn)
					bUIUpdate = true;

				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis7Time:
			{
				bReturn = !SetAxis7Time(_ttoi(strValue));

				if(bReturn)
					bUIUpdate = true;

				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis7DirAndPos:
			{
				bReturn = !SetAxis7DirAndPos(_ttoi(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis7Velocity:
			{
				bReturn = !SetAxis7Velocity(_ttof(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis7Accel:
			{
				bReturn = !SetAxis7Accel(_ttof(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis7Offset:
			{
				bReturn = !SetAxis7Offset(_ttoi(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis7AbruptStop:
			{
				bReturn = !SetAxis7AbruptStop(_ttof(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis7EStop:
			{
				bReturn = !SetAxis7EStop(_ttof(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis8Band:
			{
				bReturn = !SetAxis8Band(_ttoi(strValue));

				if(bReturn)
					bUIUpdate = true;

				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis8Time:
			{
				bReturn = !SetAxis8Time(_ttoi(strValue));

				if(bReturn)
					bUIUpdate = true;

				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis8DirAndPos:
			{
				bReturn = !SetAxis8DirAndPos(_ttoi(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis8Velocity:
			{
				bReturn = !SetAxis8Velocity(_ttof(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis8Accel:
			{
				bReturn = !SetAxis8Accel(_ttof(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis8Offset:
			{
				bReturn = !SetAxis8Offset(_ttoi(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis8AbruptStop:
			{
				bReturn = !SetAxis8AbruptStop(_ttof(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_Axis8EStop:
			{
				bReturn = !SetAxis8EStop(_ttof(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterGoogolGTS_UseInputNo0:
		case EDeviceParameterGoogolGTS_UseInputNo1:
		case EDeviceParameterGoogolGTS_UseInputNo2:
		case EDeviceParameterGoogolGTS_UseInputNo3:
		case EDeviceParameterGoogolGTS_UseInputNo4:
		case EDeviceParameterGoogolGTS_UseInputNo5:
		case EDeviceParameterGoogolGTS_UseInputNo6:
		case EDeviceParameterGoogolGTS_UseInputNo7:
		case EDeviceParameterGoogolGTS_UseInputNo8:
		case EDeviceParameterGoogolGTS_UseInputNo9:
		case EDeviceParameterGoogolGTS_UseInputNo10:
		case EDeviceParameterGoogolGTS_UseInputNo11:
		case EDeviceParameterGoogolGTS_UseInputNo12:
		case EDeviceParameterGoogolGTS_UseInputNo13:
		case EDeviceParameterGoogolGTS_UseInputNo14:
		case EDeviceParameterGoogolGTS_UseInputNo15:
			{
				int nPreValue = _ttoi(GetParamValue(nParam));
				int nValue = _ttoi(strValue);

				if(SetParamValue(nParam, strValue))
				{
					m_pBUsingInputArray[nParam - EDeviceParameterGoogolGTS_UseInputNo0] = nValue;					
					bReturn = SaveSettings(nParam);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[nParam], g_lpszGoogolGTSSwitch[nPreValue], g_lpszGoogolGTSSwitch[nValue]);

				if(bReturn)
					bUIUpdate = true;
			}
			break;
		case EDeviceParameterGoogolGTS_UseOutputNo0:
		case EDeviceParameterGoogolGTS_UseOutputNo1:
		case EDeviceParameterGoogolGTS_UseOutputNo2:
		case EDeviceParameterGoogolGTS_UseOutputNo3:
		case EDeviceParameterGoogolGTS_UseOutputNo4:
		case EDeviceParameterGoogolGTS_UseOutputNo5:
		case EDeviceParameterGoogolGTS_UseOutputNo6:
		case EDeviceParameterGoogolGTS_UseOutputNo7:
		case EDeviceParameterGoogolGTS_UseOutputNo8:
		case EDeviceParameterGoogolGTS_UseOutputNo9:
		case EDeviceParameterGoogolGTS_UseOutputNo10:
		case EDeviceParameterGoogolGTS_UseOutputNo11:
		case EDeviceParameterGoogolGTS_UseOutputNo12:
		case EDeviceParameterGoogolGTS_UseOutputNo13:
		case EDeviceParameterGoogolGTS_UseOutputNo14:
		case EDeviceParameterGoogolGTS_UseOutputNo15:
			{
				int nPreValue = _ttoi(GetParamValue(nParam));
				int nValue = _ttoi(strValue);

				if(SetParamValue(nParam, strValue))
				{
					m_pBUsingOutputArray[nParam - EDeviceParameterGoogolGTS_UseOutputNo0] = nValue;					
					bReturn = SaveSettings(nParam);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[nParam], g_lpszGoogolGTSSwitch[nPreValue], g_lpszGoogolGTSSwitch[nValue]);

				if(bReturn)
					bUIUpdate = true;
			}
			break;
		case EDeviceParameterGoogolGTS_InputNo0Name:
		case EDeviceParameterGoogolGTS_InputNo1Name:
		case EDeviceParameterGoogolGTS_InputNo2Name:
		case EDeviceParameterGoogolGTS_InputNo3Name:
		case EDeviceParameterGoogolGTS_InputNo4Name:
		case EDeviceParameterGoogolGTS_InputNo5Name:
		case EDeviceParameterGoogolGTS_InputNo6Name:
		case EDeviceParameterGoogolGTS_InputNo7Name:
		case EDeviceParameterGoogolGTS_InputNo8Name:
		case EDeviceParameterGoogolGTS_InputNo9Name:
		case EDeviceParameterGoogolGTS_InputNo10Name:
		case EDeviceParameterGoogolGTS_InputNo11Name:
		case EDeviceParameterGoogolGTS_InputNo12Name:
		case EDeviceParameterGoogolGTS_InputNo13Name:
		case EDeviceParameterGoogolGTS_InputNo14Name:
		case EDeviceParameterGoogolGTS_InputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				int nIndex = nParam - EDeviceParameterGoogolGTS_InputNo0Name;

				if(SetParamValue(nParam, strValue))
				{
					m_pStrInputNameArray[nIndex] = strValue;					
					bReturn = SaveSettings(nParam);
				}

				if(bReturn)
				{
					CString strFormat;
					strFormat.Format(_T("%d : %s"), nIndex, strValue);
					SetDlgItemText(EDeviceAddEnumeratedControlID_InputLedLabelStart + nIndex, strFormat);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterGoogolGTS_OutputNo0Name:
		case EDeviceParameterGoogolGTS_OutputNo1Name:
		case EDeviceParameterGoogolGTS_OutputNo2Name:
		case EDeviceParameterGoogolGTS_OutputNo3Name:
		case EDeviceParameterGoogolGTS_OutputNo4Name:
		case EDeviceParameterGoogolGTS_OutputNo5Name:
		case EDeviceParameterGoogolGTS_OutputNo6Name:
		case EDeviceParameterGoogolGTS_OutputNo7Name:
		case EDeviceParameterGoogolGTS_OutputNo8Name:
		case EDeviceParameterGoogolGTS_OutputNo9Name:
		case EDeviceParameterGoogolGTS_OutputNo10Name:
		case EDeviceParameterGoogolGTS_OutputNo11Name:
		case EDeviceParameterGoogolGTS_OutputNo12Name:
		case EDeviceParameterGoogolGTS_OutputNo13Name:
		case EDeviceParameterGoogolGTS_OutputNo14Name:
		case EDeviceParameterGoogolGTS_OutputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				int nIndex = nParam - EDeviceParameterGoogolGTS_OutputNo0Name;

				if(SetParamValue(nParam, strValue))
				{
					m_pStrOutputNameArray[nIndex] = strValue;					
					bReturn = SaveSettings(nParam);
				}

				if(bReturn)
				{
					CString strFormat;
					strFormat.Format(_T("%d : %s"), nIndex, strValue);
					SetDlgItemText(EDeviceAddEnumeratedControlID_OutputLedLabelStart + nIndex, strFormat);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterGoogolGTS_Ext1InputNo0Name:
		case EDeviceParameterGoogolGTS_Ext1InputNo1Name:
		case EDeviceParameterGoogolGTS_Ext1InputNo2Name:
		case EDeviceParameterGoogolGTS_Ext1InputNo3Name:
		case EDeviceParameterGoogolGTS_Ext1InputNo4Name:
		case EDeviceParameterGoogolGTS_Ext1InputNo5Name:
		case EDeviceParameterGoogolGTS_Ext1InputNo6Name:
		case EDeviceParameterGoogolGTS_Ext1InputNo7Name:
		case EDeviceParameterGoogolGTS_Ext1InputNo8Name:
		case EDeviceParameterGoogolGTS_Ext1InputNo9Name:
		case EDeviceParameterGoogolGTS_Ext1InputNo10Name:
		case EDeviceParameterGoogolGTS_Ext1InputNo11Name:
		case EDeviceParameterGoogolGTS_Ext1InputNo12Name:
		case EDeviceParameterGoogolGTS_Ext1InputNo13Name:
		case EDeviceParameterGoogolGTS_Ext1InputNo14Name:
		case EDeviceParameterGoogolGTS_Ext1InputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				int nIndex = nParam - EDeviceParameterGoogolGTS_Ext1InputNo0Name;
				int nModule = 0;

				if(SetParamValue(nParam, strValue))
				{
					m_ppStrExtInputName[nModule][nIndex] = strValue;
					bReturn = SaveSettings(nParam);
				}

				if(bReturn)
				{
					CString strFormat;
					strFormat.Format(_T("%d : %s"), nIndex, strValue);
					(m_pDlgExtDioViewer + nModule)->SetLabelString(EDeviceAddEnumeratedControlID_ExtInputLedLabelStart + nIndex + (GTS_EXTIO_PORT * nModule), strFormat);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterGoogolGTS_Ext2InputNo0Name:
		case EDeviceParameterGoogolGTS_Ext2InputNo1Name:
		case EDeviceParameterGoogolGTS_Ext2InputNo2Name:
		case EDeviceParameterGoogolGTS_Ext2InputNo3Name:
		case EDeviceParameterGoogolGTS_Ext2InputNo4Name:
		case EDeviceParameterGoogolGTS_Ext2InputNo5Name:
		case EDeviceParameterGoogolGTS_Ext2InputNo6Name:
		case EDeviceParameterGoogolGTS_Ext2InputNo7Name:
		case EDeviceParameterGoogolGTS_Ext2InputNo8Name:
		case EDeviceParameterGoogolGTS_Ext2InputNo9Name:
		case EDeviceParameterGoogolGTS_Ext2InputNo10Name:
		case EDeviceParameterGoogolGTS_Ext2InputNo11Name:
		case EDeviceParameterGoogolGTS_Ext2InputNo12Name:
		case EDeviceParameterGoogolGTS_Ext2InputNo13Name:
		case EDeviceParameterGoogolGTS_Ext2InputNo14Name:
		case EDeviceParameterGoogolGTS_Ext2InputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				int nIndex = nParam - EDeviceParameterGoogolGTS_Ext2InputNo0Name;
				int nModule = 1;

				if(SetParamValue(nParam, strValue))
				{
					m_ppStrExtInputName[nModule][nIndex] = strValue;
					bReturn = SaveSettings(nParam);
				}

				if(bReturn)
				{
					CString strFormat;
					strFormat.Format(_T("%d : %s"), nIndex, strValue);
					(m_pDlgExtDioViewer + nModule)->SetLabelString(EDeviceAddEnumeratedControlID_ExtInputLedLabelStart + nIndex + (GTS_EXTIO_PORT * nModule), strFormat);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterGoogolGTS_Ext3InputNo0Name:
		case EDeviceParameterGoogolGTS_Ext3InputNo1Name:
		case EDeviceParameterGoogolGTS_Ext3InputNo2Name:
		case EDeviceParameterGoogolGTS_Ext3InputNo3Name:
		case EDeviceParameterGoogolGTS_Ext3InputNo4Name:
		case EDeviceParameterGoogolGTS_Ext3InputNo5Name:
		case EDeviceParameterGoogolGTS_Ext3InputNo6Name:
		case EDeviceParameterGoogolGTS_Ext3InputNo7Name:
		case EDeviceParameterGoogolGTS_Ext3InputNo8Name:
		case EDeviceParameterGoogolGTS_Ext3InputNo9Name:
		case EDeviceParameterGoogolGTS_Ext3InputNo10Name:
		case EDeviceParameterGoogolGTS_Ext3InputNo11Name:
		case EDeviceParameterGoogolGTS_Ext3InputNo12Name:
		case EDeviceParameterGoogolGTS_Ext3InputNo13Name:
		case EDeviceParameterGoogolGTS_Ext3InputNo14Name:
		case EDeviceParameterGoogolGTS_Ext3InputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				int nIndex = nParam - EDeviceParameterGoogolGTS_Ext3InputNo0Name;
				int nModule = 2;

				if(SetParamValue(nParam, strValue))
				{
					m_ppStrExtInputName[nModule][nIndex] = strValue;
					bReturn = SaveSettings(nParam);
				}

				if(bReturn)
				{
					CString strFormat;
					strFormat.Format(_T("%d : %s"), nIndex, strValue);
					(m_pDlgExtDioViewer + nModule)->SetLabelString(EDeviceAddEnumeratedControlID_ExtInputLedLabelStart + nIndex + (GTS_EXTIO_PORT * nModule), strFormat);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterGoogolGTS_Ext4InputNo0Name:
		case EDeviceParameterGoogolGTS_Ext4InputNo1Name:
		case EDeviceParameterGoogolGTS_Ext4InputNo2Name:
		case EDeviceParameterGoogolGTS_Ext4InputNo3Name:
		case EDeviceParameterGoogolGTS_Ext4InputNo4Name:
		case EDeviceParameterGoogolGTS_Ext4InputNo5Name:
		case EDeviceParameterGoogolGTS_Ext4InputNo6Name:
		case EDeviceParameterGoogolGTS_Ext4InputNo7Name:
		case EDeviceParameterGoogolGTS_Ext4InputNo8Name:
		case EDeviceParameterGoogolGTS_Ext4InputNo9Name:
		case EDeviceParameterGoogolGTS_Ext4InputNo10Name:
		case EDeviceParameterGoogolGTS_Ext4InputNo11Name:
		case EDeviceParameterGoogolGTS_Ext4InputNo12Name:
		case EDeviceParameterGoogolGTS_Ext4InputNo13Name:
		case EDeviceParameterGoogolGTS_Ext4InputNo14Name:
		case EDeviceParameterGoogolGTS_Ext4InputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				int nIndex = nParam - EDeviceParameterGoogolGTS_Ext4InputNo0Name;
				int nModule = 3;

				if(SetParamValue(nParam, strValue))
				{
					m_ppStrExtInputName[nModule][nIndex] = strValue;
					bReturn = SaveSettings(nParam);
				}

				if(bReturn)
				{
					CString strFormat;
					strFormat.Format(_T("%d : %s"), nIndex, strValue);
					(m_pDlgExtDioViewer + nModule)->SetLabelString(EDeviceAddEnumeratedControlID_ExtInputLedLabelStart + nIndex + (GTS_EXTIO_PORT * nModule), strFormat);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterGoogolGTS_Ext5InputNo0Name:
		case EDeviceParameterGoogolGTS_Ext5InputNo1Name:
		case EDeviceParameterGoogolGTS_Ext5InputNo2Name:
		case EDeviceParameterGoogolGTS_Ext5InputNo3Name:
		case EDeviceParameterGoogolGTS_Ext5InputNo4Name:
		case EDeviceParameterGoogolGTS_Ext5InputNo5Name:
		case EDeviceParameterGoogolGTS_Ext5InputNo6Name:
		case EDeviceParameterGoogolGTS_Ext5InputNo7Name:
		case EDeviceParameterGoogolGTS_Ext5InputNo8Name:
		case EDeviceParameterGoogolGTS_Ext5InputNo9Name:
		case EDeviceParameterGoogolGTS_Ext5InputNo10Name:
		case EDeviceParameterGoogolGTS_Ext5InputNo11Name:
		case EDeviceParameterGoogolGTS_Ext5InputNo12Name:
		case EDeviceParameterGoogolGTS_Ext5InputNo13Name:
		case EDeviceParameterGoogolGTS_Ext5InputNo14Name:
		case EDeviceParameterGoogolGTS_Ext5InputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				int nIndex = nParam - EDeviceParameterGoogolGTS_Ext5InputNo0Name;
				int nModule = 4;

				if(SetParamValue(nParam, strValue))
				{
					m_ppStrExtInputName[nModule][nIndex] = strValue;
					bReturn = SaveSettings(nParam);
				}

				if(bReturn)
				{
					CString strFormat;
					strFormat.Format(_T("%d : %s"), nIndex, strValue);
					(m_pDlgExtDioViewer + nModule)->SetLabelString(EDeviceAddEnumeratedControlID_ExtInputLedLabelStart + nIndex + (GTS_EXTIO_PORT * nModule), strFormat);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterGoogolGTS_Ext6InputNo0Name:
		case EDeviceParameterGoogolGTS_Ext6InputNo1Name:
		case EDeviceParameterGoogolGTS_Ext6InputNo2Name:
		case EDeviceParameterGoogolGTS_Ext6InputNo3Name:
		case EDeviceParameterGoogolGTS_Ext6InputNo4Name:
		case EDeviceParameterGoogolGTS_Ext6InputNo5Name:
		case EDeviceParameterGoogolGTS_Ext6InputNo6Name:
		case EDeviceParameterGoogolGTS_Ext6InputNo7Name:
		case EDeviceParameterGoogolGTS_Ext6InputNo8Name:
		case EDeviceParameterGoogolGTS_Ext6InputNo9Name:
		case EDeviceParameterGoogolGTS_Ext6InputNo10Name:
		case EDeviceParameterGoogolGTS_Ext6InputNo11Name:
		case EDeviceParameterGoogolGTS_Ext6InputNo12Name:
		case EDeviceParameterGoogolGTS_Ext6InputNo13Name:
		case EDeviceParameterGoogolGTS_Ext6InputNo14Name:
		case EDeviceParameterGoogolGTS_Ext6InputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				int nIndex = nParam - EDeviceParameterGoogolGTS_Ext6InputNo0Name;
				int nModule = 5;

				if(SetParamValue(nParam, strValue))
				{
					m_ppStrExtInputName[nModule][nIndex] = strValue;
					bReturn = SaveSettings(nParam);
				}

				if(bReturn)
				{
					CString strFormat;
					strFormat.Format(_T("%d : %s"), nIndex, strValue);
					(m_pDlgExtDioViewer + nModule)->SetLabelString(EDeviceAddEnumeratedControlID_ExtInputLedLabelStart + nIndex + (GTS_EXTIO_PORT * nModule), strFormat);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterGoogolGTS_Ext7InputNo0Name:
		case EDeviceParameterGoogolGTS_Ext7InputNo1Name:
		case EDeviceParameterGoogolGTS_Ext7InputNo2Name:
		case EDeviceParameterGoogolGTS_Ext7InputNo3Name:
		case EDeviceParameterGoogolGTS_Ext7InputNo4Name:
		case EDeviceParameterGoogolGTS_Ext7InputNo5Name:
		case EDeviceParameterGoogolGTS_Ext7InputNo6Name:
		case EDeviceParameterGoogolGTS_Ext7InputNo7Name:
		case EDeviceParameterGoogolGTS_Ext7InputNo8Name:
		case EDeviceParameterGoogolGTS_Ext7InputNo9Name:
		case EDeviceParameterGoogolGTS_Ext7InputNo10Name:
		case EDeviceParameterGoogolGTS_Ext7InputNo11Name:
		case EDeviceParameterGoogolGTS_Ext7InputNo12Name:
		case EDeviceParameterGoogolGTS_Ext7InputNo13Name:
		case EDeviceParameterGoogolGTS_Ext7InputNo14Name:
		case EDeviceParameterGoogolGTS_Ext7InputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				int nIndex = nParam - EDeviceParameterGoogolGTS_Ext7InputNo0Name;
				int nModule = 6;

				if(SetParamValue(nParam, strValue))
				{
					m_ppStrExtInputName[nModule][nIndex] = strValue;
					bReturn = SaveSettings(nParam);
				}

				if(bReturn)
				{
					CString strFormat;
					strFormat.Format(_T("%d : %s"), nIndex, strValue);
					(m_pDlgExtDioViewer + nModule)->SetLabelString(EDeviceAddEnumeratedControlID_ExtInputLedLabelStart + nIndex + (GTS_EXTIO_PORT * nModule), strFormat);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterGoogolGTS_Ext8InputNo0Name:
		case EDeviceParameterGoogolGTS_Ext8InputNo1Name:
		case EDeviceParameterGoogolGTS_Ext8InputNo2Name:
		case EDeviceParameterGoogolGTS_Ext8InputNo3Name:
		case EDeviceParameterGoogolGTS_Ext8InputNo4Name:
		case EDeviceParameterGoogolGTS_Ext8InputNo5Name:
		case EDeviceParameterGoogolGTS_Ext8InputNo6Name:
		case EDeviceParameterGoogolGTS_Ext8InputNo7Name:
		case EDeviceParameterGoogolGTS_Ext8InputNo8Name:
		case EDeviceParameterGoogolGTS_Ext8InputNo9Name:
		case EDeviceParameterGoogolGTS_Ext8InputNo10Name:
		case EDeviceParameterGoogolGTS_Ext8InputNo11Name:
		case EDeviceParameterGoogolGTS_Ext8InputNo12Name:
		case EDeviceParameterGoogolGTS_Ext8InputNo13Name:
		case EDeviceParameterGoogolGTS_Ext8InputNo14Name:
		case EDeviceParameterGoogolGTS_Ext8InputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				int nIndex = nParam - EDeviceParameterGoogolGTS_Ext8InputNo0Name;
				int nModule = 7;

				if(SetParamValue(nParam, strValue))
				{
					m_ppStrExtInputName[nModule][nIndex] = strValue;
					bReturn = SaveSettings(nParam);
				}

				if(bReturn)
				{
					CString strFormat;
					strFormat.Format(_T("%d : %s"), nIndex, strValue);
					(m_pDlgExtDioViewer + nModule)->SetLabelString(EDeviceAddEnumeratedControlID_ExtInputLedLabelStart + nIndex + (GTS_EXTIO_PORT * nModule), strFormat);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterGoogolGTS_Ext9InputNo0Name:
		case EDeviceParameterGoogolGTS_Ext9InputNo1Name:
		case EDeviceParameterGoogolGTS_Ext9InputNo2Name:
		case EDeviceParameterGoogolGTS_Ext9InputNo3Name:
		case EDeviceParameterGoogolGTS_Ext9InputNo4Name:
		case EDeviceParameterGoogolGTS_Ext9InputNo5Name:
		case EDeviceParameterGoogolGTS_Ext9InputNo6Name:
		case EDeviceParameterGoogolGTS_Ext9InputNo7Name:
		case EDeviceParameterGoogolGTS_Ext9InputNo8Name:
		case EDeviceParameterGoogolGTS_Ext9InputNo9Name:
		case EDeviceParameterGoogolGTS_Ext9InputNo10Name:
		case EDeviceParameterGoogolGTS_Ext9InputNo11Name:
		case EDeviceParameterGoogolGTS_Ext9InputNo12Name:
		case EDeviceParameterGoogolGTS_Ext9InputNo13Name:
		case EDeviceParameterGoogolGTS_Ext9InputNo14Name:
		case EDeviceParameterGoogolGTS_Ext9InputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				int nIndex = nParam - EDeviceParameterGoogolGTS_Ext9InputNo0Name;
				int nModule = 8;

				if(SetParamValue(nParam, strValue))
				{
					m_ppStrExtInputName[nModule][nIndex] = strValue;
					bReturn = SaveSettings(nParam);
				}

				if(bReturn)
				{
					CString strFormat;
					strFormat.Format(_T("%d : %s"), nIndex, strValue);
					(m_pDlgExtDioViewer + nModule)->SetLabelString(EDeviceAddEnumeratedControlID_ExtInputLedLabelStart + nIndex + (GTS_EXTIO_PORT * nModule), strFormat);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterGoogolGTS_Ext10InputNo0Name:
		case EDeviceParameterGoogolGTS_Ext10InputNo1Name:
		case EDeviceParameterGoogolGTS_Ext10InputNo2Name:
		case EDeviceParameterGoogolGTS_Ext10InputNo3Name:
		case EDeviceParameterGoogolGTS_Ext10InputNo4Name:
		case EDeviceParameterGoogolGTS_Ext10InputNo5Name:
		case EDeviceParameterGoogolGTS_Ext10InputNo6Name:
		case EDeviceParameterGoogolGTS_Ext10InputNo7Name:
		case EDeviceParameterGoogolGTS_Ext10InputNo8Name:
		case EDeviceParameterGoogolGTS_Ext10InputNo9Name:
		case EDeviceParameterGoogolGTS_Ext10InputNo10Name:
		case EDeviceParameterGoogolGTS_Ext10InputNo11Name:
		case EDeviceParameterGoogolGTS_Ext10InputNo12Name:
		case EDeviceParameterGoogolGTS_Ext10InputNo13Name:
		case EDeviceParameterGoogolGTS_Ext10InputNo14Name:
		case EDeviceParameterGoogolGTS_Ext10InputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				int nIndex = nParam - EDeviceParameterGoogolGTS_Ext10InputNo0Name;
				int nModule = 9;

				if(SetParamValue(nParam, strValue))
				{
					m_ppStrExtInputName[nModule][nIndex] = strValue;
					bReturn = SaveSettings(nParam);
				}

				if(bReturn)
				{
					CString strFormat;
					strFormat.Format(_T("%d : %s"), nIndex, strValue);
					(m_pDlgExtDioViewer + nModule)->SetLabelString(EDeviceAddEnumeratedControlID_ExtInputLedLabelStart + nIndex + (GTS_EXTIO_PORT * nModule), strFormat);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterGoogolGTS_Ext11InputNo0Name:
		case EDeviceParameterGoogolGTS_Ext11InputNo1Name:
		case EDeviceParameterGoogolGTS_Ext11InputNo2Name:
		case EDeviceParameterGoogolGTS_Ext11InputNo3Name:
		case EDeviceParameterGoogolGTS_Ext11InputNo4Name:
		case EDeviceParameterGoogolGTS_Ext11InputNo5Name:
		case EDeviceParameterGoogolGTS_Ext11InputNo6Name:
		case EDeviceParameterGoogolGTS_Ext11InputNo7Name:
		case EDeviceParameterGoogolGTS_Ext11InputNo8Name:
		case EDeviceParameterGoogolGTS_Ext11InputNo9Name:
		case EDeviceParameterGoogolGTS_Ext11InputNo10Name:
		case EDeviceParameterGoogolGTS_Ext11InputNo11Name:
		case EDeviceParameterGoogolGTS_Ext11InputNo12Name:
		case EDeviceParameterGoogolGTS_Ext11InputNo13Name:
		case EDeviceParameterGoogolGTS_Ext11InputNo14Name:
		case EDeviceParameterGoogolGTS_Ext11InputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				int nIndex = nParam - EDeviceParameterGoogolGTS_Ext11InputNo0Name;
				int nModule = 10;

				if(SetParamValue(nParam, strValue))
				{
					m_ppStrExtInputName[nModule][nIndex] = strValue;
					bReturn = SaveSettings(nParam);
				}

				if(bReturn)
				{
					CString strFormat;
					strFormat.Format(_T("%d : %s"), nIndex, strValue);
					(m_pDlgExtDioViewer + nModule)->SetLabelString(EDeviceAddEnumeratedControlID_ExtInputLedLabelStart + nIndex + (GTS_EXTIO_PORT * nModule), strFormat);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterGoogolGTS_Ext12InputNo0Name:
		case EDeviceParameterGoogolGTS_Ext12InputNo1Name:
		case EDeviceParameterGoogolGTS_Ext12InputNo2Name:
		case EDeviceParameterGoogolGTS_Ext12InputNo3Name:
		case EDeviceParameterGoogolGTS_Ext12InputNo4Name:
		case EDeviceParameterGoogolGTS_Ext12InputNo5Name:
		case EDeviceParameterGoogolGTS_Ext12InputNo6Name:
		case EDeviceParameterGoogolGTS_Ext12InputNo7Name:
		case EDeviceParameterGoogolGTS_Ext12InputNo8Name:
		case EDeviceParameterGoogolGTS_Ext12InputNo9Name:
		case EDeviceParameterGoogolGTS_Ext12InputNo10Name:
		case EDeviceParameterGoogolGTS_Ext12InputNo11Name:
		case EDeviceParameterGoogolGTS_Ext12InputNo12Name:
		case EDeviceParameterGoogolGTS_Ext12InputNo13Name:
		case EDeviceParameterGoogolGTS_Ext12InputNo14Name:
		case EDeviceParameterGoogolGTS_Ext12InputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				int nIndex = nParam - EDeviceParameterGoogolGTS_Ext12InputNo0Name;
				int nModule = 11;

				if(SetParamValue(nParam, strValue))
				{
					m_ppStrExtInputName[nModule][nIndex] = strValue;
					bReturn = SaveSettings(nParam);
				}

				if(bReturn)
				{
					CString strFormat;
					strFormat.Format(_T("%d : %s"), nIndex, strValue);
					(m_pDlgExtDioViewer + nModule)->SetLabelString(EDeviceAddEnumeratedControlID_ExtInputLedLabelStart + nIndex + (GTS_EXTIO_PORT * nModule), strFormat);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterGoogolGTS_Ext13InputNo0Name:
		case EDeviceParameterGoogolGTS_Ext13InputNo1Name:
		case EDeviceParameterGoogolGTS_Ext13InputNo2Name:
		case EDeviceParameterGoogolGTS_Ext13InputNo3Name:
		case EDeviceParameterGoogolGTS_Ext13InputNo4Name:
		case EDeviceParameterGoogolGTS_Ext13InputNo5Name:
		case EDeviceParameterGoogolGTS_Ext13InputNo6Name:
		case EDeviceParameterGoogolGTS_Ext13InputNo7Name:
		case EDeviceParameterGoogolGTS_Ext13InputNo8Name:
		case EDeviceParameterGoogolGTS_Ext13InputNo9Name:
		case EDeviceParameterGoogolGTS_Ext13InputNo10Name:
		case EDeviceParameterGoogolGTS_Ext13InputNo11Name:
		case EDeviceParameterGoogolGTS_Ext13InputNo12Name:
		case EDeviceParameterGoogolGTS_Ext13InputNo13Name:
		case EDeviceParameterGoogolGTS_Ext13InputNo14Name:
		case EDeviceParameterGoogolGTS_Ext13InputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				int nIndex = nParam - EDeviceParameterGoogolGTS_Ext13InputNo0Name;
				int nModule = 12;

				if(SetParamValue(nParam, strValue))
				{
					m_ppStrExtInputName[nModule][nIndex] = strValue;
					bReturn = SaveSettings(nParam);
				}

				if(bReturn)
				{
					CString strFormat;
					strFormat.Format(_T("%d : %s"), nIndex, strValue);
					(m_pDlgExtDioViewer + nModule)->SetLabelString(EDeviceAddEnumeratedControlID_ExtInputLedLabelStart + nIndex + (GTS_EXTIO_PORT * nModule), strFormat);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterGoogolGTS_Ext14InputNo0Name:
		case EDeviceParameterGoogolGTS_Ext14InputNo1Name:
		case EDeviceParameterGoogolGTS_Ext14InputNo2Name:
		case EDeviceParameterGoogolGTS_Ext14InputNo3Name:
		case EDeviceParameterGoogolGTS_Ext14InputNo4Name:
		case EDeviceParameterGoogolGTS_Ext14InputNo5Name:
		case EDeviceParameterGoogolGTS_Ext14InputNo6Name:
		case EDeviceParameterGoogolGTS_Ext14InputNo7Name:
		case EDeviceParameterGoogolGTS_Ext14InputNo8Name:
		case EDeviceParameterGoogolGTS_Ext14InputNo9Name:
		case EDeviceParameterGoogolGTS_Ext14InputNo10Name:
		case EDeviceParameterGoogolGTS_Ext14InputNo11Name:
		case EDeviceParameterGoogolGTS_Ext14InputNo12Name:
		case EDeviceParameterGoogolGTS_Ext14InputNo13Name:
		case EDeviceParameterGoogolGTS_Ext14InputNo14Name:
		case EDeviceParameterGoogolGTS_Ext14InputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				int nIndex = nParam - EDeviceParameterGoogolGTS_Ext14InputNo0Name;
				int nModule = 13;

				if(SetParamValue(nParam, strValue))
				{
					m_ppStrExtInputName[nModule][nIndex] = strValue;
					bReturn = SaveSettings(nParam);
				}

				if(bReturn)
				{
					CString strFormat;
					strFormat.Format(_T("%d : %s"), nIndex, strValue);
					(m_pDlgExtDioViewer + nModule)->SetLabelString(EDeviceAddEnumeratedControlID_ExtInputLedLabelStart + nIndex + (GTS_EXTIO_PORT * nModule), strFormat);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterGoogolGTS_Ext15InputNo0Name:
		case EDeviceParameterGoogolGTS_Ext15InputNo1Name:
		case EDeviceParameterGoogolGTS_Ext15InputNo2Name:
		case EDeviceParameterGoogolGTS_Ext15InputNo3Name:
		case EDeviceParameterGoogolGTS_Ext15InputNo4Name:
		case EDeviceParameterGoogolGTS_Ext15InputNo5Name:
		case EDeviceParameterGoogolGTS_Ext15InputNo6Name:
		case EDeviceParameterGoogolGTS_Ext15InputNo7Name:
		case EDeviceParameterGoogolGTS_Ext15InputNo8Name:
		case EDeviceParameterGoogolGTS_Ext15InputNo9Name:
		case EDeviceParameterGoogolGTS_Ext15InputNo10Name:
		case EDeviceParameterGoogolGTS_Ext15InputNo11Name:
		case EDeviceParameterGoogolGTS_Ext15InputNo12Name:
		case EDeviceParameterGoogolGTS_Ext15InputNo13Name:
		case EDeviceParameterGoogolGTS_Ext15InputNo14Name:
		case EDeviceParameterGoogolGTS_Ext15InputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				int nIndex = nParam - EDeviceParameterGoogolGTS_Ext15InputNo0Name;
				int nModule = 14;

				if(SetParamValue(nParam, strValue))
				{
					m_ppStrExtInputName[nModule][nIndex] = strValue;
					bReturn = SaveSettings(nParam);
				}

				if(bReturn)
				{
					CString strFormat;
					strFormat.Format(_T("%d : %s"), nIndex, strValue);
					(m_pDlgExtDioViewer + nModule)->SetLabelString(EDeviceAddEnumeratedControlID_ExtInputLedLabelStart + nIndex + (GTS_EXTIO_PORT * nModule), strFormat);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterGoogolGTS_Ext16InputNo0Name:
		case EDeviceParameterGoogolGTS_Ext16InputNo1Name:
		case EDeviceParameterGoogolGTS_Ext16InputNo2Name:
		case EDeviceParameterGoogolGTS_Ext16InputNo3Name:
		case EDeviceParameterGoogolGTS_Ext16InputNo4Name:
		case EDeviceParameterGoogolGTS_Ext16InputNo5Name:
		case EDeviceParameterGoogolGTS_Ext16InputNo6Name:
		case EDeviceParameterGoogolGTS_Ext16InputNo7Name:
		case EDeviceParameterGoogolGTS_Ext16InputNo8Name:
		case EDeviceParameterGoogolGTS_Ext16InputNo9Name:
		case EDeviceParameterGoogolGTS_Ext16InputNo10Name:
		case EDeviceParameterGoogolGTS_Ext16InputNo11Name:
		case EDeviceParameterGoogolGTS_Ext16InputNo12Name:
		case EDeviceParameterGoogolGTS_Ext16InputNo13Name:
		case EDeviceParameterGoogolGTS_Ext16InputNo14Name:
		case EDeviceParameterGoogolGTS_Ext16InputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				int nIndex = nParam - EDeviceParameterGoogolGTS_Ext16InputNo0Name;
				int nModule = 15;

				if(SetParamValue(nParam, strValue))
				{
					m_ppStrExtInputName[nModule][nIndex] = strValue;
					bReturn = SaveSettings(nParam);
				}

				if(bReturn)
				{
					CString strFormat;
					strFormat.Format(_T("%d : %s"), nIndex, strValue);
					(m_pDlgExtDioViewer + nModule)->SetLabelString(EDeviceAddEnumeratedControlID_ExtInputLedLabelStart + nIndex + (GTS_EXTIO_PORT * nModule), strFormat);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterGoogolGTS_Ext1OutputNo0Name:
		case EDeviceParameterGoogolGTS_Ext1OutputNo1Name:
		case EDeviceParameterGoogolGTS_Ext1OutputNo2Name:
		case EDeviceParameterGoogolGTS_Ext1OutputNo3Name:
		case EDeviceParameterGoogolGTS_Ext1OutputNo4Name:
		case EDeviceParameterGoogolGTS_Ext1OutputNo5Name:
		case EDeviceParameterGoogolGTS_Ext1OutputNo6Name:
		case EDeviceParameterGoogolGTS_Ext1OutputNo7Name:
		case EDeviceParameterGoogolGTS_Ext1OutputNo8Name:
		case EDeviceParameterGoogolGTS_Ext1OutputNo9Name:
		case EDeviceParameterGoogolGTS_Ext1OutputNo10Name:
		case EDeviceParameterGoogolGTS_Ext1OutputNo11Name:
		case EDeviceParameterGoogolGTS_Ext1OutputNo12Name:
		case EDeviceParameterGoogolGTS_Ext1OutputNo13Name:
		case EDeviceParameterGoogolGTS_Ext1OutputNo14Name:
		case EDeviceParameterGoogolGTS_Ext1OutputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				int nIndex = nParam - EDeviceParameterGoogolGTS_Ext1OutputNo0Name;
				int nModule = 0;

				if(SetParamValue(nParam, strValue))
				{
					m_ppStrExtOutputName[nModule][nIndex] = strValue;
					bReturn = SaveSettings(nParam);
				}

				if(bReturn)
				{
					CString strFormat;
					strFormat.Format(_T("%d : %s"), nIndex, strValue);
					(m_pDlgExtDioViewer + nModule)->SetLabelString(EDeviceAddEnumeratedControlID_ExtOutputLedLabelStart + nIndex + (GTS_EXTIO_PORT * nModule), strFormat);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterGoogolGTS_Ext2OutputNo0Name:
		case EDeviceParameterGoogolGTS_Ext2OutputNo1Name:
		case EDeviceParameterGoogolGTS_Ext2OutputNo2Name:
		case EDeviceParameterGoogolGTS_Ext2OutputNo3Name:
		case EDeviceParameterGoogolGTS_Ext2OutputNo4Name:
		case EDeviceParameterGoogolGTS_Ext2OutputNo5Name:
		case EDeviceParameterGoogolGTS_Ext2OutputNo6Name:
		case EDeviceParameterGoogolGTS_Ext2OutputNo7Name:
		case EDeviceParameterGoogolGTS_Ext2OutputNo8Name:
		case EDeviceParameterGoogolGTS_Ext2OutputNo9Name:
		case EDeviceParameterGoogolGTS_Ext2OutputNo10Name:
		case EDeviceParameterGoogolGTS_Ext2OutputNo11Name:
		case EDeviceParameterGoogolGTS_Ext2OutputNo12Name:
		case EDeviceParameterGoogolGTS_Ext2OutputNo13Name:
		case EDeviceParameterGoogolGTS_Ext2OutputNo14Name:
		case EDeviceParameterGoogolGTS_Ext2OutputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				int nIndex = nParam - EDeviceParameterGoogolGTS_Ext2OutputNo0Name;
				int nModule = 1;

				if(SetParamValue(nParam, strValue))
				{
					m_ppStrExtOutputName[nModule][nIndex] = strValue;
					bReturn = SaveSettings(nParam);
				}

				if(bReturn)
				{
					CString strFormat;
					strFormat.Format(_T("%d : %s"), nIndex, strValue);
					(m_pDlgExtDioViewer + nModule)->SetLabelString(EDeviceAddEnumeratedControlID_ExtOutputLedLabelStart + nIndex + (GTS_EXTIO_PORT * nModule), strFormat);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterGoogolGTS_Ext3OutputNo0Name:
		case EDeviceParameterGoogolGTS_Ext3OutputNo1Name:
		case EDeviceParameterGoogolGTS_Ext3OutputNo2Name:
		case EDeviceParameterGoogolGTS_Ext3OutputNo3Name:
		case EDeviceParameterGoogolGTS_Ext3OutputNo4Name:
		case EDeviceParameterGoogolGTS_Ext3OutputNo5Name:
		case EDeviceParameterGoogolGTS_Ext3OutputNo6Name:
		case EDeviceParameterGoogolGTS_Ext3OutputNo7Name:
		case EDeviceParameterGoogolGTS_Ext3OutputNo8Name:
		case EDeviceParameterGoogolGTS_Ext3OutputNo9Name:
		case EDeviceParameterGoogolGTS_Ext3OutputNo10Name:
		case EDeviceParameterGoogolGTS_Ext3OutputNo11Name:
		case EDeviceParameterGoogolGTS_Ext3OutputNo12Name:
		case EDeviceParameterGoogolGTS_Ext3OutputNo13Name:
		case EDeviceParameterGoogolGTS_Ext3OutputNo14Name:
		case EDeviceParameterGoogolGTS_Ext3OutputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				int nIndex = nParam - EDeviceParameterGoogolGTS_Ext3OutputNo0Name;
				int nModule = 2;

				if(SetParamValue(nParam, strValue))
				{
					m_ppStrExtOutputName[nModule][nIndex] = strValue;
					bReturn = SaveSettings(nParam);
				}

				if(bReturn)
				{
					CString strFormat;
					strFormat.Format(_T("%d : %s"), nIndex, strValue);
					(m_pDlgExtDioViewer + nModule)->SetLabelString(EDeviceAddEnumeratedControlID_ExtOutputLedLabelStart + nIndex + (GTS_EXTIO_PORT * nModule), strFormat);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterGoogolGTS_Ext4OutputNo0Name:
		case EDeviceParameterGoogolGTS_Ext4OutputNo1Name:
		case EDeviceParameterGoogolGTS_Ext4OutputNo2Name:
		case EDeviceParameterGoogolGTS_Ext4OutputNo3Name:
		case EDeviceParameterGoogolGTS_Ext4OutputNo4Name:
		case EDeviceParameterGoogolGTS_Ext4OutputNo5Name:
		case EDeviceParameterGoogolGTS_Ext4OutputNo6Name:
		case EDeviceParameterGoogolGTS_Ext4OutputNo7Name:
		case EDeviceParameterGoogolGTS_Ext4OutputNo8Name:
		case EDeviceParameterGoogolGTS_Ext4OutputNo9Name:
		case EDeviceParameterGoogolGTS_Ext4OutputNo10Name:
		case EDeviceParameterGoogolGTS_Ext4OutputNo11Name:
		case EDeviceParameterGoogolGTS_Ext4OutputNo12Name:
		case EDeviceParameterGoogolGTS_Ext4OutputNo13Name:
		case EDeviceParameterGoogolGTS_Ext4OutputNo14Name:
		case EDeviceParameterGoogolGTS_Ext4OutputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				int nIndex = nParam - EDeviceParameterGoogolGTS_Ext4OutputNo0Name;
				int nModule = 3;

				if(SetParamValue(nParam, strValue))
				{
					m_ppStrExtOutputName[nModule][nIndex] = strValue;
					bReturn = SaveSettings(nParam);
				}

				if(bReturn)
				{
					CString strFormat;
					strFormat.Format(_T("%d : %s"), nIndex, strValue);
					(m_pDlgExtDioViewer + nModule)->SetLabelString(EDeviceAddEnumeratedControlID_ExtOutputLedLabelStart + nIndex + (GTS_EXTIO_PORT * nModule), strFormat);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterGoogolGTS_Ext5OutputNo0Name:
		case EDeviceParameterGoogolGTS_Ext5OutputNo1Name:
		case EDeviceParameterGoogolGTS_Ext5OutputNo2Name:
		case EDeviceParameterGoogolGTS_Ext5OutputNo3Name:
		case EDeviceParameterGoogolGTS_Ext5OutputNo4Name:
		case EDeviceParameterGoogolGTS_Ext5OutputNo5Name:
		case EDeviceParameterGoogolGTS_Ext5OutputNo6Name:
		case EDeviceParameterGoogolGTS_Ext5OutputNo7Name:
		case EDeviceParameterGoogolGTS_Ext5OutputNo8Name:
		case EDeviceParameterGoogolGTS_Ext5OutputNo9Name:
		case EDeviceParameterGoogolGTS_Ext5OutputNo10Name:
		case EDeviceParameterGoogolGTS_Ext5OutputNo11Name:
		case EDeviceParameterGoogolGTS_Ext5OutputNo12Name:
		case EDeviceParameterGoogolGTS_Ext5OutputNo13Name:
		case EDeviceParameterGoogolGTS_Ext5OutputNo14Name:
		case EDeviceParameterGoogolGTS_Ext5OutputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				int nIndex = nParam - EDeviceParameterGoogolGTS_Ext5OutputNo0Name;
				int nModule = 4;

				if(SetParamValue(nParam, strValue))
				{
					m_ppStrExtOutputName[nModule][nIndex] = strValue;
					bReturn = SaveSettings(nParam);
				}

				if(bReturn)
				{
					CString strFormat;
					strFormat.Format(_T("%d : %s"), nIndex, strValue);
					(m_pDlgExtDioViewer + nModule)->SetLabelString(EDeviceAddEnumeratedControlID_ExtOutputLedLabelStart + nIndex + (GTS_EXTIO_PORT * nModule), strFormat);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterGoogolGTS_Ext6OutputNo0Name:
		case EDeviceParameterGoogolGTS_Ext6OutputNo1Name:
		case EDeviceParameterGoogolGTS_Ext6OutputNo2Name:
		case EDeviceParameterGoogolGTS_Ext6OutputNo3Name:
		case EDeviceParameterGoogolGTS_Ext6OutputNo4Name:
		case EDeviceParameterGoogolGTS_Ext6OutputNo5Name:
		case EDeviceParameterGoogolGTS_Ext6OutputNo6Name:
		case EDeviceParameterGoogolGTS_Ext6OutputNo7Name:
		case EDeviceParameterGoogolGTS_Ext6OutputNo8Name:
		case EDeviceParameterGoogolGTS_Ext6OutputNo9Name:
		case EDeviceParameterGoogolGTS_Ext6OutputNo10Name:
		case EDeviceParameterGoogolGTS_Ext6OutputNo11Name:
		case EDeviceParameterGoogolGTS_Ext6OutputNo12Name:
		case EDeviceParameterGoogolGTS_Ext6OutputNo13Name:
		case EDeviceParameterGoogolGTS_Ext6OutputNo14Name:
		case EDeviceParameterGoogolGTS_Ext6OutputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				int nIndex = nParam - EDeviceParameterGoogolGTS_Ext6OutputNo0Name;
				int nModule = 5;

				if(SetParamValue(nParam, strValue))
				{
					m_ppStrExtOutputName[nModule][nIndex] = strValue;
					bReturn = SaveSettings(nParam);
				}

				if(bReturn)
				{
					CString strFormat;
					strFormat.Format(_T("%d : %s"), nIndex, strValue);
					(m_pDlgExtDioViewer + nModule)->SetLabelString(EDeviceAddEnumeratedControlID_ExtOutputLedLabelStart + nIndex + (GTS_EXTIO_PORT * nModule), strFormat);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterGoogolGTS_Ext7OutputNo0Name:
		case EDeviceParameterGoogolGTS_Ext7OutputNo1Name:
		case EDeviceParameterGoogolGTS_Ext7OutputNo2Name:
		case EDeviceParameterGoogolGTS_Ext7OutputNo3Name:
		case EDeviceParameterGoogolGTS_Ext7OutputNo4Name:
		case EDeviceParameterGoogolGTS_Ext7OutputNo5Name:
		case EDeviceParameterGoogolGTS_Ext7OutputNo6Name:
		case EDeviceParameterGoogolGTS_Ext7OutputNo7Name:
		case EDeviceParameterGoogolGTS_Ext7OutputNo8Name:
		case EDeviceParameterGoogolGTS_Ext7OutputNo9Name:
		case EDeviceParameterGoogolGTS_Ext7OutputNo10Name:
		case EDeviceParameterGoogolGTS_Ext7OutputNo11Name:
		case EDeviceParameterGoogolGTS_Ext7OutputNo12Name:
		case EDeviceParameterGoogolGTS_Ext7OutputNo13Name:
		case EDeviceParameterGoogolGTS_Ext7OutputNo14Name:
		case EDeviceParameterGoogolGTS_Ext7OutputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				int nIndex = nParam - EDeviceParameterGoogolGTS_Ext7OutputNo0Name;
				int nModule = 6;

				if(SetParamValue(nParam, strValue))
				{
					m_ppStrExtOutputName[nModule][nIndex] = strValue;
					bReturn = SaveSettings(nParam);
				}

				if(bReturn)
				{
					CString strFormat;
					strFormat.Format(_T("%d : %s"), nIndex, strValue);
					(m_pDlgExtDioViewer + nModule)->SetLabelString(EDeviceAddEnumeratedControlID_ExtOutputLedLabelStart + nIndex + (GTS_EXTIO_PORT * nModule), strFormat);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterGoogolGTS_Ext8OutputNo0Name:
		case EDeviceParameterGoogolGTS_Ext8OutputNo1Name:
		case EDeviceParameterGoogolGTS_Ext8OutputNo2Name:
		case EDeviceParameterGoogolGTS_Ext8OutputNo3Name:
		case EDeviceParameterGoogolGTS_Ext8OutputNo4Name:
		case EDeviceParameterGoogolGTS_Ext8OutputNo5Name:
		case EDeviceParameterGoogolGTS_Ext8OutputNo6Name:
		case EDeviceParameterGoogolGTS_Ext8OutputNo7Name:
		case EDeviceParameterGoogolGTS_Ext8OutputNo8Name:
		case EDeviceParameterGoogolGTS_Ext8OutputNo9Name:
		case EDeviceParameterGoogolGTS_Ext8OutputNo10Name:
		case EDeviceParameterGoogolGTS_Ext8OutputNo11Name:
		case EDeviceParameterGoogolGTS_Ext8OutputNo12Name:
		case EDeviceParameterGoogolGTS_Ext8OutputNo13Name:
		case EDeviceParameterGoogolGTS_Ext8OutputNo14Name:
		case EDeviceParameterGoogolGTS_Ext8OutputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				int nIndex = nParam - EDeviceParameterGoogolGTS_Ext8OutputNo0Name;
				int nModule = 7;

				if(SetParamValue(nParam, strValue))
				{
					m_ppStrExtOutputName[nModule][nIndex] = strValue;
					bReturn = SaveSettings(nParam);
				}

				if(bReturn)
				{
					CString strFormat;
					strFormat.Format(_T("%d : %s"), nIndex, strValue);
					(m_pDlgExtDioViewer + nModule)->SetLabelString(EDeviceAddEnumeratedControlID_ExtOutputLedLabelStart + nIndex + (GTS_EXTIO_PORT * nModule), strFormat);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterGoogolGTS_Ext9OutputNo0Name:
		case EDeviceParameterGoogolGTS_Ext9OutputNo1Name:
		case EDeviceParameterGoogolGTS_Ext9OutputNo2Name:
		case EDeviceParameterGoogolGTS_Ext9OutputNo3Name:
		case EDeviceParameterGoogolGTS_Ext9OutputNo4Name:
		case EDeviceParameterGoogolGTS_Ext9OutputNo5Name:
		case EDeviceParameterGoogolGTS_Ext9OutputNo6Name:
		case EDeviceParameterGoogolGTS_Ext9OutputNo7Name:
		case EDeviceParameterGoogolGTS_Ext9OutputNo8Name:
		case EDeviceParameterGoogolGTS_Ext9OutputNo9Name:
		case EDeviceParameterGoogolGTS_Ext9OutputNo10Name:
		case EDeviceParameterGoogolGTS_Ext9OutputNo11Name:
		case EDeviceParameterGoogolGTS_Ext9OutputNo12Name:
		case EDeviceParameterGoogolGTS_Ext9OutputNo13Name:
		case EDeviceParameterGoogolGTS_Ext9OutputNo14Name:
		case EDeviceParameterGoogolGTS_Ext9OutputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				int nIndex = nParam - EDeviceParameterGoogolGTS_Ext9OutputNo0Name;
				int nModule = 8;

				if(SetParamValue(nParam, strValue))
				{
					m_ppStrExtOutputName[nModule][nIndex] = strValue;
					bReturn = SaveSettings(nParam);
				}

				if(bReturn)
				{
					CString strFormat;
					strFormat.Format(_T("%d : %s"), nIndex, strValue);
					(m_pDlgExtDioViewer + nModule)->SetLabelString(EDeviceAddEnumeratedControlID_ExtOutputLedLabelStart + nIndex + (GTS_EXTIO_PORT * nModule), strFormat);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterGoogolGTS_Ext10OutputNo0Name:
		case EDeviceParameterGoogolGTS_Ext10OutputNo1Name:
		case EDeviceParameterGoogolGTS_Ext10OutputNo2Name:
		case EDeviceParameterGoogolGTS_Ext10OutputNo3Name:
		case EDeviceParameterGoogolGTS_Ext10OutputNo4Name:
		case EDeviceParameterGoogolGTS_Ext10OutputNo5Name:
		case EDeviceParameterGoogolGTS_Ext10OutputNo6Name:
		case EDeviceParameterGoogolGTS_Ext10OutputNo7Name:
		case EDeviceParameterGoogolGTS_Ext10OutputNo8Name:
		case EDeviceParameterGoogolGTS_Ext10OutputNo9Name:
		case EDeviceParameterGoogolGTS_Ext10OutputNo10Name:
		case EDeviceParameterGoogolGTS_Ext10OutputNo11Name:
		case EDeviceParameterGoogolGTS_Ext10OutputNo12Name:
		case EDeviceParameterGoogolGTS_Ext10OutputNo13Name:
		case EDeviceParameterGoogolGTS_Ext10OutputNo14Name:
		case EDeviceParameterGoogolGTS_Ext10OutputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				int nIndex = nParam - EDeviceParameterGoogolGTS_Ext10OutputNo0Name;
				int nModule = 9;

				if(SetParamValue(nParam, strValue))
				{
					m_ppStrExtOutputName[nModule][nIndex] = strValue;
					bReturn = SaveSettings(nParam);
				}

				if(bReturn)
				{
					CString strFormat;
					strFormat.Format(_T("%d : %s"), nIndex, strValue);
					(m_pDlgExtDioViewer + nModule)->SetLabelString(EDeviceAddEnumeratedControlID_ExtOutputLedLabelStart + nIndex + (GTS_EXTIO_PORT * nModule), strFormat);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterGoogolGTS_Ext11OutputNo0Name:
		case EDeviceParameterGoogolGTS_Ext11OutputNo1Name:
		case EDeviceParameterGoogolGTS_Ext11OutputNo2Name:
		case EDeviceParameterGoogolGTS_Ext11OutputNo3Name:
		case EDeviceParameterGoogolGTS_Ext11OutputNo4Name:
		case EDeviceParameterGoogolGTS_Ext11OutputNo5Name:
		case EDeviceParameterGoogolGTS_Ext11OutputNo6Name:
		case EDeviceParameterGoogolGTS_Ext11OutputNo7Name:
		case EDeviceParameterGoogolGTS_Ext11OutputNo8Name:
		case EDeviceParameterGoogolGTS_Ext11OutputNo9Name:
		case EDeviceParameterGoogolGTS_Ext11OutputNo10Name:
		case EDeviceParameterGoogolGTS_Ext11OutputNo11Name:
		case EDeviceParameterGoogolGTS_Ext11OutputNo12Name:
		case EDeviceParameterGoogolGTS_Ext11OutputNo13Name:
		case EDeviceParameterGoogolGTS_Ext11OutputNo14Name:
		case EDeviceParameterGoogolGTS_Ext11OutputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				int nIndex = nParam - EDeviceParameterGoogolGTS_Ext11OutputNo0Name;
				int nModule = 10;

				if(SetParamValue(nParam, strValue))
				{
					m_ppStrExtOutputName[nModule][nIndex] = strValue;
					bReturn = SaveSettings(nParam);
				}

				if(bReturn)
				{
					CString strFormat;
					strFormat.Format(_T("%d : %s"), nIndex, strValue);
					(m_pDlgExtDioViewer + nModule)->SetLabelString(EDeviceAddEnumeratedControlID_ExtOutputLedLabelStart + nIndex + (GTS_EXTIO_PORT * nModule), strFormat);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterGoogolGTS_Ext12OutputNo0Name:
		case EDeviceParameterGoogolGTS_Ext12OutputNo1Name:
		case EDeviceParameterGoogolGTS_Ext12OutputNo2Name:
		case EDeviceParameterGoogolGTS_Ext12OutputNo3Name:
		case EDeviceParameterGoogolGTS_Ext12OutputNo4Name:
		case EDeviceParameterGoogolGTS_Ext12OutputNo5Name:
		case EDeviceParameterGoogolGTS_Ext12OutputNo6Name:
		case EDeviceParameterGoogolGTS_Ext12OutputNo7Name:
		case EDeviceParameterGoogolGTS_Ext12OutputNo8Name:
		case EDeviceParameterGoogolGTS_Ext12OutputNo9Name:
		case EDeviceParameterGoogolGTS_Ext12OutputNo10Name:
		case EDeviceParameterGoogolGTS_Ext12OutputNo11Name:
		case EDeviceParameterGoogolGTS_Ext12OutputNo12Name:
		case EDeviceParameterGoogolGTS_Ext12OutputNo13Name:
		case EDeviceParameterGoogolGTS_Ext12OutputNo14Name:
		case EDeviceParameterGoogolGTS_Ext12OutputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				int nIndex = nParam - EDeviceParameterGoogolGTS_Ext12OutputNo0Name;
				int nModule = 11;

				if(SetParamValue(nParam, strValue))
				{
					m_ppStrExtOutputName[nModule][nIndex] = strValue;
					bReturn = SaveSettings(nParam);
				}

				if(bReturn)
				{
					CString strFormat;
					strFormat.Format(_T("%d : %s"), nIndex, strValue);
					(m_pDlgExtDioViewer + nModule)->SetLabelString(EDeviceAddEnumeratedControlID_ExtOutputLedLabelStart + nIndex + (GTS_EXTIO_PORT * nModule), strFormat);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterGoogolGTS_Ext13OutputNo0Name:
		case EDeviceParameterGoogolGTS_Ext13OutputNo1Name:
		case EDeviceParameterGoogolGTS_Ext13OutputNo2Name:
		case EDeviceParameterGoogolGTS_Ext13OutputNo3Name:
		case EDeviceParameterGoogolGTS_Ext13OutputNo4Name:
		case EDeviceParameterGoogolGTS_Ext13OutputNo5Name:
		case EDeviceParameterGoogolGTS_Ext13OutputNo6Name:
		case EDeviceParameterGoogolGTS_Ext13OutputNo7Name:
		case EDeviceParameterGoogolGTS_Ext13OutputNo8Name:
		case EDeviceParameterGoogolGTS_Ext13OutputNo9Name:
		case EDeviceParameterGoogolGTS_Ext13OutputNo10Name:
		case EDeviceParameterGoogolGTS_Ext13OutputNo11Name:
		case EDeviceParameterGoogolGTS_Ext13OutputNo12Name:
		case EDeviceParameterGoogolGTS_Ext13OutputNo13Name:
		case EDeviceParameterGoogolGTS_Ext13OutputNo14Name:
		case EDeviceParameterGoogolGTS_Ext13OutputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				int nIndex = nParam - EDeviceParameterGoogolGTS_Ext13OutputNo0Name;
				int nModule = 12;

				if(SetParamValue(nParam, strValue))
				{
					m_ppStrExtOutputName[nModule][nIndex] = strValue;
					bReturn = SaveSettings(nParam);
				}

				if(bReturn)
				{
					CString strFormat;
					strFormat.Format(_T("%d : %s"), nIndex, strValue);
					(m_pDlgExtDioViewer + nModule)->SetLabelString(EDeviceAddEnumeratedControlID_ExtOutputLedLabelStart + nIndex + (GTS_EXTIO_PORT * nModule), strFormat);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterGoogolGTS_Ext14OutputNo0Name:
		case EDeviceParameterGoogolGTS_Ext14OutputNo1Name:
		case EDeviceParameterGoogolGTS_Ext14OutputNo2Name:
		case EDeviceParameterGoogolGTS_Ext14OutputNo3Name:
		case EDeviceParameterGoogolGTS_Ext14OutputNo4Name:
		case EDeviceParameterGoogolGTS_Ext14OutputNo5Name:
		case EDeviceParameterGoogolGTS_Ext14OutputNo6Name:
		case EDeviceParameterGoogolGTS_Ext14OutputNo7Name:
		case EDeviceParameterGoogolGTS_Ext14OutputNo8Name:
		case EDeviceParameterGoogolGTS_Ext14OutputNo9Name:
		case EDeviceParameterGoogolGTS_Ext14OutputNo10Name:
		case EDeviceParameterGoogolGTS_Ext14OutputNo11Name:
		case EDeviceParameterGoogolGTS_Ext14OutputNo12Name:
		case EDeviceParameterGoogolGTS_Ext14OutputNo13Name:
		case EDeviceParameterGoogolGTS_Ext14OutputNo14Name:
		case EDeviceParameterGoogolGTS_Ext14OutputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				int nIndex = nParam - EDeviceParameterGoogolGTS_Ext14OutputNo0Name;
				int nModule = 13;

				if(SetParamValue(nParam, strValue))
				{
					m_ppStrExtOutputName[nModule][nIndex] = strValue;
					bReturn = SaveSettings(nParam);
				}

				if(bReturn)
				{
					CString strFormat;
					strFormat.Format(_T("%d : %s"), nIndex, strValue);
					(m_pDlgExtDioViewer + nModule)->SetLabelString(EDeviceAddEnumeratedControlID_ExtOutputLedLabelStart + nIndex + (GTS_EXTIO_PORT * nModule), strFormat);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterGoogolGTS_Ext15OutputNo0Name:
		case EDeviceParameterGoogolGTS_Ext15OutputNo1Name:
		case EDeviceParameterGoogolGTS_Ext15OutputNo2Name:
		case EDeviceParameterGoogolGTS_Ext15OutputNo3Name:
		case EDeviceParameterGoogolGTS_Ext15OutputNo4Name:
		case EDeviceParameterGoogolGTS_Ext15OutputNo5Name:
		case EDeviceParameterGoogolGTS_Ext15OutputNo6Name:
		case EDeviceParameterGoogolGTS_Ext15OutputNo7Name:
		case EDeviceParameterGoogolGTS_Ext15OutputNo8Name:
		case EDeviceParameterGoogolGTS_Ext15OutputNo9Name:
		case EDeviceParameterGoogolGTS_Ext15OutputNo10Name:
		case EDeviceParameterGoogolGTS_Ext15OutputNo11Name:
		case EDeviceParameterGoogolGTS_Ext15OutputNo12Name:
		case EDeviceParameterGoogolGTS_Ext15OutputNo13Name:
		case EDeviceParameterGoogolGTS_Ext15OutputNo14Name:
		case EDeviceParameterGoogolGTS_Ext15OutputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				int nIndex = nParam - EDeviceParameterGoogolGTS_Ext15OutputNo0Name;
				int nModule = 14;

				if(SetParamValue(nParam, strValue))
				{
					m_ppStrExtOutputName[nModule][nIndex] = strValue;
					bReturn = SaveSettings(nParam);
				}

				if(bReturn)
				{
					CString strFormat;
					strFormat.Format(_T("%d : %s"), nIndex, strValue);
					(m_pDlgExtDioViewer + nModule)->SetLabelString(EDeviceAddEnumeratedControlID_ExtOutputLedLabelStart + nIndex + (GTS_EXTIO_PORT * nModule), strFormat);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterGoogolGTS_Ext16OutputNo0Name:
		case EDeviceParameterGoogolGTS_Ext16OutputNo1Name:
		case EDeviceParameterGoogolGTS_Ext16OutputNo2Name:
		case EDeviceParameterGoogolGTS_Ext16OutputNo3Name:
		case EDeviceParameterGoogolGTS_Ext16OutputNo4Name:
		case EDeviceParameterGoogolGTS_Ext16OutputNo5Name:
		case EDeviceParameterGoogolGTS_Ext16OutputNo6Name:
		case EDeviceParameterGoogolGTS_Ext16OutputNo7Name:
		case EDeviceParameterGoogolGTS_Ext16OutputNo8Name:
		case EDeviceParameterGoogolGTS_Ext16OutputNo9Name:
		case EDeviceParameterGoogolGTS_Ext16OutputNo10Name:
		case EDeviceParameterGoogolGTS_Ext16OutputNo11Name:
		case EDeviceParameterGoogolGTS_Ext16OutputNo12Name:
		case EDeviceParameterGoogolGTS_Ext16OutputNo13Name:
		case EDeviceParameterGoogolGTS_Ext16OutputNo14Name:
		case EDeviceParameterGoogolGTS_Ext16OutputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				int nIndex = nParam - EDeviceParameterGoogolGTS_Ext16OutputNo0Name;
				int nModule = 15;

				if(SetParamValue(nParam, strValue))
				{
					m_ppStrExtOutputName[nModule][nIndex] = strValue;
					bReturn = SaveSettings(nParam);
				}

				if(bReturn)
				{
					CString strFormat;
					strFormat.Format(_T("%d : %s"), nIndex, strValue);
					(m_pDlgExtDioViewer + nModule)->SetLabelString(EDeviceAddEnumeratedControlID_ExtOutputLedLabelStart + nIndex + (GTS_EXTIO_PORT * nModule), strFormat);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGoogolGTS[nParam], strPreValue, strValue);
			}
			break;
		default:
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtofindtheparameter);
			break;
		}

		if(bUIUpdate)
		{
			MakeViewThreadDisplayer(pDisp)
			{
				AddControls();
			};

			AddThreadDisplayer(pDisp);
		}
	}
	while(false);

	if(!bFoundID)
	{
		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);
	}
	
	return bReturn;
}

bool CDeviceGoogolGTS::DoesModuleExist()
{
	bool bReturn = false;

	CString strModuleName;

	do
	{
		strModuleName.Format(_T("gts.dll"));

		EDeviceLibraryStatus eStatus = CLibraryManager::GetFoundLibrary(strModuleName);

		if(!eStatus)
		{
			HMODULE hModule = LoadLibrary(strModuleName);

			if(!hModule)
			{
				CLibraryManager::SetFoundLibrary(strModuleName, false);
				break;
			}

			FreeLibrary(hModule);

			CLibraryManager::SetFoundLibrary(strModuleName, true);

			bReturn = true;
		}
		else
			bReturn = eStatus == EDeviceLibraryStatus_FoundTrue ? true : false;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, CMultiLanguageManager::GetString(ELanguageParameter__s_d_s_toload_s), GetClassNameStr(), GetObjectID(), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), strModuleName);

	return bReturn;
}

void CDeviceGoogolGTS::BroadcastViewInit()
{
	do
	{
		if(!m_pDlgExtDioViewer)
		   break;

		bool bError = false;

		for(int i = 0; i < GTS_EXTIO_MODULES; ++i)
		{
			if(!(m_pDlgExtDioViewer + i))
			{
				bError = true;
				break;
			}

			(m_pDlgExtDioViewer + i)->SetInitialize(m_bIsInitialized);
		}

		if(bError)
			break;
	}
	while(false);
}

bool CDeviceGoogolGTS::AddControls()
{
	bool bReturn = false;
	CDC* pDC = nullptr;
	CFont* pOldFont = nullptr;

	LockWindowUpdate();

	do
	{
		pDC = GetDC();

		if(!pDC)
			break;

		for(auto iter = m_vctCtrl.begin(); iter != m_vctCtrl.end(); ++iter)
			delete *iter;

		m_vctCtrl.clear();
					
		CWnd* pWndParamTree = GetDlgItem(IDC_SHEET_RAVID_DEVICE_PARAM);
		if(!pWndParamTree)
			break;

		CWnd* pWndInitBtn = GetDlgItem(IDC_BTN_RAVID_DEVICE_INITIALIZE);
		if(!pWndInitBtn)
			break;

		CWnd* pWndTerBtn = GetDlgItem(IDC_BTN_RAVID_DEVICE_TERMINATE);
		if(!pWndTerBtn)
			break;

		CFont* pBaseFont = pWndInitBtn->GetFont();
		if(!pBaseFont)
			break;

		double dblDPIScale = DPISCALE;

		pOldFont = pDC->SelectObject(pBaseFont);

		CRavidRect<int> rrDlg, rrSheet, rrInitBtn, rrTerBtn;

		GetClientRect(rrDlg);

		pWndParamTree->GetWindowRect(rrSheet);
		ScreenToClient(rrSheet);

		pWndInitBtn->GetWindowRect(rrInitBtn);
		ScreenToClient(rrInitBtn);

		pWndTerBtn->GetWindowRect(rrTerBtn);
		ScreenToClient(rrTerBtn);

		int nDialogEdgeMargin = 10 * dblDPIScale;
		int nButtonInterval = 5 * dblDPIScale;
		int nInfoTextHeight = 20 * dblDPIScale;
		int nGroupBoxVerticalMargin = 25 * dblDPIScale;
		int nMoveHeight = 45 * dblDPIScale;
		int nLedLength = 15 * dblDPIScale;
				
		CRavidRect<int> rrResultGroupBox, rrResultMotionBox;

		CRavidPoint<int> rpUILeftTop(rrSheet.right + nDialogEdgeMargin, rrInitBtn.top);
		CRavidPoint<int> rpUIRightBottom(rpUILeftTop.x + rrSheet.GetWidth(), rrSheet.bottom + nDialogEdgeMargin);
	
		bool bUsed = false;

		for(int i = 0; i < GTS_DIO_IN_PORT; ++i)
		{
			if(!m_pBUsingInputArray[i])
				continue;

			bUsed = true;
			break;
		}

		if(!bUsed)
		{
			for(int i = 0; i < GTS_DIO_OUT_PORT; ++i)
			{
				if(!m_pBUsingOutputArray[i])
					continue;

				bUsed = true;
				break;
			}
		}

 		if(bUsed)
		{
 			AddIOs(dblDPIScale, pBaseFont, rpUILeftTop, rpUIRightBottom, rrResultGroupBox);

			rpUILeftTop.SetPoint(rpUILeftTop.x, rrResultGroupBox.bottom);
			rpUIRightBottom.SetPoint(rpUILeftTop.x + rrSheet.GetWidth(), rpUILeftTop.y + nDialogEdgeMargin);
		}

		AddMotions(dblDPIScale, pBaseFont, rpUILeftTop, rpUIRightBottom, rrResultMotionBox);

		rrDlg.right = __max(__max(rrResultGroupBox.right, rrSheet.right), rrResultMotionBox.right);
		rrDlg.bottom = __max(__max(rrResultGroupBox.bottom, 500), rrResultMotionBox.bottom);

		pWndParamTree->SetWindowPos(nullptr, 0, 0, rrSheet.GetWidth(), rrDlg.GetHeight() - rrSheet.top, SWP_NOMOVE);

		rrDlg.right += nDialogEdgeMargin;
		rrDlg.bottom += nDialogEdgeMargin;

		RecalcSheet();

		SetDefaultDialogRect(rrDlg);		

		rpUILeftTop.SetPoint(nDialogEdgeMargin, nDialogEdgeMargin);
		rpUIRightBottom.SetPoint(rpUILeftTop.x + rrSheet.right - rrSheet.left, rpUILeftTop.y + nDialogEdgeMargin);

		if(m_pDlgExtDioViewer)
		{
			for(int i = 0; i < GTS_EXTIO_MODULES; ++i)
			{
				if(!(m_pDlgExtDioViewer + i))
					continue;

				if((m_pDlgExtDioViewer + i)->m_vctCtrl.size())
					continue;

				CRavidRect<int> rrExtIO;

				AddExtIOs((m_pDlgExtDioViewer + i), dblDPIScale, i * GTS_EXTIO_PORT, pBaseFont, rpUILeftTop, rpUIRightBottom, rrExtIO);

				rrExtIO.right += nDialogEdgeMargin + nDialogEdgeMargin;
				rrExtIO.bottom += nDialogEdgeMargin + nDialogEdgeMargin;

				(m_pDlgExtDioViewer + i)->SetDefaultDialogRect(rrExtIO);
			}
		}

		bReturn = true;
	}
	while(false);

	if(pDC)
	{
		pDC->SelectObject(pOldFont);
		ReleaseDC(pDC);
	}

	UnlockWindowUpdate();

	return bReturn;
}

long CDeviceGoogolGTS::GetOutputPortCount()
{
	return GTS_DIO_OUT_PORT;
}

long CDeviceGoogolGTS::GetInputPortCount()
{
	return GTS_DIO_IN_PORT;
}

long CDeviceGoogolGTS::GetExtIOPortCount()
{
	return GTS_EXTIO_PORT;
}

void CDeviceGoogolGTS::AddIOs(double dblDPIScale, CFont* pBaseFont, CRavidPoint<int> rpLeftTopPt, CRavidPoint<int> rpRightTopPt, CRavidRect<int>& rrResultGroupBox)
{
	do
	{
		CButton* pGroupBox[2];

		for(int i = 0; i < 2; ++i)
			pGroupBox[i] = new CButton;

		CRavidRect<int> rrGroupBox[2];

		rrGroupBox[0].SetRect(rpLeftTopPt.x, rpLeftTopPt.y, (rpLeftTopPt.x + rpRightTopPt.x) / 2., rpRightTopPt.y + 100 * dblDPIScale);
		rrGroupBox[1].SetRect((rpLeftTopPt.x + rpRightTopPt.x) / 2., rpLeftTopPt.y, rpRightTopPt.x, rpRightTopPt.y + 100 * dblDPIScale);

		CRavidPoint<int> rpLedBasePos(rrGroupBox[0].left + 10 * dblDPIScale, rrGroupBox[0].top + 20 * dblDPIScale);
		CRavidRect<int> rrLedRect(rpLedBasePos.x, rpLedBasePos.y, rpLedBasePos.x + 15 * dblDPIScale, rpLedBasePos.y + 15 * dblDPIScale);
		CRavidPoint<int> rpLedYGap(0, 20 * dblDPIScale);
		CRavidPoint<int> rpLedXGap(20 * dblDPIScale, 0);

		int nInputMaxLed = GetInputPortCount();
		int nOutputMaxLed = GetOutputPortCount();

		int nInputLedCreatedCount = 0;
		int nOutputLedCreatedCount = 0;

		int nRealPos = 0;

		for(int i = 0; i < nInputMaxLed; ++i)
		{
			if(!m_pBUsingInputArray[i])
				continue;

			CRavidLedCtrl* pLed = new CRavidLedCtrl;
			pLed->Create(nullptr, WS_VISIBLE | WS_CHILD | SS_NOTIFY, rrLedRect + rpLedYGap * nRealPos, this, EDeviceAddEnumeratedControlID_InputLedStart + i);
			pLed->SetFont(pBaseFont);
			m_vctCtrl.push_back(pLed);

			CStatic* pLabel = new CStatic;
			CRavidRect<int> rrLabelPosition(rrLedRect + rpLedYGap * nRealPos);
			rrLabelPosition += rpLedXGap;
			rrLabelPosition.right = rrGroupBox[0].right - 10 * dblDPIScale;

			CString strName;
			strName.Format(_T("%d : %s"), i, m_pStrInputNameArray[i]);
			pLabel->Create(strName, WS_VISIBLE | WS_CHILD | SS_NOTIFY, rrLabelPosition, this, EDeviceAddEnumeratedControlID_InputLedLabelStart + i);
			pLabel->SetFont(pBaseFont);
			m_vctCtrl.push_back(pLabel);

			++nRealPos;
		}

		nInputLedCreatedCount = nRealPos;

		rpLedBasePos.SetPoint(rrGroupBox[1].left + 10 * dblDPIScale, rrGroupBox[1].top + 20 * dblDPIScale);
		rrLedRect.SetRect(rpLedBasePos.x, rpLedBasePos.y, rpLedBasePos.x + 15 * dblDPIScale, rpLedBasePos.y + 15 * dblDPIScale);

		nRealPos = 0;

		for(int i = 0; i < nOutputMaxLed; ++i)
		{
			if(!m_pBUsingOutputArray[i])
				continue;

			CRavidLedCtrl* pLed = new CRavidLedCtrl;
			pLed->Create(nullptr, WS_VISIBLE | WS_CHILD | SS_NOTIFY, rrLedRect + rpLedYGap * nRealPos, this, EDeviceAddEnumeratedControlID_OutputLedStart + i);
			pLed->SetFont(pBaseFont);
			m_vctCtrl.push_back(pLed);

			CStatic* pLabel = new CStatic;
			CRavidRect<int> rrLabelPosition(rrLedRect + rpLedYGap * nRealPos);
			rrLabelPosition += rpLedXGap;
			rrLabelPosition.right = rrGroupBox[1].right - 10 * dblDPIScale;
			CString strName;
			strName.Format(_T("%d : %s"), i, m_pStrOutputNameArray[i]);
			pLabel->Create(strName, WS_VISIBLE | WS_CHILD | SS_NOTIFY, rrLabelPosition, this, EDeviceAddEnumeratedControlID_OutputLedLabelStart + i);
			pLabel->SetFont(pBaseFont);
			m_vctCtrl.push_back(pLabel);

			++nRealPos;
		}

		nOutputLedCreatedCount = nRealPos;

		rrGroupBox[0].bottom = (rrLedRect + rpLedYGap * (__max(nInputLedCreatedCount, nOutputLedCreatedCount) - 1)).bottom + 10 * dblDPIScale;
		rrGroupBox[1].bottom = (rrLedRect + rpLedYGap * (__max(nInputLedCreatedCount, nOutputLedCreatedCount) - 1)).bottom + 10 * dblDPIScale;

		for(int i = 0; i < 2; ++i)
		{
			pGroupBox[i]->Create(i ? CMultiLanguageManager::GetString(ELanguageParameter_Output) : CMultiLanguageManager::GetString(ELanguageParameter_Input), BS_GROUPBOX | WS_CHILD | WS_VISIBLE, rrGroupBox[i], this, EDeviceAddEnumeratedControlID_StaticStart + i);
			pGroupBox[i]->SetFont(pBaseFont);
			m_vctCtrl.push_back(pGroupBox[i]);
		}

		rrResultGroupBox.left = rrGroupBox[0].left;
		rrResultGroupBox.top = rrGroupBox[0].top;
		rrResultGroupBox.right = rrGroupBox[1].right;
		rrResultGroupBox.bottom = rrGroupBox[1].bottom;
	}
	while(false);
}

void CDeviceGoogolGTS:: AddExtIOs(Ravid::Framework::CGoogolDioViewer* pViewr, double dblDPIScale, int nStartID, CFont * pBaseFont, CRavidPoint<int> rpLeftTopPt, CRavidPoint<int> rpRightTopPt, CRavidRect<int>& rrResultGroupBox)
{
	do
	{
		if(!pViewr)
			break;

		CButton* pGroupBox[2];

		for(int i = 0; i < 2; ++i)
			pGroupBox[i] = new CButton;

		CRavidRect<int> rrGroupBox[2];

		rrGroupBox[0].SetRect(rpLeftTopPt.x, rpLeftTopPt.y, (rpLeftTopPt.x + rpRightTopPt.x) / 2., rpRightTopPt.y + 100 * dblDPIScale);
		rrGroupBox[1].SetRect((rpLeftTopPt.x + rpRightTopPt.x) / 2., rpLeftTopPt.y, rpRightTopPt.x, rpRightTopPt.y + 100 * dblDPIScale);

		CRavidPoint<int> rpLedBasePos(rrGroupBox[0].left + 10 * dblDPIScale, rrGroupBox[0].top + 20 * dblDPIScale);
		CRavidRect<int> rrLedRect(rpLedBasePos.x, rpLedBasePos.y, rpLedBasePos.x + 15 * dblDPIScale, rpLedBasePos.y + 15 * dblDPIScale);
		CRavidPoint<int> rpLedYGap(0, 20 * dblDPIScale);
		CRavidPoint<int> rpLedXGap(20 * dblDPIScale, 0);

		int nExtIndex = nStartID / 16;

		if(nExtIndex > 15)
			break;

		CString* pStrInput = m_ppStrExtInputName[nExtIndex];
		CString* pStrOutput = m_ppStrExtOutputName[nExtIndex];
		
		int nIOMaxLed = GetExtIOPortCount();

		int nInputLedCreatedCount = 0;
		int nOutputLedCreatedCount = 0;

		int nRealPos = 0;

		for(int i = 0; i < nIOMaxLed; ++i)
		{
			CRavidLedCtrl* pLed = new CRavidLedCtrl;
			pLed->Create(nullptr, WS_VISIBLE | WS_CHILD | SS_NOTIFY, rrLedRect + rpLedYGap * nRealPos, pViewr, EDeviceAddEnumeratedControlID_ExtInputLedStart + i + nStartID);
			pLed->SetFont(pBaseFont);
			pViewr->m_vctCtrl.push_back(pLed);

			CStatic* pLabel = new CStatic;
			CRavidRect<int> rrLabelPosition(rrLedRect + rpLedYGap * nRealPos);
 			rrLabelPosition += rpLedXGap;
			rrLabelPosition.right = rrGroupBox[0].right - 10 * dblDPIScale;

			CString strName;
			strName.Format(_T("%d : %s"), i, pStrInput[i]);
			pLabel->Create(strName, WS_VISIBLE | WS_CHILD | SS_NOTIFY, rrLabelPosition, pViewr, EDeviceAddEnumeratedControlID_ExtInputLedLabelStart + i + nStartID);
			pLabel->SetFont(pBaseFont);
			pViewr->m_vctCtrl.push_back(pLabel);

			++nRealPos;
		}

		nInputLedCreatedCount = nRealPos;

		rpLedBasePos.SetPoint(rrGroupBox[1].left + 10 * dblDPIScale, rrGroupBox[1].top + 20 * dblDPIScale);
		rrLedRect.SetRect(rpLedBasePos.x, rpLedBasePos.y, rpLedBasePos.x + 15 * dblDPIScale, rpLedBasePos.y + 15 * dblDPIScale);

		nRealPos = 0;

		for(int i = 0; i < nIOMaxLed; ++i)
		{
			CRavidLedCtrl* pLed = new CRavidLedCtrl;
			pLed->Create(nullptr, WS_VISIBLE | WS_CHILD | SS_NOTIFY, rrLedRect + rpLedYGap * nRealPos, pViewr, EDeviceAddEnumeratedControlID_ExtOutputLedStart + i + nStartID);
			pLed->SetFont(pBaseFont);
			pViewr->m_vctCtrl.push_back(pLed);

			CStatic* pLabel = new CStatic;
			CRavidRect<int> rrLabelPosition(rrLedRect + rpLedYGap * nRealPos);
			rrLabelPosition += rpLedXGap;
			rrLabelPosition.right = rrGroupBox[1].right - 10 * dblDPIScale;
			CString strName;
			strName.Format(_T("%d : %s"), i, pStrOutput[i]);
			pLabel->Create(strName, WS_VISIBLE | WS_CHILD | SS_NOTIFY, rrLabelPosition, pViewr, EDeviceAddEnumeratedControlID_ExtOutputLedLabelStart + i + nStartID);
			pLabel->SetFont(pBaseFont);
			pViewr->m_vctCtrl.push_back(pLabel);

			++nRealPos;
		}

		nOutputLedCreatedCount = nRealPos;

		rrGroupBox[0].bottom = (rrLedRect + rpLedYGap * (__max(nInputLedCreatedCount, nOutputLedCreatedCount) - 1)).bottom + 10 * dblDPIScale;
		rrGroupBox[1].bottom = (rrLedRect + rpLedYGap * (__max(nInputLedCreatedCount, nOutputLedCreatedCount) - 1)).bottom + 10 * dblDPIScale;

		for(int i = 0; i < 2; ++i)
		{
			pGroupBox[i]->Create(i ? CMultiLanguageManager::GetString(ELanguageParameter_Output) : CMultiLanguageManager::GetString(ELanguageParameter_Input), BS_GROUPBOX | WS_CHILD | WS_VISIBLE, rrGroupBox[i], pViewr, EDeviceAddEnumeratedControlID_StaticStart + i);
			pGroupBox[i]->SetFont(pBaseFont);
			pViewr->m_vctCtrl.push_back(pGroupBox[i]);
		}

		rrResultGroupBox.left = rrGroupBox[0].left;
		rrResultGroupBox.top = rrGroupBox[0].top;
		rrResultGroupBox.right = rrGroupBox[1].right;
		rrResultGroupBox.bottom = rrGroupBox[1].bottom;
	}
	while(false);
}

void CDeviceGoogolGTS::AddMotions(double dblDPIScale, CFont * pBaseFont, CRavidPoint<int> rpLeftTopPt, CRavidPoint<int> rpRightTopPt, CRavidRect<int>& rrResultGroupBox)
{
	do 
	{
		int nDialogEdgeMargin = 10 * dblDPIScale;
		int nButtonInterval = 5 * dblDPIScale;
		int nInfoTextHeight = 20 * dblDPIScale;
		int nGroupBoxVerticalMargin = 25 * dblDPIScale;
		int nMoveHeight = 45 * dblDPIScale;
		int nLedLength = 15 * dblDPIScale;

		CRavidRect<int> rrGrpMotionSelect, rrCapMotionSelect, rrBtnMotionSelect;

		rrGrpMotionSelect.left = rpLeftTopPt.x;
		rrGrpMotionSelect.top = rpLeftTopPt.y + nDialogEdgeMargin;
		rrGrpMotionSelect.right = rpRightTopPt.x;
		rrGrpMotionSelect.bottom = rrGrpMotionSelect.top + (nInfoTextHeight * 2);

		CButton* pGrpMotionCtrl = new CButton;

		if(!pGrpMotionCtrl)
			break;

		pGrpMotionCtrl->Create(CMultiLanguageManager::GetString(ELanguageParameter_Axis) + _T(" ") + CMultiLanguageManager::GetString(ELanguageParameter_Selector), BS_GROUPBOX | WS_CHILD | WS_VISIBLE, rrGrpMotionSelect, this, EDeviceAddEnumeratedControlID_StaticStart + 2);
		pGrpMotionCtrl->SetFont(pBaseFont);
		m_vctCtrl.push_back(pGrpMotionCtrl);

		rrCapMotionSelect.left = rpLeftTopPt.x + nMoveHeight;
		rrCapMotionSelect.top = rrGrpMotionSelect.top + (17 * dblDPIScale);
		rrCapMotionSelect.right = rrCapMotionSelect.left + nMoveHeight;
		rrCapMotionSelect.bottom = rrCapMotionSelect.top + nInfoTextHeight;

		CStatic* pCtnMotionCtrl = new CStatic;

		if(!pCtnMotionCtrl)
			break;

		pCtnMotionCtrl->Create(CMultiLanguageManager::GetString(ELanguageParameter_Axis), WS_CHILD | WS_VISIBLE | ES_CENTER, rrCapMotionSelect, this, EDeviceAddEnumeratedControlID_StaticStart + 3);
		pCtnMotionCtrl->SetFont(pBaseFont);
		m_vctCtrl.push_back(pCtnMotionCtrl);

		rrBtnMotionSelect.left = rrCapMotionSelect.right + (60 * dblDPIScale);
		rrBtnMotionSelect.top = rrGrpMotionSelect.top + (13 * dblDPIScale);
		rrBtnMotionSelect.right = rrGrpMotionSelect.right - nDialogEdgeMargin;
		rrBtnMotionSelect.bottom = rrCapMotionSelect.bottom + (nInfoTextHeight * 10);

		CComboBox* pBtnMotionCtrl = new CComboBox;

		if(!pBtnMotionCtrl)
			break;

		pBtnMotionCtrl->Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | ES_CENTER, rrBtnMotionSelect, this, EDeviceAddControlID_ComboAxisSelect);
		pBtnMotionCtrl->SetFont(pBaseFont);

		for(int i = 0; i < GTS_MOTION_MAX; ++i)
		{
			CString strAdd;
			strAdd.Format(_T("%d"), i + 1);
			pBtnMotionCtrl->AddString(strAdd);
		}

		pBtnMotionCtrl->SetCurSel(0);

		m_vctCtrl.push_back(pBtnMotionCtrl);

		CRavidRect<int> rrGrpControl, rrGrpStatus, rrGrpCmdPos, rrGrpActPos, rrGrpAlarmReset;
		CRavidRect<int> rrBtnMinus, rrBtnPause, rrBtnPlus;
		CRavidRect<int> rrRadioJog, rrRadioMove, rrRadioPosition;
		CRavidRect<int> rrCaptionVel, rrCaptionAccel, rrCaptionDistance, rrCaptionPosition;
		CRavidRect<int> rrEditVel, rrEditAccel, rrEditDistance, rrEditPosition;
		CRavidRect<int> rrLedRect(0, 0, nLedLength, nLedLength);
		CRavidRect<int> rrCaptionStatus[5];
		CRavidRect<int> rrCaptionCmdValue, rrCaptionActValue;
		CRavidRect<int> rrBtnCmdReset, rrBtnActReset, rrBtnAlarmReset;
		CRavidRect<int> rrBtnServoOn, rrBtnServoOff, rrGrpServo, rrBtnHome, rrGrpHome;

		int nGroupIntrvalWidth = (rpRightTopPt.x - rpLeftTopPt.x- (nDialogEdgeMargin * 2)) / 10;
		int nGroupIntrvalWidth2 = nGroupIntrvalWidth * 4.125;

		rrGrpControl.left = rpLeftTopPt.x;
		rrGrpControl.top = rrGrpMotionSelect.bottom + nDialogEdgeMargin;
		rrGrpControl.right = rpRightTopPt.x;
//		rrGrpControl.bottom = rrGrpControl.top + (nGroupBoxVerticalMargin + nInfoTextHeight * 3.4);
		rrGrpControl.bottom = rrGrpControl.top + (nGroupBoxVerticalMargin + nInfoTextHeight * 4.0);

		rrGrpStatus.left = rrGrpControl.left;
		rrGrpStatus.top = rrGrpControl.bottom + nDialogEdgeMargin;
		rrGrpStatus.right = rrGrpStatus.left + nGroupIntrvalWidth * 6 + nDialogEdgeMargin;
		rrGrpStatus.bottom = rrGrpStatus.top + (nGroupBoxVerticalMargin + nInfoTextHeight * 2);

		rrGrpHome.left = rrGrpStatus.right + nDialogEdgeMargin;
		rrGrpHome.top = rrGrpStatus.top;
		rrGrpHome.right = rrGrpHome.left + nGroupIntrvalWidth * 2;
		rrGrpHome.bottom = rrGrpStatus.bottom;

		rrGrpServo.left = rrGrpHome.right + nDialogEdgeMargin;
		rrGrpServo.top = rrGrpStatus.top;
		rrGrpServo.right = rpRightTopPt.x;
		rrGrpServo.bottom = rrGrpStatus.bottom;

		//////////////////////////////////////////////////////////////////////////
		// move button

		int nBtnSize = 48 * dblDPIScale;

		rrBtnMinus.left = rrGrpControl.left + nDialogEdgeMargin;
//		rrBtnMinus.top = rrGrpControl.top + 25 * dblDPIScale;
		rrBtnMinus.top = rrGrpControl.top + 32 * dblDPIScale;
		rrBtnMinus.right = rrBtnMinus.left + nBtnSize;
		rrBtnMinus.bottom = rrBtnMinus.top + nBtnSize;

		rrBtnPause.left = rrBtnMinus.right + nDialogEdgeMargin;
		rrBtnPause.top = rrBtnMinus.top;
		rrBtnPause.right = rrBtnPause.left + nBtnSize;
		rrBtnPause.bottom = rrBtnMinus.bottom;

		rrBtnPlus.left = rrBtnPause.right + nDialogEdgeMargin;
		rrBtnPlus.top = rrBtnMinus.top;
		rrBtnPlus.right = rrBtnPlus.left + nBtnSize;
		rrBtnPlus.bottom = rrBtnMinus.bottom;

		//////////////////////////////////////////////////////////////////////////
		// Radio button
		
		rrRadioJog.left = rrBtnPlus.right + nDialogEdgeMargin;
		rrRadioJog.top = rrGrpControl.top + 23 * dblDPIScale;
		rrRadioJog.right = rrRadioJog.left + nBtnSize + 10;
		rrRadioJog.bottom = rrRadioJog.top + nInfoTextHeight;

		rrRadioMove.left = rrRadioJog.left;
		rrRadioMove.top = rrRadioJog.bottom + 5 * dblDPIScale;
		rrRadioMove.right = rrRadioJog.right;
		rrRadioMove.bottom = rrRadioMove.top + nInfoTextHeight;

		rrRadioPosition.left = rrRadioJog.left;
		rrRadioPosition.top = rrRadioMove.bottom + 5 * dblDPIScale;
		rrRadioPosition.right = rrRadioJog.right;
		rrRadioPosition.bottom = rrRadioPosition.top + nInfoTextHeight;

		//////////////////////////////////////////////////////////////////////////
		// vel, acc, dis caption

		int nCaptionWidth = 70 * dblDPIScale;

		rrCaptionVel.left = rrRadioJog.right + nDialogEdgeMargin;
		rrCaptionVel.top = rrGrpControl.top + 14 * dblDPIScale;
		rrCaptionVel.right = rrCaptionVel.left + nCaptionWidth;
		rrCaptionVel.bottom = rrCaptionVel.top + nInfoTextHeight;

		rrCaptionAccel.left = rrCaptionVel.left;
		rrCaptionAccel.top = rrCaptionVel.bottom + 2 * dblDPIScale;
		rrCaptionAccel.right = rrCaptionVel.right;
		rrCaptionAccel.bottom = rrCaptionAccel.top + nInfoTextHeight;

		rrCaptionDistance.left = rrCaptionVel.left;
		rrCaptionDistance.top = rrCaptionAccel.bottom + 2 * dblDPIScale;
		rrCaptionDistance.right = rrCaptionVel.right;
		rrCaptionDistance.bottom = rrCaptionDistance.top + nInfoTextHeight;

		rrCaptionPosition.left = rrCaptionVel.left;
		rrCaptionPosition.top = rrCaptionDistance.bottom + 2 * dblDPIScale;
		rrCaptionPosition.right = rrCaptionVel.right;
		rrCaptionPosition.bottom = rrCaptionPosition.top + nInfoTextHeight;

		//////////////////////////////////////////////////////////////////////////
		// vel, acc, dis editbox

		int nEditWidth = 120 * dblDPIScale;
		int nEditInterval = 2 * dblDPIScale;

		rrEditVel.left = rrCaptionVel.right + nDialogEdgeMargin;
		rrEditVel.top = rrCaptionVel.top - nEditInterval;
		rrEditVel.right = rrGrpControl.right - nDialogEdgeMargin;
		rrEditVel.bottom = rrCaptionVel.bottom - nEditInterval;

		rrEditAccel.left = rrEditVel.left;
		rrEditAccel.top = rrCaptionAccel.top - nEditInterval;
		rrEditAccel.right = rrEditVel.right;
		rrEditAccel.bottom = rrCaptionAccel.bottom - nEditInterval;

		rrEditDistance.left = rrEditVel.left;
		rrEditDistance.top = rrCaptionDistance.top - nEditInterval;
		rrEditDistance.right = rrEditVel.right;
		rrEditDistance.bottom = rrCaptionDistance.bottom - nEditInterval;

		rrEditPosition.left = rrEditVel.left;
		rrEditPosition.top = rrCaptionPosition.top - nEditInterval;
		rrEditPosition.right = rrEditVel.right;
		rrEditPosition.bottom = rrCaptionPosition.bottom - nEditInterval;

		CButton* pBtnGroupCtrl = new CButton;

		if(!pBtnGroupCtrl)
			break;

		pBtnGroupCtrl->Create(CMultiLanguageManager::GetString(ELanguageParameter_MotorControlling), BS_GROUPBOX | WS_CHILD | WS_VISIBLE, rrGrpControl, this, EDeviceAddEnumeratedControlID_StaticStart + 4);
		pBtnGroupCtrl->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnGroupCtrl);

		CButton* pBtnGroupStatus = new CButton;

		if(!pBtnGroupStatus)
			break;

		pBtnGroupStatus->Create(CMultiLanguageManager::GetString(ELanguageParameter_SensorStatus), BS_GROUPBOX | WS_CHILD | WS_VISIBLE, rrGrpStatus, this, EDeviceAddEnumeratedControlID_StaticStart + 5);
		pBtnGroupStatus->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnGroupStatus);

		CButton* pBtnGroupServo = new CButton;

		if(!pBtnGroupServo)
			break;

		pBtnGroupServo->Create(CMultiLanguageManager::GetString(ELanguageParameter_ServoMode), BS_GROUPBOX | WS_CHILD | WS_VISIBLE, rrGrpServo, this, EDeviceAddEnumeratedControlID_StaticStart + 6);
		pBtnGroupServo->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnGroupServo);


		CButton* pBtnGroupHome = new CButton;

		if(!pBtnGroupHome)
			break;

		pBtnGroupHome->Create(CMultiLanguageManager::GetString(ELanguageParameter_Home), BS_GROUPBOX | WS_CHILD | WS_VISIBLE, rrGrpHome, this, EDeviceAddEnumeratedControlID_StaticStart + 7);
		pBtnGroupHome->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnGroupHome);


		CButton* pBtnMinus = new CButton;

		if(!pBtnMinus)
			break;

		pBtnMinus->Create(_T("◀"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_FLAT, rrBtnMinus, this, EDeviceAddControlID_BtnMinus);
		pBtnMinus->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnMinus);

		CButton* pBtnPause = new CButton;

		if(!pBtnPause)
			break;

		pBtnPause->Create(_T("■"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_FLAT, rrBtnPause, this, EDeviceAddControlID_BtnPause);
		pBtnPause->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnPause);

		CButton* pBtnPlus = new CButton;

		if(!pBtnPlus)
			break;

		pBtnPlus->Create(_T("▶"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_FLAT, rrBtnPlus, this, EDeviceAddControlID_BtnPlus);
		pBtnPlus->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnPlus);

		CButton* pBtnJog = new CButton;

		if(!pBtnJog)
			break;

		pBtnJog->Create(_T("Jog"), WS_CHILD | WS_VISIBLE | WS_GROUP | BS_AUTORADIOBUTTON, rrRadioJog, this, EDeviceAddControlID_BtnJog);
		pBtnJog->SetFont(pBaseFont);
		pBtnJog->SetCheck(true);
		m_vctCtrl.push_back(pBtnJog);

		CButton* pBtnMove = new CButton;

		if(!pBtnMove)
			break;

		pBtnMove->Create(_T("Move"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, rrRadioMove, this, EDeviceAddControlID_BtnMove);
		pBtnMove->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnMove);

		CButton* pBtnPosition = new CButton;

		if(!pBtnPosition)
			break;

		pBtnPosition->Create(_T("Position"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, rrRadioPosition, this, EDeviceAddControlID_BtnPosition);
		pBtnPosition->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnPosition);

		CStatic* pCtnVel = new CStatic;

		if(!pCtnVel)
			break;

		pCtnVel->Create(CMultiLanguageManager::GetString(ELanguageParameter_Velocity) + _T(" :"), WS_CHILD | WS_VISIBLE | DT_RIGHT, rrCaptionVel, this, EDeviceAddEnumeratedControlID_StaticStart + 8);
		pCtnVel->SetFont(pBaseFont);
		m_vctCtrl.push_back(pCtnVel);

		CStatic* pCtnAccel = new CStatic;

		if(!pCtnAccel)
			break;

		pCtnAccel->Create(CMultiLanguageManager::GetString(ELanguageParameter_Acceleration) + _T(" :"), WS_CHILD | WS_VISIBLE | DT_RIGHT, rrCaptionAccel, this, EDeviceAddEnumeratedControlID_StaticStart + 9);
		pCtnAccel->SetFont(pBaseFont);
		m_vctCtrl.push_back(pCtnAccel);

		CStatic* pCtnDistance = new CStatic;

		if(!pCtnDistance)
			break;

		pCtnDistance->Create(CMultiLanguageManager::GetString(ELanguageParameter_Distance) + _T(" :"), WS_CHILD | WS_VISIBLE | DT_RIGHT, rrCaptionDistance, this, EDeviceAddEnumeratedControlID_StaticStart + 10);
		pCtnDistance->SetFont(pBaseFont);
		m_vctCtrl.push_back(pCtnDistance);

		CStatic* pCtnPosition = new CStatic;

		if(!pCtnPosition)
			break;

		pCtnPosition->Create(CMultiLanguageManager::GetString(ELanguageParameter_Position) + _T(" :"), WS_CHILD | WS_VISIBLE | DT_RIGHT, rrCaptionPosition, this, EDeviceAddEnumeratedControlID_StaticStart + 11);
		pCtnPosition->SetFont(pBaseFont);
		m_vctCtrl.push_back(pCtnPosition);

		CEdit* pEditVel = new CEdit;

		if(!pEditVel)
			break;

		pEditVel->Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_LEFT, rrEditVel, this, EDeviceAddControlID_EditVelocity);
		pEditVel->SetFont(pBaseFont);
		pEditVel->SetWindowTextW(_T("0.0"));
		m_vctCtrl.push_back(pEditVel);

		CEdit* pEditAccel = new CEdit;

		if(!pEditAccel)
			break;

		pEditAccel->Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_LEFT, rrEditAccel, this, EDeviceAddControlID_EditAcceleration);
		pEditAccel->SetFont(pBaseFont);
		pEditAccel->SetWindowTextW(_T("0.0"));
		m_vctCtrl.push_back(pEditAccel);

		CEdit* pEditDistance = new CEdit;

		if(!pEditDistance)
			break;

		pEditDistance->Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_LEFT, rrEditDistance, this, EDeviceAddControlID_EditDistance);
		pEditDistance->SetFont(pBaseFont);
		pEditDistance->SetWindowTextW(_T("0.0"));
		m_vctCtrl.push_back(pEditDistance);

		CEdit* pEditPosition = new CEdit;

		if(!pEditPosition)
			break;

		pEditPosition->Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_LEFT, rrEditPosition, this, EDeviceAddControlID_EditPosition);
		pEditPosition->SetFont(pBaseFont);
		pEditPosition->SetWindowTextW(_T("0.0"));
		m_vctCtrl.push_back(pEditPosition);

		//////////////////////////////////////////////////////////////////////////
		// LED Box

		bool bError = false;
		int nLedPosY = 22 * dblDPIScale;
		int nLedIntervalWidth = (rrGrpStatus.right - rrGrpStatus.left) * 0.1;
		int nLedWidth = nLedIntervalWidth * 2;
		int nHalfLedSize = 7 * dblDPIScale;

		rrLedRect.Offset(rrGrpStatus.left + nLedIntervalWidth - nHalfLedSize, rrGrpStatus.top + nLedPosY);

		int nCaptionStart = nBtnSize * 0.5 - nHalfLedSize;

		for(int i = 0; i < 5; ++i)
		{
			CRavidLedCtrl* pLedCtrl = new CRavidLedCtrl;

			if(!pLedCtrl)
			{
				bError = true;
				break;
			}

			pLedCtrl->Create(nullptr, WS_VISIBLE | WS_CHILD | SS_NOTIFY, rrLedRect, this, EDeviceAddControlID_LedLimitPlus + i);
			pLedCtrl->SetFont(pBaseFont);
			m_vctCtrl.push_back(pLedCtrl);

			rrCaptionStatus[i].left = rrLedRect.left - nCaptionStart + 1;
			rrCaptionStatus[i].top = rrLedRect.bottom + nButtonInterval;
			rrCaptionStatus[i].right = rrCaptionStatus[i].left + nBtnSize;
			rrCaptionStatus[i].bottom = rrCaptionStatus[i].top + nInfoTextHeight;

			rrLedRect.left += nLedWidth;
			rrLedRect.right += nLedWidth;
		}

		if(bError)
			break;

		CStatic* pCtnStatusPluse = new CStatic;

		if(!pCtnStatusPluse)
			break;

		pCtnStatusPluse->Create(CMultiLanguageManager::GetString(ELanguageParameter_LimitPlus), WS_CHILD | WS_VISIBLE | ES_CENTER, rrCaptionStatus[0], this, EDeviceAddEnumeratedControlID_StaticStart + 12);
		pCtnStatusPluse->SetFont(pBaseFont);
		m_vctCtrl.push_back(pCtnStatusPluse);

		CStatic* pCtnStatusHome = new CStatic;

		if(!pCtnStatusHome)
			break;

		pCtnStatusHome->Create(CMultiLanguageManager::GetString(ELanguageParameter_Home), WS_CHILD | WS_VISIBLE | ES_CENTER, rrCaptionStatus[1], this, EDeviceAddEnumeratedControlID_StaticStart + 13);
		pCtnStatusHome->SetFont(pBaseFont);
		m_vctCtrl.push_back(pCtnStatusHome);

		CStatic* pCtnStatusMinus = new CStatic;

		if(!pCtnStatusMinus)
			break;

		pCtnStatusMinus->Create(CMultiLanguageManager::GetString(ELanguageParameter_LimitMinus), WS_CHILD | WS_VISIBLE | ES_CENTER, rrCaptionStatus[2], this, EDeviceAddEnumeratedControlID_StaticStart + 14);
		pCtnStatusMinus->SetFont(pBaseFont);
		m_vctCtrl.push_back(pCtnStatusMinus);

		CStatic* pCtnStatusAlarm = new CStatic;

		if(!pCtnStatusAlarm)
			break;

		pCtnStatusAlarm->Create(CMultiLanguageManager::GetString(ELanguageParameter_Alarm), WS_CHILD | WS_VISIBLE | ES_CENTER, rrCaptionStatus[3], this, EDeviceAddEnumeratedControlID_StaticStart + 15);
		pCtnStatusAlarm->SetFont(pBaseFont);
		m_vctCtrl.push_back(pCtnStatusAlarm);

		CStatic* pCtnStatusInPos = new CStatic;

		if(!pCtnStatusInPos)
			break;

		pCtnStatusInPos->Create(CMultiLanguageManager::GetString(ELanguageParameter_Inposition), WS_CHILD | WS_VISIBLE | ES_CENTER, rrCaptionStatus[4], this, EDeviceAddEnumeratedControlID_StaticStart + 16);
		pCtnStatusInPos->SetFont(pBaseFont);
		m_vctCtrl.push_back(pCtnStatusInPos);

		rrBtnHome.left = rrGrpHome.left + nDialogEdgeMargin;
		rrBtnHome.top = rrGrpControl.bottom + nDialogEdgeMargin + nInfoTextHeight;
		rrBtnHome.right = rrGrpHome.right - nDialogEdgeMargin;
		rrBtnHome.bottom = rrGrpHome.bottom - nDialogEdgeMargin;

		int nHalfServoWidth = ((rrGrpServo.right - nDialogEdgeMargin) - (rrGrpServo.left + nDialogEdgeMargin)) / 2.;

		rrBtnServoOff.left = rrGrpServo.left + nDialogEdgeMargin;
		rrBtnServoOff.top = rrGrpControl.bottom + nDialogEdgeMargin + nInfoTextHeight;
		rrBtnServoOff.right = rrBtnServoOff.left + nHalfServoWidth;
		rrBtnServoOff.bottom = rrGrpServo.bottom - nDialogEdgeMargin;

		rrBtnServoOn.left = rrBtnServoOff.right;
		rrBtnServoOn.top = rrBtnServoOff.top;
		rrBtnServoOn.right = rrBtnServoOn.left + nHalfServoWidth;
		rrBtnServoOn.bottom = rrBtnServoOff.bottom;

		CButton* pBtnHome = new CButton;

		if(!pBtnHome)
			break;

		pBtnHome->Create(CMultiLanguageManager::GetString(ELanguageParameter_Home), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_FLAT, rrBtnHome, this, EDeviceAddControlID_BtnHomming);
		pBtnHome->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnHome);

		CButton* pBtnServoOff = new CButton;

		if(!pBtnServoOff)
			break;

		pBtnServoOff->Create(CMultiLanguageManager::GetString(ELanguageParameter_Off), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_FLAT, rrBtnServoOff, this, EDeviceAddControlID_BtnServoModeOff);
		pBtnServoOff->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnServoOff);

		CButton* pBtnServoOn = new CButton;

		if(!pBtnServoOn)
			break;

		pBtnServoOn->Create(CMultiLanguageManager::GetString(ELanguageParameter_On), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_FLAT, rrBtnServoOn, this, EDeviceAddControlID_BtnServoModeOn);
		pBtnServoOn->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnServoOn);


		//////////////////////////////////////////////////////////////////////////
		// command Group

		rrGrpCmdPos.left = rrGrpControl.left;
		rrGrpCmdPos.top = rrGrpStatus.bottom + nDialogEdgeMargin;
		rrGrpCmdPos.right = rrGrpCmdPos.left + nGroupIntrvalWidth2;
		rrGrpCmdPos.bottom = rrGrpCmdPos.top + (nGroupBoxVerticalMargin + nInfoTextHeight * 2.2);

		rrGrpActPos.left = rrGrpCmdPos.right + nDialogEdgeMargin;
		rrGrpActPos.top = rrGrpCmdPos.top;
		rrGrpActPos.right = rrGrpActPos.left + nGroupIntrvalWidth2;
		rrGrpActPos.bottom = rrGrpCmdPos.bottom;

		rrGrpAlarmReset.left = rrGrpActPos.right + nDialogEdgeMargin;
		rrGrpAlarmReset.top = rrGrpCmdPos.top;
		rrGrpAlarmReset.right = rpRightTopPt.x;
		rrGrpAlarmReset.bottom = rrGrpCmdPos.bottom;

		//////////////////////////////////////////////////////////////////////////
		// button caption

		rrCaptionCmdValue.left = rrGrpCmdPos.left + nDialogEdgeMargin;
		rrCaptionCmdValue.top = rrGrpCmdPos.top + nInfoTextHeight;
		rrCaptionCmdValue.right = rrGrpCmdPos.right - nDialogEdgeMargin;
		rrCaptionCmdValue.bottom = rrCaptionCmdValue.top + nInfoTextHeight;

		rrBtnCmdReset.left = rrCaptionCmdValue.left;
		rrBtnCmdReset.top = rrCaptionCmdValue.bottom;
		rrBtnCmdReset.right = rrCaptionCmdValue.right;
		rrBtnCmdReset.bottom = rrBtnCmdReset.top + nInfoTextHeight;

		rrCaptionActValue.left = rrGrpActPos.left + nDialogEdgeMargin;
		rrCaptionActValue.top = rrGrpActPos.top + nInfoTextHeight;
		rrCaptionActValue.right = rrGrpActPos.right - nDialogEdgeMargin;
		rrCaptionActValue.bottom = rrCaptionActValue.top + nInfoTextHeight;

		rrBtnActReset.left = rrCaptionActValue.left;
		rrBtnActReset.top = rrCaptionActValue.bottom;
		rrBtnActReset.right = rrCaptionActValue.right;
		rrBtnActReset.bottom = rrBtnActReset.top + nInfoTextHeight;

		rrBtnAlarmReset.left = rrGrpAlarmReset.left + nDialogEdgeMargin;
		rrBtnAlarmReset.top = rrCaptionActValue.top;
		rrBtnAlarmReset.right = rrGrpAlarmReset.right - nDialogEdgeMargin;
		rrBtnAlarmReset.bottom = rrBtnActReset.bottom;

		CButton* pBtnGroupCmdPos = new CButton;

		if(!pBtnGroupCmdPos)
			break;

		pBtnGroupCmdPos->Create(CMultiLanguageManager::GetString(ELanguageParameter_CommandPosition), BS_GROUPBOX | WS_CHILD | WS_VISIBLE, rrGrpCmdPos, this, EDeviceAddEnumeratedControlID_StaticStart + 17);
		pBtnGroupCmdPos->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnGroupCmdPos);

		CButton* pBtnGroupActPos = new CButton;

		if(!pBtnGroupActPos)
			break;

		pBtnGroupActPos->Create(CMultiLanguageManager::GetString(ELanguageParameter_ActualPosition), BS_GROUPBOX | WS_CHILD | WS_VISIBLE, rrGrpActPos, this, EDeviceAddEnumeratedControlID_StaticStart + 18);
		pBtnGroupActPos->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnGroupActPos);

		CButton* pBtnGroupAlarm = new CButton;

		if(!pBtnGroupAlarm)
			break;

		pBtnGroupAlarm->Create(CMultiLanguageManager::GetString(ELanguageParameter_Alarm), BS_GROUPBOX | WS_CHILD | WS_VISIBLE, rrGrpAlarmReset, this, EDeviceAddEnumeratedControlID_StaticStart + 19);
		pBtnGroupAlarm->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnGroupAlarm);

		CStatic* pSttCommandPosition = new CStatic;

		if(!pSttCommandPosition)
			break;

		pSttCommandPosition->Create(CMultiLanguageManager::GetString(ELanguageParameter_Value) + _T(" : 0.0000"), WS_CHILD | WS_VISIBLE, rrCaptionCmdValue, this, EDeviceAddControlID_CommandPosition);
		pSttCommandPosition->SetFont(pBaseFont);
		m_vctCtrl.push_back(pSttCommandPosition);

		CButton* pBtnCommandReset = new CButton;

		if(!pBtnCommandReset)
			break;

		pBtnCommandReset->Create(CMultiLanguageManager::GetString(ELanguageParameter_Reset), WS_CHILD | WS_VISIBLE | BS_FLAT | WS_TABSTOP, rrBtnCmdReset, this, EDeviceAddControlID_BtnCommandReset);
		pBtnCommandReset->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnCommandReset);

		CStatic* pSttActualPosition = new CStatic;

		if(!pSttActualPosition)
			break;

		pSttActualPosition->Create(CMultiLanguageManager::GetString(ELanguageParameter_Value) + _T(" : 0.0000"), WS_CHILD | WS_VISIBLE, rrCaptionActValue, this, EDeviceAddControlID_ActualPosition);
		pSttActualPosition->SetFont(pBaseFont);
		m_vctCtrl.push_back(pSttActualPosition);

		CButton* pBtnActualReset = new CButton;

		if(!pBtnActualReset)
			break;

		pBtnActualReset->Create(CMultiLanguageManager::GetString(ELanguageParameter_Reset), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_FLAT, rrBtnActReset, this, EDeviceAddControlID_BtnActualReset);
		pBtnActualReset->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnActualReset);

		CButton* pBtnAlarmReset = new CButton;

		if(!pBtnAlarmReset)
			break;

		pBtnAlarmReset->Create(CMultiLanguageManager::GetString(ELanguageParameter_Reset), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_FLAT, rrBtnAlarmReset, this, EDeviceAddControlID_BtnAlarmReset);
		pBtnAlarmReset->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnAlarmReset);
		
		rrResultGroupBox.right = rpRightTopPt.x;
		rrResultGroupBox.bottom = rrBtnAlarmReset.bottom;
	} 
	while(false);
}

void CDeviceGoogolGTS::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case RAVID_TIMER_DIO:
		{
			if(IsWindowVisible())
			{
				if(m_pDlgExtDioViewer)
				{
					bool arrExtIO[GTS_EXTIO_MODULES] = { 0, };

					memset(arrExtIO, false, sizeof(bool) * GTS_EXTIO_MODULES);

					GetShowExternalIO1(&arrExtIO[0]);
					GetShowExternalIO2(&arrExtIO[1]);
					GetShowExternalIO3(&arrExtIO[2]);
					GetShowExternalIO4(&arrExtIO[3]);
					GetShowExternalIO5(&arrExtIO[4]);
					GetShowExternalIO6(&arrExtIO[5]);
					GetShowExternalIO7(&arrExtIO[6]);
					GetShowExternalIO8(&arrExtIO[7]);
					GetShowExternalIO9(&arrExtIO[8]);
					GetShowExternalIO10(&arrExtIO[9]);
					GetShowExternalIO11(&arrExtIO[10]);
					GetShowExternalIO12(&arrExtIO[11]);
					GetShowExternalIO13(&arrExtIO[12]);
					GetShowExternalIO14(&arrExtIO[13]);
					GetShowExternalIO15(&arrExtIO[14]);
					GetShowExternalIO16(&arrExtIO[15]);

					for(int i = 0; i < GTS_EXTIO_MODULES; ++i)
					{
						if((m_pDlgExtDioViewer + i))
						{
							(m_pDlgExtDioViewer + i)->ShowWindow(arrExtIO[i]);

							if(arrExtIO[i])
							{
								//MakeViewThreadDisplayer(pDisp)
								//{
								(m_pDlgExtDioViewer + i)->OnLedStatus();
							//};

							//AddThreadDisplayer(pDisp);
							}
						}
					}
				}

				int nInputMaxLed = GTS_DIO_IN_PORT;
				int nOutputMaxLed = GTS_DIO_OUT_PORT;

				for(int i = 0; i < nInputMaxLed; ++i)
				{
					if(m_pBUsingInputArray[i])
					{
						CRavidLedCtrl* pLed = (CRavidLedCtrl*)GetDlgItem(EDeviceAddEnumeratedControlID_InputLedStart + i);

						if(pLed)
						{
							pLed->SetActivate(IsInitialized());

							if(ReadGenInputBit(i))
								pLed->On();
							else
								pLed->Off();
						}
					}
				}

				for(int i = 0; i < nOutputMaxLed; ++i)
				{
					if(m_pBUsingOutputArray[i])
					{
						CRavidLedCtrl* pLed = (CRavidLedCtrl*)GetDlgItem(EDeviceAddEnumeratedControlID_OutputLedStart + i);

						if(pLed)
						{
							pLed->SetActivate(IsInitialized());

							if(ReadGenOutBit(i))
								pLed->On();
							else
								pLed->Off();
						}
					}
				}
			}
		}
		break;
	default:
		break;
	}

	CDeviceMotion::OnTimer(nIDEvent);
}


BOOL CDeviceGoogolGTS::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if(pMsg->hwnd == GetDlgItem(EDeviceAddControlID_EditVelocity)->GetSafeHwnd())
	{
		if(pMsg->message == WM_KEYDOWN)
		{
			if(pMsg->wParam == VK_RETURN)
			{
				CComboBox* pCombo = (CComboBox*)GetDlgItem(EDeviceAddControlID_ComboAxisSelect);
				if(!pCombo)
					return TRUE;

				int nSelected = pCombo->GetCurSel();

				CString strData;

				GetDlgItemText(EDeviceAddControlID_EditVelocity, strData);
				m_sMotionInfo[nSelected].dblVel = _ttof(strData);
				strData.Format(_T("%f"), m_sMotionInfo[nSelected].dblVel);
				SetDlgItemText(EDeviceAddControlID_EditVelocity, strData);
				GetDlgItem(EDeviceAddControlID_EditAcceleration)->SetFocus();
			}
		}
	}
	else if(pMsg->hwnd == GetDlgItem(EDeviceAddControlID_EditAcceleration)->GetSafeHwnd())
	{
		if(pMsg->message == WM_KEYDOWN)
		{
			if(pMsg->wParam == VK_RETURN)
			{
				CComboBox* pCombo = (CComboBox*)GetDlgItem(EDeviceAddControlID_ComboAxisSelect);
				if(!pCombo)
					return TRUE;

				int nSelected = pCombo->GetCurSel();

				CString strData;

				GetDlgItemText(EDeviceAddControlID_EditAcceleration, strData);
				m_sMotionInfo[nSelected].dblAcl = _ttof(strData);
				strData.Format(_T("%f"), m_sMotionInfo[nSelected].dblAcl);
				SetDlgItemText(EDeviceAddControlID_EditAcceleration, strData);
				GetDlgItem(EDeviceAddControlID_EditDistance)->SetFocus();
			}
		}
	}
	else if(pMsg->hwnd == GetDlgItem(EDeviceAddControlID_EditDistance)->GetSafeHwnd())
	{
		if(pMsg->message == WM_KEYDOWN)
		{
			if(pMsg->wParam == VK_RETURN)
			{
				CComboBox* pCombo = (CComboBox*)GetDlgItem(EDeviceAddControlID_ComboAxisSelect);
				if(!pCombo)
					return TRUE;

				int nSelected = pCombo->GetCurSel();

				CString strData;

				GetDlgItemText(EDeviceAddControlID_EditDistance, strData);
				m_sMotionInfo[nSelected].dblDis = _ttof(strData);
				strData.Format(_T("%f"), m_sMotionInfo[nSelected].dblDis);
				SetDlgItemText(EDeviceAddControlID_EditDistance, strData);
				GetDlgItem(EDeviceAddControlID_EditVelocity)->SetFocus();
			}
		}
	}
	else if(pMsg->hwnd == GetDlgItem(EDeviceAddControlID_ComboAxisSelect)->GetSafeHwnd())
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(EDeviceAddControlID_ComboAxisSelect);
		if(!pCombo)
			return TRUE;

		int nSelected = pCombo->GetCurSel();

		CString strData, strPreData;

		strData.Format(_T("%f"), m_sMotionInfo[nSelected].dblVel);

		GetDlgItemText(EDeviceAddControlID_EditVelocity, strPreData);

		if(strData.CompareNoCase(strPreData))
			SetDlgItemText(EDeviceAddControlID_EditVelocity, strData);

		strData.Format(_T("%f"), m_sMotionInfo[nSelected].dblAcl);

		GetDlgItemText(EDeviceAddControlID_EditAcceleration, strPreData);

		if(strData.CompareNoCase(strPreData))
			SetDlgItemText(EDeviceAddControlID_EditAcceleration, strData);

		strData.Format(_T("%f"), m_sMotionInfo[nSelected].dblDis);

		GetDlgItemText(EDeviceAddControlID_EditDistance, strPreData);

		if(strData.CompareNoCase(strPreData))
			SetDlgItemText(EDeviceAddControlID_EditDistance, strData);

		if(pMsg->message == WM_KEYDOWN)
		{
			if(pMsg->wParam == VK_RETURN)
				GetDlgItem(EDeviceAddControlID_EditVelocity)->SetFocus();
		}
	}

	if(pMsg->message == WM_COMMAND)
	{
		if(pMsg->wParam == WM_DESTROY)
		{
			SetShowExternalIO1(false);
			SetShowExternalIO2(false);
			SetShowExternalIO3(false);
			SetShowExternalIO4(false);
			SetShowExternalIO5(false);
			SetShowExternalIO6(false);
			SetShowExternalIO7(false);
			SetShowExternalIO8(false);
			SetShowExternalIO9(false);
			SetShowExternalIO10(false);
			SetShowExternalIO11(false);
			SetShowExternalIO12(false);
			SetShowExternalIO13(false);
			SetShowExternalIO14(false);
			SetShowExternalIO15(false);
			SetShowExternalIO16(false);

			if(m_pDlgExtDioViewer)
			{
				for(int i = 0; i < GTS_EXTIO_MODULES; ++i)
					(m_pDlgExtDioViewer + i)->OnCancel();
			}
		}
	}
	
	return CDeviceMotion::PreTranslateMessage(pMsg);
}

BOOL CDeviceGoogolGTS::OnInitDialog()
{
	CDeviceMotion::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	SetTimer(RAVID_TIMER_DIO, 10, nullptr);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDeviceGoogolGTS::OnDestroy()
{
	CDeviceMotion::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	KillTimer(RAVID_TIMER_DIO);
}

void CDeviceGoogolGTS::OnCancel()
{
	SetShowExternalIO1(false);
	SetShowExternalIO2(false);
	SetShowExternalIO3(false);
	SetShowExternalIO4(false);
	SetShowExternalIO5(false);
	SetShowExternalIO6(false);
	SetShowExternalIO7(false);
	SetShowExternalIO8(false);
	SetShowExternalIO9(false);
	SetShowExternalIO10(false);
	SetShowExternalIO11(false);
	SetShowExternalIO12(false);
	SetShowExternalIO13(false);
	SetShowExternalIO14(false);
	SetShowExternalIO15(false);
	SetShowExternalIO16(false);

	for(int i = 0; i < GTS_EXTIO_MODULES; ++i)
		(m_pDlgExtDioViewer + i)->OnCancel();

	CDeviceMotion::OnCancel();
}

void CDeviceGoogolGTS::OnBnClickedOutLed(UINT nID)
{
	do
	{
		int nNumber = nID - EDeviceAddEnumeratedControlID_OutputLedStart;

		if((CAuthorityManager::GetDeviceControlPermission() <= CAuthorityManager::GetCurrentUserType()) && !CSequenceManager::IsRunning())
		{
 			bool bBit = ReadGenOutBit(nNumber);
 
 			WriteGenOutBit(nNumber, !bBit);
		}
	}
	while(false);
}

void CDeviceGoogolGTS::OnBnClickedExtOutLed(UINT nID)
{
	do
	{
		int nNumber = nID - EDeviceAddEnumeratedControlID_ExtOutputLedStart;

		if((CAuthorityManager::GetDeviceControlPermission() <= CAuthorityManager::GetCurrentUserType()) && !CSequenceManager::IsRunning())
		{
			bool bBit = ReadGenOutBit(nNumber);

			WriteGenOutBit(nNumber, !bBit);
		}
	}
	while(false);
}

short CDeviceGoogolGTS::__gts_GetVersion(char **pVersion)
{
	short shReturn = SHORT_MAX;

	do 
	{
		if(!(*pVersion))
			break;

		shReturn = GT_GetVersion(_ttoi(GetDeviceID()), pVersion);
	} 
	while(false);

	return shReturn;
}

short CDeviceGoogolGTS::__gts_AlarmOff(short axis)
{
	return GT_AlarmOff(_ttoi(GetDeviceID()), axis);
}

short CDeviceGoogolGTS::__gts_AlarmOn(short axis)
{
	return GT_AlarmOn(_ttoi(GetDeviceID()), axis);
}

short CDeviceGoogolGTS::__gts_LmtsOn(short axis, short limitType)
{
	return GT_LmtsOn(_ttoi(GetDeviceID()), axis, limitType);
}

short CDeviceGoogolGTS::__gts_LmtsOff(short axis, short limitType)
{
	return GT_LmtsOff(_ttoi(GetDeviceID()), axis, limitType);
}
	  
short CDeviceGoogolGTS::__gts_LmtSns(unsigned short sense)
{
	return GT_LmtSns(_ttoi(GetDeviceID()), sense);
}

short CDeviceGoogolGTS::__gts_GetSts(short axis, long *pSts, short count, unsigned long *pClock)
{
	short shReturn = SHORT_MAX;

	do 
	{
		if(!pSts)
			break;

		shReturn = GT_GetSts(_ttoi(GetDeviceID()), axis, pSts, count, pClock);
	} 
	while(false);

	return shReturn;
}

short CDeviceGoogolGTS::__gts_ClrSts(short axis, short count)
{
	return GT_ClrSts(_ttoi(GetDeviceID()), axis, count);
}

short CDeviceGoogolGTS::__gts_ZeroPos(short axis, short count)
{
	return GT_ZeroPos(_ttoi(GetDeviceID()), axis, count);
}

	  
short CDeviceGoogolGTS::__gts_SetSoftLimit(short axis, long positive, long negative)
{
	return GT_SetSoftLimit(_ttoi(GetDeviceID()), axis, positive, negative);
}

short CDeviceGoogolGTS::__gts_GetSoftLimit(short axis, long *pPositive, long *pNegative)
{
	short shReturn = SHORT_MAX;

	do 
	{
		if(!pPositive || !pNegative)
			break;

		shReturn = GT_GetSoftLimit(_ttoi(GetDeviceID()), axis, pPositive, pNegative);
	} 
	while(false);

	return shReturn;
}

	  
short CDeviceGoogolGTS::__gts_PrfTrap(short profile)
{
	return GT_PrfTrap(_ttoi(GetDeviceID()), profile);
}

short CDeviceGoogolGTS::__gts_SetTrapPrm(short profile, __gts_STTrapPrm *pPrm)
{
	short shReturn = SHORT_MAX;

	do 
	{
		if(!pPrm)
			break;

		TTrapPrm sData;

		sData.acc = pPrm->acc;
		sData.dec = pPrm->dec;
		sData.velStart = pPrm->velStart;
		sData.smoothTime = pPrm->smoothTime;

		shReturn = GT_SetTrapPrm(_ttoi(GetDeviceID()), profile, &sData);
	} 
	while(false);

	return shReturn;
}

short CDeviceGoogolGTS::__gts_GetTrapPrm(short profile, __gts_STTrapPrm *pPrm)
{
	short shReturn = SHORT_MAX;

	do
	{
		if(!pPrm)
			break;

		TTrapPrm sData;

		shReturn = GT_GetTrapPrm(_ttoi(GetDeviceID()), profile, &sData);

		if(!shReturn)
		{
			pPrm->acc = sData.acc;
			pPrm->dec = sData.dec;
			pPrm->velStart = sData.velStart;
			pPrm->smoothTime = sData.smoothTime;
		}
	}
	while(false);

	return shReturn;
}

short CDeviceGoogolGTS::__gts_Update(long mask)
{
	return GT_Update(_ttoi(GetDeviceID()), mask);
}

short CDeviceGoogolGTS::__gts_SetPos(short profile, long pos)
{
	return GT_SetPos(_ttoi(GetDeviceID()), profile, pos);
}

short CDeviceGoogolGTS::__gts_GetPos(short profile, long *pPos)
{
	short shReturn = SHORT_MAX;

	do 
	{
		if(!pPos)
			break;

		shReturn = GT_GetPos(_ttoi(GetDeviceID()), profile, pPos);
	} 
	while(false);

	return shReturn;
}

short CDeviceGoogolGTS::__gts_SetVel(short profile, double vel)
{
	return GT_SetVel(_ttoi(GetDeviceID()), profile, vel);
}

short CDeviceGoogolGTS::__gts_GetVel(short profile, double *pVel)
{
	short shReturn = SHORT_MAX;

	do
	{
		if(!pVel)
			break;

		shReturn = GT_GetVel(_ttoi(GetDeviceID()), profile, pVel);
	}
	while(false);

	return shReturn;
}

short CDeviceGoogolGTS::__gts_SetCrdPrm(short crd, __gts_STCrdPrm *pCrdPrm)
{
	short shReturn = SHORT_MAX;

	do 
	{
		if(!pCrdPrm)
			break;

		TCrdPrm sData;
		sData.dimension = pCrdPrm->dimension;
		memcpy(sData.profile, pCrdPrm->profile, sizeof(short) * 8);
		sData.synVelMax = pCrdPrm->synVelMax;
		sData.synAccMax = pCrdPrm->synAccMax;
		sData.evenTime = pCrdPrm->evenTime;
		sData.setOriginFlag = pCrdPrm->setOriginFlag;
		memcpy(sData.originPos, pCrdPrm->originPos, sizeof(short) * 8);

		shReturn = GT_SetCrdPrm(_ttoi(GetDeviceID()), crd, &sData);
	} 
	while(false);
	
	return shReturn;
}

short CDeviceGoogolGTS::__gts_GetCrdPrm(short crd, __gts_STCrdPrm *pCrdPrm)
{
	short shReturn = SHORT_MAX;

	do
	{
		if(!pCrdPrm)
			break;

		TCrdPrm sData;
		sData.dimension = pCrdPrm->dimension;
		memcpy(sData.profile, pCrdPrm->profile, sizeof(short) * 8);
		sData.synVelMax = pCrdPrm->synVelMax;
		sData.synAccMax = pCrdPrm->synAccMax;
		sData.evenTime = pCrdPrm->evenTime;
		sData.setOriginFlag = pCrdPrm->setOriginFlag;
		memcpy(sData.originPos, pCrdPrm->originPos, sizeof(short) * 8);
		
		shReturn = GT_GetCrdPrm(_ttoi(GetDeviceID()), crd, &sData);

		if(!shReturn)
		{
			pCrdPrm->dimension = sData.dimension;
			memcpy(pCrdPrm->profile, sData.profile, sizeof(short) * 8);
			pCrdPrm->synVelMax = sData.synVelMax;
			pCrdPrm->synAccMax = sData.synAccMax;
			pCrdPrm->evenTime = sData.evenTime;
			pCrdPrm->setOriginFlag = sData.setOriginFlag;
			memcpy(pCrdPrm->originPos, sData.originPos, sizeof(short) * 8);
		}
	}
	while(false);

	return shReturn;
}

short CDeviceGoogolGTS::__gts_CrdSpace(short crd, long *pSpace, short fifo)
{
	short shReturn = SHORT_MAX;

	do 
	{
		if(!pSpace)
			break;

		shReturn = GT_CrdSpace(_ttoi(GetDeviceID()), crd, pSpace, fifo);
	} 
	while(false);

	return shReturn;
}

short CDeviceGoogolGTS::__gts_CrdData(short crd, __gts_STCrdData *pCrdData, short fifo)
{
	short shReturn = SHORT_MAX;

	do
	{
		if(!pCrdData)
			break;

		TCrdData sData;
		sData.motionType = pCrdData->motionType;
		sData.circlePlat = pCrdData->circlePlat;
		memcpy(sData.pos, pCrdData->pos, sizeof(long) * INTERPOLATION_AXIS_MAX);
		sData.radius = pCrdData->radius;
		sData.circleDir = pCrdData->circleDir;
		memcpy(sData.center, pCrdData->center, sizeof(double) * 3);
		sData.vel = pCrdData->vel;
		sData.acc = pCrdData->acc;
		sData.velEndZero = pCrdData->velEndZero;

		sData.operation.flag = pCrdData->operation.flag;
		sData.operation.delay = pCrdData->operation.delay;
		sData.operation.doType = pCrdData->operation.doType;
		sData.operation.doMask = pCrdData->operation.doMask;
		sData.operation.doValue = pCrdData->operation.doValue;
		memcpy(sData.operation.dataExt, pCrdData->operation.dataExt, sizeof(unsigned short) * CRD_OPERATION_DATA_EXT_MAX);

		memcpy(sData.cos, pCrdData->cos, sizeof(double) * INTERPOLATION_AXIS_MAX);
		sData.velEnd = pCrdData->velEnd;
		sData.velEndAdjust = pCrdData->velEndAdjust;
		sData.r = pCrdData->r;

		shReturn = GT_CrdData(_ttoi(GetDeviceID()), crd, &sData, fifo);
	}
	while(false);

	return shReturn;
}

short CDeviceGoogolGTS::__gts_CrdDataCircle(short crd, __gts_STCrdData *pCrdData, short fifo)
{
	short shReturn = SHORT_MAX;

	do
	{
		if(!pCrdData)
			break;

		TCrdData sData;
		sData.motionType = pCrdData->motionType;
		sData.circlePlat = pCrdData->circlePlat;
		memcpy(sData.pos, pCrdData->pos, sizeof(long) * INTERPOLATION_AXIS_MAX);
		sData.radius = pCrdData->radius;
		sData.circleDir = pCrdData->circleDir;
		memcpy(sData.center, pCrdData->center, sizeof(double) * 3);
		sData.vel = pCrdData->vel;
		sData.acc = pCrdData->acc;
		sData.velEndZero = pCrdData->velEndZero;

		sData.operation.flag = pCrdData->operation.flag;
		sData.operation.delay = pCrdData->operation.delay;
		sData.operation.doType = pCrdData->operation.doType;
		sData.operation.doMask = pCrdData->operation.doMask;
		sData.operation.doValue = pCrdData->operation.doValue;
		memcpy(sData.operation.dataExt, pCrdData->operation.dataExt, sizeof(unsigned short) * CRD_OPERATION_DATA_EXT_MAX);

		memcpy(sData.cos, pCrdData->cos, sizeof(double) * INTERPOLATION_AXIS_MAX);
		sData.velEnd = pCrdData->velEnd;
		sData.velEndAdjust = pCrdData->velEndAdjust;
		sData.r = pCrdData->r;


		shReturn = GT_CrdDataCircle(_ttoi(GetDeviceID()), crd, &sData, fifo);
	}
	while(false);

	return shReturn;
}

short CDeviceGoogolGTS::__gts_LnXY(short crd, long x, long y, double synVel, double synAcc, double velEnd, short fifo)
{
	return GT_LnXY(_ttoi(GetDeviceID()), crd, x, y, synVel, synAcc, velEnd, fifo);;
}

short CDeviceGoogolGTS::__gts_LnXYZ(short crd, long x, long y, long z, double synVel, double synAcc, double velEnd, short fifo)
{
	return GT_LnXYZ(_ttoi(GetDeviceID()), crd, x, y, z, synVel, synAcc, velEnd, fifo);
}

short CDeviceGoogolGTS::__gts_LnXYZA(short crd, long x, long y, long z, long a, double synVel, double synAcc, double velEnd, short fifo)
{
	return GT_LnXYZA(_ttoi(GetDeviceID()), crd, x, y, z, a, synVel, synAcc, velEnd, fifo);
}

short CDeviceGoogolGTS::__gts_LnXYG0(short crd, long x, long y, double synVel, double synAcc, short fifo)
{
	return GT_LnXYG0(_ttoi(GetDeviceID()), crd, x, y, synVel, synAcc, fifo);
}

short CDeviceGoogolGTS::__gts_LnXYZG0(short crd, long x, long y, long z, double synVel, double synAcc, short fifo)
{
	return GT_LnXYZG0(_ttoi(GetDeviceID()), crd, x, y, z, synVel, synAcc, fifo);
}

short CDeviceGoogolGTS::__gts_LnXYZAG0(short crd, long x, long y, long z, long a, double synVel, double synAcc, short fifo)
{
	return GT_LnXYZAG0(_ttoi(GetDeviceID()), crd, x, y, z, a, synVel, synAcc, fifo);
}

short CDeviceGoogolGTS::__gts_ArcXYR(short crd, long x, long y, double radius, short circleDir, double synVel, double synAcc, double velEnd, short fifo)
{
	return GT_ArcXYR(_ttoi(GetDeviceID()), crd, x, y, radius, circleDir, synVel, synAcc, velEnd, fifo);
}

short CDeviceGoogolGTS::__gts_ArcXYC(short crd, long x, long y, double xCenter, double yCenter, short circleDir, double synVel, double synAcc, double velEnd, short fifo)
{
	return GT_ArcXYC(_ttoi(GetDeviceID()), crd, x, y, xCenter, yCenter, circleDir, synVel, synAcc, velEnd, fifo);
}

short CDeviceGoogolGTS::__gts_ArcYZR(short crd, long y, long z, double radius, short circleDir, double synVel, double synAcc, double velEnd, short fifo)
{
	return GT_ArcYZR(_ttoi(GetDeviceID()), crd, y, z, radius, circleDir, synVel, synAcc, velEnd, fifo);
}

short CDeviceGoogolGTS::__gts_ArcYZC(short crd, long y, long z, double yCenter, double zCenter, short circleDir, double synVel, double synAcc, double velEnd, short fifo)
{
	return GT_ArcYZC(_ttoi(GetDeviceID()), crd, y, z, yCenter, zCenter, circleDir, synVel, synAcc, velEnd, fifo);
}

short CDeviceGoogolGTS::__gts_ArcZXR(short crd, long z, long x, double radius, short circleDir, double synVel, double synAcc, double velEnd, short fifo)
{
	return GT_ArcZXR(_ttoi(GetDeviceID()), crd, z, x, radius, circleDir, synVel, synAcc, velEnd, fifo);
}

short CDeviceGoogolGTS::__gts_ArcZXC(short crd, long z, long x, double zCenter, double xCenter, short circleDir, double synVel, double synAcc, double velEnd, short fifo)
{
	return GT_ArcZXC(_ttoi(GetDeviceID()), crd, z, x, zCenter, xCenter, circleDir, synVel, synAcc, velEnd, fifo);
}

short CDeviceGoogolGTS::__gts_ArcXYZ(short crd, long x, long y, long z, double interX, double interY, double interZ, double synVel, double synAcc, double velEnd, short fifo)
{
	return GT_ArcXYZ(_ttoi(GetDeviceID()), crd, x, y, z, interX, interY, interZ, synVel, synAcc, velEnd, fifo);
}
short CDeviceGoogolGTS::__gts_LnXYOverride2(short crd, long x, long y, double synVel, double synAcc, double velEnd, short fifo)
{
	return GT_LnXYOverride2(_ttoi(GetDeviceID()), crd, x, y, synVel, synAcc, velEnd, fifo);
}

short CDeviceGoogolGTS::__gts_LnXYZOverride2(short crd, long x, long y, long z, double synVel, double synAcc, double velEnd, short fifo)
{
	return GT_LnXYZOverride2(_ttoi(GetDeviceID()), crd, x, y, z, synVel, synAcc, velEnd, fifo);
}

short CDeviceGoogolGTS::__gts_LnXYZAOverride2(short crd, long x, long y, long z, long a, double synVel, double synAcc, double velEnd, short fifo)
{
	return GT_LnXYZAOverride2(_ttoi(GetDeviceID()), crd, x, y, z, a, synVel, synAcc, velEnd, fifo);
}

short CDeviceGoogolGTS::__gts_LnXYG0Override2(short crd, long x, long y, double synVel, double synAcc, short fifo)
{
	return GT_LnXYG0Override2(_ttoi(GetDeviceID()), crd, x, y, synVel, synAcc, fifo);
}

short CDeviceGoogolGTS::__gts_LnXYZG0Override2(short crd, long x, long y, long z, double synVel, double synAcc, short fifo)
{
	return GT_LnXYZG0Override2(_ttoi(GetDeviceID()), crd, x, y, z, synVel, synAcc, fifo);
}

short CDeviceGoogolGTS::__gts_LnXYZAG0Override2(short crd, long x, long y, long z, long a, double synVel, double synAcc, short fifo)
{
	return GT_LnXYZAG0Override2(_ttoi(GetDeviceID()), crd, x, y, z, a, synVel, synAcc, fifo);
}

short CDeviceGoogolGTS::__gts_ArcXYROverride2(short crd, long x, long y, double radius, short circleDir, double synVel, double synAcc, double velEnd, short fifo)
{
	return GT_ArcXYROverride2(_ttoi(GetDeviceID()), crd, x, y, radius, circleDir, synVel, synAcc, velEnd, fifo);
}

short CDeviceGoogolGTS::__gts_ArcXYCOverride2(short crd, long x, long y, double xCenter, double yCenter, short circleDir, double synVel, double synAcc, double velEnd, short fifo)
{
	return GT_ArcXYCOverride2(_ttoi(GetDeviceID()), crd, x, y, xCenter, yCenter, circleDir, synVel, synAcc, velEnd, fifo);
}

short CDeviceGoogolGTS::__gts_ArcYZROverride2(short crd, long y, long z, double radius, short circleDir, double synVel, double synAcc, double velEnd, short fifo)
{
	return GT_ArcYZROverride2(_ttoi(GetDeviceID()), crd, y, z, radius, circleDir, synVel, synAcc, velEnd, fifo);
}

short CDeviceGoogolGTS::__gts_ArcYZCOverride2(short crd, long y, long z, double yCenter, double zCenter, short circleDir, double synVel, double synAcc, double velEnd, short fifo)
{
	return GT_ArcYZCOverride2(_ttoi(GetDeviceID()), crd, y, z, yCenter, zCenter, circleDir, synVel, synAcc, velEnd, fifo);
}

short CDeviceGoogolGTS::__gts_ArcZXROverride2(short crd, long z, long x, double radius, short circleDir, double synVel, double synAcc, double velEnd, short fifo)
{
	return GT_ArcZXROverride2(_ttoi(GetDeviceID()), crd, z, x, radius, circleDir, synVel, synAcc, velEnd, fifo);
}

short CDeviceGoogolGTS::__gts_ArcZXCOverride2(short crd, long z, long x, double zCenter, double xCenter, short circleDir, double synVel, double synAcc, double velEnd, short fifo)
{
	return GT_ArcZXCOverride2(_ttoi(GetDeviceID()), crd, z, x, zCenter, xCenter, circleDir, synVel, synAcc, velEnd, fifo);
}

short CDeviceGoogolGTS::__gts_HelixXYRZ(short crd, long x, long y, long z, double radius, short circleDir, double synVel, double synAcc, double velEnd, short fifo)
{
	return GT_HelixXYRZ(_ttoi(GetDeviceID()), crd, x, y, z, radius, circleDir, synVel, synAcc, velEnd, fifo);
}
short CDeviceGoogolGTS::__gts_HelixXYCZ(short crd, long x, long y, long z, double xCenter, double yCenter, short circleDir, double synVel, double synAcc, double velEnd, short fifo)
{
	return GT_HelixXYCZ(_ttoi(GetDeviceID()), crd, x, y, z, xCenter, yCenter, circleDir, synVel, synAcc, velEnd, fifo);
}

short CDeviceGoogolGTS::__gts_HelixYZRX(short crd, long x, long y, long z, double radius, short circleDir, double synVel, double synAcc, double velEnd, short fifo)
{
	return GT_HelixYZRX(_ttoi(GetDeviceID()), crd, x, y, z, radius, circleDir, synVel, synAcc, velEnd, fifo);
}

short CDeviceGoogolGTS::__gts_HelixYZCX(short crd, long x, long y, long z, double yCenter, double zCenter, short circleDir, double synVel, double synAcc, double velEnd, short fifo)
{
	return GT_HelixYZCX(_ttoi(GetDeviceID()), crd, x, y, z, yCenter, zCenter, circleDir, synVel, synAcc, velEnd, fifo);
}

short CDeviceGoogolGTS::__gts_HelixZXRY(short crd, long x, long y, long z, double radius, short circleDir, double synVel, double synAcc, double velEnd, short fifo)
{
	return GT_HelixZXRY(_ttoi(GetDeviceID()), crd, x, y, z, radius, circleDir, synVel, synAcc, velEnd, fifo);
}

short CDeviceGoogolGTS::__gts_HelixZXCY(short crd, long x, long y, long z, double zCenter, double xCenter, short circleDir, double synVel, double synAcc, double velEnd, short fifo)
{
	return GT_HelixZXCY(_ttoi(GetDeviceID()), crd, x, y, z, zCenter, xCenter, circleDir, synVel, synAcc, velEnd, fifo);
}

short CDeviceGoogolGTS::__gts_HelixXYRZOverride2(short crd, long x, long y, long z, double radius, short circleDir, double synVel, double synAcc, double velEnd, short fifo)
{
	return GT_HelixXYRZOverride2(_ttoi(GetDeviceID()), crd, x, y, z, radius, circleDir, synVel, synAcc, velEnd, fifo);
}

short CDeviceGoogolGTS::__gts_HelixXYCZOverride2(short crd, long x, long y, long z, double xCenter, double yCenter, short circleDir, double synVel, double synAcc, double velEnd, short fifo)
{
	return GT_HelixXYCZOverride2(_ttoi(GetDeviceID()), crd, x, y, z, xCenter, yCenter, circleDir, synVel, synAcc, velEnd, fifo);
}

short CDeviceGoogolGTS::__gts_HelixYZRXOverride2(short crd, long x, long y, long z, double radius, short circleDir, double synVel, double synAcc, double velEnd, short fifo)
{
	return GT_HelixYZRXOverride2(_ttoi(GetDeviceID()), crd, x, y, z, radius, circleDir, synVel, synAcc, velEnd, fifo);
}

short CDeviceGoogolGTS::__gts_HelixYZCXOverride2(short crd, long x, long y, long z, double yCenter, double zCenter, short circleDir, double synVel, double synAcc, double velEnd, short fifo)
{
	return GT_HelixYZCXOverride2(_ttoi(GetDeviceID()), crd, x, y, z, yCenter, zCenter, circleDir, synVel, synAcc, velEnd, fifo);
}

short CDeviceGoogolGTS::__gts_HelixZXRYOverride2(short crd, long x, long y, long z, double radius, short circleDir, double synVel, double synAcc, double velEnd, short fifo)
{
	return GT_HelixZXRYOverride2(_ttoi(GetDeviceID()), crd, x, y, z, radius, circleDir, synVel, synAcc, velEnd, fifo);
}

short CDeviceGoogolGTS::__gts_HelixZXCYOverride2(short crd, long x, long y, long z, double zCenter, double xCenter, short circleDir, double synVel, double synAcc, double velEnd, short fifo)
{
	return GT_HelixZXCYOverride2(_ttoi(GetDeviceID()), crd, x, y, z, zCenter, xCenter, circleDir, synVel, synAcc, velEnd, fifo);
}

short CDeviceGoogolGTS::__gts_LnXYWN(short crd, long x, long y, double synVel, double synAcc, double velEnd, long segNum, short fifo)
{
	return GT_LnXYWN(_ttoi(GetDeviceID()), crd, x, y, synVel, synAcc, velEnd, segNum, fifo);
}

short CDeviceGoogolGTS::__gts_LnXYZWN(short crd, long x, long y, long z, double synVel, double synAcc, double velEnd, long segNum, short fifo)
{
	return GT_LnXYZWN(_ttoi(GetDeviceID()), crd, x, y, z, synVel, synAcc, velEnd, segNum, fifo);
}

short CDeviceGoogolGTS::__gts_LnXYZAWN(short crd, long x, long y, long z, long a, double synVel, double synAcc, double velEnd, long segNum, short fifo)
{
	return GT_LnXYZAWN(_ttoi(GetDeviceID()), crd, x, y, z, a, synVel, synAcc, velEnd, segNum, fifo);
}

short CDeviceGoogolGTS::__gts_LnXYG0WN(short crd, long x, long y, double synVel, double synAcc, long segNum, short fifo)
{
	return GT_LnXYG0WN(_ttoi(GetDeviceID()), crd, x, y, synVel, synAcc, segNum, fifo);
}

short CDeviceGoogolGTS::__gts_LnXYZG0WN(short crd, long x, long y, long z, double synVel, double synAcc, long segNum, short fifo)
{
	return GT_LnXYZG0WN(_ttoi(GetDeviceID()), crd, x, y, z, synVel, synAcc, segNum, fifo);
}

short CDeviceGoogolGTS::__gts_LnXYZAG0WN(short crd, long x, long y, long z, long a, double synVel, double synAcc, long segNum, short fifo)
{
	return GT_LnXYZAG0WN(_ttoi(GetDeviceID()), crd, x, y, z, a, synVel, synAcc, segNum, fifo);
}

short CDeviceGoogolGTS::__gts_ArcXYRWN(short crd, long x, long y, double radius, short circleDir, double synVel, double synAcc, double velEnd, long segNum, short fifo)
{
	return GT_ArcXYRWN(_ttoi(GetDeviceID()), crd, x, y, radius, circleDir, synVel, synAcc, velEnd, segNum, fifo);
}

short CDeviceGoogolGTS::__gts_ArcXYCWN(short crd, long x, long y, double xCenter, double yCenter, short circleDir, double synVel, double synAcc, double velEnd, long segNum, short fifo)
{
	return GT_ArcXYCWN(_ttoi(GetDeviceID()), crd, x, y, xCenter, yCenter, circleDir, synVel, synAcc, velEnd, segNum, fifo);
}

short CDeviceGoogolGTS::__gts_ArcYZRWN(short crd, long y, long z, double radius, short circleDir, double synVel, double synAcc, double velEnd, long segNum, short fifo)
{
	return GT_ArcYZRWN(_ttoi(GetDeviceID()), crd, y, z, radius, circleDir, synVel, synAcc, velEnd, segNum, fifo);
}

short CDeviceGoogolGTS::__gts_ArcYZCWN(short crd, long y, long z, double yCenter, double zCenter, short circleDir, double synVel, double synAcc, double velEnd, long segNum, short fifo)
{
	return GT_ArcYZCWN(_ttoi(GetDeviceID()), crd, y, z, yCenter, zCenter, circleDir, synVel, synAcc, velEnd, segNum, fifo);
}

short CDeviceGoogolGTS::__gts_ArcZXRWN(short crd, long z, long x, double radius, short circleDir, double synVel, double synAcc, double velEnd, long segNum, short fifo)
{
	return GT_ArcZXRWN(_ttoi(GetDeviceID()), crd, z, x, radius, circleDir, synVel, synAcc, velEnd, segNum, fifo);
}

short CDeviceGoogolGTS::__gts_ArcZXCWN(short crd, long z, long x, double zCenter, double xCenter, short circleDir, double synVel, double synAcc, double velEnd, long segNum, short fifo)
{
	return GT_ArcZXCWN(_ttoi(GetDeviceID()), crd, z, x, zCenter, xCenter, circleDir, synVel, synAcc, velEnd, segNum, fifo);
}

short CDeviceGoogolGTS::__gts_ArcXYZWN(short crd, long x, long y, long z, double interX, double interY, double interZ, double synVel, double synAcc, double velEnd, long segNum, short fifo)
{
	return GT_ArcXYZWN(_ttoi(GetDeviceID()), crd, x, y, z, interX, interY, interZ, synVel, synAcc, velEnd, segNum, fifo);
}

short CDeviceGoogolGTS::__gts_LnXYOverride2WN(short crd, long x, long y, double synVel, double synAcc, double velEnd, long segNum, short fifo)
{
	return GT_LnXYOverride2WN(_ttoi(GetDeviceID()), crd, x, y, synVel, synAcc, velEnd, segNum, fifo);
}

short CDeviceGoogolGTS::__gts_LnXYZOverride2WN(short crd, long x, long y, long z, double synVel, double synAcc, double velEnd, long segNum, short fifo)
{
	return GT_LnXYZOverride2WN(_ttoi(GetDeviceID()), crd, x, y, z, synVel, synAcc, velEnd, segNum, fifo);
}

short CDeviceGoogolGTS::__gts_LnXYZAOverride2WN(short crd, long x, long y, long z, long a, double synVel, double synAcc, double velEnd, long segNum, short fifo)
{
	return GT_LnXYZAOverride2WN(_ttoi(GetDeviceID()), crd, x, y, z, a, synVel, synAcc, velEnd, segNum, fifo);
}

short CDeviceGoogolGTS::__gts_LnXYG0Override2WN(short crd, long x, long y, double synVel, double synAcc, long segNum, short fifo)
{
	return GT_LnXYG0Override2WN(_ttoi(GetDeviceID()), crd, x, y, synVel, synAcc, segNum, fifo);
}

short CDeviceGoogolGTS::__gts_LnXYZG0Override2WN(short crd, long x, long y, long z, double synVel, double synAcc, long segNum, short fifo)
{
	return GT_LnXYZG0Override2WN(_ttoi(GetDeviceID()), crd, x, y, z, synVel, synAcc, segNum, fifo);
}

short CDeviceGoogolGTS::__gts_LnXYZAG0Override2WN(short crd, long x, long y, long z, long a, double synVel, double synAcc, long segNum, short fifo)
{
	return GT_LnXYZAG0Override2WN(_ttoi(GetDeviceID()), crd, x, y, z, a, synVel, synAcc, segNum, fifo);
}

short CDeviceGoogolGTS::__gts_ArcXYROverride2WN(short crd, long x, long y, double radius, short circleDir, double synVel, double synAcc, double velEnd, long segNum, short fifo)
{
	return GT_ArcXYROverride2WN(_ttoi(GetDeviceID()), crd, x, y, radius, circleDir, synVel, synAcc, velEnd, segNum, fifo);
}

short CDeviceGoogolGTS::__gts_ArcXYCOverride2WN(short crd, long x, long y, double xCenter, double yCenter, short circleDir, double synVel, double synAcc, double velEnd, long segNum, short fifo)
{
	return GT_ArcXYCOverride2WN(_ttoi(GetDeviceID()), crd, x, y, xCenter, yCenter, circleDir, synVel, synAcc, velEnd, segNum, fifo);
}

short CDeviceGoogolGTS::__gts_ArcYZROverride2WN(short crd, long y, long z, double radius, short circleDir, double synVel, double synAcc, double velEnd, long segNum, short fifo)
{
	return GT_ArcYZROverride2WN(_ttoi(GetDeviceID()), crd, y, z, radius, circleDir, synVel, synAcc, velEnd, segNum, fifo);
}

short CDeviceGoogolGTS::__gts_ArcYZCOverride2WN(short crd, long y, long z, double yCenter, double zCenter, short circleDir, double synVel, double synAcc, double velEnd, long segNum, short fifo)
{
	return GT_ArcYZCOverride2WN(_ttoi(GetDeviceID()), crd, y, z, yCenter, zCenter, circleDir, synVel, synAcc, velEnd, segNum, fifo);
}

short CDeviceGoogolGTS::__gts_ArcZXROverride2WN(short crd, long z, long x, double radius, short circleDir, double synVel, double synAcc, double velEnd, long segNum, short fifo)
{
	return GT_ArcZXROverride2WN(_ttoi(GetDeviceID()), crd, z, x, radius, circleDir, synVel, synAcc, velEnd, segNum, fifo);
}

short CDeviceGoogolGTS::__gts_ArcZXCOverride2WN(short crd, long z, long x, double zCenter, double xCenter, short circleDir, double synVel, double synAcc, double velEnd, long segNum, short fifo)
{
	return GT_ArcZXCOverride2WN(_ttoi(GetDeviceID()), crd, z, x, zCenter, xCenter, circleDir, synVel, synAcc, velEnd, segNum, fifo);
}

short CDeviceGoogolGTS::__gts_HelixXYRZWN(short crd, long x, long y, long z, double radius, short circleDir, double synVel, double synAcc, double velEnd, long segNum, short fifo)
{
	return GT_HelixXYRZWN(_ttoi(GetDeviceID()), crd, x, y, z, radius, circleDir, synVel, synAcc, velEnd, segNum, fifo);
}

short CDeviceGoogolGTS::__gts_HelixXYCZWN(short crd, long x, long y, long z, double xCenter, double yCenter, short circleDir, double synVel, double synAcc, double velEnd, long segNum, short fifo)
{
	return GT_HelixXYCZWN(_ttoi(GetDeviceID()), crd, x, y, z, xCenter, yCenter, circleDir, synVel, synAcc, velEnd, segNum, fifo);
}

short CDeviceGoogolGTS::__gts_HelixYZRXWN(short crd, long x, long y, long z, double radius, short circleDir, double synVel, double synAcc, double velEnd, long segNum, short fifo)
{
	return GT_HelixYZRXWN(_ttoi(GetDeviceID()), crd, x, y, z, radius, circleDir, synVel, synAcc, velEnd, segNum, fifo);
}

short CDeviceGoogolGTS::__gts_HelixYZCXWN(short crd, long x, long y, long z, double yCenter, double zCenter, short circleDir, double synVel, double synAcc, double velEnd, long segNum, short fifo)
{
	return GT_HelixYZCXWN(_ttoi(GetDeviceID()), crd, x, y, z, yCenter, zCenter, circleDir, synVel, synAcc, velEnd, segNum, fifo);
}

short CDeviceGoogolGTS::__gts_HelixZXRYWN(short crd, long x, long y, long z, double radius, short circleDir, double synVel, double synAcc, double velEnd, long segNum, short fifo)
{
	return GT_HelixZXRYWN(_ttoi(GetDeviceID()), crd, x, y, z, radius, circleDir, synVel, synAcc, velEnd, segNum, fifo);
}

short CDeviceGoogolGTS::__gts_HelixZXCYWN(short crd, long x, long y, long z, double zCenter, double xCenter, short circleDir, double synVel, double synAcc, double velEnd, long segNum, short fifo)
{
	return GT_HelixZXCYWN(_ttoi(GetDeviceID()), crd, x, y, z, zCenter, xCenter, circleDir, synVel, synAcc, velEnd, segNum, fifo);
}

short CDeviceGoogolGTS::__gts_HelixXYRZOverride2WN(short crd, long x, long y, long z, double radius, short circleDir, double synVel, double synAcc, double velEnd, long segNum, short fifo)
{
	return GT_HelixXYRZOverride2WN(_ttoi(GetDeviceID()), crd, x, y, z, radius, circleDir, synVel, synAcc, velEnd, segNum, fifo);
}

short CDeviceGoogolGTS::__gts_HelixXYCZOverride2WN(short crd, long x, long y, long z, double xCenter, double yCenter, short circleDir, double synVel, double synAcc, double velEnd, long segNum, short fifo)
{
	return GT_HelixXYCZOverride2WN(_ttoi(GetDeviceID()), crd, x, y, z, xCenter, yCenter, circleDir, synVel, synAcc, velEnd, segNum, fifo);
}

short CDeviceGoogolGTS::__gts_HelixYZRXOverride2WN(short crd, long x, long y, long z, double radius, short circleDir, double synVel, double synAcc, double velEnd, long segNum, short fifo)
{
	return GT_HelixYZRXOverride2WN(_ttoi(GetDeviceID()), crd, x, y, z, radius, circleDir, synVel, synAcc, velEnd, segNum, fifo);
}

short CDeviceGoogolGTS::__gts_HelixYZCXOverride2WN(short crd, long x, long y, long z, double yCenter, double zCenter, short circleDir, double synVel, double synAcc, double velEnd, long segNum, short fifo)
{
	return GT_HelixYZCXOverride2WN(_ttoi(GetDeviceID()), crd, x, y, z, yCenter, zCenter, circleDir, synVel, synAcc, velEnd, segNum, fifo);
}

short CDeviceGoogolGTS::__gts_HelixZXRYOverride2WN(short crd, long x, long y, long z, double radius, short circleDir, double synVel, double synAcc, double velEnd, long segNum, short fifo)
{
	return GT_HelixZXRYOverride2WN(_ttoi(GetDeviceID()), crd, x, y, z, radius, circleDir, synVel, synAcc, velEnd, segNum, fifo);
}

short CDeviceGoogolGTS::__gts_HelixZXCYOverride2WN(short crd, long x, long y, long z, double zCenter, double xCenter, short circleDir, double synVel, double synAcc, double velEnd, long segNum, short fifo)
{
	return GT_HelixZXCYOverride2WN(_ttoi(GetDeviceID()), crd, x, y, z, zCenter, xCenter, circleDir, synVel, synAcc, velEnd, segNum, fifo);
}

short CDeviceGoogolGTS::__gts_BufIO(short crd, unsigned short doType, unsigned short doMask, unsigned short doValue, short fifo)
{
	return GT_BufIO(_ttoi(GetDeviceID()), crd, doType, doMask, doValue, fifo);
}

short CDeviceGoogolGTS::__gts_BufEnableDoBitPulse(short crd, short doType, short doIndex, unsigned short highLevelTime, unsigned short lowLevelTime, long pulseNum, short firstLevel, short fifo)
{
	return GT_BufEnableDoBitPulse(_ttoi(GetDeviceID()), crd, doType, doIndex, highLevelTime, lowLevelTime, pulseNum, firstLevel, fifo);
}

short CDeviceGoogolGTS::__gts_BufDisableDoBitPulse(short crd, short doType, short doIndex, short fifo)
{
	return GT_BufDisableDoBitPulse(_ttoi(GetDeviceID()), crd, doType, doIndex, fifo);
}

short CDeviceGoogolGTS::__gts_BufDelay(short crd, unsigned short delayTime, short fifo)
{	
	return GT_BufDelay(_ttoi(GetDeviceID()), crd, delayTime, fifo);
}

short CDeviceGoogolGTS::__gts_BufComparePulse(short crd, short level, short outputType, short time, short fifo)
{
	return GT_BufComparePulse(_ttoi(GetDeviceID()), crd, level, outputType, time, fifo);
}

short CDeviceGoogolGTS::__gts_BufDA(short crd, short chn, short daValue, short fifo)
{
	return GT_BufDA(_ttoi(GetDeviceID()), crd, chn, daValue, fifo);
}

short CDeviceGoogolGTS::__gts_BufLmtsOn(short crd, short axis, short limitType, short fifo)
{
	return GT_BufLmtsOn(_ttoi(GetDeviceID()), crd, axis, limitType, fifo);
}

short CDeviceGoogolGTS::__gts_BufLmtsOff(short crd, short axis, short limitType, short fifo)
{
	return GT_BufLmtsOff(_ttoi(GetDeviceID()), crd, axis, limitType, fifo);
}

short CDeviceGoogolGTS::__gts_BufSetStopIo(short crd, short axis, short stopType, short inputType, short inputIndex, short fifo)
{

	return GT_BufSetStopIo(_ttoi(GetDeviceID()), crd, axis, stopType, inputType, inputIndex, fifo);
}

short CDeviceGoogolGTS::__gts_BufMove(short crd, short moveAxis, long pos, double vel, double acc, short modal, short fifo)
{
	return GT_BufMove(_ttoi(GetDeviceID()), crd, moveAxis, pos, vel, acc, modal, fifo);
}

short CDeviceGoogolGTS::__gts_BufGear(short crd, short gearAxis, long pos, short fifo)
{
	return GT_BufGear(_ttoi(GetDeviceID()), crd, gearAxis, pos, fifo);
}

short CDeviceGoogolGTS::__gts_BufGearPercent(short crd, short gearAxis, long pos, short accPercent, short decPercent, short fifo)
{
	return GT_BufGearPercent(_ttoi(GetDeviceID()), crd, gearAxis, pos, accPercent, decPercent, fifo);
}

short CDeviceGoogolGTS::__gts_BufStopMotion(short crd, short fifo)
{
	return GT_BufStopMotion(_ttoi(GetDeviceID()), crd, fifo);
}

short CDeviceGoogolGTS::__gts_BufSetVarValue(short crd, short pageId, __gts_STVarInfo *pVarInfo, double value, short fifo)
{
	short shReturn = SHORT_MAX;

	do 
	{
		if(!pVarInfo)
			break;

		
		TVarInfo sData;

		sData.id = pVarInfo->id;
		sData.dataType = pVarInfo->dataType;
		memcpy(sData.name, pVarInfo->name, sizeof(char) * 32);

		shReturn = GT_BufSetVarValue(_ttoi(GetDeviceID()), crd, pageId, &sData, value, fifo);

	} 
	while(false);

	return shReturn;
}

short CDeviceGoogolGTS::__gts_BufJumpNextSeg(short crd, short axis, short limitType, short fifo)
{
	return GT_BufJumpNextSeg(_ttoi(GetDeviceID()), crd, axis, limitType, fifo);
}

short CDeviceGoogolGTS::__gts_BufSynchPrfPos(short crd, short encoder, short profile, short fifo)
{
	return GT_BufSynchPrfPos(_ttoi(GetDeviceID()), crd, encoder, profile, fifo);
}

short CDeviceGoogolGTS::__gts_BufVirtualToActual(short crd, short fifo)
{
	return GT_BufVirtualToActual(_ttoi(GetDeviceID()), crd, fifo);
}

short CDeviceGoogolGTS::__gts_BufSetLongVar(short crd, short index, long value, short fifo)
{
	return GT_BufSetLongVar(_ttoi(GetDeviceID()), crd, index, value, fifo);
}

short CDeviceGoogolGTS::__gts_BufSetDoubleVar(short crd, short index, double value, short fifo)
{
	return GT_BufSetDoubleVar(_ttoi(GetDeviceID()), crd, index, value, fifo);
}

short CDeviceGoogolGTS::__gts_CrdStart(short mask, short option)
{
	return GT_CrdStart(_ttoi(GetDeviceID()), mask, option);
}

short CDeviceGoogolGTS::__gts_CrdStartStep(short mask, short option)
{
	return GT_CrdStartStep(_ttoi(GetDeviceID()), mask, option);
}

short CDeviceGoogolGTS::__gts_CrdStepMode(short mask, short option)
{
	return GT_CrdStepMode(_ttoi(GetDeviceID()), mask, option);
}

short CDeviceGoogolGTS::__gts_SetOverride(short crd, double synVelRatio)
{
	return GT_SetOverride(_ttoi(GetDeviceID()), crd, synVelRatio);
}

short CDeviceGoogolGTS::__gts_SetOverride2(short crd, double synVelRatio)
{
	return GT_SetOverride2(_ttoi(GetDeviceID()), crd, synVelRatio);
}

short CDeviceGoogolGTS::__gts_InitLookAhead(short crd, short fifo, double T, double accMax, short n, __gts_STCrdData *pLookAheadBuf)
{
	short shReturn = SHORT_MAX;

	do
	{
		if(!pLookAheadBuf)
			break;

		TCrdData sData;
		sData.motionType = pLookAheadBuf->motionType;
		sData.circlePlat = pLookAheadBuf->circlePlat;
		memcpy(sData.pos, pLookAheadBuf->pos, sizeof(long) * INTERPOLATION_AXIS_MAX);
		sData.radius = pLookAheadBuf->radius;
		sData.circleDir = pLookAheadBuf->circleDir;
		memcpy(sData.center, pLookAheadBuf->center, sizeof(double) * 3);
		sData.vel = pLookAheadBuf->vel;
		sData.acc = pLookAheadBuf->acc;
		sData.velEndZero = pLookAheadBuf->velEndZero;

		sData.operation.flag = pLookAheadBuf->operation.flag;
		sData.operation.delay = pLookAheadBuf->operation.delay;
		sData.operation.doType = pLookAheadBuf->operation.doType;
		sData.operation.doMask = pLookAheadBuf->operation.doMask;
		sData.operation.doValue = pLookAheadBuf->operation.doValue;
		memcpy(sData.operation.dataExt, pLookAheadBuf->operation.dataExt, sizeof(unsigned short) * CRD_OPERATION_DATA_EXT_MAX);

		memcpy(sData.cos, pLookAheadBuf->cos, sizeof(double) * INTERPOLATION_AXIS_MAX);
		sData.velEnd = pLookAheadBuf->velEnd;
		sData.velEndAdjust = pLookAheadBuf->velEndAdjust;
		sData.r = pLookAheadBuf->r;

		shReturn = GT_InitLookAhead(_ttoi(GetDeviceID()), crd, fifo, T, accMax, n, &sData);
	}
	while(false);

	return shReturn;
}

short CDeviceGoogolGTS::__gts_GetLookAheadSpace(short crd, long *pSpace, short fifo)
{
	short shReturn = SHORT_MAX;

	do
	{
		if(!pSpace)
			break;

		shReturn = GT_GetLookAheadSpace(_ttoi(GetDeviceID()), crd, pSpace, fifo);
	}
	while(false);

	return shReturn;
}

short CDeviceGoogolGTS::__gts_GetLookAheadSegCount(short crd, long *pSegCount, short fifo)
{
	short shReturn = SHORT_MAX;

	do
	{
		if(!pSegCount)
			break;

		shReturn = GT_GetLookAheadSegCount(_ttoi(GetDeviceID()), crd, pSegCount, fifo);
	}
	while(false);

	return shReturn;
}

short CDeviceGoogolGTS::__gts_CrdClear(short crd, short fifo)
{
	return GT_CrdClear(_ttoi(GetDeviceID()), crd, fifo);
}

short CDeviceGoogolGTS::__gts_CrdStatus(short crd, short *pRun, long *pSegment, short fifo)
{
	short shReturn = SHORT_MAX;

	do
	{
		if(!pRun || !pSegment)
			break;

		shReturn = GT_CrdStatus(_ttoi(GetDeviceID()), crd, pRun, pSegment, fifo);
	}
	while(false);

	return shReturn;
}

short CDeviceGoogolGTS::__gts_SetUserSegNum(short crd, long segNum, short fifo)
{
	return GT_SetUserSegNum(_ttoi(GetDeviceID()), crd, segNum, fifo);
}

short CDeviceGoogolGTS::__gts_GetUserSegNum(short crd, long *pSegment, short fifo)
{
	short shReturn = SHORT_MAX;

	do
	{
		if(!pSegment)
			break;

		shReturn = GT_GetUserSegNum(_ttoi(GetDeviceID()), crd, pSegment, fifo);
	}
	while(false);

	return shReturn;
}

short CDeviceGoogolGTS::__gts_GetUserSegNumWN(short crd, long *pSegment, short fifo)
{
	short shReturn = SHORT_MAX;

	do
	{
		if(!pSegment)
			break;

		shReturn = GT_GetUserSegNumWN(_ttoi(GetDeviceID()), crd, pSegment, fifo);
	}
	while(false);

	return shReturn;
}

short CDeviceGoogolGTS::__gts_GetRemainderSegNum(short crd, long *pSegment, short fifo)
{
	short shReturn = SHORT_MAX;

	do
	{
		if(!pSegment)
			break;

		shReturn = GT_GetRemainderSegNum(_ttoi(GetDeviceID()), crd, pSegment, fifo);
	}
	while(false);

	return shReturn;
}

short CDeviceGoogolGTS::__gts_SetCrdStopDec(short crd, double decSmoothStop, double decAbruptStop)
{
	return GT_SetCrdStopDec(_ttoi(GetDeviceID()), crd, decSmoothStop, decAbruptStop);
}

short CDeviceGoogolGTS::__gts_GetCrdStopDec(short crd, double *pDecSmoothStop, double *pDecAbruptStop)
{
	short shReturn = SHORT_MAX;

	do
	{
		if(!pDecSmoothStop || !pDecAbruptStop)
			break;

		shReturn = GT_GetCrdStopDec(_ttoi(GetDeviceID()), crd, pDecSmoothStop, pDecAbruptStop);
	}
	while(false);

	return shReturn;
}

short CDeviceGoogolGTS::__gts_SetCrdLmtStopMode(short crd, short lmtStopMode)
{
	return GT_SetCrdLmtStopMode(_ttoi(GetDeviceID()), crd, lmtStopMode);
}

short CDeviceGoogolGTS::__gts_GetCrdLmtStopMode(short crd, short *pLmtStopMode)
{
	short shReturn = SHORT_MAX;

	do
	{
		if(!pLmtStopMode)
			break;

		shReturn = GT_GetCrdLmtStopMode(_ttoi(GetDeviceID()), crd, pLmtStopMode);
	}
	while(false);

	return shReturn;
}

short CDeviceGoogolGTS::__gts_GetUserTargetVel(short crd, double *pTargetVel)
{
	short shReturn = SHORT_MAX;

	do
	{
		if(!pTargetVel)
			break;

		shReturn = GT_GetUserTargetVel(_ttoi(GetDeviceID()), crd, pTargetVel);
	}
	while(false);

	return shReturn;
}

short CDeviceGoogolGTS::__gts_GetSegTargetPos(short crd, long *pTargetPos)
{
	short shReturn = SHORT_MAX;

	do
	{
		if(!pTargetPos)
			break;

		shReturn = GT_GetSegTargetPos(_ttoi(GetDeviceID()), crd, pTargetPos);
	}
	while(false);

	return shReturn;
}

short CDeviceGoogolGTS::__gts_GetCrdPos(short crd, double *pPos)
{
	short shReturn = SHORT_MAX;

	do
	{
		if(!pPos)
			break;

		shReturn = GT_GetCrdPos(_ttoi(GetDeviceID()), crd, pPos);
	}
	while(false);

	return shReturn;
}

short CDeviceGoogolGTS::__gts_GetCrdVel(short crd, double *pSynVel)
{
	short shReturn = SHORT_MAX;

	do
	{
		if(!pSynVel)
			break;

		shReturn = GT_GetCrdVel(_ttoi(GetDeviceID()), crd, pSynVel);
	}
	while(false);

	return shReturn;
}

short CDeviceGoogolGTS::__gts_SetCrdSingleMaxVel(short crd, double *pMaxVel)
{
	short shReturn = SHORT_MAX;

	do
	{
		if(!pMaxVel)
			break;

		shReturn = GT_SetCrdSingleMaxVel(_ttoi(GetDeviceID()), crd, pMaxVel);
	}
	while(false);

	return shReturn;
}

short CDeviceGoogolGTS::__gts_GetCrdSingleMaxVel(short crd, double *pMaxVel)
{
	short shReturn = SHORT_MAX;

	do
	{
		if(!pMaxVel)
			break;

		shReturn = GT_GetCrdSingleMaxVel(_ttoi(GetDeviceID()), crd, pMaxVel);
	}
	while(false);

	return shReturn;
}

short CDeviceGoogolGTS::__gts_BufLaserOn(short crd, short fifo, short channel)
{
	return GT_BufLaserOn(_ttoi(GetDeviceID()), crd, fifo, channel);
}

short CDeviceGoogolGTS::__gts_BufLaserOff(short crd, short fifo, short channel)
{
	return GT_BufLaserOff(_ttoi(GetDeviceID()), crd, fifo, channel);
}
//////////////////////////////////////////////////////////////////////////
/////////

short CDeviceGoogolGTS::__gts_BufLaserPrfCmd(short crd, double laserPower, short fifo, short channel)
{
	return GT_BufLaserPrfCmd(_ttoi(GetDeviceID()), crd, laserPower, fifo, channel);
}

short CDeviceGoogolGTS::__gts_BufLaserFollowRatio(short crd, double ratio, double minPower, double maxPower, short fifo, short channel)
{
	return GT_BufLaserFollowRatio(_ttoi(GetDeviceID()), crd, ratio, minPower, maxPower, fifo, channel);
}

short CDeviceGoogolGTS::__gts_BufLaserFollowMode(short crd, short source, short fifo, short channel, double startPower)
{
	return GT_BufLaserFollowMode(_ttoi(GetDeviceID()), crd, source, fifo, channel, startPower);
}

short CDeviceGoogolGTS::__gts_BufLaserFollowOff(short crd, short fifo, short channel)
{
	return GT_BufLaserFollowOff(_ttoi(GetDeviceID()), crd, fifo, channel);
}

short CDeviceGoogolGTS::__gts_BufLaserFollowSpline(short crd, short tableId, double minPower, double maxPower, short fifo, short channel)
{
	return GT_BufLaserFollowSpline(_ttoi(GetDeviceID()), crd, tableId, minPower, maxPower, fifo, channel);
}

short CDeviceGoogolGTS::__gts_PrfPvt(short profile)
{
	return GT_PrfPvt(_ttoi(GetDeviceID()), profile);
}

short CDeviceGoogolGTS::__gts_SetPvtLoop(short profile, long loop)
{
	return GT_SetPvtLoop(_ttoi(GetDeviceID()), profile, loop);
}

short CDeviceGoogolGTS::__gts_GetPvtLoop(short profile, long *pLoopCount, long *pLoop)
{
	short shReturn = SHORT_MAX;

	do
	{
		if(!pLoopCount || !pLoop)
			break;

		shReturn = GT_GetPvtLoop(_ttoi(GetDeviceID()), profile, pLoopCount, pLoop);
	}
	while(false);

	return shReturn;
}

short CDeviceGoogolGTS::__gts_PvtStatus(short profile, short *pTableId, double *pTime, short count)
{
	short shReturn = SHORT_MAX;

	do
	{
		if(!pTableId || !pTime)
			break;

		shReturn = GT_PvtStatus(_ttoi(GetDeviceID()), profile, pTableId, pTime, count);
	}
	while(false);

	return shReturn;
}

short CDeviceGoogolGTS::__gts_PvtStart(long mask)
{
	return GT_PvtStart(_ttoi(GetDeviceID()), mask);
}

short CDeviceGoogolGTS::__gts_PvtTableSelect(short profile, short tableId)
{
	return GT_PvtTableSelect(_ttoi(GetDeviceID()), profile, tableId);
}


short CDeviceGoogolGTS::__gts_PvtTable(short tableId, long count, double *pTime, double *pPos, double *pVel)
{
	short shReturn = SHORT_MAX;

	do
	{
		if(!pTime || !pPos || !pVel)
			break;

		shReturn = GT_PvtTable(_ttoi(GetDeviceID()), tableId, count, pTime, pPos, pVel);
	}
	while(false);

	return shReturn;
}

short CDeviceGoogolGTS::__gts_PvtTableEx(short tableId, long count, double *pTime, double *pPos, double *pVelBegin, double *pVelEnd)
{
	short shReturn = SHORT_MAX;

	do
	{
		if(!pTime || !pPos || !pVelBegin || !pVelEnd)
			break;

		shReturn = GT_PvtTableEx(_ttoi(GetDeviceID()), tableId, count, pTime, pPos, pVelBegin, pVelEnd);
	}
	while(false);

	return shReturn;
}

short CDeviceGoogolGTS::__gts_PvtTableComplete(short tableId, long count, double *pTime, double *pPos, double *pA, double *pB, double *pC, double velBegin, double velEnd)
{
	short shReturn = SHORT_MAX;

	do
	{
		if(!pTime || !pPos || !pA || !pB || !pC)
			break;

		shReturn = GT_PvtTableComplete(_ttoi(GetDeviceID()), tableId, count, pTime, pPos, pA, pB, pC, velBegin, velEnd);
	}
	while(false);

	return shReturn;
}

short CDeviceGoogolGTS::__gts_PvtTablePercent(short tableId, long count, double *pTime, double *pPos, double *pPercent, double velBegin)
{
	short shReturn = SHORT_MAX;

	do
	{
		if(!pTime || !pPos || !pPercent)
			break;

		shReturn = GT_PvtTablePercent(_ttoi(GetDeviceID()), tableId, count, pTime, pPos, pPercent, velBegin);
	}
	while(false);

	return shReturn;
}

short CDeviceGoogolGTS::__gts_PvtPercentCalculate(long n, double *pTime, double *pPos, double *pPercent, double velBegin, double *pVel)
{
	short shReturn = SHORT_MAX;

	do
	{
		if(!pTime || !pPos || !pPercent || pVel)
			break;

		shReturn = GT_PvtPercentCalculate(_ttoi(GetDeviceID()), n, pTime, pPos, pPercent, velBegin, pVel);
	}
	while(false);

	return shReturn;
}

short CDeviceGoogolGTS::__gts_PvtTableContinuous(short tableId, long count, double *pPos, double *pVel, double *pPercent, double *pVelMax, double *pAcc, double *pDec, double timeBegin)
{
	short shReturn = SHORT_MAX;

	do
	{
		if(!pPos || !pVel || !pPercent || !pVelMax || !pAcc || !pDec)
			break;

		shReturn = GT_PvtTableContinuous(_ttoi(GetDeviceID()), tableId, count, pPos, pVel, pPercent, pVelMax, pAcc, pDec, timeBegin);
	}
	while(false);

	return shReturn;
}

short CDeviceGoogolGTS::__gts_PvtContinuousCalculate(long n, double *pPos, double *pVel, double *pPercent, double *pVelMax, double *pAcc, double *pDec, double *pTime)
{
	short shReturn = SHORT_MAX;

	do
	{
		if(!pPos || !pVel || !pPercent || !pVelMax || !pAcc || !pDec || !pTime)
			break;

		shReturn = GT_PvtContinuousCalculate(_ttoi(GetDeviceID()), n, pPos, pVel, pPercent, pVelMax, pAcc, pDec, pTime);
	}
	while(false);

	return shReturn;
}

short CDeviceGoogolGTS::__gts_PvtTableMovePercent(short tableId, long distance, double vm, double acc, double pa1, double pa2, double dec, double pd1, double pd2, double *pVel, double *pAcc, double *pDec, double *pTime)
{
	short shReturn = SHORT_MAX;

	do
	{
		if(!pVel || !pAcc || !pDec || !pTime)
			break;


		shReturn = GT_PvtTableMovePercent(_ttoi(GetDeviceID()), tableId, distance, vm, acc, pa1, pa2, dec, pd1, pd2, pVel, pAcc, pDec, pTime);
	}
	while(false);

	return shReturn;
}

short CDeviceGoogolGTS::__gts_HomeInit()
{
	return GT_HomeInit(_ttoi(GetDeviceID()));
}

short CDeviceGoogolGTS::__gts_Home(short axis, long pos, double vel, double acc, long offset)
{
	return GT_Home(_ttoi(GetDeviceID()), axis, pos, vel, acc, offset);
}

short CDeviceGoogolGTS::__gts_Index(short axis, long pos, long offset)
{
	return GT_Index(_ttoi(GetDeviceID()), axis, pos, offset);
}

short CDeviceGoogolGTS::__gts_HomeStop(short axis, long pos, double vel, double acc)
{
	return GT_HomeStop(_ttoi(GetDeviceID()), axis, pos, vel, acc);
}

short CDeviceGoogolGTS::__gts_HomeSts(short axis, unsigned short *pStatus)
{
	short shReturn = SHORT_MAX;

	do
	{
		if(!pStatus)
			break;

		shReturn = GT_HomeSts(_ttoi(GetDeviceID()), axis, pStatus);
	}
	while(false);

	return shReturn;
}


short CDeviceGoogolGTS::__gts_HandwheelInit()
{
	return GT_HandwheelInit(_ttoi(GetDeviceID()));
}

short CDeviceGoogolGTS::__gts_SetHandwheelStopDec(short slave, double decSmoothStop, double decAbruptStop)
{
	return GT_SetHandwheelStopDec(_ttoi(GetDeviceID()), slave, decSmoothStop, decAbruptStop);
}

short CDeviceGoogolGTS::__gts_StartHandwheel(short slave, short master, short masterEven, short slaveEven, short intervalTime, double acc, double dec, double vel, short stopWaitTime)
{
	return GT_StartHandwheel(_ttoi(GetDeviceID()), slave, master, masterEven, slaveEven, intervalTime, acc, dec, vel, stopWaitTime);
}

short CDeviceGoogolGTS::__gts_EndHandwheel(short slave)
{
	return GT_EndHandwheel(_ttoi(GetDeviceID()), slave);
}


short CDeviceGoogolGTS::__gts_GoHome(short axis, __gts_STHomePrm *pHomePrm)
{
	short shReturn = SHORT_MAX;

	do
	{
		if(!pHomePrm)
			break;

		THomePrm sData;

		sData.mode = pHomePrm->mode;
		sData.moveDir = pHomePrm->moveDir;
		sData.indexDir = pHomePrm->indexDir;
		sData.edge = pHomePrm->edge;
		sData.triggerIndex = pHomePrm->triggerIndex;
		memcpy(sData.pad1, pHomePrm->pad1, sizeof(short) * 3);
		sData.velHigh = pHomePrm->velHigh;
		sData.velLow = pHomePrm->velLow;
		sData.acc = pHomePrm->acc;
		sData.dec = pHomePrm->dec;
		sData.smoothTime = pHomePrm->smoothTime;
		memcpy(sData.pad2, pHomePrm->pad2, sizeof(short) * 3);
		sData.homeOffset = pHomePrm->homeOffset;
		sData.searchHomeDistance = pHomePrm->searchHomeDistance;
		sData.searchIndexDistance = pHomePrm->searchIndexDistance;
		sData.escapeStep = pHomePrm->escapeStep;
		memcpy(sData.pad3, pHomePrm->pad3, sizeof(long) * 2);

		shReturn = GT_GoHome(_ttoi(GetDeviceID()), axis, &sData);
	}
	while(false);

	return shReturn;
}

short CDeviceGoogolGTS::__gts_GetHomePrm(short axis, __gts_STHomePrm *pHomePrm)
{
	short shReturn = SHORT_MAX;

	do
	{
		if(!pHomePrm)
			break;
		
		THomePrm sData;

		shReturn = GT_GetHomePrm(_ttoi(GetDeviceID()), axis, &sData);

		if(!shReturn)
		{
			pHomePrm->mode = sData.mode;
			pHomePrm->moveDir = sData.moveDir;
			pHomePrm->indexDir = sData.indexDir;
			pHomePrm->edge = sData.edge;
			pHomePrm->triggerIndex = sData.triggerIndex;
			memcpy(pHomePrm->pad1, sData.pad1, sizeof(short) * 3);
			pHomePrm->velHigh = sData.velHigh;
			pHomePrm->velLow = sData.velLow;
			pHomePrm->acc = sData.acc;
			pHomePrm->dec = sData.dec;
			pHomePrm->smoothTime = sData.smoothTime;
			memcpy(pHomePrm->pad2, sData.pad2, sizeof(short) * 3);
			pHomePrm->homeOffset = sData.homeOffset;
			pHomePrm->searchHomeDistance = sData.searchHomeDistance;
			pHomePrm->searchIndexDistance = sData.searchIndexDistance;
			pHomePrm->escapeStep = sData.escapeStep;
			memcpy(pHomePrm->pad3, sData.pad3, sizeof(long) * 2);
		}
	}
	while(false);

	return shReturn;
}

short CDeviceGoogolGTS::__gts_GetHomeStatus(short axis, __gts_STHomeStatus *pHomeStatus)
{
	short shReturn = SHORT_MAX;

	do
	{
		if(!pHomeStatus)
			break;

		THomeStatus sData;


		typedef struct
		{
			short run;
			short stage;
			short error;
			short pad1;
			long capturePos;
			long targetPos;
		} THomeStatus;


		shReturn = GT_GetHomeStatus(_ttoi(GetDeviceID()), axis, &sData);

		if(!shReturn)
		{
			pHomeStatus->run = sData.run;
			pHomeStatus->stage = sData.stage;
			pHomeStatus->error = sData.error;
			pHomeStatus->pad1 = sData.pad1;
			pHomeStatus->capturePos = sData.capturePos;
			pHomeStatus->targetPos = sData.targetPos;
		}
	}
	while(false);

	return shReturn;
}


IMPLEMENT_DYNAMIC(CGoogolDioViewer, CRavidDialogBase)

BEGIN_MESSAGE_MAP(CGoogolDioViewer, CRavidDialogBase)
	ON_COMMAND_RANGE(EDeviceAddEnumeratedControlID_ExtOutputLedStart, EDeviceAddEnumeratedControlID_ExtOutputLedEnd, OnBnClickedExtOutLed)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

int Ravid::Framework::CGoogolDioViewer::m_nTotalID = 0;

CGoogolDioViewer::CGoogolDioViewer(CWnd * pParent)
	: CRavidDialogBase(IDD_RAVID_DEVICE_GOOGOL_IO, pParent)
{
	m_dwExtOutStatus = 0;

	SetReceiveMessage(true);
}

CGoogolDioViewer::~CGoogolDioViewer()
{
	for(auto &iter : m_vctCtrl)
	{
		if(iter)
		{
			iter->DestroyWindow();
			delete iter;
			iter = nullptr;
		}
	}

	m_vctCtrl.clear();

	DestroyWindow();
}

ptrdiff_t CGoogolDioViewer::OnMessage(CMessageBase * pMessage)
{
	__super::OnMessage(pMessage);

	do
	{
		if(!pMessage)
			break;

		if(pMessage->GetMessage() != EMessage_LanguageChange)
			break;

		if(!IsWindow(GetSafeHwnd()))
			break;

		CString strForm;
		strForm.Format(_T("%s %d %s"), CMultiLanguageManager::GetString(ELanguageParameter_GoogolExternalIO), (m_nViewID % 16) + 1, CMultiLanguageManager::GetString(ELanguageParameter_Page));

		SetWindowText(strForm);

		CWnd* pCtrlUI = nullptr;

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Input));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 1);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Output));
	}
	while(false);

	return 0;
}

void CGoogolDioViewer::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

//	DDX_Control(pDX, IDC_EDIT_RAVID_IMAGE_TOOL_SCRIPT, m_editScript);
}

BOOL CGoogolDioViewer::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	do
	{
		SetAdaptiveScaleAlign(EAdaptiveScaleAlign_LeftTop);
		
		CMenu* pMenu = this->GetSystemMenu(FALSE);
		if(pMenu)
			pMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);

		EnableAdaptiveScale(false);
		SetWindowSizeRatioMode(false);
		EnableWindowSize(false);

		CString strForm;
		strForm.Format(_T("%s %d %s"), CMultiLanguageManager::GetString(ELanguageParameter_GoogolExternalIO), (m_nViewID % 16) + 1, CMultiLanguageManager::GetString(ELanguageParameter_Page));

		SetWindowText(strForm);
	}
	while(false);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CGoogolDioViewer::PreTranslateMessage(MSG * pMsg)
{
	return CRavidDialogBase::PreTranslateMessage(pMsg);
}

bool CGoogolDioViewer::Create(CWnd* pParent)
{
	bool bReturn = false;

	do
	{
		m_nViewID = m_nTotalID;
		++m_nTotalID;

		if(!__super::Create(IDD_RAVID_DEVICE_GOOGOL_IO, pParent))
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

void CGoogolDioViewer::SetInitialize(bool bInit)
{
	m_bIsInitialize = bInit;
}

bool CGoogolDioViewer::IsInitialized()
{
	return m_bIsInitialize;
}

void CGoogolDioViewer::SetLabelString(int nID, CString strFormat)
{
	SetDlgItemText(nID, strFormat);
}

bool CGoogolDioViewer::WriteGenExtOutBit(int nBit, bool bOn)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nBit < 0 || nBit >= GTS_DIO_EXTOUT_PORT)
			break;

		if(GT_SetExtIoBit(m_nDeviceID, m_nViewID, nBit, !bOn))
			break;

		if (bOn)
			m_dwExtOutStatus |= (1 << nBit);
		else
			m_dwExtOutStatus &= ~(1 << nBit);

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CGoogolDioViewer::ReadGenExtOutBit(int nBit)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nBit >= GTS_DIO_EXTOUT_PORT)
			break;

		bReturn = (m_dwExtOutStatus >> nBit) & 0x01;
	}
	while(false);

	return bReturn;
}

bool CGoogolDioViewer::ReadGenExtInputBit(int nBit)
{
	bool bReturn = false;
	
	do
	{
		if(!IsInitialized())
			break;

		if(nBit < 0 || nBit >= GTS_DIO_EXTIN_PORT)
			break;

		unsigned short nsValue = 0;

		if(GT_GetExtIoBit(m_nDeviceID, m_nViewID, nBit, &nsValue))
			break;

		bReturn = nsValue == 1 ? false : true;
	}
	while(false);

	return bReturn;
}

void CGoogolDioViewer::ClearExtOutport()
{
	do
	{
		for (int i = 0; i < GTS_EXTIO_PORT; ++i)
			WriteGenExtOutBit(i, true);

		for (int i = 0; i < GTS_EXTIO_PORT; ++i)
			WriteGenExtOutBit(i, false);
	}
	while (false);
}

void CGoogolDioViewer::OnBnClickedExtOutLed(UINT nID)
{
	int nNumber = nID - EDeviceAddEnumeratedControlID_ExtOutputLedStart;
	nNumber %= 16;

	if((CAuthorityManager::GetDeviceControlPermission() <= CAuthorityManager::GetCurrentUserType()) && !CSequenceManager::IsRunning())
	{
		bool bBit = ReadGenExtOutBit(nNumber);

		WriteGenExtOutBit(nNumber, !bBit);
	}
}

void CGoogolDioViewer::OnDestroy()
{
	__super::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	for(auto &iter : m_vctCtrl)
	{
		if(iter)
		{
			iter->DestroyWindow();
			delete iter;
			iter = nullptr;
		}
	}

	m_vctCtrl.clear();
}

void CGoogolDioViewer::OnCancel()
{
	CRavidDialogBase::OnCancel();
}

void CGoogolDioViewer::OnLedStatus()
{
	if(IsWindowVisible())
	{
		int nStartID = m_nViewID * GTS_EXTIO_PORT;

		for(int i = nStartID; i < nStartID + GTS_EXTIO_PORT; ++i)
		{
			CRavidLedCtrl* pLed = (CRavidLedCtrl*)GetDlgItem(EDeviceAddEnumeratedControlID_ExtInputLedStart + i);

			if(pLed)
			{
				pLed->SetActivate(IsInitialized());

				if(ReadGenExtInputBit(i))
					pLed->On();
				else
					pLed->Off();
			}
		}

		for(int i = nStartID; i < nStartID + GTS_EXTIO_PORT; ++i)
		{
			CRavidLedCtrl* pLed = (CRavidLedCtrl*)GetDlgItem(EDeviceAddEnumeratedControlID_ExtOutputLedStart + i);

			if(pLed)
			{
				pLed->SetActivate(IsInitialized());

				if(ReadGenExtOutBit(i))
					pLed->On();
				else
					pLed->Off();
			}
		}
	}
}
