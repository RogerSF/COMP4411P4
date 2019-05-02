#pragma warning(disable : 4786)

#include "particleSystem.h"

// Colors
#define COLOR_RED			1.0f, 0.0f, 0.0f
#define COLOR_GREEN		0.0f, 1.0f, 0.0f
#define COLOR_BLUE			0.0f, 0.0f, 1.0f
#define COLOR_WHITE		1.0f, 1.0f, 0.0f
#define COLOR_BLACK		0.0f, 0.0f, 0.0f

// #include <stdio.h>
// #include <stdlib.h>
// #include <assert.h>
// #include <math.h>
// #include <limits.h>


/***************
 * Constructors
 ***************/

ParticleSystem::ParticleSystem()
{
	// TODO
	numberOfParticle = 0;
	maxNumberOfParticle = 100;
	bake_fps = 30;
	bake_start_time = -1;
	bake_end_time = -1;
	// state = new float[maxNumberOfParticle];
	// cout << "set simulation to false 1" << endl;
	simulate = false;
	dirty = false;
	directionOfWindIsLeft = true;
}

ParticleSystem::ParticleSystem(int fps, int max) {
	numberOfParticle = 0;
	maxNumberOfParticle = max;
	bake_fps = fps;
	bake_start_time = -1;
	bake_end_time = -1;
	// state = new float[maxNumberOfParticle];
	// cout << "set simulation to false 2" << endl;
	// cout << maxNumberOfParticle << endl;
	simulate = false;
	dirty = false;
	directionOfWindIsLeft = true;
}

/*************
 * Destructor
 *************/

ParticleSystem::~ParticleSystem() 
{
	// TODO
	clearBaked();
}

void ParticleSystem::setTranslationMatrix(const Mat4d& transMatrix) {
	// cout << transMatrix[0] << "," << transMatrix[1] << "," << transMatrix[2] << "," << transMatrix[3] << endl;
	translationMatrix = transMatrix;
	/*
	cout << "translation matrix is set" << endl;
	Vec4d testOrigin(0.0, 0.0, 0.0,1.0);
	Vec4d testResult = translationMatrix*testOrigin;
	cout << "move local origin to " << testResult[0] << "," << testResult[1] << "," << testResult[2] << endl;
	*/
}

Vec4d ParticleSystem::getWorldCoordAt(float local_x, float local_y, float local_z) {
	Vec4d localCoord((double)local_x, (double)local_y, (double)local_z, 1.0);
	// Vec4d worldCoord = translationMatrix*localCoord;
	 // cout << "world coord is " << worldCoord[0] << " " << worldCoord[1] << " " << worldCoord[2] << endl;
	return translationMatrix * localCoord;
}

void ParticleSystem::drawParticleAt(float world_x, float world_y, float world_z) {
	cout << "Drawing 1 box at: " << world_x << " " << world_y << " " << world_z <<endl;
	glPushMatrix();
	glTranslated(world_x, world_y, world_z);
	setAmbientColor(.1f, .1f, .1f);
	setDiffuseColor(COLOR_WHITE);
	// drawBox(0.1, 0.1, 0.1);
	drawBox(0.2, 0.2, 0.2);
	// drawSphere(0.2);
	glPopMatrix();
}

/******************
 * Simulation fxns
 ******************/

/** Start the simulation */
void ParticleSystem::startSimulation(float t)
{
    
	// TODO
	clearBaked();
	// These values are used by the UI ...
	// -ve bake_end_time indicates that simulation
	// is still progressing, and allows the
	// indicator window above the time slider
	// to correctly show the "baked" region
	// in grey.
	bake_start_time = t;
	bake_end_time = -1;
	simulate = true;
	dirty = true;

}

