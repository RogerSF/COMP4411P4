#ifndef INCLUDED_BSpline_CURVE_EVALUATOR_H
#define INCLUDED_BSpline_CURVE_EVALUATOR_H

#pragma warning(disable : 4786)  

#include "CurveEvaluator.h"

//using namespace std;

class BSplineCurveEvaluator : public CurveEvaluator
{
public:
	void evaluateCurve(const std::vector<Point>& ptvCtrlPts,	//ptv = Points in a Vector
		std::vector<Point>& ptvEvaluatedCurvePts,
		const float& fAniLength,
		const bool& bWrap) const;

	static std::vector<Point> converttoV(Point b0, Point b1, Point b2, Point b3);

};

#endif