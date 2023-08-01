#define GL_GLEXT_PROTOTYPES

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <vector>
#include <iostream>
#include <random>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Program.h"
#include "Chunk.h"
#include "Texture.h"

using namespace std;

const float pi{3.14159f};
const int grid_width{100};
const int grid_height{100};

unsigned int dirt_grass_texture{};
unsigned int grass_texture{};
unsigned int dirt_texture{};
unsigned int flower_texture{};

float zNear{0.5f};
float zFar{1000.0f};

float screen_width{};
float screen_height{};
float sensitivity{0.0003};

float yaw{-pi / 2.f};
float pitch{};
float total_pitch{};

Program program{};
Chunk ground{};
random_device rd{};

Camera camera{
	glm::vec4{0.f, 0.f, 3.f, 0.f},
	glm::vec4{0.f, 0.f, -1.f, 0.f},
	glm::vec4{0.f, 1.f, 0.f, 0.f}
};


const vector<float> flower_vertices
{
	-0.5f, 0.5f, 0.f, 1.f,
	0.5f, 0.5f, 0.f, 1.f,
	0.5f, -0.5f, 0.f, 1.f,

	-0.5f, 0.5f, 0.f, 1.f,
	0.5f, -0.5f, 0.f, 1.f,
	-0.5f, -0.5f, 0.f, 1.f,

	0.f, 0.5f, -0.5f, 1.f,
	0.f, 0.5f, 0.5f, 1.f,
	0.f, -0.5f, 0.5f, 1.f,

	0.f, 0.5f, -0.5f, 1.f,
	0.f, -0.5f, 0.5f, 1.f,
	0.f, -0.5f, -0.5f, 1.f,

	0.5f, 0.5f, 0.f, 1.f,
	-0.5f, 0.5f, 0.f, 1.f,
	0.5f, -0.5f, 0.f, 1.f,

	0.5f, -0.5f, 0.f, 1.f,
	-0.5f, 0.5f, 0.f, 1.f,
	-0.5f, -0.5f, 0.f, 1.f,

	0.f, 0.5f, 0.5f, 1.f,
	0.f, 0.5f, -0.5f, 1.f,
	0.f, -0.5f, 0.5f, 1.f,

	0.f, -0.5f, 0.5f, 1.f,
	0.f, 0.5f, -0.5f, 1.f,
	0.f, -0.5f, -0.5f, 1.f,

	0.f,  1.f,
	1.f, 1.f,
	1.f, 0.f,

	0.f,  1.f,
	1.f, 0.f,
	0.f,  0.f,

	0.f,  1.f,
	1.f, 1.f,
	1.f, 0.f,

	0.f,  1.f,
	1.f, 0.f,
	0.f,  0.f,

	1.f, 1.f,
	0.f,  1.f,
	1.f, 0.f,

	1.f, 0.f,
	0.f,  1.f,
	0.f,  0.f,

	1.f, 1.f,
	0.f,  1.f,
	1.f, 0.f,

	1.f, 0.f,
	0.f,  1.f,
	0.f,  0.f,
};

glm::mat4 rotation_z_matrix(float const angle)
{
	glm::mat4 matrix{1.f};

	matrix[0].x = cos(angle);
	matrix[0].y = sin(angle);
	matrix[1].x = -sin(angle);
	matrix[1].y = cos(angle);

	return matrix;
}

void set_perspective_matrix()
{
	glm::mat4 matrix{glm::perspective(glm::radians(30.0f), screen_width / screen_height, zNear, zFar)};

	program.use();
	int perspective_matrix_location{glGetUniformLocation(program.get(), "perspectiveMatrix")};
	glUniformMatrix4fv(perspective_matrix_location, 1, GL_FALSE, glm::value_ptr(matrix));
	program.clear_use();
}

//TODO: Implement with Chunk class
//void create_grid(int const x, int const y)
//{
//	for(int i{}; i < y; ++i)
//	{
//		for(int j{}; j < x; ++j)
//		{
//			ground.push_back(Voxel{static_cast<float>(i), 0.f, static_cast<float>(j), Voxel::GRASS});
//		}
//	}
//}

//void load_ground()
//{
//	for(unsigned i{}; i < ground.size(); ++i)
//	{
//		for(auto vertex : voxel_vertices)
//		{
//			vertex_positions.push_back(vertex);
//		}
//	}
//}

