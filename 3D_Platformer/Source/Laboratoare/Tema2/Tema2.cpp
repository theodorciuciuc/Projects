#include "Tema2.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}

void Tema2::Init()
{
	camera = new MyCamera::Camera();
	camera->Set(glm::vec3(0, 3, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader* shader = new Shader("MyShader");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	player = new Player();
	jumping = false;
	player_speed = 0;
	first_person = false;
	game_over = false;
	deformation = 0;

	interface = new Interface();

	platform = new Platform(0, -0.125f, 0, 1, 0.25f, 3, glm::vec3(0, 0, 1));
	row_1.push_back(new Platform(-2, -0.125f, -4, 1, 0.25f, 3, glm::vec3(0, 0, 1)));
	row_2.push_back(new Platform(0, -0.125f, -4, 1, 0.25f, 3, glm::vec3(0, 0, 1)));
	row_3.push_back(new Platform(2, -0.125f, -4, 1, 0.25f, 3, glm::vec3(0, 0, 1)));
	platform_speed = 0;
	orange_buff = 0;
	colors[0] = glm::vec3(0, 0, 1);
	colors[1] = glm::vec3(0, 0, 1);
	colors[2] = glm::vec3(0, 0, 1);
	colors[3] = glm::vec3(0, 0, 1);
	colors[4] = glm::vec3(1, 0, 0);
	colors[5] = glm::vec3(1, 1, 0);
	colors[6] = glm::vec3(1, 0.5, 0);
	colors[7] = glm::vec3(0, 1, 0);
	colors[8] = glm::vec3(0, 0, 1);
	colors[9] = glm::vec3(0, 0, 1);
	colors[10] = glm::vec3(0, 0, 1);
	colors[11] = glm::vec3(0, 0, 1);

	for (int i = 0; i < 9; i++) {
		aux = row_1.at(i);
		scale = rand() / (float)RAND_MAX + 4;
		gap = rand() / (float)RAND_MAX + 0.5f;
		index = rand() % 12;
		row_1.push_back(new Platform(-2, -0.125f, aux->z - 0.5f * aux->scale_z - gap - 0.5f * scale, 1, 0.25f, scale, colors[index]));

		aux = row_2.at(i);
		scale = rand() / (float)RAND_MAX + 4;
		gap = rand() / (float)RAND_MAX + 0.5f;
		index = rand() % 12;
		row_2.push_back(new Platform(0, -0.125f, aux->z - 0.5f * aux->scale_z - gap - 0.5f * scale, 1, 0.25f, scale, colors[index]));

		aux = row_3.at(i);
		scale = rand() / (float)RAND_MAX + 4;
		gap = rand() / (float)RAND_MAX + 0.5f;
		index = rand() % 12;
		row_3.push_back(new Platform(2, -0.125f, aux->z - 0.5f * aux->scale_z - gap - 0.5f * scale, 1, 0.25f, scale, colors[index]));
	}
}

void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds)
{
	interface->scale -= deltaTimeSeconds / 50;
	orange_buff -= deltaTimeSeconds / 2;
	if (interface->scale <= 0) {
		cout << "Game Over!\n";
		window->Close();
	}
	if (orange_buff > 0) {
		deformation = 1;
		player->color = glm::vec3(1, 0.5, 0);
	}
	else {
		deformation = 0;
		player->color = glm::vec3(0.5, 0.5, 0.5);
	}

	//Camera
	if (first_person) {
		camera->Set(glm::vec3(player->x, player->y, player->z - 0.5f), glm::vec3(player->x, player->y, player->z - 0.6f), glm::vec3(0, 1, 0));
		//Interface
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(player->x - 0.5, player->y + 0.25, player->z - 1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15));
		RenderMesh(interface->body, shaders["MyShader"], modelMatrix, interface->color, 0);
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(player->x - 0.5, player->y + 0.25, player->z - 0.999));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(interface->scale, 1, 1));
		RenderMesh(interface->fuel, shaders["MyShader"], modelMatrix, interface->fuel_color, 0);
	}
	else {
		camera->Set(glm::vec3(0, 3, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
		//Interface
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(interface->x, interface->y, interface->z));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(-30.0f), glm::vec3(1, 0, 0));
		RenderMesh(interface->body, shaders["MyShader"], modelMatrix, interface->color, 0);
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(interface->x, interface->y, interface->z + 0.001));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(-30.0f), glm::vec3(1, 0, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(interface->scale, 1, 1));
		RenderMesh(interface->fuel, shaders["MyShader"], modelMatrix, interface->fuel_color, 0);
	}

	//Player
	if (game_over) {
		player->y -= deltaTimeSeconds;
		player->scale -= deltaTimeSeconds / 2;
		platform_speed = 0;
		player_speed = 0;
		if (player->scale < 0) {
			cout << "Game Over!\n";
			window->Close();
		}
	}
	else {
		if (jumping && player->y < 1) {
			player->y += deltaTimeSeconds;
		}
		if (player->y >= 1) {
			jumping = false;
		}
		if (!jumping) {
			player->y -= deltaTimeSeconds;
		}
		if (player->y < 0.5f && !jumping) {
			player->y = 0.5f;
		}
	}
	player->x += player_speed * deltaTimeSeconds;
	bool falling = false;
	for (int i = 0; i < 10; i++) {
		old_color = row_1.at(i)->color;
		falling |= row_1.at(i)->player_landed(player->x, player->y, player->z);
		if (row_1.at(i)->player_landed(player->x, player->y, player->z)) {
			if (old_color == glm::vec3(1, 0, 0)) {
				cout << "Game Over!\n";
				window->Close();
			}
			if (old_color == glm::vec3(1, 1, 0)) {
				interface->scale -= 0.25;
			}
			if (old_color == glm::vec3(1, 0.5, 0)) {
				platform_speed = 5;
				orange_buff = 1;
			}
			if (old_color == glm::vec3(0, 1, 0)) {
				interface->scale += 0.25;
				if (interface->scale > 1) {
					interface->scale = 1;
				}
			}
		}

		old_color = row_2.at(i)->color;
		falling |= row_2.at(i)->player_landed(player->x, player->y, player->z);
		if (row_2.at(i)->player_landed(player->x, player->y, player->z)) {
			if (old_color == glm::vec3(1, 0, 0)) {
				cout << "Game Over!\n";
				window->Close();
			}
			if (old_color == glm::vec3(1, 1, 0)) {
				interface->scale -= 0.25;
			}
			if (old_color == glm::vec3(1, 0.5, 0)) {
				platform_speed = 5;
				orange_buff = 1;
			}
			if (old_color == glm::vec3(0, 1, 0)) {
				interface->scale += 0.25;
				if (interface->scale > 1) {
					interface->scale = 1;
				}
			}
		}

		old_color = row_3.at(i)->color;
		falling |= row_3.at(i)->player_landed(player->x, player->y, player->z);
		if (row_3.at(i)->player_landed(player->x, player->y, player->z)) {
			if (old_color == glm::vec3(1, 0, 0)) {
				cout << "Game Over!\n";
				window->Close();
			}
			if (old_color == glm::vec3(1, 1, 0)) {
				interface->scale -= 0.25;
			}
			if (old_color == glm::vec3(1, 0.5, 0)) {
				platform_speed = 5;
				orange_buff = 1;
			}
			if (old_color == glm::vec3(0, 1, 0)) {
				interface->scale += 0.25;
				if (interface->scale > 1) {
					interface->scale = 1;
				}
			}
		}
	}
	falling |= platform->player_landed(player->x, player->y, player->z);
	if (!falling && player->y == 0.5f) {
		game_over = true;
	}
	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(player->x, player->y, player->z));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(player->scale));
	RenderMesh(player->body, shaders["MyShader"], modelMatrix, player->color, deformation);
	
	//Starting Platform
	platform->z += platform_speed * deltaTimeSeconds;
	if (platform->z < 3) {
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(platform->x, platform->y, platform->z));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(platform->scale_x, platform->scale_y, platform->scale_z));
		platform->player_landed(player->x, player->y, player->z);
		RenderMesh(platform->body, shaders["MyShader"], modelMatrix, platform->color, 0);
	}

	//Platform Rows
	for (int i = 0; i < 10; i++) {
		aux = row_1.at(i);
		aux->z += platform_speed * deltaTimeSeconds;
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(aux->x, aux->y, aux->z));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(aux->scale_x, aux->scale_y, aux->scale_z));
		//aux->player_landed(player->x, player->y, player->z);
		RenderMesh(aux->body, shaders["MyShader"], modelMatrix, aux->color, 0);

		aux = row_2.at(i);
		aux->z += platform_speed * deltaTimeSeconds;
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(aux->x, aux->y, aux->z));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(aux->scale_x, aux->scale_y, aux->scale_z));
		//aux->player_landed(player->x, player->y, player->z);
		RenderMesh(aux->body, shaders["MyShader"], modelMatrix, aux->color, 0);

		aux = row_3.at(i);
		aux->z += platform_speed * deltaTimeSeconds;
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(aux->x, aux->y, aux->z));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(aux->scale_x, aux->scale_y, aux->scale_z));
		//aux->player_landed(player->x, player->y, player->z);
		RenderMesh(aux->body, shaders["MyShader"], modelMatrix, aux->color, 0);
	}

	//Platforms that have to be removed
	if (row_1.front()->z > 4) {
		row_1.erase(row_1.begin());
		aux = row_1.back();
		scale = rand() / (float)RAND_MAX + 4;
		gap = rand() / (float)RAND_MAX + 0.5f;
		index = rand() % 12;
		row_1.push_back(new Platform(-2, -0.125f, aux->z - 0.5f * aux->scale_z - gap - 0.5f * scale, 1, 0.25f, scale, colors[index]));
	}
	if (row_2.front()->z > 4) {
		row_2.erase(row_2.begin());
		aux = row_2.back();
		scale = rand() / (float)RAND_MAX + 4;
		gap = rand() / (float)RAND_MAX + 0.5f;
		index = rand() % 12;
		row_2.push_back(new Platform(0, -0.125f, aux->z - 0.5f * aux->scale_z - gap - 0.5f * scale, 1, 0.25f, scale, colors[index]));
	}
	if (row_3.front()->z > 4) {
		row_3.erase(row_3.begin());
		aux = row_3.back();
		scale = rand() / (float)RAND_MAX + 4;
		gap = rand() / (float)RAND_MAX + 0.5f;
		index = rand() % 12;
		row_3.push_back(new Platform(2, -0.125f, aux->z - 0.5f * aux->scale_z - gap - 0.5f * scale, 1, 0.25f, scale, colors[index]));
	}
}

