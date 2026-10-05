#type vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aCol;
uniform mat4 uMVP;
uniform float uSize;
out vec3 vCol;
void main() {
    vCol = aCol;
    gl_Position  = uMVP * vec4(aPos, 1.0);
    gl_PointSize = uSize;
}

#type fragment
#version 330 core
in vec3 vCol;
out vec4 o;
void main() {
    vec2 d = gl_PointCoord - vec2(0.5);
    if (dot(d, d) > 0.25) discard;     // round tip disc
    o = vec4(vCol, 1.0);
}