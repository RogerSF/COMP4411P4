#ifndef INCLUDED_C2Interpolating_CURVE_EVALUATOR_H
#define INCLUDED_C2Interpolating_CURVE_EVALUATOR_H

#pragma warning(disable : 4786)  

#include "CurveEvaluator.h"

//using namespace std;

class C3InterpolatingCurveEvaluator : public CurveEvaluator
{
public:
	void evaluateCurve(const std::vector<Point>& ptvCtrlPts,	//ptv = Points in a Vector
		std::vector<Point>& ptvEvaluatedCurvePts,
		const float& fAniLength,
		const bool& bWrap) const;

};


#endif