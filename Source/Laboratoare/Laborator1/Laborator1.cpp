#include "Laborator1.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

bool PRESSED_KEY_F;
bool PRESSED_KEY_A, PRESSED_KEY_D;
bool PRESSED_KEY_W, PRESSED_KEY_S;
bool PRESSED_KEY_E, PRESSED_KEY_Q;

float x, y, z;
float box_x, box_y, box_z;

// Order of function calling can be seen in "Source/Core/World.cpp::LoopUpdate()"
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/World.cpp

Laborator1::Laborator1()
{
}

Laborator1::~Laborator1()
{
}

void Laborator1::Init()
{
	// Load a mesh from file into GPU memory
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;

		Mesh* mesh1 = new Mesh("sphere");
		mesh1->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh1->GetMeshID()] = mesh1;

		Mesh* mesh2 = new Mesh("teapot");
		mesh2->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "teapot.obj");
		meshes[mesh2->GetMeshID()] = mesh2;

		PRESSED_KEY_F = false;

		PRESSED_KEY_A = PRESSED_KEY_D = false;
		PRESSED_KEY_W = PRESSED_KEY_S = false;
		PRESSED_KEY_E = PRESSED_KEY_Q = false;

		x = 0, z = 0.5f, y = 0;
		box_x = 1, box_y = 0, box_z = 0.5f;
	}
}

void Laborator1::FrameStart()
{

}

void Laborator1::Update(float deltaTimeSeconds)
{
	glm::ivec2 resolution = window->props.resolution;

	// sets the clear color for the color buffer
	if (PRESSED_KEY_F) {
		glClearColor(1, 0, 0, 1);
	} else {
		glClearColor(0, 0, 0, 1);
	}

	// clears the color buffer (using the previously set color) and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);

	// render the object again but with different properties
	if (PRESSED_KEY_F) {
		RenderMesh(meshes["teapot"], glm::vec3(-1, 0.5f, 0));
	} else {
		RenderMesh(meshes["box"], glm::vec3(-1, 0.5f, 0));
	}

	float x_off = 0, y_off = 0, z_off = 0;
	if (PRESSED_KEY_A) {
		x_off -= deltaTimeSeconds;
	}

	if (PRESSED_KEY_D) {
		x_off += deltaTimeSeconds;
	}

	if (PRESSED_KEY_W) {
		y_off += deltaTimeSeconds;
	}

	if (PRESSED_KEY_S) {
		y_off -= deltaTimeSeconds;
	}

	if (PRESSED_KEY_E) {
		z_off += deltaTimeSeconds;
	}

	if (PRESSED_KEY_Q) {
		z_off -= deltaTimeSeconds;
	}

	x += x_off; y += y_off; z += z_off;

	RenderMesh(meshes["sphere"], glm::vec3(x, z, y), glm::vec3(0.5f));

	RenderMesh(meshes["box"], glm::vec3(box_x, box_z, box_y), glm::vec3(0.5f));

}

void Laborator1::FrameEnd()
{
	DrawCoordinatSystem();
}

// Read the documentation of the following functions in: "Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Laborator1::OnInputUpdate(float deltaTime, int mods)
{
	// treat continuous update based on input
	if (window->KeyHold(GLFW_KEY_A)) {
		PRESSED_KEY_A = true;
	}

	if (window->KeyHold(GLFW_KEY_D)) {
		PRESSED_KEY_D = true;
	}

	if (window->KeyHold(GLFW_KEY_W)) {
		PRESSED_KEY_W = true;
	}

	if (window->KeyHold(GLFW_KEY_S)) {
		PRESSED_KEY_S = true;
	}

	if (window->KeyHold(GLFW_KEY_E)) {
		PRESSED_KEY_E = true;
	}

	if (window->KeyHold(GLFW_KEY_Q)) {
		PRESSED_KEY_Q = true;
	}

};

void Laborator1::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_F) {
		// do something
		PRESSED_KEY_F = true;
	}

	if (mods == GLFW_MOD_CONTROL) {
		if (key == GLFW_KEY_A) {
			box_x -= 0.1;
		}

		if (key == GLFW_KEY_D) {
			box_x += 0.1;
		}

		if (key == GLFW_KEY_W) {
			box_y += 0.1;
		}

		if (key == GLFW_KEY_S) {
			box_y -= 0.1;
		}

		if (key == GLFW_KEY_E) {
			box_z += 0.1;
		}

		if (key == GLFW_KEY_Q) {
			box_z -= 0.1;
		}
	}
};

void Laborator1::OnKeyRelease(int key, int mods)
{
	// add key release event
	if (key == GLFW_KEY_A) {
		PRESSED_KEY_A = false;
	}

	if (key == GLFW_KEY_D) {
		PRESSED_KEY_D = false;
	}

	if (key == GLFW_KEY_W) {
		PRESSED_KEY_W = false;
	}

	if (key == GLFW_KEY_S) {
		PRESSED_KEY_S = false;
	}

	if (key == GLFW_KEY_E) {
		PRESSED_KEY_E = false;
	}

	if (key == GLFW_KEY_Q) {
		PRESSED_KEY_Q = false;
	}
};

void Laborator1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
};

void Laborator1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
};

void Laborator1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
	// treat mouse scroll event
}

void Laborator1::OnWindowResize(int width, int height)
{
	// treat window resize event
}
