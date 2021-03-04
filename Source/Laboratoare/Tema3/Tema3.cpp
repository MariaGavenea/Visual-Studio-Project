#include "Tema3.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>
#include <Laboratoare\Laborator3\Transform2D.h>

using namespace std;

Tema3::Tema3()
{
}

Tema3::~Tema3()
{
}

void Tema3::Init()
{
	renderCameraTarget = true;

	// 2D
	glm::ivec2 resolution = window->GetResolution();
	camera2D = new Laborator::Camera();
	camera2D->Set(glm::vec3(0, 0, 50.f), glm::vec3(0, 0, 25.f), glm::vec3(0, 1.f, 0));

	orthoMatrix = glm::ortho(0.f, (float)resolution.x, 0.f, (float)resolution.y, 0.01f, 400.f);

	
	// 3D
	camera = new Laborator::Camera();
	camera->Set(glm::vec3(0, PLAYER_OY_OFFSET, 1.f), glm::vec3(0, PLAYER_OY_OFFSET, 0), glm::vec3(0, 1.f, 0));
	camera->RotateThirdPerson_OX(-CAMERA_ANGLE);

	// matricea de proiectie
	fov = RADIANS(60);
	zNear = 0.01f;
	zFar = 200.0f;

	projectionMatrix = glm::perspective(fov, window->props.aspectRatio, zNear, zFar);

	// instantieri
	platform = new Platform();
	player = new Player();

	platformGenerator = new PlatformGenerator();
	configurations = new std::vector<std::pair<int, float>*>();

	// configuratia initiala/primul rand de platforme
	configurations->push_back(new std::pair<int, float>(3, 2.f)); // 3 platforme bune, de lungime 2

	for (int i = 0; i < NUM_PLATFORMS_ROW; i++) {
		glm::mat4 modelMatrixTranslate = glm::mat4(1);
		modelMatrixTranslate *= Transform3D::Translate(i-1, PLATFORM_OY_OFFSET, -1);

		platforms[i] = new std::vector<PlatformInfo*>();
		platforms[i]->push_back(new PlatformInfo(modelMatrixTranslate)); // platforme albastre, de lungime 2
	}

	for (int i = 0; i < INITIAL_NUM_PLATFORM_ROWS - 1; i++) {
		AddNewPlatforms();
	}

	// variabile folosite pentru cadere si game over
	gameOver = stopFalling = startFalling = outOfFuel = false;
	absNearestPlatform = glm::vec3(0, 0, 0);
	nearestPlatformLength = 0;

	// vieti
	numLives = 0;

	// efecte
	effectTime = 0;
	effectType = EFFECT_NONE;

	// combustibil
	float length = resolution.x / 8;
	float height = resolution.y / 16;

	fuel = new Fuel(height, length, glm::vec3(height / 2, resolution.y - height, 1));

	life = new Life(glm::vec3(3 * resolution.x / 4, resolution.y - 3 * height / 2, 1));

	textPosition = glm::ivec2(3 * resolution.x / 4 - life->GetWidth(HEART_SCALE) / 2, height / 2);

	// textures
	const string textureLoc = "Source/Laboratoare/Tema3/Textures/";

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "grass1.jpg").c_str(), GL_REPEAT);
		mapTextures["grass"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "water13.jpg").c_str(), GL_REPEAT);
		mapTextures["water"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "pinetree.jpg").c_str(), GL_REPEAT);
		mapTextures["tree"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "trunk.jpg").c_str(), GL_REPEAT);
		mapTextures["trunk"] = texture;
	}

	
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "wood.jpg").c_str(), GL_REPEAT);
		mapTextures["wood"] = texture;
	} 

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "metal.jpg").c_str(), GL_REPEAT);
		mapTextures["metal"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "background5.jpg").c_str(), GL_REPEAT);
		mapTextures["background"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "stone.jpg").c_str(), GL_REPEAT);
		mapTextures["stone"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "pinecone4.png").c_str(), GL_REPEAT);
		mapTextures["pinecone"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "hedgehog.jpg").c_str(), GL_REPEAT);
		mapTextures["hedgehog"] = texture;
	}
	
	// Create meshes
	CreateSquare();
	CreateCone();
	CreateCube();
	CreateSphere();

	water_OZ_offset = grass_OZ_offset = 0;
	tree_OZ_offset = -1;

	{
		Shader* shader = new Shader("ShaderTema2");
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		Shader* shader = new Shader("ShaderTema3");
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/VertexShaderTextures.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/FragmentShaderTextures.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	//Light & material properties
	{
		materialShininess = 30;

		cut_off = 30;
		angleRotationSpotlight = 0;

		Ke = glm::vec3(0, 0, 0);
		Ka = glm::vec3(0.25, 0.25, 0.25);
		Kd = glm::vec3(0.5, 0.5, 0.5);
		Ks = glm::vec3(0.5, 0.5, 0.5);

		Ia = glm::vec3(1, 1, 1);

		numLights = 0;
	}


	// Text renderer
	Text = new TextRenderer(resolution.x, resolution.y);

	Text->Load("Source/TextRenderer/Fonts/Arial.ttf", 18);
}

