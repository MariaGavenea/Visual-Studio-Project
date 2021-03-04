#pragma once

#include <Core/Engine.h>
#define SHURIKEN_SPEED 200
#define SHURIKEN_ROTATION_SPEED 3
#define SQRT_2 1.41421356237
#define SCALE_OX_HIT_FACTOR 1
#define SCALE_OY_HIT_FACTOR 1

class Shuriken {
public:
	Shuriken(glm::vec3 centerPoint, float radius, glm::vec3 color);
	~Shuriken();

	Mesh* GetShuriken();

	glm::vec3 GetShurikenCenterPoint();
	float GetShurikenCenterPointX();
	float GetShurikenCenterPointY();
	float GetTranslateX();
	float GetScaleX();
	float GetScaleY();
	float GetRotateAngle();
	float GetBigRadius();
	bool WasHit();

	void DecTranslateX(float step);
	void DecScaleX(float step);
	void DecScaleY(float step);
	void IncRotationAngle(float step);
	void SetHit(bool val);
	void resetTranslate(float offset);
	void ResetTranslateOutOfWindow(float absCenterX);
	void ResetHit();

private:
	void createShuriken();

protected:
	Mesh* shuriken;

	glm::vec3 shurikenCenterPoint;
	float shurikenLittleRadius;
	glm::vec3 shurikenColor;

	float translateX;
	float scaleX;
	float scaleY;
	float rotateAngle;

	bool hit;
};