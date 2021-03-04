#pragma once

#include <Core/Engine.h>

#define PLATFORM_OY_SCALE 0.125
#define PLATFORM_OY_OFFSET -(PLATFORM_OY_SCALE / 2)
#define PLATFORM_WIDTH 0.4

class Platform {
public:
	Platform();
	~Platform();

	Mesh* GetPlatformMesh();

protected:
	Mesh* platform;
};