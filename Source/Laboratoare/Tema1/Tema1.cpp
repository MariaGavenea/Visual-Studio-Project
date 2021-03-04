#include "Tema1.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform2D.h"

using namespace std;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	score = 0;
	std::cout << "Score: " << score << "\n";

	lives = 3;
	std::cout << "Lives: " << lives << "\n";

	gameOver = showedGameOver = false;

	// bow
	glm::vec3 bowStringMiddlePoint = glm::vec3(resolution.y / 8, resolution.y / 2, 1);
	float bowStringLength = resolution.y / 4;
	glm::vec3 bowColor = glm::vec3(0, 1, 0);

	bow = new Bow(bowStringMiddlePoint, bowStringLength, bowColor);

	// shuriken
	for (int i = 0; i < SHURIKEN_NUM; i++) {
		shurikens.push_back(new Shuriken(glm::vec3(resolution.x, resolution.y / 2, 1), 
			resolution.y / 16, glm::vec3(1, 0, 1)));

		shurikenDelay[i] = resolution.x / (1 << i);
	}

	// balloon
	int j = 1;

	for (int i = 0; i < BALLOON_NUM; i++) {
		balloons.push_back(new Balloon(glm::vec3(3 * resolution.x / 4, -resolution.y / 16, 1),
			resolution.y / 16, glm::vec3(1, j, 0)));

		balloonDelay[i] = -resolution.y / (1 << i);
		j = 1 - j;
	}

	// power bar
	float barHeight = bowStringLength / 8;
	float barWidth = bowStringLength / 3; // three bars
	glm::vec3 bar1LeftMidPoint = bowStringMiddlePoint + glm::vec3(0, -bowStringLength / 2 - barHeight, 0);
	glm::vec3 green = glm::vec3(0, 1, 0);
	glm::vec3 yellow = glm::vec3(1, 1, 0);
	glm::vec3 red = glm::vec3(1, 0, 0);

	bar1 = new PowerBar(bar1LeftMidPoint, barHeight, barWidth, green, yellow, "greenBar");
	bar2 = new PowerBar(bar1LeftMidPoint + glm::vec3(barWidth, 0, 0), barHeight, barWidth, yellow, yellow, "yellowBar");
	bar3 = new PowerBar(bar1LeftMidPoint + glm::vec3(2 * barWidth, 0, 0), barHeight, barWidth, yellow, red, "redBar");

	mouseButtonLeftPressed = 0;
}

void Tema1::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
	if (gameOver == false) {
		RenderBow(deltaTimeSeconds);
		RenderShurikens(deltaTimeSeconds);
		RenderBalloons(deltaTimeSeconds);
	}
	else {
		if (!showedGameOver) {
			std::cout << "\nGAME OVER\n\nPress \"esc\" to exit.\n";
			showedGameOver = true;
		}
	}
}

void Tema1::FrameEnd()
{

}


void Tema1::RenderBow(float step)
{
	modelMatrix = glm::mat3(1);
	// (2) translatia pe OY
	modelMatrix *= Transform2D::Translate(0, bow->GetTranslateY());

	if (mouseButtonLeftPressed && !bow->GetArrow()->WasThrown()) {
		RenderPowerBar(step);
	}

	// (1) rotatia dupa cursorul mouse-ului
	modelMatrix *= Transform2D::Translate(bow->GetStringMiddlePointX(), bow->GetStringMiddlePointY());
	modelMatrix *= Transform2D::Rotate(bow->GetRotateAngle());
	modelMatrix *= Transform2D::Translate(-bow->GetStringMiddlePointX(), -bow->GetStringMiddlePointY());

	bow->SetModelMatrix(modelMatrix);

	RenderMesh2D(bow->GetBowstringMesh(), shaders["VertexColor"], modelMatrix);
	RenderMesh2D(bow->GetBowstickMesh(), shaders["VertexColor"], modelMatrix);

	RenderArrow(step);
}

