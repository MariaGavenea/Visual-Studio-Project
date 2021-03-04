#pragma once

#include <Core/Engine.h>

#define PI 3.1415926535897932384626433832795
#define HALF_PI PI / 2
#define QUARTER_PI PI / 4
#define NUM_ITER 50
#define BALLOON_SPEED 100
#define BALLOON_OX_SCALE 0.75
#define SCALE_OX_FACTOR 1
#define SCALE_OY_FACTOR 2

class Balloon {
public:
	Balloon(glm::vec3 centerPoint, float radius, glm::vec3 color);
	~Balloon();

	Mesh* GetBalloon();
	Mesh* GetRope();
	float GetTranslateY();
	float GetScaleX();
	float GetScaleY();
	glm::vec3 GetCenterPoint();
	float GetRadius();
	float GetCenterPointX();
	float GetCenterPointY();
	glm::vec3 GetColor();
	bool WasHit();
	void SetHit(bool val);

	void IncBalloonTranslateY(float step);
	void DecScaleX(float step);
	void DecScaleY(float step);
	void ResetTranslateOutOfWindow(float absCenterY, float height);
	void ResetHit();
private:
	void createBalloon();
	void createRope();

protected:
	Mesh* balloon;
	Mesh* rope;

	glm::vec3 balloonCenterPoint;
	float balloonRadius;
	glm::vec3 balloonColor;

	float translateY;
	float scaleX;
	float scaleY;
	bool hit;
};