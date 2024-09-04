#pragma once

#include <gdiplus.h>
#include <vector>

typedef	unsigned short			word, ushort;
typedef	unsigned int			uint, dword;
typedef	INT_PTR					intptr;
typedef	UINT_PTR				uintptr;
typedef	__int64					int64;
typedef	unsigned __int64		uint64, qword;
typedef	WCHAR					wchar;
typedef	HANDLE					handle;
typedef	__m128					m128, xmmf;	// 128-bit XMM 레지스터
typedef __m128i					m128i, xmmi;
typedef	__m128d					m128d, xmmd;
typedef	__m256					m256, ymmf;	// 256-bit YMM 레지스터
typedef __m256i					m256i, ymmi;
typedef	__m256d					m256d, ymmd;
// typedef	__m512					m512, zmmf;	// 512-bit ZMM 레지스터
// typedef __m512i					m512i, zmmi;
// typedef	__m512d					m512d, zmmd;
typedef	std::vector<int>		vectori;
typedef	std::vector<uint>		vectoru;
typedef	std::vector<int64>		vectori64;
typedef	std::vector<uint64>		vectoru64;
typedef	std::vector<float>		vectorf;
typedef	std::vector<double>		vectord;
typedef	std::vector<CString>	vectort;

typedef union char2
{
	struct { char   x, y; };
	struct { char   w, h; };
	struct { char min, max; };
} char2;

typedef union byte2
{
	struct { unsigned char   x, y; };
	struct { unsigned char   w, h; };
	struct { unsigned char min, max; };
} byte2;

typedef union short2
{
	struct { short   x, y; };
	struct { short   w, h; };
	struct { short min, max; };
} short2;

typedef union int2
{
	struct { int   x, y; };
	struct { int   w, h; };
	struct { int min, max; };
} int2;

typedef union float2
{
	struct { float   x, y; };
	struct { float   w, h; };
	struct { float min, max; };
} float2;

typedef union double2
{
	struct { double   x, y; };
	struct { double   w, h; };
	struct { double min, max; };
} double2;


typedef union char3
{
	struct { char	  x, y, z; };
	struct { char	  w, h, d; };	// width, height, depth
	struct { char	  r, g, b; };
	struct { char	min, max, avg; };	// min, max, average
	struct { char	min, max, len; };	// min, max, length
} char3;

typedef union byte3
{
	struct { unsigned char	  x, y, z; };
	struct { unsigned char	  w, h, d; };
	struct { unsigned char	  r, g, b; };
	struct { unsigned char	min, max, avg; };
	struct { unsigned char	min, max, len; };
} byte3;

typedef union short3
{
	struct { short	  x, y, z; };
	struct { short	  w, h, d; };
	struct { short	  r, g, b; };
	struct { short	min, max, avg; };
	struct { short	min, max, len; };
} short3;

typedef union ushort3
{
	struct { ushort	  x, y, z; };
	struct { ushort	  w, h, d; };
	struct { ushort	  r, g, b; };
	struct { ushort	min, max, avg; };
	struct { ushort	min, max, len; };
} ushort3;

typedef union int3
{
	struct { int	  x, y, z; };
	struct { int	  w, h, d; };
	struct { int	  r, g, b; };
	struct { int	min, max, avg; };
	struct { int	min, max, len; };
} int3;

typedef union float3
{
	struct { float	  x, y, z; };
	struct { float	  w, h, d; };
	struct { float	  r, g, b; };
	struct { float	min, max, avg; };
	struct { float	min, max, len; };
} float3;

typedef union double3
{
	struct { double	  x, y, z; };
	struct { double	  w, h, d; };
	struct { double	  r, g, b; };
	struct { double	min, max, avg; };
	struct { double	min, max, len; };
} double3;


typedef union char4
{
	struct { char x, y, z, w; };
	struct { char r, g, b, a; };
	struct { char x, y, width, height; };
	struct { char x1, y1, x2, y2; };
} char4;

typedef union byte4
{
	struct { unsigned char x, y, z, w; };
	struct { unsigned char r, g, b, a; };
	struct { unsigned char x, y, width, height; };
	struct { unsigned char x1, y1, x2, y2; };
} byte4;

typedef union short4
{
	struct { short x, y, z, w; };
	struct { short r, g, b, a; };
	struct { short x, y, width, height; };
	struct { short x1, y1, x2, y2; };
} short4;

typedef union int4
{
	struct { int x, y, z, w; };
	struct { int r, g, b, a; };
	struct { int x, y, width, height; };
	struct { int x1, y1, x2, y2; };
} int4;

typedef union float4
{
	struct { float x, y, z, w; };
	struct { float r, g, b, a; };
	struct { float x, y, width, height; };
	struct { float x1, y1, x2, y2; };
} float4;

typedef union double4
{
	struct { double x, y, z, w; };
	struct { double r, g, b, a; };
	struct { double x, y, width, height; };
	struct { double x1, y1, x2, y2; };
} double4;


typedef union float4x4
{
	float		a[16];

	struct
	{
		float	a11, a12, a13, a14,
			a21, a22, a23, a24,
			a31, a32, a33, a34,
			a41, a42, a43, a44;
	};

	struct
	{
		float	x1, x2, x3, x4,
			y1, y2, y3, y4,
			z1, z2, z3, z4,
			w1, w2, w3, w4;
	};

	struct
	{
		float4	a1,
			a2,
			a3,
			a4;
	};
} float4x4;

typedef union double4x4
{
	double		a[16];

	struct
	{
		double	a11, a12, a13, a14,
			a21, a22, a23, a24,
			a31, a32, a33, a34,
			a41, a42, a43, a44;
	};

	struct
	{
		double	x1, x2, x3, x4,
			y1, y2, y3, y4,
			z1, z2, z3, z4,
			w1, w2, w3, w4;
	};

	struct
	{
		double4	a1,
			a2,
			a3,
			a4;
	};
} double4x4;