void Tema1::RenderArrow(float step)
{
	if (bow->GetArrow()->GetSpeedFactor()) {
		// sageata a fost aruncata
		bow->GetArrow()->IncTranslateX(step);

		// (1) determina miscarea, iar (2) si (3) directia de deplasare
		modelMatrix = glm::mat3(1);

		// (3) se translateaza pe OY cu inaltimea la care a fost aruncata sageata
		modelMatrix *= Transform2D::Translate(0, bow->GetArrow()->GetTranslateY());

		// (2) se face rotatia fata de punctul din mijlocul corzii arcului
		modelMatrix *= Transform2D::Translate(bow->GetStringMiddlePointX(), bow->GetStringMiddlePointY());
		modelMatrix *= Transform2D::Rotate(bow->GetArrow()->GetArrowAngle());
		modelMatrix *= Transform2D::Translate(-bow->GetStringMiddlePointX(), -bow->GetStringMiddlePointY());

		// (1) initial, sageata este paralela cu axa OX, si se deplaseaza pe OX cate putin la fiecare frame
		modelMatrix *= Transform2D::Translate(bow->GetArrow()->GetTranslateX(), 0);

		/* daca sageata a iesit din fereastra complet, i se reseteaza factorii de miscare ca
		 la urmatorul frame sa fie desenata impreuna cu arcul */
		glm::vec3 absArrLeftPoint = modelMatrix * bow->GetArrow()->GetLeftPoint();
		bow->GetArrow()->ResetOutOfWindow(absArrLeftPoint, window->GetResolution().x, window->GetResolution().y);
	}

	bow->GetArrow()->SetModelMatrix(modelMatrix);
	RenderMesh2D(bow->GetArrow()->GetStick(), shaders["VertexColor"], modelMatrix);
	RenderMesh2D(bow->GetArrow()->GetArrowhead(), shaders["VertexColor"], modelMatrix);
}

void Tema1::RenderPowerBar(float step) {

	RenderBarN(bar1, step, modelMatrix);

	if (bar1->GetScaleX() == 1) {
		RenderBarN(bar2, step, modelMatrix);

		if (bar2->GetScaleX() == 1) {
			RenderBarN(bar3, step, modelMatrix);
		}
	}
}

void Tema1::RenderBarN(PowerBar* barN, float step, glm::mat3 modelMatBarN)
{
	barN->incScaleX(step);

	modelMatBarN *= Transform2D::Translate(barN->GetLeftMidPointX(), barN->GetLeftMidPointY());
	modelMatBarN *= Transform2D::Scale(barN->GetScaleX(), 1);
	modelMatBarN *= Transform2D::Translate(-barN->GetLeftMidPointX(), -barN->GetLeftMidPointY());

	RenderMesh2D(barN->GetBar(), shaders["VertexColor"], modelMatBarN);
}

