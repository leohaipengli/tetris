#version 150

in  vec4 color;
out vec4 fColor;

// // You'll immediately notice that we're not using some built-in variable for outputting the color, say gl_FragColor. This is because a fragment shader can in fact output multiple colors and we'll see how to handle this when actually loading these shaders.

void main() {
// // it has one mandatory output, the final color of a fragment
    fColor = color;
}
