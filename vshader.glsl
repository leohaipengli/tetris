#version 330 

in vec4 vPosition;
in vec4 vColor;
out vec4 fColor;


void main() {
    gl_Position = vPosition;

    // The color of each vertex will be interpolated
    // to produce the color of each fragment
    fColor = vColor;
}
