#include <Core/Engine.h>

class PowerBar {
public:
	Mesh* body;
	float x, y;

	PowerBar() {
		x = 50.f;
		y = 160.f;
		//body
		std::vector<VertexFormat> vertices = {
			VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1, 0, 1)),
			VertexFormat(glm::vec3(0, 40, 0), glm::vec3(1, 0, 1)),
			VertexFormat(glm::vec3(1, 40, 0), glm::vec3(1, 0, 1)),
			VertexFormat(glm::vec3(1 , 0, 0), glm::vec3(1, 0, 1)),
		};
		std::vector<unsigned short> indices = { 0, 1, 2, 2, 3, 0 };
		body = new Mesh("body");
		body->InitFromData(vertices, indices);
	}
};