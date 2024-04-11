#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include "Shader.h"

// c262_lab02.cpp
// shader path configuration:
#define    BCC_PROJECT_NAME "c262_lab02"
#define    BCC_ROOT_PATH "../../labs/"
#define    BCC_VERTEX_SHADER BCC_ROOT_PATH BCC_PROJECT_NAME "/Shaders/vertexShader_v.c"
#define    BCC_FRAGMENT_SHADER  BCC_ROOT_PATH BCC_PROJECT_NAME "/Shaders/fragmentShader_f.c"

GLuint triangle_vao;
//GLint vertexID;
//GLint fragID;
//GLuint shader;

// This function asks the driver for a vertex array object, which will store information
// about what is being drawn, such as geometry, color information, texture information and so on.
// Hold on to the ID that this function returns, because you can use it later on!
GLuint create_vertex_array() {
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    return vao;
}

// This function takes in a vertex array object ID and an array of vertices. It should be called 
// immediately after calling create_vertex_array.  It returns a buffer ID (which you probably won't
// use).  NOTE: this assumes that you want triangles and not points or lines.
GLuint upload_vertex_positions(GLuint shader_program_ID, GLuint vao, GLfloat* vVerts, GLuint total_vertices) {
    glBindVertexArray(vao);
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer (GL_ARRAY_BUFFER, buffer);
    
    // Copy the array data into the active OpenGL buffer
    glBufferData(GL_ARRAY_BUFFER, total_vertices*3*sizeof(GLfloat), vVerts, GL_STATIC_DRAW);
    
    // Figure out where vPosition is in our shader
    GLuint loc = glGetAttribLocation(shader_program_ID, "vPosition");
    // Turn it on
    glEnableVertexAttribArray(loc);
    // Tell it how the data should be parsed
    glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);

    return buffer;
}

//--------------------------------------------------
// added for Lab2
//--------------------------------------------------

// Any time the window is resized, this function gets called. Set by the
// "glutReshapeFunc" in main.
void change_viewport(int w, int h){
    glViewport(0, 0, w, h);
}

// Here is the function that gets called each time the window needs to be redrawn.
// It is the "paint" function for our program, and is set up from the glutDisplayFunc in main
void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glutSwapBuffers();
}

int main (int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);

    // Set up some memory buffers for our display
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
    // Set the window size
    glutInitWindowSize(800, 600);
    // Create the window with the title "Triangle"
    glutCreateWindow("Shaders");
    // Bind the two functions (above) to respond when necessary
    glutReshapeFunc(change_viewport);
    glutDisplayFunc(render);

    // Very important!  This initializes the entry points in the OpenGL driver so we can 
    // call all the functions in the API.
    
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW error");
        return 1;
    }

	Shader* shader = new Shader(BCC_VERTEX_SHADER, BCC_FRAGMENT_SHADER);

    // Add geometry to draw a red triangle on the screen
    /*YOUR ANSWER*/
    GLfloat vertices[] = {-0.5f, -0.5f, 0.0f, 
                        0.5f, -0.5f, 0.0f, 
                        0.0f, 0.5f, 0.0f};

    triangle_vao = create_vertex_array();
    upload_vertex_positions(shader->program_ID, triangle_vao, vertices, 3);


    glUseProgram(shader->program_ID);

    //glDeleteProgram(shader->program_ID);
    //int temp;
    //scanf ("%d", &temp);

    // Start up a loop that runs in the background (you never see it).
    glutMainLoop();

    return 0;
}
