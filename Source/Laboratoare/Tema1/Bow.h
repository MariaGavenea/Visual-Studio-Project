#pragma once

#include <Core/Engine.h>
#include <Laboratoare\Tema1\Arrow.h>
#define PI 3.1415926535897932384626433832795
#define HALF_PI PI / 2
#define QUARTER_PI PI / 4
#define NUM_ITER 50
#define SQRT_2 1.41421356237
#define BOW_SPEED 100

class Bow {
public:
	Bow(glm::vec3 middlePoint, float length, glm::vec3 color);
	~Bow();

	Mesh* GetBowstringMesh();
	Mesh* GetBowstickMesh();
	Arrow* GetArrow();
	glm::vec3 GetStringMiddlePoint();
	float GetStringMiddlePointX();
	float GetStringMiddlePointY();
	float GetStringLength();
	float GetTranslateY();
	float GetRotateAngle();
	glm::mat3 GetModelMatrix();

	glm::vec3 GetCenterPoint();
	float GetBigRadius();

	void SetModelMatrix(glm::mat3 matrix);
	void IncTranslateY(float step, float windowHeight);
	void SetRotateAngle(float angle);
private:
	void createBowstring();
	void createBowstick();

protected:
	Mesh* bowstring;
	Mesh* bowstick;
	Arrow* arrow;

	glm::vec3 bowstringMiddlePoint;
	float bowstringLength;
	glm::vec3 bowColor;

	float translateY;
	float rotateAngle;

	float bigRadius;
	glm::vec3 centerPoint;

	glm::mat3 modelMatrix;
};