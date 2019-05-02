 // SAMPLE_SOLUTION
#include "particle.h"

/***************
* Constructors
***************/

Particle::Particle(void)
	:t(0.0),
	numberOfParticles(0)
{
	// positionOfParticles = new float[numberOfParticles * 3];
}

Particle::Particle(float new_t, int new_numOfParticles)
	:t(new_t),
	numberOfParticles(new_numOfParticles)
{
	// positionOfParticles = new float[numberOfParticles * 3];
}

Particle::~Particle(void) {
	/*
	if (positionOfParticles != NULL) {
	delete[] positionOfParticles;
	}
	*/
}

void Particle::copyPosition(const vector<float>& state) {
	// positionOfParticles = new float[numberOfParticles * 3];
	for (int i = 0; i < numberOfParticles; i++) {
		  if (i >= 0 && ((i * 6 + 5) < state.size())) {
			float temp_x = state[i * 6];
			float temp_y = state[i * 6 + 1];
			float temp_z = state[i * 6 + 2];
			positionOfParticles.push_back(temp_x);
			positionOfParticles.push_back(temp_y);
			positionOfParticles.push_back(temp_z);
		}
		// cout << "save particle at position of " << state[i * 6] << " " << state[i * 6 + 1] << " " << state[i * 6 + 2] << endl;
	}
}

Vec3f Particle::getPositionOf(const int& index, bool& flag) {
	if (index >= 0 && ((index * 3 + 2) < positionOfParticles.size())) {
		flag = true;
		return Vec3f(positionOfParticles[index * 3], positionOfParticles[index * 3 + 1], positionOfParticles[index * 3 + 2]);
	}
	return Vec3f(0.0, 0.0, 0.0);
}