void Tema3::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema3::Update(float deltaTimeSeconds)
{
	if (stopFalling) {
		if (outOfFuel) {
			std::cout << "Out of fuel.\n";
		}

		std::cout << "GAME OVER\n";
		std::exit(0);
	}

	// afisare scor
	Text->RenderText("Score: " + to_string(score), textPosition.x, textPosition.y, 1.0f, glm::vec3(1.0, 1.0, 1.0));

	// afisare obiecte decor, platforme si background
	RenderTreesAndPoles();
	RenderPlatformsWithObjects();
	RenderWater();
	RenderGrass();
	RenderBackground();

	if (gameOver) {
		RenderFuelBar();
		RenderLife();

		if (startFalling) {
			// afisare jucator in cadere
			RenderPlayerFalling(deltaTimeSeconds);
			return;
		}

		if (effectType == EFFECT_RED_PLATFORM) {
			// afisare jucator asupra caruia se aplica efectul platformei rosii
			glm::mat4 modelMatrix = GetPlayerModelMatrix();

			if (renderCameraTarget) {
				RenderPlayer();
			}

			effectTime += deltaTimeSeconds;

			if (effectTime > RED_TIME) {
				std::cout << "GAME OVER\n";
				std::exit(0);
			}

			return;
		}

		if (outOfFuel && (player->GetPlayerSpeed() == 0)) {
			// jucatorul a ramas fara combustibil si s-a oprit
			std::cout << "Out of fuel.\n";
			std::cout << "GAME OVER\n";
			std::exit(0);
		}

		if (hitByObstacle) {
			std::cout << "Hit by obstacle.\n";
			std::cout << "GAME OVER\n";
			std::exit(0);
		}
	}

	// stergere platforme care au iesit din imagine si adaugare de platforme noi
	glm::vec3 absFirstLeftPlatform = glm::vec3(platforms[0]->front()->GetModelMatrixTranslate() * glm::vec4(0, 0, 0, 1));
	if (absFirstLeftPlatform.z - configurations->front()->second / 2 >= camera->position.z) {
		// stergere platforme
		for (int i = 0; i < NUM_PLATFORMS_ROW; i++) {
			platforms[i]->erase(platforms[i]->begin());
		}

		configurations->erase(configurations->begin());

		// adaugare platforme
		AddNewPlatforms();
	}


	// jucator - viteza pe OZ
	if (outOfFuel) {
		// a ramas fara combustibil, deci va incetini
		player->DecSpeedUntilStop(deltaTimeSeconds);
		camera->MoveForward(player->GetPlayerSpeed() * deltaTimeSeconds);
	} else {
		if (effectType == EFFECT_ORANGE_PLATFORM) {
			// se blocheaza jucatorul la o viteza apropiata de cea maxima pentru o perioada de timp
			camera->MoveForward(PLAYER_MED_SPEED * deltaTimeSeconds);

			effectTime += deltaTimeSeconds;

			if (effectTime > ORANGE_TIME) {
				effectTime = 0;
				effectType = EFFECT_NONE;
			}
		} else {
			camera->MoveForward(player->GetPlayerSpeed() * deltaTimeSeconds);
		}
	}

	// jucator - saritura
	if (player->IsJumping()) {
		float step = player->ComputeJumpStep(PLAYER_OY_SPEED * deltaTimeSeconds);
		camera->TranslateUpword(step);
	}

	glm::vec3 absPlayerPosition = camera->position;

	if (renderCameraTarget)
	{
		// afisare sfera (jucator)
		RenderPlayer();

		glm::mat4 modelMatrix = GetPlayerModelMatrix();
		absPlayerPosition = glm::vec3(modelMatrix * glm::vec4(0, 0, 0, 1));
	}

	if (!player->IsJumping()) {
		// verificare daca jucatorul este pe o platforma, iar daca da verifica si coliziunea cu obstacolul/colectabilul de pe ea
		CheckPlayerOnPlatform(absPlayerPosition);
	}
	else {
		CheckCollisionWithObjectsJump(absPlayerPosition);
	}

	if (!outOfFuel) {
		ComputeAndCheckFuel(deltaTimeSeconds);
		RenderFuelBar();
		RenderLife();
	}

}

void Tema3::FrameEnd()
{
	//DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema3::RenderMesh2D(Mesh* mesh, Shader* shader, const glm::mat3& modelMatrix, const glm::vec3& color)
{
	if (!mesh || !shader || !shader->program)
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);
	glUniform3fv(glGetUniformLocation(shader->program, "object_color"), 1, glm::value_ptr(color));

	// Bind model matrix
	glm::mat3 mm = modelMatrix;
	glm::mat4 model = glm::mat4(
		mm[0][0], mm[0][1], mm[0][2], 0.f,
		mm[1][0], mm[1][1], mm[1][2], 0.f,
		0.f, 0.f, mm[2][2], 0.f,
		mm[2][0], mm[2][1], 0.f, 1.f);

	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(model));

	// Bind view matrix
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera2D->GetViewMatrix()));

	// Bind projection matrix
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(orthoMatrix));

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{

	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);
	glUniform3fv(glGetUniformLocation(shader->program, "object_color"), 1, glm::value_ptr(color));

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	int location4 = glGetUniformLocation(shader->program, "time");
	glUniform1f(location4, Engine::GetElapsedTime());

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema3::RenderSimpleMeshText(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture, int mappingType)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	glUniform1i(glGetUniformLocation(shader->program, "mapping_type"), mappingType);

	if (texture)
	{
		//TODO : activate texture location 0
		glActiveTexture(GL_TEXTURE0);

		//TODO : Bind the texture1 ID
		glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());

		//TODO : Send texture uniform value
		glUniform1i(glGetUniformLocation(shader->program, "texture"), 0);
	}

	// TODO: Set light position uniform
	int light_position = glGetUniformLocation(shader->program, "light_position");
	glUniform3fv(light_position, MAX_LIGHTS, &lightPosition[0][0]);

	int light_direction = glGetUniformLocation(shader->program, "light_direction");
	glUniform3fv(light_direction, MAX_LIGHTS, &lightDirection[0][0]);

	// TODO: Set eye position (camera position) uniform
	glm::vec3 eyePosition = camera->position;
	int eye_position = glGetUniformLocation(shader->program, "eye_position");
	glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

	// TODO: Set material property uniforms (shininess, kd, ks, object color) 
	int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(material_shininess, materialShininess);

	glUniform3f(glGetUniformLocation(shader->program, "Ke"), Ke.x, Ke.y, Ke.z);
	glUniform3f(glGetUniformLocation(shader->program, "Ka"), Ka.x, Ka.y, Ka.z);
	glUniform3f(glGetUniformLocation(shader->program, "Kd"), Kd.x, Kd.y, Kd.z);
	glUniform3f(glGetUniformLocation(shader->program, "Ks"), Ks.x, Ks.y, Ks.z);

	glUniform3fv(glGetUniformLocation(shader->program, "Isursa"), MAX_LIGHTS, &Isursa[0][0]);
	glUniform3f(glGetUniformLocation(shader->program, "Ia"), Ia.x, Ia.y, Ia.z);

	glUniform1iv(glGetUniformLocation(shader->program, "is_spotlight"), MAX_LIGHTS, isSpotlight);
	glUniform1f(glGetUniformLocation(shader->program, "cut_off_angle"), cut_off);

	glUniform1i(glGetUniformLocation(shader->program, "num_lights"), numLightsShown);

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema3::RenderPlatformsWithObjects()
{
	bool firstPineCone = true;
	for (int i = 0; i < NUM_PLATFORMS_ROW; i++) {
		for (int j = 0; j < platforms[i]->size(); j++) {
			PlatformInfo* platformInfo = platforms[i]->at(j);

			// daca exista platforma, o afisez
			if (platformInfo->GetColor() != glm::vec3(0, 0, 0)) {
				glm::mat4 modelMatrix = platformInfo->GetModelMatrixTranslate();
				modelMatrix *= Transform3D::Scale(PLATFORM_WIDTH, PLATFORM_OY_SCALE, configurations->at(j)->second);

				// afisare obstacol
				if (platformInfo->HasObstacle()) {
					RenderStone(platformInfo->GetModelMatrixTranslate());
				}

				// afisare colectabil
				if (platformInfo->HasPineCone()) {
					if (firstPineCone) {
						// rotire con
						coneRotateAngle += 0.05;
						if (coneRotateAngle == 360) {
							coneRotateAngle = 0;
						}

						firstPineCone = false;
					}

					RenderPineCone(platformInfo->GetModelMatrixTranslate());
				}

				// afisare platforma
				glm::vec3 auxKe = Ke;
				auxKe = Ke;
				Ke = platformInfo->GetColor();
				RenderSimpleMeshText(platform->GetPlatformMesh(), shaders["ShaderTema3"],
					modelMatrix, mapTextures["wood"], 0);
				Ke = auxKe;
			}
		}
	}
}

