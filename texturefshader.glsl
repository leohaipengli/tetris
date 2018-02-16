#version 330 core

// Interpolated values from the vertex shaders
in vec2 texPosition;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
// type sampler2D, which will have a default value of 0. This only needs to be changed when access has to be provided to multiple textures, which will not be considered in this case.
uniform sampler2D myTextureSampler;

void main(){

    // Output color = color of the texture at the specified UV
    color = texture( myTextureSampler, texPosition ).rgb;
}