#version 420 core

layout (location=0) in vec3 vertexPosition;
layout (location=1) in vec3 vertexNormal;
layout (location=2) in vec2 textureCoords;

uniform mat4 modelViewProjectionMatrix;
uniform int isDrawingSkybox;

// cubemap reflections
uniform mat4 model;
out vec3 fr_normal;
out vec3 fr_position;

out vec2 fr_textureCoords;
out vec3 fr_vertexPosition;

void main() {
    if(isDrawingSkybox == 0){
        gl_Position = modelViewProjectionMatrix * vec4(vertexPosition, 1.0);

        fr_normal = mat3(transpose(inverse(model))) * vertexNormal;
        // WCS pos, without view and perspective
        fr_position = vec3(model * vec4(vertexPosition, 1.0));
    }
    else if(isDrawingSkybox == 1){
        vec4 pos = modelViewProjectionMatrix * vec4(vertexPosition, 1.0);

        // z = w for depth to be always maxed
        gl_Position = pos.xyww; 
    }

    fr_textureCoords = textureCoords;
    fr_vertexPosition = vertexPosition;
}