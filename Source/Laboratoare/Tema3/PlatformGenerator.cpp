#include "PlatformGenerator.h"

PlatformGenerator::PlatformGenerator()
{
	numGenerationsNoGreen = numGenerationsNoPink = 0;
	srand(time(NULL));
}

PlatformGenerator::~PlatformGenerator()
{
}

void PlatformGenerator::NextConfiguration(int oldConfigurationNumber, int& newConfigurationNumber,
	float& platformLength, float& platformDistance, int colors[NUM_PLATFORMS_ROW])
{
	numGenerationsNoGreen++;
	numGenerationsNoPink++;

	switch (oldConfigurationNumber) {
		case 0:	newConfigurationNumber = rand() % (NUM_CONFIGURATINS - 2); // orice configuratie mai putin ultimele 2
				break;

		case 1: newConfigurationNumber = rand() % (NUM_CONFIGURATINS - 1); // orice configuratie mai putin ultima
				break;

		case 2: newConfigurationNumber = rand() % (NUM_CONFIGURATINS - 2) + 1; // orice configuratie mai putin prima si ultima
				break;

		case 3: newConfigurationNumber = rand() % (NUM_CONFIGURATINS - 1); // orice configuratie mai putin ultima
				break;

		default: newConfigurationNumber = 0;
				 break;
		}

	platformDistance = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); // [0, 1]
	platformLength = platformDistance + 1; // [1, 2]
	//platformDistance = 1;
	// platformLength = 2;
	
	if (platformDistance < 0.5) {
		// pentru a evita cazul cand distanta dintre platforme nu se vede la timp pentru saritura
		platformDistance = 0;
	}

	for (int i = 0; i < NUM_PLATFORMS_ROW; i++) {
		if (generalConfigurations[newConfigurationNumber][i] == 1) {
			// platforma buna
			if (numGenerationsNoGreen < MIN_GREEN_PLATFORM_DIST) {
				if (numGenerationsNoPink >= MIN_PINK_PLATFORM_DIST) {
					colors[i] = PINK;
					numGenerationsNoPink = 0;
				}
				else {
					colors[i] = BLUE;
				}
			}
			else {
				colors[i] = GREEN;
				numGenerationsNoGreen = 0;
			}
		}
		else {
			// platforma rea/fara platforma

			colors[i] = rand() % 3 + 4; // galben, oranj, gri
			if (rand() % 3 == 0) {
				if (rand() % 3 == 1) {
					colors[i] = RED; // probabilitate mai mica pentru rosu
				}
				else {
					colors[i] = 0; // nu exista platforma
				}
			}
		}
	}
}

glm::vec3 PlatformGenerator::GetColor(int colorId)
{
	switch (colorId) {
		case BLUE: return blue;

		case GREEN: return green;

		case RED: return red;

		case YELLOW: return yellow;

		case ORANGE: return orange;

		case GRAY: return gray;

		case PINK: return pink;

		default: return glm::vec3(0, 0, 0); // nu exista platforma
	}
}
