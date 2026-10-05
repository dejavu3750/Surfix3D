#type vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aCol;
uniform mat4 uMVP;
out vec3 vCol;
void main() {
    vCol = aCol;
    gl_Position = uMVP * vec4(aPos, 1.0);
}

#type fragment
#version 330 core
in vec3 vCol;
out vec4 o;
void main() {
    o = vec4(vCol, 1.0);
}