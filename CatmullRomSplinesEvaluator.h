#ifndef INCLUDED_Catmull_Rom_SPLINES_EVALUATOR_H
#define INCLUDED_Catmull_Rom_SPLINES_EVALUATOR_H

#pragma warning(disable : 4786)  

#include "CurveEvaluator.h"

//using namespace std;

class CatmullRomSplinesEvaluator : public CurveEvaluator
{
public:
	void evaluateCurve(const std::vector<Point>& ptvCtrlPts,	//ptv = Points in a Vector
		std::vector<Point>& ptvEvaluatedCurvePts,
		const float& fAniLength,
		const bool& bWrap) const;

	static float tension;
};

#endif