void Tema3::RenderStone(glm::mat4 platformModelMatTrans)
{
	glm::mat4 modelMatrix1 = glm::mat4(1);
	modelMatrix1 *= Transform3D::Translate(0, STONE_OY_OFFSET, 0);
	modelMatrix1 *= platformModelMatTrans;
	modelMatrix1 *= Transform3D::Scale(STONE_SCALE, STONE_SCALE, STONE_SCALE);
	modelMatrix1 *= Transform3D::RotateOY(RADIANS(180));

	glm::vec3 auxKe = Ke;
	Ke = glm::vec3(STONE_KE_FACTOR, STONE_KE_FACTOR, STONE_KE_FACTOR);
	RenderSimpleMeshText(meshes["mysphere"], shaders["ShaderTema3"], modelMatrix1, mapTextures["stone"], 0);
	Ke = auxKe;
}

void Tema3::RenderPineCone(glm::mat4 platformModelMatTrans)
{
	glm::mat4 modelMatrix1 = glm::mat4(1);
	modelMatrix1 *= Transform3D::Translate(0, PINECONE_OY_OFFSET, 0);
	modelMatrix1 *= platformModelMatTrans;
	modelMatrix1 *= Transform3D::RotateOY(coneRotateAngle);
	modelMatrix1 *= Transform3D::Scale(PINECONE_XZ_SCALE, PINECONE_OY_SCALE, PINECONE_XZ_SCALE);
	RenderSimpleMeshText(meshes["mysphere"], shaders["ShaderTema3"], modelMatrix1, mapTextures["pinecone"], 0);
}

// jucatorul va cadea pana la o inaltime prestabilita
void Tema3::RenderPlayerFalling(float deltaTimeSeconds)
{
	camera->TranslateUpword(-0.5 * deltaTimeSeconds);
	glm::vec3 absPlayerPosition = camera->position;

	if (renderCameraTarget)
	{
		// calculez pozitia sferei (jucatorului in third person)
		glm::mat4 modelMatrix = GetPlayerModelMatrix();
		absPlayerPosition = glm::vec3(modelMatrix * glm::vec4(0, 0, 0, 1));
	}

	// daca a ajuns partial pe platforma si e instabil
	if (nearestPlatformLength) {
		// verific daca trebuie sa il misc pe OZ ca sa cada pe langa platforma (si nu prin ea)
		float distOZ = abs(absPlayerPosition.z - absNearestPlatform.z);
		float minDistOZ = nearestPlatformLength / 2 + PLAYER_RADIUS;

		if (distOZ < minDistOZ && distOZ > nearestPlatformLength / 2) {
			if (absPlayerPosition.z > absNearestPlatform.z) {
				camera->MoveForward(distOZ - minDistOZ);
			}
			else {
				camera->MoveForward(minDistOZ - distOZ);
			}
		}
		
		// verific daca trebuie sa il misc pe OX ca sa cada pe langa platforma (si nu prin ea)
		float distOX = abs(absPlayerPosition.x - absNearestPlatform.x);
		float minDistOX = PLATFORM_WIDTH / 2 + PLAYER_RADIUS;

		if (distOX <= minDistOX && distOX >= PLATFORM_WIDTH / 2) {
			if (absPlayerPosition.x > absNearestPlatform.x) {
				camera->TranslateRight(minDistOX - distOX);
			}
			else {
				camera->TranslateRight(distOX - minDistOX);
			}
		}

		// setez valoarea pe 0 deoarece nu va mai fi nevoie sa mut jucatorul (pe OZ sau OX)
		nearestPlatformLength = 0;
	}

	if (absPlayerPosition.y < FALL_HEIGHT) {
		stopFalling = true;
		// camera->position.y = FALL_HEIGHT;
	}

	if (renderCameraTarget) {
		// afisez sfera cu noile coordonate calculate
		RenderPlayer();
	}

	
}

void Tema3::RenderFuelBar()
{
	glm::mat3 modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(fuel->GetLeftMiddlePoint().x, fuel->GetLeftMiddlePoint().y);
	modelMatrix *= Transform2D::Scale(fuel->GetScaleFactor(), 1);
	modelMatrix *= Transform2D::Translate(-fuel->GetLeftMiddlePoint().x, -fuel->GetLeftMiddlePoint().y);

	RenderMesh2D(fuel->GetFuelMesh(), shaders["ShaderTema2"], modelMatrix, fuel->GetFuelColor());
	RenderMesh2D(fuel->GetBackgroundMesh(), shaders["ShaderTema2"], glm::mat3(1), fuel->GetBackgroundColor());
}

void Tema3::RenderPlayer()
{
	if (effectType == EFFECT_NONE) {
		RenderSimpleMeshText(player->GetPlayerMesh(), shaders["ShaderTema3"], GetPlayerModelMatrix(), mapTextures["hedgehog"], 0);
		return;
	}

	glm::vec3 auxKe = Ke;
	Ke = glm::vec3(0.4, 0.4, 0.4) * player->GetEffectColor();
	RenderSimpleMeshText(player->GetPlayerMesh(), shaders["ShaderTema3"], GetPlayerModelMatrix(), mapTextures["hedgehog"], 0);
	Ke = auxKe;
}

