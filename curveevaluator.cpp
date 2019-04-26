#include "CurveEvaluator.h"

float CurveEvaluator::s_fFlatnessEpsilon = 0.00001f;
float CurveEvaluator::flatThreshhold = 0.0001f;
int CurveEvaluator::s_iSegCount = 16;

CurveEvaluator::~CurveEvaluator(void)
{
}

//the points passed into this function should only be in the range of 0~fAniLength. expanding will be taken care of.
void CurveEvaluator::displayBezier(Point v0, Point v1, Point v2, Point v3, int depth, std::vector<Point>& evaluatePts, const float& fAniLength)
{
	if (v0.x > v3.x) {
		v3.x += fAniLength;
	}/*
	if (v2.x <= v0.x)
		v2.x += fAniLength;
	if (v1.x <= v0.x)
		v1.x += fAniLength;*/

	if (
		 (v0.distance(v1) + v1.distance(v2) + v2.distance(v3)) / (v0.distance(v3)) < 1 + flatThreshhold
	|| depth==16) {
		if (v3.x > fAniLength && v0.x < fAniLength && v1.x<=fAniLength && v2.x<=fAniLength) {
			float y = (fAniLength - v0.x) * (v3.y - v0.y) / (v3.x - v0.x) + v0.y;
			//cout << "output edge when Bezier: "<< v3.x << " " << v0.x << endl;
			evaluatePts.push_back(Point(0.0, y));
			evaluatePts.push_back(Point(fAniLength, y));
		}
		while (v0.x > fAniLength) {
			v0.x -= fAniLength;
		}
		evaluatePts.push_back(v0);
		while (v3.x > fAniLength) {
			v3.x -= fAniLength;
		}
		evaluatePts.push_back(v3);
		return;
	}

	Point v0p((v0.x + v1.x) / 2, (v0.y + v1.y) / 2);
	Point v1p((v1.x + v2.x) / 2, (v1.y + v2.y) / 2);
	Point v2p ((v2.x + v3.x) / 2, (v2.y + v3.y) / 2);

	Point v0pp((v0p.x + v1p.x) / 2, (v0p.y + v1p.y) / 2);
	Point v1pp((v1p.x + v2p.x) / 2, (v1p.y + v2p.y) / 2);

	Point qu((v0pp.x + v1pp.x) / 2, (v0pp.y + v1pp.y) / 2);


	displayBezier(v0, v0p, v0pp, qu, depth + 1, evaluatePts,fAniLength);
	displayBezier(qu, v1pp, v2p, v3, depth + 1, evaluatePts,fAniLength);
}

bool CurveEvaluator::flatEnough(Point v0, Point v1, Point v2, Point v3)
{
	return (v0.distance(v1) + v1.distance(v2) + v2.distance(v3)) / (v0.distance(v3)) < 1 + flatThreshhold;
}

void CurveEvaluator::setFlatThresh(float th) {
	flatThreshhold = th;
}