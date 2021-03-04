#pragma once

#include <Core/Engine.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <Laboratoare\Tema3\Colors.h>

#define NUM_PLATFORMS_ROW 3
#define NUM_CONFIGURATINS 4

#define BLUE 1
#define GREEN 2

#define RED 3
#define YELLOW 4
#define ORANGE 5

#define GRAY 6
#define PINK 7

#define MIN_GREEN_PLATFORM_DIST 7
#define MIN_PINK_PLATFORM_DIST 10

class PlatformGenerator {
public:
	PlatformGenerator();
	~PlatformGenerator();

	void NextConfiguration(int oldConfigurationNumber, int &newConfigurationNumber, float &platformLength,
							float &platformDistance, int colors[NUM_PLATFORMS_ROW]);

	glm::vec3 GetColor(int colorId);

protected:
	// 1 = platforma buna (albastra/verde/roz)
	// 0 = fara platforma/platforma rea (rosie, galbena, oranj, gri)
	const int generalConfigurations[NUM_CONFIGURATINS][NUM_PLATFORMS_ROW] =	{ 1, 0, 0, // 0 
																			  0, 1, 0, // 1
																			  0, 0, 1, // 2
																			  1, 1, 1}; // 3

	int numGenerationsNoGreen;
	int numGenerationsNoPink;
};