//void randomize_flowers(int const count, int const distribution)
//{
//	mt19937 gen{rd()};
//	std::uniform_int_distribution<> distrib(1, distribution);
//
//	for(int i{}; i < count; ++i)
//	{
//		flowers.push_back(
//			Voxel{static_cast<float>(distrib(gen)),
//				1.f,
//				static_cast<float>(distrib(gen)),
//				Voxel::FLOWER});
//	}
//}
//
//void load_flowers()
//{
//	for(unsigned i{}; i < flowers.size(); ++i)
//	{
//		for(unsigned j{}; j < flower_vertices.size(); ++j)
//		{
//			vertex_positions_flowers.push_back(flower_vertices.at(j));
//		}
//	}
//}
//
//void randomize_trees(int const count, int const distribution)
//{
//	mt19937 gen{rd()};
//	std::uniform_int_distribution<> distrib(1, distribution);
//
//	for(int i{}; i < count; ++i)
//	{
//		trees.push_back(
//			Voxel{static_cast<float>(distrib(gen)),
//				1.f,
//				static_cast<float>(distrib(gen)),
//				Voxel::TREE});
//	}
//}
//
//void load_trees()
//{
//	for(unsigned i{}; i < trees.size(); ++i)
//	{
//		for(int j{}; j < 13; ++j)
//		{
//			for(unsigned k{}; k < voxel_vertices.size(); ++k)
//			{
//				vertex_positions_trees.push_back(voxel_vertices.at(k));
//			}
//		}
//	}
//}

void init()
{
	program.init();
	ground.init_vao();
	ground.create_voxel(Voxel_Type::grass, glm::vec3{1.f, 0.f, 2.f});
	ground.create_voxel(Voxel_Type::grass, glm::vec3{1.f, 0.f, 4.f});
	ground.load_vertices();
	ground.init_vbo();

	//randomize_flowers(100, 100);
	//load_flowers();
	//randomize_trees(20, 100);
	//load_trees();

	//glGenVertexArrays(1, &vao_flowers);
	//glBindVertexArray(vao_flowers);
	//flower_texture = Texture::load("assets/flower.png");

	//glBindBuffer(GL_ARRAY_BUFFER, vbo_flowers);
	//glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(4 * 4 * 3 * 8));

	//glGenVertexArrays(1, &vao_trees);
	//glBindVertexArray(vao_trees);

	//glBindBuffer(GL_ARRAY_BUFFER, vbo_trees);
	//glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(4 * 4 * 3 * 12));

	glBindVertexArray(0);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glEnable(GL_DEPTH_TEST);
}

void display()
{
	set_perspective_matrix();
	camera.set_camera_matrix(program.get());
	program.use();
	ground.render(program);

	//glBindVertexArray(vao_flowers);

	//for(auto flower : flowers)
	//{
	//	glm::vec3 pos{flower.get_position()};
	//	glm::mat4 matrix{translate_matrix(glm::vec3{pos.x, pos.y, -pos.z - 0.5f})};
	//	int transform_matrix_location{glGetUniformLocation(program.get(), "transformMatrix")};
	//	glUniformMatrix4fv(transform_matrix_location, 1, GL_FALSE, glm::value_ptr(matrix));

	//	glBindTexture(GL_TEXTURE_2D, flower_texture);
	//	glDrawArrays(GL_TRIANGLES, 0, 3 * 8);
	//}

	//glBindVertexArray(vao_trees);

	//for(auto tree : trees)
	//{
	//	glm::vec3 pos{tree.get_position()};
	//	glBindTexture(GL_TEXTURE_2D, dirt_texture);
	//	for(int i{}; i < 4; ++i)
	//	{
	//		glm::mat4 matrix{translate_matrix(glm::vec3{pos.x, pos.y + i, -pos.z - 0.5f})};
	//		int transform_matrix_location{glGetUniformLocation(program.get(), "transformMatrix")};
	//		glUniformMatrix4fv(transform_matrix_location, 1, GL_FALSE, glm::value_ptr(matrix));
	//		glDrawArrays(GL_TRIANGLES, 0, 3 * 12);
	//	}

	//	glm::mat4 matrix{translate_matrix(glm::vec3{pos.x, pos.y + 4, -pos.z - 0.5f})};
	//	int transform_matrix_location{glGetUniformLocation(program.get(), "transformMatrix")};
	//	glUniformMatrix4fv(transform_matrix_location, 1, GL_FALSE, glm::value_ptr(matrix));

	//	glBindTexture(GL_TEXTURE_2D, grass_texture);
	//	glDrawArrays(GL_TRIANGLES, 0, 3 * 12);

	//	//crown starts
	//	matrix = translate_matrix(glm::vec3{pos.x + 1, pos.y + 3, -pos.z - 1 - 0.5f});
	//	transform_matrix_location = glGetUniformLocation(program.get(), "transformMatrix");
	//	glUniformMatrix4fv(transform_matrix_location, 1, GL_FALSE, glm::value_ptr(matrix));
	//	glDrawArrays(GL_TRIANGLES, 0, 3 * 12);

	//	matrix = translate_matrix(glm::vec3{pos.x + 1, pos.y + 3, -pos.z - 0.5f});
	//	transform_matrix_location = glGetUniformLocation(program.get(), "transformMatrix");
	//	glUniformMatrix4fv(transform_matrix_location, 1, GL_FALSE, glm::value_ptr(matrix));
	//	glDrawArrays(GL_TRIANGLES, 0, 3 * 12);

	//	matrix = translate_matrix(glm::vec3{pos.x + 1, pos.y + 3, -pos.z + 1 - 0.5f});
	//	transform_matrix_location = glGetUniformLocation(program.get(), "transformMatrix");
	//	glUniformMatrix4fv(transform_matrix_location, 1, GL_FALSE, glm::value_ptr(matrix));
	//	glDrawArrays(GL_TRIANGLES, 0, 3 * 12);

	//	matrix = translate_matrix(glm::vec3{pos.x, pos.y + 3, -pos.z - 1 - 0.5f});
	//	transform_matrix_location = glGetUniformLocation(program.get(), "transformMatrix");
	//	glUniformMatrix4fv(transform_matrix_location, 1, GL_FALSE, glm::value_ptr(matrix));
	//	glDrawArrays(GL_TRIANGLES, 0, 3 * 12);

	//	matrix = translate_matrix(glm::vec3{pos.x, pos.y + 3, -pos.z + 1 - 0.5f});
	//	transform_matrix_location = glGetUniformLocation(program.get(), "transformMatrix");
	//	glUniformMatrix4fv(transform_matrix_location, 1, GL_FALSE, glm::value_ptr(matrix));
	//	glDrawArrays(GL_TRIANGLES, 0, 3 * 12);

	//	matrix = translate_matrix(glm::vec3{pos.x - 1, pos.y + 3, -pos.z - 1 - 0.5f});
	//	transform_matrix_location = glGetUniformLocation(program.get(), "transformMatrix");
	//	glUniformMatrix4fv(transform_matrix_location, 1, GL_FALSE, glm::value_ptr(matrix));
	//	glDrawArrays(GL_TRIANGLES, 0, 3 * 12);

	//	matrix = translate_matrix(glm::vec3{pos.x - 1, pos.y + 3, -pos.z - 0.5f});
	//	transform_matrix_location = glGetUniformLocation(program.get(), "transformMatrix");
	//	glUniformMatrix4fv(transform_matrix_location, 1, GL_FALSE, glm::value_ptr(matrix));
	//	glDrawArrays(GL_TRIANGLES, 0, 3 * 12);

	//	matrix = translate_matrix(glm::vec3{pos.x - 1, pos.y + 3, -pos.z + 1 - 0.5f});
	//	transform_matrix_location = glGetUniformLocation(program.get(), "transformMatrix");
	//	glUniformMatrix4fv(transform_matrix_location, 1, GL_FALSE, glm::value_ptr(matrix));
	//	glDrawArrays(GL_TRIANGLES, 0, 3 * 12);
	//}
	program.clear_use();
}

