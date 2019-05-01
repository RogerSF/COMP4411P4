#ifndef INCLUDED_Lane_Riesenfeld_SPLINES_EVALUATOR_H
#define INCLUDED_Lane_Riesenfeld_SPLINES_EVALUATOR_H

#pragma warning(disable : 4786)  

#include "CurveEvaluator.h"

//using namespace std;

class LaneRiesenfeldSplinesEvaluator : public CurveEvaluator
{
public:
	void evaluateCurve(const std::vector<Point>& ptvCtrlPts,	//ptv = Points in a Vector
		std::vector<Point>& ptvEvaluatedCurvePts,
		const float& fAniLength,
		const bool& bWrap) const;

	static int degree;
};

#endif