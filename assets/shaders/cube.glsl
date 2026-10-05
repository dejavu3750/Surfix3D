#type vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNrm;
uniform mat4 uMVP;
uniform mat3 uNrmMat;
out vec3 vNrm;
void main() {
    vNrm = uNrmMat * aNrm;
    gl_Position = uMVP * vec4(aPos, 1.0);
}

#type fragment
#version 330 core
in vec3 vNrm;
out vec4 o;
uniform vec3 uLightDir;
void main() {
    o = vec4(0.86, 0.87, 0.90, 1.0);   // flat, uniform gray
}