#include "Player.h"

Player::Player()
{
	player = new Mesh("sphere");
	player->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");

	speed = 0;
	heightOffsetJump = 0;

	jumpUp = jumpDown = jump = false;

	effectColor = defaultColor;
}

Player::~Player()
{
	delete player;
}

Mesh* Player::GetPlayerMesh()
{
	return player;
}

float Player::GetPlayerSpeed()
{
	return speed;
}

glm::vec3 Player::GetDefaultColor()
{
	return defaultColor;
}

glm::vec3 Player::GetEffectColor()
{
	return effectColor;
}

void Player::SetEffectColor(glm::vec3 color)
{
	effectColor = color;
}

void Player::IncSpeed(float step)
{
	speed += step;

	if (speed > PLAYER_MAX_SPEED) {
		speed = PLAYER_MAX_SPEED;
		return;
	}

	if (speed < PLAYER_MIN_SPEED) {
		speed = PLAYER_MIN_SPEED;
	}
}

void Player::DecSpeedUntilStop(float step)
{
	speed -= step;
	if (speed < 0) {
		speed = 0;
	}
}

bool Player::IsJumping()
{
	return jump;
}

void Player::SetJump()
{
	jump = true;
	jumpUp = true;
	jumpDown = false;
}

float Player::ComputeJumpStep(float step)
{
	if (jumpUp) {
		return this->IncHeightOffset(step);
	}
		
	if (jumpDown) {
		return this->DecHeightOffset(step);
	}

	return 0;
}

// asigura saritura pana la o anumita inaltime
float Player::IncHeightOffset(float step)
{
	float newHeight = heightOffsetJump + step;

	if (newHeight > PLAYER_MAX_HEIGHT_OFFSET) {
		float incValue = PLAYER_MAX_HEIGHT_OFFSET - heightOffsetJump;
		heightOffsetJump = PLAYER_MAX_HEIGHT_OFFSET;

		jumpUp = false;
		jumpDown = true;

		return incValue;
	}

	heightOffsetJump = newHeight;
	return step;
}

// jucatorul se intoarce la inaltimea de la care a inceput saritura
float Player::DecHeightOffset(float step)
{
	float newHeight = heightOffsetJump - step;

	if (newHeight < 0) {
		float decValue = -heightOffsetJump;
		heightOffsetJump = 0;

		jumpDown = false;
		jump = false;
		return decValue;
	}

	heightOffsetJump = newHeight;
	return -step;
}
