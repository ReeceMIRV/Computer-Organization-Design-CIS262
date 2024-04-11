#include "FPSCamera.h"


FPSCamera::FPSCamera(GLfloat fov/*degrees*/, GLfloat aspect, GLfloat nearPlane, GLfloat farPlane)
{
    // The View matrix (the position and orientation of the camera)
    mV = glm::mat4( 1.0f ); // identity

    // The Perspective matrix captures fov, aspect, and the clipping planes
    mP = glm::perspective( glm::radians( fov ), aspect, nearPlane, farPlane );
}

void FPSCamera::set_uniform_view_perspective( GLuint shader_ID )
{
    // Find the location of the "mP" variable in the GPU vertex shader
    GLuint mpLoc = glGetUniformLocation(shader_ID, "mP");

    // Upload mP to the GPU shader
    glUniformMatrix4fv( mpLoc, 1, GL_FALSE, &mP[0][0] );

    // Upload mV to the GPU shader
    GLuint mvLoc = glGetUniformLocation(shader_ID, "mV");
    glUniformMatrix4fv( mvLoc, 1, GL_FALSE, &mV[0][0] );
}