void Tema1::RenderShurikens(float step)
{
	for (int i = 0; i < SHURIKEN_NUM; i++) {
		if (shurikens[i]->WasHit()) {
			// se executa deformarea shuriken-ului, ramane in locul in care a fost lovit
			shurikens[i]->DecScaleX(step);
			shurikens[i]->DecScaleY(step);
		} else {
			// shuriken-ul isi continua deplasarea
			shurikens[i]->DecTranslateX(step);
		}

		shurikens[i]->IncRotationAngle(step);

		modelMatrix = glm::mat3(1);
		// are loc deplasarea pe axa OX, fiecare shuriken de pe o linie are un anumit delay
		// pe axa OY shuriken-urile sunt asezate in oglinda fata de mijlocul ferestrei
		modelMatrix *= Transform2D::Translate(shurikens[i]->GetTranslateX() + shurikenDelay[i],
			window->GetResolution().y * (i - SHURIKEN_NUM / 2) / SHURIKEN_NUM);

		// numai shuriken-urile lovite sunt deformate prin scalare, toate shuriken-urile se rotesc in jurul centrului propriu
		modelMatrix *= Transform2D::Translate(shurikens[i]->GetShurikenCenterPointX(), shurikens[i]->GetShurikenCenterPointY());
		modelMatrix *= Transform2D::Rotate(shurikens[i]->GetRotateAngle());
		modelMatrix *= Transform2D::Scale(shurikens[i]->GetScaleX(), shurikens[i]->GetScaleY());
		modelMatrix *= Transform2D::Translate(-shurikens[i]->GetShurikenCenterPointX(), -shurikens[i]->GetShurikenCenterPointY());

		RenderMesh2D(shurikens[i]->GetShuriken(), shaders["VertexColor"], modelMatrix);

		// daca shuriken-ul a iesit din fereastra complet, i se reseteaza factorul de translatie pe OX
		glm::vec3 absShurikenCenter = modelMatrix * shurikens[i]->GetShurikenCenterPoint();
		shurikens[i]->ResetTranslateOutOfWindow(absShurikenCenter.x);

		// verificare coliziune sageata aruncata - shuriken
		if (bow->GetArrow()->WasThrown() && !shurikens[i]->WasHit()) {
			glm::vec3 absArrheadPoint = bow->GetArrow()->GetModelMatrix() * bow->GetArrow()->GetArrowheadPoint();

			if (checkCollisionPointCircle(absArrheadPoint, absShurikenCenter, shurikens[i]->GetBigRadius())) {
				// bow->GetArrow()->ResetMoveFactors();
				shurikens[i]->SetHit(true);

				score += SHURIKEN_POINT;
				std::cout << "Score: " << score << "\n";
			}
		}

		// verificare coliziune arc - shuriken
		glm::vec3 absBowCenterPoint = bow->GetModelMatrix() * bow->GetCenterPoint();
		if (!shurikens[i]->WasHit() && checkCollisionCircleCircle(absBowCenterPoint, bow->GetBigRadius(),
			absShurikenCenter, shurikens[i]->GetBigRadius())) {

			lives--;
			std::cout << "Lives: "<< lives << "\n";

			if (lives <= 0) {
				gameOver = true;
			}

			// shuriken-ul dispare din scena si va aparea peste cateva frame-uri (in functie de delay)
			shurikens[i]->ResetHit();

		}

		// daca a ajuns scalat la 0 pe ambele axe, shuriken-ul lovit va aparea peste cateva frame-uri (in functie de delay)
		if (shurikens[i]->GetScaleX() == 0 && shurikens[i]->GetScaleY() == 0) {
			shurikens[i]->ResetHit();
		}
	}
}

void Tema1::RenderBalloons(float step) {
	for (int i = 0; i < BALLOON_NUM; i++) {
		if (balloons[i]->WasHit()) {
			// se executa deformarea balonului-ului, ramane in locul in care a fost lovit
			balloons[i]->DecScaleX(step);
			balloons[i]->DecScaleY(step);
		}
		else {
			// balonul isi continua deplasarea
			balloons[i]->IncBalloonTranslateY(step);
		}

		modelMatrix = glm::mat3(1);
		// are loc deplasarea pe axa OY, fiecare balon de pe o coloana are un anumit delay
		// pe axa OX baloanele sunt asezate in oglinda fata de mijlocul celei de-a doua jumatati a ferestrei
		modelMatrix *= Transform2D::Translate(window->GetResolution().x / 2 * (i - BALLOON_NUM / 2) / BALLOON_NUM,
			balloons[i]->GetTranslateY() + balloonDelay[i]);

		// numai baloanele lovite sunt deformate prin scalare
		modelMatrix *= Transform2D::Translate(balloons[i]->GetCenterPointX(), balloons[i]->GetCenterPointY());
		modelMatrix *= Transform2D::Scale(balloons[i]->GetScaleX(), balloons[i]->GetScaleY());
		modelMatrix *= Transform2D::Translate(-balloons[i]->GetCenterPointX(), -balloons[i]->GetCenterPointY());

		RenderMesh2D(balloons[i]->GetBalloon(), shaders["VertexColor"], modelMatrix);
		RenderMesh2D(balloons[i]->GetRope(), shaders["VertexColor"], modelMatrix);

		// daca balonul a iesit din fereastra complet, i se reseteaza factorul de translatie pe OY
		glm::vec3 absBalloonCenter = modelMatrix * balloons[i]->GetCenterPoint();
		balloons[i]->ResetTranslateOutOfWindow(absBalloonCenter.y, window->GetResolution().y);

		// verificare coliziune sageata aruncata - balon
		if (bow->GetArrow()->WasThrown() && !balloons[i]->WasHit()) {
			glm::vec3 absArrheadPoint = bow->GetArrow()->GetModelMatrix() * bow->GetArrow()->GetArrowheadPoint();

			if (checkCollisionPointCircle(absArrheadPoint, absBalloonCenter, balloons[i]->GetRadius())) {
				// bow->GetArrow()->ResetMoveFactors();
				balloons[i]->SetHit(true);

				if (balloons[i]->GetColor() == glm::vec3(1, 0, 0)) {
					// balon rosu
					score += RED_BALLOON_POINT;
				} else {
					if (balloons[i]->GetColor() == glm::vec3(1, 1, 0)) {
						// balon galben
						score += YELLOW_BALLOON_POINT;
					}
				}
				
				std::cout << "Score: " << score << "\n";
			}
		}

		// daca a ajuns scalat la 0 pe ambele axe, balonul lovit va aparea peste cateva frame-uri (in functie de delay)
		if (balloons[i]->GetScaleX() == 0 && balloons[i]->GetScaleY() == 0) {
			balloons[i]->ResetHit();
		}
	}
}

