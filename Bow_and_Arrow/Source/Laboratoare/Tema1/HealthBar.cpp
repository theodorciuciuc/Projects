#include <Core/Engine.h>

class HealthBar {
public:
	Mesh* body;
	float x, y;

	HealthBar() {
		x = 50.f;
		y = 500.f;
		//body
		std::vector<VertexFormat> vertices = {
			VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0)),
			VertexFormat(glm::vec3(0, 40, 0), glm::vec3(1, 0, 0)),
			VertexFormat(glm::vec3(100, 40, 0), glm::vec3(1, 0, 0)),
			VertexFormat(glm::vec3(100 , 0, 0), glm::vec3(1, 0, 0)),
		};
		std::vector<unsigned short> indices = { 0, 1, 2, 2, 3, 0 };
		body = new Mesh("body");
		body->InitFromData(vertices, indices);
	}
};