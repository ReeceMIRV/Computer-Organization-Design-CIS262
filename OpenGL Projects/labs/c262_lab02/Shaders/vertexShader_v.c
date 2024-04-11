#version 150

in vec4 vPosition;

void main () {
    // Note: the value of s_vPosition should be between -1.0 and +1.0 (to be visible on the screen)
    gl_Position = vPosition;
}
