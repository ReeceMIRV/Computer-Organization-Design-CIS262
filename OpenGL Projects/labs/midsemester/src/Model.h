#ifndef MODEL
#define MODEL
#include "Shader.h"

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Model {
public:
    Model(Shader* shader);
    ~Model();
    void set_geometry(GLfloat vertices[], int size);
    void set_normals(GLfloat* normals, int size);
    void set_vertex_colors(GLfloat* colors, int size);
    void set_texture_coordinates(GLfloat* uvs, int size);
    void set_index_buffer(GLuint* indices, int size);                // UPDATE!
    
    void upload_2_server();
    void render();

    Shader*        shader;        // The shader object for this model
    glm::mat4    mM;            // A matrix holding all orientation information for this model.
                            // This will get passed off the mV variable in the shader
                            // This is the model part of the MVP matrix

private:
    GLfloat*    vertices;    // x, y, z
    GLfloat*    normals;    // Vertex normals
    GLfloat*    uvs;        // Texture coordinates
    GLfloat*    colors;        // Colors for each vertex
    GLuint* indices;    // Index buffer                            // UPDATE!
    GLuint        total_vertices, total_normals, total_UVs, total_colors, total_indices;;
    GLuint        vao;        // Vertex Array Object
    GLuint        vbo;        // Vertex Buffer Object
    GLuint        index_vbo;    // Index Buffer Object                    // UPDATE!

    GLuint        vPosition;    // A handle for the variable "vPosition" in the shader
    GLuint        vNormal;    // A handle for the variable "vNormal" in the shader
    GLuint        vTexCoord;    // A handle for the variable "vTexCoord" in the shader
    GLuint        vColor;        // A handle for the variable "vColor" in the shader
    GLuint        umM;        // A handle for the variable "mM" variable in the shader

};
#endif