void handle_keypress()
{
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		camera.translate_x(0.33f);
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		camera.translate_x(-0.33f);
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		camera.translate_z(0.33f);
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		camera.translate_z(-0.33f);
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::E))
	{
		camera.translate_y(0.33f);
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		camera.translate_y(-0.33f);
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void handle_mouse_movement(sf::Window const& window, sf::Vector2i new_mouse_pos)
{
	new_mouse_pos.x -= screen_width / 2;
	new_mouse_pos.y -= screen_height / 2;

	sf::Vector2f direction{new_mouse_pos};

	direction *= sensitivity;
	yaw = -direction.x;
	pitch = direction.y;
	total_pitch += direction.y;

	if(total_pitch > pi / 2.f)
	{
		pitch = 0.f;
		total_pitch = pi/2.f;
	}
	else if(total_pitch < -(pi / 2.f))
	{
		pitch = 0.f;
		total_pitch = -(pi/2.f);
	}

	camera.rotate_x(yaw);
	camera.rotate_y(pitch);

	sf::Mouse::setPosition(
		sf::Vector2i{
			static_cast<int>(screen_width / 2),
			static_cast<int>(screen_height / 2)},
			window);
}

int main()
{
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 4.5;
	settings.minorVersion = 0;

    // create the window
	sf::Window window{sf::VideoMode(800, 800), "OpenGL", sf::Style::Default, settings};
    window.setVerticalSyncEnabled(true);
	window.setMouseCursorVisible(false);
	window.setMouseCursorGrabbed(true);

    // activate the window
    window.setActive(true);

    // load resources, initialize the OpenGL states, ...
	glClearColor(0.29f, 0.72f, 0.98f, 1);
	init();

    //run the main loop
    bool running{true};
    while (running)
    {
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
			if (event.type == sf::Event::Closed)
            {
                // end the program
                running = false;
            }
            else if (event.type == sf::Event::Resized)
            {
                // adjust the viewport when the window is resized
                glViewport(0, 0, event.size.width, event.size.height);
				screen_width = event.size.width;
				screen_height = event.size.height;
				sf::Mouse::setPosition(
					sf::Vector2i{
						static_cast<int>(screen_width / 2),
						static_cast<int>(screen_height / 2)},
						window);
            }
        }
		handle_mouse_movement(
			window,
			sf::Mouse::getPosition(window));
		handle_keypress();

        // clear the buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw
		display();

        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }
}
