#include "BezierCurveEvaluator.h"
#include <assert.h>
using namespace std;

void BezierCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap) const
{
	ptvEvaluatedCurvePts.clear();
	int controlPointCount = ptvCtrlPts.size();
	int numCurves = (controlPointCount - 1) / 3;
	for (int i = 0; i < numCurves; i++) {
		int preCount = ptvEvaluatedCurvePts.size();
		displayBezier(ptvCtrlPts[i * 3], ptvCtrlPts[i * 3 + 1], ptvCtrlPts[i * 3 + 2], ptvCtrlPts[i * 3 + 3], 0, ptvEvaluatedCurvePts,fAniLength);
		int postCount = ptvEvaluatedCurvePts.size();
		cout << postCount - preCount << " points are in this Bezier Curve : 1 Whistle" << endl;
	}

	if (bWrap && controlPointCount % 3 == 0) {
		displayBezier(ptvCtrlPts[numCurves * 3], ptvCtrlPts[numCurves * 3 + 1], ptvCtrlPts[numCurves * 3 + 2], ptvCtrlPts[0],0, ptvEvaluatedCurvePts, fAniLength);
	}
	else {
		for (int i = numCurves * 3; i < controlPointCount; i++) {
			ptvEvaluatedCurvePts.push_back(ptvCtrlPts[i]);
		}


		float x = 0.0;
		float y1;
		int iCtrlPtCount = controlPointCount;
		if (bWrap) {
			// if wrapping is on, interpolate the y value at xmin and
			// xmax so that the slopes of the lines adjacent to the
			// wraparound are equal.

			if ((ptvCtrlPts[0].x + fAniLength) - ptvCtrlPts[iCtrlPtCount - 1].x > 0.0f) {
				y1 = (ptvCtrlPts[0].y * (fAniLength - ptvCtrlPts[iCtrlPtCount - 1].x) +
					ptvCtrlPts[iCtrlPtCount - 1].y * ptvCtrlPts[0].x) /
					(ptvCtrlPts[0].x + fAniLength - ptvCtrlPts[iCtrlPtCount - 1].x);
			}
			else
				y1 = ptvCtrlPts[0].y;
		}
		else {
			// if wrapping is off, make the first and last segments of
			// the curve horizontal.

			y1 = ptvCtrlPts[0].y;
		}

		ptvEvaluatedCurvePts.push_back(Point(x, y1));

		/// set the endpoint based on the wrap flag.
		float y2;
		x = fAniLength;
		if (bWrap)
			y2 = y1;
		else
			y2 = ptvCtrlPts[iCtrlPtCount - 1].y;

		ptvEvaluatedCurvePts.push_back(Point(x, y2));

	}
}
