#include "Bow.h"

Bow::Bow(glm::vec3 middlePoint, float length, glm::vec3 color) {
	bowstringMiddlePoint = middlePoint;
	bowstringLength = length;
	bowColor = color;

	createBowstring();
	createBowstick();

	float halfLength = bowstringLength / 2;
	arrow = new Arrow(bowstringMiddlePoint + glm::vec3(halfLength, 0, 0), halfLength, bowColor);

	translateY = 0;
	rotateAngle = 0;
	modelMatrix = glm::mat3(1);

	// calculare centru si raza arc pentru coliziune
	bigRadius = bowstringLength * sqrt(1 - SQRT_2 / 2);
	centerPoint = middlePoint + glm::vec3( bowstringLength * (SQRT_2 - 1) / 2, 0, 0);
}


void Bow::createBowstring()
{
	bowstring = new Mesh("bowstring");
	float halfLength = bowstringLength / 2;

	std::vector<VertexFormat> vertices = {
		VertexFormat(bowstringMiddlePoint + glm::vec3(0, -halfLength, 0), bowColor),
		VertexFormat(bowstringMiddlePoint + glm::vec3(0, halfLength, 0), bowColor)
	};

	std::vector<unsigned short> indices = { 0, 1 };

	bowstring->SetDrawMode(GL_LINES);
	bowstring->InitFromData(vertices, indices);
}

void Bow::createBowstick()
{
	bowstick = new Mesh("bowstick");

	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	float angle;
	float halfLength = bowstringLength / 2;
	float radius = SQRT_2 * halfLength;
	/* semicercul este desenat cu centrul avut la distanta -(jumatate lungime coarda arc) pe OX
	 fata de mijlocul corzii */
	glm::vec3 translate = glm::vec3(bowstringMiddlePoint.x - halfLength, bowstringMiddlePoint.y, 0);

	for (unsigned short i = 0; i < NUM_ITER; i++)
	{
		angle = HALF_PI * i / NUM_ITER - QUARTER_PI;

		vertices.emplace_back(glm::vec3(cos(angle) * radius, sin(angle) * radius, 1) + translate, bowColor);
		indices.push_back(i);
	}

	vertices.emplace_back(glm::vec3(cos(QUARTER_PI) * radius, sin(QUARTER_PI) * radius, 1) + translate, bowColor);
	indices.push_back(NUM_ITER);

	bowstick->SetDrawMode(GL_LINE_STRIP);
	bowstick->InitFromData(vertices, indices);
}

Bow::~Bow()
{
	delete bowstring;
	delete bowstick;
	delete arrow;
}

Mesh* Bow::GetBowstringMesh()
{
	return bowstring;
}

Mesh* Bow::GetBowstickMesh()
{
	return bowstick;
}

Arrow* Bow::GetArrow()
{
	return arrow;
}

glm::vec3 Bow::GetStringMiddlePoint()
{
	return bowstringMiddlePoint;
}

float Bow::GetStringMiddlePointX()
{
	return bowstringMiddlePoint.x;
}

float Bow::GetStringMiddlePointY()
{
	return bowstringMiddlePoint.y;
}

float Bow::GetStringLength()
{
	return bowstringLength;
}

float Bow::GetTranslateY()
{
	return translateY;
}

float Bow::GetRotateAngle()
{
	return rotateAngle;
}

glm::mat3 Bow::GetModelMatrix()
{
	return modelMatrix;
}

glm::vec3 Bow::GetCenterPoint()
{
	return centerPoint;
}

float Bow::GetBigRadius()
{
	return bigRadius;
}

void Bow::SetModelMatrix(glm::mat3 matrix)
{
	modelMatrix = matrix;
}

void Bow::IncTranslateY(float step, float windowHeight)
{
	translateY += step * BOW_SPEED;
	float upperBound = windowHeight / 2 - bowstringLength / 2;
	float lowerBound = -upperBound;

	if (translateY > upperBound || translateY < lowerBound) {
		translateY -= step * BOW_SPEED;
	}
}

void Bow::SetRotateAngle(float angle)
{
	rotateAngle = angle;
}

