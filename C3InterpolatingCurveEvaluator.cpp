#include "C3InterpolatingCurveEvaluator.h"
#include <assert.h>
#include "Eigen/Dense"
//Citation: Linear Algebra Solver is from http://eigen.tuxfamily.org/index.php?title=Main_Page
using Eigen::MatrixXf;
using Eigen::VectorXf;
using namespace std;
#include <algorithm>

void C3InterpolatingCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap) const
{
	ptvEvaluatedCurvePts.clear();

	const int numPoints = ptvCtrlPts.size();

	//make a copy, then delete any points that are too close
	std::vector<Point> controlPoints = ptvCtrlPts;
	/*for (int i = 0; i < ptvCtrlPts.size(); i++) {
		controlPoints.push_back(ptvCtrlPts[i]);
	}
	for (std::vector<Point>::iterator itr = controlPoints.begin(); itr != controlPoints.end();) {
		if (abs((*itr).x - (*(itr + 1)).x) <= 0.1)
			controlPoints.erase(itr + 1);
		if (itr != controlPoints.end())
			itr++;
	}*/
	 
	if (controlPoints.size() >= 4) {
		MatrixXf c2matrix(numPoints, numPoints);
		c2matrix = MatrixXf::Zero(numPoints,numPoints);

		//natural C2 constraint: 2nd derivative is 0 at endpoints
		c2matrix(0, 0) = 2;
		c2matrix(0, 1) = 1;
		c2matrix(numPoints - 1, numPoints - 1) = 2;
		c2matrix(numPoints - 1, numPoints - 2) = 1;

		//other points
		for (int i = 1; i <= numPoints - 2; i++) {
			c2matrix(i, i - 1) = 1;
			c2matrix(i, i) = 4;
			c2matrix(i, i + 1) = 1;
		}

		//solve x and y coordinates respectively
		const int m = numPoints - 1;	//shorthand
		VectorXf xrhs = VectorXf::Zero(m+1);
		xrhs(0) = 3 * (controlPoints[1].x - controlPoints[0].x);
		xrhs(numPoints - 1) = 3 * (controlPoints[m].x - controlPoints[m-1].x);
		for (int i = 1; i <= m - 1; i++) {
			xrhs(i) = 3 * (controlPoints[i + 1].x - controlPoints[i - 1].x);
		}
		VectorXf yrhs = VectorXf::Zero(m + 1);
		yrhs(0) = 3 * (controlPoints[1].y - controlPoints[0].y);
		yrhs(numPoints - 1) = 3 * (controlPoints[m].y - controlPoints[m - 1].y);
		for (int i = 1; i <= m - 1; i++) {
			yrhs(i) = 3 * (controlPoints[i + 1].y - controlPoints[i - 1].y);
		}

		VectorXf d_xcoord = c2matrix.fullPivHouseholderQr().solve(xrhs);
		VectorXf d_ycoord = c2matrix.fullPivHouseholderQr().solve(yrhs);
		
		//cout << c2matrix << endl;
		for (int i = 0; i < m; i++) {
			Point v0 = controlPoints[i];
			Point v1 (d_xcoord[i] / float(3) + v0.x, d_ycoord[i] / float(3) + v0.y);
			Point v3 = controlPoints[i + 1];
			Point v2(v3.x - d_xcoord[i + 1] / float(3), v3.y - d_ycoord[i + 1] / float(3));
			cout << v0 << " " << v1 << " " << v2 << " " << v3 << endl;
			displayBezier(v0, v1, v2, v3, 0, ptvEvaluatedCurvePts, fAniLength);
		}
	}


	float y1 = controlPoints[0].y;
	float y2 = controlPoints[ptvCtrlPts.size() - 1].y;
	ptvEvaluatedCurvePts.push_back(Point(0, y1));
	ptvEvaluatedCurvePts.push_back(Point(fAniLength, y2));

}
