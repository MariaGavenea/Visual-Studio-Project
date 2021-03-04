#include "Fuel.h"

Fuel::Fuel(float height, float length, glm::vec3 position)
{
	this->height = height;
	this->length = length;
	positionLeftMiddlePoint = position;

	backgroundColor = white;
	fuelColor = green;
	scaleFactor = 1;

	CreateBackground();
	CreateFuel();
}

Fuel::~Fuel()
{
	delete background;
	delete fuel;
}

Mesh* Fuel::GetBackgroundMesh()
{
	return background;
}

Mesh* Fuel::GetFuelMesh()
{
	return fuel;
}

glm::vec3 Fuel::GetLeftMiddlePoint()
{
	return positionLeftMiddlePoint;
}

float Fuel::GetScaleFactor()
{
	return scaleFactor;
}

glm::vec3 Fuel::GetBackgroundColor()
{
	return backgroundColor;
}

glm::vec3 Fuel::GetFuelColor()
{
	return fuelColor;
}

void Fuel::IncScaleFactor(float step)
{
	scaleFactor += step;

	if (scaleFactor > 1) {
		scaleFactor = 1;
	}
}

void Fuel::DecScaleFactor(float step)
{
	scaleFactor -= step;

	if (scaleFactor < 0) {
		scaleFactor = 0;
	}
}

void Fuel::ResetScaleFactor()
{
	scaleFactor = 1;
}

void Fuel::CreateBackground()
{
	
	background = new Mesh("background");
	float halfHeight = height / 2;

	std::vector<VertexFormat> vertices = {
		VertexFormat(positionLeftMiddlePoint + glm::vec3(0, -halfHeight, 0), backgroundColor),
		VertexFormat(positionLeftMiddlePoint + glm::vec3(length, -halfHeight, 0), backgroundColor),
		VertexFormat(positionLeftMiddlePoint + glm::vec3(length, halfHeight, 0), backgroundColor),
		VertexFormat(positionLeftMiddlePoint + glm::vec3(0, halfHeight, 0), backgroundColor)
	};

	std::vector<unsigned short> indices = { 0, 1, 2, 0, 2, 3};

	background->SetDrawMode(GL_TRIANGLES);
	background->InitFromData(vertices, indices);
}

void Fuel::CreateFuel()
{
	fuel = new Mesh("fuel");
	float halfHeight = height / 2;

	std::vector<VertexFormat> vertices = {
		VertexFormat(positionLeftMiddlePoint + glm::vec3(0, -halfHeight, 0), fuelColor),
		VertexFormat(positionLeftMiddlePoint + glm::vec3(length, -halfHeight, 0), fuelColor),
		VertexFormat(positionLeftMiddlePoint + glm::vec3(length, halfHeight, 0), fuelColor),
		VertexFormat(positionLeftMiddlePoint + glm::vec3(0, halfHeight, 0), fuelColor)
	};

	std::vector<unsigned short> indices = { 0, 1, 2, 0, 2, 3};

	fuel->SetDrawMode(GL_TRIANGLES);
	fuel->InitFromData(vertices, indices);
}
