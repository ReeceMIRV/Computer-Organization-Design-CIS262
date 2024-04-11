#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Shader.h"
#include "Model.h"
#include "FPSCamera.h"

// c262_lab05_solution.cpp
// shader path configuration:
#define    BCC_PROJECT_NAME "midsemester"
#define    BCC_ROOT_PATH "../../labs/"
#define    BCC_VERTEX_SHADER BCC_ROOT_PATH BCC_PROJECT_NAME "/Shaders/vertexShader_v.c"
#define    BCC_FRAGMENT_SHADER  BCC_ROOT_PATH BCC_PROJECT_NAME "/Shaders/fragmentShader_f.c"

Model* ship_model;
Model* ocean_surface_model;
Model* silhouette_model;

FPSCamera* camera;
GLfloat theta;

void animate()
{
    glutPostRedisplay();
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //----------------------------------------------
    // take care of the boat drawing
    //----------------------------------------------
    theta += .0005f; // glm uses radians for angles
    GLfloat sway_angle = 2/*degree*/*sin(theta);
    
    glm::mat4 mtx_trans = glm::translate(
        glm::mat4(1.0f),
        glm::vec3(-2.0f, 1.0f, -15.0f ) // move the boat left, up, and into into the page
    );

    glm::mat4 mtx_rot_z = glm::rotate(
        mtx_trans,
        glm::radians(sway_angle), //glm::degrees(theta),
        glm::vec3(0.0f, 0.0f, 1.0f)
    );

    // Load up the mM (Model) matrix for the model
    ship_model->mM = mtx_rot_z; //shader calculates projection * view * model

    // We'll only set the View and Perspective matrix when/if we change shaders
    camera->set_uniform_view_perspective(ship_model->shader->program_ID);
    ship_model->render();

	//----------------------------------------------
    // take care of the ocean surface
    //----------------------------------------------
    glm::mat4 mtx_trans_ocean = glm::translate(
        glm::mat4(1.0f),
        glm::vec3( 0.0f, 0.0f, -10.0f ) // move the ocean 10 units forward
    );

    glm::mat4 mtx_rot_ocean = glm::rotate(
        mtx_trans_ocean,
        glm::radians( -75.0f/*degrees*/ ), //glm::degrees( theta ),
        glm::vec3( 1.0f, 0.0f, 0.0f )
    );

    ocean_surface_model->mM = mtx_rot_ocean; // no changes for the ocean surface
    camera->set_uniform_view_perspective(ocean_surface_model->shader->program_ID);
    ocean_surface_model->render();

	//----------------------------------------------
    // take care of the boat silhouette
    //----------------------------------------------

	glm::mat4 mtx_scale_silhouette = glm::scale(
        glm::mat4(1.0f),
        glm::vec3(0.7f, 0.4f, 1.0f) // Scale the x,y,z to the specified float
    );

	glm::mat4 mtx_trans_silhouette = glm::translate(
        mtx_scale_silhouette,
        glm::vec3(-2.0f, 1.8f, -10.0f) // move the silhouette left, up, and into the page
    );

    glm::mat4 mtx_rot_silhouette_x = glm::rotate(
        mtx_trans_silhouette,
        glm::radians(-180.0f),      // Convert -180 degrees to radians and rotate by that number
        glm::vec3(1.0f, 0.0f, 0.0f) // The axis that I'm rotating about - currently about the x axis
    );

	glm::mat4 mtx_rot_silhouette_y = glm::rotate(
        mtx_rot_silhouette_x,
        glm::radians(-sway_angle),
        glm::vec3(0.0f, 0.0f, 1.0f)
    );

    silhouette_model->mM = mtx_rot_silhouette_y; // no changes for the ocean surface
    camera->set_uniform_view_perspective(silhouette_model->shader->program_ID);
    silhouette_model->render();

    glutSwapBuffers();
}

