#include <Core/Engine.h>
#include <iostream>

class Shuriken {
public:
	Mesh* body;
	Mesh* corpse;
	float x, y;
	bool hit;
	bool destroyed;
	float scale;

	Shuriken() {
		x = 1400.f;
		y = 500.f;
		hit = false;
		destroyed = true;
		scale = 0;
		//body
		std::vector<VertexFormat> vertices = {
			VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)),
			VertexFormat(glm::vec3(0, 50, 0), glm::vec3(0, 0, 0)),
			VertexFormat(glm::vec3(-50, 50, 0), glm::vec3(0, 0, 0)),
			VertexFormat(glm::vec3(-50 , 0, 0), glm::vec3(0, 0, 0)),
			VertexFormat(glm::vec3(-50, -50, 0), glm::vec3(0, 0, 0)),
			VertexFormat(glm::vec3(0, -50, 0), glm::vec3(0, 0, 0)),
			VertexFormat(glm::vec3(50, -50, 0), glm::vec3(0, 0, 0)),
			VertexFormat(glm::vec3(50, 0, 0), glm::vec3(0, 0, 0)),
			VertexFormat(glm::vec3(50, 50, 0), glm::vec3(0, 0, 0)),
		};
		std::vector<unsigned short> indices = { 0, 1, 2, 0, 3, 4, 0, 5, 6, 0, 7, 8 };
		body = new Mesh("body");
		body->InitFromData(vertices, indices);
		//corpse
		vertices = {
			VertexFormat(glm::vec3(0, 20, 0), glm::vec3(0, 0, 0)), //0
			VertexFormat(glm::vec3(0, 50, 0), glm::vec3(0, 0, 0)), //1
			VertexFormat(glm::vec3(-50, 50, 0), glm::vec3(0, 0, 0)), //2
			VertexFormat(glm::vec3(-50 , 0, 0), glm::vec3(0, 0, 0)), //3
			VertexFormat(glm::vec3(-50, -50, 0), glm::vec3(0, 0, 0)), //4
			VertexFormat(glm::vec3(0, -50, 0), glm::vec3(0, 0, 0)), //5
			VertexFormat(glm::vec3(50, -50, 0), glm::vec3(0, 0, 0)), //6
			VertexFormat(glm::vec3(50, 0, 0), glm::vec3(0, 0, 0)), //7
			VertexFormat(glm::vec3(50, 50, 0), glm::vec3(0, 0, 0)), //8
			VertexFormat(glm::vec3(-20, 0, 0), glm::vec3(0, 0, 0)), //9
			VertexFormat(glm::vec3(0, -20, 0), glm::vec3(0, 0, 0)), //10
			VertexFormat(glm::vec3(20, 0, 0), glm::vec3(0, 0, 0)), //11
		};
		indices = { 0, 1, 2, 9, 3, 4, 10, 5, 6, 11, 7, 8 };
		corpse = new Mesh("corpse");
		corpse->InitFromData(vertices, indices);
	}

	bool hit_player(float x, float y) {
		float dist = sqrt((this->x - x) * (this->x - x) + (this->y - y) * (this->y - y));
		if (dist <= 100 + 50) {
			hit = true;
			return true;
		}
		return false;
	}

	bool is_hit(float x, float y) {
		float dist = sqrt((this->x - x) * (this->x - x) + (this->y - y) * (this->y - y));
		if (dist <= 20 + 50) {
			destroyed = true;
			return true;
		}
		return false;
	}

	void reset() {
		x = 1400.f;
		y = (rand() % (650 - 100 + 1)) + 100;
		hit = false;
		destroyed = false;
		scale = 1;
	}
};