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

#ifndef _SIMILARITY_TRANSFORMATION_SOLVER_H_
#define _SIMILARITY_TRANSFORMATION_SOLVER_H_

#include "SimilarityTransformation.h"
#include "RigidTransformationSolver.h"

class SimilarityTransformationSolver : public RigidTransformationSolver
{

public:

	bool Run(const ThreeMatches3D &data, SimilarityTransformation3D &S, AlignedVector<__m128> &work);
	bool Run(const FourMatches3D &data, SimilarityTransformation3D &S, AlignedVector<__m128> &work);
	bool Run(const AlignedVector<Point3D> &X1s, const AlignedVector<Point3D> &X2s, SimilarityTransformation3D &S, AlignedVector<__m128> &work);

protected:

	void ComputeMeanScaleAndCovariance(const ThreeMatches3D &data, AlignedVector<__m128> &work);
	void ComputeMeanScaleAndCovariance(const FourMatches3D &data, AlignedVector<__m128> &work);
	void ComputeMeanScaleAndCovariance(const AlignedVector<Point3D> &X1s, const AlignedVector<Point3D> &X2s, AlignedVector<__m128> &work);

protected:

	__m128 m_sss1;

};

#endif