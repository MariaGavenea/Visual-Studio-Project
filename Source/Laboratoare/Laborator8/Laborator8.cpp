#include "Laborator8.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Laborator8::Laborator8()
{
}

Laborator8::~Laborator8()
{
}

void Laborator8::Init()
{
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("plane");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "plane50.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader *shader = new Shader("ShaderLab8");
		shader->AddShader("Source/Laboratoare/Laborator8/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Laborator8/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		Shader* shader = new Shader("ShaderLab8Bonus");
		shader->AddShader("Source/Laboratoare/Laborator8/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Laborator8/Shaders/FragmentShaderBonus.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	//Light & material properties
	{
		lightPosition = glm::vec3(0, 1, 1);
		lightDirection = glm::vec3(0, -1, 0);
		materialShininess = 30;
		materialKd = 0.5;
		materialKs = 0.5;

		// bonus 1
		isSpotlight = 0;
		cut_off = 30;
		angleOX = angleOY = 0;

		// bonus 2
		Ke = glm::vec3(0, 0, 1);
		Ka = glm::vec3(0.7, 0.7, 0.5);
		Kd = glm::vec3(1, 0.7, 0.5);
		Ks = glm::vec3(0.2, 0.7, 0.5);

		Isursa = glm::vec3(1, 0.9, 0.9);
		Ia = glm::vec3(0.25, 0.25, 0.3);

	}

	{
		vector<glm::vec3> vertices
		{
			glm::vec3(-0.5, -0.5, 0.5), // 0
			glm::vec3(0.5, -0.5, 0.5), // 1
			glm::vec3(0.5, 0.5, 0.5), // 2
			glm::vec3(-0.5, 0.5, 0.5), // 3

			glm::vec3(0.5, -0.5, 0.5), // 4
			glm::vec3(0.5, -0.5, -0.5), // 5
			glm::vec3(0.5, 0.5, -0.5), // 6
			glm::vec3(0.5, 0.5, 0.5), // 7

			glm::vec3(0.5, -0.5, -0.5), // 8
			glm::vec3(-0.5, -0.5, -0.5), // 9
			glm::vec3(-0.5, 0.5, -0.5), // 10
			glm::vec3(0.5, 0.5, -0.5), // 11

			glm::vec3(-0.5, -0.5, -0.5), // 12
			glm::vec3(-0.5, -0.5, 0.5), // 13
			glm::vec3(-0.5, 0.5, 0.5), // 14
			glm::vec3(-0.5, 0.5, -0.5), // 15

			glm::vec3(-0.5, 0.5, 0.5), // 16
			glm::vec3(0.5, 0.5, 0.5), // 17
			glm::vec3(0.5, 0.5, -0.5), // 18
			glm::vec3(-0.5, 0.5, -0.5), // 19

			glm::vec3(-0.5, -0.5, 0.5), // 20
			glm::vec3(0.5, -0.5, 0.5), // 21
			glm::vec3(0.5, -0.5, -0.5), // 22
			glm::vec3(-0.5, -0.5, -0.5), // 23
		};

		vector<unsigned short> indices =
		{
			// fata 1
			0, 1, 2,
			0, 2, 3,

			// fata 2
			4, 5, 6,
			4, 6, 7,

			// fata 3
			8, 9, 10,
			8, 10, 11,

			// fata 4
			12, 13, 14,
			12, 14, 15,

			// fata 5
			16, 17, 18,
			16, 18, 19,

			// fata 6
			20, 22, 21,
			20, 23, 22

		};

		vector<glm::vec3> normals
		{
			glm::vec3(0, 0, 1),
			glm::vec3(0, 0, 1),
			glm::vec3(0, 0, 1),
			glm::vec3(0, 0, 1),

			glm::vec3(1, 0, 0),
			glm::vec3(1, 0, 0),
			glm::vec3(1, 0, 0),
			glm::vec3(1, 0, 0),

			glm::vec3(0, 0, -1),
			glm::vec3(0, 0, -1),
			glm::vec3(0, 0, -1),
			glm::vec3(0, 0, -1),

			glm::vec3(-1, 0, 0),
			glm::vec3(-1, 0, 0),
			glm::vec3(-1, 0, 0),
			glm::vec3(-1, 0, 0),

			glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 0),

			glm::vec3(0, -1, 0),
			glm::vec3(0, -1, 0),
			glm::vec3(0, -1, 0),
			glm::vec3(0, -1, 0)
		};

		// TODO : Complete texture coordinates for the square
		vector<glm::vec2> textureCoords
		{
			glm::vec2(0.f, 1.f),
			glm::vec2(1.f, 1.f),
			glm::vec2(1.f, 0.f),
			glm::vec2(0.f, 0.f),

			glm::vec2(0.f, 1.f),
			glm::vec2(1.f, 1.f),
			glm::vec2(1.f, 0.f),
			glm::vec2(0.f, 0.f),

			glm::vec2(0.f, 1.f),
			glm::vec2(1.f, 1.f),
			glm::vec2(1.f, 0.f),
			glm::vec2(0.f, 0.f),

			glm::vec2(0.f, 1.f),
			glm::vec2(1.f, 1.f),
			glm::vec2(1.f, 0.f),
			glm::vec2(0.f, 0.f),

			glm::vec2(0.f, 1.f),
			glm::vec2(1.f, 1.f),
			glm::vec2(1.f, 0.f),
			glm::vec2(0.f, 0.f),

			glm::vec2(0.f, 1.f),
			glm::vec2(1.f, 1.f),
			glm::vec2(1.f, 0.f),
			glm::vec2(0.f, 0.f)
		};

		Mesh* mesh = new Mesh("mycube");
		mesh->InitFromData(vertices, normals, textureCoords, indices);
		meshes[mesh->GetMeshID()] = mesh;

	}
}

void Laborator8::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);	
}

