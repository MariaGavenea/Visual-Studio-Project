#pragma once

#include <Core/Engine.h>
#define SPEED_AMPLIFIER 500

class Arrow {
public:
	Arrow(glm::vec3 leftPoint ,float length, glm::vec3 color);
	~Arrow();

	Mesh* GetStick();
	Mesh* GetArrowhead();
	glm::vec3 GetLeftPoint();
	float GetLeftPointX();
	float GetLeftPointY();
	float GetSpeedFactor();
	float GetTranslateX();
	float GetTranslateY();
	float GetArrowAngle();
	glm::vec3 GetArrowheadPoint();
	glm::mat3 GetModelMatrix();
	bool WasThrown();

	void SetModelMatrix(glm::mat3 mat);
	void SetThrown(int thrown);
	void SetSpeedFactor(float factor);
	void SetTranslateY(float transY);
	void IncTranslateX(float step);
	void setArrowAngle(float angle);
	void ResetOutOfWindow(glm::vec3 absLeftPoint, float windowX, float windowY);
	void ResetMoveFactors();
private:
	void createStick();
	void createArrowhead();

protected:
	Mesh* stick;
	Mesh* arrowhead;

	glm::vec3 stickLeftPoint;
	glm::vec3 arrowColor;
	float stickLength;

	float speedFactor;
	float translateX, translateY;
	float arrowAngle;
	bool thrown;

	glm::mat3 modelMatrix;
};