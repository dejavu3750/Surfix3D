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
void main() {
    o = vec4(0.55, 0.57, 0.62, 1.0);
}