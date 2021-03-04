#pragma once

#include <Core/Engine.h>
#include <Laboratoare\Tema3\Colors.h>

#define LENGTH_LIMIT_OBSTACLE 1.5

class PlatformInfo {
public:
	PlatformInfo(glm::mat4 modelMatrixTranslate);
	PlatformInfo(glm::mat4 modelMatrixTranslate, glm::vec3 color, float length);
	~PlatformInfo();

	glm::mat4 GetModelMatrixTranslate();
	glm::vec3 GetColor();

	void SetColor(glm::vec3 color);

	bool HasObstacle();
	bool ObstacleWasHit();
	void SetObstacleHit();

	bool HasPineCone();
	void SetHasPineCone(bool value);

protected:
	glm::mat4 modelMatrixTranslate;
	glm::vec3 color;
	bool hasObstacle;
	bool obstacleWasHit;
	bool hasPineCone;
};