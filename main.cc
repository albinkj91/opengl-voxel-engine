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

using namespace std;

GLuint vao{};
GLuint vertex_buffer_obj{};
GLuint program{};

float screen_width{1.0f};
float screen_height{1.0f};
float offset_x{0.0f};
float offset_y{0.0f};

sf::Clock rotation_clock{};

const vector<float> vertex_positions
{
	0.5f, 0.5f, -1.0f, 1.0f,
	0.5f, -0.5f, -1.0f, 1.0f,
	-0.5f, 0.5f, -1.0f, 1.0f,

	-0.5f, 0.5f, -1.0f, 1.0f,
	0.5f, -0.5f, -1.0f, 1.0f,
	-0.5f, -0.5f, -1.0f, 1.0f,

	-0.5f, 0.5f, -2.0f, 1.0f,
	-0.5f, 0.5f, -1.0f, 1.0f,
	-0.5f, -0.5f, -1.0f, 1.0f,

	-0.5f, 0.5f, -2.0f, 1.0f,
	-0.5f, -0.5f, -1.0f, 1.0f,
	-0.5f, -0.5f, -2.0f, 1.0f,

	-0.5f, -0.5f, -1.0f, 1.0f,
	0.5f, -0.5f, -1.0f, 1.0f,
	0.5f, -0.5f, -2.0f, 1.0f,

	-0.5f, -0.5f, -1.0f, 1.0f,
	0.5f, -0.5f, -2.0f, 1.0f,
	-0.5f, -0.5f, -2.0f, 1.0f,

	0.5f, 0.5f, -1.0f, 1.0f,
	0.5f, 0.5f, -2.0f, 1.0f,
	0.5f, -0.5f, -1.0f, 1.0f,

	0.5f, -0.5f, -1.0f, 1.0f,
	0.5f, 0.5f, -2.0f, 1.0f,
	0.5f, -0.5f, -2.0f, 1.0f,

	0.5f, 0.5f, -1.0f, 1.0f,
	-0.5f, 0.5f, -1.0f, 1.0f,
	0.5f, 0.5f, -2.0f, 1.0f,

	0.5f, 0.5f, -2.0f, 1.0f,
	-0.5f, 0.5f, -1.0f, 1.0f,
	-0.5f, 0.5f, -2.0f, 1.0f,

	0.5f, -0.5f, -2.0f, 1.0f,
	0.5f, 0.5f, -2.0f, 1.0f,
	-0.5f, 0.5f, -2.0f, 1.0f,

	0.5f, -0.5f, -2.0f, 1.0f,
	-0.5f, 0.5f, -2.0f, 1.0f,
	-0.5f, -0.5f, -2.0f, 1.0f,


	0.f, 1.f, 0.f, 1.0f,
	0.f, 1.f, 0.f, 1.0f,
	0.f, 1.f, 0.f, 1.0f,

	0.f, 1.f, 0.f, 1.0f,
	0.f, 1.f, 0.f, 1.0f,
	0.f, 1.f, 0.f, 1.0f,

	1.f, 0.f, 0.f, 1.0f,
	1.f, 0.f, 0.f, 1.0f,
	1.f, 0.f, 0.f, 1.0f,

	1.f, 0.f, 0.f, 1.0f,
	1.f, 0.f, 0.f, 1.0f,
	1.f, 0.f, 0.f, 1.0f,

	0.f, 0.f, 1.f, 1.0f,
	0.f, 0.f, 1.f, 1.0f,
	0.f, 0.f, 1.f, 1.0f,

	0.f, 0.f, 1.f, 1.0f,
	0.f, 0.f, 1.f, 1.0f,
	0.f, 0.f, 1.f, 1.0f,

	1.f, 1.f, 0.f, 1.0f,
	1.f, 1.f, 0.f, 1.0f,
	1.f, 1.f, 0.f, 1.0f,

	1.f, 1.f, 0.f, 1.0f,
	1.f, 1.f, 0.f, 1.0f,
	1.f, 1.f, 0.f, 1.0f,

	0.f, 1.f, 1.f, 1.0f,
	0.f, 1.f, 1.f, 1.0f,
	0.f, 1.f, 1.f, 1.0f,

	0.f, 1.f, 1.f, 1.0f,
	0.f, 1.f, 1.f, 1.0f,
	0.f, 1.f, 1.f, 1.0f,

	1.f, 1.f, 1.f, 1.0f,
	1.f, 1.f, 1.f, 1.0f,
	1.f, 1.f, 1.f, 1.0f,

	1.f, 1.f, 1.f, 1.0f,
	1.f, 1.f, 1.f, 1.0f,
	1.f, 1.f, 1.f, 1.0f
};

