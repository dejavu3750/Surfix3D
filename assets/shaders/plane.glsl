#type vertex
#version 330 core
layout(location = 0) in vec3 aPos;
uniform mat4 uMVP;
void main() {
    gl_Position = uMVP * vec4(aPos, 1.0);
}

#type fragment
#version 330 core
out vec4 o;
uniform vec4 uColor;
void main() {
    o = uColor;
}