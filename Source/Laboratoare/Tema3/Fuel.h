#pragma once

#include <Core/Engine.h>
#include <Laboratoare\Tema3\Colors.h>

#define FUEL_SPEED 0.025

class Fuel{
public:
	Fuel(float height, float length, glm::vec3 position);
	~Fuel();

	Mesh* GetBackgroundMesh();
	Mesh* GetFuelMesh();
	glm::vec3 GetLeftMiddlePoint();
	float GetScaleFactor();
	glm::vec3 GetBackgroundColor();
	glm::vec3 GetFuelColor();

	void IncScaleFactor(float step);
	void DecScaleFactor(float step);
	void ResetScaleFactor();

private:
	void CreateBackground();
	void CreateFuel();

protected:
	Mesh* background;
	Mesh* fuel;

	float height, length;
	glm::vec3 positionLeftMiddlePoint;

	glm::vec3 backgroundColor;
	glm::vec3 fuelColor;

	float scaleFactor;
};