/** Stop the simulation */
void ParticleSystem::stopSimulation(float t)
{
    
	// TODO
	numberOfParticle = 0;
	// These values are used by the UI
	bake_end_time = t;
	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Reset the simulation */
void ParticleSystem::resetSimulation(float t)
{
    
	// TODO
	cout << "reset simu" << endl;
	bake_start_time = -1;
	bake_end_time = -1;

	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Compute forces and update particles **/
void ParticleSystem::computeForcesAndUpdateParticles(float t)
{

	// TODO
	int numberOfFrame = roundf((t - bake_start_time)*bake_fps);
	// compute force
	if (numberOfFrame % 30 == 0) {
		if (directionOfWindIsLeft == true) {
			forceByWind = Vec3f(1.0, 3.0, 0.0);
		} else {
			forceByWind = Vec3f(-1.5, 2.0, 0.0);
		}
		directionOfWindIsLeft = !directionOfWindIsLeft;
	}


	// update particles
	if (simulate) {
		// calculate delta t by fps
		float deltaT = 1.0f / bake_fps;

		// add size of state for a new particle every 5 frame
		bool ifNewParticle = false;
		if (numberOfFrame % 5 == 0) {
			numberOfParticle += 1;
			ifNewParticle = true;
		}

		if (numberOfParticle > maxNumberOfParticle) {
			numberOfParticle = maxNumberOfParticle;
		}

		// copy the old mass to another vector
		vector<float> oldMass;
		oldMass.reserve(mass.size());
		copy(mass.begin(), mass.end(), back_inserter(oldMass));
		mass.clear();
		// copy the old state to another vector
		vector<float> oldState;
		oldState.reserve(state.size());
		copy(state.begin(), state.end(), back_inserter(oldState));
		state.clear();

		// update new state
		for (int i = 0; i < numberOfParticle; i++) {
			// add a new particle every 5 frame in the front
			if (ifNewParticle && i == 0) {
				// initial mass (random mass between 0.3 to 0.4)
				float r = rand() % 2000 + 3000;
				float m = r / 10000.0f;
				// cout << "mass of new particle is " << m << endl;
				mass.push_back(m);
				// initial postition
				Vec4d worldCoordOfPosition;
				switch ((numberOfFrame / 5) % 5) {
				case 0:
					worldCoordOfPosition = getWorldCoordAt(0.0, 0.0, 0.0);
					break;
				case 1:
					worldCoordOfPosition = getWorldCoordAt(0.3, 0.0, 0.3);
					break;
				case 2:
					worldCoordOfPosition = getWorldCoordAt(0.3, 0.0, -0.3);
					break;
				case 3:
					worldCoordOfPosition = getWorldCoordAt(-0.3, 0.0, -0.3);
					break;
				case 4:
					worldCoordOfPosition = getWorldCoordAt(-0.5, 0.0, 0.5);
					break;
				}
				// cout << "world coord of postion is " << worldCoordOfPosition[0] << " " << worldCoordOfPosition[1] << " " << worldCoordOfPosition[2] << endl;
				state.push_back(worldCoordOfPosition[0]);
				state.push_back(worldCoordOfPosition[1]);
				state.push_back(worldCoordOfPosition[2]);
				// initial speed
				state.push_back(0.0);
				state.push_back(0.0);
				state.push_back(3.0);
			}
			else {
				// update the old particles
				int indexOfOld = i;
				if (ifNewParticle) {
					indexOfOld -= 1;
				}
				// update mass
				mass.push_back(oldMass[indexOfOld]);
				// update position
				state.push_back(oldState[indexOfOld * 6] + deltaT * oldState[indexOfOld * 6 + 3]);
				state.push_back(oldState[indexOfOld * 6 + 1] + deltaT * oldState[indexOfOld * 6 + 4]);
				state.push_back(oldState[indexOfOld * 6 + 2] + deltaT * oldState[indexOfOld * 6 + 5]);
				// update speed
				Vec3f oldForce = force[indexOfOld];
				state.push_back(oldState[indexOfOld * 6 + 3] + (deltaT * oldForce[0] / oldMass[indexOfOld]));
				state.push_back(oldState[indexOfOld * 6 + 4] + (deltaT * oldForce[1] / oldMass[indexOfOld]));
				state.push_back(oldState[indexOfOld * 6 + 5] + (deltaT * oldForce[2] / oldMass[indexOfOld]));
			}
		}

		// update force accumulator
		force.clear();
		for (int i = 0; i < numberOfParticle; i++) {
			// calculate current force
			Vec3f gravity = mass[i] * Vec3f(0.0, -9.8, 0.0); // g = 9.8
			Vec3f totalForce = gravity + forceByWind;
			force.push_back(totalForce);
		}

		bakeParticles(t);

		// collision check
		for (int i = 0; i < numberOfParticle; i++) {
			for (int j = i + 1; j < numberOfParticle; j++) {
				if (abs(state[i * 6] - state[j * 6]) < 0.1) {
					if (abs(state[i * 6 + 1] - state[j * 6 + 1]) < 0.1) {
						if (abs(state[i * 6 + 2] - state[j * 6 + 2]) < 2.0) {
							// they are in the same position
							float m1 = mass[i];
							float m2 = mass[j];
							float v1x = state[i * 6 + 3];
							float v1y = state[i * 6 + 4];
							float v1z = state[i * 6 + 5];
							float v2x = state[j * 6 + 3];
							float v2y = state[j * 6 + 4];
							float v2z = state[j * 6 + 5];
							// update speed
							state[i * 6 + 3] = (v1x*(m1 - m2) + 2 * m2*v2x) / (m1 + m2);
							state[i * 6 + 4] = (v1y*(m1 - m2) + 2 * m2*v2y) / (m1 + m2);
							state[i * 6 + 5] = (v1z*(m1 - m2) + 2 * m2*v2z) / (m1 + m2);
							state[j * 6 + 3] = (v2x*(m2 - m1) + 2 * m1*v1x) / (m1 + m2);
							state[j * 6 + 4] = (v2y*(m2 - m1) + 2 * m1*v1y) / (m1 + m2);
							state[j * 6 + 5] = (v2z*(m2 - m1) + 2 * m1*v1z) / (m1 + m2);
						}
					}
				}
			}
		}


	}
}

Particle* ParticleSystem::getParticlesAt(float t) {
	if (t >= bake_start_time && t <= bake_end_time) {
		int numberOfFrame = roundf((t - bake_start_time)*bake_fps);
		int shots_size = shots_of_particles.size();
		if (numberOfFrame > 0 && numberOfFrame < shots_size) {
			return shots_of_particles[numberOfFrame];
		}
	}
	return NULL;
}


/** Render particles */
void ParticleSystem::drawParticles(float t)
{
	// TODO
	if (isSimulate()) {
		// draw particles in state
		// cout << "simulate " << t << " number of particles: " << numberOfParticle << endl;
		for (int i = 0; i < numberOfParticle; i++) {
			drawParticleAt(state[i * 6], state[i * 6 + 1], state[i * 6 + 2]);
			// cout << state[i * 6] << " " << state[i * 6 + 1] << " " << state[i * 6 + 2] << " at speed of" << state[i * 6 + 3] << " " << state[i * 6 + 4] << " " << state[i * 6 + 5] << endl;
		}

	}
	else {
		// get particles from baked_particles at t
		Particle* currentParticles = getParticlesAt(t);
		if (currentParticles != NULL) {
			int numberOfBakedParticles = currentParticles->numberOfParticles;
			cout << "baked " << t << " number of particles: " << numberOfBakedParticles << endl;
			for (int i = 0; i < numberOfBakedParticles; i++) {
				bool flag = false;
				Vec3f position = currentParticles->getPositionOf(i,flag);
				if (flag == true) {
					drawParticleAt(position[0], position[1], position[2]);
				}
				// drawParticleAt(currentParticles->getXPositionOf(i * 3), currentParticles->getYPositionOf(i * 3 + 1), currentParticles->getZPositionOf(i * 3 + 2));
				// cout << currentParticles->getXPositionOf(0) << endl;
			}
		}
	}
}

/** Adds the current configuration of particles to
  * your data structure for storing baked particles **/
void ParticleSystem::bakeParticles(float t) 
{
	// TODO
	Particle* newShotOfParticles = new Particle(t, numberOfParticle);
	newShotOfParticles->copyPosition(state);
	shots_of_particles.push_back(newShotOfParticles);
}

/** Clears out your data structure of baked particles */
void ParticleSystem::clearBaked()
{

	// TODO
	int sizeOfShots = shots_of_particles.size();
	for (int i = 0; i < sizeOfShots; i++)
	{
		if (shots_of_particles[i] != NULL) {
			delete shots_of_particles[i];
		}
	}
	shots_of_particles.clear();
}





