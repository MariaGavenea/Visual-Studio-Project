#include "Shuriken.h"

Shuriken::Shuriken(glm::vec3 centerPoint, float radius, glm::vec3 color)
{
	shurikenCenterPoint = centerPoint;
	shurikenLittleRadius = radius;
	shurikenColor = color;

	createShuriken();

	ResetHit();
}

void Shuriken::createShuriken()
{
	shuriken = new Mesh("shuriken");

	std::vector<VertexFormat> vertices = {
		VertexFormat(shurikenCenterPoint, shurikenColor),
		VertexFormat(shurikenCenterPoint + glm::vec3(0, -shurikenLittleRadius, 0), shurikenColor),
		VertexFormat(shurikenCenterPoint + glm::vec3(shurikenLittleRadius, -shurikenLittleRadius, 0), shurikenColor),

		VertexFormat(shurikenCenterPoint + glm::vec3(shurikenLittleRadius, 0, 0), shurikenColor),
		VertexFormat(shurikenCenterPoint + glm::vec3(shurikenLittleRadius, shurikenLittleRadius, 0), shurikenColor),

		VertexFormat(shurikenCenterPoint + glm::vec3(0, shurikenLittleRadius, 0), shurikenColor),
		VertexFormat(shurikenCenterPoint + glm::vec3(-shurikenLittleRadius, shurikenLittleRadius, 0), shurikenColor),

		VertexFormat(shurikenCenterPoint + glm::vec3(-shurikenLittleRadius, 0, 0), shurikenColor),
		VertexFormat(shurikenCenterPoint + glm::vec3(-shurikenLittleRadius, -shurikenLittleRadius, 0), shurikenColor),
	};

	std::vector<unsigned short> indices = { 
		0, 1, 2,
		0, 3, 4,
		0, 5, 6,
		0, 7, 8
	};

	shuriken->SetDrawMode(GL_TRIANGLES);
	shuriken->InitFromData(vertices, indices);
}


Shuriken::~Shuriken()
{
	delete shuriken;
}

Mesh* Shuriken::GetShuriken()
{
	return shuriken;
}

glm::vec3 Shuriken::GetShurikenCenterPoint()
{
	return shurikenCenterPoint;
}

float Shuriken::GetShurikenCenterPointX()
{
	return shurikenCenterPoint.x;
}

float Shuriken::GetShurikenCenterPointY()
{
	return shurikenCenterPoint.y;
}

float Shuriken::GetTranslateX()
{
	return translateX;
}

float Shuriken::GetScaleX()
{
	return scaleX;
}

float Shuriken::GetScaleY()
{
	return scaleY;
}

float Shuriken::GetRotateAngle()
{
	return rotateAngle;
}

float Shuriken::GetBigRadius()
{
	return shurikenLittleRadius * SQRT_2;
}

bool Shuriken::WasHit()
{
	return hit;
}

void Shuriken::DecTranslateX(float step)
{
	translateX -= step * SHURIKEN_SPEED;
}

void Shuriken::DecScaleX(float step)
{
	scaleX -= step * SCALE_OX_HIT_FACTOR;

	if (scaleX < 0) {
		scaleX = 0;
	}
}

void Shuriken::DecScaleY(float step)
{
	scaleY -= step * SCALE_OY_HIT_FACTOR;

	if (scaleY < 0) {
		scaleY = 0;
	}
}

void Shuriken::IncRotationAngle(float step)
{
	rotateAngle += step * SHURIKEN_ROTATION_SPEED;
}

void Shuriken::SetHit(bool val)
{
	hit = val;
}

void Shuriken::resetTranslate(float offset)
{
	if (shurikenCenterPoint.x + translateX + shurikenLittleRadius + offset < 0) {
		translateX = 0;
	}
}

void Shuriken::ResetTranslateOutOfWindow(float absCenterX)
{
	if (absCenterX + shurikenLittleRadius * SQRT_2 < 0) {
		translateX = 0;
	}
}

void Shuriken::ResetHit()
{
	translateX = 0;
	rotateAngle = 0;
	scaleX = 1;
	scaleY = 1;
	hit = false;
}
