#include "Arrow.h"

Arrow::Arrow(glm::vec3 leftPoint, float length, glm::vec3 color)
{
	stickLeftPoint = leftPoint;
	stickLength = length;
	arrowColor = color;

	createStick();
	createArrowhead();

	ResetMoveFactors();

	modelMatrix = glm::mat3(1);
}

void Arrow::createStick()
{
	stick = new Mesh("arrowStick");

	std::vector<VertexFormat> vertices = {
		VertexFormat(stickLeftPoint, arrowColor),
		VertexFormat(stickLeftPoint + glm::vec3(stickLength, 0, 0), arrowColor)
	};

	std::vector<unsigned short> indices = { 0, 1 };

	stick->SetDrawMode(GL_LINES);
	stick->InitFromData(vertices, indices);
}

void Arrow::createArrowhead()
{
	arrowhead = new Mesh("arrowhead");
	float quarterStickLen = stickLength / 4;

	std::vector<VertexFormat> vertices = {
		VertexFormat(stickLeftPoint + glm::vec3(stickLength + quarterStickLen, 0, 0), arrowColor),
		VertexFormat(stickLeftPoint + glm::vec3(stickLength, quarterStickLen, 0), arrowColor),
		VertexFormat(stickLeftPoint + glm::vec3(stickLength, - quarterStickLen, 0), arrowColor),
	};

	std::vector<unsigned short> indices = { 0, 1, 2 };

	arrowhead->SetDrawMode(GL_TRIANGLES);
	arrowhead->InitFromData(vertices, indices);
}

Arrow::~Arrow()
{
	delete stick;
	delete arrowhead;
}


Mesh* Arrow::GetStick()
{
	return stick;
}

Mesh* Arrow::GetArrowhead()
{
	return arrowhead;
}

glm::vec3 Arrow::GetLeftPoint()
{
	return stickLeftPoint;
}


float Arrow::GetLeftPointX()
{
	return stickLeftPoint.x;
}

float Arrow::GetLeftPointY()
{
	return stickLeftPoint.y;
}

float Arrow::GetSpeedFactor()
{
	return speedFactor;
}

float Arrow::GetTranslateX()
{
	return translateX;
}

float Arrow::GetTranslateY()
{
	return translateY;
}

float Arrow::GetArrowAngle()
{
	return arrowAngle;
}

glm::vec3 Arrow::GetArrowheadPoint()
{
	return stickLeftPoint + glm::vec3(5 * stickLength / 4, 0, 0);
}

glm::mat3 Arrow::GetModelMatrix()
{
	return modelMatrix;
}

bool Arrow::WasThrown()
{
	return thrown;
}

void Arrow::SetModelMatrix(glm::mat3 mat)
{
	modelMatrix = mat;
}

void Arrow::SetThrown(int thrown)
{
	this->thrown = thrown;
}

void Arrow::SetSpeedFactor(float factor)
{
	speedFactor = factor;
}

void Arrow::SetTranslateY(float transY)
{
	translateY = transY;
}

void Arrow::IncTranslateX(float step)
{
	translateX += speedFactor * step * SPEED_AMPLIFIER;
}

void Arrow::setArrowAngle(float angle)
{
	arrowAngle = angle;
}

void Arrow::ResetOutOfWindow(glm::vec3 absLeftPoint, float windowX, float windowY)
{
	if (absLeftPoint.x < 0 || absLeftPoint.x > windowX || absLeftPoint.y > windowY || absLeftPoint.y < 0) {
		ResetMoveFactors();
	}
}

void Arrow::ResetMoveFactors()
{
	speedFactor = 0;
	translateX = 0;
	translateY = 0;
	arrowAngle = 0;
	thrown = false;
}