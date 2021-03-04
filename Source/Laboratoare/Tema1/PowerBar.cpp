#include "PowerBar.h"

PowerBar::PowerBar(glm::vec3 leftMiddlePoint, float height, float width, glm::vec3 leftColor, glm::vec3 rightColor, std::string name)
{
	barLeftMidPoint = leftMiddlePoint;
	barHeight = height;
	barWidth = width;
	barLeftColor = leftColor;
	barRightColor = rightColor;
	this->name = name;

	createBar();

	scaleX = 0;
}


void PowerBar::createBar()
{
	bar = new Mesh(name);

	std::vector<VertexFormat> vertices = {
		VertexFormat(barLeftMidPoint + glm::vec3(0, barHeight / 2, 0),barLeftColor),
		VertexFormat(barLeftMidPoint + glm::vec3(0, -barHeight / 2, 0), barLeftColor),
		VertexFormat(barLeftMidPoint + glm::vec3(barWidth, -barHeight / 2, 0), barRightColor),
		VertexFormat(barLeftMidPoint + glm::vec3(barWidth, barHeight / 2, 0), barRightColor)
	};

	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	bar->SetDrawMode(GL_TRIANGLE_FAN);
	bar->InitFromData(vertices, indices);

}

PowerBar::~PowerBar()
{
	delete bar;
}

Mesh* PowerBar::GetBar()
{
	return bar;
}

float PowerBar::GetLeftMidPointX()
{
	return barLeftMidPoint.x;
}

float PowerBar::GetLeftMidPointY()
{
	return barLeftMidPoint.y;
}

float PowerBar::GetScaleX()
{
	return scaleX;
}

void PowerBar::incScaleX(float step)
{
	scaleX += step * SCALE_FACTOR;

	if (scaleX > 1) {
		scaleX = 1;
	}
}

void PowerBar::resetScaleX()
{
	scaleX = 0;
}
