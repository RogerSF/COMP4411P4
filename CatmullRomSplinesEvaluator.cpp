#include "CatmullRomSplinesEvaluator.h"
#include <assert.h>
#include <iostream>
using namespace std;

float CatmullRomSplinesEvaluator::tension = -1.0f;

void CatmullRomSplinesEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap) const
{
	if (tension < 0) {
		cout << "Please enter the tension: ";
		cin >> tension;
	}

	ptvEvaluatedCurvePts.clear();
	int controlPointCount = ptvCtrlPts.size();

	std::vector<Point> ptvCtrlPtsDirection;
	// calculate the direction if wrapping
	Point outOfRangePoint = ptvCtrlPts[0];
	outOfRangePoint.x += fAniLength;
	Point wrapDirectionForLastPoint = (outOfRangePoint - ptvCtrlPts[controlPointCount - 2]) / 3 * tension;

	Point directionForFirstPoint;
	Point directionForLastPoint;
	for (int i = 0; i < controlPointCount - 1; i++) {
		Point D0;
		Point D1;
		if (i == 0) {
			if (!bWrap) {
				D0 = (ptvCtrlPts[1] - ptvCtrlPts[0]) / 3;
			}
			else {
				Point outOfRangePoint = ptvCtrlPts[controlPointCount - 1];
				outOfRangePoint.x -= fAniLength;
				D0 = (ptvCtrlPts[1] - outOfRangePoint) / 3 * tension;
				directionForFirstPoint = D0;
			}
		}
		else {
			D0 = (ptvCtrlPts[i + 1] - ptvCtrlPts[i - 1]) / 3 * tension;
		}

		if (i == controlPointCount - 2) {
			if (!bWrap) {
				D1 = (ptvCtrlPts[i+1] - ptvCtrlPts[i]) / 3;
			}
			else {
				Point outOfRangePoint = ptvCtrlPts[0];
				outOfRangePoint.x += fAniLength;
				D1 = (outOfRangePoint - ptvCtrlPts[i]) / 3 * tension;
				directionForLastPoint = D1;
			}
		}
		else {
			D1 = (ptvCtrlPts[i + 2] - ptvCtrlPts[i]) / 3 * tension;
		}

		Point v1 = ptvCtrlPts[i] + D0;
		Point v2 = ptvCtrlPts[i + 1] - D1;
		
		
		if (v1.x >= ptvCtrlPts[i].x && v2.x >= v1.x && ptvCtrlPts[i + 1].x >= v2.x) {
			// cout << ptvCtrlPts[i] << v1 << v2 << ptvCtrlPts[i + 1] << endl;
			cout << "normal at " << i << endl;
			displayBezier(ptvCtrlPts[i], v1, v2, ptvCtrlPts[i + 1], 0, ptvEvaluatedCurvePts, fAniLength);
		} else if (ptvCtrlPts[i + 1].x > ptvCtrlPts[i].x) {
			if (v1.x > ptvCtrlPts[i + 1].x) {
				v1.x = ptvCtrlPts[i].x;
				cout << "change v1 at " << i << endl;
			}
			if (v2.x < ptvCtrlPts[i].x) {
				v2.x = ptvCtrlPts[i + 1].x;
				cout << "change v2 at " << i << endl;
			}
			if (v1.x > v2.x) {
				float temp = v1.x;
				v1.x = v2.x;
				v2.x = temp;
				cout << "change v1 and v2 at " << i << endl;
			}
			cout << ptvCtrlPts[i] << v1 << v2 << ptvCtrlPts[i + 1] << endl;
			displayBezier(ptvCtrlPts[i], v1, v2, ptvCtrlPts[i + 1], 0, ptvEvaluatedCurvePts, fAniLength);
		}
	}

	if (bWrap) {
		Point firstPoint = ptvCtrlPts[0];
		firstPoint.x += fAniLength;

		Point v1 = ptvCtrlPts[controlPointCount - 1] + directionForLastPoint;
		Point v2 = firstPoint - directionForFirstPoint;

		cout << "Wrap: " << ptvCtrlPts[controlPointCount - 1] << v1 << v2 << firstPoint << endl;
		displayBezier(ptvCtrlPts[controlPointCount - 1], v1, v2, firstPoint, 0, ptvEvaluatedCurvePts, fAniLength);
	}
	else {
		float x = 0.0;
		float y = ptvCtrlPts[0].y;
		ptvEvaluatedCurvePts.push_back(Point(x, y));

		x = fAniLength;
		y = ptvCtrlPts[controlPointCount - 1].y;
		ptvEvaluatedCurvePts.push_back(Point(x, y));
	}

	//if (bWrap && controlPointCount % 3 == 0) {
	//	displayBezier(ptvCtrlPts[numCurves * 3], ptvCtrlPts[numCurves * 3 + 1], ptvCtrlPts[numCurves * 3 + 2], ptvCtrlPts[0], 0, ptvEvaluatedCurvePts, fAniLength);
	//}
	//else {
	//	for (int i = numCurves * 3; i < controlPointCount; i++) {
	//		ptvEvaluatedCurvePts.push_back(ptvCtrlPts[i]);
	//	}

	//	float x = 0.0;
	//	float y1;
	//	int iCtrlPtCount = controlPointCount;
	//	if (bWrap) {
	//		// if wrapping is on, interpolate the y value at xmin and
	//		// xmax so that the slopes of the lines adjacent to the
	//		// wraparound are equal.

	//		if ((ptvCtrlPts[0].x + fAniLength) - ptvCtrlPts[iCtrlPtCount - 1].x > 0.0f) {
	//			y1 = (ptvCtrlPts[0].y * (fAniLength - ptvCtrlPts[iCtrlPtCount - 1].x) +
	//				ptvCtrlPts[iCtrlPtCount - 1].y * ptvCtrlPts[0].x) /
	//				(ptvCtrlPts[0].x + fAniLength - ptvCtrlPts[iCtrlPtCount - 1].x);
	//		}
	//		else
	//			y1 = ptvCtrlPts[0].y;
	//	}
	//	else {
	//		// if wrapping is off, make the first and last segments of
	//		// the curve horizontal.

	//		y1 = ptvCtrlPts[0].y;
	//	}

	//	ptvEvaluatedCurvePts.push_back(Point(x, y1));

	//	/// set the endpoint based on the wrap flag.
	//	float y2;
	//	x = fAniLength;
	//	if (bWrap)
	//		y2 = y1;
	//	else
	//		y2 = ptvCtrlPts[iCtrlPtCount - 1].y;

	//	ptvEvaluatedCurvePts.push_back(Point(x, y2));

	//}
}
