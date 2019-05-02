 // SAMPLE_SOLUTION
#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "vec.h"

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <limits.h>

class Particle
{

public:
	Particle(void);
	Particle(float new_t, int new_numOfParticles);

	~Particle(void);

	void copyPosition(const vector<float>& state);
	// float getXPositionOf(int index);
	// float getYPositionOf(int index);
	// float getZPositionOf(int index);
	Vec3f getPositionOf(const int& index, bool& flag);

public:
	float t;
	int numberOfParticles;
	std::vector<float> positionOfParticles;
};

#endif	// __PARTICLE_H__