// Any time the window is resized, this function gets called. Set by the
// "glutReshapeFunc" in main.
void change_viewport(int w, int h){
    glViewport(0, 0, w, h);
}

int main (int argc, char** argv){
    // Initialize GLUT
    glutInit(&argc, argv);

    // Set up some memory buffers for our display
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);

    // Set the initial window size
    glutInitWindowSize(800, 600);

    // Create the window title
    glutCreateWindow("Ship Silhouette");

    // Bind the two functions (above) to respond when necessary
    glutReshapeFunc(change_viewport);
    glutDisplayFunc(render);
    glutIdleFunc( animate );
    
    // Remember, we always init GLEW after we create a window
    // This asks the GPU driver for all of those gl functions
    if (GLEW_OK != glewInit()) {
        exit(1);
    }

	// clear old background color and set a new background color
	glClearColor(0.65f, 0.79f, 0.89f, 1.0f); // rgba(166, 201, 226, 1)

    Shader* myShader = new Shader( BCC_VERTEX_SHADER, BCC_FRAGMENT_SHADER );
    
    ship_model = new Model(myShader); // ship
	silhouette_model = new Model(myShader); // ship silhouette
    ocean_surface_model = new Model(myShader); // ocean surface

	// Ship Vertices & Colors    
    GLfloat ship_vertices[] = {
		// ------------------ center most triangle
		-2.0f, 0.0f, 0.0f, // bottom left vertex
		2.0f, 0.0f, 0.0f,  // bottom right vertex
		0.0f, 1.5f, 0.0f,  // top center vertex

		-4.0f, 1.5f, 0.0f, // leftmost upsidedown triangle, top left vertex
		4.0f, 1.5f, 0.0f,  // rightmost upsidedown triangle, top right vertex

		// ------------------ top center two triangles
		-1.0f, 2.55f, 0.0f, // top center vertex of left triangle
		-2.0f, 1.5f, 0.0f, // bottom left vertex of left triangle

		1.0f, 2.75f, 0.0f,  // top center vertex of right triangle
		2.25f, 1.5f, 0.0f,  // bottom right vertex of right triangle
    };

	GLfloat ship_colors[] = {
		// ------------------ center most triangle
		0.20f, 0.20f, 0.20f, 1.0f, // darkest grey - bottom left vertex
		0.51f, 0.51f, 0.53f, 1.0f, // lighter grey - bottom right vertex
		0.30f, 0.30f, 0.30f, 1.0f, // primary grey - top center vertex

		0.20f, 0.20f, 0.20f, 1.0f, // darkest grey - leftmost upsidedown triangle, top left vertex
		0.20f, 0.20f, 0.20f, 1.0f, // darkest grey - rightmost upsidedown triangle, top right vertex

		// ------------------ top center two triangles
		0.30f, 0.30f, 0.30f, 1.0f, // primary grey - top center vertex of left triangle
		0.20f, 0.20f, 0.20f, 1.0f, // darkest grey - bottom left vertex of left triangle

		0.20f, 0.20f, 0.20f, 1.0f, // darkest grey - top center vertex of right triangle
		0.51f, 0.51f, 0.53f, 1.0f, // lighter grey - bottom right vertex of right triangle
    };

	GLuint ship_indices[] = {
		0, 1, 2, // center bottom triangle
		2, 3, 0, // leftmost upsidedown triangle
		2, 1, 4, // rightmost upsidedown triangle
		2, 8, 7, // top left triangle
		2, 5, 6, // top right triangle
    };

	// We won't be creating the vertices & indices for the silhouette
	// We will just use the same vertices & indices from the ship & set its geometry, and buffer
	// We will still create an array for the silhouette colors though, as a silhouette is dark

	GLfloat silhouette_colors[] = {
		// These relative positions are all PRIOR toward translating, scaling, and flipping the ship
		// -------------------------- center most triangle
		0.06f, 0.16f, 0.33f, 1.0f, // navy blue - bottom left vertex
		0.25f, 0.45f, 0.61f, 1.0f, // light blue - bottom right vertex
		0.06f, 0.16f, 0.33f, 1.0f, // navy blue - top center vertex

		0.06f, 0.16f, 0.33f, 1.0f, // navy blue - leftmost upsidedown triangle, top left vertex
		0.15f, 0.25f, 0.41f, 1.0f, // darker light blue - rightmost upsidedown triangle, top right vertex

		// -------------------------- top center two triangles
		0.06f, 0.16f, 0.33f, 1.0f, // navy blue - top center vertex of left triangle
		0.06f, 0.16f, 0.33f, 1.0f, // navy blue - bottom left vertex of left triangle

		0.06f, 0.16f, 0.33f, 1.0f, // navy blue - top center vertex of right triangle
		0.06f, 0.16f, 0.33f, 1.0f, // navy blue - bottom right vertex of right triangle
	};

	// Ocean Vertices & Colors
	GLfloat ocean_surface_vertices[] = {
		// ------------------- centermost triangle 
		-8.0f, -8.0f, 0.0f, // bottom left vertex
		8.0f, -8.0f, 0.0f,  // bottom right vertex
		0.0f, 4.0f, 0.0f,   // top center vertex

		-8.0f, 4.0f, 0.0f,  // top left vertex  - left triangle
		8.0f, 4.0f, 0.0f,   // top right vertex - right triangle
    };

	GLfloat ocean_surface_colors[] = {
		0.06f, 0.16f, 0.33f, 1.0f, // rgba(15, 41, 85, 1)   navy blue
		0.25f, 0.45f, 0.61f, 1.0f, // rgba(65, 115, 155, 1) light blue
		0.06f, 0.16f, 0.33f, 1.0f, // rgba(15, 41, 85, 1)   navy blue

		0.06f, 0.16f, 0.33f, 1.0f, // rgba(15, 41, 85, 1)   navy blue
		0.25f, 0.45f, 0.61f, 1.0f, // rgba(65, 115, 155, 1) light blue
		0.06f, 0.16f, 0.33f, 1.0f, // rgba(15, 41, 85, 1)   navy blue
    };

	GLuint ocean_indices[] = {
		0, 1, 2, // centermost triangle
		0, 2, 3, // left triangle
		1, 4, 2, // right triangle
	};

    // Call the model->set_geometry and pass it the vertices
    ship_model->set_geometry(ship_vertices, sizeof (ship_vertices));
	silhouette_model->set_geometry(ship_vertices, sizeof(ship_vertices));
	ocean_surface_model->set_geometry(ocean_surface_vertices, sizeof (ocean_surface_vertices));

    // Call the model->set_index_buffer and pass it the array of indices
    ship_model->set_index_buffer(ship_indices, sizeof(ship_indices));
	silhouette_model->set_index_buffer(ship_indices, sizeof(ship_indices));
	ocean_surface_model->set_index_buffer(ocean_indices, sizeof(ocean_indices));

	// Call model->set_vertex_colors and pass the array of colors
	ship_model->set_vertex_colors(ship_colors, sizeof (ship_colors));
	silhouette_model->set_vertex_colors(silhouette_colors, sizeof(silhouette_colors));
	ocean_surface_model->set_vertex_colors(ocean_surface_colors, sizeof(ocean_surface_colors));

	// Upload to Server
    ship_model->upload_2_server();
	silhouette_model->upload_2_server();
    ocean_surface_model->upload_2_server();

    // A camera with a 60 degree field of view with and aspect ratio of 1.0f,
    // near plane of 1.0f and far plane of 1000.0f
    camera = new FPSCamera(60.0f, 1.0f, 1.0f, 1000.0f);

    // Set up some matrices for the model
    theta = 0.0f;
    
    // Allocate memory for the matrices
    glutMainLoop();

    return 0;
}
