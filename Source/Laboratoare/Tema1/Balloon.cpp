#include "Balloon.h"

Balloon::Balloon(glm::vec3 centerPoint, float radius, glm::vec3 color)
{
	balloonCenterPoint = centerPoint;
	balloonRadius = radius;
    balloonColor = color;

	createBalloon();
	createRope();

	ResetHit();
}

void Balloon::createBalloon()
{
	balloon = new Mesh("balloon");

	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	float angle;
	glm::vec3 translate = glm::vec3(balloonCenterPoint.x, balloonCenterPoint.y, 0);

	for (unsigned short i = 0; i < NUM_ITER; i++)
	{
		angle = 2 * PI * i / NUM_ITER;

		vertices.emplace_back(glm::vec3(cos(angle) * balloonRadius, sin(angle) * balloonRadius, 1) + translate, balloonColor);
		indices.push_back(i);
	}

	vertices.emplace_back(glm::vec3(cos(QUARTER_PI) * balloonRadius, sin(QUARTER_PI) * balloonRadius, 1) + translate, balloonColor);
	indices.push_back(NUM_ITER);

	balloon->SetDrawMode(GL_TRIANGLE_FAN);
	balloon->InitFromData(vertices, indices);
}

void Balloon::createRope()
{
	rope = new Mesh("rope");

	glm::vec3 startRopePoint = balloonCenterPoint + glm::vec3(0, -balloonRadius, 0);
	float ropeOyLen = balloonRadius / 8;

	std::vector<VertexFormat> vertices = {
		VertexFormat(startRopePoint, balloonColor),
		VertexFormat(startRopePoint + glm::vec3(-ropeOyLen / 2, -ropeOyLen, 0), balloonColor),
		VertexFormat(startRopePoint + glm::vec3(ropeOyLen / 2, -2 * ropeOyLen, 0), balloonColor),
		VertexFormat(startRopePoint + glm::vec3(-ropeOyLen / 2, -3 * ropeOyLen, 0), balloonColor),
		VertexFormat(startRopePoint + glm::vec3(ropeOyLen / 2, -4 * ropeOyLen, 0), balloonColor)
	};

	std::vector<unsigned short> indices = { 0, 1, 2, 3, 4 };

	rope->SetDrawMode(GL_LINE_STRIP);
	rope->InitFromData(vertices, indices);
}

Balloon::~Balloon()
{
	delete balloon;
	delete rope;
}

Mesh* Balloon::GetBalloon()
{
	return balloon;
}

Mesh* Balloon::GetRope()
{
	return rope;
}

float Balloon::GetTranslateY()
{
	return translateY;
}

float Balloon::GetScaleX()
{
	return scaleX;
}

float Balloon::GetScaleY()
{
	return scaleY;
}

glm::vec3 Balloon::GetCenterPoint()
{
	return balloonCenterPoint;
}

float Balloon::GetRadius()
{
	return balloonRadius;
}

float Balloon::GetCenterPointX()
{
	return balloonCenterPoint.x;
}

float Balloon::GetCenterPointY()
{
	return balloonCenterPoint.y;
}

glm::vec3 Balloon::GetColor()
{
	return balloonColor;
}

bool Balloon::WasHit()
{
	return hit;
}

void Balloon::SetHit(bool val)
{
	hit = val;
}

void Balloon::IncBalloonTranslateY(float step)
{
	translateY += step * BALLOON_SPEED;
}

void Balloon::DecScaleX(float step)
{
	scaleX -= step * SCALE_OX_FACTOR;

	if (scaleX < 0) {
		scaleX = 0;
	}
}

void Balloon::DecScaleY(float step)
{
	scaleY -= step * SCALE_OY_FACTOR;

	if (scaleY < 0) {
		scaleY = 0;
	}
}


void Balloon::ResetTranslateOutOfWindow(float absCenterY, float height)
{
	if (absCenterY - balloonRadius >= height) {
		translateY = 0;
	}
}

void Balloon::ResetHit()
{
	translateY = 0;
	scaleX = BALLOON_OX_SCALE;
	scaleY = 1;
	hit = false;
}