string load_shader(string const& filename)
{
	string shader{};
	string tmp{};
	ifstream ifs{filename};

	while(getline(ifs, tmp, '\n'))
	{
		shader
			.append(tmp)
			.append("\n");
	}
	return shader;
}

GLuint create_shader(GLenum shader_type, string & shader_file)
{
	// create OpenGL shader object
	GLuint shader{glCreateShader(shader_type)};
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
        GLint info_log_length;
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

GLuint create_program(vector<GLuint> const& shaders)
{
	// create program object and attach shaders
	GLuint program = glCreateProgram();
	
	for_each(shaders.begin(), shaders.end(),
		[program](GLuint const s)
		{
			glAttachShader(program, s);
		});

	// link program
	glLinkProgram(program);

	// check if successful
	GLint status;
    glGetProgramiv (program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint info_log_length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);

        GLchar *str_info_log = new GLchar[info_log_length + 1];
        glGetProgramInfoLog(program, info_log_length, NULL, str_info_log);
        cout << "Linker failure: " << str_info_log << endl;
        delete[] str_info_log;
    }

	// detach shaders from program after linking (resource cleanup)
	for_each(shaders.begin(), shaders.end(),
		[program](GLuint const s)
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
	GLuint vert_shader{create_shader(GL_VERTEX_SHADER, vert_shader_str)};
	GLuint frag_shader{create_shader(GL_FRAGMENT_SHADER, frag_shader_str)};

	vector<GLuint> shaders{};
	shaders.push_back(vert_shader);
	shaders.push_back(frag_shader);

	// link shaders to program
	program = create_program(shaders);
	for_each(shaders.begin(), shaders.end(), glDeleteShader);
}

// allocate memory that OpenGL can "see" (GPU memory)
void init_vertex_buffer()
{
	// generate buffer object and bind to context (OpenGL struct state)
	glGenBuffers(1, &vertex_buffer_obj);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
	glBufferData(GL_ARRAY_BUFFER,
			vertex_positions.size() * sizeof(float),
			vertex_positions.data(),
			GL_STATIC_DRAW);

	//clean up resources
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void set_rotation_matrix()
{
	sf::Time elapsed{rotation_clock.getElapsedTime()};
	float angle{(fmod(elapsed.asSeconds(), 10.0f) / 10.0f) *  (3.14159f * 2.0f)};

	glm::mat4 matrix{1.0f};

	matrix[0].x = cos(angle);
	matrix[0].y = sin(angle);
	matrix[1].x = -sin(angle);
	matrix[1].y = cos(angle);

	GLint rot_mat_loc{glGetUniformLocation(program, "rotationMatrix")};
	glUniformMatrix4fv(rot_mat_loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void set_projection_matrix()
{
	glm::mat4 matrix{1.0f};
	GLfloat frustum_scale{1.0f};
	GLfloat zNear{0.5f};
	GLfloat zFar{3.0f};

	matrix[0].x = frustum_scale * (screen_height / screen_width);
	matrix[1].y = frustum_scale;
	matrix[2].z = (zFar + zNear) / (zNear - zFar);
	matrix[2].w = -1.0f;
	matrix[3].z = (2 * zFar * zNear) / (zNear - zFar);
	matrix[3].x = offset_x;
	matrix[3].y = offset_y;

	GLint proj_mat_loc{glGetUniformLocation(program, "perspectiveMatrix")};
	glUniformMatrix4fv(proj_mat_loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void init()
{
	init_vertex_buffer();
	init_program();

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
}

void display()
{
	glUseProgram(program);
	set_projection_matrix();
	set_rotation_matrix();
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)(4 * vertex_positions.size() / 2));
	glDrawArrays(GL_TRIANGLES, 0, vertex_positions.size() / (2*4));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glUseProgram(0);
}

void handle_keypress()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		offset_x -= 0.04f * (screen_height / screen_width);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		offset_x += 0.04f * (screen_height / screen_width);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		offset_y -= 0.04f;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		offset_y += 0.04f;
	}
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
	sf::Window window(sf::VideoMode(800, 800), "OpenGL", sf::Style::Default, settings);

    window.setVerticalSyncEnabled(true);

    // activate the window
    window.setActive(true);

	init();

    // load resources, initialize the OpenGL states, ...
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

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
            }
        }
		handle_keypress();

        // clear the buffers
        glClear(GL_COLOR_BUFFER_BIT);

        // draw
		display();

        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }
    return 0;
}
