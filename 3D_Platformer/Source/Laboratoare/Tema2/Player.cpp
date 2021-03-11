#include <Core/Engine.h>

class Player {
public:
	Mesh* body;
	float x, y, z, scale;
	glm::vec3 color;

	Player() {
		x = 0;
		y = 0.5f;
		z = 0;
		scale = 1;
		color = glm::vec3(0.5f, 0.5f, 0.5f);
		//body
		body = new Mesh("body");
		body->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
	}
};