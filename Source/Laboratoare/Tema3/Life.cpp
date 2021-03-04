#include "Life.h"

Life::Life(glm::vec3 centerPoint)
{
    this->centerPoint = centerPoint;
	CreateHeart();
}

Life::~Life()
{
	delete heart;
}

Mesh* Life::GetHeartMesh()
{
	return heart;
}

glm::vec3 Life::GetLifeColor()
{
    return heartColor;
}

glm::vec3 Life::GetCenterPoint()
{
    return centerPoint;
}

float Life::GetWidth(float scale)
{
    return initialWidth * scale;
}

void Life::CreateHeart()
{
    heart = new Mesh("heart");

    std::vector<VertexFormat> vertices;
    std::vector<unsigned short> indices;

    float angle;

    vertices.emplace_back(centerPoint, heartColor);
    indices.push_back(0);

    for (unsigned short i = 1; i < NUM_ITER; i++)
    {
        angle = 2 * PI * i / NUM_ITER;

        vertices.emplace_back(glm::vec3(GetX(angle), GetY(angle), 1), heartColor);
        indices.push_back(i);
    }

    vertices.emplace_back(glm::vec3(GetX(0), GetY(0), 1), heartColor);
    indices.push_back(NUM_ITER);

    heart->SetDrawMode(GL_TRIANGLE_FAN);
    heart->InitFromData(vertices, indices);
}

float Life::GetY(float angle)
{
    return 13 * cos(angle) - 5 * cos (2 * angle) - 2 * cos(3 * angle) - cos(4 * angle) + centerPoint.y;
}

float Life::GetX(float angle)
{
    return 16 * pow(sin(angle), 3) + centerPoint.x;
}