void Tema3::RenderLife()
{
	
	for (int i = 0; i < numLives && i < MAX_LIVES; i++) {
		float heartWidth = life->GetWidth(HEART_SCALE);
		float OX_offset = 3 * i * heartWidth / 2;

		glm::mat3 modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(life->GetCenterPoint().x + OX_offset, life->GetCenterPoint().y);
		modelMatrix *= Transform2D::Scale(HEART_SCALE, HEART_SCALE);
		modelMatrix *= Transform2D::Translate(-life->GetCenterPoint().x, -life->GetCenterPoint().y);

		RenderMesh2D(life->GetHeartMesh(), shaders["ShaderTema2"], modelMatrix, life->GetLifeColor());
	}
}

void Tema3::RenderTreesAndPoles()
{
	// se parcurg toate liniile cu brazi
	for (int i = 0; i <= NUM_TREES; i++) {
		if (camera->position.z <= tree_OZ_offset - TREE_DIAMETER) {
			tree_OZ_offset -= TREE_DISTANCE;
		}

		float z = tree_OZ_offset - i * TREE_DISTANCE;

		// pe o singura linie, de ambele parti ale apei
		for (int side = -1; side <= 1; side += 2) {
			// MAX_SIMPLE_LIGHTS lumini punctiforme in varful brazilor
			if ((numLights == 0 && z < lightPosition[MAX_LIGHTS - 1].z) 
				|| (numLights > 0 && numLights < MAX_LIGHTS && numStarLights < MAX_SIMPLE_LIGHTS && z < lightPosition[numLights - 1].z)) {
				lightPosition[numLights] = glm::vec3(TREE_OX_OFFSET * treeWithLightSide, SIMPLE_LIGHT_HEIGHT, z);
				lightDirection[numLights] = glm::vec3(0, -1, 0);
				isSpotlight[numLights] = 0;
				Isursa[numLights] = colorsVector[numLights % COLORS_VECTOR_NUM]; // culoare lumina
				numLights++;
				numStarLights++;

				treeWithLightSide *= -1; // alternare pozitie brad cu lumina de partea cealalta a apei

				if (numLightsShown != MAX_LIGHTS) {
					numLightsShown = numLights;
				}
			}

			// MAX_LIGHTS - MAX_SIMPLE_LIGHTS lumini de tip spot ale stalpilor de iluminat
			if (numStarLights == MAX_SIMPLE_LIGHTS) {
				if (numLights > 0 && numLights < MAX_LIGHTS && z < lightPosition[numLights - 1].z) {
					lightPosition[numLights] = glm::vec3(lightPoleSide, SPOT_LIGHT_HEIGHT, z + POLE_OZ_OFFSET_FROM_TREE);
					lightDirection[numLights] = glm::vec3(0, -1, 0);
					isSpotlight[numLights] = 1;
					Isursa[numLights] = colorsVector[numLights % COLORS_VECTOR_NUM]; // culoare lumina
					numLights++;

					lightPoleSide *= -1; // alternare pozitie stalp de partea cealalta a apei

					if (numLightsShown != MAX_LIGHTS) {
						numLightsShown = numLights;
					}
				}
			}

			if (numLights == MAX_LIGHTS) {
				numLights = 0;
				numStarLights = 0;
			}

			RenderTree(side, z);
		}
	}

	RenderLightsAndPoles();
}

void Tema3::RenderTree(float side, float z)
{
	// coroana/con de sus
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(TREE_OX_OFFSET * side, TREE_UP_CONE_OY_OFFSET, z);
		modelMatrix *= Transform3D::Scale(TREE_UP_CONE_DIAMETER, TREE_UP_CONE_HEIGHT, TREE_UP_CONE_DIAMETER);
		RenderSimpleMeshText(meshes["mycone"], shaders["ShaderTema3"], modelMatrix, mapTextures["tree"], 1);

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(TREE_OX_OFFSET * side, TREE_UP_CIRCLE_OY_OFFSET, z);
		modelMatrix *= Transform3D::Scale(TREE_UP_CONE_DIAMETER, 1, TREE_UP_CONE_DIAMETER);
		RenderSimpleMeshText(meshes["circle"], shaders["ShaderTema3"], modelMatrix, mapTextures["tree"], 0);
	}

	// coroana/con din mijloc
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(TREE_OX_OFFSET * side, TREE_MIDDLE_CONE_OY_OFFSET, z);
		modelMatrix *= Transform3D::Scale(TREE_MIDDLE_CONE_DIAMETER, TREE_MIDDLE_CONE_HEIGHT, TREE_MIDDLE_CONE_DIAMETER);
		RenderSimpleMeshText(meshes["mycone"], shaders["ShaderTema3"], modelMatrix, mapTextures["tree"], 1);

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(TREE_OX_OFFSET * side, TREE_MIDDLE_CIRCLE_OY_OFFSET, z);
		modelMatrix *= Transform3D::Scale(TREE_MIDDLE_CONE_DIAMETER, 1, TREE_MIDDLE_CONE_DIAMETER);
		RenderSimpleMeshText(meshes["circle"], shaders["ShaderTema3"], modelMatrix, mapTextures["tree"], 0);
	}

	// coroana/con de jos
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(TREE_OX_OFFSET * side, TREE_DOWN_CONE_OY_OFFSET, z);
		modelMatrix *= Transform3D::Scale(TREE_DOWN_CONE_DIAMETER, TREE_DOWN_CONE_HEIGHT, TREE_DOWN_CONE_DIAMETER);
		RenderSimpleMeshText(meshes["mycone"], shaders["ShaderTema3"], modelMatrix, mapTextures["tree"], 1);

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(TREE_OX_OFFSET * side, TREE_DOWN_CIRCLE_OY_OFFSET, z);
		modelMatrix *= Transform3D::Scale(TREE_DOWN_CONE_DIAMETER, 1, TREE_DOWN_CONE_DIAMETER);
		RenderSimpleMeshText(meshes["circle"], shaders["ShaderTema3"], modelMatrix, mapTextures["tree"], 0);
	}

	// trunchi
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(TREE_OX_OFFSET * side, TRUNK_OY_OFFSET, z);
		modelMatrix *= Transform3D::Scale(TRUNK_DIAMETER, TRUNK_HEIGHT, TRUNK_DIAMETER);
		RenderSimpleMeshText(meshes["mycone"], shaders["ShaderTema3"], modelMatrix, mapTextures["trunk"], 1);
	}
}