void Tema2::FrameEnd()
{
	//DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema2::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix, glm::vec3 color, float deformation)
{
	if (!mesh || !shader || !shader->program)
		return;

	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	int location = glGetUniformLocation(shader->program, "v_color");
	glUniform3f(location, color.x, color.y, color.z);
	int timeLocation = glGetUniformLocation(shader->program, "time");
	glUniform1f(timeLocation, Engine::GetElapsedTime());
	int deformationLocation = glGetUniformLocation(shader->program, "deformation");
	glUniform1f(deformationLocation, deformation);
	mesh->Render();
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	if (window->KeyHold(GLFW_KEY_W) && orange_buff <= 0) {
		platform_speed += deltaTime;
		if (platform_speed > 2.5f) {
			platform_speed = 2.5f;
		}
	}
	if (window->KeyHold(GLFW_KEY_S) && orange_buff <= 0) {
		platform_speed -= deltaTime;
		if (platform_speed < 0) {
			platform_speed = 0;
		}
	}
	if (window->KeyHold(GLFW_KEY_D) && !game_over) {
		player->x += 3 * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_A) && !game_over) {
		player->x -= 3 * deltaTime;
	}
}

void Tema2::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_SPACE) {
		if (!jumping && player->y == 0.5f) {
			jumping = true;
		}
	}
	if (key == GLFW_KEY_C) {
		first_person = !first_person;
	}
}

void Tema2::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}
