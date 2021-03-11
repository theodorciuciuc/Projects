#include <Core/Engine.h>

class Balloon {
public:
	Mesh* string;
	Mesh* body;
	glm::vec3 color;
	float x, y;
	bool destroyed;
	float scale;

	Balloon(glm::vec3 color) {
		x = 400.f;
		y = -250.f;
		destroyed = true;
		this->color = color;
		scale = 0;
		//string
		std::vector<VertexFormat> vertices = {
			VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)),
			VertexFormat(glm::vec3(-20, 20, 0), glm::vec3(0, 0, 0)),
			VertexFormat(glm::vec3(20, 40, 0), glm::vec3(0, 0, 0)),
			VertexFormat(glm::vec3(0 , 60, 0), glm::vec3(0, 0, 0)),
		};
		std::vector<unsigned short> indices = { 0, 1, 2, 3 };
		string = new Mesh("string");
		string->InitFromData(vertices, indices);
		string->SetDrawMode(GL_LINE_STRIP);

		//body
		vertices.clear();
		indices.clear();
		for (int i = 0; i < 360; i++) {
			float angle = 2 * M_PI * i / 360;
			float x = 50 * cos(angle);
			float y = 50 * sin(angle);
			vertices.push_back(VertexFormat(glm::vec3(x, y, 0), color));
			indices.push_back(i);
		}
		body = new Mesh("body");
		body->InitFromData(vertices, indices);
		body->SetDrawMode(GL_POLYGON);
	}

	bool is_hit(float x, float y) {
		float dist = sqrt((this->x - x) * (this->x - x) + (this->y - y) * (this->y - y));
		if (dist <= 20 + 62.5f) {
			destroyed = true;
			return true;
		}
		return false;
	}

	void reset() {
		x = (rand() % (1200 - 400 + 1)) + 400;
		y = -250.f;
		destroyed = false;
		scale = 1;
	}
};