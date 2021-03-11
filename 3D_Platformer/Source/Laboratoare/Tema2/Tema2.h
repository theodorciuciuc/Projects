#pragma once
#include <Component/SimpleScene.h>
#include "MyCamera.h"
#include "Player.cpp"
#include "Platform.cpp"
#include "Interface.cpp"

class Tema2 : public SimpleScene
{
	public:
		Tema2();
		~Tema2();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix, glm::vec3 color, float deformation);

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

	protected:
		MyCamera::Camera *camera;
		glm::mat4 projectionMatrix;		
		glm::mat4 modelMatrix;
		
		Player* player;
		float player_speed;
		bool jumping;
		bool first_person;
		bool game_over;
		float deformation;

		Interface* interface;

		Platform* platform;
		Platform* aux;
		std::vector<Platform* > row_1, row_2, row_3;
		float platform_speed, scale, gap, orange_buff;
		int index;
		glm::vec3 colors[12];
		glm::vec3 old_color;
};
