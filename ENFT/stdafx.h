////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2017 Zhejiang University
//  All Rights Reserved
//
//  For more information see <https://github.com/ZJUCVG/ENFT>
//  If you use this code, please cite the corresponding publications as 
//  listed on the above website.
//
//  Permission to use, copy, modify and distribute this software and its
//  documentation for educational, research and non-profit purposes only.
//  Any modification based on this work must be open source and prohibited
//  for commercial use.
//  You must retain, in the source form of any derivative works that you 
//  distribute, all copyright, patent, trademark, and attribution notices 
//  from the source form of this work.
//   
//
////////////////////////////////////////////////////////////////////////////
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO: reference additional headers your program requires here
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <xutility>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <windows.h>
#include <float.h>
#include <io.h>

#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

typedef unsigned char		ubyte;
typedef unsigned short		ushort;
typedef unsigned int		uint;
typedef unsigned long long	ullong;

enum ImageType { IMAGE_TYPE_RGB, IMAGE_TYPE_GRAY, IMAGE_TYPE_GRAY_PACKED };

#ifndef PI
#define PI 3.141592654f
#endif

#ifndef PIx2
#define PIx2 6.283185308f
#endif

#ifndef MAX_NUM_THREADS
#define MAX_NUM_THREADS 8
#endif

#ifndef MAX_LINE_LENGTH
#define MAX_LINE_LENGTH 512
#endif

#ifndef FACTOR_RAD_TO_DEG
#define FACTOR_RAD_TO_DEG 57.295779505601046646705075978956f
#endif

#ifndef FACTOR_DEG_TO_RAD
#define FACTOR_DEG_TO_RAD 0.01745329252222222222222222222222f
#endif

#ifndef EQUAL_TOLERANCE_ABS
#define EQUAL_TOLERANCE_ABS 0.001f
#endif

#ifndef EQUAL_TOLERANCE_REL
#define EQUAL_TOLERANCE_REL 0.01f
#endif

#ifndef EQUAL
#define EQUAL(a, b) (fabs((a) - (b)) <= EQUAL_TOLERANCE_ABS || (fabs((a) - (b)) / std::min(fabs((a)), fabs((b)))) <= EQUAL_TOLERANCE_REL)
#endif

#ifndef ABS_DIF
#define ABS_DIF(a, b) (((a) > (b)) ? ((a) - (b)) : ((b) - (a)))
#endif

#ifndef SWAP
#define SWAP(a, b, t) (t) = (a); (a) = (b); (b) = (t)
#endif

#ifndef CLAMP
#define CLAMP(x, mn, mx) (((x) < (mn)) ? (mn) : (((x) > (mx)) ? (mx) : (x)))
#endif

#define DEFINE_THREAD_DATA(Function, ...)		class Function##_DATA{ public:__VA_ARGS__ };
#define DEFINE_THREAD_PROC(Function, data, ...)	static DWORD Function##_PROC(Function##_DATA *data){ Function(__VA_ARGS__); return 0; }
#define BEGIN_THREAD(Function, nThreads)		std::vector<HANDLE> threads(nThreads); std::vector<Function##_DATA *> pDatas(nThreads)
#define RUN_THREAD(Function, iThread, ...)		Function##_DATA data = { __VA_ARGS__ }; pDatas[iThread] = new Function##_DATA(data);\
	threads[iThread] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) Function##_PROC, pDatas[iThread], 0, 0)
#define END_THREAD(nThreads)					WaitForMultipleObjects((DWORD) (nThreads), &(threads[0]), TRUE, INFINITE);\
	for(ubyte iThread = 0; iThread < nThreads; ++iThread) delete pDatas[iThread];\
	for(ubyte iThread = 0; iThread < nThreads; ++iThread) CloseHandle(threads[iThread])
