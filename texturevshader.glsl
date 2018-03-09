#version 150

in  vec2 vPosition;
in vec2 vTexPosition;
out vec2 texPosition;

void main() {
    // output the final vertex position in device coordinates
    // The final position of the vertex is assigned to the special `gl_Position` variable
    gl_Position = vec4(vPosition, 0, 1);
    // output any data the fragment shader requires. 
    texPosition = vTexPosition;
}
