#version 150

out vec4 fColor;
in vec4 color;        // from the vertex shader.  Notice that it's an "in" vec4 this time, and was an "out" in the vertex shader.

void main () {
    fColor = color;
}