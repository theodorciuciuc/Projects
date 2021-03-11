#include <Core/Engine.h>

class Platform {
public:
	Mesh* body;
	float x, y, z;
	float scale_x, scale_y, scale_z;
	glm::vec3 color;

	Platform(float x, float y, float z, float scale_x, float scale_y, float scale_z, glm::vec3 color) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->scale_x = scale_x;
		this->scale_y = scale_y;
		this->scale_z = scale_z;
		this->color = color;
		//body
		body = new Mesh("body");
		body->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
	}

	bool player_landed(float x, float y, float z) {
		float margin_error = 0.1f;
		if (this->z + 0.5f * scale_z + margin_error >= z && this->z - 0.5f * scale_z <= z + margin_error &&
			this->x + 0.5f * scale_x + margin_error >= x && this->x - 0.5f * scale_x <= x + margin_error &&
			y == 0.5f) {
			color = glm::vec3(1, 0, 1);
			return true;
		}
		return false;
	}
};