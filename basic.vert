#version 330 core

layout(location = 0) in vec2 inPos; // Vertex position
layout(location = 1) in vec4 inCol; // Vertex color

out vec4 channelCol; // Pass color to fragment shader

void main() {
    gl_Position = vec4(inPos, 0.0, 1.0); // Transform vertex position
    channelCol = inCol; // Pass color to fragment shader
}