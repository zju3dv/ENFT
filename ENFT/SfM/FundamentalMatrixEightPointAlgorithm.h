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

#ifndef _EIGHT_POINT_ALGORITHM_H_
#define _EIGHT_POINT_ALGORITHM_H_

#include "FundamentalMatrix.h"
#include "Match.h"
#include "Matrix8.h"
#include "MatrixMxN.h"

class FundamentalMatrixEightPointAlgorithm
{

public:

	FundamentalMatrixEightPointAlgorithm() : m_4onesNeg(_mm_set1_ps(-1)) {}
	bool Run(const EightMatches2D &data, FundamentalMatrix &F, AlignedVector<__m128> &work);
	bool Run(const MatchSet2D &data, FundamentalMatrix &F, AlignedVector<__m128> &work);

//private:
//
//	bool EnforceSingularConstraint(FundamentalMatrix &F);

private:

	LA::AlignedMatrix8f		m_AT8;
	LA::AlignedMatrix8xXf	m_AT8xX;
	LA::AlignedVectorXf		m_b;
	const __m128			m_4onesNeg;

};

#endif