#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>
#include "Arrow.cpp"
#include "Bow.cpp"
#include "Balloon.cpp"
#include "Shuriken.cpp"
#include "PowerBar.cpp"
#include "HealthBar.cpp"

class Tema1 : public SimpleScene
{
	public:
		Tema1();
		~Tema1();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

	protected:
		glm::mat3 modelMatrix;
		static const int targets = 10;
		float difficulty = 1;

		Arrow* arrow;
		float rotate_arrow = 0;
		float speed_arrow = 0;

		Bow* bow;
		float rotate_bow = 0;
		int health = 3;
		int score = 0;
		int timer = 0;

		Balloon* balloons[targets];
		float offset_balloon_y = -120;

		Shuriken* shurikens[targets];
		float angle = 0;

		PowerBar* powerbar;
		float scale_powerbar = 1;

		HealthBar* healthbar;
		float scale_healthbar = 1;
};