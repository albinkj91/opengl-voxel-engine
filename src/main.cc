#define GL_GLEXT_PROTOTYPES

#include <SFML/Window.hpp>
#include <vector>
#include <iostream>
#include <random>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Program.h"
#include "Chunk.h"
#include "Grass.h"
#include "Tree.h"
#include "Flower.h"
#include "Texture.h"

using namespace std;

const float pi{3.14159f};

const int grid_width{100};
const int grid_height{100};

const int tree_count{40};
const float tree_distribution{100.f};
const int tree_height_variance{10};

const int flower_count{100};
const float flower_distribution{100.f};

float zNear{0.5f};
float zFar{1000.0f};

float screen_width{};
float screen_height{};
float sensitivity{0.0003};

float yaw{-pi / 2.f};
float pitch{};
float total_pitch{};

Program program{};

random_device rd_tree_placement{};
random_device rd_tree_height{};
random_device rd_flower_placement{};

Camera camera{
	glm::vec4{0.f, 0.f, 3.f, 0.f},
	glm::vec4{0.f, 0.f, -1.f, 0.f},
	glm::vec4{0.f, 1.f, 0.f, 0.f}
};

vector<float> voxel_vertices
{
    -0.5f, 0.5f, 0.5f, 1.f,
    0.5f, 0.5f, 0.5f, 1.f,
    0.5f, -0.5f, 0.5f, 1.f,

    -0.5f, 0.5f, 0.5f, 1.f,
    0.5f, -0.5f, 0.5f, 1.f,
    -0.5f, -0.5f, 0.5f, 1.f,

    -0.5f, 0.5f, -0.5f, 1.f,
    -0.5f, 0.5f, 0.5f, 1.f,
    -0.5f, -0.5f, 0.5f, 1.f,

    -0.5f, 0.5f, -0.5f, 1.f,
    -0.5f, -0.5f, 0.5f, 1.f,
    -0.5f, -0.5f, -0.5f, 1.f,

    0.5f, 0.5f, 0.5f, 1.f,
    0.5f, 0.5f, -0.5f, 1.f,
    0.5f, -0.5f, -0.5f, 1.f,

    0.5f, 0.5f, 0.5f, 1.f,
    0.5f, -0.5f, -0.5f, 1.f,
    0.5f, -0.5f, 0.5f, 1.f,

    0.5f, 0.5f, -0.5f, 1.f,
    -0.5f, 0.5f, -0.5f, 1.f,
    -0.5f, -0.5f, -0.5f, 1.f,

    0.5f, 0.5f, -0.5f, 1.f,
    -0.5f, -0.5f, -0.5f, 1.f,
    0.5f, -0.5f, -0.5f, 1.f,

    -0.5f, -0.5f, 0.5f, 1.f,
    0.5f, -0.5f, 0.5f, 1.f,
    0.5f, -0.5f, -0.5f, 1.f,

    -0.5f, -0.5f, 0.5f, 1.f,
    0.5f, -0.5f, -0.5f, 1.f,
    -0.5f, -0.5f, -0.5f, 1.f,

    -0.5f, 0.5f, -0.5f, 1.f,
    0.5f, 0.5f, -0.5f, 1.f,
    0.5f, 0.5f, 0.5f, 1.f,

    -0.5f, 0.5f, -0.5f, 1.f,
    0.5f, 0.5f, 0.5f, 1.f,
    -0.5f, 0.5f, 0.5f, 1.f,

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
    0.f,  0.f
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

Chunk ground{voxel_vertices};
Chunk trees{voxel_vertices};
Chunk flowers{flower_vertices};

void set_perspective_matrix()
{
	glm::mat4 matrix{glm::perspective(glm::radians(45.0f), screen_width / screen_height, zNear, zFar)};

	program.use();
	int perspective_matrix_location{glGetUniformLocation(program.get(), "perspectiveMatrix")};
	glUniformMatrix4fv(perspective_matrix_location, 1, GL_FALSE, glm::value_ptr(matrix));
	program.clear_use();
}

void create_ground(int const width, int const height)
{
	for(int i{}; i < height; ++i)
	{
		for(int j{}; j < width; ++j)
		{
			ground.add(make_unique<Grass>(Grass{
					glm::vec3{static_cast<float>(i), 0.f, static_cast<float>(j)},
					program,
					Texture::load("assets/grass.png"),
					Texture::load("assets/dirt-grass.png"),
					Texture::load("assets/dirt.png")}));
		}
	}
}

void randomize_trees(int const count, float const distribution, int const height)
{
	mt19937 gen_placement{rd_tree_placement()};
	mt19937 gen_height{rd_tree_height()};
	std::uniform_real_distribution<> distrib_placement(1.f, distribution);
	std::uniform_int_distribution<> distrib_height(5, height);

	for(int i{}; i < count; ++i)
	{
		trees.add(make_unique<Tree>(
			Tree{glm::vec3{floor(distrib_placement(gen_placement)),
					1.f,
					floor(distrib_placement(gen_placement))},
				program,
				distrib_height(gen_height),
				Texture::load("assets/leaves.png"),
				Texture::load("assets/bark.png"),
				Texture::load("assets/bark.png")}));
	}
}

void randomize_flowers(int const count, int const distribution)
{
	mt19937 gen{rd_flower_placement()};
	std::uniform_real_distribution<> distrib(1, distribution);

	for(int i{}; i < count; ++i)
	{
		flowers.add(make_unique<Flower>(
			Flower{glm::vec3{floor(distrib(gen)),
					1.f,
					floor(distrib(gen))},
				program,
				Texture::load("assets/flower.png")}));
	}
}

void init()
{
	program.init();

	ground.init_vao();
	create_ground(grid_width, grid_height);
	ground.init_vbo();

	trees.init_vao();
	randomize_trees(tree_count, tree_distribution, tree_height_variance);
	trees.init_vbo();

	flowers.init_vao();
	randomize_flowers(flower_count, flower_distribution);
	flowers.init_vbo();

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
	ground.render();
	trees.render();
	flowers.render();
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
