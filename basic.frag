#version 330 core

in vec4 channelCol; // Input color from vertex shader
out vec4 outCol;    // Output final color

void main() {
    outCol = channelCol; // Set the fragment color
}