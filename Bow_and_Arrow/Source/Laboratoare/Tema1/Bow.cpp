#include <Core/Engine.h>

class Bow {
public:
	Mesh* body;
	float x, y;

	Bow() {
		x = 50.f;
		y = 360.f;
		//body
		std::vector<VertexFormat> vertices;
		std::vector<unsigned short> indices;
		int count = 0;
		for (int i = 0; i < 360; i++) {
			float angle = 2 * AI_MATH_PI * i / 360;
			float x = 100 * cos(angle);
			float y = 100 * sin(angle);
			if (x >= -0.5f) {
				vertices.push_back(VertexFormat(glm::vec3(x, y, 0), glm::vec3(0, 0, 0)));
				indices.push_back(count++);
			}
		}
		body = new Mesh("body");
		body->InitFromData(vertices, indices);
		body->SetDrawMode(GL_LINE_LOOP);
	}
};