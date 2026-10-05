#type vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in uint aNode;
layout(location = 2) in uint aGlobal;
uniform mat4 uMVP;
uniform float uSize;
flat out uint vNode;
flat out uint vG;
void main() {
    vNode = aNode; vG = aGlobal;
    gl_Position  = uMVP * vec4(aPos, 1.0);
    gl_PointSize = uSize;
}

#type fragment
#version 330 core
flat in uint vNode;
flat in uint vG;
layout(location = 0) out uvec4 oId;
uniform usamplerBuffer uNodeState;
void main() {
    uint nf = texelFetch(uNodeState, int(vNode)).r;
    if ((nf & 1u) == 0u) discard;
    oId = uvec4(vNode + 1u, vG, 0u, 0u);
}