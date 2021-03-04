#include "PlatformInfo.h"

PlatformInfo::PlatformInfo(glm::mat4 modelMatrixTranslate)
{
	// primul rand de platforme
	this->modelMatrixTranslate = modelMatrixTranslate;
	this->color = blue;
	hasObstacle = obstacleWasHit = hasPineCone = false;
}

PlatformInfo::PlatformInfo(glm::mat4 modelMatrixTranslate, glm::vec3 color, float length)
{
	this->modelMatrixTranslate = modelMatrixTranslate;
	this->color = color;

	if (color == blue || color == green || color == orange || length < LENGTH_LIMIT_OBSTACLE) {
		hasObstacle = false;
	}
	else {
		hasObstacle = true;
	}

	if (color == blue) {
		hasPineCone = true;
	}
	else {
		hasPineCone = false;
	}

	obstacleWasHit = false;
}

PlatformInfo::~PlatformInfo()
{
}

glm::mat4 PlatformInfo::GetModelMatrixTranslate()
{
	return modelMatrixTranslate;
}

glm::vec3 PlatformInfo::GetColor()
{
	return color;
}

void PlatformInfo::SetColor(glm::vec3 color)
{
	this->color = color;
}

bool PlatformInfo::HasObstacle()
{
	return hasObstacle;
}

bool PlatformInfo::ObstacleWasHit()
{
	return obstacleWasHit;
}

void PlatformInfo::SetObstacleHit()
{
	obstacleWasHit = true;
}

bool PlatformInfo::HasPineCone()
{
	return hasPineCone;
}

void PlatformInfo::SetHasPineCone(bool value)
{
	hasPineCone = value;
}