void Tema3::RenderLightsAndPoles()
{

	bool isFirstSpot = true;
	for (int i = 0; i < numLightsShown; i++)
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(lightPosition[i].x, lightPosition[i].y, lightPosition[i].z);

		if (isSpotlight[i]) {
			if (isFirstSpot) {
				if (angleUp) {
					// crestere unghi
					if (angleRotationSpotlight < RADIANS(45)) {
						angleRotationSpotlight += SPOT_LIGHT_ROTATION_STEP;
					}
					else {
						angleRotationSpotlight -= SPOT_LIGHT_ROTATION_STEP;
						angleUp = 0;
					}
				}
				else {
					// scadere unghi
					if (angleRotationSpotlight > 0) {
						angleRotationSpotlight -= SPOT_LIGHT_ROTATION_STEP;
					}
					else {
						angleRotationSpotlight += SPOT_LIGHT_ROTATION_STEP;
						angleUp = 1;
					}
				}

				isFirstSpot = false;
			}
			glm::mat4 modelMatrix1 = glm::mat4(1);

			int aux = 1;
			if (lightPosition[i].x > 0) {
				aux = -1;
			}

			modelMatrix1 *= Transform3D::RotateOY(angleRotationSpotlight);
			modelMatrix1 *= Transform3D::RotateOZ(aux * angleRotationSpotlight);
			modelMatrix1 *= Transform3D::RotateOX(aux * angleRotationSpotlight);

			lightDirection[i] = glm::vec3(0, -1, 0);
			lightDirection[i] = glm::vec3(modelMatrix1 * glm::vec4(lightDirection[i], 1));

			// afisare stalp

			// afisare parte verticala
			glm::mat4 modelMatrix2 = glm::mat4(1);
			modelMatrix2 *= Transform3D::Translate(aux * -POLE_VERTICAL_OX_OFFSET, POLE_VERTICAL_HEIGHT, lightPosition[i].z);
			modelMatrix2 *= Transform3D::Scale(POLE_VERTICAL_XZ_SCALE, POLE_VERTICAL_OY_SCALE, POLE_VERTICAL_XZ_SCALE);
			RenderSimpleMeshText(meshes["mycube"], shaders["ShaderTema3"], modelMatrix2, mapTextures["trunk"], 1);

			// afisare parte orizonatala
			modelMatrix2 = glm::mat4(1);
			modelMatrix2 *= Transform3D::Translate(aux * -POLE_HORIZ_OX_OFFSET, POLE_CONE_HEIGHT, lightPosition[i].z);
			modelMatrix2 *= Transform3D::Scale(POLE_HORIZ_OX_SCALE, POLE_HORIZ_YZ_SCALE, POLE_HORIZ_YZ_SCALE);
			RenderSimpleMeshText(meshes["mycube"], shaders["ShaderTema3"], modelMatrix2, mapTextures["trunk"], 0);

			// afisare con
			modelMatrix2 = glm::mat4(1);
			modelMatrix2 *= Transform3D::Translate(lightPosition[i].x, POLE_CONE_HEIGHT, lightPosition[i].z);
			modelMatrix2 *= modelMatrix1; // rotatie con
			modelMatrix2 *= Transform3D::Translate(0, -POLE_CONE_HALF_SCALE, 0);
			modelMatrix2 *= Transform3D::Scale(POLE_CONE_SCALE, POLE_CONE_SCALE, POLE_CONE_SCALE);
			RenderSimpleMeshText(meshes["mycone"], shaders["ShaderTema3"], modelMatrix2, mapTextures["metal"], 1);

			modelMatrix *= Transform3D::Translate(0, POLE_CONE_LIGHT_OY_DIST, 0);
			modelMatrix *= modelMatrix1; // rotatie sfera de lumina
			modelMatrix *= Transform3D::Translate(0, -POLE_CONE_LIGHT_OY_DIST, 0);
		}

		modelMatrix *= Transform3D::Scale(LIGHT_SCALE, LIGHT_SCALE, LIGHT_SCALE);

		// afisare lumina
		RenderSimpleMesh(meshes["mysphere"], shaders["ShaderTema2"], modelMatrix, Isursa[i]);
	}
}

void Tema3::RenderWater()
{
	// se parcurg toate liniile cu parcele de apa
	for (int i = 0; i <= NUM_WATER; i++) {
		if (camera->position.z <= water_OZ_offset - WATER_LENGTH) {
			water_OZ_offset -= WATER_LENGTH;
		}

		// se afiseaza o singura parcela per linie
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(0, PLATFORM_OY_OFFSET, water_OZ_offset - i * WATER_LENGTH);
		modelMatrix *= Transform3D::Scale(WATER_WIDTH, 1, WATER_LENGTH);
		modelMatrix *= Transform3D::RotateOX(RADIANS(90));
		RenderSimpleMeshText(meshes["square"], shaders["ShaderTema3"], modelMatrix, mapTextures["water"], 0);

	}
}

void Tema3::RenderGrass()
{
	// se parcurg toate liniile cu parcele de iarba
	for (int i = 0; i <= NUM_GRASS; i++) {
		if (camera->position.z <= grass_OZ_offset - GRASS_LENGTH) {
			grass_OZ_offset -= GRASS_LENGTH;
		}

		// pe o singura linie, de ambele parti ale apei
		for (int side = -1; side <= 1; side += 2) {
			// afiseaza cate NUM_GRASS_ONE_SIDE parcele de iarba pe o parte
			for (int j = 0; j < NUM_GRASS_ONE_SIDE; j++) {
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix *= Transform3D::Translate(side * (GRASS_OX_START_OFFSET + GRASS_WIDTH * j), PLATFORM_OY_OFFSET, grass_OZ_offset - i * GRASS_LENGTH);
				modelMatrix *= Transform3D::Scale(GRASS_WIDTH, 1, GRASS_LENGTH);
				modelMatrix *= Transform3D::RotateOX(RADIANS(90));
				RenderSimpleMeshText(meshes["square"], shaders["ShaderTema3"], modelMatrix, mapTextures["grass"], 0);
			}
		}

	}
}

