#pragma once

#include <Core/Engine.h>
#include <Laboratoare\Tema3\Colors.h>

#define PLAYER_SCALE 0.26
#define PLAYER_OY_OFFSET (PLAYER_SCALE / 2)
#define PLAYER_RADIUS PLAYER_OY_OFFSET

#define PLAYER_MAX_SPEED 3
#define PLAYER_MED_SPEED 2.5
#define PLAYER_MIN_SPEED 1
#define PLAYER_OY_SPEED 2

#define PLAYER_MAX_HEIGHT_OFFSET 1

class Player {
public:
	Player();
	~Player();

	Mesh* GetPlayerMesh();
	float GetPlayerSpeed();
	glm::vec3 GetDefaultColor();
	glm::vec3 GetEffectColor();
	void SetEffectColor(glm::vec3 color);

	void IncSpeed(float step);
	void DecSpeedUntilStop(float step);

	bool IsJumping();
	void SetJump();

	float ComputeJumpStep(float step);

private:
	float IncHeightOffset(float step);
	float DecHeightOffset(float step);

protected:
	Mesh* player;

	float speed;
	float heightOffsetJump;
	bool jumpUp, jumpDown, jump;

	glm::vec3 effectColor;
	const glm::vec3 defaultColor = pink;
};