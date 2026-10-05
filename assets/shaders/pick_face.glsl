#type vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 2) in uint aFace;
layout(location = 3) in uint aElem;
layout(location = 4) in uint aPI;
layout(location = 5) in uint aPG;
layout(location = 6) in uint aLE;
uniform mat4 uMVP;
flat out uint vFace;
flat out uint vElem;
flat out uint vPI;
flat out uint vPG;
flat out uint vLE;
void main() {
    vFace = aFace; vElem = aElem; vPI = aPI; vPG = aPG; vLE = aLE;
    gl_Position = uMVP * vec4(aPos, 1.0);
}

#type fragment
#version 330 core
flat in uint vFace;
flat in uint vElem;
flat in uint vPI;
flat in uint vPG;
flat in uint vLE;
layout(location = 0) out uvec4 oId;
uniform int uMode;
uniform samplerBuffer  uFaceState;
uniform usamplerBuffer uElemState;
void main() {
    uint ff = uint(texelFetch(uFaceState, int(vFace)).a + 0.5);
    uint ef = texelFetch(uElemState, int(vElem)).r;
    if ((ff & 1u) == 0u || (ef & 1u) == 0u) discard;
    oId = uvec4(((uMode == 0) ? vFace : vElem) + 1u, vPI, vPG, vLE);
}