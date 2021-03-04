#pragma once
#include <Core/Engine.h>
#define SCALE_FACTOR 1

class PowerBar {
public:
	PowerBar(glm::vec3 leftMiddlePoint, float height, float width, glm::vec3 leftColor, glm::vec3 rightColor, std::string name);
	~PowerBar();

	Mesh* GetBar();
	float GetLeftMidPointX();
	float GetLeftMidPointY();
	float GetScaleX();

	void incScaleX(float step);
	void resetScaleX();

private:
	void createBar();

protected:
	Mesh* bar;

	glm::vec3 barLeftMidPoint;
	float barHeight;
	float barWidth;
	float scaleX;
	glm::vec3 barLeftColor, barRightColor;
	std::string name;

};