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

#ifndef _VECTOR_N_H_
#define _VECTOR_N_H_

#include "AlignedVector.h"

namespace LA
{

	template<uint N> class AlignedVectorNf : public AlignedVectorN<float, N>
	{
	};

	template<uint N> class AlignedVectorNd : public AlignedVectorN<double, N>
	{
	};

	//typedef AlignedVectorNf<12> AlignedVector12f;
	typedef AlignedVectorNf<16> AlignedVector16f;
	typedef AlignedVectorNd<12> AlignedVector12d;

	inline float Dot(const AlignedVectorNf<9> &A, const AlignedVectorNf<9> &B)
	{
		return A[8] * B[8] + SSE::Sum0123(_mm_add_ps(
			_mm_mul_ps(VALUE_TO_SSE(A[0]), VALUE_TO_SSE(B[0])), 
			_mm_mul_ps(VALUE_TO_SSE(A[4]), VALUE_TO_SSE(B[4]))
			));
	}

	typedef AlignedVector<float > AlignedVectorXf;
	typedef AlignedVector<double> AlignedVectorXd;
	typedef AlignedVector<long int> AlignedVectorXli;

	template<typename TYPE> inline TYPE NormLinf(const AlignedVector<TYPE> &V)
	{
		TYPE d, normLinf = -1;
		const uint N = V.Size();
		for(uint i = 0; i < N; ++i)
		{
			d = fabs(V[i]);
			normLinf = std::max(normLinf, d);
		}
		return normLinf;
	}

	inline void Distribution(const AlignedVectorXf &V, float &avg, float &std)
	{
		uint N = V.Size(), _N = N - (N & 3);
		__m128 sseSum = _mm_setzero_ps();
		const float *p = V.Data();
		for(uint i = 0; i < _N; i += 4, p += 4)
			sseSum = _mm_add_ps(PTR_TO_SSE(p), sseSum);
		for(uint i = _N; i < N; ++i, p++)
			sseSum.m128_f32[i - _N] += *p;
		avg = SSE::Sum0123(sseSum) / N;

		__m128 sseAvg = _mm_set1_ps(avg), sseSub;
		sseSum = _mm_setzero_ps();
		p = V.Data();
		for(uint i = 0; i < _N; i += 4, p += 4)
		{
			sseSub = _mm_sub_ps(PTR_TO_SSE(p), sseAvg);
			sseSum = _mm_add_ps(_mm_mul_ps(sseSub, sseSub), sseSum);
		}
		for(uint i = _N; i < N; ++i, p++)
			sseSum.m128_f32[i - _N] += (*p - avg) * (*p - avg);
		std = sqrt(SSE::Sum0123(sseSum) / N);
	}
}

#endif