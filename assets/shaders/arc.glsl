#type vertex
#version 330 core
layout(location = 0) in vec2 aPos;
uniform mat4 uMVP;
void main() {
    gl_Position = uMVP * vec4(aPos, 0.0, 1.0);
}

#type fragment
#version 330 core
out vec4 o;
uniform vec3 uColor;
void main() {
    o = vec4(uColor, 1.0);
}