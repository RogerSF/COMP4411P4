#include <assert.h>
#include <iostream>
#include "LaneRiesenfeldSplinesEvaluator.h"
#include "modelerapp.h"
using namespace std;

int LaneRiesenfeldSplinesEvaluator::degree = 2;

void LaneRiesenfeldSplinesEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap) const
{
	LaneRiesenfeldSplinesEvaluator::degree = ModelerApplication::Instance()->GetDegree();

	ptvEvaluatedCurvePts.clear();



	vector<Point>* degreePoints = new vector<Point>[degree];

	// Init
	vector<Point> degree0 = vector<Point>();
	degree0.push_back(Point(0, ptvCtrlPts[0].y));
	for(Point p : ptvCtrlPts)
	{
		cout << p << endl;
		degree0.push_back(p);
		degree0.push_back(p);
	}
	degree0.push_back(Point(fAniLength, ptvCtrlPts[ptvCtrlPts.size() - 1].y));

	degreePoints[0] = degree0;

	for(int i = 1; i < degree; i++)
	{
		cout << degree << endl;
		vector<Point> curDegreePoints = vector<Point>();
		vector<Point> prevDegreePoints = degreePoints[i-1];
		for(int j = 0; j < prevDegreePoints.size() - 1; j++)
		{
			Point p = Point(0.5*(prevDegreePoints[j].x + prevDegreePoints[j + 1].x), 0.5*(prevDegreePoints[j].y + prevDegreePoints[j + 1].y));
			cout << p << endl;
			curDegreePoints.push_back(p);
		}
		cout << endl;
		degreePoints[i] = curDegreePoints;
	}
	ptvEvaluatedCurvePts = degreePoints[degree - 1];
	ptvEvaluatedCurvePts.push_back(Point(0, ptvCtrlPts[0].y));
	ptvEvaluatedCurvePts.push_back(Point(fAniLength, ptvCtrlPts[ptvCtrlPts.size() - 1].y));

	// delete[] degreePoints;
}
