#include <Core/Engine.h>

class Arrow {
	public:
		Mesh* body;
		float x, y;
		bool fired;
		
		Arrow() {
			x = 50.f;
			y = 357.f;
			fired = false;
			//body
			std::vector<VertexFormat> vertices = {
				VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)),
				VertexFormat(glm::vec3(0, 6, 0), glm::vec3(0, 0, 0)),
				VertexFormat(glm::vec3(150, 6, 0), glm::vec3(0, 0, 0)),
				VertexFormat(glm::vec3(150, 0, 0), glm::vec3(0, 0, 0)),
				VertexFormat(glm::vec3(150, 24, 0), glm::vec3(0, 0, 0)),
				VertexFormat(glm::vec3(180, 3, 0), glm::vec3(0, 0, 0)),
				VertexFormat(glm::vec3(150, -18, 0), glm::vec3(0, 0, 0)),
			};
			std::vector<unsigned short> indices = { 0, 1, 2, 2, 3, 0, 4, 5, 6 };
			body = new Mesh("body");
			body->InitFromData(vertices, indices);
		}
};