void Tema3::RenderBackground()
{
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(0, BACKGROUND_OY_OFFSET, camera->position.z + BACKGROUND_OZ_OFFSET);
	modelMatrix *= Transform3D::Scale(BACKGROUND_XY_SCALE, BACKGROUND_XY_SCALE, BACKGROUND_OZ_SCALE);
	RenderSimpleMeshText(meshes["mycube"], shaders["ShaderTema3"], modelMatrix, mapTextures["background"], 0);
}



void Tema3::CreateSquare()
{
	vector<glm::vec3> vertices
	{
		glm::vec3(0.5f,   0.5f, 0.0f),	// Top Right
		glm::vec3(0.5f,  -0.5f, 0.0f),	// Bottom Right
		glm::vec3(-0.5f, -0.5f, 0.0f),	// Bottom Left
		glm::vec3(-0.5f,  0.5f, 0.0f),	// Top Left
	};

	vector<glm::vec3> normals
	{
		glm::vec3(0, 0, -1),
		glm::vec3(0, 0, -1),
		glm::vec3(0, 0, -1),
		glm::vec3(0, 0, -1)
	};

	vector<glm::vec2> textureCoords
	{
		glm::vec2(1.f, 0.f),
		glm::vec2(1.f, 1.f),
		glm::vec2(0.f, 1.f),
		glm::vec2(0.f, 0.f)
	};

	vector<unsigned short> indices =
	{
		0, 1, 3,
		1, 2, 3
	};

	Mesh* mesh = new Mesh("square");
	mesh->InitFromData(vertices, normals, textureCoords, indices);
	meshes[mesh->GetMeshID()] = mesh;
}

void Tema3::CreateCone()
{
	{
		// Create a cone
		vector<glm::vec3> vertices;
		std::vector<unsigned short> indices;
		vector<glm::vec3> normals;
		vector<glm::vec2> textureCoords;
		int num_iter = 50;
		float angle;


		vertices.push_back(glm::vec3(0, 0.5f, 0));
		normals.push_back(glm::vec3(0, 1, 0));
		textureCoords.push_back(glm::vec2(0, 0));

		for (unsigned short i = 0; i <= num_iter; i++)
		{
			angle = 2 * PI * i / num_iter;
			double x = cos(angle) * 0.5;
			double y = -0.5;
			double z = sin(angle) * 0.5;

			vertices.emplace_back(glm::vec3(x, y, z));
			indices.push_back(i);
			float coneAngle = atan(0.5);
			normals.push_back(glm::vec3(cos(coneAngle) * cos(angle), sin(coneAngle), cos(coneAngle) * sin(angle)));

			textureCoords.push_back(glm::vec2(1.0f / (2 * PI) * atan2(x, z), 1 - (y + 0.5)));
		}

		indices.push_back(num_iter + 1);

		Mesh* mesh = new Mesh("mycone");

		mesh->SetDrawMode(GL_TRIANGLE_FAN);

		mesh->InitFromData(vertices, normals, textureCoords, indices);
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		// Create cone base
		vector<glm::vec3> vertices;
		std::vector<unsigned short> indices;
		vector<glm::vec3> normals;
		vector<glm::vec2> textureCoords;
		int num_iter = 50;
		float angle;


		vertices.push_back(glm::vec3(0, 0, 0));
		normals.push_back(glm::vec3(0, 1, 0));
		textureCoords.push_back(glm::vec2(0, 0));

		for (unsigned short i = 0; i <= num_iter; i++)
		{
			angle = 2 * PI * i / num_iter;
			double x = cos(angle) * 0.5;
			double y = 0;
			double z = sin(angle) * 0.5;

			vertices.emplace_back(glm::vec3(x, y, z));
			indices.push_back(i);
			normals.push_back(glm::vec3(0, -1, 0));

			textureCoords.push_back(glm::vec2(x / 2, z / 2));
		}

		indices.push_back(num_iter + 1);

		Mesh* mesh = new Mesh("circle");

		mesh->SetDrawMode(GL_TRIANGLE_FAN);

		mesh->InitFromData(vertices, normals, textureCoords, indices);
		meshes[mesh->GetMeshID()] = mesh;
	}
}

