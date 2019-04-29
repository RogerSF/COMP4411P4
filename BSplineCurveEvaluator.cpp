#include "BSplineCurveEvaluator.h"
#include <assert.h>
using namespace std;

void BSplineCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap) const
{
	ptvEvaluatedCurvePts.clear();

	if (!bWrap) {
		//repeat the first and last point (if any)
		std::vector<Point> ctrlPtswithRepetition = ptvCtrlPts;
		if (!ctrlPtswithRepetition.empty()) {
			ctrlPtswithRepetition.insert(ctrlPtswithRepetition.begin(), ctrlPtswithRepetition[0]);
			ctrlPtswithRepetition.push_back(ctrlPtswithRepetition.back());
		}

		int controlPointCount = ctrlPtswithRepetition.size();

		std::vector<Point>& ctrPt = ctrlPtswithRepetition; //shorthand
		//when not wrapping, make b-spline pass first and last control points (use the repeated version)
			std::vector<Point> V = converttoV(ctrPt[0], ctrPt[1], ctrPt[2], ctrPt[3]);
			displayBezier(ctrPt[0], V[1], V[2], V[3], 0, ptvEvaluatedCurvePts, fAniLength);
			V = converttoV(ctrPt[controlPointCount-4], ctrPt[controlPointCount-3], ctrPt[controlPointCount-2], ctrPt[controlPointCount-1]);
			displayBezier(V[0], V[1], V[2], ctrPt[controlPointCount-1], 0, ptvEvaluatedCurvePts, fAniLength);
			
			
			//major parts
			for (int i = 1; i < controlPointCount - 4; i++) {
				std::vector<Point> V = converttoV(ctrPt[i], ctrPt[i + 1], ctrPt[i + 2], ctrPt[i + 3]);
				displayBezier(V[0], V[1], V[2], V[3], 0, ptvEvaluatedCurvePts, fAniLength);

			}


			//final horizontal wrapping
			float y1 = ctrPt[0].y;
			float y2 = ctrPt[controlPointCount - 1].y;
			ptvEvaluatedCurvePts.push_back(Point(0, y1));
			ptvEvaluatedCurvePts.push_back(Point(fAniLength, y2));
	}
	else {		//Wrapping required. Use unrepeated version of CtrlPts Vector
		for (int i = 0; i < ptvCtrlPts.size() - 3; i++) {
			std::vector<Point> V = converttoV(ptvCtrlPts[i], ptvCtrlPts[i + 1], ptvCtrlPts[i + 2], ptvCtrlPts[i + 3]);
			displayBezier(V[0], V[1], V[2], V[3], 0, ptvEvaluatedCurvePts, fAniLength);
		}
		int n = ptvCtrlPts.size(); //number of unrepeated ctrl points

		Point p0Moved(ptvCtrlPts[0].x + fAniLength, ptvCtrlPts[0].y);
		Point p1Moved(ptvCtrlPts[1].x + fAniLength, ptvCtrlPts[1].y);
		Point p2Moved(ptvCtrlPts[2].x + fAniLength, ptvCtrlPts[2].y);
		std::vector<Point> V = converttoV(ptvCtrlPts[n - 3], ptvCtrlPts[n - 2], ptvCtrlPts[n - 1], p0Moved);

		displayBezier(V[0], V[1], V[2], V[3], 0, ptvEvaluatedCurvePts, fAniLength);

		V = converttoV(ptvCtrlPts[n - 2], ptvCtrlPts[n - 1], p0Moved, p1Moved);
		displayBezier(V[0], V[1], V[2], V[3], 0, ptvEvaluatedCurvePts, fAniLength);

		V = converttoV(ptvCtrlPts[n - 1], p0Moved, p1Moved, p2Moved);

		displayBezier(V[0], V[1], V[2], V[3], 0, ptvEvaluatedCurvePts, fAniLength);
	}

	

	
	
}

std::vector<Point> BSplineCurveEvaluator::converttoV(Point b0, Point b1, Point b2, Point b3) {
	Point v0((float(1) / float(6))*b0.x + (float(2) / float(3)) * b1.x + (float(1) / float(6))*b2.x,
		(float(1) / float(6))*b0.y + (float(2) / float(3)) * b1.y + (float(1) / float(6))*b2.y);
	Point v1((float(2) / float(3))*b1.x + (float(1) / float(3))*b2.x,
		(float(2) / float(3))*b1.y + (float(1) / float(3))*b2.y);
	Point v2((float(1) / float(3))*b1.x + (float(2) / float(3))*b2.x,
		(float(1) / float(3))*b1.y + (float(2) / float(3))*b2.y);
	Point v3((float(1) / float(6))*b1.x + (float(2) / float(3)) * b2.x + (float(1) / float(6))*b3.x,
		(float(1) / float(6))*b1.y + (float(2) / float(3)) * b2.y + (float(1) / float(6))*b3.y);
	std::vector<Point> vpoints;
	vpoints.push_back(v0);
	vpoints.push_back(v1);
	vpoints.push_back(v2);
	vpoints.push_back(v3);
	return vpoints;
}