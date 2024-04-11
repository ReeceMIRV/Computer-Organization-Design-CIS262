  
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Shader.h"
#include "Model.h"
#include "FPSCamera.h"

// c262_lab05.cpp
// shader path configuration:
#define    BCC_PROJECT_NAME "c262_lab05"
#define    BCC_ROOT_PATH "../../labs/"
#define    BCC_VERTEX_SHADER BCC_ROOT_PATH BCC_PROJECT_NAME "/Shaders/vertexShader_v.c"
#define    BCC_FRAGMENT_SHADER  BCC_ROOT_PATH BCC_PROJECT_NAME "/Shaders/fragmentShader_f.c"

Model* myModel; 
FPSCamera* camera;
GLfloat theta;

void animate()
{
    glutPostRedisplay();
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    theta += 0.001f; // glm uses radians for angles
    
    glm::mat4 mtx_trans = glm::translate(
        glm::mat4(1.0f),
        glm::vec3( 0.0f, 0.0f, -15.0f )
        );

    glm::mat4 mtx_rot_Y = glm::rotate(
        mtx_trans,
        theta, //glm::degrees( theta ),
        glm::vec3(0.0f, 1.0f, 0.0f)
        );

    // Load up the mM (Model) matrix for the model
    myModel->mM = mtx_rot_Y; //shader calculates projection * view * model

    // We'll only set the View and Perspective matrix when/if we change shaders
    camera->set_uniform_view_perspective(myModel->shader->program_ID);
    myModel->render();

    glutSwapBuffers();
}

// Any time the window is resized, this function gets called. Set by the
// "glutReshapeFunc" in main.
void change_viewport(int w, int h){
    glViewport(0, 0, w, h);
}

int main (int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);

    // Set up some memory buffers for our display
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
    // Set the window size
    glutInitWindowSize(800, 800);
    // Create the window with the title "Triangle"
    glutCreateWindow("Index Lab");
    // Bind the two functions (above) to respond when necessary
    glutReshapeFunc(change_viewport);
    glutDisplayFunc(render);
    glutIdleFunc( animate );
    
    // Remember, we always init GLEW after we create a window
    // This asks the GPU driver for all of those gl functions
    if (GLEW_OK != glewInit()) {
        exit(1);
    }
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    Shader* myShader = new Shader( BCC_VERTEX_SHADER, BCC_FRAGMENT_SHADER );
    
    myModel = new Model(myShader);

    // 1) TODO -- there are redundant vertices[] -- rewrite the array to eliminate them 
    //    Define another array called "vertices2" - it should only have 6 vertices!
    /*YOUR ANSWER*/
	GLfloat vertices[] = {
		// -------------------- bottom right triangle
		5.0f, -5.0f, 0.0f,    // bottom right vertex
		2.5f, 0.0f, 0.0f,     // top center vertex
		// 0.0f, -5.0f, 0.0f, // bottom left vertex - not needed

		// --------------------- top center triangle
		// 2.5f, 0.0f, 0.0f,  // bottom right vertex - not needed
		0.0f, 5.0f, 0.0f,     // top center vertex
		-2.5f, 0.0f, 0.0f,    // bottom left vertex

		// --------------------- bottom left triangle
		// -2.5f, 0.0f, 0.0f, // top center vertex - not needed
		-5.0f, -5.0f, 0.0f,   // bottom left vertex
		0.0f, -5.0f, 0.0f,    // bottom right vertex
    };
    
    // Note: you can keep these colors if you want...
    GLfloat colors[] = {
        1.0f, 0.0f, 0.0f, 1.0f, // red
        0.0f, 1.0f, 0.0f, 1.0f, // green
        0.0f, 0.0f, 1.0f, 1.0f, // blue
        1.0f, 0.0f, 0.0f, 1.0f, // red
        0.0f, 1.0f, 0.0f, 1.0f, // green
        0.0f, 0.0f, 1.0f, 1.0f, // blue
        //1.0f, 0.0f, 0.0f, 1.0f, // red
        //0.0f, 1.0f, 0.0f, 1.0f, // green
        //0.0f, 0.0f, 1.0f, 1.0f  // blue
    };

    /*YOUR ANSWER*/
    // 2) Create an array of indices[] of triangles. This should have 9 numbers (think about why)
    GLuint indices[] = {
        /*YOUR ANSWER*/
		0, 1, 5, // bottom right triangle
		1, 2, 3, // center top triangle
		3, 4, 5, // bottom left triangle
		
    };

    // 3) Update this so that you pass it vertices2 instead of vertices
	/*YOUR ANSWER*/
    myModel->set_geometry(vertices, sizeof(vertices));


    // 4) Call myModel->set_index_buffer() and pass it the array you made in step 2
    /*YOUR ANSWER*/
	myModel->set_index_buffer(indices, sizeof(indices));

    myModel->set_vertex_colors(colors, sizeof(colors));
    myModel->upload_2_server();

    // A camera with a 60 degree field of view with and aspect ratio of 1.0f,
    //    near plane of 1.0f and far plane of 1000.0f
    camera = new FPSCamera(60.0f, 1.0f, 1.0f, 1000.0f);

    // Set up some matrices for the model
    theta = 0.0f;
    
    // Allocate memory for the matrices
    glutMainLoop();

    return 0;
}