void Laborator8::Update(float deltaTimeSeconds)
{
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0));
		//RenderSimpleMesh(meshes["sphere"], shaders["ShaderLab8"], modelMatrix);

		// Bonus 2
		Ke = glm::vec3(1, 0, 0);
		Ka = glm::vec3(0.5, 0.5, 0.7);
		Kd = glm::vec3(0.5, 0.7, 1);
		Ks = glm::vec3(0.5, 0.7, 0.2);

		Ia = glm::vec3(0.3, 0.25, 0.25);

		RenderSimpleMeshBonus(meshes["sphere"], shaders["ShaderLab8Bonus"], modelMatrix);
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(2, 0.5f, 0));
		//modelMatrix = glm::rotate(modelMatrix, RADIANS(60.0f), glm::vec3(1, 0, 0));
		//modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
		//RenderSimpleMesh(meshes["box"], shaders["ShaderLab8"], modelMatrix);

		// Bonus 2
		Ke = glm::vec3(0, 0.2, 0.7);
		Ka = glm::vec3(0.2, 0.5, 0.5);
		Kd = glm::vec3(1, 0.5, 0.5);
		Ks = glm::vec3(0.2, 0.5, 0.5);

		Ia = glm::vec3(0.2, 0.2, 0.2);

		RenderSimpleMeshBonus(meshes["mycube"], shaders["ShaderLab8Bonus"], modelMatrix);
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-2, 0.5f, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(60.0f), glm::vec3(1, 1, 0));
		//RenderSimpleMesh(meshes["box"], shaders["ShaderLab8"], modelMatrix, glm::vec3(0, 0.5, 0));

		// Bonus 2
		Ke = glm::vec3(0, 1, 0);
		Ka = glm::vec3(0.2, 0.5, 0.7);
		Kd = glm::vec3(1, 0.5, 0.7);
		Ks = glm::vec3(0.2, 0.5, 0.7);

		Ia = glm::vec3(0.25, 0.3, 0.25);

		RenderSimpleMeshBonus(meshes["box"], shaders["ShaderLab8Bonus"], modelMatrix);
	}

	// Render ground
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.01f, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f));
		//RenderSimpleMesh(meshes["plane"], shaders["ShaderLab8"], modelMatrix);

		// Bonus 2
		Ke = glm::vec3(0, 0, 1);
		Ka = glm::vec3(0.7, 0.7, 0.5);
		Kd = glm::vec3(1, 0.7, 0.5);
		Ks = glm::vec3(0.2, 0.7, 0.5);

		Ia = glm::vec3(0.25, 0.25, 0.3);

		RenderSimpleMeshBonus(meshes["plane"], shaders["ShaderLab8Bonus"], modelMatrix);
	}

	// Render the point light in the scene
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, lightPosition);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
		RenderMesh(meshes["sphere"], shaders["Simple"], modelMatrix);
	}
}

void Laborator8::FrameEnd()
{
	DrawCoordinatSystem();
}

