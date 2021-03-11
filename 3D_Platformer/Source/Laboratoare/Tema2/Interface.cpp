#include <Core/Engine.h>

class Interface {
public:
	Mesh* body;
	Mesh* fuel;
	float x, y, z, scale;
	glm::vec3 color;
	glm::vec3 fuel_color;

	Interface() {
		x = -3.1;
		y = 2.7;
		z - 0;
		color = glm::vec3(0.5, 0.5, 0.5);
		fuel_color = glm::vec3(1, 1, 0);
		scale = 1;
		//body
		std::vector<VertexFormat> vertices = {
			VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0.5, 0.5, 0.5)),
			VertexFormat(glm::vec3(0, 0.25, 0), glm::vec3(0.5, 0.5, 0.5)),
			VertexFormat(glm::vec3(1, 0.25, 0), glm::vec3(0.5, 0.5, 0.5)),
			VertexFormat(glm::vec3(1, 0, 0), glm::vec3(0.5, 0.5, 0.5)),
		};
		std::vector<unsigned short> indices = { 0, 1, 2, 2, 3, 0 };
		body = new Mesh("body");
		body->InitFromData(vertices, indices);
		//fuel
		vertices = {
			VertexFormat(glm::vec3(0.1, 0, 0), glm::vec3(0.5, 0.5, 0.5)),
			VertexFormat(glm::vec3(0.1, 0.2, 0), glm::vec3(0.5, 0.5, 0.5)),
			VertexFormat(glm::vec3(0.9, 0.2, 0), glm::vec3(0.5, 0.5, 0.5)),
			VertexFormat(glm::vec3(0.9, 0, 0), glm::vec3(0.5, 0.5, 0.5)),
		};
		indices = { 0, 1, 2, 2, 3, 0 };
		fuel = new Mesh("fuel");
		fuel->InitFromData(vertices, indices);
	}
};