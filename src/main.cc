#define GL_GLEXT_PROTOTYPES

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <memory>
#include <array>
#include <algorithm>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"
#include "Camera.h"

using namespace std;

const float pi{3.14159f};

unsigned int vao{};
unsigned int vbo{};
unsigned int ebo{};
unsigned int program{};

float zNear{0.5f};
float zFar{100.0f};

float screen_width{};
float screen_height{};
float sensitivity{0.0005};

float yaw{-pi / 2.f};
float pitch{};
float total_pitch{};

int image_width{};
int image_height{};
int channels{};

Camera camera{
	glm::vec4{0.f, 0.f, 3.f, 0.f},
	glm::vec4{0.f, 0.f, -1.f, 0.f},
	glm::vec4{0.f, 1.f, 0.f, 0.f}
};

const vector<float> vertex_positions
{
	-0.5f, 0.5f, 0.5f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f,
	0.5f, -0.5f, 0.5f, 1.0f,

	-0.5f, 0.5f, 0.5f, 1.0f,
	0.5f, -0.5f, 0.5f, 1.0f,
	-0.5f, -0.5f, 0.5f, 1.0f,

	-0.5f, 0.5f, -0.5f, 1.0f,
	-0.5f, 0.5f, 0.5f, 1.0f,
	-0.5f, -0.5f, 0.5f, 1.0f,

	-0.5f, 0.5f, -0.5f, 1.0f,
	-0.5f, -0.5f, 0.5f, 1.0f,
	-0.5f, -0.5f, -0.5f, 1.0f,

	-0.5f, -0.5f, 0.5f, 1.0f,
	0.5f, -0.5f, 0.5f, 1.0f,
	0.5f, -0.5f, -0.5f, 1.0f,

	-0.5f, -0.5f, 0.5f, 1.0f,
	0.5f, -0.5f, -0.5f, 1.0f,
	-0.5f, -0.5f, -0.5f, 1.0f,

	0.5f, 0.5f, 0.5f, 1.0f,
	0.5f, 0.5f, -0.5f, 1.0f,
	0.5f, -0.5f, -0.5f, 1.0f,

	0.5f, 0.5f, 0.5f, 1.0f,
	0.5f, -0.5f, -0.5f, 1.0f,
	0.5f, -0.5f, 0.5f, 1.0f,

	-0.5f, 0.5f, -0.5f, 1.0f,
	0.5f, 0.5f, -0.5f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f,

	-0.5f, 0.5f, -0.5f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f,
	-0.5f, 0.5f, 0.5f, 1.0f,

	0.5f, 0.5f, -0.5f, 1.0f,
	-0.5f, 0.5f, -0.5f, 1.0f,
	-0.5f, -0.5f, -0.5f, 1.0f,

	0.5f, 0.5f, -0.5f, 1.0f,
	-0.5f, -0.5f, -0.5f, 1.0f,
	0.5f, -0.5f, -0.5f, 1.0f,


	0.f, 1.f,
	1.f, 1.f,
	1.f, 0.f,

	0.f, 1.f,
	1.f, 0.f,
	0.f, 0.f,
	
	0.f, 1.f,
	1.f, 1.f,
	1.f, 0.f,

	0.f, 1.f,
	1.f, 0.f,
	0.f, 0.f,

	0.f, 1.f,
	1.f, 1.f,
	1.f, 0.f,

	0.f, 1.f,
	1.f, 0.f,
	0.f, 0.f,

	0.f, 1.f,
	1.f, 1.f,
	1.f, 0.f,

	0.f, 1.f,
	1.f, 0.f,
	0.f, 0.f,

	0.f, 1.f,
	1.f, 1.f,
	1.f, 0.f,

	0.f, 1.f,
	1.f, 0.f,
	0.f, 0.f,

	0.f, 1.f,
	1.f, 1.f,
	1.f, 0.f,

	0.f, 1.f,
	1.f, 0.f,
	0.f, 0.f,
};

string load_shader(string const& filename)
{
	string shader{};
	string tmp{};
	ifstream ifs{filename};

	while(getline(ifs, tmp, '\n'))
	{
		shader.append(tmp)
			.append("\n");
	}
	return shader;
}