void Laborator8::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, const glm::vec3 &color)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Set shader uniforms for light & material properties
	// TODO: Set light position uniform
	int light_position = glGetUniformLocation(shader->program, "light_position");
	glUniform3f(light_position, lightPosition.x, lightPosition.y, lightPosition.z);

	int light_direction = glGetUniformLocation(shader->program, "light_direction");
	glUniform3f(light_direction, lightDirection.x, lightDirection.y, lightDirection.z);

	// TODO: Set eye position (camera position) uniform
	glm::vec3 eyePosition = GetSceneCamera()->transform->GetWorldPosition();
	int eye_position = glGetUniformLocation(shader->program, "eye_position");
	glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

	// TODO: Set material property uniforms (shininess, kd, ks, object color) 
	int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(material_shininess, materialShininess);

	int material_kd = glGetUniformLocation(shader->program, "material_kd");
	glUniform1f(material_kd, materialKd);

	int material_ks = glGetUniformLocation(shader->program, "material_ks");
	glUniform1f(material_ks, materialKs);

	int object_color = glGetUniformLocation(shader->program, "object_color");
	glUniform3f(object_color, color.r, color.g, color.b);

	glUniform1i(glGetUniformLocation(shader->program, "is_spotlight"), isSpotlight);
	glUniform1f(glGetUniformLocation(shader->program, "cut_off_angle"), cut_off);

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Laborator8::RenderSimpleMeshBonus(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Set shader uniforms for light & material properties
	// TODO: Set light position uniform
	int light_position = glGetUniformLocation(shader->program, "light_position");
	glUniform3f(light_position, lightPosition.x, lightPosition.y, lightPosition.z);

	int light_direction = glGetUniformLocation(shader->program, "light_direction");
	glUniform3f(light_direction, lightDirection.x, lightDirection.y, lightDirection.z);

	// TODO: Set eye position (camera position) uniform
	glm::vec3 eyePosition = GetSceneCamera()->transform->GetWorldPosition();
	int eye_position = glGetUniformLocation(shader->program, "eye_position");
	glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

	// TODO: Set material property uniforms (shininess, kd, ks, object color) 
	int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(material_shininess, materialShininess);

	glUniform3f(glGetUniformLocation(shader->program, "Ke"), Ke.x, Ke.y, Ke.z);
	glUniform3f(glGetUniformLocation(shader->program, "Ka"), Ka.x, Ka.y, Ka.z);
	glUniform3f(glGetUniformLocation(shader->program, "Kd"), Kd.x, Kd.y, Kd.z);
	glUniform3f(glGetUniformLocation(shader->program, "Ks"), Ks.x, Ks.y, Ks.z);

	glUniform3f(glGetUniformLocation(shader->program, "Isursa"), Isursa.x, Isursa.y, Isursa.z);
	glUniform3f(glGetUniformLocation(shader->program, "Ia"), Ia.x, Ia.y, Ia.z);

	glUniform1i(glGetUniformLocation(shader->program, "is_spotlight"), isSpotlight);
	glUniform1f(glGetUniformLocation(shader->program, "cut_off_angle"), cut_off);

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Laborator8::OnInputUpdate(float deltaTime, int mods)
{
	float speed = 2;

	if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		glm::vec3 up = glm::vec3(0, 1, 0);
		glm::vec3 right = GetSceneCamera()->transform->GetLocalOXVector();
		glm::vec3 forward = GetSceneCamera()->transform->GetLocalOZVector();
		forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));

		// Control light position using on W, A, S, D, E, Q
		if (window->KeyHold(GLFW_KEY_W)) lightPosition -= forward * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_A)) lightPosition -= right * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_S)) lightPosition += forward * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_D)) lightPosition += right * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_E)) lightPosition += up * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_Q)) lightPosition -= up * deltaTime * speed;

		// Bonus 1
		if (!isSpotlight)
			return;
	
		if (window->KeyHold(GLFW_KEY_J) && (cut_off < 90)) {
			cut_off += deltaTime * speed * 10;

			if (cut_off > 90) {
				cut_off = 90;
			}
		}

		if (window->KeyHold(GLFW_KEY_H) && cut_off > 0) {
			cut_off -= deltaTime * speed * 10;

			if (cut_off < 0) {
				cut_off = 0;
			}
		}

		if (window->KeyHold(GLFW_KEY_UP))
		{
			angleOX += deltaTime * speed;
		}
		if (window->KeyHold(GLFW_KEY_DOWN))
		{
			angleOX -= deltaTime * speed;
		}
		if (window->KeyHold(GLFW_KEY_LEFT))
		{
			angleOY += deltaTime * speed;
		}
		if (window->KeyHold(GLFW_KEY_RIGHT))
		{
			angleOY -= deltaTime * speed;
		}

		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::rotate(modelMatrix, angleOY, glm::vec3(0, 1, 0));
		modelMatrix = glm::rotate(modelMatrix, angleOX, glm::vec3(1, 0, 0));

		lightDirection = glm::vec3(0, -1, 0);
		lightDirection = glm::vec3(modelMatrix * glm::vec4(lightDirection, 1));
	}
}

void Laborator8::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_F) {
		isSpotlight = !isSpotlight;
	}
}

void Laborator8::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator8::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Laborator8::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator8::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator8::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator8::OnWindowResize(int width, int height)
{
}
