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

using namespace std;

GLuint vao{};
GLuint vertex_buffer_obj{};
GLuint program{};

const vector<float> vertex_positions
{
	0.75f, 0.75f, 0.0f, 1.0f,
	0.75f, -0.75f, 0.0f, 1.0f,
	-0.75f, -0.75f, 0.0f, 1.0f
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

void init()
{
	init_vertex_buffer();
	init_program();

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
}

void display()
{
	glUseProgram(program);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);
	glUseProgram(0);
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
            }
        }

        // clear the buffers
        glClear(GL_COLOR_BUFFER_BIT);

        // draw
		display();

        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }

    // release resources...

    return 0;
}
