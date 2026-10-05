#type vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in uint aId;
uniform mat4 uMVP;
uniform uint uHoverId;
flat out uint vKeep;
void main() {
    vKeep = (aId == uHoverId) ? 1u : 0u;              // draw only the hovered cell
    gl_Position = uMVP * vec4(aPos * 1.03, 1.0);      // slightly outset over the cube
}

#type fragment
#version 330 core
flat in uint vKeep;
uniform vec3 uColor;
out vec4 o;
void main() {
    if (vKeep == 0u) discard;
    o = vec4(uColor, 0.45);
}