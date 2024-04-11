#ifndef CAMERA
#define CAMERA

#include <GL/glew.h>
#include <GL/freeglut.h>

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::perspective
// glm::translate, glm::rotate, glm::scale
#include <glm/gtc/matrix_transform.hpp>

class FPSCamera {
public:
    FPSCamera(GLfloat fov, GLfloat aspect, GLfloat nearPlane, GLfloat farPlane);
    void set_uniform_view_perspective(GLuint shader_ID); // This function sets the View and Perspective in the shader

    glm::mat4 mP;    // the P_erspective matrix.  This is usually set once.
    glm::mat4 mV;    // the V_iew matrix.  This usually changes every frame.
};

#endif