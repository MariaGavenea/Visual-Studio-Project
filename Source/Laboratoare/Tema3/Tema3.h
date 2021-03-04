#pragma once
#include <Component/SimpleScene.h>
#include "Laboratoare/Laborator5/LabCamera.h"
#include "Laboratoare/Laborator4/Transform3D.h"
#include <Laboratoare\Tema3\Platform.h>
#include <Laboratoare\Tema3\Player.h>
#include <Laboratoare\Tema3\PlatformInfo.h>
#include <Laboratoare\Tema3\PlatformGenerator.h>
#include <Laboratoare\Tema3\Fuel.h>
#include <Laboratoare\Tema3\Life.h>
#include <TextRenderer/TextRenderer.h>

#define CAMERA_ANGLE 0.5
#define FALL_HEIGHT -0.5
#define INITIAL_NUM_PLATFORM_ROWS 9
#define MIN_CHECK_LINES 3

#define RED_TIME 3
#define ORANGE_TIME 3
#define GREEN_TIME 7
#define YELLOW_TIME 3

#define EFFECT_NONE 0
#define EFFECT_RED_PLATFORM 1
#define EFFECT_ORANGE_PLATFORM 2
#define EFFECT_YELLOW_PLATFORM 3
#define EFFECT_GREEN_PLATFORM 4

#define MAX_LIVES 3

#define LEFT_BOUND -1.1
#define RIGHT_BOUND 1.1

// apa
#define NUM_WATER 10
#define WATER_LENGTH 2
#define WATER_WIDTH 2.5

// iarba
#define NUM_GRASS 10
#define GRASS_LENGTH 2.0
#define GRASS_WIDTH 2.0
#define GRASS_OX_START_OFFSET (WATER_WIDTH / 2 + GRASS_WIDTH / 2)
#define NUM_GRASS_ONE_SIDE 2


// background
#define BACKGROUND_XY_SCALE 2 * (GRASS_OX_START_OFFSET - GRASS_WIDTH / 2 + NUM_GRASS_ONE_SIDE * GRASS_WIDTH)
#define BACKGROUND_OZ_SCALE 20
#define BACKGROUND_OY_OFFSET 4.5
#define BACKGROUND_OZ_OFFSET -9

// brazi
#define NUM_TREES 5
#define TREE_DIAMETER 1
#define TREE_DISTANCE 3
#define TREE_OX_OFFSET 1.75

// trunchi
#define TRUNK_HEIGHT 1.5
#define TRUNK_DIAMETER 0.25
#define TRUNK_OY_OFFSET (TRUNK_HEIGHT / 2)

// coroana de jos
#define TREE_DOWN_CONE_HEIGHT 1.0
#define TREE_DOWN_CONE_DIAMETER 1.0
#define TREE_DOWN_CIRCLE_OY_OFFSET 0.25
#define TREE_DOWN_CONE_OY_OFFSET (TREE_DOWN_CONE_HEIGHT / 2 + TREE_DOWN_CIRCLE_OY_OFFSET)

// coroana din mijloc
#define TREE_MIDDLE_CONE_HEIGHT 0.75
#define TREE_MIDDLE_CONE_DIAMETER 0.75
#define TREE_MIDDLE_CONE_OY_OFFSET (TREE_DOWN_CONE_OY_OFFSET + TREE_DOWN_CONE_HEIGHT / 4)
#define TREE_MIDDLE_CIRCLE_OY_OFFSET (TREE_MIDDLE_CONE_OY_OFFSET - TREE_MIDDLE_CONE_HEIGHT / 2)

// coroana de sus
#define TREE_UP_CONE_HEIGHT 0.5
#define TREE_UP_CONE_DIAMETER 0.5
#define TREE_UP_CONE_OY_OFFSET (TREE_MIDDLE_CONE_OY_OFFSET + TREE_DOWN_CONE_HEIGHT / 4)
#define TREE_UP_CIRCLE_OY_OFFSET (TREE_UP_CONE_OY_OFFSET - TREE_UP_CONE_HEIGHT / 2)

// lumina
#define MAX_LIGHTS 20
#define MAX_SIMPLE_LIGHTS 10
#define SIMPLE_LIGHT_HEIGHT (TREE_UP_CIRCLE_OY_OFFSET + TREE_UP_CONE_HEIGHT)
#define SPOT_LIGHT_HEIGHT 1.4
#define SPOT_LIGHT_ROTATION_STEP RADIANS(0.5)
#define LIGHT_SCALE 0.1

// stalpi
#define POLE_OZ_OFFSET_FROM_TREE (-TREE_DISTANCE / 2)

#define POLE_CONE_SCALE 0.2
#define POLE_CONE_HALF_SCALE (POLE_CONE_SCALE / 2)
#define POLE_CONE_LIGHT_OY_DIST (POLE_CONE_HALF_SCALE + LIGHT_SCALE / 2)
#define POLE_CONE_HEIGHT (SPOT_LIGHT_HEIGHT + POLE_CONE_LIGHT_OY_DIST)