void Tema3::CreateCube()
{
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

void Tema3::CreateSphere()
{
	{ // Creaza un mesh care aproximeaza o sfera centrata in origine
	  // Se calculeaza puncte de pe sfera folosind ecuatiile parametrice ale sferei

	//	x = r * sin(θ)*sin(φ)          0 <= θ < 2π
	//	y = r * cos(φ)                    0 <= φ <= π
	//	z = r * cos(θ)*sin(φ)


		vector<VertexFormat> vertices;

		int columns = 50;
		int rows = 50;

		float radius = 0.5f;

		for (int i = 0; i <= columns; i++) {
			float theta = 2 * M_PI * ((float)i / columns);
			for (int j = 0; j <= rows; j++) {

				float phi = M_PI * ((float)j / rows);

				glm::vec3 pos = glm::vec3(
					radius * std::sin(theta) * std::sin(phi),//x
					radius * std::cos(phi),                  //y   
					radius * std::cos(theta) * std::sin(phi)  //z
				);

				glm::vec2 texcoord = glm::vec2(
					theta / (2 * M_PI), //u
					phi / M_PI          //v
				);

				//glm::vec3 color = glm::vec3(0, 0, 1);
				vertices.push_back(VertexFormat(pos,
					pos, //culoare
					pos,//vectorul normala este chiar pos: vectorul din originea sferei in pos 
					texcoord));
			}
		}


		vector<unsigned short> indices;

		int rows1 = rows + 1;
		int columns1 = columns + 1;

		for (int i = 1; i < columns1; i++) {
			for (int j = 1; j < rows1; j++) {
				indices.push_back((i - 1) * rows1 + j - 1);
				indices.push_back((i - 1) * rows1 + j);
				indices.push_back(i * rows1 + j - 1);

				indices.push_back((i - 1) * rows1 + j);
				indices.push_back(i * rows1 + j - 1);
				indices.push_back(i * rows1 + j);
			}
		}


		Mesh* mesh = new Mesh("mysphere");
		mesh->InitFromData(vertices, indices);
		meshes[mesh->GetMeshID()] = mesh;
	}
}

void Tema3::AddNewPlatforms()
{
	int oldConfigurationNumber, newConfigurationNumber;
	float platformLength, platformDistance;
	int colors[NUM_PLATFORMS_ROW];

	platformGenerator->NextConfiguration(configurations->back()->first, newConfigurationNumber, platformLength, platformDistance, colors);

	for (int j = 0; j < NUM_PLATFORMS_ROW; j++) {
		glm::mat4 modelMatrixTranslate = glm::mat4(1);
		modelMatrixTranslate *= Transform3D::Translate(0, 0, -(platformDistance + configurations->back()->second / 2 + platformLength / 2));
		modelMatrixTranslate *= platforms[j]->back()->GetModelMatrixTranslate();

		glm::vec3 color = platformGenerator->GetColor(colors[j]);
		platforms[j]->push_back(new PlatformInfo(modelMatrixTranslate, color, platformLength));
	}

	configurations->push_back(new std::pair<int, float>(newConfigurationNumber, platformLength));
}

void Tema3::CheckPlayerOnPlatform(glm::vec3 absPlayerPosition)
{
	bool isOnPlatform = false;

	for (int i = 0; i < NUM_PLATFORMS_ROW && !isOnPlatform; i++) {
		for (int j = 0; j < platforms[i]->size() && j < MIN_CHECK_LINES && !isOnPlatform; j++) {
			PlatformInfo* platformInfo = platforms[i]->at(j);

			// daca exista platforma
			if (platformInfo->GetColor() != glm::vec3(0, 0, 0)) {
				glm::mat4 modelMatrix = platformInfo->GetModelMatrixTranslate();
				modelMatrix *= Transform3D::Scale(PLATFORM_WIDTH, PLATFORM_OY_SCALE, configurations->at(j)->second);

				// calculez coordonatele globale ale centrului platformei actuale
				glm::vec3 absPlatformPosition = glm::vec3(modelMatrix * glm::vec4(0, 0, 0, 1));
				float length = configurations->at(j)->second;

				// obtin tipul de coliziune
				int collisionType = GetCollisionType(absPlayerPosition, absPlatformPosition, length);

				if (collisionType == 1) { // pe platforma
					isOnPlatform = true;

					CheckPlatformEffect(platformInfo->GetColor());
					CheckPlatformLife(platformInfo->GetColor());

					platformInfo->SetColor(purple); // colorez mov

					// coliziunea cu obstacole
					if (platformInfo->HasObstacle() && !platformInfo->ObstacleWasHit()) {
						glm::vec3 stonePos = absPlatformPosition + glm::vec3(0, STONE_OY_OFFSET, 0);
						if (CheckCollisionSphereSphere(absPlayerPosition, PLAYER_RADIUS, stonePos, STONE_RADIUS)) {
							platformInfo->SetObstacleHit();

							if (numLives > 0) {
								numLives--;
							}
							else {
								gameOver = true;
								hitByObstacle = true;
							}
						}
					}

					// coliziune cu colectabil
					if (platformInfo->HasPineCone()) {
						glm::vec3 conePos = absPlatformPosition + glm::vec3(0, PINECONE_OY_OFFSET, 0);
						if (CheckCollisionSphereSphere(absPlayerPosition, PLAYER_RADIUS, conePos, PINECONE_RADIUS)) {
							score += PINECONE_POINT;

							platformInfo->SetHasPineCone(false);
						}
					}

					break;
				}

				if (collisionType == 2) { // pe platforma, pozitie instabila (va cadea)
					absNearestPlatform = absPlatformPosition;
					nearestPlatformLength = length;
				}
			}
		}
	}

	if (!isOnPlatform) {
		gameOver = true;
		startFalling = true;

		// daca jucatorul cade, efectele nu se mai aplica
		effectType = EFFECT_NONE;
		effectTime = 0;
	}
}

void Tema3::CheckPlatformEffect(glm::vec3 color)
{
	if (!(color == red || color == orange || color == green || color == yellow)) {
		return;
	}

	if (outOfFuel && (color != red)) {
		// nu mai poate lua abilitati ce au legatura cu combustibilul/viteza de deplasare
		return;
	}

	player->SetEffectColor(color);
	effectTime = 0;

	if (color == red) {
		gameOver = true;
		effectType = EFFECT_RED_PLATFORM;
		return;
	}

	if (color == orange) {
		effectType = EFFECT_ORANGE_PLATFORM;
		return;
	}

	if (color == green) {
		effectType = EFFECT_GREEN_PLATFORM;
		return;
	}

	if (color == yellow) {
		effectType = EFFECT_YELLOW_PLATFORM;
		return;
	}
}

void Tema3::CheckPlatformLife(glm::vec3 color)
{
	if (color == pink) {
		if (numLives < MAX_LIVES) {
			numLives++;
		}
		return;
	}

	if (color == gray) {
		if (numLives > 0) {
			numLives--;
		}
	}
}

void Tema3::ComputeAndCheckFuel(float deltaTimeSeconds)
{
	if (effectType == EFFECT_GREEN_PLATFORM) {
		// va lua combustibil proportional cu viteza jucatorului pentru un anumit timp
		effectTime += deltaTimeSeconds;
		fuel->IncScaleFactor(2 * FUEL_SPEED * player->GetPlayerSpeed() * deltaTimeSeconds);

		if (effectTime > ORANGE_TIME) {
			effectTime = 0;
			effectType = EFFECT_NONE;
		}
	}
	else {
		if (effectType == EFFECT_YELLOW_PLATFORM) {
			// va pierde combustibil proportional cu viteza jucatorului pentru un anumit timp
			effectTime += deltaTimeSeconds;
			fuel->DecScaleFactor(0.5 * FUEL_SPEED * player->GetPlayerSpeed() * deltaTimeSeconds);

			if (effectTime > ORANGE_TIME) {
				effectTime = 0;
				effectType = EFFECT_NONE;
			}
		}

		if (effectType == EFFECT_ORANGE_PLATFORM) {
			// va pierde combustibil proportional cu viteza jucatorului aflat sub efectul platformei oranj
			fuel->DecScaleFactor(FUEL_SPEED * PLAYER_MED_SPEED * deltaTimeSeconds);
		}
		else {
			// va pierde combustibil proportional cu viteza jucatorului pentru un anumit timp
			fuel->DecScaleFactor(FUEL_SPEED * player->GetPlayerSpeed() * deltaTimeSeconds);
		}
	}

	if (fuel->GetScaleFactor() == 0) {
		// daca ramane fara combustibil si are vieti, jucatorul poate continua jocul cu o viata mai putin si cu rezervorul plin
		if (numLives > 0) {
			numLives--;
			fuel->ResetScaleFactor();

			return;
		}

		// jucatorul a ramas fara combustibil si n-a avut nicio viata
		gameOver = true;
		outOfFuel = true;

		// efectele ce au legatura cu combustibilul/viteza sunt anulate
		if (effectType == EFFECT_GREEN_PLATFORM || effectType == EFFECT_YELLOW_PLATFORM
			|| effectType == EFFECT_ORANGE_PLATFORM) {
			effectType = EFFECT_NONE;
			effectTime = 0;
		}
	}
}

void Tema3::CheckCollisionWithObjectsJump(glm::vec3 absPlayerPosition)
{
	bool hitObject = false;
	for (int i = 0; i < NUM_PLATFORMS_ROW && !hitObject; i++) {
		for (int j = 0; j < platforms[i]->size() && j < MIN_CHECK_LINES; j++) {
			PlatformInfo* platformInfo = platforms[i]->at(j);

			// daca exista platforma
			if (platformInfo->GetColor() != glm::vec3(0, 0, 0)) {
				glm::mat4 modelMatrix = platformInfo->GetModelMatrixTranslate();
				modelMatrix *= Transform3D::Scale(PLATFORM_WIDTH, PLATFORM_OY_SCALE, configurations->at(j)->second);

				// calculez coordonatele globale ale centrului platformei actuale
				glm::vec3 absPlatformPosition = glm::vec3(modelMatrix * glm::vec4(0, 0, 0, 1));
				float length = configurations->at(j)->second;

				// coliziunea cu obstacole
				if (platformInfo->HasObstacle() && !platformInfo->ObstacleWasHit()) {
					glm::vec3 stonePos = absPlatformPosition + glm::vec3(0, STONE_OY_OFFSET, 0);
					if (CheckCollisionSphereSphere(absPlayerPosition, PLAYER_RADIUS, stonePos, STONE_RADIUS)) {
						platformInfo->SetObstacleHit();

						if (numLives > 0) {
							numLives--;
						}
						else {
							gameOver = true;
							hitByObstacle = true;
						}

						hitObject = true;
						break;
					}
				}

				// coliziune cu colectabil
				if (platformInfo->HasPineCone()) {
					glm::vec3 conePos = absPlatformPosition + glm::vec3(0, PINECONE_OY_OFFSET, 0);
					if (CheckCollisionSphereSphere(absPlayerPosition, PLAYER_RADIUS, conePos, PINECONE_RADIUS)) {
						score += PINECONE_POINT;

						platformInfo->SetHasPineCone(false);

						hitObject = true;
						break;
					}
				}
			}
		}
	}
}

bool Tema3::CheckCollisionSphereSphere(glm::vec3 centerPos1, float radius1, glm::vec3 centerPos2, float radius2)
{
	float distance = sqrt((centerPos1.x - centerPos2.x) * (centerPos1.x - centerPos2.x) +
		(centerPos1.y - centerPos2.y) * (centerPos1.y - centerPos2.y) +
		(centerPos1.z - centerPos2.z) * (centerPos1.z - centerPos2.z));
	return distance < (radius1 + radius2);

}

int Tema3::GetCollisionType(glm::vec3 absPlayerPosition, glm::vec3 absPlatformPosition, float length)
{
	float distOX = abs(absPlatformPosition.x - absPlayerPosition.x);
	float distOZ = abs(absPlatformPosition.z - absPlayerPosition.z);

	if (distOX <= PLATFORM_WIDTH / 2 && distOZ <= length / 2)
		return 1; // se afla pe placa

	if (distOX < PLATFORM_WIDTH / 2 + PLAYER_RADIUS && distOZ < length / 2 + PLAYER_RADIUS)
		return 2; // se afla partial pe placa, dar tot cade pentru ca e instabil

	return 0; // se afla cu totul in afara placii
}

glm::mat4 Tema3::GetPlayerModelMatrix()
{
	glm::vec3 targetPosition = camera->GetTargetPosition();
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(targetPosition.x, targetPosition.y, targetPosition.z);
	modelMatrix *= Transform3D::Scale(PLAYER_SCALE, PLAYER_SCALE, PLAYER_SCALE);

	return modelMatrix;
}
// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Tema3::OnInputUpdate(float deltaTime, int mods)
{
	float cameraSpeed = 1.f;

	if (gameOver) {
		return;
	}

	// daca a fost pe o platforma oranj, se blocheaza frana si acceleratia
	if (!(effectType == EFFECT_ORANGE_PLATFORM)) {
		if (window->KeyHold(GLFW_KEY_W)) {
			// maresc viteza pe OZ
			player->IncSpeed(deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			// micsorez viteza pe OZ
			player->IncSpeed(-deltaTime);
		}
	}

	if (window->KeyHold(GLFW_KEY_A)) {
		// miscare la stanga
		if (camera->position.x - cameraSpeed * deltaTime >= LEFT_BOUND) {
			camera->TranslateRight(-cameraSpeed * deltaTime);
		}

	}

	if (window->KeyHold(GLFW_KEY_D)) {
		// miscare la dreapta
		if (camera->position.x + cameraSpeed * deltaTime <= RIGHT_BOUND) {
			camera->TranslateRight(cameraSpeed * deltaTime);
		}
	}
	
}

void Tema3::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_C)
	{
		renderCameraTarget = !renderCameraTarget;

		if (renderCameraTarget) {
			// Third person camera
			camera->TranslateForward(-camera->distanceToTarget);
			camera->RotateThirdPerson_OX(-CAMERA_ANGLE);
		}
		else {
			// First person camera
			camera->RotateThirdPerson_OX(CAMERA_ANGLE);
			camera->TranslateForward(camera->distanceToTarget);
		}
	}

	if (key == GLFW_KEY_SPACE) {
		// daca nu s-a terminat jocul (cadere, fara combustibil sau platforma rosie) si nu e in timpul jump-ului
		if (!gameOver && !player->IsJumping()) {
			player->SetJump();
		}
	}
}

void Tema3::OnKeyRelease(int key, int mods)
{
	// add key release event

}

void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema3::OnWindowResize(int width, int height)
{
}
