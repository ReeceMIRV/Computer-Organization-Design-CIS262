#version 150

in vec4 vPosition;
uniform mat4 mM;    // The matrix for the pose of the model
uniform mat4 mV;    // The matrix for the pose of the camera
uniform mat4 mP;    // The perspective matrix
in vec4 vColor;        // Coming in from your OpenGL program
out vec4 color;        // Going to (and defined in) your fragment shader.

void main () {
    gl_Position = mP*mV*mM*vPosition;
    color = vColor;
}