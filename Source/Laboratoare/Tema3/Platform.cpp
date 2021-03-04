#include "Platform.h"

Platform::Platform()
{
	platform = new Mesh("box");
	platform->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
}

Platform::~Platform()
{
	delete platform;
}

Mesh* Platform::GetPlatformMesh()
{
	return platform;
}
