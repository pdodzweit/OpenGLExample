// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GL/glfw.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <vector>
#include <iostream>

#include <string>
#include <fstream>
#include <streambuf>

#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <string_view>
#include <Windows.h>



// helper to check and display for shader compiler errors
bool check_shader_compile_status(GLuint obj) {
    GLint status;
    glGetShaderiv(obj, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE)
    {
        GLint length;
        glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> log(length);
        glGetShaderInfoLog(obj, length, &length, &log[0]);
        OutputDebugString(&log[0]);
        return false;
    }
    return true;
}

// helper to check and display for shader linker error
bool check_program_link_status(GLuint obj) {
    GLint status;
    glGetProgramiv(obj, GL_LINK_STATUS, &status);
    if(status == GL_FALSE)
    {
        GLint length;
        glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> log(length);
        glGetProgramInfoLog(obj, length, &length, &log[0]);
        OutputDebugString(&log[0]);
        return false;
    }
    return true;
}


int main() {
    int width = 640;
    int height = 480;

    if(glfwInit() == GL_FALSE) {
        std::cerr << "failed to init GLFW" << std::endl;
        return 1;
    }

	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 4);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 0);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	if( !glfwOpenWindow( 1024, 768, 0,0,0,0, 32,0, GLFW_WINDOW ) )
	{
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
		return -1;
	}

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	glfwSetWindowTitle( "Tutorial 04" );

    std::vector<glm::vec4> attrib0;
    attrib0.emplace_back(-0.25f, -0.25f, 0.0f, 1.0f);
    attrib0.emplace_back( 0.25f, -0.25f, 0.0f, 1.0f);
    attrib0.emplace_back(-0.25f,  0.25f, 0.0f, 1.0f);

    std::vector<unsigned short> indices;
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);

    // dummy
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

    GLuint vbo1, vbo2, vbo3, vbo4;
	glGenBuffers(1, &vbo1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo1);
    glBufferData(GL_ARRAY_BUFFER, attrib0.size() * sizeof(attrib0[0]), attrib0.data(), GL_STATIC_DRAW);

    // Generate a buffer for the indices as well
	GLuint elementbuffer;
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);


    GLenum error = glGetError();

    std::ifstream vertex_source_f("vert.glsl");
    std::string vertex_source((std::istreambuf_iterator<char>(vertex_source_f)), std::istreambuf_iterator<char>());

    std::ifstream tess_control_source_f("tcs.glsl");
    std::string tess_control_source((std::istreambuf_iterator<char>(tess_control_source_f)), std::istreambuf_iterator<char>());

    std::ifstream tess_eval_source_f("tes.glsl");
    std::string tess_eval_source((std::istreambuf_iterator<char>(tess_eval_source_f)), std::istreambuf_iterator<char>());

    std::ifstream fragment_source_f("frag.glsl");
    std::string fragment_source((std::istreambuf_iterator<char>(fragment_source_f)), std::istreambuf_iterator<char>());

    // program and shader handles
    GLuint shader_program, vertex_shader, tess_control_shader, tess_eval_shader, fragment_shader;

    // we need these to properly pass the strings
    const char *source;
    int length;

    // create and compiler vertex shader
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    source = vertex_source.c_str();
    length = vertex_source.size();
    glShaderSource(vertex_shader, 1, &source, &length);
    glCompileShader(vertex_shader);
    if(!check_shader_compile_status(vertex_shader)) {
        glfwTerminate();
        return 1;
    }

    // create and compiler tesselation control shader
    tess_control_shader = glCreateShader(GL_TESS_CONTROL_SHADER);
    source = tess_control_source.c_str();
    length = tess_control_source.size();
    glShaderSource(tess_control_shader, 1, &source, &length);
    glCompileShader(tess_control_shader);
    if(!check_shader_compile_status(tess_control_shader)) {
        glfwTerminate();
        return 1;
    }

    // create and compiler tesselation evaluation shader
    tess_eval_shader = glCreateShader(GL_TESS_EVALUATION_SHADER);
    source = tess_eval_source.c_str();
    length = tess_eval_source.size();
    glShaderSource(tess_eval_shader, 1, &source, &length);
    glCompileShader(tess_eval_shader);
    if(!check_shader_compile_status(tess_eval_shader)) {
        glfwTerminate();
        return 1;
    }

    // create and compiler fragment shader
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    source = fragment_source.c_str();
    length = fragment_source.size();
    glShaderSource(fragment_shader, 1, &source, &length);
    glCompileShader(fragment_shader);
    if(!check_shader_compile_status(fragment_shader)) {
        glfwTerminate();
        return 1;
    }

    // create program
    shader_program = glCreateProgram();

    // attach shaders
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, tess_control_shader);
    glAttachShader(shader_program, tess_eval_shader);
    glAttachShader(shader_program, fragment_shader);

    // link the program and check for errors
    glLinkProgram(shader_program);
    check_program_link_status(shader_program);
    error = glGetError();

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // mouse position
    double mousex = 0.0, mousey = 0.0;
	while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
		   glfwGetWindowParam( GLFW_OPENED ) )
    {
        glfwPollEvents();

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // clear first
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // use the shader program
        glUseProgram(shader_program);

        glEnableVertexAttribArray(0);
	    glBindBuffer(GL_ARRAY_BUFFER, vbo1);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0);

        // Generate a buffer for the indices as well
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

        glDrawElements(GL_PATCHES, 3, GL_UNSIGNED_SHORT, (void*)0);

        // check for errors
        error = glGetError();
        if(error != GL_NO_ERROR) {
            std::cerr << error << std::endl;
            break;
        }

        // finally swap buffers
        glfwSwapBuffers();
    }

    // delete the created objects

    glDeleteBuffers(1, &vbo1);
    glDeleteBuffers(1, &vbo2);
    glDeleteBuffers(1, &vbo3);
    glDeleteBuffers(1, &vbo4);
    glDetachShader(shader_program, vertex_shader);
    glDetachShader(shader_program, tess_control_shader);
    glDetachShader(shader_program, tess_eval_shader);
    glDetachShader(shader_program, fragment_shader);
    glDeleteShader(vertex_shader);
    glDeleteShader(tess_control_shader);
    glDeleteShader(tess_eval_shader);
    glDeleteShader(fragment_shader);
    glDeleteProgram(shader_program);

    glfwTerminate();
    return 0;
}