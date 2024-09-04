#pragma once

#ifndef _M_X64		// 32bit
	#ifdef WIN64
		#undef WIN64
	#endif

	#ifdef _WIN64
		#undef _WIN64
	#endif

	#ifndef WIN32
		#define WIN32
	#endif

	#ifndef _WIN32
		#define _WIN32
	#endif
#else				// 64bit
	#ifndef WIN64
		#define WIN64
	#endif

	#ifndef _WIN64
		#define _WIN64
	#endif
#endif


#ifndef _WIN64
	#ifdef _DEBUG
		#define	LIB_DEBUG_PREFIX	"../ZDebug/"
		#define	DLL_DEBUG_PREFIX	"../ZDebug/"
		#define	LIB_DEBUG_POSTFIX	"D.lib"
		#define	DLL_DEBUG_POSTFIX	"D.dll"
	#else
		#define	LIB_DEBUG_PREFIX	"../ZRelease/"
		#define	DLL_DEBUG_PREFIX	"../ZRelease/"
		#define	LIB_DEBUG_POSTFIX	".lib"
		#define	DLL_DEBUG_POSTFIX	".dll"
	#endif
#else
	#ifdef _DEBUG
		#define	LIB_DEBUG_PREFIX	"../ZDebugX64/"
		#define	DLL_DEBUG_PREFIX	"../ZDebugX64/"
		#define	LIB_DEBUG_POSTFIX	"D.lib"
		#define	DLL_DEBUG_POSTFIX	"D.dll"
	#else
		#define	LIB_DEBUG_PREFIX	"../ZReleaseX64/"
		#define	DLL_DEBUG_PREFIX	"../ZReleaseX64/"
		#define	LIB_DEBUG_POSTFIX	".lib"
		#define	DLL_DEBUG_POSTFIX	".dll"
	#endif
#endif

#ifndef _WIN64
	#define	COMMONLIB_PREFIX	"../Libraries/x86/Vcc/"
	#define	COMMONDLL_PREFIX	"../Libraries/x86/Shared/"
#else
	#define	COMMONLIB_PREFIX	"../Libraries/x64/Vcc/"
	#define	COMMONDLL_PREFIX	"../Libraries/x64/Shared/"
#endif

#ifndef _WIN64
	#ifdef _DEBUG
		#define	LIB_DEBUG_PREFIX2	"ZDebug/"
		#define	DLL_DEBUG_PREFIX2	"ZDebug/"
		#define	LIB_DEBUG_POSTFIX2	"D.lib"
		#define	DLL_DEBUG_POSTFIX2	"D.dll"
	#else
		#define	LIB_DEBUG_PREFIX2	"ZRelease/"
		#define	DLL_DEBUG_PREFIX2	"ZRelease/"
		#define	LIB_DEBUG_POSTFIX2	".lib"
		#define	DLL_DEBUG_POSTFIX2	".dll"
	#endif
#else
	#ifdef _DEBUG
		#define	LIB_DEBUG_PREFIX2	"ZDebugX64/"
		#define	DLL_DEBUG_PREFIX2	"ZDebugX64/"
		#define	LIB_DEBUG_POSTFIX2	"D.lib"
		#define	DLL_DEBUG_POSTFIX2	"D.dll"
	#else
		#define	LIB_DEBUG_PREFIX2	"ZReleaseX64/"
		#define	DLL_DEBUG_PREFIX2	"ZReleaseX64/"
		#define	LIB_DEBUG_POSTFIX2	".lib"
		#define	DLL_DEBUG_POSTFIX2	".dll"
	#endif
#endif
