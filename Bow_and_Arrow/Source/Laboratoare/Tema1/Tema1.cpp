#include "Tema1.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform2D.h"

using namespace std;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	arrow = new Arrow();
	bow = new Bow();
	powerbar = new PowerBar();
	healthbar = new HealthBar();
	for (int i = 0; i < targets; i++) {
		if (i < targets * 2 / 3) {
			balloons[i] = new Balloon(glm::vec3(1, 0, 0));
		}
		else {
			balloons[i] = new Balloon(glm::vec3(1, 1, 0));
		}
		shurikens[i] = new Shuriken();
	}
}

void Tema1::FrameStart()
{
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{	
	glLineWidth(3);
	timer++;
	difficulty += 0.001f;
	if (timer % 60 == 0) {
		cout << "Your score is: " << score << endl;
	}

	//arrow
	if (arrow->x > 1280 || arrow->y > 720 || arrow->y < 0) {
		arrow->fired = false;
		speed_arrow = 0;
		scale_powerbar = 1;
		arrow->x = bow->x;
		arrow->y = bow->y - 3;
		rotate_arrow = rotate_bow;
	}
	if (arrow->fired) {
		arrow->x += deltaTimeSeconds * speed_arrow * cos(rotate_arrow);
		arrow->y += deltaTimeSeconds * speed_arrow * sin(rotate_arrow);
	}
	modelMatrix = Transform2D::Translate(arrow->x, arrow->y) *
		 		  Transform2D::Rotate(rotate_arrow);
	RenderMesh2D(arrow->body, shaders["VertexColor"], modelMatrix);

	//bow
	modelMatrix = Transform2D::Translate(bow->x, bow->y) * Transform2D::Rotate(rotate_bow);
	RenderMesh2D(bow->body, shaders["VertexColor"], modelMatrix);

	//powerbar
	modelMatrix = Transform2D::Translate(powerbar->x, powerbar->y) * Transform2D::Scale(scale_powerbar, 1);
	RenderMesh2D(powerbar->body, shaders["VertexColor"], modelMatrix);

	//healthbar
	modelMatrix = Transform2D::Translate(healthbar->x, healthbar->y) * Transform2D::Scale(scale_healthbar, 1);
	RenderMesh2D(healthbar->body, shaders["VertexColor"], modelMatrix);

	//balloon spawn and draw logic
	for (int i = 0; i < targets; i++) {
		if (balloons[i]->y > 900) {
			balloons[i]->destroyed = true;
			balloons[i]->scale = 0;
		}
		if (!balloons[i]->destroyed) {
			balloons[i]->y += 100 * deltaTimeSeconds * difficulty;
			balloons[i]->x += sin(Engine::GetElapsedTime());
			modelMatrix = Transform2D::Translate(balloons[i]->x, balloons[i]->y) * Transform2D::Scale(1, 1.25f);
			RenderMesh2D(balloons[i]->body, shaders["VertexColor"], modelMatrix);
			modelMatrix = Transform2D::Translate(balloons[i]->x, balloons[i]->y + offset_balloon_y);
			RenderMesh2D(balloons[i]->string, shaders["VertexColor"], modelMatrix);
		}
		else if (balloons[i]->scale > 0.25f) {
			balloons[i]->scale -= 10 * deltaTimeSeconds;
			modelMatrix = Transform2D::Translate(balloons[i]->x, balloons[i]->y) * Transform2D::Scale(balloons[i]->scale, 
																									  balloons[i]->scale);
			RenderMesh2D(balloons[i]->body, shaders["VertexColor"], modelMatrix);
		}
		else {
			if (rand() % 1000 < difficulty) {
				balloons[i]->reset();
			}
		}
		//arrow hits a balloon
		if (!balloons[i]->destroyed && balloons[i]->is_hit(arrow->x + 160 * cos(rotate_arrow),
			arrow->y + 160 * sin(rotate_arrow))) {
			if (balloons[i]->color == glm::vec3(1, 0, 0)) {
				score++;
			}
			else {
				score--;
			}
		}
	}

	//shuriken spawn and draw logic
	angle += deltaTimeSeconds * 5;
	for (int i = 0; i < targets; i++) {
		if (shurikens[i]->x < -100) {
			shurikens[i]->destroyed = true;
			shurikens[i]->scale = 0;
		}
		if (!shurikens[i]->destroyed) {
			shurikens[i]->x -= 100 * deltaTimeSeconds * difficulty;
			modelMatrix = Transform2D::Translate(shurikens[i]->x, shurikens[i]->y) * Transform2D::Rotate(angle);
			RenderMesh2D(shurikens[i]->body, shaders["VertexColor"], modelMatrix);
		}
		else if (shurikens[i]->scale > 0.25f) {
			shurikens[i]->scale -= 5 * deltaTimeSeconds;
			modelMatrix = Transform2D::Translate(shurikens[i]->x, shurikens[i]->y) * Transform2D::Rotate(angle) *
						  Transform2D::Scale(shurikens[i]->scale, shurikens[i]->scale);
			RenderMesh2D(shurikens[i]->corpse, shaders["VertexColor"], modelMatrix);
		}
		else {
			if (rand() % 1000 < difficulty) {
				shurikens[i]->reset();
			}
		}
		//shuriken hits the player
		if (!shurikens[i]->destroyed && !shurikens[i]->hit && shurikens[i]->hit_player(bow->x, bow->y)) {
			health--;
			scale_healthbar -= 0.33f;
			if (health == 0) {
				cout << "You lost!" << endl;
				window->Close();
			}
		}
		//arrow hits a shuriken
		if (arrow->fired && !shurikens[i]->destroyed && shurikens[i]->is_hit(arrow->x + 160 * cos(rotate_arrow),
			arrow->y + 160 * sin(rotate_arrow))) {
			score++;
		}
	}
}

void Tema1::FrameEnd()
{
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
	if (window->KeyHold(GLFW_KEY_W)) {
		bow->y += 200 * deltaTime;
		if (!arrow->fired) {
			arrow->y += 200 * deltaTime;
		}
		powerbar->y += 200 * deltaTime;
		healthbar->y += 200 * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_S)) {
		bow->y -= 200 * deltaTime;
		if (!arrow->fired) {
			arrow->y -= 200 * deltaTime;
		}
		powerbar->y -= 200 * deltaTime;
		healthbar->y -= 200 * deltaTime;
	}
	if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT) && speed_arrow < 1000 && !arrow->fired) {
		speed_arrow += 600 * deltaTime;
		scale_powerbar += 50 * deltaTime;
	}
}

void Tema1::OnKeyPress(int key, int mods)
{
}

void Tema1::OnKeyRelease(int key, int mods)
{
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	if (!arrow->fired) {
		rotate_arrow = atan2((720 - mouseY) - arrow->y, mouseX - arrow->x);
		if (mouseY < 360 && rotate_arrow > M_PI_2) {
			rotate_arrow = M_PI_2;
		}
		if (mouseY > 360 && rotate_arrow < -M_PI_2) {
			rotate_arrow = -M_PI_2;
		}
	}
	rotate_bow = atan2((720 - mouseY) - bow->y, mouseX - bow->x);
	if (mouseY < 360 && rotate_bow > M_PI_2) {
		rotate_bow = M_PI_2;
	}
	if (mouseY > 360 && rotate_bow < -M_PI_2) {
		rotate_bow = -M_PI_2;
	}
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	if (!window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) {
		arrow->fired = true;
	}
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}