unsigned int create_shader(GLenum shader_type, string & shader_file)
{
	// create OpenGL shader object
	unsigned int shader{glCreateShader(shader_type)};
	// shader code as c-style string
	auto source = shader_file.data();
	// fed into shader object. Length = NULL implies the strings are null-terminated
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	// check if compilation was successful and if not, log error
	int status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        int info_log_length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);

		GLchar *str_info_log = new GLchar[info_log_length + 1];
        glGetShaderInfoLog(shader, info_log_length, NULL, str_info_log);

        string str_shader_type{};
        switch(shader_type)
        {
			case GL_VERTEX_SHADER: str_shader_type = "vertex"; break;
			case GL_GEOMETRY_SHADER: str_shader_type = "geometry"; break;
			case GL_FRAGMENT_SHADER: str_shader_type = "fragment"; break;
        }

        cout << "Compile failure in "
			 <<  str_shader_type
			 << " shader:\n"
			 << str_info_log
			 << endl;
		delete[] str_info_log;
    }
	return shader;
}

unsigned int create_program(vector<unsigned int> const& shaders)
{
	// create program object and attach shaders
	unsigned int program = glCreateProgram();
	
	for_each(shaders.begin(), shaders.end(),
		[program](unsigned int const s)
		{
			glAttachShader(program, s);
		});

	// link program
	glLinkProgram(program);

	// check if successful
	int status;
    glGetProgramiv (program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        int info_log_length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);

        GLchar *str_info_log = new GLchar[info_log_length + 1];
        glGetProgramInfoLog(program, info_log_length, NULL, str_info_log);
        cout << "Linker failure: " << str_info_log << endl;
        delete[] str_info_log;
    }

	// detach shaders from program after linking (resource cleanup)
	for_each(shaders.begin(), shaders.end(),
		[program](unsigned int const s)
		{
			glDetachShader(program, s);
		});
	return program;
}

void init_program()
{
	// load each string containing each shaders code
	string vert_shader_str{load_shader("data/vertshader.vert")};
	string frag_shader_str{load_shader("data/fragshader.frag")};

	// compile shaders
	unsigned int vert_shader{create_shader(GL_VERTEX_SHADER, vert_shader_str)};
	unsigned int frag_shader{create_shader(GL_FRAGMENT_SHADER, frag_shader_str)};

	vector<unsigned int> shaders{};
	shaders.push_back(vert_shader);
	shaders.push_back(frag_shader);

	// link shaders to program
	program = create_program(shaders);
	for_each(shaders.begin(), shaders.end(), glDeleteShader);
}

void init_texture(string const& filepath)
{
	unsigned char* image{stbi_load(filepath.data(), &image_width, &image_height, &channels, 0)};
	if(!image)
	{
		cout << "Failed loading image" << endl;
		exit(0);
	}

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGB,
		image_width,
		image_height,
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		image);

	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
}

glm::mat4 rotation_z_matrix(float const angle)
{
	glm::mat4 matrix{1.0f};

	matrix[0].x = cos(angle);
	matrix[0].y = sin(angle);
	matrix[1].x = -sin(angle);
	matrix[1].y = cos(angle);

	return matrix;
}

glm::mat4 translate_matrix()
{
	glm::mat4 matrix{1.0f};

	matrix[3].x = 0.f;
	matrix[3].y = 0.f;
	matrix[3].z = -1.5f;
	return matrix;
}

void set_perspective_matrix()
{
	glm::mat4 matrix{glm::perspective(glm::radians(30.0f), screen_width / screen_height, zNear, zFar)};

	glUseProgram(program);
	int perspective_matrix_location{glGetUniformLocation(program, "perspectiveMatrix")};
	glUniformMatrix4fv(perspective_matrix_location, 1, GL_FALSE, glm::value_ptr(matrix));
	glUseProgram(0);
}

void init_vertex_buffer()
{
	// generate buffer object and bind to context (OpenGL struct state)
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER,
			vertex_positions.size() * sizeof(float),
			vertex_positions.data(),
			GL_STATIC_DRAW);

	////clean up resources
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void init()
{
	init_program();
	init_vertex_buffer();

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	init_texture("assets/bippi.jpg");
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(4 * 4 * 3 * 12));

	glBindVertexArray(0);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
}

void display()
{
	set_perspective_matrix();
	camera.set_camera_matrix(program);

	glUseProgram(program);
	glBindVertexArray(vao);

	glm::mat4 matrix{translate_matrix()};
	int transform_matrix_location{glGetUniformLocation(program, "transformMatrix")};
	glUniformMatrix4fv(transform_matrix_location, 1, GL_FALSE, glm::value_ptr(matrix));

	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

	glBindVertexArray(0);
	glUseProgram(0);
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
	glClearColor(0.2f, 0.2f, 0.4f, 0.0f);
	init();

    // run the main loop
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
        glClear(GL_COLOR_BUFFER_BIT);

        // draw
		display();

        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }
}
