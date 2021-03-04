#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>
#include <Laboratoare\Tema1\Bow.h>
#include <Laboratoare\Tema1\Shuriken.h>
#include <Laboratoare\Tema1\Balloon.h>
#include <Laboratoare\Tema1\PowerBar.h>

#define SHURIKEN_NUM 5
#define BALLOON_NUM 5
#define SCALE_FACTOR_HIT 10
#define RED_BALLOON_POINT 30
#define YELLOW_BALLOON_POINT -50
#define SHURIKEN_POINT 10

class Tema1 : public SimpleScene
{
public:
	Tema1();
	~Tema1();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void RenderBow(float step);
	void RenderArrow(float step);
	void RenderPowerBar(float step);
	void RenderBarN(PowerBar* barN, float step, glm::mat3 modelMatBarN);
	void RenderShurikens(float step);
	void RenderBalloons(float step);

	bool checkCollisionCircleCircle(glm::vec3 centerPoint1, float radius1, glm::vec3 centerPoint2, float radius2);
	bool checkCollisionPointCircle(glm::vec3 point, glm::vec3 circleMidPoint, float radius);

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

protected:
	Bow* bow;
	std::vector<Shuriken*> shurikens;
	std::vector<Balloon*> balloons;
	PowerBar* bar1, *bar2, *bar3;
	int score, lives;
	bool gameOver, showedGameOver;

	glm::mat3 modelMatrix;
	
	int shurikenDelay[SHURIKEN_NUM];
	int balloonDelay[BALLOON_NUM];
	int mouseButtonLeftPressed;
};
