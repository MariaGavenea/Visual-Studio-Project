#pragma once

#include <Core/Engine.h>
#include <Laboratoare\Tema3\Colors.h>

#define NUM_ITER 50
#define PI 3.1415926535897932384626433832795
#define HEART_SCALE 1.5

class Life {
public:
	Life(glm::vec3 centerPoint);
	~Life();

	Mesh* GetHeartMesh();
	glm::vec3 GetLifeColor();
	glm::vec3 GetCenterPoint();
	float GetWidth(float scale);

private:
	void CreateHeart();
	float GetY(float angle);
	float GetX(float angle);

protected:
	Mesh* heart;
	const glm::vec3 heartColor = red;
	glm::vec3 centerPoint;

	const float initialWidth = 32;
};