bool Tema1::checkCollisionCircleCircle(glm::vec3 centerPoint1, float radius1, glm::vec3 centerPoint2, float radius2)
{
	float deltaX = centerPoint1.x - centerPoint2.x;
	float deltaY = centerPoint1.y - centerPoint2.y;
	return sqrt(deltaX * deltaX + deltaY * deltaY) < radius1 + radius2;
}

bool Tema1::checkCollisionPointCircle(glm::vec3 point, glm::vec3 circleMidPoint, float radius)
{
	float deltaX = point.x - circleMidPoint.x;
	float deltaY = point.y - circleMidPoint.y;

	return sqrt(deltaX * deltaX + deltaY * deltaY) <= radius;
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
	if (window->KeyHold(GLFW_KEY_W)) {
		bow->IncTranslateY(deltaTime, window->GetResolution().y);
	}

	if (window->KeyHold(GLFW_KEY_S)) {
		bow->IncTranslateY(-deltaTime, window->GetResolution().y);
	}
}

void Tema1::OnKeyPress(int key, int mods)
{
	// add key press event
}

void Tema1::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
	int mouseX2 = mouseX - bow->GetStringMiddlePointX();
	int mouseY2 = (window->GetResolution().y - mouseY) - bow->GetStringMiddlePointY() - bow->GetTranslateY();

	if (mouseY2 == 0) {
		bow->SetRotateAngle(0);
		return;
	}

	/* daca cursorul se afla in cadranele 2, respectiv 3 arcul cu sageata vor fi rotiti cu
	PI/3, respectiv -PI/3 fata de OX */
	if (mouseX2 <= 0) {
		if (mouseY2 > 0) {
			bow->SetRotateAngle(PI / 3);
		}
		else {
			bow->SetRotateAngle(-PI / 3);
		}
		return;
	}

	// calculez unghiul pentru cadranele 1 si 4 (m-am asigurat mai sus ca mouseX2 > 0)
	float arcTan = atan((float)mouseY2 / mouseX2);

	// deschiderea arcului va fi intre -PI/3 si PI/3
	if (arcTan > PI / 3) {
		bow->SetRotateAngle(PI / 3);
		return;
	}

	if (arcTan < -PI / 3) {
		bow->SetRotateAngle(-PI / 3);
		return;
	}

	bow->SetRotateAngle(arcTan);
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
	if (button == GLFW_MOUSE_BUTTON_2) {
		mouseButtonLeftPressed = 1;
	}
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{

	// add mouse button release event
	if (button == GLFW_MOUSE_BUTTON_2) {
		if (!bow->GetArrow()->WasThrown()) {
			// seteaza factorul de viteza in functie de lungimea powerbar-ului
			bow->GetArrow()->SetSpeedFactor(bar1->GetScaleX() + bar2->GetScaleX() + bar3->GetScaleX());

			// pastreaza informatiile despre pozitia arcului la momentul tragerii sagetii
			bow->GetArrow()->SetTranslateY(bow->GetTranslateY());
			bow->GetArrow()->setArrowAngle(bow->GetRotateAngle());
			bow->GetArrow()->SetThrown(true);

		}

		mouseButtonLeftPressed = 0;

		bar1->resetScaleX();
		bar2->resetScaleX();
		bar3->resetScaleX();
	}
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}
