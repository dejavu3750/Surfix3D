#type vertex
#version 330 core
layout(location = 0) in vec3 aPos;
uniform mat4 uMVP;
uniform float uSize;
void main() {
    gl_Position  = uMVP * vec4(aPos, 1.0);
    gl_PointSize = uSize;
}

#type fragment
#version 330 core
out vec4 o;
uniform vec3 uColor;
void main() {
    vec2 d = gl_PointCoord - vec2(0.5);
    if (dot(d, d) > 0.25) discard;     // round point sprite
    o = vec4(uColor, 1.0);
}