#define POLE_VERTICAL_XZ_SCALE 0.1
#define POLE_VERTICAL_OY_SCALE 1.5
#define POLE_VERTICAL_HEIGHT (POLE_VERTICAL_OY_SCALE / 2)
#define POLE_VERTICAL_OX_OFFSET 1.75

#define POLE_HORIZ_YZ_SCALE 0.1
#define POLE_HORIZ_OX_SCALE 1.0
#define POLE_HORIZ_OX_OFFSET (POLE_VERTICAL_OX_OFFSET - POLE_HORIZ_OX_SCALE / 2 + POLE_HORIZ_YZ_SCALE)


// obstacole (pietre)
#define STONE_SCALE 0.3
#define STONE_OY_OFFSET (PLATFORM_OY_SCALE / 2 +  STONE_SCALE / 2)
#define STONE_RADIUS (STONE_SCALE / 2)
#define STONE_KE_FACTOR 0.5

// colectabile (conuri)
#define PINECONE_OY_SCALE 0.3
#define PINECONE_OY_OFFSET (PLATFORM_OY_SCALE / 2 +  PINECONE_OY_SCALE / 2)
#define PINECONE_XZ_SCALE 0.1
#define PINECONE_RADIUS (PINECONE_OY_SCALE + PINECONE_XZ_SCALE) / 2

#define PINECONE_POINT 1

class Tema3 : public SimpleScene
{
public:
	Tema3();
	~Tema3();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void RenderMesh2D(Mesh* mesh, Shader* shader, const glm::mat3& modelMatrix, const glm::vec3& color);
	void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color);
	void RenderSimpleMeshText(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture, int mappingType);
	void RenderPlatformsWithObjects();
	void RenderStone(glm::mat4 platformModelMatTrans);
	void RenderPineCone(glm::mat4 platformModelMatTrans);
	void RenderPlayerFalling(float deltaTimeSeconds);
	void RenderFuelBar();
	void RenderPlayer();
	void RenderLife();
	void RenderTreesAndPoles();
	void RenderTree(float side, float z);
	void RenderLightsAndPoles();
	void RenderWater();
	void RenderGrass();
	void RenderBackground();

	void CreateSquare();
	void CreateCone();
	void CreateCube();
	void CreateSphere();

	void AddNewPlatforms();
	void CheckPlayerOnPlatform(glm::vec3 absPlayerPosition);
	void CheckPlatformEffect(glm::vec3 color);
	void CheckPlatformLife(glm::vec3 color);
	void ComputeAndCheckFuel(float deltaTimeSeconds);
	void CheckCollisionWithObjectsJump(glm::vec3 absPlayerPosition);
	bool CheckCollisionSphereSphere(glm::vec3 centerPos1, float radius1, glm::vec3 centerPos2, float radius2);

	int GetCollisionType(glm::vec3 absPlayerPosition, glm::vec3 absPlatformPosition, float length);
	glm::mat4 GetPlayerModelMatrix();

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

protected:
	Laborator::Camera* camera;
	Laborator::Camera* camera2D;
	glm::mat4 projectionMatrix, orthoMatrix;
	bool renderCameraTarget;

	float zNear, zFar;
	float fov;

	Platform* platform;
	Player* player;
	Fuel* fuel;
	Life* life;

	std::vector<PlatformInfo*>* platforms[NUM_PLATFORMS_ROW];
	std::vector<std::pair<int, float>*>* configurations; // <configuratie, lungime platforma>

	PlatformGenerator* platformGenerator;

	bool gameOver;
	bool stopFalling, startFalling;
	bool outOfFuel;

	glm::vec3 absNearestPlatform;
	float nearestPlatformLength;

	float effectTime;
	int effectType;

	int numLives;

	std::unordered_map<std::string, Texture2D*> mapTextures;
	float water_OZ_offset, grass_OZ_offset, tree_OZ_offset;

	glm::vec3 lightPosition[MAX_LIGHTS] = {glm::vec3(0, 0, 0)};
	glm::vec3 lightDirection[MAX_LIGHTS] = { glm::vec3(0, 0, 0) };
	unsigned int materialShininess;

	int isSpotlight[MAX_LIGHTS] = {0};
	float cut_off;
	float angleRotationSpotlight = 0;
	int angleUp = 1;

	glm::vec3 Ke, Ka, Kd, Ks;
	glm::vec3 Isursa[MAX_LIGHTS] = { glm::vec3(0, 0, 0) }, Ia;
	int numLights, numLightsShown = 0;
	int numStarLights = 0;

	int treeWithLightSide = -1;
	int lightPoleSide = -1;

	bool hitByObstacle = false;

	float coneRotateAngle = 0;
	int score = 0;
	TextRenderer* Text;
	glm::ivec2 textPosition;
};
