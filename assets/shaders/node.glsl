#type vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in uint aNode;
uniform mat4 uMVP;
uniform float uSize;
flat out uint vNode;
void main() {
    vNode = aNode;
    gl_Position  = uMVP * vec4(aPos, 1.0);
    gl_PointSize = uSize;
}

#type fragment
#version 330 core
flat in uint vNode;
out vec4 o;
uniform vec3 uColor;
uniform vec3 uSel;
uniform vec3 uHover;
uniform int  uSelOnly;
uniform usamplerBuffer uNodeState;
void main() {
    uint nf = texelFetch(uNodeState, int(vNode)).r;
    if ((nf & 1u) == 0u) discard;
    bool s  = ((nf & 2u) != 0u);
    bool hv = ((nf & 4u) != 0u);
    if (uSelOnly != 0 && !s && !hv) discard;
    o = vec4(s ? uSel : (hv ? uHover : uColor), 1.0);
}